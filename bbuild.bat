rem @echo off
mkdir build
pushd build

rem Win32
set k32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\kernel32.lib"
set u32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\user32.lib"
set g32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\gdi32.lib"
set s32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\shell32.lib"

rem program
set root=C:\devel
set inc=/I %root%\glfw\include\ /I %root%\glad\include\
set mlibs=%root%\glfw\build\src\Debug\glfw3.lib

cl /nologo /MDd /c /nologo %inc% ..\source\main.c %root%\glad\src\glad.c
link /nologo %root%\gfx\build\main.obj %root%\gfx\build\glad.obj %mlibs% %k32% %u32% %g32% %s32%  
popd build
