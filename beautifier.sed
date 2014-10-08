#!/bin/sed -f

:a
N
$!ba
s/\n/ /g

s/{ /{\n/g
s/} /\n}\n/g
s/ ; /;\n/g
s/ \. /./g
s/ ( /(/g
s/ )/)/g
s/ ,/,/g
