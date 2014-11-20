package toolc
package analyzer

import utils._
import ast.Trees._
import Symbols._
import scala.annotation.tailrec

object NameAnalysis extends Pipeline[Program, Program] {

  private val classRedef = "a class is defined more than once"
  private val classDefMain = "a class uses the same name as the main object"
  private val varRedef = "a variable is defined more than once"
  private val memberOverload = "a class member is overloaded (forbidden in Tool)"
  private val methodOverload = "a method is overloaded (forbidden in Tool)"
  private val methodArgVarRedef = "a method argument is shadowed by a local variable declaration (forbidden in Java, and we follow this convention)"
  private val methodArgRedef = "two method arguments have the same name"
  private val classNotDef = "a class name is used as a symbol (as parent class or type, for instance) but is not declared"
  private val idNotDef = "an identifier is used as a variable but is not declared"
  private val classCycle = "the inheritance graph has a cycle (eg. “class A extends B {} class B extends A {}”)"
  private val unusedVar = "declared variable is never accessed (read or written)"

  def run(ctx: Context)(prog: Program): Program = {
    import ctx.reporter._

    def orderClasses(list: List[ClassDecl]): List[ClassDecl] = {

      @tailrec
      def orderClassesAcc(acc: List[ClassDecl], list: List[ClassDecl], last_update: Int): List[ClassDecl] = {

        def parentAlreadyThere(c: ClassDecl): Boolean = acc.contains(c)
        def getParent(id: Identifier): ClassDecl = {
          val filt = acc.union(list).filter(_.id == id)
          if (filt.isEmpty)
            ctx.reporter.fatal(classNotDef, id)
          else filt.head
        }

        if (list.isEmpty) acc
        else if (last_update == list.size) ctx.reporter.fatal(classCycle, list.head)
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

    val global = new GlobalScope

    // return true if already defined
    def getCreateClass(name: String): ClassSymbol = {
      val g = global.lookupClass(name)

      if (g.isDefined) {
        g.get
      } else {
        val s = new ClassSymbol(name)
        global.classes += (name -> s)
        s
      }
    }

    def parseProgram(prog: Program) = {
      parseMainObject(prog.main)

      for (c <- orderClasses(prog.classes)) parseClass(c)
    }

    def parseMainObject(main: MainObject) = {
      val s = new ClassSymbol(main.id.value)
      global.mainClass = s
      main.setSymbol(s)

      parseIdentifier(s, main.id)
      parseStats(s, main.stats)
    }

    def parseClass(c: ClassDecl) = {

      val name = c.id.value

      if (global.lookupClass(global.mainClass.name).isDefined)
        ctx.reporter.error(classDefMain, c)

      val w = global lookupClass (name)
      if (w.isDefined && w.get.totaly_defined)
        ctx.reporter.error(classRedef, c)

      val meth_name = c.methods.map(_.id)
      if (meth_name.toSet.size != meth_name.size)
        ctx.reporter.error(methodOverload, c)

      // create new class if needed
      val cs = getCreateClass(name)
      cs.totaly_defined = true
      c.setSymbol(cs)
      parseIdentifier(cs, c.id)

      // check parent
      if (c.parent.isDefined) {

        val ident = c.parent.get.value
        val ps = getCreateClass(ident)

        if (c.vars.exists(x => ps.lookupVar(x.id.value).isDefined))
          ctx.reporter.error(memberOverload, c)

        c.parent.get.setSymbol(ps)
        cs.parent = Some(ps)
      }

      // check members and vars
      for (v <- c.vars) cs.members += (v.id.value -> parseVar(cs, v))
      for (m <- c.methods) cs.methods += (m.id.value -> parseMethods(cs, m))
    }

    def parseMethods(scope: ClassSymbol, m: MethodDecl): MethodSymbol = {

      val name = m.id.value

      val args_list = m.args.map(_.id.value)
      val vars_list = m.vars.map(_.id.value)
      val args_name = args_list.toSet
      val vars_name = vars_list.toSet

      if (!args_name.intersect(vars_name).isEmpty)
        ctx.reporter.error(methodArgVarRedef, m)

      if (args_list.size != args_name.size)
        ctx.reporter.error(methodArgRedef, m)

      if (vars_list.size != vars_name.size)
        ctx.reporter.error(methodArgRedef, m)

      val s = new MethodSymbol(m.id.value, scope)

      val old = scope.lookupMethod(name)
      if (old.isDefined) {
        if (old.get.params.size == m.args.size)
          old.get.overridden = Some(s)
        else
          ctx.reporter.error(methodOverload, s)

      }

      m.setSymbol(s)
      parseIdentifier(s, m.id)

      val l = m.args.foldLeft(List[VariableSymbol]())((a, b) => parseFormal(s, b) :: a)
      s.argList = l.reverse

      for (v <- m.vars) s.members += (v.id.value -> parseVar(s, v))

      parseStats(s, m.stats)
      parseExpr(s, m.retExpr)

      parseType(m.retType)

      if (!s.members.filter(!_._2.used).isEmpty)
        ctx.reporter.warning(unusedVar, m)

      s
    }

    def parseType(t: TypeTree) = {
      t match {
        case x: Identifier => {
          val name = x.value
          val i = getCreateClass(name)
          x.setSymbol(i)
        }

        case _ => {}
      }
    }

    def parseBlock(scope: MethodSymbol, b: Block) =
      for (t <- b.stats) parseStat(scope, t)

    def parseStats(scope: Symbol, l: List[StatTree]) =
      for (t <- l) parseStat(scope, t)

    def parseStat(scope: Symbol, t: StatTree): Unit = {
      t match {
        case x: Block =>
          parseStats(scope, x.stats)

        case x: Println =>
          parseExpr(scope, x.expr)

        case x: Assign => {
          parseExpr(scope, x.id)
          parseExpr(scope, x.expr)
        }

        case x: While => {
          parseExpr(scope, x.expr)
          parseStat(scope, x.stat)
        }

        case x: ArrayAssign => {
          parseExpr(scope, x.expr)
          parseIdentifier(scope, x.id)
          parseExpr(scope, x.index)
        }

        case x: If => {
          parseExpr(scope, x.expr)
          if (x.els.isDefined) parseStat(scope, x.els.get)
          parseStat(scope, x.thn)
        }
      }
    }

    def parseExprs(scope: Symbol, l: List[ExprTree]) =
      for (e <- l) parseExpr(scope, e)

    def parseExpr(scope: Symbol, e: ExprTree): Unit = e match {

      case x: New => {
        val name = x.tpe.value

        val i = global.lookupClass(name)
        val cs =
          if (i.isDefined) i.get
          else {
            val s = new ClassSymbol(name)
            global.classes += (name -> s)
            s.totaly_defined = false
            s
          }

        parseIdentifier(cs, x.tpe)
      }

      case x: MethodCall => {
        parseExpr(scope, x.obj)
        parseExprs(scope, x.args)
      }

      case x: Identifier => {

        val name = x.value

        val vs = scope match {
          case w: MethodSymbol => {
            val o = w.lookupVar(name)
            if (o.isDefined) {
              o.get.used = true
              o.get
            } else ctx.reporter.fatal(idNotDef, x)

          }

          case _ => ??? // may not be needed
        }

        parseIdentifier(vs, x)
      }

      case x: And => {
        parseExpr(scope, x.lhs)
        parseExpr(scope, x.rhs)
      }

      case x: ArrayRead => {
        parseExpr(scope, x.arr)
        parseExpr(scope, x.index)
      }

      case x: ArrayLength =>
        parseExpr(scope, x.arr)

      case x: Div => {
        parseExpr(scope, x.lhs)
        parseExpr(scope, x.rhs)
      }

      case x: Equals => {
        parseExpr(scope, x.lhs)
        parseExpr(scope, x.rhs)
      }

      case x: LessThan => {
        parseExpr(scope, x.lhs)
        parseExpr(scope, x.rhs)
      }

      case x: Minus => {
        parseExpr(scope, x.lhs)
        parseExpr(scope, x.rhs)
      }

      case x: Or => {
        parseExpr(scope, x.lhs)
        parseExpr(scope, x.rhs)
      }

      case x: Plus => {
        parseExpr(scope, x.lhs)
        parseExpr(scope, x.rhs)
      }

      case x: Times => {
        parseExpr(scope, x.lhs)
        parseExpr(scope, x.rhs)
      }

      case x: NewIntArray => {
        parseExpr(scope, x.size)
      }

      case x: Not => {
        parseExpr(scope, x.expr)
      }

      case x: This => {

        scope match {
          case w: MethodSymbol => x.setSymbol(w.classSymbol)
          case _ => ??? // should not happen
        }
      }

      case _ => {}
    }

    def parseVar(scope: Symbol, v: VarDecl): VariableSymbol = {
      val name = v.id.value

      lazy val s = new VariableSymbol(name)

      scope match {
        case x: ClassSymbol => {

          if (x.members.contains(name))
            ctx.reporter.error(varRedef, v)

          val i = x.lookupVar(name)
          if (i.isDefined) x.members += (name -> i.get)
          else x.members += (name -> s)
        }

        case x: MethodSymbol => {
          val i = x.lookupVar(name)
          if (i.isDefined) x.members += (name -> i.get)
          else x.members += (name -> s)
        }

        case x: VariableSymbol => ??? // may never happen
      }

      v.setSymbol(s)
      parseIdentifier(s, v.id)
      parseType(v.tpe)

      s
    }

    def parseFormal(scope: MethodSymbol, f: Formal): VariableSymbol = {
      val s = new VariableSymbol(f.id.value)

      f.setSymbol(s)
      parseIdentifier(s, f.id)

      s
    }

    def parseIdentifier(s: Symbol, i: Identifier) =
      i.setSymbol(s)

    // This is a suggestion:
    // Step 1: Collect symbols in declarations
    // Step 2: Attach symbols to identifiers (except method calls) in method bodies

    // Make sure you check for all constraints

    parseProgram(prog)
    prog
  }
}
