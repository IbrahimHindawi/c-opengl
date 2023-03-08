@echo off
mkdir build
pushd build

rem rem Win32
rem set k32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\kernel32.lib"
rem set u32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\user32.lib"
rem set g32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\gdi32.lib"
rem set s32="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x64\shell32.lib"
rem 
rem rem paths
rem set source=..\source
rem set thirdparty=..\thirdparty
rem set libroot=C:\devel
rem 
rem rem program
rem set slib=%libroot%\glfw\build\src\Debug\glfw3.lib %libroot%\cglm\win\x64\Debug\cglm.lib
rem set cfiles=%thirdparty%\glad\src\glad.c %source%\main.c %source%\shader.c %source%\fileops.c 
rem 
rem rem set ofiles=glad.obj main.obj shader.obj fileops.obj
rem set ofiles=glad.obj main.obj shader.obj fileops.obj
rem 
rem echo:
rem echo compile...
rem 
rem rem clang
rem rem set clinc=-I %libroot%\glfw\include -I %libroot%\cglm\include -I %thirdparty%\glad\include -I %thirdparty%\stb
rem rem clang -c %cfiles% %clinc% -MJ compile_commands.json
rem 
rem rem msvc++
rem set inc=/I %libroot%\glfw\include /I %libroot%\cglm\include /I %thirdparty%\glad\include /I %thirdparty%\stb
rem cl /Zi /MDd /c /nologo %inc% %cfiles%
rem 
rem echo:
rem echo link...
rem link /nologo /FORCE:MULTIPLE /out:main.exe %ofiles% %slib% %k32% %u32% %g32% %s32%  

rem msbuild c-opengl.sln

cmake ..
powershell -Command ..\clang-build.ps1 -export-jsondb

popd build
