@echo off

set ignoredWarnings= -wd4505 -wd4201 -wd4100 -wd4189
set commonCompilerFlags= -Ddebug=1 -MTd -nologo -Gm- -GR- -EHa- -Od -Oi -WX -W4 -FC -Z7 %ignoredWarnings%
set commonLinkerFlags= -incremental:no -opt:ref user32.lib ole32.lib comdlg32.lib

IF NOT EXIST bin mkdir bin
pushd bin

uicc ..\src\ribbon.xml markup.bin /header:..\src\resource.h /res:ribbon.rc /name:test
windres ribbon.rc ribbon.o
cl %commonCompilerFlags% ..\src\main.cpp /link %commonLinkerFlags% ribbon.o

popd
