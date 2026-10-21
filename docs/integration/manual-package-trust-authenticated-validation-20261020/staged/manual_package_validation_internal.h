#ifndef PAPACC_MANUAL_PACKAGE_VALIDATION_INTERNAL_H
#define PAPACC_MANUAL_PACKAGE_VALIDATION_INTERNAL_H
#include "manual_package_validation.h"
/* PRIVATE stage port: product caller cannot supply or select this table.
 * OFFLINE_TEST table is not an authority adapter and never yields VALIDATED.
 * Any future factual source binding needs separate exact review. */
enum {MP_AUTH_STEP_OK=1,MP_AUTH_STEP_INVALID=-101,MP_AUTH_STEP_SOURCE_ABSENT=-102,
 MP_AUTH_STEP_UNCERTAIN=-103,MP_AUTH_STEP_REVOKED=-104,MP_AUTH_STEP_UNKNOWN=-105,
 MP_AUTH_STEP_EXPIRED=-106,MP_AUTH_STEP_WRONG_PURPOSE=-107,MP_AUTH_STEP_PROVIDER=-108};
typedef struct MP_AUTH_PORT {
 int (*step)(void *,MP_AUTH_STAGE,MP_AUTH_OPERATION *,const MP_ENVELOPE *,const MP_PAYLOAD *);
 void (*wipe)(void *,unsigned char *,size_t);
 /* All native work, source release and borrowers factually quiescent? */
 int (*quiescent)(void *);
} MP_AUTH_PORT;
#ifndef MP_PACKAGE_VALIDATION_OFFLINE_TEST
extern const MP_AUTH_PORT mp_auth_native_port;
#else
MP_AUTH_RESULT mp_auth_offline_test_run(MP_AUTH_OPERATION *,const MP_AUTH_INPUT *,const MP_AUTH_PORT *,void *);
#endif
#endif
