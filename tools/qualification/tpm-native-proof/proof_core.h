#ifndef TP_NATIVE_PROOF_CORE_H
#define TP_NATIVE_PROOF_CORE_H
#define _WIN32_WINNT 0x0602
#include <windows.h>
#include <bcrypt.h>
#include <stddef.h>
#define TP_MAX_RESPONSE 4096U
#define TP_MAX_SAMPLES 256U
#define TP_NATIVE_LIMIT 128U
#define TP_OBSERVER_LIMIT 124U
#define TP_ADMIN_LIMIT 1024U
/* Private modern Windows diagnostic. No product owner or generic authority. */
typedef struct TP_OWNER TP_OWNER;
typedef unsigned long (*TP_OPEN)(void *,void **);
typedef unsigned long (*TP_SEND)(void *,void *,const unsigned char *,unsigned,unsigned char *,unsigned *);
typedef unsigned long (*TP_CLOSE)(void *,void *);
typedef int (*TP_BOUNDARY)(TP_OWNER *,const char *);
typedef struct TP_TRANSPORT {void *user;TP_OPEN open;TP_SEND send;TP_CLOSE close;} TP_TRANSPORT;
typedef struct TP_SAMPLE {const char *phase;ULONGLONG tick;DWORD count,error;unsigned own_refs;int known,memory_known;DWORD memory_error;SIZE_T private_bytes,working_set;} TP_SAMPLE;
struct TP_OWNER {
 DWORD owner_thread;volatile LONG busy,stop;unsigned used,closed,frozen,retained;
 const char *stage,*first,*first_stage;unsigned long first_native;
 BCRYPT_ALG_HANDLE sha,hmac,rsa,aes;BCRYPT_KEY_HANDLE public_key,work_key;BCRYPT_HASH_HANDLE work_hash;
 unsigned char hash_object[4096],key_object[4096];DWORD sha_object,hmac_object;
 unsigned char context_binding[32],salt[32],start_nonce[32],proof_nonce[32],session_key[32],tpm_nonce[32];
 unsigned char modulus[256],public_area[314],public_name[34],command[1024],response[TP_MAX_RESPONSE];
 void *context;unsigned context_attempted,context_opened,context_closed,session_attempted,session_ended,proof,calls,session;
 TP_TRANSPORT transport;TP_BOUNDARY boundary;void *guard_data;
 TP_SAMPLE samples[TP_MAX_SAMPLES];unsigned sample_count,peak,controlled_refs;
 unsigned hash_calls,inject_hash_failure,inject_close_failure;
};
void tp_init(TP_OWNER *o);
int tp_fail(TP_OWNER *o,const char *error,unsigned long native);
int tp_sample(TP_OWNER *o,const char *phase);
int tp_crypto_init(TP_OWNER *o);
int tp_hash(TP_OWNER *o,const unsigned char *p,size_t n,unsigned char out[32]);
int tp_mac(TP_OWNER *o,const unsigned char *key,size_t kn,const unsigned char *p,size_t n,unsigned char out[32]);
int tp_kdf(TP_OWNER *o,const unsigned char key[32],const char *label,const unsigned char *u,size_t un,const unsigned char *v,size_t vn,unsigned char out[32]);
int tp_rsa_import(TP_OWNER *o,const unsigned char modulus[256]);
int tp_cfb_fixture(TP_OWNER *o,const unsigned char key[16],const unsigned char iv[16],const unsigned char *plain,unsigned n,unsigned char *out);
unsigned tp_u16(const unsigned char *p);unsigned tp_u32(const unsigned char *p);
void tp_w16(unsigned char *p,unsigned n);void tp_w32(unsigned char *p,unsigned n);
int tp_eq(const unsigned char *a,const unsigned char *b,size_t n);
int tp_capability(const unsigned char *r,unsigned n,unsigned handles[8],unsigned *count);
/* Public return:1 matching EK,0 noncandidate profile, -1 malformed authoritative frame. */
int tp_public(TP_OWNER *o,const unsigned char *r,unsigned n,const unsigned char modulus[256],unsigned char area[314],unsigned char name[34]);
int tp_start_packet(unsigned handle,const unsigned char nonce[32],const unsigned char cipher[256],unsigned char out[319]);
int tp_start_response(const unsigned char *r,unsigned n,unsigned *session,unsigned char nonce[32]);
int tp_query(TP_OWNER *o,unsigned session,const unsigned char key[32],const unsigned char caller[32],const unsigned char tpm[32],unsigned char out[89]);
int tp_verify(TP_OWNER *o,const unsigned char *r,unsigned n,const unsigned char key[32],const unsigned char caller[32]);
int tp_run(TP_OWNER *o,const unsigned char context_binding[32]);
int tp_finish(TP_OWNER *o);
#endif
