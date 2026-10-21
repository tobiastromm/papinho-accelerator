#ifndef PAPACC_ISSUER_TRANSACTION_H
#define PAPACC_ISSUER_TRANSACTION_H
/* Private nonserialized outcomes. No platform types, epoch or authority.
 * PENDING/UNKNOWN never mean cancellation or failure without effects. */
typedef enum PAPACC_ISSUER_CAS_RESULT {
 PAPACC_COMMIT_IDLE=0,PAPACC_COMMIT_PENDING,PAPACC_CAS_SUCCESS,
 PAPACC_CAS_CONFLICT,PAPACC_CAS_UNKNOWN,PAPACC_COMMIT_FAILED
} PAPACC_ISSUER_CAS_RESULT;
#endif
