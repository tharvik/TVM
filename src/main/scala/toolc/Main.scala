package toolc

import utils._
import java.io.File

import lexer._
import ast._
import eval._
import analyzer._

object Main {

  def processOptions(args: Array[String]): Context = {
    val (opts, files) = args.toSeq.partition(_.startsWith("--"))
    val reporter = new Reporter()

    if (files.size != 1) {
      reporter.fatal("Exactly one file expected, "+files.size+" file(s) given.")
    }

    Context(reporter = reporter, file = new File(files.head))
  }


  def main(args: Array[String]) {
    val ctx = processOptions(args)

    val dump = ASTDumper.apply(args)

    val pipeline = ASTDumpParser andThen
                   NameAnalysis andThen
                   TypeChecking

    val result = pipeline.run(ctx)(dump)

    ctx.reporter.terminateIfErrors

    println(Printer(result))
  }
}
