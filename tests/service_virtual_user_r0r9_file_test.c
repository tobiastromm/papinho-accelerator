/* Same recorder/owner and native API substitutions; no real file effects. */
#define main preserved_file_model_main
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
 const unsigned modes[]={0,13,14,4,15,18,1,16};unsigned i;DWORD e;PSRF_FRAME *f;
 T(GetConsoleWindow()==NULL);
 T(ConvertStringSecurityDescriptorToSecurityDescriptorW(L"O:S-1-5-21-785699550-1440458426-1682014700-1108G:BAD:P(A;;FA;;;BA)(A;;FA;;;SY)(A;;GRGX;;;BU)",SDDL_REVISION_1,&dm_parent_sd,NULL));
 T(ConvertStringSecurityDescriptorToSecurityDescriptorW(L"O:S-1-5-21-785699550-1440458426-1682014700-1108G:BAD:P(A;;FA;;;S-1-5-21-785699550-1440458426-1682014700-1108)(A;;FA;;;SY)(A;;FA;;;BA)(A;;0x120083;;;S-1-5-80-3567404667-26456103-3018599297-139793421-3962028451)",SDDL_REVISION_1,&dm_file_sd,NULL));
 for(i=0;i<sizeof(modes)/sizeof(modes[0]);i++){
  dm_reset(0);T(ids());native.readback=1U;model_r4_fault=0;model_r4_original=model_r4_duplicate=NULL;model_r4_duplicate_stats=0;T(psrd_bind()==0);model_mode=modes[i];e=psr_token();
  T((e==0)==(i<2));if(!e){T(psrd_prepare_complete()==0&&native.runtime.ready&&native.runtime.diagnostic.receipt==2U);}else T(psrt_record.returned==e&&native.runtime.diagnostic.receipt==1U);
  T(dm_size==2048&&psrd_quiescent());f=(PSRF_FRAME *)(dm_disk+1024);T(psrf_valid(f,psrd_attempt,i<2?2U:1U));T(strstr((char *)f->text,"q_error=")!=NULL);T(psr_close()==0);
 }
 T(LocalFree(dm_parent_sd)==NULL);T(LocalFree(dm_file_sd)==NULL);
 printf("R4_ACTUAL_FILE_RECORDER=PASS cases=8 FULL_RUNTIME=%zu NATIVE=%zu TOTAL=%zu; ALL_FILE_APIS_MODELED\n",sizeof(PS_RUNTIME),sizeof(PS_NATIVE),papacc_server_service_storage_bytes());return 0;
}
