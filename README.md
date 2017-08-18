# RTLSDR

Patched to work with ARM devices, tested on Raspberry Pi or Beaglebone Black running Debian Wheezy.

### Checkout all submodules

`git submodule update --init --recursive`

## Compile rtl-sdr 

```
cd rtl-sdr/
mkdir build
cd build
cmake ../
make
sudo make install
sudo cp ../rtl-sdr.rules /etc/udev/rules.d/rtl-sdr.rules
sudo ldconfig
```

### Blacklist

`sudo vi /etc/modprobe.d/blacklist-rtl.conf`

Add these three lines

```
blacklist dvb_usb_rtl28xxu
blacklist rtl2832
blacklist rtl2830
```

Now reboot with dongle attached

## Additional programs

* dsd
* dump1090
* dumpvdl2
* mbelib
* multimon-ng

# Issues

#### Error: symbol lookup error undefined symbol: rtlsdr set bias tee

Removing librtlsdr0 fixed this issue.

`sudo apt-get remove librtlsdr0`
