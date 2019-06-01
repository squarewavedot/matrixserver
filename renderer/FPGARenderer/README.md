#SPIDEV Driver limitations

By default the spidev driver on raspberry pi has a maximum transfer size of 4096 bytes.

To change the default add `spidev.bufsiz=65536` to `/boot/cmdline.txt` and reboot. Where 65536 is the maximum size you want to allow.

Note that /boot/cmdline.txt is a single line. After the above change mine looked like: