echo off
echo ----------------------------------------
echo Please select the sample application to build and run
echo 1. SPI-Static
echo 2. SPI-Dynamic
echo 3. Test all

set /p input=
echo Selected input is %input%
echo ----------------------------------------
if "%input%"=="1" (goto label1)
if "%input%"=="2" (goto label2)
if "%input%"=="3" (goto label1)
goto end

:label1
echo Building SPI sample (static library)
echo ----------------------------------------
cd SPI
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
copy ..\..\..\include\libMPSSE_spi.h .
copy ..\..\..\include\windows\ftd2xx.h .
copy ..\..\..\lib\windows\i386\*.* .
gcc sample-dynamic.c -o sample-dynamic.exe
dir *.exe
echo Running the application ...
sample-dynamic
echo off
cd ..
if "%input%"=="2" (goto end)

:end
pause