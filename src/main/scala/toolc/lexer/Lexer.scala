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
      var current = source.next();
      def remain = source.hasNext
      def next = current = source.next();
    }

    def currentPos(): Positioned = {
      new Positioned {}.setPos(f, source.pos)
    }

    def readNextToken(): Token = {

      var current: TokenKind = BAD;

      if (ch.remain) {

        var parsed = false;
        while (!parsed) {

          parsed = true // generally true
          var loadNext = true; // generally true
          ch.current match {

            case ':' => current = COLON
            case ';' => current = SEMICOLON
            case '.' => current = DOT
            case ',' => current = COMMA

            case '=' =>
              ch.next
              if (ch.current == '=') {
                current = EQUALS
              } else {
                current = EQSIGN
                loadNext = false
              }

            case '!' => current = BANG
            case '(' => current = LPAREN
            case ')' => current = RPAREN
            case '[' => current = LBRACKET
            case ']' => current = RBRACKET
            case '{' => current = LBRACE
            case '}' => current = RBRACE

            case '&' =>
              ch.next
              if (ch.current == '&') current = AND
              else loadNext = false

            case '|' =>
              ch.next
              if (ch.current == '|') current = OR
              else loadNext = false

            case '<' => current = LESSTHAN
            case '+' => current = PLUS
            case '-' => current = MINUS
            case '*' => current = TIMES

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

            case _ => BAD

          }

          println(currentPos().line + ":" + currentPos.col + ":" + ch.current)

          if (loadNext && ch.remain) ch.next
        }
      } else {
        current = EOF
      }
      val token = new Token(current);
      token.setPos(currentPos)
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
