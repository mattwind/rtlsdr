/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "PMCPDLCMessageSetVersion1"
 * 	found in "atn-cpdlc.asn1"
 * 	`asn1c -fcompound-names -gen-PER`
 */

#ifndef	_TimeToFromPosition_H_
#define	_TimeToFromPosition_H_


#include <asn_application.h>

/* Including external dependencies */
#include "Time.h"
#include "ToFrom.h"
#include "Position.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* TimeToFromPosition */
typedef struct TimeToFromPosition {
	Time_t	 time;
	ToFrom_t	 tofrom;
	Position_t	 position;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} TimeToFromPosition_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_TimeToFromPosition;

#ifdef __cplusplus
}
#endif

#endif	/* _TimeToFromPosition_H_ */
#include <asn_internal.h>
