/* Isolated candidate ONLY. Actual runtime predicate below is unchanged. */
#define main prior_actual_runtime_model_main
#include "service_runtime_component_test.c"
#undef main
typedef struct R_FACTS {
 unsigned primary,user_exact,stable,query_complete,sid_type,generic_service,admin_enabled;
 unsigned matches,attrs,restricted_count,filtered_history,effective_access,stop,close_unknown;
} R_FACTS;
static int candidate(const R_FACTS *f){
 if(!f||!f->primary||!f->user_exact||!f->stable||!f->query_complete||f->sid_type!=SERVICE_SID_TYPE_UNRESTRICTED||!f->generic_service||f->admin_enabled||f->restricted_count||f->filtered_history||!f->effective_access||f->stop||f->close_unknown)return 0;
 if(f->matches==0)return f->attrs==0;
 return f->matches==1&&(f->attrs&SE_GROUP_ENABLED)&&!(f->attrs&SE_GROUP_USE_FOR_DENY_ONLY);
}
static unsigned checks;
#define C(x) do{checks++;if(!(x)){printf("FAIL candidate line=%d\n",__LINE__);return 1;}}while(0)
int main(void){
 R_FACTS base={1,1,1,1,SERVICE_SID_TYPE_UNRESTRICTED,1,0,0,0,0,0,1,0,0},f;
 const unsigned modes[]={0,13,14,4,15,18,1,16,17};unsigned i,n;DWORD e;
 C(prior_actual_runtime_model_main()==0);C(ids());
 for(i=0;i<sizeof(modes)/sizeof(modes[0]);i++){
  ZeroMemory(&native.runtime,sizeof(native.runtime));ZeroMemory(&host,sizeof(host));ZeroMemory(&psrt_record,sizeof(psrt_record));model_stop_flag=0;model_mode=modes[i];
  e=psr_token();C((e==ERROR_SUCCESS)==(modes[i]==0));
  if(modes[i]==13){C(psrt_record.predicate&&!strcmp(psrt_record.predicate,"token.specific_group.enabled"));C(psrt_record.value_a==0&&psrt_record.value_b==0);}
  C(psr_close()==0);
 }
 C(candidate(&base));C(!candidate(NULL));
 f=base;f.matches=1;f.attrs=SE_GROUP_ENABLED;C(candidate(&f));
 for(n=0;n<21;n++){
  f=base;
  switch(n){
   case 0:f.primary=0;break;case 1:f.user_exact=0;break;case 2:f.stable=0;break;case 3:f.query_complete=0;break;
   case 4:f.sid_type=SERVICE_SID_TYPE_NONE;break;case 5:f.sid_type=SERVICE_SID_TYPE_RESTRICTED;break;
   case 6:f.generic_service=0;break;case 7:f.admin_enabled=1;break;case 8:f.matches=1;f.attrs=0;break;
   case 9:f.matches=1;f.attrs=SE_GROUP_ENABLED_BY_DEFAULT|SE_GROUP_OWNER;break;
   case 10:f.matches=1;f.attrs=SE_GROUP_USE_FOR_DENY_ONLY;break;
   case 11:f.matches=1;f.attrs=SE_GROUP_ENABLED|SE_GROUP_USE_FOR_DENY_ONLY;break;
   case 12:f.matches=2;f.attrs=SE_GROUP_ENABLED;break;case 13:f.attrs=SE_GROUP_ENABLED;break;
   case 14:f.restricted_count=1;break;case 15:f.filtered_history=1;break;case 16:f.effective_access=0;break;
   case 17:f.stop=1;break;case 18:f.close_unknown=1;break;case 19:f.user_exact=0;f.matches=1;f.attrs=SE_GROUP_ENABLED;break;
   default:f.query_complete=0;f.matches=1;f.attrs=SE_GROUP_ENABLED;break;
  }C(!candidate(&f));
 }
 printf("IDENTITY_RULE_CANDIDATE=PASS checks=%u; ACTUAL_OLD_PREDICATE_UNCHANGED; PROSPECTIVE_NOT_ACCEPTED_NOT_INTEGRATED; NO_SCM_NO_START\n",checks);return 0;
}
