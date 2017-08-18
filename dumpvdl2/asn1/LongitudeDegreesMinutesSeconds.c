/*
 * Generated by asn1c-0.9.28 (http://lionet.info/asn1c)
 * From ASN.1 module "PMCPDLCMessageSetVersion1"
 * 	found in "atn-cpdlc.asn1"
 * 	`asn1c -fcompound-names -gen-PER`
 */

#include "LongitudeDegreesMinutesSeconds.h"

static asn_TYPE_member_t asn_MBR_LongitudeDegreesMinutesSeconds_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct LongitudeDegreesMinutesSeconds, longitudeWholeDegrees),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_LongitudeWholeDegrees,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"longitudeWholeDegrees"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct LongitudeDegreesMinutesSeconds, latLonWholeMinutes),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_LatLonWholeMinutes,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"latLonWholeMinutes"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct LongitudeDegreesMinutesSeconds, secondsLatLon),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_SecondsLatLon,
		0,	/* Defer constraints checking to the member type */
		0,	/* No PER visible constraints */
		0,
		"secondsLatLon"
		},
};
static const ber_tlv_tag_t asn_DEF_LongitudeDegreesMinutesSeconds_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_LongitudeDegreesMinutesSeconds_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 0, 0, 2 }, /* longitudeWholeDegrees */
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 1, -1, 1 }, /* latLonWholeMinutes */
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 2, -2, 0 } /* secondsLatLon */
};
static asn_SEQUENCE_specifics_t asn_SPC_LongitudeDegreesMinutesSeconds_specs_1 = {
	sizeof(struct LongitudeDegreesMinutesSeconds),
	offsetof(struct LongitudeDegreesMinutesSeconds, _asn_ctx),
	asn_MAP_LongitudeDegreesMinutesSeconds_tag2el_1,
	3,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* Start extensions */
	-1	/* Stop extensions */
};
asn_TYPE_descriptor_t asn_DEF_LongitudeDegreesMinutesSeconds = {
	"LongitudeDegreesMinutesSeconds",
	"LongitudeDegreesMinutesSeconds",
	SEQUENCE_free,
	SEQUENCE_print,
	SEQUENCE_constraint,
	SEQUENCE_decode_ber,
	SEQUENCE_encode_der,
	SEQUENCE_decode_xer,
	SEQUENCE_encode_xer,
	SEQUENCE_decode_uper,
	SEQUENCE_encode_uper,
	0,	/* Use generic outmost tag fetcher */
	asn_DEF_LongitudeDegreesMinutesSeconds_tags_1,
	sizeof(asn_DEF_LongitudeDegreesMinutesSeconds_tags_1)
		/sizeof(asn_DEF_LongitudeDegreesMinutesSeconds_tags_1[0]), /* 1 */
	asn_DEF_LongitudeDegreesMinutesSeconds_tags_1,	/* Same as above */
	sizeof(asn_DEF_LongitudeDegreesMinutesSeconds_tags_1)
		/sizeof(asn_DEF_LongitudeDegreesMinutesSeconds_tags_1[0]), /* 1 */
	0,	/* No PER visible constraints */
	asn_MBR_LongitudeDegreesMinutesSeconds_1,
	3,	/* Elements count */
	&asn_SPC_LongitudeDegreesMinutesSeconds_specs_1	/* Additional specs */
};

