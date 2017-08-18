/*
 *  dumpvdl2 - a VDL Mode 2 message decoder and protocol analyzer
 *
 *  Copyright (c) 2017 Tomasz Lemiech <szpajder@gmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdint.h>
#include "dumpvdl2.h"
#define RTL_BUFSIZE 320000
#define RTL_BUFCNT 15
#define RTL_OVERSAMPLE 10
#define RTL_RATE (SYMBOL_RATE * SPS * RTL_OVERSAMPLE)
// rtl.c
void rtl_init(vdl2_state_t *ctx, char *dev, int freq, float gain, int correction);
void rtl_cancel();
