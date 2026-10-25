/* Exercises canonical owner/recorder with modelled native file effects. */
#define main runtime_base_main
#include "service_runtime_component_test.c"
#undef main
static void dm_reset(unsigned mode)
{
 ZeroMemory(&native,sizeof(native));ZeroMemory(&host,sizeof(host));ZeroMemory(&psrt_record,sizeof(psrt_record));ZeroMemory(dm_disk,sizeof(dm_disk));
 dm=mode;dm_size=1024;dm_pos=dm_writes=dm_closes=dm_creates=0;dm_stop=&host.stop;model_mode=model_stop_flag=0;
 {PSRF_FRAME *f=(PSRF_FRAME *)dm_disk;memcpy(f->magic,psrf_magic,16);memcpy(f->attempt,psrd_attempt,32);if(mode==21)memcpy(f->attempt,"75330d11cc7e4123ae62eadc9a59c2f5",32);f->version=1;f->check=psrf_checksum(f);if(mode==12)f->attempt[0]^=1;}
 if(mode==14)dm_size=2048;
}
int main(void)
{
 unsigned scenario,c,w;DWORD result;PSRF_FRAME *f;char saved[1024];
 T(GetConsoleWindow()==NULL);T(runtime_base_main()==0);
 T(ConvertStringSecurityDescriptorToSecurityDescriptorW(L"O:S-1-5-21-785699550-1440458426-1682014700-1108G:BAD:P(A;;FA;;;BA)(A;;FA;;;SY)(A;;GRGX;;;BU)",SDDL_REVISION_1,&dm_parent_sd,NULL));
 T(ConvertStringSecurityDescriptorToSecurityDescriptorW(L"O:S-1-5-21-785699550-1440458426-1682014700-1108G:BAD:P(A;;FA;;;S-1-5-21-785699550-1440458426-1682014700-1108)(A;;FA;;;SY)(A;;FA;;;BA)(A;;0x120083;;;S-1-5-80-3567404667-26456103-3018599297-139793421-3962028451)",SDDL_REVISION_1,&dm_file_sd,NULL));
 T(ConvertStringSecurityDescriptorToSecurityDescriptorW(L"O:S-1-5-18G:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;FA;;;BU)",SDDL_REVISION_1,&dm_bad_sd,NULL));
 for(scenario=0;scenario<=21;scenario++){
  if(scenario==17||scenario==18)continue;
  dm_reset(scenario);memcpy(saved,dm_disk,1024);if(scenario==4)dm=0;result=psrd_bind();
  if(scenario==1||scenario==2||scenario==3||scenario==5||scenario==12||scenario==13||scenario==14||scenario==15||scenario==16||scenario==20||scenario==21){T(result!=0);T(dm_writes==0);if(native.runtime.retained){c=dm_closes;T(psr_close()!=0&&dm_closes==c);}continue;}
  T(result==0&&native.runtime.diagnostic.bound);if(scenario==4)dm=0; /* replace only AFTER binding */
  if(scenario==4)dm=4;
  if(scenario==11){T(host.stop);}
  model_nt_status=(LONG)0xc0000022UL;model_nt_handle=0;SetLastError(12345);
  result=psr_open(NULL,L"\\Device\\MODEL_ONLY",&native.runtime.parent,1);
  T(result==ERROR_ACCESS_DENIED&&psrt_record.ntstatus==(LONG)0xc0000022UL&&psrt_record.returned==ERROR_ACCESS_DENIED);T(!memcmp(saved,dm_disk,1024));
  if(scenario==4||scenario==6||scenario==7||scenario==8||scenario==9||scenario==10||scenario==11){T(native.runtime.retained&&native.runtime.diagnostic.error);c=dm_closes;w=dm_writes;T(psr_close()!=0&&dm_closes==c);psrd_emit("retry forbidden",1);T(dm_writes==w);continue;}
  T(dm_size==2048&&native.runtime.diagnostic.receipt==1U&&psrd_quiescent());f=(PSRF_FRAME *)(dm_disk+1024);T(psrf_valid(f,psrd_attempt,1));T(strstr((char *)f->text,"ntstatus=c0000022")!=NULL);T(strstr((char *)f->text,"expected_config_sha=")!=NULL);T(psr_close()==0);
 }
 {
 const LONG statuses[]={(LONG)0xc0000022UL,(LONG)0xc0000034UL,(LONG)0xc000003aUL,(LONG)0xc0000043UL,(LONG)0xc0000035UL,(LONG)0xc000000dUL,(LONG)0xe1234567UL};
 unsigned j;char hex[32];
 for(j=0;j<sizeof(statuses)/sizeof(statuses[0]);j++){
  dm_reset(0);T(psrd_bind()==0);PSRT_CONTEXT("PARENT");(void)PSRT_NT("NtCreateFile",FILE_TRAVERSE|FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE,statuses[j]);SetLastError(12345);
  T(PSRT_RETURN(ERROR_ACCESS_DENIED,"prepare_propagation")==ERROR_ACCESS_DENIED&&GetLastError()==12345);T(psrt_record.ntstatus==statuses[j]);
  (void)_snprintf_s(hex,sizeof(hex),_TRUNCATE,"ntstatus=%08lx",(unsigned long)(DWORD)statuses[j]);f=(PSRF_FRAME *)(dm_disk+1024);
  T(psrf_valid(f,psrd_attempt,1)&&strstr((char *)f->text,hex)!=NULL&&native.runtime.diagnostic.receipt==1U&&psrd_quiescent());T(psr_close()==0);
 }
 }
 /* The existing final frame must carry the differentiating facts. */
 for(scenario=4U;scenario<=18U;scenario++){
  if(scenario>4U&&scenario<13U)continue;if(scenario==16U||scenario==17U)continue;
  dm_reset(0);T(ids());T(psrd_bind()==0);model_mode=scenario;
  T(psr_token()==ERROR_ACCESS_DENIED);T(dm_size==2048&&psrd_quiescent());
  f=(PSRF_FRAME *)(dm_disk+1024);T(psrf_valid(f,psrd_attempt,1));
  T(strstr((char *)f->text,"predicate=token.specific_group.enabled")!=NULL);
  T(strstr((char *)f->text,scenario==13U?"observed_a=0 observed_b=0":"observed_a=1 observed_b=")!=NULL);
  if(scenario==15U)T(strstr((char *)f->text,"observed_b=20")!=NULL);
  if(scenario==4U)T(strstr((char *)f->text,"observed_b=16")!=NULL);
  if(scenario==18U)T(strstr((char *)f->text,"observed_b=10")!=NULL);
  T(strstr((char *)f->text,"q_error=0 q_known=15 q_complete=1")!=NULL);
    T(strstr((char *)f->text,"filtered_raw=1 filtered_bytes=1 restricted_count=0")!=NULL);
    T(strstr((char *)f->text,"nt_valid=0")!=NULL&&strstr((char *)f->text,"token_handle=NOT_EXPORTED")!=NULL);
  w=dm_writes;PSRT_RETURN(ERROR_INVALID_DATA,"later_failure_must_not_replace_first");T(dm_writes==w&&psrt_record.returned==ERROR_ACCESS_DENIED);
  T(psr_close()==0);
 }
 dm_reset(0);host.stop=1;c=dm_creates;T(psrd_bind()==ERROR_CANCELLED&&dm_creates==c&&psrd_quiescent());
 dm_reset(0);T(psrd_bind()==0);host.handle=(SERVICE_STATUS_HANDLE)(ULONG_PTR)9;host.thread=GetCurrentThreadId();host.run_active=1;T(ps_begin(&host.life));T(ps_stop(&host.life));papacc_server_service_runtime_closed();T(host.terminal_receipt==2&&host.life.phase==PS_RETAINED&&!psrd_quiescent());
 dm_reset(0);T(ids());T(psrd_bind()==0);T(psr_token()==0);T(psrd_prepare_complete()==0&&native.runtime.ready&&native.runtime.diagnostic.receipt==2&&psrd_quiescent());f=(PSRF_FRAME *)(dm_disk+1024);T(psrf_valid(f,psrd_attempt,2));T(strstr((char *)f->text,"q_error=0 q_known=15 q_complete=1")!=NULL);T(strstr((char *)f->text,"sid_count=1 sid_attrs=00000004")!=NULL);T(psr_close()==0);
 dm_reset(0);T(ids());T(psrd_bind()==0);T(psr_token()==0);dm=7;T(psrd_prepare_complete()!=0&&!native.runtime.ready&&native.runtime.retained);host.handle=(SERVICE_STATUS_HANDLE)(ULONG_PTR)9;host.run_active=1;host.thread=GetCurrentThreadId();T(ps_begin(&host.life));T(!papacc_server_service_runtime_ready());T(psr_close()!=0);
 T(LocalFree(dm_parent_sd)==NULL);T(LocalFree(dm_file_sd)==NULL);T(LocalFree(dm_bad_sd)==NULL);
 printf("CANONICAL_FILE_RUNTIME_MODEL=PASS bind_identity_owner_dacl_attempt_stop_partial_close_retention_quiescence_native_status_no_console; DIAGNOSTIC=%zu RUNTIME=%zu NATIVE=%zu TOTAL_STORAGE=%zu\n",sizeof(PS_DIAGNOSTIC),sizeof(PS_RUNTIME),sizeof(PS_NATIVE),papacc_server_service_storage_bytes());return 0;
}
