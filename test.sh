#!/bin/bash

t='^Loading /.*/sbt-.*sbt-launch-lib.bash$'
t="$t|^.\[0m\[.[\d+m(info|trace|error|success).\[0m\]"
readonly sbt_cleanup="$t"
readonly ref='toolc-reference-*.jar'
readonly libs='lib/refparser.jar:lib/cafebabe.jar'

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

reference() {
	local f="${1}"

	java -jar ${ref} "$f" 2>&1 >/dev/null
	check $?
}

generate() {
	local f="${1}"
	local com="${2}"

	local ret1
	if $use_sbt
	then
		sbt_cmd=";compile; run $f;exit"
		sbt <<< "$sbt_cmd" | \
			grep -vE "$sbt_cleanup|^> $sbt_cmd$" \
				2>&1 >/dev/null
		ret1=$?
	else
		scala -cp "$com" 'toolc.Main' "$f" \
			2>&1 >/dev/null
		ret1=$?
	fi
	check $ret1
}

match() {
	local com="$1"
	local f="$2"

	local l=$(grep -hE '^\s*(class|object)' "${f}" | \
		sed 's/^\s*//' | \
		cut -d ' ' -f 2 | \
		tr -d '\r{')

	echo -n "Testing: $f "

	reference "${f}"
	for p in ${l}
	do
		javap -c "${p}" > "${p}_ref"
		check $?
	done
	generate "${f}" "${com}"

	for p in ${l}
	do
		javap -c "${p}" > "${p}_tes"
		diff -q "${p}_ref" "${p}_tes" > /dev/null
		check $?
		rm "${p}"{_{tes,ref},.class}
	done

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
		match "$local_test:${libs}" "$f"
	done
else
	for f in program/*
	do
		match "$local_test:${libs}" "$f"
	done
fi
