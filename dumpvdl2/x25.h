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
#ifndef _X25_H
#define _X25_H
#include <endian.h>
#include <stdint.h>
#include "tlv.h"
#define X25_MIN_LEN		3
#define GFI_X25_MOD8		1
#define MAX_X25_ADDR_LEN	8	// bytes
#define MAX_X25_EXT_ADDR_LEN	20	// bytes
#define X25_SNDCF_ID		0xc1
#define X25_SNDCF_VERSION	1
#define MIN_X25_SNDCF_LEN	4

#define SN_PROTO_CLNP_INIT_COMPRESSED	0x01
#define	SN_PROTO_CLNP			0x81
#define	SN_PROTO_ESIS			0x82
#define	SN_PROTO_IDRP			0x85
#define	SN_PROTO_COTP			0xFF	// dummy value

/*
 * X.25 packet identifiers
 * (ITU-T Rec. X.25, Tab. 5-2/X.25)
 * INTERRUPT, INTERRUPT_CONFIRM and RNR are not listed,
 * because they are not supported in VDL2 (ICAO Doc 9776 6.3.4)
 */
#define X25_CALL_REQUEST	0x0b
#define X25_CALL_ACCEPTED	0x0f
#define X25_CLEAR_REQUEST	0x13
#define X25_CLEAR_CONFIRM	0x17
#define X25_DATA		0x00
#define X25_RR			0x01
#define X25_REJ			0x09
#define X25_RESET_REQUEST	0x1b
#define X25_RESET_CONFIRM	0x1f
#define X25_RESTART_REQUEST	0xfb
#define X25_RESTART_CONFIRM	0xff
#define X25_DIAG		0xf1

typedef struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN
	uint8_t chan_group:4;
	uint8_t gfi:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
	uint8_t gfi:4;
	uint8_t chan_group:4;
#else
#error Unsupported endianness
#endif
	uint8_t chan_num;
	union {
		uint8_t val;
		struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN
			uint8_t pad:1;
			uint8_t sseq:3;
			uint8_t more:1;
			uint8_t rseq:3;
#elif __BYTE_ORDER == __BIG_ENDIAN
			uint8_t rseq:3;
			uint8_t more:1;
			uint8_t sseq:3;
			uint8_t pad:1;
#endif
		} data;
	} type;
} x25_hdr_t;

typedef struct {
	uint8_t addr[MAX_X25_ADDR_LEN];
	uint8_t len;		// nibbles
} x25_addr_t;

typedef struct {
	x25_hdr_t *hdr;
	uint8_t type;
	uint8_t addr_block_present;
	x25_addr_t calling, called;
	tlv_list_t *facilities;
	uint8_t compression;
	uint8_t clr_cause;
	uint8_t diag_code;
	uint8_t more_data;
	uint8_t rseq, sseq;
	uint8_t proto;
	uint8_t data_valid;
	void *data;
	uint32_t datalen;
} x25_pkt_t;

// x25.c
x25_pkt_t *parse_x25(uint8_t *buf, uint32_t len, uint32_t *msg_type);
void output_x25(x25_pkt_t *pkt);
#endif // !_X25_H
