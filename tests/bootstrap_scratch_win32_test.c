/* EXACT production function body from configure-time extraction; only resolver,
 * Stop and native begin dependencies are modelled. Begin ALWAYS DENIES.
 * Real BODY05 codecs/hash run, no filesystem bootstrap or certificate creation. */
#include "server_config_source_binding_win32.h"
#include "config_root8.h"
#include "server_config_root8_data_win32.h"
#include "server_configuration_body05_win32.h"
#include "config_source.h"
#include <stdio.h>
#include <string.h>
#define bytes root8_fixture_bytes
#define main existing_root8_fixture_not_run
#include "config_root8_test.c"
#undef main
#undef bytes
static PAPACC_SERVER_LAUNCHER_OWNER_WIN32 launcher;
static PAPACC_CONFIG_PENDING pending_before;
static PC_DOCUMENT document;
static unsigned resolve_ok,stop_model,begin_calls;
static PC_RESULT fixture_resolve(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,const PC_DOCUMENT *doc,
 PAPACC_BOOTSTRAP_RESOLUTION *out,PC_DIAGNOSTIC *diag)
{
 (void)o;(void)doc;(void)diag;memset(out,0,sizeof(*out));
 if(!resolve_ok)return PC_REFERENCE;out->initial_resolved=1U;return PC_OK;
}
static PAPACC_BOOL fixture_stop(void){return stop_model?PAPACC_TRUE:PAPACC_FALSE;}
static PAPACC_RESULT fixture_begin(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_U8 *image,
 PAPACC_SIZE n,const char *leaf,PAPACC_SIZE reserved)
{
 (void)o;(void)image;(void)n;(void)leaf;(void)reserved;begin_calls++;
 return PAPACC_RESULT_NOT_SUPPORTED; /* trap: no native operation or success */
}
#define papacc_server_config_initial_bootstrap_resolve fixture_resolve
#define papacc_server_console_win32_stop_requested fixture_stop
#define papacc_configuration_bootstrap_commit_begin fixture_begin
#define papacc_server_config_initial_bootstrap_commit fixture_commit
#include "bootstrap_scratch_commit.inc"
#undef papacc_server_config_initial_bootstrap_resolve
#undef papacc_server_console_win32_stop_requested
#undef papacc_configuration_bootstrap_commit_begin
#undef papacc_server_config_initial_bootstrap_commit
struct GUARDED_BODY { unsigned char before[16],body[1024],after[16]; };
static struct GUARDED_BODY guarded,unchanged;
static PAPACC_CONFIG_VIEW decoded_body;
static int guards(void)
{
 unsigned j;for(j=0;j<16U;j++)if(guarded.before[j]!=0xA5||guarded.after[j]!=0xA5)return 0;return 1;
}
static int scratch_cases(void)
{
 unsigned j,prior;PAPACC_RESULT result;char path[MAX_PATH];DWORD n;
 n=GetFullPathNameA("bootstrap-scratch-MODEL-NO-STORE",MAX_PATH,path,NULL);CHECK(n&&n<MAX_PATH);
 CHECK(papacc_configuration_commit_init03(&owner,path)==0);CHECK(setup(1,0));
 CHECK(sizeof(launcher.configuration_commit.pending_commit.observed)==512U);
 memset(&guarded,0xA5,sizeof(guarded));
 CHECK(papacc_configuration_body05_encode(&fixture_store_hash,&view,guarded.body,sizeof(guarded.body))==0);
 CHECK(guards()&&!memcmp(guarded.body,root8_fixture_bytes+1024U,sizeof(guarded.body)));
 CHECK(papacc_configuration_body05_decode(&fixture_store_hash,guarded.body,sizeof(guarded.body),&decoded_body)==0);
 for(j=0;j<3U;j++){
  PAPACC_SIZE cap=j==0U?512U:(j==1U?1023U:1025U);
  memset(&guarded,0xA5,sizeof(guarded));unchanged=guarded;
  CHECK(papacc_configuration_body05_encode(&fixture_store_hash,&view,guarded.body,cap)!=0);
  CHECK(!memcmp(&guarded,&unchanged,sizeof(guarded)));
  CHECK(papacc_configuration_body05_decode(&fixture_store_hash,guarded.body,cap,&decoded_body)!=0);
  CHECK(guards());
 }
 launcher.configuration_preparation.model=view;memcpy(launcher.configuration_commit.domain,root8_fixture_bytes+72,32);
 document.listener_count=1;document.listeners[0].block=0;document.blocks[0].opener=0;
 strcpy_s(document.lines[0].tokens[1].text,sizeof(document.lines[0].tokens[1].text),"La");
 memset(launcher.configuration_commit.pending_commit.observed,0x5A,512U);
 strcpy_s(launcher.configuration_commit.pending_commit.path,MAX_PATH,"ADJACENT_PATH_CANARY");
 launcher.configuration_commit.pending_commit.expected_volume=0x12345678U;
 pending_before=launcher.configuration_commit.pending_commit;
 resolve_ok=1;prior=begin_calls;
 result=fixture_commit(&launcher,&document,root8_fixture_bytes,used,"not-created",PAPACC_ED11_PROCESS_LIMIT);
 CHECK(result==PAPACC_RESULT_NOT_SUPPORTED&&begin_calls==prior+1U);
 CHECK(!memcmp(&pending_before,&launcher.configuration_commit.pending_commit,sizeof(pending_before)));
 root8_fixture_bytes[1024U]^=1;prior=begin_calls;CHECK(fixture_commit(&launcher,&document,root8_fixture_bytes,used,"not-created",PAPACC_ED11_PROCESS_LIMIT)!=0&&begin_calls==prior);root8_fixture_bytes[1024U]^=1;
 CHECK(fixture_commit(&launcher,&document,root8_fixture_bytes,PC_ROOT8_MIN-1U,"not-created",PAPACC_ED11_PROCESS_LIMIT)!=0);
 CHECK(fixture_commit(&launcher,&document,root8_fixture_bytes,PC_ROOT8_NO_ISSUER_MAX+1U,"not-created",PAPACC_ED11_PROCESS_LIMIT)!=0);
 CHECK(fixture_commit(&launcher,&document,root8_fixture_bytes,used,"not-created",0U)==PAPACC_RESULT_LIMIT_EXCEEDED);
 stop_model=1;prior=begin_calls;CHECK(fixture_commit(&launcher,&document,root8_fixture_bytes,used,"not-created",PAPACC_ED11_PROCESS_LIMIT)!=0&&begin_calls==prior);stop_model=0;
 launcher.configuration_commit.store_hash.stopped=1;
 CHECK(fixture_commit(&launcher,&document,root8_fixture_bytes,used,"not-created",PAPACC_ED11_PROCESS_LIMIT)!=0&&begin_calls==prior);
 CHECK(!memcmp(&pending_before,&launcher.configuration_commit.pending_commit,sizeof(pending_before)));
 /* Clear only synthetic Stop state; no native owner/retained resource exists. */
 launcher.configuration_commit.store_hash.stopped=0;
 resolve_ok=0;CHECK(fixture_commit(&launcher,&document,root8_fixture_bytes,used,"not-created",PAPACC_ED11_PROCESS_LIMIT)!=0&&begin_calls==prior);
 /* Actual authority guard, no model resolver in this call. */
 CHECK(papacc_server_config_initial_bootstrap_commit(&launcher,&document,root8_fixture_bytes,used,"not-created",PAPACC_ED11_PROCESS_LIMIT)!=0);
 CHECK(!launcher.configuration_commit.published&&!launcher.configuration_commit.pending_commit.active);
 CHECK(!memcmp(&pending_before,&launcher.configuration_commit.pending_commit,sizeof(pending_before)));
 CHECK(papacc_configuration_stop(&owner)==0&&papacc_configuration_finish(&owner)==0);
 printf("PASS %u REAL_BODY05_CANARIES MODEL_RESOLVER_BEGIN_DENIES REAL_AUTHORITY_DENIES NO_BOOTSTRAP_NO_CURRENT\n",checks);return 1;
}
int main(void){return scratch_cases()?0:1;}
