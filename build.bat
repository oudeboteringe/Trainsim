REM @echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64     
REM call “C:\Program Files (x86)\Microsoft Visual C++ Build Tools\vcbuildtools.bat” x64
echo Current directory:
echo %CD%
set compilerflags=/Od /Zi /EHsc /I./Pugixml/src
set linkerflags=/OUT:trainsim.exe
cl.exe %compilerflags% Pugixml\src\pugixml.cpp Trainsim\Trainsim.cpp Trainsim\leg.cpp Trainsim\Network.cpp Trainsim\Route.cpp Trainsim\Station.cpp Trainsim\stdafx.cpp Trainsim\Train.cpp /link %linkerflags%
