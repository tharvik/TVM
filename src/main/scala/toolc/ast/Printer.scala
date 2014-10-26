package toolc
package ast

import Trees._

object Printer {

  val eol = "\n"
  val tab = "  "

  def just(level: Int): String =
    ((1).to(level)).foldLeft(""){(l,r) => l + tab}

  def unroll(level: Int, list: List[Tree], sep: String): String =
    if(list.isEmpty) ""
    else {
      val tabs = just(level)
      val start = tabs + Printer(level, list.head)
      list.tail.map(Printer(level,_)).foldLeft(start){_ + sep + tabs + _}
    }

  def apply(t: Tree): String =
    Printer(0, t)

  def expr(repr: String, lhs: ExprTree, rhs: ExprTree): String =
    "(" + Printer(lhs) + ") " + repr + " (" + Printer(rhs) + ")"


  def apply(l: Int, t: Tree): String = {

    t match {
      case Program(main: MainObject, classes: List[ClassDecl]) =>
        Printer(main) + eol +
        eol +
        unroll(l, classes, eol)
      case MainObject(id: Identifier, stats: List[StatTree]) =>
        "object " + Printer(id) + " {" + eol +
        just(l + 1) + "def main() : Unit = {" + eol +
        unroll(l + 2, stats, eol) + eol +
        just(l + 1) + "}" + eol +
        "}"
      case ClassDecl(id: Identifier, parent: Option[Identifier], vars: List[VarDecl], methods: List[MethodDecl]) => {
        val ext =
          if(!parent.isEmpty) " extends " + Printer(parent.get) else ""

        "class " + Printer(id) + ext + " {" + eol +
        eol +
        unroll(l + 1, vars, eol) + eol +
        eol +
        unroll(l + 1, methods, eol) + eol +
        just(l) + "}"
      }

      case VarDecl(tpe: TypeTree, id: Identifier) =>
        "var " + Printer(id) + " : " + Printer(tpe) + ";"
      case MethodDecl(retType: TypeTree, id: Identifier, args: List[Formal], vars: List[VarDecl], stats: List[StatTree], retExpr: ExprTree) => {
        val strVar =
          if(vars.isEmpty) ""
          else eol + unroll(l + 1, vars, eol) + eol
        val strStats =
          if(stats.isEmpty) ""
          else eol + unroll(l + 1, stats, eol) + eol

        "def " + Printer(id) + "(" + unroll(0, args, ", ") + ") : " +
        Printer(retType) + " = {" + eol +
        strVar +
        strStats + eol +
        just(l + 1) + "return " + Printer(retExpr) + ";" + eol +
        just(l) + "}" + eol
      }
      case Formal(tpe: TypeTree, id: Identifier) =>
        Printer(id) + " : " + Printer(tpe)

      case IntArrayType() => "Int[]"
      case IntType() => "Bool"
      case BooleanType() => "Int"
      case StringType() => "String"

      case Block(stats: List[StatTree]) =>
        "{" + eol +
        unroll(l + 1, stats, eol) + eol +
        just(l) + "}"

      case If(expr: ExprTree, thn: StatTree, els: Option[StatTree]) => {
        val next = if(els.isEmpty) "" else "else " + Printer(l,els.get)
        "if (" + Printer(expr) + ") " + Printer(l,thn) + next
      }
      case While(expr: ExprTree, stat: StatTree) =>
        "while (" + Printer(expr) + ") " + Printer(l, stat)
      case Println(expr: ExprTree) =>
        "println(" + Printer(expr) + ");"
      case Assign(id: Identifier, expr: ExprTree) =>
        Printer(id) + " = " + Printer(expr) + ";"
      case ArrayAssign(id: Identifier, index: ExprTree, expr: ExprTree) =>
        Printer(id) + "[" + Printer(index) + "] = " + Printer(expr)

      case And(lhs: ExprTree, rhs: ExprTree) => expr("&&", lhs, rhs)
      case Or(lhs: ExprTree, rhs: ExprTree) => expr("||", lhs, rhs)
      case Plus(lhs: ExprTree, rhs: ExprTree) => expr("+", lhs, rhs)
      case Minus(lhs: ExprTree, rhs: ExprTree) => expr("-", lhs, rhs)
      case Times(lhs: ExprTree, rhs: ExprTree) => expr("*", lhs, rhs)
      case Div(lhs: ExprTree, rhs: ExprTree) => expr("/", lhs, rhs)
      case LessThan(lhs: ExprTree, rhs: ExprTree) => expr("<", lhs, rhs)
      case Equals(lhs: ExprTree, rhs: ExprTree) => expr("==", lhs, rhs)
      case ArrayRead(arr: ExprTree, index: ExprTree) =>
        "(" + Printer(arr) + ")" + "[" + Printer(index) + "]"
      case ArrayLength(arr: ExprTree) =>
        "(" + Printer(arr) + ")" + ".length"
      case MethodCall(obj: ExprTree, meth: Identifier, args: List[ExprTree]) =>
        "(" + Printer(obj) + ")." + Printer(meth) + "(" + unroll(0, args, ", ") + ")"
      case IntLit(value: Int) => value.toString
      case StringLit(value: String) => "\"" + value + "\""

      case True() => "true"
      case False() => "false"
      case Identifier(value: String) => value
      case This() => "this"
      case NewIntArray(size: ExprTree) => "new Int[" + Printer(size) + "]"
      case New(tpe: Identifier) => "new " + Printer(tpe) + "()"
      case Not(expr: ExprTree) => "! (" + Printer(expr) + ")"
    }
  }
}
