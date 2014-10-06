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

    def currentPos(): Positioned = {
      new Positioned {}.setPos(f, source.pos)
    }

    @tailrec
    def parseNextToken(buffer: String, pos: Positioned, remains: Set[String]): (String, Positioned) = {

      if (pos.line == 37) {
        None
      }

      if (!buffered.hasNext)
        (buffer, pos)
      else {

        if (remains.size == 0) {
          val head = buffered.head
          if (variable_like.contains(buffer) && !default_component.contains(head))
            (buffer, pos)
          else if (default_component.contains(head))
            parseNextToken(buffer + buffered.next, pos, Set())
          else
            (buffer, pos)

        } else {
          val head = buffered.head
          val str = buffer + head
          val rest = remains.filter(_.startsWith(str))
          if (rest.size == 0)
            if (remains.contains(buffer))
              if (variable_like.contains(buffer))
                parseNextToken(buffer, pos, Set())
              else
                (buffer, pos)
            else
              parseNextToken(buffer, pos, rest)
          else
            parseNextToken(buffer + buffered.next, pos, rest)
        }

      }
    }

    @tailrec
    def readNextTokenPos: (Token, Positioned) = {

      if (buffered.hasNext) {
        val tuple = parseNextToken("", currentPos, set)
        val str = tuple._1
        val pos = tuple._2

        map(str)(str, buffered) match {
          case Some(x) => (x, pos)
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
