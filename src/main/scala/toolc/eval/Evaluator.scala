package toolc
package eval

import ast.Trees.And
import ast.Trees.ArrayAssign
import ast.Trees.ArrayLength
import ast.Trees.ArrayRead
import ast.Trees.Assign
import ast.Trees.Block
import ast.Trees.ClassDecl
import ast.Trees.Div
import ast.Trees.Equals
import ast.Trees.ExprTree
import ast.Trees.False
import ast.Trees.Identifier
import ast.Trees.If
import ast.Trees.IntLit
import ast.Trees.LessThan
import ast.Trees.MethodCall
import ast.Trees.MethodDecl
import ast.Trees.Minus
import ast.Trees.New
import ast.Trees.NewIntArray
import ast.Trees.Not
import ast.Trees.Or
import ast.Trees.Plus
import ast.Trees.Println
import ast.Trees.Program
import ast.Trees.StatTree
import ast.Trees.StringLit
import ast.Trees.This
import ast.Trees.Times
import ast.Trees.True
import ast.Trees.While
import utils.Context

class Evaluator(ctx: Context, prog: Program) {
  import ctx.reporter._

  def eval() {
    // Initialize the context for the main method
    val ectx = new MainMethodContext

    // Evaluate each statement of the main method
    prog.main.stats.foreach(evalStatement(ectx, _))
  }

  def evalStatement(ectx: EvaluationContext, stmt: StatTree): Unit = stmt match {
    case Block(stats) =>
      for (stat <- stats) evalStatement(ectx, stat)

    case If(expr, thn, els) =>
      val test = evalExpr(ectx, expr).asBool
      if (test) evalStatement(ectx, thn)
      else els.map(evalStatement(ectx, _))

    case While(expr, stat) =>
      var test = evalExpr(ectx, expr).asBool
      while (test) {
        evalStatement(ectx, stat)
        test = evalExpr(ectx, expr).asBool
      }

    case Println(expr) =>
      val v = evalExpr(ectx, expr)
      v match {
        case StringValue(str) => println(str)
        case IntValue(x) => println(x)
        case BoolValue(x) => println(x)
        case x => fatal("Println only accept String and Int: " + x, stmt)
      }

    case Assign(id, expr) =>
      val v = evalExpr(ectx, expr)
      ectx.setVariable(id.value, v)

    case ArrayAssign(id, index, expr) =>
      val array = ectx.getVariable(id.value).asArray
      val i = evalExpr(ectx, index).asInt
      val v = evalExpr(ectx, expr).asInt
      array.setIndex(i, v)

    case x =>
      fatal("unnexpected statement: " + x, stmt)
  }

  def evalExpr(ectx: EvaluationContext, e: ExprTree): Value = e match {
    case IntLit(value) => IntValue(value)
    case StringLit(value) => StringValue(value)
    case True() => BoolValue(true)
    case False() => BoolValue(false)

    case And(lhs, rhs) =>
      val lv = evalExpr(ectx, lhs).asBool
      if (!lv) BoolValue(false)
      else evalExpr(ectx, rhs)

    case Or(lhs, rhs) =>
      val lv = evalExpr(ectx, lhs).asBool
      if (lv) BoolValue(true)
      else evalExpr(ectx, rhs)

    case Plus(lhs, rhs) =>
      val lv = evalExpr(ectx, lhs)
      val rv = evalExpr(ectx, rhs)
      (lv, rv) match {
        case (IntValue(l), IntValue(r)) => IntValue(l + r)
        case (StringValue(l), IntValue(r)) => StringValue(l + r)
        case (IntValue(l), StringValue(r)) => StringValue(l + r)
        case (StringValue(l), StringValue(r)) => StringValue(l + r)
        case x => fatal("Can only add Int and String: " + x, e)
      }

    case Minus(lhs, rhs) =>
      val lv = evalExpr(ectx, lhs)
      val rv = evalExpr(ectx, rhs)
      val res = (lv, rv) match {
        case (IntValue(l), IntValue(r)) => l - r
        case x => fatal("Can only substract Int: " + x, e)
      }
      IntValue(res)

    case Times(lhs, rhs) =>
      val lv = evalExpr(ectx, lhs)
      val rv = evalExpr(ectx, rhs)
      val res = (lv, rv) match {
        case (IntValue(l), IntValue(r)) => l * r
        case x => fatal("Can only multyplie Int: " + x, e)
      }
      IntValue(res)

    case Div(lhs, rhs) =>
      val lv = evalExpr(ectx, lhs)
      val rv = evalExpr(ectx, rhs)
      val res = (lv, rv) match {
        case (IntValue(l), IntValue(r)) => l / r
        case x => fatal("Can only divide Int: " + x, e)
      }
      IntValue(res)

    case LessThan(lhs, rhs) =>
      val lv = evalExpr(ectx, lhs)
      val rv = evalExpr(ectx, rhs)
      val res = (lv, rv) match {
        case (IntValue(l), IntValue(r)) => l < r
        case x => fatal("Can only use less than on Int: " + x, e)
      }
      BoolValue(res)

    case Not(expr) =>
      val v = evalExpr(ectx, expr)
      val res = (v) match {
        case BoolValue(a) => !a
        case x => fatal("Can only negate Bool: " + x, e)
      }
      BoolValue(res)

    case Equals(lhs, rhs) =>
      val lv = evalExpr(ectx, lhs)
      val rv = evalExpr(ectx, rhs)
      val res = (lv, rv) match {
        case (IntValue(l), IntValue(r)) => l == r
        case (BoolValue(l), BoolValue(r)) => l == r
        case (lr, rr) => lr eq rr
      }
      BoolValue(res)

    case ArrayRead(arr, index) =>
      val a = evalExpr(ectx, arr).asArray
      val i = evalExpr(ectx, index).asInt
      IntValue(a.getIndex(i))

    case ArrayLength(arr) =>
      val a = evalExpr(ectx, arr).asArray
      IntValue(a.size)

    case MethodCall(obj, meth, args) =>

      val o = evalExpr(ectx, obj).asObject
      val m = findMethod(o.cd, meth.value)
      val mctx = new MethodContext(o)

      if (args.length != m.args.length)
        fatal("Size of args to " + meth + " is not of the right size", e)

      for (i <- 0 until m.args.length) {
        val s = m.args(i).id.value
        val v = evalExpr(ectx, args(i))
        mctx.declareVariable(s)
        mctx.setVariable(s, v)
      }

      mctx.declareVariable("this") // TODO better way than reserving word
      mctx.setVariable("this", o)

      for (v <- m.vars)
        mctx.declareVariable(v.id.value)

      m.stats.foreach(evalStatement(mctx, _))

      evalExpr(mctx, m.retExpr)

    case Identifier(name) =>
      ectx.getVariable(name)

    case New(tpe) => tpe.value match {
      case "Int" => IntValue(0)
      case "Bool" => BoolValue(false)
      case "String" => StringValue("")
      case s: String =>
        val cd = findClass(s)
        val o = ObjectValue(cd)
        fieldsOfClass(cd).foreach(o.declareField(_))
        o
      case x => fatal("Unknow type: " + x, e)
    }

    case This() =>
      ectx.getVariable("this")

    case NewIntArray(size) =>
      val s = evalExpr(ectx, size).asInt
      ArrayValue(new Array(s), s)

    case x =>
      fatal("Undefined expression: " + x, e)
  }

  // Define the scope of evaluation, with methods to access/declare/set local variables(or arguments)
  abstract class EvaluationContext {
    def getVariable(name: String): Value
    def setVariable(name: String, v: Value): Unit
    def declareVariable(name: String): Unit
  }

  // A Method context consists of the execution context within an object method.
  // getVariable can fallback to the fields of the current object
  class MethodContext(val obj: ObjectValue) extends EvaluationContext {
    var vars = Map[String, Option[Value]]()

    def getVariable(name: String): Value = {
      vars.get(name) match {
        case Some(ov) =>
          ov.getOrElse(fatal("Uninitialized variable '" + name + "'"))
        case _ =>
          obj.getField(name)
      }
    }

    def setVariable(name: String, v: Value) {
      if (vars contains name) {
        vars += name -> Some(v)
      } else {
        obj.setField(name, v)
      }
    }

    def declareVariable(name: String) {
      vars += name -> None
    }
  }

  // Special execution context for the main method, which is very limitted.
  class MainMethodContext extends EvaluationContext {
    def getVariable(name: String): Value = fatal("The main method contains no variable and/or field")
    def setVariable(name: String, v: Value): Unit = fatal("The main method contains no variable and/or field")
    def declareVariable(name: String): Unit = fatal("The main method contains no variable and/or field")
  }

  def findMethod(cd: ClassDecl, name: String): MethodDecl = {
    cd.methods.find(_.id.value == name).orElse(
      cd.parent.map(p => findMethod(findClass(p.value), name)))
        .getOrElse(fatal("Unknown method " + cd.id + "." + name))
  }

  def findClass(name: String): ClassDecl = {
    prog.classes.find(_.id.value == name).getOrElse(fatal("Unknown class '" + name + "'"))
  }

  def fieldsOfClass(cl: ClassDecl): Set[String] = {
    cl.vars.map(_.id.value).toSet ++
      cl.parent.map(p => fieldsOfClass(findClass(p.value))).getOrElse(Set())
  }

  // Runtime evaluation values, with as* methods which act as typecasts for convenience.
  sealed abstract class Value {
    def asInt: Int = fatal("Unnexpected value, found " + this + " expected Int")
    def asString: String = fatal("Unnexpected value, found " + this + " expected String")
    def asBool: Boolean = fatal("Unnexpected value, found " + this + " expected Boolean")
    def asObject: ObjectValue = fatal("Unnexpected value, found " + this + " expected Object")
    def asArray: ArrayValue = fatal("Unnexpected value, found " + this + " expected Array")
  }

  case class ObjectValue(cd: ClassDecl) extends Value {
    var fields = Map[String, Option[Value]]()

    def setField(name: String, v: Value) {
      if (fields contains name) {
        fields += name -> Some(v)
      } else {
        fatal("Unknown field '" + name + "'")
      }
    }

    def getField(name: String) = {
      fields.get(name).flatten.getOrElse(fatal("Unknown field '" + name + "'"))
    }

    def declareField(name: String) {
      fields += name -> None
    }

    override def asObject = this
  }

  case class ArrayValue(var entries: Array[Int], val size: Int) extends Value {
    def setIndex(i: Int, v: Int) {
      if (i >= size || i < 0) {
        fatal("Index '" + i + "' out of bounds (0 .. " + size + ")")
      }
      entries(i) = v
    }

    def getIndex(i: Int) = {
      if (i >= size || i < 0) {
        fatal("Index '" + i + "' out of bounds (0 .. " + size + ")")
      }
      entries(i)
    }

    override def asArray = this
  }

  case class StringValue(var v: String) extends Value {
    override def asString = v
  }

  case class IntValue(var v: Int) extends Value {
    override def asInt = v
  }

  case class BoolValue(var v: Boolean) extends Value {
    override def asBool = v
  }
}

