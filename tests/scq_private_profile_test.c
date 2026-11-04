#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <stdio.h>
static ULONGLONG test_tick=1000;
static ULONGLONG WINAPI model_tick(void){return test_tick;}
#define GetTickCount64 model_tick
#include "scq_profile_fixture.h"
#include "../apps/server/personal_scq_profile_win32.inc"
typedef char no_padding_peer[sizeof(PAPACC_SCQ_PEER_DATA)==220?1:-1];
typedef char no_padding_ref[sizeof(PAPACC_SCQ_REF_DATA)==100?1:-1];
typedef char exact_projection[sizeof(PAPACC_SCQ_PLAN_DATA)==PAPACC_SCQ_RECORD_BYTES?1:-1];
static unsigned checks;
#define C(x) do{checks++;if(!(x)){printf("FAIL SCQ_PROFILE line=%u\n",(unsigned)__LINE__);return 1;}}while(0)
static PAPACC_SCQ_PROFILE_DATA s,r;
static PAPACC_SCQ_PLAN_DATA p,q;
static int source_owner,target_owner;
static volatile LONG stop;
static BYTE frame[PAPACC_SCQ_RECORD_BYTES+1],receipt[PAPACC_SCQ_RECEIPT_BYTES];
static int setup(void)
{
 memset(&s,0,sizeof(s));memset(&r,0,sizeof(r));stop=0;test_tick=1000;
 if(!scq_fixture_plan(&p))return 0;
 return !papacc_scq_profile_prepare_data(&s,&source_owner,1,&p,&p.administrator)&&
  !papacc_scq_profile_prepare_data(&r,&target_owner,2,&p,&p.runtime);
}
static DWORD WINAPI foreign_take(LPVOID unused)
{(void)unused;return papacc_scq_profile_take_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,&stop);}
int main(void)
{
 unsigned i,j,before;DWORD result;HANDLE worker;PAPACC_SCQ_PEER_DATA peer;PAPACC_SCQ_REF_DATA refs[3];
 /* TO2: only the typed closed constructor accepts purpose2. */
 C(scq_fixture_plan(&q));q.purpose=PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY;memset(q.key,0,sizeof(q.key));q.runtime.service_state=SERVICE_START_PENDING;
 memset(&s,0,sizeof(s));C(!papacc_scq_profile_prepare_transport_data(&s,&source_owner,1U,&q,&q.administrator));
 C(s.pre_readiness&&s.phase==1U);C(papacc_scq_plan_encode(&q,frame,PAPACC_SCQ_RECORD_BYTES)!=0);
 C(!scqp_encode_fields(&q,frame,PAPACC_SCQ_RECORD_BYTES));C(papacc_scq_plan_decode(frame,PAPACC_SCQ_RECORD_BYTES,&p)!=0);
 memset(&r,0,sizeof(r));C(papacc_scq_profile_prepare_data(&r,&target_owner,2U,&q,&q.runtime)!=0);
 memset(&r,0,sizeof(r));C(papacc_scq_profile_prepare_closed_data(&r,&target_owner,2U,&q,&q.runtime)!=0);
 for(i=0;i<sizeof(q.key);i++){
  q.key[i]='X';memset(&r,0,sizeof(r));C(papacc_scq_profile_prepare_transport_data(&r,&target_owner,2U,&q,&q.runtime)!=0);q.key[i]=0;
 }
 /* Excess rights, N0, identity mismatch and concurrent use still reject. */
 for(i=0;i<5U;i++){
  p=q;if(i==0U)p.refs[0].rights|=DELETE;if(i==1U)p.profile=1U;
  if(i==2U)p.runtime.image[2]++;if(i==3U)p.runtime.enumerated--;if(i==4U)p.runtime.restricted=1U;
  memset(&r,0,sizeof(r));C(papacc_scq_profile_prepare_transport_data(&r,&target_owner,2U,&p,&p.runtime)!=0);
 }
 worker=CreateThread(NULL,0,foreign_take,NULL,0,NULL);C(worker!=NULL);
 C(WaitForSingleObject(worker,10000)==WAIT_OBJECT_0);C(GetExitCodeThread(worker,&result)&&result==ERROR_INVALID_STATE&&!s.taken);C(CloseHandle(worker));
 q.purpose=PAPACC_SCQ_PURPOSE_READ;memset(&r,0,sizeof(r));C(papacc_scq_profile_prepare_closed_data(&r,&target_owner,2U,&q,&q.runtime)!=0);
 C(scq_fixture_plan(&q));memset(&r,0,sizeof(r));C(papacc_scq_profile_prepare_transport_data(&r,&target_owner,2U,&q,&q.runtime)!=0);
 C(setup());C(!papacc_scq_plan_encode(&p,frame,PAPACC_SCQ_RECORD_BYTES));C(!papacc_scq_plan_decode(frame,PAPACC_SCQ_RECORD_BYTES,&q));C(!memcmp(&p,&q,sizeof(p)));
 C(papacc_scq_profile_offer_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop)==ERROR_INVALID_STATE&&!s.taken);
 C(!papacc_scq_profile_take_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,&stop)&&s.taken);
 /* First modeled protected effect could occur ONLY after take. No Start API. */
 C(papacc_scq_profile_take_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,&stop)==ERROR_INVALID_STATE);
 C(!papacc_scq_profile_offer_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop));
 C(!papacc_scq_profile_receive_data(&r,&target_owner,&p.runtime,&p.administrator,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop));
 C(!papacc_scq_profile_receipt_data(&r,&target_owner,receipt,sizeof(receipt)));
 C(!papacc_scq_profile_ack_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,receipt,sizeof(receipt),&stop));
 C(!papacc_scq_profile_close_data(&r,&target_owner,1)&&!papacc_scq_profile_close_data(&s,&source_owner,1));
 C(!papacc_scq_profile_close_data(&s,&source_owner,1));C(papacc_scq_profile_prepare_data(&s,&source_owner,1,&p,&p.administrator)==ERROR_INVALID_STATE);
 /* Invalid projections: N0/other operation/secret schema, bad SID/token,
  * incomplete groups, restrictions, excessive/unknown references and aliases. */
 for(i=0;i<42;i++){
  C(scq_fixture_plan(&q));
  if(i==0)q.profile=1;if(i==1)q.purpose=2;if(i==2)q.schema=2;if(i==3)q.version=2;
  if(i==4)memset(q.attempt,0,32);if(i==5)q.key[0]='X';if(i==6)q.key[111]='X';
  if(i==7)q.runtime.enumerated--;if(i==8)q.runtime.restricted=1;if(i==9)q.runtime.elevated=1;
  if(i==10){q.runtime.matches=1;q.runtime.attributes=SE_GROUP_USE_FOR_DENY_ONLY|SE_GROUP_ENABLED;}
  if(i==11){q.runtime.matches=1;q.runtime.attributes=0;}
  if(i==12)q.runtime.matches=2;if(i==13)q.administrator.attributes=SE_GROUP_USE_FOR_DENY_ONLY|SE_GROUP_ENABLED;
  if(i==14)q.administrator.elevated=0;if(i==15)q.runtime.token_type=TokenImpersonation;
  if(i==16)q.refs[0].role=99;if(i==17)q.refs[1].rights|=WRITE_DAC;if(i==18)q.refs[2].attributes=FILE_ATTRIBUTE_REPARSE_POINT;
  if(i==19)q.refs[2].links=2;if(i==20)q.refs[1].low=q.refs[0].low;if(i==21)q.runtime.pid=q.administrator.pid;
  if(i==22)q.runtime.sid[67]=1;if(i==23)q.runtime.sid_bytes--;if(i==24)q.runtime.elevation_type=TokenElevationTypeLimited;
  if(i==25)q.administrator.enumerated--;if(i==26)memset(q.key+sizeof("PapinhoAccelerator.CA.")-1,'0',32);if(i==27)memset(q.refs[0].descriptor,0,32);
  if(i==28)q.administrator.pid=0;if(i==29)q.refs[0].rights=GENERIC_ALL;if(i==30)q.service=2;if(i==31)q.runtime.wait_status=WAIT_OBJECT_0;if(i==32)q.administrator.query_error=ERROR_ACCESS_DENIED;if(i==33)q.runtime.exit_code=0;
  if(i==34)q.runtime.logon=2;if(i==35)q.administrator.session=0;if(i==36)q.runtime.administrator_attributes=SE_GROUP_ENABLED;
  if(i==37)q.runtime.service_sid_type=3;if(i==38)q.runtime.service_type=SERVICE_WIN32_SHARE_PROCESS;if(i==39)q.runtime.service_state=SERVICE_STOPPED;if(i==40)q.runtime.image[2]++;if(i==41)q.runtime.image_digest[0]^=1;
  C(papacc_scq_plan_encode(&q,frame,PAPACC_SCQ_RECORD_BYTES)!=0);
 }
 C(scq_fixture_plan(&q));q.runtime.matches=1;q.runtime.attributes=SE_GROUP_ENABLED;C(!papacc_scq_plan_encode(&q,frame,PAPACC_SCQ_RECORD_BYTES));
 /* Strict lengths and every individual byte substitution. */
 C(setup());C(!papacc_scq_plan_encode(&p,frame,PAPACC_SCQ_RECORD_BYTES));
 for(i=0;i<PAPACC_SCQ_RECORD_BYTES;i++)C(papacc_scq_plan_decode(frame,i,&q)!=0);
 C(papacc_scq_plan_decode(frame,sizeof(frame),&q)!=0);
 for(i=0;i<PAPACC_SCQ_RECORD_BYTES;i++){
  memset(&r,0,sizeof(r));C(!papacc_scq_profile_prepare_data(&r,&target_owner,2,&p,&p.runtime));
  frame[i]^=1;C(papacc_scq_profile_receive_data(&r,&target_owner,&p.runtime,&p.administrator,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop)!=0);frame[i]^=1;
 }
 /* Independent peer observations changed on EITHER end, including PID reuse. */
 for(i=0;i<8;i++)for(j=0;j<2;j++){
  C(setup());peer=j?p.runtime:p.administrator;
  if(i==0)peer.pid++;if(i==1)peer.creation[0]++;if(i==2)peer.token_id[0]++;if(i==3)peer.modified[0]++;
  if(i==4)peer.authentication[0]++;if(i==5)peer.sid[peer.sid_bytes-1]++;if(i==6)peer.restricted=1;if(i==7)peer.enumerated--;
  C(papacc_scq_profile_take_data(&s,&source_owner,j?&p.administrator:&peer,j?&peer:&p.runtime,p.refs,&stop)!=0&&!s.taken);
 }
 C(setup());memcpy(refs,p.refs,sizeof(refs));refs[0].rights|=DELETE;
 C(papacc_scq_profile_take_data(&s,&source_owner,&p.administrator,&p.runtime,refs,&stop)!=0);
 C(setup());C(!papacc_scq_plan_encode(&p,frame,PAPACC_SCQ_RECORD_BYTES));peer=p.runtime;peer.creation[0]++;
 C(papacc_scq_profile_receive_data(&r,&target_owner,&peer,&p.administrator,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop)!=0);
 C(setup());{PAPACC_SCQ_PROFILE_DATA copied=s;C(papacc_scq_profile_take_data(&copied,&source_owner,&p.administrator,&p.runtime,p.refs,&stop)==ERROR_INVALID_STATE);}
 C(papacc_scq_profile_take_data(&s,&target_owner,&p.administrator,&p.runtime,p.refs,&stop)==ERROR_INVALID_STATE);
 /* A forwarded frame cannot be retargeted to a different independently bound owner. */
 C(!papacc_scq_plan_encode(&p,frame,PAPACC_SCQ_RECORD_BYTES));q=p;q.runtime.pid++;
 memset(&r,0,sizeof(r));C(!papacc_scq_profile_prepare_data(&r,&target_owner,2,&q,&q.runtime));
 C(papacc_scq_profile_receive_data(&r,&target_owner,&q.runtime,&q.administrator,q.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop)!=0);
 C(setup());stop=1;C(papacc_scq_profile_take_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,&stop)==ERROR_CANCELLED&&!s.taken);
 C(setup());C(!papacc_scq_profile_take_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,&stop));stop=1;
 C(papacc_scq_profile_offer_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop)==ERROR_CANCELLED&&s.taken&&!s.used);
 C(setup());C(!papacc_scq_profile_take_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,&stop));
 C(!papacc_scq_profile_offer_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop));
 C(papacc_scq_profile_close_data(&s,&source_owner,1)==ERROR_INVALID_HANDLE&&s.phase==4&&s.taken&&s.used);
 C(papacc_scq_profile_close_data(&s,&source_owner,1)==ERROR_INVALID_STATE);
 C(setup());C(papacc_scq_profile_close_data(&r,&target_owner,0)==ERROR_INVALID_HANDLE&&r.phase==4);
 C(papacc_scq_profile_close_data(&r,&target_owner,1)==ERROR_INVALID_STATE);
 /* Foreign native thread cannot consume or mutate the original DATA context. */
 C(setup());before=s.phase;worker=CreateThread(NULL,0,foreign_take,NULL,0,NULL);C(worker!=NULL);
 C(WaitForSingleObject(worker,10000)==WAIT_OBJECT_0);C(GetExitCodeThread(worker,&result)&&result==ERROR_INVALID_STATE&&s.phase==before&&!s.taken);C(CloseHandle(worker));
 C(papacc_scq_plan_encode(&p,(BYTE *)&p,sizeof(p))!=0);
 memcpy(&q,frame,sizeof(q));C(papacc_scq_plan_decode((BYTE *)&q,sizeof(q),&q)!=0);
 /* NEW_INSTALL ordering: no predicted PID/token, consume before modeled
  * Start, then bind actual observed runtime DATA without changing the request. */
 C(setup());q=p;memset(&q.runtime,0,sizeof(q.runtime));q.runtime.sid_bytes=p.runtime.sid_bytes;memcpy(q.runtime.sid,p.runtime.sid,68);
 memset(&s,0,sizeof(s));C(!papacc_scq_profile_prepare_data(&s,&source_owner,1,&q,&q.administrator)&&s.waiting_runtime&&!s.taken);
 C(papacc_scq_plan_decode(s.record,sizeof(s.record),&q)!=0); /* cannot deliver an unbound record */
 C(papacc_scq_profile_bind_runtime_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,&stop)==ERROR_INVALID_STATE);
 C(!papacc_scq_profile_take_data(&s,&source_owner,&p.administrator,NULL,p.refs,&stop)&&s.taken&&s.waiting_runtime);
 {unsigned modeled_start=0;C(s.taken==1);modeled_start++;C(modeled_start==1);}
 C(papacc_scq_profile_offer_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop)==ERROR_INVALID_STATE);
 C(!papacc_scq_profile_bind_runtime_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,&stop)&&!s.waiting_runtime);
 C(papacc_scq_profile_bind_runtime_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,&stop)==ERROR_INVALID_STATE);
 C(!papacc_scq_profile_offer_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop));
 C(!papacc_scq_profile_receive_data(&r,&target_owner,&p.runtime,&p.administrator,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop));
 C(!papacc_scq_profile_receipt_data(&r,&target_owner,receipt,sizeof(receipt)));
 receipt[10]^=1;C(papacc_scq_profile_ack_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,receipt,sizeof(receipt),&stop)!=0);
 C(papacc_scq_profile_close_data(&s,&source_owner,1)!=0&&s.phase==4&&s.taken); /* wrong receipt retains */
 for(i=0;i<3;i++){
  C(setup());test_tick=i==0?11000:i==1?999:1000;
  if(i<2)C(papacc_scq_profile_take_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,&stop)==ERROR_TIMEOUT&&!s.taken);
  else{C(!papacc_scq_profile_take_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,&stop));test_tick=11000;
   C(papacc_scq_profile_offer_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop)==ERROR_TIMEOUT&&s.taken&&!s.used);}
 }
 C(setup());C(!papacc_scq_profile_take_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,&stop));
 C(!papacc_scq_profile_offer_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop));
 C(!papacc_scq_profile_receive_data(&r,&target_owner,&p.runtime,&p.administrator,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop));
 C(papacc_scq_profile_receive_data(&r,&target_owner,&p.runtime,&p.administrator,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop)==ERROR_INVALID_STATE);
 C(papacc_scq_profile_offer_data(&s,&source_owner,&p.administrator,&p.runtime,p.refs,frame,PAPACC_SCQ_RECORD_BYTES,&stop)!=0&&s.taken&&s.used);
 printf("PASS SCQ_PRIVATE_PROFILE checks=%u RECORD=%u RECEIPT=%u CONTEXT=%zu REAL_SID_THREAD MODELED_PEERS_REFERENCES_CONSUMPTION ENDPOINT_CALLS=0 NO_AUTHORITY\n",checks,(unsigned)PAPACC_SCQ_RECORD_BYTES,(unsigned)PAPACC_SCQ_RECEIPT_BYTES,sizeof(s));return 0;
}
