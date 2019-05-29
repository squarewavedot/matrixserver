echo off
cls
echo Cleaning project... 
mingw32-make clean

echo --------------------------------------------------------------------------------
echo Building project... 
mingw32-make

'echo --------------------------------------------------------------------------------
'echo Building test application...
'cd ../../../Test
'del *.o *.exe
'mingw32-make

'echo Copying test application to PWD
'cd ../libMPSSE/Build/Windows
'copy ..\..\..\Test\test.exe .

'echo --------------------------------------------------------------------------------
'echo Running Test Application
'test.exe > output.txt
'start notepad output.txt
'echo --------------------------------------------------------------------------------
