#!/bin/bash

freq="-f 152.830M"
#freq="-f 152.830M:153.830M:15k"    # requires sqelch and gain
#gain="-g 20"
#squelch="-l 5"
sample="-s 22050"

rtl_fm $freq $sample $gain $squelch | multimon-ng -t raw -a POCSAG512 -a POCSAG1200 -a POCSAG2400 /dev/stdin
