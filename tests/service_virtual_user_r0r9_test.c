/* Actual psr_token/current source, modeled API boundaries, no SCM/files. */
#ifdef PAPACC_PSR_FILE_RUNTIME_LAB
#undef PAPACC_PSR_FILE_RUNTIME_LAB
#endif
#define main preserved_default_model_main
#include "service_runtime_component_test.c"
#undef main
static unsigned checks,scenarios;
#define C(x) do{checks++;if(!(x)){printf("R4_FAIL line=%d mode=%u fault=%u\n",__LINE__,model_mode,model_r4_fault);return 1;}}while(0)
static int reset_r4(unsigned mode,unsigned fault)
{
 ZeroMemory(&native,sizeof(native));ZeroMemory(&host,sizeof(host));ZeroMemory(&psrt_record,sizeof(psrt_record));model_mode=mode;model_r4_fault=fault;model_stop_flag=0;model_closes=0;model_r4_original=model_r4_duplicate=NULL;model_r4_query_count=model_r4_duplicate_stats=model_r4_calls_at_stop=0;
 if(!ids())return 0;native.readback=1;return 1;
}
int main(void)
{
 unsigned i,c,before;DWORD e;PSID owner=NULL;const unsigned modes[]={0,13,14,4,15,18,1,16,17,2,3,5};
 for(i=0;i<sizeof(modes)/sizeof(modes[0]);i++){
  C(reset_r4(modes[i],0));e=psr_token();C((e==0)==(modes[i]==0||modes[i]==13));if(e)C(psrt_record.emitted&&psrt_record.returned==e);C(psr_close()==0);scenarios++;
 }
 for(i=1;i<=16;i++){
  if(i==5)continue;C(reset_r4(0,i));e=psr_token();C(e!=0);C(psrt_record.emitted&&psrt_record.returned==e);
  if(i==13)C(model_r4_calls_at_stop&&model_r4_query_count==model_r4_calls_at_stop);
  if(native.runtime.retained){c=model_closes;before=model_r4_query_count;C(psr_close()!=0&&model_closes==c);C(psr_r4_current()!=0&&model_r4_query_count==before);}else C(psr_close()==0);scenarios++;
 }
 C(reset_r4(13,0));native.readback=0;C(psr_token()!=0);C(psr_close()==0);scenarios++;
 C(reset_r4(13,0));native.service_sid[8]^=1;C(psr_token()!=0);C(psr_close()==0);scenarios++;
 C(reset_r4(13,0));C(psr_token()==0);before=model_r4_query_count;c=model_closes;model_stop_flag=1;C(psr_r4_current()!=0&&model_r4_query_count==before&&model_closes==c);model_stop_flag=0;C(psr_close()==0);scenarios++;
 C(reset_r4(13,0));C(psr_token()==0);model_mode=9;c=model_closes;C(psr_r4_current()!=0&&native.runtime.retained&&native.runtime.check_token);C(model_closes==c+1);c=model_closes;before=model_r4_query_count;C(psr_close()!=0&&model_closes==c);C(psr_r4_current()!=0&&model_r4_query_count==before);scenarios++;
 C(reset_r4(13,0));model_stop_flag=1;before=model_next;C(psr_token()!=0&&model_next==before&&model_r4_query_count==0);model_stop_flag=0;C(psr_close()==0);scenarios++;
 /* Explicit source-linked effective access checks still reject write grants. */
 C(reset_r4(13,0));C(psr_token()==0);C(ConvertStringSidToSidW(L"S-1-5-21-785699550-1440458426-1682014700-1108",&owner));
 C(InitializeSecurityDescriptor(&model_sd,SECURITY_DESCRIPTOR_REVISION));C(SetSecurityDescriptorOwner(&model_sd,owner,FALSE));C(SetSecurityDescriptorGroup(&model_sd,model_admin,FALSE));C(InitializeAcl((PACL)model_acl_bytes,sizeof(model_acl_bytes),ACL_REVISION));C(AddAccessAllowedAce((PACL)model_acl_bytes,ACL_REVISION,FILE_GENERIC_READ|FILE_GENERIC_EXECUTE,model_sid));C(SetSecurityDescriptorDacl(&model_sd,TRUE,(PACL)model_acl_bytes,FALSE));C(SetSecurityDescriptorControl(&model_sd,SE_DACL_PROTECTED,SE_DACL_PROTECTED));
 native.runtime.parent=(HANDLE)(ULONG_PTR)1001;native.runtime.configuration=(HANDLE)(ULONG_PTR)1002;native.runtime.relative_image=(HANDLE)(ULONG_PTR)1003;C(m_info(native.runtime.relative_image,&native.image_id));C(psr_current()==0);model_mode=8;C(psr_current()!=0);model_mode=0;C(psr_current()==0);model_mode=10;C(psr_current()!=0);model_mode=0;C(psr_close()==0);C(LocalFree(owner)==NULL);scenarios+=3;
 printf("R0_R9_ACTUAL_CALLER=PASS scenarios=%u assertions=%u; RUNTIME=%zu NATIVE=%zu; NO_OPERATIONAL_AUTHORITY\n",scenarios,checks,sizeof(PS_RUNTIME),sizeof(PS_NATIVE));return 0;
}
