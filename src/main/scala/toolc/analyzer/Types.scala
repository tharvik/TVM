package toolc
package analyzer

import Symbols._

object Types {
  trait Typed {
    self =>

    private var _tpe: Type = TUntyped

    def setType(tpe: Type): self.type = { _tpe = tpe; this }
    def getType: Type = _tpe
  }

  sealed abstract class Type {
    def isSubTypeOf(tpe: Type): Boolean
  }

  case object TError extends Type {
    override def isSubTypeOf(tpe: Type): Boolean = true
    override def toString = "[error]"
  }

  case object TUntyped extends Type {
    override def isSubTypeOf(tpe: Type): Boolean = false
    override def toString = "[untyped]"
  }

  lazy val typeToStr: Map[Type, String] = Map(
    TInt -> "int",
    TBoolean -> "boolean",
    TIntArray -> "int[array]",
    TString -> "string")

  case object TInt extends Type {
    override def isSubTypeOf(tpe: Type): Boolean = tpe match {
      case TInt => true
      case _ => false
    }
    override def toString = typeToStr(TInt)
  }

  // TODO: Complete by creating necessary types

  case object TBoolean extends Type {
    override def isSubTypeOf(tpe: Type): Boolean = tpe match {
      case TBoolean => true
      case _ => false
    }
    override def toString = typeToStr(TBoolean)
  }

  case object TIntArray extends Type {
    override def isSubTypeOf(tpe: Type): Boolean = tpe match {
      case TIntArray => true
      case _ => false
    }
    override def toString = typeToStr(TIntArray)
  }

  case object TString extends Type {
    override def isSubTypeOf(tpe: Type): Boolean = tpe match {
      case TString => true
      case _ => false
    }
    override def toString = typeToStr(TString)
  }

  case class TObject(classSymbol: ClassSymbol) extends Type {
    override def isSubTypeOf(tpe: Type): Boolean = tpe match {
      case TAnyObject => true
      case TObject(x) => {
        val p = classSymbol.parent
        println(x.name + " ~ " + classSymbol.name)
        val a =
          if (classSymbol == x) true
          else if (p.isDefined) p.get.getType.isSubTypeOf(tpe)
          else false
        println(x.name + " ~ " + classSymbol.name + " = " + a)
        a
      }
      case _ => false
    }
    override def toString = classSymbol.name
  }

  // special object to implement the fact that all objects are its subclasses
  case object TAnyObject extends Type {
    override def isSubTypeOf(tpe: Type): Boolean = tpe match {
      case _: TObject => true
      case TAnyObject => true
      case _ => false
    }
    override def toString = "Object"
  }
}
