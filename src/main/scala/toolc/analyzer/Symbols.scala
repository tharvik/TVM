package toolc
package analyzer

import utils._
import Types._

object Symbols {
  trait Symbolic[S <: Symbol] {
    private var _sym: Option[S] = None

    def setSymbol(sym: S): this.type = {
      _sym = Some(sym)
      this
    }

    def getSymbol: S = _sym match {
      case Some(s) => s
      case None => sys.error("Accessing undefined symbol.")
    }

    def isDefined = _sym.isDefined
  }

  sealed abstract class Symbol extends Positioned with Typed {
    val id: Int = ID.next
    val name: String
  }

  private object ID {
    private var c: Int = 0

    def next: Int = {
      val ret = c
      c = c + 1
      ret
    }
  }

  class GlobalScope {
    var mainClass: ClassSymbol = _
    var classes = Map[String, ClassSymbol]()

    def lookupClass(n: String): Option[ClassSymbol] =
      classes.get(n)
  }

  class ClassSymbol(val name: String) extends Symbol {
    var parent: Option[ClassSymbol] = None
    var methods = Map[String, MethodSymbol]()
    var members = Map[String, VariableSymbol]()
    var totaly_defined = false

    def lookupMethod(n: String): Option[MethodSymbol] =
      if (methods.get(n).isDefined)
        methods.get(n)
      else if (parent.isDefined)
        parent.get.lookupMethod(n)
      else None

    def lookupVar(n: String): Option[VariableSymbol] =
      if (members.get(n).isDefined) members.get(n)
      else if (parent.isDefined) parent.get.lookupVar(n)
      else None

  }

  class MethodSymbol(val name: String, val classSymbol: ClassSymbol) extends Symbol {
    var params = Map[String, VariableSymbol]()
    var members = Map[String, VariableSymbol]()
    var argList: List[VariableSymbol] = Nil
    var overridden: Option[MethodSymbol] = None

    def lookupVar(n: String): Option[VariableSymbol] =
      if (params.get(n).isDefined) params.get(n)
      else if (members.get(n).isDefined) members.get(n)
      else if (argList.exists(_.name == n)) Some(argList.filter(_.name == n).head)
      else classSymbol.lookupVar(n)

  }

  class VariableSymbol(val name: String) extends Symbol {
    var used = false
    var bc_id = -1
    var clss: Option[ClassSymbol] = None
    def is_field = clss.isDefined
    var is_arg = false
  }
}
