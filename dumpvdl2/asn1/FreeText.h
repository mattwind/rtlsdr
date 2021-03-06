/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "PMCPDLCMessageSetVersion1"
 * 	found in "atn-cpdlc.asn1"
 * 	`asn1c -fcompound-names -gen-PER`
 */

#ifndef	_FreeText_H_
#define	_FreeText_H_


#include <asn_application.h>

/* Including external dependencies */
#include <IA5String.h>

#ifdef __cplusplus
extern "C" {
#endif

/* FreeText */
typedef IA5String_t	 FreeText_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_FreeText;
asn_struct_free_f FreeText_free;
asn_struct_print_f FreeText_print;
asn_constr_check_f FreeText_constraint;
ber_type_decoder_f FreeText_decode_ber;
der_type_encoder_f FreeText_encode_der;
xer_type_decoder_f FreeText_decode_xer;
xer_type_encoder_f FreeText_encode_xer;
per_type_decoder_f FreeText_decode_uper;
per_type_encoder_f FreeText_encode_uper;

#ifdef __cplusplus
}
#endif

#endif	/* _FreeText_H_ */
#include <asn_internal.h>
