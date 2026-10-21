#ifndef PAPACC_ISSUER_CODEC_H
#define PAPACC_ISSUER_CODEC_H
#include "issuer_core.h"
#define PAPACC_ISSUER_IMAGE_BYTES 8192U
#define PAPACC_ISSUER_IMAGE_HEADER 256U
#define PAPACC_ISSUER_EVENT_FIXED 356U
/* Private format1 inside ROOT06. Hash function is SHA256 in real adapters;
 * test hashes cannot establish native integrity/authority. No proof flags or
 * operational epoch are reconstructed from this image. */
typedef int (*PAPACC_ISSUER_HASH)(void *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_U8[32]);
PAPACC_ISSUER_RESULT papacc_issuer_image_init(PAPACC_U8 *,const PAPACC_ISSUER_VIEW *,const PAPACC_U8[32],PAPACC_ISSUER_HASH,void *);
PAPACC_ISSUER_RESULT papacc_issuer_image_validate(const PAPACC_U8 *,PAPACC_ISSUER_VIEW *,PAPACC_ISSUER_HASH,void *);
PAPACC_ISSUER_RESULT papacc_issuer_image_find(const PAPACC_U8 *,const PAPACC_U8[16],PAPACC_ISSUER_RECORD *,PAPACC_ISSUER_HASH,void *);
PAPACC_ISSUER_RESULT papacc_issuer_image_append(PAPACC_U8 *,const PAPACC_ISSUER_RECORD *,const PAPACC_U8 *,PAPACC_SIZE,PAPACC_ISSUER_HASH,void *);
PAPACC_ISSUER_RESULT papacc_issuer_image_certificate(const PAPACC_U8 *,const PAPACC_U8[16],PAPACC_U8 *,PAPACC_SIZE,PAPACC_SIZE *,PAPACC_ISSUER_HASH,void *);
#endif