#!/bin/bash

readonly toolc_path="toolc-reference-2.3.jar"
readonly libraries="lib/toolc-parser_2.11-2.0.jar"
readonly prefix='test_'

set -e

compile() {
	sbt <<< ';package;exit' > /dev/null

	ls target/scala-*/toolc_*.jar
}

match() {
	local com="$1"
	local f="$2"

	echo -n "Testing: $f ."

	classname="$(basename $(echo "$f" | cut -d . -f 1))"

	scala -cp "$com:$libraries" 'toolc.Main' "$f" > "$prefix"1

	echo -n '.'

	java -jar "$toolc_path" "$f"
	echo -n '.'
	java $classname > "$prefix"2

	rm -f *.class

	diff -u "$prefix"1 "$prefix"2 || {
		echo '!'
		exit 1
	}
	echo '.'
}

echo -n 'Compiling...'
local_test="$(compile)"
echo

if [ $# -ne 0 ]
then
	for f in "$@"
	do
		match "$local_test" "$f"
	done
else
	for f in programs/*
	do
		match "$local_test" "$f"
	done
fi

rm "$prefix"1 "$prefix"2
