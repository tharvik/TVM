#!/bin/bash

readonly ref='toolc-reference-*.jar'
readonly tes='tvm/bin/Debug/tvm'

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
	local f="${1}"

	java -jar ${ref} "$f"
	check $?
}

reference() {
	local f="${1}"

	java "$(echo "${f}" | cut -d . -f 1)" > "${f}_ref"
	check $?
}

generate() {
	local f="${1}"

	${tes} "${f}" 2>/dev/null > "${f}_tes"
	check $?
}

match() {
	local com="$1"
	local f="$2"

	local l=$(grep -hE '^\s*(object)' "${f}" | \
		sed 's/^\s*//' | \
		cut -d ' ' -f 2 | \
		tr -d '\r{').class

	echo -n "Testing: $f "
	compile "${f}"

	reference "${l}"
	generate "${l}"

	diff "${l}"_{ref,tes}
	check $?

	rm *class{_{tes,ref},}

	echo
}

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
