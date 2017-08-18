# RTLSDR

Patched to work with ARM devices, tested on Raspberry Pi or Beaglebone Black running Debian Wheezy.

### Checkout all submodules

`git submodule update --init --recursive`

## rtl-sdr 

```
cd rtl-sdr/
mkdir build
cd build
cmake ../
make
sudo make install
sudo ldconfig
```

## Programs

* dsd
* dump1090
* dumpvdl2
* mbelib
* multimon-ng

## Decoding

* pocsaq
* mode select
* df formats
* vdl mode 2
