#ifndef TP_HOST_PLATFORM_H
#define TP_HOST_PLATFORM_H
#include "proof_core.h"
#include "json_data.h"
#include <winevt.h>
#include <wincrypt.h>
#define TP_FILES 4
#define TP_CERT_MAX 16384
#define TP_ADMIN_SAMPLES 16
#define TP_OPERATOR "S-1-5-21-785699550-1440458426-1682014700-1108"
#define TP_HOST_SHA "fc2d00eb95bdc15ee7f56c7abd78f1c88670ba8359a0786ab4b49523fd91f482"
#define TP_EVENT_HOST_SHA "2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff"
#define TP_CERT_SHA "965f56aa0d4c5b5772b423d65d8a06a623109f5828c0eddcb8ec4ae8fa3de761"
#define TP_BOOT_START "2026-10-26T21:35:16.5000000Z"
#define TP_BOOT_RECORD 237900ULL
typedef struct TP_FILE {HANDLE handle;BY_HANDLE_FILE_INFORMATION id;DWORD bytes;int info_known,hash_known;char sha256[65];} TP_FILE;
typedef struct TP_ADMIN_SAMPLE {DWORD parent_pid,console_pid,parent_handles,console_handles;ULONGLONG parent_creation,console_creation,tick;} TP_ADMIN_SAMPLE;
typedef struct TP_PLATFORM {
 TP_OWNER proof;TP_FILE files[TP_FILES];TJ_DOC request,approval;
 HANDLE token,parent,console;EVT_HANDLE query,event,render;PCCERT_CONTEXT cert;
 unsigned char token_buffer[4096],token_ids[24],expected_context[32],cert_bytes[TP_CERT_MAX];
 wchar_t folder[MAX_PATH],executable[MAX_PATH];char request_hash[65],approval_hash[65],exe_hash[65],live_binding[128];
 DWORD pid,parent_pid,console_pid,topology_parent,topology_console,topology_count,topology_ids[8];ULONGLONG creation,parent_creation,console_creation;
 int token_bound,context_bound,observer_attempted,observer_known;DWORD observer_count,observer_error,observer_close_error;
 TP_ADMIN_SAMPLE admins[TP_ADMIN_SAMPLES];unsigned admin_count;int receipt_created,receipt_uncertain;DWORD receipt_written,receipt_expected;
} TP_PLATFORM;
void hp_init(TP_PLATFORM *p);
void hp_hex(const unsigned char *in,unsigned n,char *out);
int hp_hex_input(const wchar_t *s,char out[65]);
int hp_token(TP_PLATFORM *p);
int hp_boot(TP_PLATFORM *p);
int hp_admin(TP_PLATFORM *p);
int hp_console_stop(TP_PLATFORM *p);
int hp_files_current(TP_PLATFORM *p);
int hp_context_current(TP_PLATFORM *p,const char *phase);
int hp_boundary(TP_OWNER *o,const char *phase);
int hp_files(TP_PLATFORM *p,const char exe_sha[65],const char approval_sha[65]);
int hp_request_contract(TP_PLATFORM *p);
int hp_approval_contract(TP_PLATFORM *p);
int hp_confirm(TP_PLATFORM *p);
int hp_receipt(TP_PLATFORM *p);
int hp_load_certificate(TP_PLATFORM *p);
int hp_close(TP_PLATFORM *p);
void hp_print(const TP_PLATFORM *p,const char *transport_kind);
int hp_utc_filetime(const wchar_t *s,ULONGLONG *out);
#endif
