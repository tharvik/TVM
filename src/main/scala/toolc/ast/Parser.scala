package toolc
package ast

import scala.annotation.tailrec

import Trees._
import lexer._
import utils._
import Tokens._

object Parser extends Pipeline[Iterator[Token], Program] {

  def run(ctx: Context)(tokens: Iterator[Token]): Program = {
    import ctx.reporter._

    // Store the current token, as read from the lexer.
    var currentToken: Token = new Token(BAD)

    def readToken: Unit = {
      if (tokens.hasNext) {
        currentToken = tokens.next

        // skips bad tokens
        while(currentToken.kind == BAD && tokens.hasNext) {
          currentToken = tokens.next
        }
      }
    }

    // ''Eats'' the expected token, or terminates with an error.
    def eat(kind: TokenKind): Unit =
      if(currentToken.kind == kind) readToken
      else expected(kind)

    // Complains that what was found was not expected.
    def expected(kind: TokenKind, more: TokenKind*): Nothing =
      expectedS((kind :: more.toList).toSet)

    def expectedS(more: Set[TokenKind]): Nothing = {
      fatal("expected: " + (more.toList).mkString(" or ") + ", found: " + currentToken, currentToken)
    }

    def parseListWhile[T](tks: Set[TokenKind], add: (() => T),
      sep: Option[TokenKind]) = {

      new Function1[List[T],List[T]]{

        val nexts = sep match {
          case Some(tk) => tks + tk
          case None => tks
        }

        @tailrec
        def apply(l: List[T]) = {

          if(!nexts.contains(currentToken.kind)) l
          else {
            if(!l.isEmpty) sep.map(eat(_))
            this(l :+ add())
          }
        }
      }
    }

    def parseGoal: Program = {

      val pos = currentToken

      val main = parseMainObject
      val classes = parseListClassDecl
      eat(EOF)

      Program(main, classes).setPos(pos)
    }

    def parseMainObject: MainObject = {

      val pos = currentToken

      eat(OBJECT)
      val id = parseIdentifier

      eat(LBRACE)
      eat(DEF)
      eat(MAIN)
      eat(LPAREN)
      eat(RPAREN)
      eat(COLON)
      eat(UNIT)
      eat(EQSIGN)
      eat(LBRACE)
      val stats = parseStatTreeList
      eat(RBRACE)
      eat(RBRACE)

      MainObject(id, stats).setPos(pos)
    }

    def parseClassDecl(): ClassDecl = {

      val pos = currentToken

      eat(CLASS)
      val id = parseIdentifier

      val parent = currentToken.kind match {
        case EXTENDS => {
          eat(EXTENDS)
          Some(parseIdentifier)
        }
        case LBRACE  => None
        case _ => expected(EXTENDS, LBRACE)
      }

      eat(LBRACE)
      val vars = parseVarDeclList
      val methods = parseMethodDeclList
      eat(RBRACE)

      ClassDecl(id, parent, vars, methods).setPos(pos)
    }

    def parseListClassDecl: List[ClassDecl] =
      parseListWhile[ClassDecl](Set(CLASS), parseClassDecl, None)(List())

    def parseVarDecl(): VarDecl = {

      val pos = currentToken

      eat(VAR)
      val id = parseIdentifier
      eat(COLON)
      val tpe = parseType
      eat(SEMICOLON)

      VarDecl(tpe, id).setPos(pos)
    }

    def parseVarDeclList: List[VarDecl] =
      parseListWhile[VarDecl](Set(VAR), parseVarDecl,
        None)(List())

    def parseMethodDecl(): MethodDecl = {

      val pos = currentToken

      eat(DEF)
      val id = parseIdentifier
      eat(LPAREN)
      val args = parseFormalList
      eat(RPAREN)
      eat(COLON)
      val retType = parseType
      eat(EQSIGN)
      eat(LBRACE)
      val vars = parseVarDeclList
      val stats = parseStatTreeList
      eat(RETURN)
      val retExpr = parseExprTree()
      eat(SEMICOLON)
      eat(RBRACE)

      MethodDecl(retType, id, args, vars, stats, retExpr).setPos(pos)
    }

    def parseMethodDeclList: List[MethodDecl] =
      parseListWhile[MethodDecl](Set(DEF), parseMethodDecl, None)(List())

    def parseFormal(): Formal = {

      val pos = currentToken

      val id = parseIdentifier
      eat(COLON)
      val tpe = parseType

      Formal(tpe, id).setPos(pos)
    }

    def parseFormalList: List[Formal] =
      parseListWhile[Formal](Set(IDKIND, COMMA),
      parseFormal, Some(COMMA))(List())

    def parseType: TypeTree = types(currentToken.kind)()

    def parseIntArrayType(): IntArrayType = {

      val pos = currentToken

      eat(LBRACKET)
      eat(RBRACKET)

      IntArrayType().setPos(pos)
    }
    def parseBooleanType(): BooleanType = {

      val pos = currentToken

      eat(BOOLEAN)

      BooleanType().setPos(pos)
    }

    def parseIntType(): TypeTree = {

      val pos = currentToken

      eat(INT)
      val tt = currentToken.kind match {
        case LBRACKET => parseIntArrayType()
        case _ => IntType()
      }

      tt.setPos(pos)
    }

    def parseStringType(): StringType = {

      val pos = currentToken

      eat(STRING)

      StringType().setPos(pos)
    }

    def parseStatTree: StatTree =
      statTree.get(currentToken.kind) match {
        case Some(f) => f()
        case None    => expectedS(statTree.keySet)
      }

    def parseStatTreeList: List[StatTree] =
      parseListWhile[StatTree](Set(LBRACE,IDKIND,IF,WHILE,PRINTLN),
                               () => parseStatTree, None)(List())

    def parseBlock(): Block = {

      val pos = currentToken

      eat(LBRACE)
      val stats = parseStatTreeList
      eat(RBRACE)

      Block(stats).setPos(pos)
    }

    def parseIf(): If = {

      val pos = currentToken

      eat(IF)
      eat(LPAREN)
      val expr = parseExprTree()
      eat(RPAREN)
      val thn = parseStatTree
      val els = currentToken.kind match {
        case ELSE => {
          eat(ELSE)
          Some(parseStatTree)
        }
        case _    => None
      }

      If(expr, thn, els).setPos(pos)
    }

    def parseWhile(): While = {

      val pos = currentToken

      eat(WHILE)
      eat(LPAREN)
      val expr = parseExprTree()
      eat(RPAREN)
      val stat = parseStatTree

      While(expr, stat).setPos(pos)
    }

    def parsePrintln(): Println = {

      val pos = currentToken

      eat(PRINTLN)
      eat(LPAREN)
      val expr = parseExprTree()
      eat(RPAREN)
      eat(SEMICOLON)

      Println(expr).setPos(pos)
    }

    def parseTreeID(): StatTree = {

      val id = parseIdentifier
      statTreeID.get(currentToken.kind) match  {
        case Some(f) => f(id)
        case None    => expectedS(statTreeID.keySet)
      }
    }

    def parseAssign(id: Identifier): Assign = {

      val pos = currentToken

      eat(EQSIGN)
      val expr = parseExprTree()
      eat(SEMICOLON)

      Assign(id,expr).setPos(pos)
    }

    def parseArrayAssign(id: Identifier): ArrayAssign = {

      val pos = currentToken

      eat(LBRACKET)
      val index = parseExprTree()
      eat(RBRACKET)
      eat(EQSIGN)
      val expr = parseExprTree()
      eat(SEMICOLON)

      ArrayAssign(id, index, expr).setPos(pos)
    }

    def parseExprTree(): ExprTree =
      parseExprTreeHelper(null)

    @tailrec
    def parseExprTreeHelper(acc: ExprTree) : ExprTree = {
      val cur = currentToken.kind

      val finite = exprFinite.keySet
      val infinite = exprInfinite.keySet

      val obj =
        if(infinite.contains(cur)) parseExprTreeInfinite(acc)
        else if(finite.contains(cur)) parseExprTreeFinite
        else expectedS(expr)

      if(!expr.contains(currentToken.kind)) obj
      else parseExprTreeHelper(obj)
    }

    def parseExprTreeFinite: ExprTree =
      exprFinite.get(currentToken.kind) match {
        case Some(f) => f()
        case None => expectedS(exprFinite.keySet)
      }

    def parseExprTreeInfinite(obj: ExprTree): ExprTree =
      exprInfinite.get(currentToken.kind) match {
        case Some(f) => f(obj).setPos(obj)
        case None => expectedS(exprInfinite.keySet)
      }

    def parseExprTreeInfiniteLevel(c: TokenKind): Int =
      tokenOrder.indexWhere(_.contains(c))

    def parseExprTreeInfiniteHelper(obj: ExprTree, ll: Int): ExprTree = {

      val c = currentToken.kind

      if(c == TIMES) {
        eat(TIMES)
        Times(obj, parseExprTree)
      } else if(c == PLUS) {
        eat(PLUS)
        Plus(obj, parseExprTree)
      } else obj
    }

    def parseArrayRead(lhs: ExprTree) = {
      eat(LBRACKET)
      val rhs = parseExprTree()
      eat(RBRACKET)

      ArrayRead(lhs,rhs).setPos(lhs)
    }

    def parseDot(obj: ExprTree): ExprTree = {
      eat(DOT)
      exprDot.get(currentToken.kind) match {
        case Some(f) => f(obj).setPos(obj)
        case None => expectedS(exprDot.keySet)
      }
    }

    def parseArrayLength(array: ExprTree): ArrayLength = {

      eat(LENGTH)

      ArrayLength(array).setPos(array)
    }

    def parseMethodCall(obj: ExprTree) = {
      val meth = parseIdentifier
      eat(LPAREN)
      val args = parseMethodCallArgs
      eat(RPAREN)

      MethodCall(obj,meth,args).setPos(obj)
    }

    def parseMethodCallArgs: List[ExprTree] =
      parseListWhile[ExprTree](expr, parseExprTree, Some(COMMA))(List())

    def parseIntLit(): IntLit = {

      val pos = currentToken

      val value = if(currentToken.kind == INTLITKIND) {
        val token = currentToken.asInstanceOf[INTLIT] // TODO avoid casting
        token.value
      } else expected(INTLITKIND)
      eat(INTLITKIND)
      IntLit(value).setPos(pos)
    }

    def parseStrLit(): StringLit = {

      val pos = currentToken

      val value = if(currentToken.kind == STRLITKIND) {
        val token = currentToken.asInstanceOf[STRLIT] // TODO avoid casting
        token.value
      } else expected(STRLITKIND)
      eat(STRLITKIND)
      StringLit(value).setPos(pos)
    }

    def parseTrue(): True = {

      val pos = currentToken

      eat(TRUE)

      True().setPos(pos)
    }

    def parseFalse(): False = {

      val pos = currentToken

      eat(FALSE)

      False().setPos(pos)
    }

    def parseIdentifier(): Identifier = {

      val pos = currentToken

      val value = if(currentToken.kind == IDKIND) {
        val token = currentToken.asInstanceOf[ID] // TODO avoid casting
        token.value
      } else expected(IDKIND)
      eat(IDKIND)
      Identifier(value).setPos(pos)
    }

    def parseThis(): This = {

      val pos = currentToken

      eat(THIS)

      This().setPos(pos)
    }

    def parseNewIntArray(): NewIntArray = {

      val pos = currentToken

      eat(INT)
      eat(LBRACKET)
      val size = parseExprTree()
      eat(RBRACKET)

      NewIntArray(size).setPos(pos)
    }

    def parseNew(): ExprTree = {

      val pos = currentToken

      eat(NEW)
      if(currentToken.kind == INT) parseNewIntArray.setPos(pos)
      else {
        val tpe = parseIdentifier
        eat(LPAREN)
        eat(RPAREN)

        New(tpe).setPos(pos)
      }
    }

    def parseNot(): Not = {

      val pos = currentToken

      eat(BANG)
      val expr = parseExprTree()

      Not(expr).setPos(pos)
    }

    def parseParenthesis(): ExprTree = {

      eat(LPAREN)
      val expr = parseExprTree()
      eat(RPAREN)

      expr
    }

    // Set

    def parseExpr(kind: TokenKind): (ExprTree => ExprTree) =
      (lhs) => {
        eat(kind)

        val pos = currentToken

        val rhs = parseExprTree()

        tokensExpr.get(kind) match {
          case Some(t) => t(lhs,rhs).setPos(pos)
          case None => expectedS(tokensExpr.keySet)
        }
      }

    lazy val tokenOrder: List[Set[TokenKind]] = List(
      Set(TIMES,DIV),
      Set(MINUS,PLUS),
      Set(EQUALS,LESSTHAN),
      Set(AND),
      Set(OR)
    )

    lazy val exprOrder: List[Set[(ExprTree,ExprTree) => ExprTree]] =
      tokenOrder.map(_.map(tokensExpr(_)))

    lazy val exprInfinite: Map[TokenKind,ExprTree => ExprTree] = Map(
      AND      -> parseExpr(AND),
      OR       -> parseExpr(OR),
      EQUALS   -> parseExpr(EQUALS),
      LESSTHAN -> parseExpr(LESSTHAN),
      PLUS     -> parseExpr(PLUS),
      MINUS    -> parseExpr(MINUS),
      TIMES    -> parseExpr(TIMES),
      DIV      -> parseExpr(DIV),
      LBRACKET -> parseArrayRead,
      DOT      -> parseDot
    )

    lazy val exprFinite: Map[TokenKind,() => ExprTree] = Map(
      INTLITKIND -> parseIntLit,
      STRLITKIND -> parseStrLit,
      TRUE       -> parseTrue,
      FALSE      -> parseFalse,
      IDKIND     -> parseIdentifier,
      THIS       -> parseThis,
      NEW        -> parseNew,
      BANG       -> parseNot,
      LPAREN     -> parseParenthesis
    )

    lazy val expr: Set[TokenKind] = exprFinite.keySet union exprInfinite.keySet

    lazy val tokensExpr: Map[TokenKind,(ExprTree,ExprTree) => ExprTree] = Map(
      AND      -> And,
      OR       -> Or,
      EQUALS   -> Equals,
      LESSTHAN -> LessThan,
      PLUS     -> Plus,
      MINUS    -> Minus,
      TIMES    -> Times,
      DIV      -> Div
    )

    lazy val statTree: Map[TokenKind,() => StatTree] = Map(
      LBRACE  -> parseBlock,
      IF      -> parseIf,
      WHILE   -> parseWhile,
      PRINTLN -> parsePrintln,
      IDKIND  -> parseTreeID
    )

    lazy val statTreeID: Map[TokenKind,(Identifier) => StatTree] = Map(
      EQSIGN   -> parseAssign,
      LBRACKET -> parseArrayAssign
    )

    lazy val types: Map[TokenKind,() => TypeTree] = Map(
      INT       -> parseIntType,
      BOOLEAN   -> parseBooleanType,
      STRING    -> parseStringType,
      IDKIND    -> parseIdentifier
    )

    lazy val exprDot: Map[TokenKind,(ExprTree) => ExprTree] = Map(
      LENGTH -> parseArrayLength,
      IDKIND -> parseMethodCall
    )

    // Initialize the first token
    readToken

    // Parse
    parseGoal
  }
}
