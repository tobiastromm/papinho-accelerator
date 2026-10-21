#ifndef PAPACC_CONFIGURATION_PENDING_WIN32_H
#define PAPACC_CONFIGURATION_PENDING_WIN32_H
#include "../../src/issuer/issuer_transaction.h"
/* Embedded scratch in the SAME durable root. Worker owns no root pointer,
 * lock, Session, lease, publication callback or operational authority.
 * Inputs frozen before CreateThread; outputs read only after thread signal.
 * 64KiB reserved stack, one thread, no queue, no retry or forced termination. */
#define PAPACC_PENDING_STACK_BYTES 65536U
typedef struct PAPACC_CONFIG_PENDING {
 unsigned char previous[16384],next[16384],observed[16384];
 char path[MAX_PATH];DWORD previous_bytes,native_error,close_error;
 HANDLE thread,file,transaction;volatile LONG stop;
 PAPACC_ISSUER_CAS_RESULT result;int active,committed,thread_close_attempted;
#ifdef PAPACC_ISSUER_STORAGE_COMPONENT_TEST
 HANDLE test_entered,test_release;int test_unknown;
#endif
} PAPACC_CONFIG_PENDING;
#endif
