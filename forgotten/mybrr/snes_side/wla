#!/bin/sh

echo '[objects]' > temp
echo $1.obj >> temp

wla-65816 -o $1.asm $1.obj
wlalink -vr temp $1.smc

rm $1.obj
rm temp
