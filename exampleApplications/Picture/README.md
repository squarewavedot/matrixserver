#USB Storage automount on raspbian lite
1. Install the package:  
sudo apt-get install usbmount`
2. Make sure it works in Stretch by changing `MountFlags=slave` to `MountFlags=shared` here:  
`sudo nano /lib/systemd/system/systemd-udevd.service`
3. Reboot and it works!