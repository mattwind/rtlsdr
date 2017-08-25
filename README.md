# RTLSDR

Patched to work with ARM devices, tested on Raspberry Pi 3 and Beaglebone Black Rev C, running Debian Wheezy.

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

## Programs

* dsd
* dump1090
* dumpvdl2
* gpredict
* mbelib
* multimon-ng

# Additional Resources

* [Signal Identification](http://www.sigidwiki.com/wiki/Signal_Identification_Guide)
* [Reddit Community](https://www.reddit.com/r/RTLSDR/)
* [RTL-SDR Blog](http://www.rtl-sdr.com)
* [rtl_power](http://kmkeen.com/rtl-power/)

# rtl_power

Send band survey to another computer over SSH

`rtl_power -f 118M:137M:8k -g 50 -i 10 -e 5h | ssh user@172.16.0.10 "cat > /home/user/airband.csv"`

# Issues

#### Error: symbol lookup error undefined symbol: rtlsdr set bias tee

Removing librtlsdr0 fixed this issue.

`sudo apt-get remove librtlsdr0`
