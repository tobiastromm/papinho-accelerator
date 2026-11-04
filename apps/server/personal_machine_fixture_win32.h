#ifndef PAPACC_PERSONAL_MACHINE_FIXTURE_WIN32_H
#define PAPACC_PERSONAL_MACHINE_FIXTURE_WIN32_H
#include "server_ca_key_win32.h"
#include "server_store_hash_win32.h"
#define PAPACC_MACHINE_FIXTURE_MANAGED_EXTRA (15U*65536U+65536U) /* SD payload slots + named synchronous frames; opaque overhead UNKNOWN */
typedef struct PAPACC_MACHINE_FIXTURE_INPUT {
 const WCHAR *request_path,*decision_path;size_t known_managed;BYTE pins[5][32]; /* request, decision, executor, SSL, crypto */
} PAPACC_MACHINE_FIXTURE_INPUT;
typedef struct PAPACC_MACHINE_FIXTURE_REFERENCE {
 HANDLE handle;BY_HANDLE_FILE_INFORMATION identity;PSECURITY_DESCRIPTOR sd;DWORD sd_bytes;
 BYTE digest[32];
} PAPACC_MACHINE_FIXTURE_REFERENCE;
/* K1/K2 substate of the existing native owner. This is custody/identity,
 * not an administrative grant or an operational fixture constructor. */
typedef struct PAPACC_MACHINE_FIXTURE_CONTROL {
 struct PAPACC_MACHINE_FIXTURE_CONTROL *self;const void *owner;
 DWORD thread,phase,first,stage;volatile LONG used;
 PAPACC_CA_KEY key;PAPACC_STORE_HASH hash;
 NCRYPT_KEY_HANDLE original_key;NCRYPT_PROV_HANDLE original_provider;
 BYTE public_blob[283],public_id[32];DWORD public_bytes;
 const void *source_owner;DWORD source_thread,source_phase,source_first,source_stage,source_close,source_unknown,source_open_calls,source_chain_calls,source_rng_calls;
 volatile LONG source_consumed;
 ULONGLONG source_until;BYTE attempt[32],authority[16],generation[16],plan_id[32],chain_id[32];
 BYTE runtime_sid[68];DWORD runtime_sid_bytes;
 PAPACC_MACHINE_FIXTURE_REFERENCE references[5];PSECURITY_DESCRIPTOR source_scratch;
 WCHAR executable[MAX_PATH],key_name[96],native_name[MAX_PATH+4];
 IO_STATUS_BLOCK source_io;OBJECT_ATTRIBUTES source_attributes;UNICODE_STRING source_unicode;NTSTATUS source_status,source_rng_status;
 BYTE plan_bytes[1024];DWORD plan_bytes_count;

} PAPACC_MACHINE_FIXTURE_CONTROL;
#endif
