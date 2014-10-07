package toolc
package lexer

import utils._
import scala.io.BufferedSource

sealed class Token(val kind: TokenKind) extends Positioned {
  override def toString = kind.toString
}

sealed trait TokenKind;

object Tokens {

  object STRLITKIND extends TokenKind {
    override def toString = "string literal"
  }

  object INTLITKIND extends TokenKind {
    override def toString = "integer literal"
  }

  object IDKIND extends TokenKind {
    override def toString = "identifier"
  }

  object Kinded {
    def unapply(t: Token): Option[TokenKind] = {
      Some(t.kind)
    }
  }

  case object BAD extends TokenKind         // represents incorrect tokens.
  case object EOF extends TokenKind
  case object COLON extends TokenKind       // :
  case object SEMICOLON extends TokenKind   // ;
  case object DOT extends TokenKind         // .
  case object COMMA extends TokenKind       // ,
  case object EQSIGN extends TokenKind      // =
  case object EQUALS extends TokenKind      // ==
  case object BANG extends TokenKind        // !
  case object LPAREN extends TokenKind      // (
  case object RPAREN extends TokenKind      // )
  case object LBRACKET extends TokenKind    // [
  case object RBRACKET extends TokenKind    // ]
  case object LBRACE extends TokenKind      // {
  case object RBRACE extends TokenKind      // }
  case object AND extends TokenKind         // &&
  case object OR extends TokenKind          // ||
  case object LESSTHAN extends TokenKind    // <
  case object PLUS extends TokenKind        // +
  case object MINUS extends TokenKind       // -
  case object TIMES extends TokenKind       // *
  case object DIV extends TokenKind         // /
  case object OBJECT extends TokenKind      // object
  case object CLASS extends TokenKind       // class
  case object DEF extends TokenKind         // def
  case object VAR extends TokenKind         // var
  case object UNIT extends TokenKind        // unit
  case object MAIN extends TokenKind        // main
  case object STRING extends TokenKind      // string
  case object EXTENDS extends TokenKind     // extends
  case object INT extends TokenKind         // int
  case object BOOLEAN extends TokenKind     // boolean
  case object WHILE extends TokenKind       // while
  case object IF extends TokenKind          // if
  case object ELSE extends TokenKind        // else
  case object RETURN extends TokenKind      // return
  case object LENGTH extends TokenKind      // length
  case object TRUE extends TokenKind        // true
  case object FALSE extends TokenKind       // false
  case object THIS extends TokenKind        // this
  case object NEW extends TokenKind         // new
  case object PRINTLN extends TokenKind     // println
  
  def identity(tk: TokenKind): (String,BufferedIterator[Char]) => Option[(Token, String)] =
    (_,_) => Some(new Token(tk), null)

  def skip: (String,BufferedIterator[Char]) => Option[(Token, String)] =
    (_,_) => None

  def default(key: String) : (String,BufferedIterator[Char]) => Option[(Token, String)] =
    (_,_) =>
      
      if(key.forall(_.isDigit))
        if(key.size > 1 && key(0) == '0') {
          Some(new Token(BAD),"Integral beginning with a zero")
        } else
          Some(new INTLIT(key.toInt), null)
          
      else if(key.toSet.subsetOf(default_component))
        if(key(0).isLetter)
          Some(new ID(key), null)
        else {
          Some(new Token(BAD), "Identifier begin without a letter")
        }
      
      else
        Some(new Token(BAD), "Invalid character")

  def char_range(base: Char, count: Int): IndexedSeq[Char] =
    for(i <- 0 to (count - 1))
      yield (Char.char2int(base) + i).toChar

  val default_component: Set[Char] =
    Set('_') ++
    char_range('A', 26) ++
    char_range('a', 26) ++
    char_range('0', 10)

  val map: Map[String,(String,BufferedIterator[Char]) => Option[(Token,String)]] = Map(
    ":"            -> identity(COLON),
    ";"            -> identity(SEMICOLON),
    "."            -> identity(DOT),
    ","            -> identity(COMMA),
    "="            -> identity(EQSIGN),
    "=="           -> identity(EQUALS),
    "!"            -> identity(BANG),
    "("            -> identity(LPAREN),
    ")"            -> identity(RPAREN),
    "["            -> identity(LBRACKET),
    "]"            -> identity(RBRACKET),
    "{"            -> identity(LBRACE),
    "}"            -> identity(RBRACE),
    "&&"           -> identity(AND),
    "||"           -> identity(OR),
    "<"            -> identity(LESSTHAN),
    "+"            -> identity(PLUS),
    "-"            -> identity(MINUS),
    "*"            -> identity(TIMES),
    "/"            -> identity(DIV),
    "object"       -> identity(OBJECT),
    "class"        -> identity(CLASS),
    "def"          -> identity(DEF),
    "var"          -> identity(VAR),
    "Unit"         -> identity(UNIT),
    "main"         -> identity(MAIN),
    "String"       -> identity(STRING),
    "extends"      -> identity(EXTENDS),
    "Int"          -> identity(INT),
    "Bool"         -> identity(BOOLEAN),
    "while"        -> identity(WHILE),
    "if"           -> identity(IF),
    "else"         -> identity(ELSE),
    "return"       -> identity(RETURN),
    "length"       -> identity(LENGTH),
    "true"         -> identity(TRUE),
    "false"        -> identity(FALSE),
    "this"         -> identity(THIS),
    "new"          -> identity(NEW),
    "println"      -> identity(PRINTLN),

    " "            -> skip,
    "\t"           -> skip,
    "\n"           -> skip,

    "//"           -> ((_: String, buffered: BufferedIterator[Char]) => {
                        while(buffered.head != '\n') buffered.next
                        None
                      }),
    "/*"           -> ((x: String, buffered: BufferedIterator[Char]) => {
                        var continue = true
                        while(continue) {
                          while(buffered.next != '*') {}
                          continue = (buffered.head != '/')
                        }
                        buffered.next
                        None
                      }),

    "\""           -> ((x: String, buffered: BufferedIterator[Char]) => {
                        val str = buffered.takeWhile(_ != '"').mkString
                        if(str.contains('\n')) {
                          Some(new Token(BAD), "Line feed inside of string litteral")
                        } else
                          Some(new STRLIT(str), null)
                      })
  ).withDefault(default)
  
  val set = map.keySet

  // Identifiers
  class ID(val value: String) extends Token(IDKIND) {
    override def toString = "ID("+value+")"
  }

  // Integer literals
  class INTLIT(val value: Int) extends Token(INTLITKIND) {
    override def toString = "INT("+value+")"
  }

  // String literals
  class STRLIT(val value: String) extends Token(STRLITKIND) {
    override def toString = "STR("+value+")"
  }
}
