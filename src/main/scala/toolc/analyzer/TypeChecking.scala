package toolc
package analyzer

import ast.Trees._

import Symbols._
import Types._
import utils._

object TypeChecking extends Pipeline[Program, Program] {
  /**
   * Typechecking does not produce a value, but has the side effect of
   * attaching types to trees and potentially outputting error messages.
   */
  def run(ctx: Context)(prog: Program): Program = {
    import ctx.reporter._

    def expecte(expected: Seq[Type], expr: ExprTree, tpe: Type): Type = {

      // Check result and return a valid type in case of error
      if (expected.isEmpty) {
        tpe
      } else {
        if (!expected.exists(e => tpe.isSubTypeOf(e)) || tpe == TError) {
          error("Type error: Expected: " + expected.toList.mkString(" or ") + ", found: " + tpe + " >> " + expr, expr)
          expected.head
        } else {
          tpe
        }
      }
    }

    def tcExpr(expr: ExprTree, expected: Type*): Type = {

      def trivialRecurse(lhs: ExprTree, rhs: ExprTree, expected: Type): Type = {
        tcExpr(lhs, expected)
        tcExpr(rhs, expected)
      }

      val tpe: Type = expr match {

        case And(lhs: ExprTree, rhs: ExprTree) => trivialRecurse(lhs, rhs, TBoolean)

        case Or(lhs: ExprTree, rhs: ExprTree) => trivialRecurse(lhs, rhs, TBoolean)

        case Plus(lhs: ExprTree, rhs: ExprTree) =>
          (tcExpr(lhs), tcExpr(rhs)) match {
            case (TInt, TInt) => TInt
            case (TString, TInt) => TString
            case (TInt, TString) => TString
            case (TString, TString) => TString
            case _ => TError
          }

        case w: Minus => trivialRecurse(w.lhs, w.rhs, TInt)

        case Times(lhs: ExprTree, rhs: ExprTree) => trivialRecurse(lhs, rhs, TInt)

        case Div(lhs: ExprTree, rhs: ExprTree) => trivialRecurse(lhs, rhs, TInt)

        case LessThan(lhs: ExprTree, rhs: ExprTree) => {
          tcExpr(lhs, TInt)
          tcExpr(rhs, TInt)
          TBoolean
        }

        case Equals(lhs: ExprTree, rhs: ExprTree) =>
          (tcExpr(lhs), tcExpr(rhs)) match {
            case (TInt, TInt) => TBoolean
            case (TString, TString) => TBoolean
            case (TIntArray, TIntArray) => TBoolean
            case (TBoolean, TBoolean) => TBoolean
            case (TObject(_), TObject(_)) => TBoolean
            case _ => TError
          }

        case ArrayRead(arr: ExprTree, index: ExprTree) => {
          tcExpr(arr, TIntArray)
          tcExpr(index, TInt)
        }

        case ArrayLength(arr: ExprTree) => {
          val t = tcExpr(arr, TIntArray)
          arr.setType(t)
          TInt
        }

        case x: MethodCall => {
          tcExpr(x.obj, TAnyObject) match {
            case TObject(cs) => {
              val s = cs.lookupMethod(x.meth.value).get
              x.meth.setSymbol(s)

              if (s.argList.size != x.args.size)
                ctx.reporter.error("Arguments not of the same size", x)

              for ((v, e) <- s.argList.zip(x.args)) {
                e.setType(tcExpr(e, v.getType))
              }

              s.getType
            }

            case _ => TError
          }
        }

        case IntLit(value: Int) => TInt

        case StringLit(value: String) => TString

        case True() => TBoolean

        case False() => TBoolean

        case x: Identifier => x.getSymbol.getType

        case x: This => x.getSymbol.getType

        case NewIntArray(size: ExprTree) => {
          tcExpr(size, TInt)
          TIntArray
        }

        case New(tpe: Identifier) => tpe.getSymbol match {
          case x: ClassSymbol => TObject(x)
          case _ => TError
        }

        case Not(expr: ExprTree) => tcExpr(expr, TBoolean)

      }

      expr.setType(tpe)
      expecte(expected, expr, tpe)
    }

    def tcStat(stat: StatTree): Unit = stat match {

      case Block(stats: List[StatTree]) =>
        for (s <- stats)
          tcStat(s)

      case If(expr: ExprTree, thn: StatTree, els: Option[StatTree]) => {
        tcExpr(expr, TBoolean)
        tcStat(thn)
        if (els.isDefined) tcStat(els.get)
      }

      case While(expr: ExprTree, stat: StatTree) => {
        tcExpr(expr, TBoolean)
        tcStat(stat)
      }

      case Println(expr: ExprTree) => tcExpr(expr, TInt, TBoolean, TString)

      case Assign(id: Identifier, expr: ExprTree) => tcExpr(expr, id.getType)

      case ArrayAssign(id: Identifier, index: ExprTree, expr: ExprTree) => {
        tcExpr(id, TIntArray)
        tcExpr(index, TInt)
        tcExpr(expr, TInt)
      }

    }

    // set type symbol
    val map: Map[TypeTree, Type] = Map(
      IntType() -> TInt,
      BooleanType() -> TBoolean,
      IntArrayType() -> TIntArray,
      StringType() -> TString)

    def tcProgram(p: Program) = {
      val ordered = NameAnalysis.orderClasses(ctx, p.classes)
      for (c <- ordered) tcClassFirstPass(c)
      for (c <- ordered) tcClassSecondPass(c)
      tcMainObject(p.main)
    }

    def tcClassFirstPass(c: ClassDecl) = {
      c.getSymbol.setType(TObject(c.getSymbol))
      for (m <- c.methods) tcMethodFirstPass(m)
    }

    def tcClassSecondPass(c: ClassDecl) = {
      for (v <- c.vars) tcVar(v)
      for (m <- c.methods) tcMethodSecondPass(m)
    }

    def tcMethodFirstPass(m: MethodDecl) = {
      val t = tcType(m.retType)
      val s = m.getSymbol

      s.classSymbol.methods += (m.id.value -> s)

      s.setType(t)
      m.id.setSymbol(s)
      m.id.setType(t)

      for (a <- m.args) tcFormal(a)
    }

    def tcMethodSecondPass(m: MethodDecl) = {
      for (v <- m.vars) tcVar(v)
      for (f <- m.args) tcFormal(f)
      tcExpr(m.retExpr, tcType(m.retType))
      for (s <- m.stats) tcStat(s)
    }

    def tcVar(v: VarDecl) = {
      val t = tcType(v.tpe)

      v.id.setType(t)
      v.getSymbol.setType(t)
    }

    def tcFormal(f: Formal) = {
      val t = tcType(f.tpe)

      f.id.setType(t)
      f.getSymbol.setType(t)
    }

    def tcType(t: TypeTree) = {
      val tpe = t match {
        case _: IntArrayType => TIntArray
        case _: IntType => TInt
        case _: BooleanType => TBoolean
        case _: StringType => TString
        case x: Identifier => {
          if (!x.isDefined) println(x)
          x.getSymbol match {
            case y: ClassSymbol => {
              val o = TObject(y)
              t.setType(o)
              o
            }
          }
        }
      }

      t.setType(tpe)

      tpe
    }

    def tcMainObject(m: MainObject) = {
      m.getSymbol.setType(TAnyObject)
      for (s <- m.stats)
        tcStat(s)
    }

    tcProgram(prog)
    prog
  }
}
