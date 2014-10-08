#!/bin/bash

readonly toolc_path="toolc-reference-2.6.jar"
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
		echo -n '!'
	fi
}

compile() {
	sbt <<< ';package;exit' > /dev/null

	ls target/scala-*/toolc_*.jar
}

match() {
	local com="$1"
	local f="$2"

	echo -n "Testing: $f "

	local ret1
	if $use_sbt
	then
		sbt_cmd=";compile; run $f;exit"
		sbt <<< "$sbt_cmd" | \
			grep -vE "$sbt_cleanup|^> $sbt_cmd$" \
				> "$prefix"1_tmp 2>/dev/null
		ret1=$?
		check $ret1
	else
		scala -cp "$com:$libraries" 'toolc.Main' "$f" \
			> "$prefix"1_tmp 2>/dev/null
		ret1=$?
		check $ret1
	fi

	local ret2
	java -jar "$toolc_path" --tokens "$f" > "$prefix"2_tmp 2>/dev/null
	ret2=$?
	check $ret2

	[[ $ret1 -ne $ret2 ]] && echo && exit 1
	rm -f *.class

	awk 'BEGIN {b=0} b == 0 {print} b == 0 && /^BAD/ {b=1}' \
		"$prefix"1_tmp > "$prefix"1
	awk 'BEGIN {b=0} b == 0 {print} b == 0 && /^BAD/ {b=1}' \
		"$prefix"2_tmp > "$prefix"2

	local ret3
	diff -q "$prefix"1 "$prefix"2 > /dev/null
	ret3=$?
	check $ret3

	echo
	[[ $ret3 -ne 0 ]] && exit 1
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
	for f in programs/* wrong_programs/*
	do
		match "$local_test" "$f"
	done
fi

rm "$prefix"{1,2}{_tmp,}
