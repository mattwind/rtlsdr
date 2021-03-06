/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "PMCPDLCAPDUsVersion1"
 * 	found in "atn-cpdlc.asn1"
 * 	`asn1c -fcompound-names -gen-PER`
 */

#ifndef	_ProtectedStartDownMessage_H_
#define	_ProtectedStartDownMessage_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ProtectedMode.h"
#include "ProtectedDownlinkMessage.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ProtectedStartDownMessage */
typedef struct ProtectedStartDownMessage {
	ProtectedMode_t	 mode	/* DEFAULT 0 */;
	ProtectedDownlinkMessage_t	 startDownlinkMessage;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtectedStartDownMessage_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ProtectedStartDownMessage;

#ifdef __cplusplus
}
#endif

#endif	/* _ProtectedStartDownMessage_H_ */
#include <asn_internal.h>
