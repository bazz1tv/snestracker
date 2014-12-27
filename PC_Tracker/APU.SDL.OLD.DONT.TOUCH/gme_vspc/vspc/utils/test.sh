#!/bin/bash
#


if [ $# -ne 2 ]
then
	echo "Syntax: ./test spc_dir pattern"
	echo
	echo "This script does this:"
	echo "1- Copies all the files in the first directory into a temporary"
	echo "   directory."
	echo "2- Run applymask32 on each spc file. (You should have run vspcplay"
	echo "   on them to generate the .msk files)"
	echo "3- Delete the mask files"
	echo "4- Build test.7z from the modified files"

	exit
fi


mkdir tmpdir
cp $1/* tmpdir

find tmpdir -iname "*.spc" -exec ./applymask32 {} $2 \;
rm tmpdir/*.msk
rm test.7z

7z a test.7z tmpdir/*

rm tmpdir/*
rmdir tmpdir

ls -l test.7z

