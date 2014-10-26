#!/bin/bash

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

generate() {
	local com="${1}"
	local out="${2}"

	local ret1
	if $use_sbt
	then
		sbt_cmd=";compile; run $f;exit"
		sbt <<< "$sbt_cmd" | \
			grep -vE "$sbt_cleanup|^> $sbt_cmd$" \
				> "${out}" 2>/dev/null
		ret1=$?
		check $ret1
	else
		scala -cp "$com" 'toolc.Main' "$f" \
			> "${out}" 2>/dev/null
		ret1=$?
		check $ret1
	fi
}

match() {
	local com="$1"
	local f="$2"

	echo -n "Testing: $f "

	generate "${com}" "${prefix}"1
	generate "${com}" "${prefix}"2

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

#rm "$prefix"{1,2}
