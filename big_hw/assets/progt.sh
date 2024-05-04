#!/bin/bash
#
# progT - Progtest Tinker
#
#   This script assembles multiple source header and implementaion files
#   into one, that is acceptable by ProgTest.
#
# HOW TO USE:
#
#   Put delimiters into your code, so progTinker knows what to throw out
#   and what to assemble into one source file.
#   The progtest.cpp source file will be tinkered (generated).
#
# DEPENDENCIES:
#
#   Using with Makefile progTinker expects src.cpp file form
#   the ProgTest in the ./assets/ directory.
#   Otherwise it should be put with the other source files
#   if you want to run it using ./progt.sh
#
# DELIMITERS:
#
#   PROGT_BEGIN
#
#     All lines here will be deleted, and won't be
#     assembled into one source file.
#
#     Using these delimiters specify what pieces of code should
#     not be in the progt.cpp file.
#
#   PROGT_END
#
#   PROGT_INSERT
#     starting from this point all source files will be
#     inserted, placed between text above and
#     under of the PROGT_INSERT delimiter.
#
[ -z "$1" ] && echo "Source file as first argument not specified." && exit
[ -z "$2" ] && echo "Sources folder as second argument not specified." && exit
! [ -e  "$1" ]  && echo "$1 doesn't exist." && exit
[ ! -e  "$1" ]  && echo "$2 doesn't exist." && exit
! [ -f  "$1" ]  && echo "$1 is not a regular file." && exit
[ ! -d  "$2" ]  && echo "$2 is not a directory." && exit
mkdir progtmp

> progtest.cpp
#sed '/#EX_BEGIN/,/#EX_END/d' "$1" > progtmp/s
sed '1,/#IN_SRC/!d'         "$1" | head -n -1 > progtmp/0s
sed '1,/#IN_SRC/ d'         "$1"              > progtmp/1s
for HEADER in "$2"/*.h;
do
	if PRIOR=$(grep -E '#PRIOR_NO' "$HEADER" );
	then
				cat "$HEADER" >> progtmp/h
	elif	PRIOR=$(grep -E '#PRIOR_' "$HEADER" )
	then
		#echo "$HEADER has priority $PRIOR"
		PRIOR=$( echo "$PRIOR" | grep -E -o '_[[:digit:]]*' | grep -o '[[:digit:]]*')
		FILENAME="$PRIOR"h
		#echo "$HEADER has priority $PRIOR with filename $FILENAME"
		[ -e progtmp/"$FILENAME" ] && echo "Header with priority $PRIOR already exists. Add current to end."
		cat "$HEADER" >> progtmp/"$FILENAME"
		#if has .inl include - add
		if grep -E -o -q '#include.*\".*\.inl\"' "$HEADER";
		then
		INLINE=$( grep -E -o '#include.*\".*\.inl\"' "$HEADER" | grep -o '\".*\"' | grep -o '[^"]' | tr -d '\n' )
		cat "$INLINE" >> progtmp/"$FILENAME";
		fi
	else
		echo "$HEADER does not have priority. Insert as last."
		cat "$HEADER" >> progtmp/h
	fi
done

for IMPLEM in "$2"/*.cpp
do cat "$IMPLEM" >> progtmp/c;
done

for FILE in progtmp/*;
do
	sed '/#EX_BEGIN/,/#EX_END/d' "$FILE" > progtmp/tmp;
	sed '/#PRIOR_/d' progtmp/tmp > "$FILE";
done

cat progtmp/0s > progtest.cpp

SORTED=$(ls progtmp | grep -E '.h')
for HEADER in $SORTED;
do
	cat progtmp/"$HEADER" >> progtest.cpp ;
done
cat progtmp/c >> progtest.cpp
cat progtmp/1s >> progtest.cpp

sed -i '/^[[:blank:]]*$/d' progtest.cpp

#sed '/#EX_BEGIN/,/EX_END/d' "$1"          > tmp
#sed '/PROGT_BEGIN/,/PROGT_END/d' "$2"/*.h "$2"/*.cpp  >> progtmp
rm -dr progtmp
