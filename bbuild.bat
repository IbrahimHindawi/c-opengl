@echo off
mkdir build
pushd build

rem Win32
set k32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\kernel32.lib"
set u32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\user32.lib"
set g32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\gdi32.lib"
set s32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\shell32.lib"

rem paths
set source=..\source
set thirdparty=..\thirdparty
set libroot=C:\devel

rem program
set inc=/I %libroot%\glfw\include /I %thirdparty%\glad\include /I %thirdparty%\stb
set slib=%libroot%\glfw\build\src\Debug\glfw3.lib
set cfiles=%thirdparty%\glad\src\glad.c %source%\main.c %source%\shader.c 
set ofiles=glad.obj main.obj shader.obj

echo:
echo compile...
cl /Zi /nologo /MDd /c /nologo %inc% %cfiles%

echo:
echo link...
link /nologo /out:main.exe %ofiles% %slib% %k32% %u32% %g32% %s32%  
popd build
