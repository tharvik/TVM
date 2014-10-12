#!/bin/bash

## Cookies is Netscape file format (for wget)
readonly cookies="$1"
readonly lara_domaine='http://larasrv09.epfl.ch:9000'
readonly lara_course='clp14'
readonly lara_tab='tests'
readonly lara_group='64'
readonly wget_opt='-nv'
readonly tokens_transformer='./tokensToCode.awk'
readonly beautifier='./beautifier.sed'

set -e

lara_url="$lara_domaine/$lara_course/$lara_tab"
testrun=$(wget $wget_opt --load-cookies "$cookies" "$lara_url" -O - | \
	awk '	BEGIN {FS="\""}
		/class="testrun"/ {if(!testrun) {testrun=$4}}
		END {print testrun}')

lara_url="$lara_domaine/$lara_course/api/$lara_group/$lara_tab/get/$testrun"
wget $wget_opt --load-cookies "$cookies" "$lara_url" -O - | \
	awk 'BEGIN {FS="\""} /title="See log"/ {print $8}' | \
	while read log
	do
		file="$(tempfile)"
		lara_url="$lara_domaine/$lara_course/api/$lara_group/$lara_tab"
		lara_url="$lara_url/log/$testrun/$log"
		wget $wget_opt --load-cookies "$cookies" "$lara_url" -O - | \
			awk '	BEGIN {b=0}
				b == 1 && /^<\/code><\/pre>$/ {b=2}
				b == 1 {print}
				b == 0 && /class="diff"/ {b=1}' | \
			grep -v '\-- Diff hidden --' | tail -n +4 > "$file"
		[[ -z "$(cat "$file")" ]] && continue

		name=$(awk '	BEGIN {FS="[)(]"}
				/ID/ {print $2; exit 0}' "$file")
		"$tokens_transformer" "$file" | "$beautifier" | \
			awk '	BEGIN {i=0}
				/}/ {i--}
				{str="";for(j=0; j<i; j++) {str = str "\t"};
					print str $0}
				/{/ {i++}
				' > ${name}.tool
	done
