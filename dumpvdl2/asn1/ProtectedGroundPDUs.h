/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "PMCPDLCAPDUsVersion1"
 * 	found in "atn-cpdlc.asn1"
 * 	`asn1c -fcompound-names -gen-PER`
 */

#ifndef	_ProtectedGroundPDUs_H_
#define	_ProtectedGroundPDUs_H_


#include <asn_application.h>

/* Including external dependencies */
#include "PMCPDLCUserAbortReason.h"
#include "PMCPDLCProviderAbortReason.h"
#include "ProtectedUplinkMessage.h"
#include "ATCForwardMessage.h"
#include "ATCForwardResponse.h"
#include <constr_CHOICE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ProtectedGroundPDUs_PR {
	ProtectedGroundPDUs_PR_NOTHING,	/* No components present */
	ProtectedGroundPDUs_PR_abortUser,
	ProtectedGroundPDUs_PR_abortProvider,
	ProtectedGroundPDUs_PR_startup,
	ProtectedGroundPDUs_PR_send,
	ProtectedGroundPDUs_PR_forward,
	ProtectedGroundPDUs_PR_forwardresponse,
	/* Extensions may appear below */
	
} ProtectedGroundPDUs_PR;

/* ProtectedGroundPDUs */
typedef struct ProtectedGroundPDUs {
	ProtectedGroundPDUs_PR present;
	union ProtectedGroundPDUs_u {
		PMCPDLCUserAbortReason_t	 abortUser;
		PMCPDLCProviderAbortReason_t	 abortProvider;
		ProtectedUplinkMessage_t	 startup;
		ProtectedUplinkMessage_t	 send;
		ATCForwardMessage_t	 forward;
		ATCForwardResponse_t	 forwardresponse;
		/*
		 * This type is extensible,
		 * possible extensions are below.
		 */
	} choice;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ProtectedGroundPDUs_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ProtectedGroundPDUs;

#ifdef __cplusplus
}
#endif

#endif	/* _ProtectedGroundPDUs_H_ */
#include <asn_internal.h>
