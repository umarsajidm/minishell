#! /usr/bin/env bash

echo "PROGRAM = '$program_name'"
make
if [ $? -ne 0 ]; then  exit 1
fi

program_name=$(grep -E '^NAME' Makefile | awk -F'=' '{print $NF}' | awk '{print $NF}')

#without any flags

if [ -z "$1" ]; then
	./"$program_name"
	exit 0
fi

#with classic valgrind

if [ "$1" == "l" ]; then
	valgrind --supression=readline.supp ./"$program_name"

#with valgrind --leak-check=ful

elif [ "$1" == "lf" ]; then
	valgrind --leak-check=full --supression=readline.supp ./"$program_name"

#with valgrind file descriptors

elif [ "$1" == "fd" ]; then
	valgrind --track-fd=yes --supression=readline.supp ./"$program_name"

#with valgrind memleak and file descriptors

elif [ "$1" == "fdl" ]; then
	valgrind --leak-check=full --track-fds=yes --supression=readline.supp ./"$program_name"


else
	echo "usage; ./run.sh [l | lf | fdl]"
fi

