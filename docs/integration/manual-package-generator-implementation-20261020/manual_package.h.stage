#ifndef PAPACC_MANUAL_PACKAGE_H
#define PAPACC_MANUAL_PACKAGE_H
#include <stddef.h>
#define MP_INPUT_MAX 131072U
#define MP_ARCHIVE_MAX 49654U
#define MP_OUTER_MAX 51042U
#define MP_MEMBERS 5U
/* DATA_ONLY is never authentication, valid X509, RESOLVED or CURRENT.
 * No crypto provider is selected here. No allocation/I/O/secret generation.
 * Serialized synchronous caller: all input bytes immutable, all outputs
 * disjoint from inputs/each other. Returned slices borrow caller input only;
 * caller holds its existing ownership/lease for their entire use. No reentry.
 * No asynchronous borrower or cleanup/retirement authority is created.
 * Runtime secret integration requires reviewed custody/wipe/credit first. */
typedef enum MP_RESULT { MP_DATA_ONLY=1,MP_INVALID=-1,MP_LIMIT=-2,
 MP_STOPPED=-3,MP_CRYPTO_UNAVAILABLE=-4 } MP_RESULT;
typedef struct MP_SLICE {const unsigned char *data;size_t size;} MP_SLICE;
typedef struct MP_PAYLOAD {MP_SLICE member[MP_MEMBERS];} MP_PAYLOAD;
typedef struct MP_ENVELOPE {MP_SLICE spki,ciphertext,tag,signature;} MP_ENVELOPE;
MP_RESULT mp_payload_inspect(const unsigned char *,size_t,MP_PAYLOAD *,int);
MP_RESULT mp_payload_prepare(const MP_PAYLOAD *,unsigned char *,size_t,size_t *,int);
MP_RESULT mp_envelope_inspect(const unsigned char *,size_t,MP_ENVELOPE *,int);
/* Writes framing of ALREADY supplied protected bytes; does not encrypt/sign.
 * Header supplied by caller includes salt/nonce/SPKI hash, all untrusted DATA.
 * Never produces an operational package or file. */
MP_RESULT mp_envelope_prepare(const unsigned char *,const MP_ENVELOPE *,
 unsigned char *,size_t,size_t *,int);
/* Fail-closed authentication boundary, not a mock crypto implementation.
 * No password is accepted/retained and no private signature is called. */
MP_RESULT mp_package_validate(const unsigned char *,size_t,int);
#endif
