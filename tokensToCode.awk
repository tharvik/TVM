#!/bin/awk -f

BEGIN {FS="\\)"}

function get_name(remove) {
		remove++;
		s=$0
		i=index(s,")");
		c=i;
		for(j=3;j<NF && i!=0;j++) {
			i++;
			s=substr(s,i);
			i=index(s,")");
			c+=i;
		}

		c-=remove;
		s=substr($0,remove,c);
		return s;
	}

/^\+/		{gsub("+","")}

/^ID/		{print get_name(3);}
/^INT(\([^)]*\)){2,}/ \
		{print get_name(4)}
/^STR(\([^)]*\)){2,}/ \
		{print "\"" get_name(4) "\"";}

/^EOF/		{}
/^BAD/		{print "~";}

/^SEMICOLON/	{print ";";}
/^COLON/	{print ":";}
/^DOT/		{print ".";}
/^COMMA/	{print ",";}
/^EQSIGN/	{print "=";}
/^EQUALS/	{print "==";}
/^BANG/		{print "!";}
/^LPAREN/	{print "(";}
/^RPAREN/	{print ")";}
/^LBRACKET/	{print "[";}
/^RBRACKET/	{print "]";}
/^LBRACE/	{print "{";}
/^RBRACE/	{print "}";}
/^AND/		{print "&&";}
/^OR/		{print "||";}
/^LESSTHAN/	{print "<";}
/^PLUS/		{print "+";}
/^MINUS/	{print "-";}
/^TIMES/	{print "*";}
/^DIV/		{print "/";}
/^OBJECT/	{print "object";}
/^CLASS/	{print "class";}
/^DEF/		{print "def";}
/^VAR/		{print "var";}
/^UNIT/		{print "Unit";}
/^MAIN/		{print "main";}
/^STRING/	{print "String";}
/^EXTENDS/	{print "extends";}
/^INT\([^)]*\)$/{print "Int";}
/^BOOLEAN/	{print "Boolean";}
/^WHILE/	{print "while";}
/^IF/		{print "if";}
/^ELSE/		{print "else";}
/^RETURN/	{print "return";}
/^LENGTH/	{print "length";}
/^TRUE/		{print "true";}
/^FALSE/	{print "false";}
/^THIS/		{print "this";}
/^NEW/		{print "new";}
/^PRINTLN/	{print "println";}
