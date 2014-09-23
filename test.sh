#!/bin/bash

readonly toolc_path="toolc-reference-2.3.jar"
readonly prefix='test_'

set -e

compile_test() {
	f="$1"

	classname="$(basename $(echo "$f" | cut -d . -f 1))"

	sbt <<< "compile
run $f
exit" | tail -n +8 | head -n -2 > "$prefix"1

	java -jar "$toolc_path" "$f"
	java $classname > "$prefix"2
	rm -f *.class

	diff -u "$prefix"1 "$prefix"2 || {
		echo "Error in $f"
		exit 1
	}
}

if [ $# -ne 0 ]
then
	compile_test "$@"
else
	for f in programs/*
	do
		compile_test "$f"
	done
fi

rm "$prefix"1 "$prefix"2
