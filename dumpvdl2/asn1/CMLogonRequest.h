/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "CMMessageSetVersion1"
 * 	found in "atn-cm.asn1"
 * 	`asn1c -fcompound-names -gen-PER`
 */

#ifndef	_CMLogonRequest_H_
#define	_CMLogonRequest_H_


#include <asn_application.h>

/* Including external dependencies */
#include "AircraftFlightIdentification.h"
#include "LongTsap.h"
#include "FacilityDesignation.h"
#include "Airport.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct DateTime;
struct AEQualifierVersionAddress;
struct AEQualifierVersion;

/* CMLogonRequest */
typedef struct CMLogonRequest {
	AircraftFlightIdentification_t	 aircraftFlightIdentification;
	LongTsap_t	 cMLongTSAP;
	struct CMLogonRequest__groundInitiatedApplications {
		A_SEQUENCE_OF(struct AEQualifierVersionAddress) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *groundInitiatedApplications;
	struct CMLogonRequest__airOnlyInitiatedApplications {
		A_SEQUENCE_OF(struct AEQualifierVersion) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *airOnlyInitiatedApplications;
	FacilityDesignation_t	*facilityDesignation	/* OPTIONAL */;
	Airport_t	*airportDeparture	/* OPTIONAL */;
	Airport_t	*airportDestination	/* OPTIONAL */;
	struct DateTime	*dateTimeDepartureETD	/* OPTIONAL */;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} CMLogonRequest_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_CMLogonRequest;

#ifdef __cplusplus
}
#endif

/* Referred external types */
#include "DateTime.h"
#include "AEQualifierVersionAddress.h"
#include "AEQualifierVersion.h"

#endif	/* _CMLogonRequest_H_ */
#include <asn_internal.h>
