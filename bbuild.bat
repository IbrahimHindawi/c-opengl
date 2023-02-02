rem @echo off
mkdir build
pushd build

rem Win32
set k32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\kernel32.lib"
set u32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\user32.lib"
set g32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\gdi32.lib"
set s32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\shell32.lib"

rem paths
set libroot=C:\devel
set source=..\source

rem program
set inc=/I %libroot%\glfw\include\ /I %libroot%\glad\include\
set slib=%libroot%\glfw\build\src\Debug\glfw3.lib
set cfiles=%libroot%\glad\src\glad.c %source%\main.c 
set ofiles=glad.obj main.obj

cl /nologo /MDd /c /nologo %inc% %cfiles%
link /nologo /out:main.exe %ofiles% %slib% %k32% %u32% %g32% %s32%  
popd build
