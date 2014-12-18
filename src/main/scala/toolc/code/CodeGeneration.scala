package toolc
package code

import ast.Trees._
import analyzer.Symbols._
import analyzer.Types._
import cafebabe._
import AbstractByteCodes.{ New => _, _ }
import ByteCodes._
import utils._
import scala.annotation.tailrec

object CodeGeneration extends Pipeline[Program, Unit] {

  def run(ctx: Context)(prog: Program): Unit = {
    import ctx.reporter._

    def orderClasses(list: List[ClassDecl]): List[ClassDecl] = {

      @tailrec
      def orderClassesAcc(acc: List[ClassDecl], list: List[ClassDecl], last_update: Int): List[ClassDecl] = {

        def parentAlreadyThere(c: ClassDecl): Boolean = acc.contains(c)
        def getParent(id: Identifier): ClassDecl =
          acc.union(list).filter(_.id == id).head

        if (list.isEmpty) acc
        else {
          val c = list.head
          if (c.parent.isDefined && !parentAlreadyThere(getParent(c.parent.get)))
            orderClassesAcc(acc, list.tail :+ c, last_update + 1)
          else
            orderClassesAcc(c :: acc, list.tail, 0)
        }
      }

      orderClassesAcc(List(), list, 0).reverse
    }

    /** Writes the proper .class file in a given directory. An empty string for dir is equivalent to "./". */
    def generateClassFile(sourceName: String, ct: ClassDecl, dir: String): Unit = {
      val classFile = new ClassFile(ct.id.value, ct.parent.map(_.value))
      classFile.setSourceFile(sourceName)
      classFile.addDefaultConstructor

      for (v <- ct.vars) {
        val tpe = typeToString(v.tpe.getType)
        val name = v.id.value
        classFile.addField(tpe, name)
        v.getSymbol.clss = Some(ct.getSymbol)
      }

      for (m <- ct.methods) {
        val tpe = typeToString(m.retType.getType)
        val name = m.id.value
        val signature = methToStringArgs(m.getSymbol)

        val mh = classFile.addMethod(tpe, name, signature)

        generateMethodCode(mh.codeHandler, ct.getSymbol, m)
      }

      classFile.writeToFile(dir + ct.id.value + ".class")
    }

    def formalsToString(formals: List[Formal]): String = {
      formals.map(x => typeToString(x.id.getType)).mkString("")
    }

    def typeToString(t: Type): String = t match {
      case TInt => "I"
      case TBoolean => "Z"
      case TIntArray => "[I"
      case TString => "Ljava/lang/String;"
      case TObject(x) => "L" + x.name + ";"
      case _ => ???
    }

    def methToStringArgs(m: MethodSymbol): String =
      m.argList.map(a => typeToString(a.getType)).mkString("")

    def methToString(m: MethodSymbol): String =
      "(" + methToStringArgs(m) + ")" + typeToString(m.getType)

    def generateMethodCode(ch: CodeHandler, c: ClassSymbol, mt: MethodDecl): Unit = {
      val methSym = mt.getSymbol

      for ((v, i) <- mt.args.zipWithIndex) {
        v.getSymbol.is_arg = true
        v.getSymbol.bc_id = i + 1
      }
      for (v <- mt.vars) v.getSymbol.bc_id = ch.getFreshVar
      for (s <- mt.stats) generateStatCode(ch, c, s)

      ch << LineNumber(mt.retExpr.line)
      generateExprCode(ch, c, mt.retExpr)

      mt.retType.getType match {
        case TInt => ch << IRETURN
        case TIntArray => ch << ARETURN
        case TBoolean => ch << IRETURN
        case TString => ch << ARETURN
        case TObject(_) => ch << ARETURN
        case _ => ???
      }

      ch.freeze
    }

    def generateStatCode(ch: CodeHandler, c: ClassSymbol, stat: StatTree): Unit = {
      ch << LineNumber(stat.line)
      stat match {

        case Block(stats) =>
          for (s <- stats) generateStatCode(ch, c, s)

        case If(expr, thn, els) => {

          val end = ch.getFreshLabel("if end")

          generateExprCode(ch, c, expr)

          if (els.isEmpty) {
            ch << IfEq(end)

            generateStatCode(ch, c, thn)
          } else {
            val esl = ch.getFreshLabel("if else")

            ch << IfEq(esl)

            generateStatCode(ch, c, thn)
            ch << Goto(end)

            ch << Label(esl)
            generateStatCode(ch, c, els.get)
          }

          ch << Label(end)
        }

        case While(expr, stat) => {
          val begin = ch.getFreshLabel("while begin")
          val end = ch.getFreshLabel("while end")

          ch << Label(begin)

          generateExprCode(ch, c, expr)

          ch << IfEq(end)
          generateStatCode(ch, c, stat)
          ch << Goto(begin)

          ch << Label(end)
        }

        case Println(expr) => {
          ch << GetStatic("java/lang/System", "out", "Ljava/io/PrintStream;")
          generateExprCode(ch, c, expr)
          val sign = "(" + typeToString(expr.getType) + ")V"
          ch << InvokeVirtual("java/io/PrintStream", "println", sign)
        }

        case Assign(id, expr) => {
          val s = id.getSymbol.asInstanceOf[VariableSymbol]
          if (s.is_field) {
            ch << ALoad(0)
            generateExprCode(ch, c, expr)
            ch << PutField(c.name, id.value, typeToString(id.getType))
          } else {
            generateExprCode(ch, c, expr)
            s.getType match {
              case TInt => ch << IStore(s.bc_id)
              case TBoolean => ch << IStore(s.bc_id)
              case TIntArray => ch << AStore(s.bc_id)
              case TString => ch << AStore(s.bc_id)
              case TObject(_) => ch << AStore(s.bc_id)
              case _ => ???
            }
          }
        }

        case ArrayAssign(id, index, expr) => {
          val s = id.getSymbol.asInstanceOf[VariableSymbol]
          if (s.is_field) {
            //            ch << ALoad(0)
            //            ch << GetField(s.parent_class.get.name, id.value, typeToString(id.getType))

            generateExprCode(ch, c, id)
            generateExprCode(ch, c, index)
            generateExprCode(ch, c, expr)
            ch << IASTORE
            //            ch << PutField(s.parent_class.get.name, id.value, typeToString(id.getType))
          } else {
            generateExprCode(ch, c, id)
            generateExprCode(ch, c, index)
            generateExprCode(ch, c, expr)

            ch << IASTORE
          }
        }

      }
    }

    def generateExprCode(ch: CodeHandler, c: ClassSymbol, expr: ExprTree): Unit = {

      def trivialCode(lhs: ExprTree, rhs: ExprTree, bc: ByteCode) = {
        generateExprCode(ch, c, lhs)
        generateExprCode(ch, c, rhs)

        ch << bc
      }

      def trivialBool(lhs: ExprTree, rhs: ExprTree, bcf: (String) => AbstractByteCode) = {
        val end = ch.getFreshLabel("end")

        ch << ICONST_1

        generateExprCode(ch, c, lhs)
        generateExprCode(ch, c, rhs)

        ch << bcf(end)

        ch << POP
        ch << ICONST_0

        ch << Label(end)
      }

      expr match {

        case And(lhs, rhs) => { // TODO merge and / or
          val end = ch.getFreshLabel("and end")

          ch << ICONST_0

          generateExprCode(ch, c, lhs)

          ch << IfEq(end)
          ch << POP

          generateExprCode(ch, c, rhs)

          ch << Label(end)
        }

        case Or(lhs, rhs) => {
          val end = ch.getFreshLabel("or end")

          ch << ICONST_1

          generateExprCode(ch, c, lhs)

          ch << IfNe(end)
          ch << POP

          generateExprCode(ch, c, rhs)

          ch << Label(end)
        }

        case Plus(lhs, rhs) => {
          (lhs.getType, rhs.getType) match {
            case (TInt, TInt) => trivialCode(lhs, rhs, IADD)
            case _ => {
              ch << DefaultNew("java/lang/StringBuilder")

              generateExprCode(ch, c, lhs)
              val sign1 = "(" + typeToString(lhs.getType) + ")" + "Ljava/lang/StringBuilder;"
              ch << InvokeVirtual("java/lang/StringBuilder", "append", sign1)

              generateExprCode(ch, c, rhs)
              val sign2 = "(" + typeToString(rhs.getType) + ")" + "Ljava/lang/StringBuilder;"
              ch << InvokeVirtual("java/lang/StringBuilder", "append", sign2)

              ch << InvokeVirtual("java/lang/StringBuilder", "toString", "()Ljava/lang/String;")
            }
          }
        }

        case Minus(lhs, rhs) => trivialCode(lhs, rhs, ISUB)
        case Times(lhs, rhs) => trivialCode(lhs, rhs, IMUL)
        case Div(lhs, rhs) => trivialCode(lhs, rhs, IDIV)

        case LessThan(lhs, rhs) => trivialBool(lhs, rhs, label => If_ICmpLt(label))
        case Equals(lhs, rhs) => {
          val f = lhs.getType match {
            case TInt => label => If_ICmpEq(label)
            case TBoolean => label => If_ICmpEq(label)
            case TIntArray => label => If_ACmpEq(label)
            case TString => label => If_ACmpEq(label)
            case TObject(_) => label => If_ACmpEq(label)
          }
          trivialBool(lhs, rhs, f)
        }

        case ArrayRead(arr, index) => {
          generateExprCode(ch, c, arr)
          generateExprCode(ch, c, index)
          ch << IALOAD
        }
        case ArrayLength(arr) =>
          generateExprCode(ch, c, arr)
          ch << ARRAYLENGTH

        case MethodCall(obj, meth, args) => {
          generateExprCode(ch, c, obj)
          for (a <- args) generateExprCode(ch, c, a)
          val sym = meth.getSymbol.asInstanceOf[MethodSymbol]
          ch << InvokeVirtual(sym.classSymbol.name, meth.value, methToString(sym))
        }

        case IntLit(value) => ch << Ldc(value)
        case StringLit(value) => ch << Ldc(value)

        case True() => ch << ICONST_1
        case False() => ch << ICONST_0

        case x: Identifier => {
          val sym = x.getSymbol.asInstanceOf[VariableSymbol]
          if (sym.is_field) {
            ch << ALoad(0)
            ch << GetField(c.name, x.value, typeToString(x.getType))
          } else if (sym.is_arg) {
            ch << ArgLoad(sym.bc_id)
          } else
            sym.getType match {
              case TInt => ch << ILoad(sym.bc_id)
              case TBoolean => ch << ILoad(sym.bc_id)
              case TIntArray => ch << ALoad(sym.bc_id)
              case TString => ch << ALoad(sym.bc_id)
              case TObject(_) => ch << ALoad(sym.bc_id)
              case _ => ???
            }
        }

        case x: This =>
          ch << ALoad(0)

        case NewIntArray(size) =>
          generateExprCode(ch, c, size)
          ch << NewArray(10) // 10 == T_INT

        case New(id) =>
          ch << DefaultNew(id.value)

        case Not(expr) => {
          val okay = ch.getFreshLabel("not")

          generateExprCode(ch, c, expr)

          ch << ICONST_1
          ch << SWAP
          ch << IfEq(okay)
          ch << POP
          ch << ICONST_0
          ch << Label(okay)
          //          ch << ICONST_1
          //          generateExprCode(ch, expr)
          //          ch << ISUB
        }

      }
    }

    def generateMainMethodCode(ch: CodeHandler, stmts: List[StatTree], c: ClassSymbol): Unit = {
      for (s <- stmts) generateStatCode(ch, c, s);
      ch << RETURN
      ch.freeze
    }

    val outDir = ctx.outDir.map(_.getPath + "/").getOrElse("./")

    val f = new java.io.File(outDir)
    if (!f.exists()) {
      f.mkdir()
    }

    val sourceName = ctx.file.getName

    orderClasses(prog.classes) foreach {
      ct => generateClassFile(sourceName, ct, outDir)
    }

    val classFile = new ClassFile(prog.main.id.value, None)
    classFile.setSourceFile(sourceName)
    classFile.addDefaultConstructor

    val ch = classFile.addMainMethod.codeHandler
    generateMainMethodCode(ch, prog.main.stats, prog.main.getSymbol)
    classFile.writeToFile(outDir + prog.main.id.value + ".class")
  }

}
