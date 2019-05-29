D2XX for Linux
--------------

As Linux distributions vary these instructions are a guide to installation 
and use.  FTDI has tested the driver and samples with Ubuntu 12.04 (kernel 
version 3.2) for i386 and x86_64, and Debian 6 'squeeze' (kernel version 
2.6.32) for arm926.

FTDI developed libftd2xx primarily to aid porting Windows applications 
written with D2XX to Linux.  We intend the APIs to behave the same on
Windows and Linux so if you notice any differences, please contact us 
(see http://www.ftdichip.com/FTSupport.htm).

FTDI do not release the source code for libftd2xx.  If you prefer to work
with source code and are starting a project from scratch, consider using
the open-source libFTDI.

libftd2xx uses an unmodified version of libusb 
(http://sourceforge.net/projects/libusb/).  Source code for libusb is 
included in the driver distribution.



Installing the D2XX shared library and static library.
------------------------------------------------------

1.  tar xfvz libftd2xx1.1.12.tar.gz

This unpacks the archive, creating the following directory structure:

    build
        arm926
        i386
        x86_64
    examples
    libusb
    ftd2xx.h
    WinTypes.h

2.  cd build/arm926

3.  sudo -s 
  or, if sudo is not available on your system: 
    su

Promotes you to super-user, with installation privileges.  If you're
already root, then step 3 (and step 7) is not necessary.

4.  cp lib* /usr/local/lib

Copies the libraries to a central location.

5.  chmod 0755 /usr/local/lib/libftd2xx.so.1.1.12

Allows non-root access to the shared object.

6.  ln -sf /usr/local/lib/libftd2xx.so.1.1.12 /usr/local/lib/libftd2xx.so

Creates a symbolic link to the 1.1.12 version of the shared object.

7.  exit

Ends your super-user session.



Building the shared-object examples.
------------------------------------

1.  cd examples

2.  make -B

This builds all the shared-object examples in subdirectories.

With an FTDI device connected to a USB port, try one of the 
examples, e.g. reading EEPROM.

3.  cd EEPROM/read

4.  sudo ./read

If the message "FT_Open failed" appears:
    Perhaps the kernel automatically loaded another driver for the 
    FTDI USB device.

    sudo lsmod

    If "ftdi_sio" is listed:
        Unload it (and its helper module, usbserial), as follows.

        sudo rmmod ftdi_sio
        sudo rmmod usbserial

    Otherwise, it's possible that libftd2xx does not recognise your 
    device's Vendor and Product Identifiers.  Call FT_SetVIDPID before
    calling FT_Open/FT_OpenEx/FT_ListDevices.



Building the static-library example.
------------------------------------

1.  cd examples/static

2.  rm lib*

Cleans out any existing libraries built for another target.

3.  cp /usr/local/lib/libftd2xx.a .

4.  make -B

5.  sudo ./static_link

This example demonstrates writing to, and reading from, a device with
a loop-back connector attached.



The examples show how to call a small subset of the D2XX API.  The full
API is available here:
http://www.ftdichip.com/Support/Documents/ProgramGuides/D2XX_Programmer%27s_Guide(FT_000071).pdf

