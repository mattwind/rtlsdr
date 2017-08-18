/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "PMCPDLCMessageSetVersion1"
 * 	found in "atn-cpdlc.asn1"
 * 	`asn1c -fcompound-names -gen-PER`
 */

#ifndef	_Distance_H_
#define	_Distance_H_


#include <asn_application.h>

/* Including external dependencies */
#include "DistanceNm.h"
#include "DistanceKm.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Distance_PR {
	Distance_PR_NOTHING,	/* No components present */
	Distance_PR_distanceNm,
	Distance_PR_distanceKm
} Distance_PR;

/* Distance */
typedef struct Distance {
	Distance_PR present;
	union Distance_u {
		DistanceNm_t	 distanceNm;
		DistanceKm_t	 distanceKm;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Distance_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Distance;

#ifdef __cplusplus
}
#endif

#endif	/* _Distance_H_ */
#include <asn_internal.h>
