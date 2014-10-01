package toolc
package lexer

import java.io.File

import scala.io.Source

import Tokens.AND
import Tokens.BAD
import Tokens.BANG
import Tokens.COLON
import Tokens.COMMA
import Tokens.DIV
import Tokens.DOT
import Tokens.EOF
import Tokens.EQSIGN
import Tokens.EQUALS
import Tokens.LBRACE
import Tokens.LBRACKET
import Tokens.LESSTHAN
import Tokens.LPAREN
import Tokens.MINUS
import Tokens.OR
import Tokens.PLUS
import Tokens.RBRACE
import Tokens.RBRACKET
import Tokens.RPAREN
import Tokens.SEMICOLON
import Tokens.TIMES
import toolc.utils.Pipeline
import utils.Context
import utils.Pipeline
import utils.Positioned

object Lexer extends Pipeline[File, Iterator[Token]] {
  import Tokens._

  def run(ctx: Context)(f: File): Iterator[Token] = {
    val source = Source.fromFile(f)
    import ctx.reporter._

    object ch {
      var current: Char = source.next
      def remain = source.hasNext
      def next = current = source.next();
    }

    def currentPos(): Positioned = {
      new Positioned {}.setPos(f, source.pos)
    }

    def readNextToken(): Token = {

      var current: Token = new Token(BAD);
      var beginPos = currentPos

      if (ch.remain) {

        beginPos = currentPos
        var parsed = false
        var parsingString = false
        var oldParsingString = false
        var stringToken = ""
        var litteralString = false
        while (!parsed) {

          parsed = true // generally true
          var loadNext = true // generally true

          oldParsingString = parsingString
          parsingString = false // generally false

          ch.current match {

            case ':' => current = new Token(COLON)
            case ';' => current = new Token(SEMICOLON)
            case '.' => current = new Token(DOT)
            case ',' => current = new Token(COMMA)

            case '=' =>
              ch.next
              if (ch.current == '=') {
                current = new Token(EQUALS)
              } else {
                current = new Token(EQSIGN)
                loadNext = false
              }

            case '!' => current = new Token(BANG)
            case '(' => current = new Token(LPAREN)
            case ')' => current = new Token(RPAREN)
            case '[' => current = new Token(LBRACKET)
            case ']' => current = new Token(RBRACKET)
            case '{' => current = new Token(LBRACE)
            case '}' => current = new Token(RBRACE)

            case '&' =>
              ch.next
              if (ch.current == '&') current = new Token(AND)
              else loadNext = false

            case '|' =>
              ch.next
              if (ch.current == '|') current = new Token(OR)
              else loadNext = false

            case '<' => current = new Token(LESSTHAN)
            case '+' => current = new Token(PLUS)
            case '-' => current = new Token(MINUS)
            case '*' => current = new Token(TIMES)

            case '/' =>
              ch.next
              if (ch.current == '/') {
                parsed = false
                while (ch.remain && ch.current != '\n') ch.next
              } else if (ch.current == '*') {
                parsed = false
                var stillComment = true;
                while (stillComment) {
                  while (ch.remain && ch.current != '*') ch.next
                  ch.next
                  stillComment = (ch.current == '/');
                }
              } else {
                loadNext = false
                DIV
              }

            case '"' =>
              litteralString = !litteralString

            case _ =>
              parsed = false
              if (!ch.current.isWhitespace) {

                if(!oldParsingString)
                  beginPos = currentPos
                
                parsingString = true
                stringToken += ch.current
              } else if (oldParsingString) {
                parsed = true;
              }
          }

          if (oldParsingString && !parsingString) {

            if (litteralString) {
              current = new STRLIT(stringToken)
            } else {
              if (Tokens.set.contains(stringToken)) {
                current = new Token(map(stringToken))
              } else {
                if (stringToken.forall(_.isDigit))
                  current = new INTLIT(stringToken.toInt)
                else
                  current = new ID(stringToken)
              }
            }

          }
          if (loadNext && ch.remain) ch.next
        }
      } else {
        current = new Token(EOF)
      }
      current.setPos(beginPos)
    }

    new Iterator[Token] {
      var nextToken: Token = readNextToken
      var reachedEnd = false

      def hasNext = {
        nextToken.kind != EOF || !reachedEnd
      }

      def next = {
        val r = nextToken
        nextToken = readNextToken
        if (r.kind == EOF) {
          reachedEnd = true
        }
        r
      }
    }
  }
}
