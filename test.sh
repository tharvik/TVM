#!/bin/bash

readonly toolc_path="toolc-reference-2.4.jar"
readonly libraries=""
readonly prefix='test_'
t='^Loading /.*/sbt-.*sbt-launch-lib.bash$'
t="$t|^.\[0m\[.[\d+m(info|trace|error|success).\[0m\]"
readonly sbt_cleanup="$t"

if [[ $1 = '-s' ]]
then
	t='true'
	shift
else
	t='false'
fi
readonly use_sbt="$t"

check() {
	local status=$1

	if [ $status -eq 0 ]
	then
		echo -n '.'
	else
		echo '!'
		exit 1
	fi
}

compile() {
	sbt <<< ';package;exit' > /dev/null

	ls target/scala-*/toolc_*.jar
}

match() {
	local com="$1"
	local f="$2"

	echo -n "Testing: $f ."

	if $use_sbt
	then
		sbt_cmd=";compile; run $f;exit"
		sbt <<< "$sbt_cmd" | \
			grep -vE "$sbt_cleanup|^> $sbt_cmd$" > "$prefix"1
		check $?
	else
		scala -cp "$com:$libraries" 'toolc.Main' "$f" > "$prefix"1
		check $?
	fi

	java -jar "$toolc_path" --tokens "$f" > "$prefix"2
	check $?
	rm -f *.class

	#> DEBUG
	sed -i 's/[ ]*$//' "$prefix"{1,2}
	#< DEBUG

	diff -q "$prefix"1 "$prefix"2 > /dev/null
	check $?

	echo
}

if ! $use_sbt
then
	echo -n 'Compiling ...'
	local_test="$(compile)"
	echo
fi

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
