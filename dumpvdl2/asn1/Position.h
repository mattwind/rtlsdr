/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "PMCPDLCMessageSetVersion1"
 * 	found in "atn-cpdlc.asn1"
 * 	`asn1c -fcompound-names -gen-PER`
 */

#ifndef	_Position_H_
#define	_Position_H_


#include <asn_application.h>

/* Including external dependencies */
#include "FixName.h"
#include "Navaid.h"
#include "Airport.h"
#include "LatitudeLongitude.h"
#include "PlaceBearingDistance.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum Position_PR {
	Position_PR_NOTHING,	/* No components present */
	Position_PR_fixName,
	Position_PR_navaid,
	Position_PR_airport,
	Position_PR_latitudeLongitude,
	Position_PR_placeBearingDistance
} Position_PR;

/* Position */
typedef struct Position {
	Position_PR present;
	union Position_u {
		FixName_t	 fixName;
		Navaid_t	 navaid;
		Airport_t	 airport;
		LatitudeLongitude_t	 latitudeLongitude;
		PlaceBearingDistance_t	 placeBearingDistance;
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} Position_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_Position;

#ifdef __cplusplus
}
#endif

#endif	/* _Position_H_ */
#include <asn_internal.h>
