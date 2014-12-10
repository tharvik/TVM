package toolc

import utils._
import java.io.File

import lexer._
import ast._
import eval._
import analyzer._
import code._

object Main {

  def processOptions(args: Array[String]): Context = {
    val reporter = new Reporter()
    var files: List[File] = Nil
    var outDir: Option[File] = None

    def rec(args: List[String]): Unit = args match {
      case "-d" :: dir :: xs =>
        outDir = Some(new File(dir))
        rec(xs)

      case f :: xs =>
        files  ::= new File(f)
        rec(xs)

      case _ =>
    }

    rec(args.toList)

    if (files.size != 1) {
      reporter.fatal("Exactly one file expected, "+files.size+" file(s) given.")
    }

    Context(reporter = reporter, file = files.head, outDir = outDir)
  }


  def main(args: Array[String]) {
    val ctx = processOptions(args)

    val dump = ASTDumper.apply(args)

    val pipeline = ASTDumpParser andThen
                   NameAnalysis andThen
                   TypeChecking andThen
                   CodeGeneration

    pipeline.run(ctx)(dump)

    ctx.reporter.terminateIfErrors
  }
}
