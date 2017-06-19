#!/bin/bash

for file in *.s; do
	echo "$file" > /tmp/filename
	objname=`sed s/"\.s"/"\.o"/ /tmp/filename`
	wla-65816 -Mo $file $objname
	printf '\t\t$(CC) $(CFLAGS) $< $(OBJDIR)/$@\n'
done

