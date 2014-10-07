#!/bin/sed -f

s/^+//

s/^ID(\([^)]*\))(.*)/\1/
s/^INT(\([^)]*\))(.*)/\1/
s/^STR(\([^)]*\))(.*)/"\1"/

s/^EOF(.*)//
s/^BAD(.*)/~/

s/^SEMICOLON(.*)/;\n/
s/^COLON(.*)/:/
s/^DOT(.*)/./
s/^COMMA(.*)/,/
s/^EQSIGN(.*)/=/
s/^EQUALS(.*)/==/
s/^BANG(.*)/!/
s/^LPAREN(.*)/(/
s/^RPAREN(.*)/)/
s/^LBRACKET(.*)/[/
s/^RBRACKET(.*)/]/
s/^LBRACE(.*)/{/
s/^RBRACE(.*)/}/
s/^AND(.*)/&&/
s/^OR(.*)/||/
s/^LESSTHAN(.*)/</
s/^PLUS(.*)/+/
s/^MINUS(.*)/-/
s/^TIMES(.*)/*/
s/^DIV(.*)/\//
s/^OBJECT(.*)/object/
s/^CLASS(.*)/class/
s/^DEF(.*)/def/
s/^VAR(.*)/var/
s/^UNIT(.*)/Unit/
s/^MAIN(.*)/main/
s/^STRING(.*)/String/
s/^EXTENDS(.*)/extends/
s/^INT(.*)/Int/
s/^BOOLEAN(.*)/Boolean/
s/^WHILE(.*)/while/
s/^IF(.*)/if/
s/^ELSE(.*)/else/
s/^RETURN(.*)/return/
s/^LENGTH(.*)/length/
s/^TRUE(.*)/true/
s/^FALSE(.*)/false/
s/^THIS(.*)/this/
s/^NEW(.*)/new/
s/^PRINTLN(.*)/println/
