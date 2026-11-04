#ifndef Q4_NV_REVIEW_H
#define Q4_NV_REVIEW_H
#include "proof_core.h"
#include <tbs.h>
#define Q4_NV_INDEX 0x01804143U
/* Borrowed original TP_OWNER only; no context creation/close, secret or new owner. */
typedef TBS_RESULT (WINAPI *Q4_TBS_SUBMIT)(TBS_HCONTEXT,TBS_COMMAND_LOCALITY,TBS_COMMAND_PRIORITY,PCBYTE,UINT32,PBYTE,PUINT32);
unsigned long q4_tbs_public_only(Q4_TBS_SUBMIT api,void *original,const unsigned char *command,unsigned n,unsigned char *response,unsigned *rn);
int q4_nv_public(TP_OWNER *o,int written,unsigned char area[14],unsigned char name[34]);
int q4_nv_read_packet(TP_OWNER *o,unsigned session,const unsigned char key[32],const unsigned char auth[32],const unsigned char name[34],const unsigned char caller[32],const unsigned char previous[32],unsigned char out[99]);
int q4_nv_read_result(TP_OWNER *o,const unsigned char *packet,unsigned n,const unsigned char key[32],const unsigned char auth[32],const unsigned char caller[32],ULONGLONG *value);
#endif
