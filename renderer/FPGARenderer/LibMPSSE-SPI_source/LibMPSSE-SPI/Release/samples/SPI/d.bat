echo off
cls
del *.o *.exe *.dll output.txt

echo --------------------------------------------------------------------------------
echo Building libMPSSE ...
echo ----------------------------------------
del/q ..\..\lib\windows\i386\libMPSSE.a
del/q ..\..\lib\windows\i386\libMPSSE.dll
pushd ..\..\..\libMPSSE\Build\Windows
mingw32-make clean
mingw32-make
copy/y libMPSSE.a ..\..\..\Release\lib\windows\i386
copy/y libMPSSE.dll ..\..\..\Release\lib\windows\i386
popd

set input=1
echo ----------------------------------------
if "%input%"=="1" (goto label1)
if "%input%"=="2" (goto label2)
if "%input%"=="3" (goto label1)
goto end

:label1
echo Building SPI sample (static library)
echo ----------------------------------------
cd SPI
del/q libMPSSE.a
del/q libMPSSE.dll
del/q sample-static.exe
copy ..\..\..\include\libMPSSE_spi.h .
copy ..\..\..\include\windows\ftd2xx.h .
copy ..\..\..\lib\windows\i386\*.* .
echo on
gcc -c sample-static.c -o sample-static.o
gcc -o sample-static.exe sample-static.o -L. -lMPSSE
dir *.exe
echo Running the application ...
sample-static
echo off
cd ..
if "%input%"=="1" (goto end)
pause

:label2
echo Building SPI sample (dynamic library)
echo ----------------------------------------
cd SPI
del/q libMPSSE.a
del/q libMPSSE.dll
del/q sample-dynamic.exe
copy ..\..\..\include\libMPSSE_spi.h .
copy ..\..\..\include\windows\ftd2xx.h .
copy ..\..\..\lib\windows\i386\*.* .
echo on
gcc sample-dynamic.c -o sample-dynamic.exe
dir *.exe
echo Running the application ...
sample-dynamic
echo off
cd ..
if "%input%"=="2" (goto end)
REM pause

:end
pause