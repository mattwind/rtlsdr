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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "dumpvdl2.h"
#include "tlv.h"

void *xcalloc(size_t nmemb, size_t size, const char *file, const int line, const char *func) {
	void *ptr = calloc(nmemb, size);
	if(ptr == NULL) {
		fprintf(stderr, "%s:%d: %s(): calloc(%zu, %zu) failed: %s\n",
			file, line, func, nmemb, size, strerror(errno));
		_exit(1);
	}
	return ptr;
}

void *xrealloc(void *ptr, size_t size, const char *file, const int line, const char *func) {
	ptr = realloc(ptr, size);
	if(ptr == NULL) {
		fprintf(stderr, "%s:%d: %s(): realloc(%zu) failed: %s\n",
			file, line, func, size, strerror(errno));
		_exit(1);
	}
	return ptr;
}

char *fmt_hexstring(uint8_t *data, uint16_t len) {
	static const char hex[] = "0123456789abcdef";
	char *buf = NULL;
	if(data == NULL) return strdup("<undef>");
	if(len == 0) return strdup("none");
	buf = XCALLOC(3 * len + 1, sizeof(char));
	char *ptr = buf;
	for(uint16_t i = 0; i < len; i++) {
		*ptr++ = hex[((data[i] >> 4) & 0xf)];
		*ptr++ = hex[data[i] & 0xf];
		*ptr++ = ' ';
	}
	if(ptr != buf)
		ptr[-1] = '\0';		// trim trailing space
	return buf;
}

char *fmt_hexstring_with_ascii(uint8_t *data, uint16_t len) {
	if(data == NULL) return strdup("<undef>");
	if(len == 0) return strdup("none");
	char *buf = fmt_hexstring(data, len);
	int buflen = strlen(buf);
	buf = XREALLOC(buf, buflen + len + 4); // add tab, quotes, ascii dump and '\0'
	char *ptr = buf + buflen;
	*ptr++ = '\t';
	*ptr++ = '"';
	for(uint16_t i = 0; i < len; i++) {
		if(data[i] < 32 || data[i] > 126)	// replace non-printable chars
			*ptr++ = '.';
		else
			*ptr++ = data[i];
	}
	*ptr++ = '"';
	*ptr = '\0';
	return buf;
}

char *fmt_bitfield(uint8_t val, const dict *d) {
	if(val == 0) return strdup("none");
	char *buf = XCALLOC(256, sizeof(char));
	for(dict *ptr = (dict *)d; ptr->val != NULL; ptr++) {
		if((val & ptr->id) == ptr->id) {
			strcat(buf, (char *)ptr->val);
			strcat(buf, ", ");
		}
	}
	int slen = strlen(buf);
	if(slen == 0)
		strcat(buf, "none");
	else
		buf[slen-2] = '\0';	// throw out trailing delimiter
	return buf;
}
