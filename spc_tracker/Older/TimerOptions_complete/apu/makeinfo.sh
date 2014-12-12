#!/bin/bash

for file in *.s; do
	echo "$file" > /tmp/filename
	objname=`sed s/"\.s"/"\.o"/ /tmp/filename`
	wla-spc700 -Mo $file $objname
done

