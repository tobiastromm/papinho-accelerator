#ifndef ST20_DELIVERY_H
#define ST20_DELIVERY_H
#include "proof_core.h"
#include <tbs.h>
/* Isolated borrowed scratch attached to existing TP_OWNER lifetime. No authority,
 * context acquisition/close, secret persistence or new owner is implemented. */
#define ST20_CAPACITY_LIMIT 64U
typedef TBS_RESULT (WINAPI *ST20_GET)(TBS_HCONTEXT,TBS_OWNERAUTH_TYPE,PBYTE,PUINT32);
typedef BOOL (WINAPI *ST20_LOCK)(LPVOID,SIZE_T);
typedef BOOL (*ST20_CLEAR)(unsigned char *,unsigned);
BOOL st20_secure_clear(unsigned char *,unsigned);
typedef struct ST20_DELIVERY ST20_DELIVERY;
typedef int (*ST20_GUARD)(TP_OWNER *,const ST20_DELIVERY *,const char *);
/* Synchronous borrowed use only:0 reject,1 known complete,2 UNKNOWN borrow. */
typedef int (*ST20_USE)(TP_OWNER *,const unsigned char *,unsigned);
struct ST20_DELIVERY {
 TP_OWNER *owner;void *original_context;
 unsigned char *buffer;unsigned capacity,length;
 volatile LONG used;
 unsigned locked,retained,queried,delivered,wiped;
 void *guard_context; /* borrowed binding, same retained lifetime */
 ST20_GET used_source; /* engine snapshot; never a new authority */
};
typedef struct ST20_API {ST20_GET get;ST20_LOCK lock,unlock;ST20_GUARD guard;ST20_USE use;ST20_CLEAR clear;} ST20_API;
int st20_deliver(ST20_DELIVERY *,const ST20_API *);
#endif
