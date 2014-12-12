@echo off
goto continue

Notes:
to compile EXAMPLE.ASM ---> EXAMPLE.FIG
from the command prompt type:  wla EXAMPLE


:continue
echo [objects] > temp.prj
echo %1.obj >> temp.prj

echo on
wla-65816 -o %1.asm %1.obj
wlalink -vr temp.prj %1.fig
@echo off

del %1.obj
del temp.prj