#!/bin/bash

getArch()
{
#  STR=getconf LONG_BIT
  STR=64
  if [ $STR -eq 64 ]; then
    ARCH=x86_64
  else
# default is 32bit
    ARCH=i386
  fi
  echo "------------------------------------------------"
  echo "in function getArch"
  echo STR = $STR
  echo ARCH = $ARCH
  echo "------------------------------------------------"
}

SPI-Static()
{
cd SPI
rm -rf libMPSSE.so
rm -rf libMPSSE.a
rm -rf sample-dynamic.o
rm -rf sample-static.o
cp -f ../../../include/libMPSSE_spi.h .
cp -f ../../../include/linux/*.h .
cp -f ../../../lib/linux/i386/* .
echo ------------------------------------------------
echo Building sample by linking to static library and running it
gcc -g -I. -o sample-static.o sample-static.c libMPSSE.a -ldl
./sample-static.o
cd ..
}

SPI-Dynamic()
{
cd SPI
rm -rf libMPSSE.so
rm -rf libMPSSE.a
rm -rf sample-dynamic.o
rm -rf sample-static.o
cp -f ../../../include/libMPSSE_spi.h .
cp -f ../../../include/linux/*.h .
cp -f ../../../lib/linux/i386/* .
echo ------------------------------------------------
echo Building sample by linking to dynamic library and running it
gcc sample-dynamic.c -o sample-dynamic.o -ldl
./sample-dynamic.o
cd ..
}

testAll()
{
	SPI-Static
	echo Press enter to continue
	read
	SPI-Dynamic
}

#Execution starts from here
getArch ;
echo "----------------------------------------"
echo Please select the sample application to build and run
echo 1. SPI-Static
echo 2. SPI-Dynamic
echo 3. Test all
read input
#Select input
    case $input in
	1 ) SPI-Static ;;
	2 ) SPI-Dynamic ;;
	3 ) testAll ;;
	* ) echo "invalid input" ;;
    esac
#Done

