package toolc
package lexer

import java.io.File

import scala.annotation.tailrec
import scala.io.Source

import Tokens.EOF
import Tokens.map
import Tokens.set
import toolc.utils.Pipeline
import utils.Context
import utils.Pipeline
import utils.Positioned

object Lexer extends Pipeline[File, Iterator[Token]] {
  import Tokens._

  def run(ctx: Context)(f: File): Iterator[Token] = {
    val source = Source.fromFile(f)
    val buffered = source.buffered
    import ctx.reporter._
    import Tokens._

    val variable_like = set.filter(_.toSet.subsetOf(default_component))
    val specials = set.diff(variable_like)

    def currentPos(): Positioned = {
      new Positioned {}.setPos(f, source.pos)
    }

    @tailrec
    def parseNextId(buffer: String): String = {
      val head = buffered.head
      if (default_component.contains(head))
        parseNextId(buffer + buffered.next)
      else
        buffer
    }

    @tailrec
    def parseNextToken(buffer: String, position: Positioned, remains: Set[String]): (String, Positioned) = {

      if (!buffered.hasNext)
        (buffer, position)

      else {
        val head = buffered.head
        val rest = remains.filter(_.startsWith(buffer + head))

        def nextBuffer = buffer + buffered.next

        val pos = if (buffer == "") currentPos else position

        if (pos.line == 5 && pos.col == 17) {
          None
        }

        if (rest.isEmpty)
          if (remains.contains(buffer))
            (buffer, pos)
          else
            (parseNextId(nextBuffer), pos)

        else if (rest.size == 1 && rest.contains(buffer + head))
          (nextBuffer, pos)

        else
          parseNextToken(nextBuffer, pos, rest)
      }
    }

    @tailrec
    def readNextTokenPos: (Token, Positioned) = {

      if (buffered.hasNext) {
        val tuple = parseNextToken("", currentPos, specials)
        val str = tuple._1
        val pos = tuple._2

        map(str)(str, buffered) match {
          case Some(t: (Token,String)) =>
            if(t._2 != null) ctx.reporter.error(t._2, pos)
            (t._1, pos)
          case None => readNextTokenPos
        }
      } else {
        (new Token(EOF), currentPos)
      }
    }

    def readNextToken(): Token = {
      if (buffered.hasNext) buffered.head

      val tuple = readNextTokenPos
      tuple._1.setPos(tuple._2)
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
