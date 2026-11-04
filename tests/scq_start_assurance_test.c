/* G0-G9 machines with original-source transitions; native acquisition,
 * console and resource observations MODELED. No operational limits selected. */
#define PAPACC_SCQ_PRE_READINESS_ENTRY preserved_g0_chain_main
#include "scq_pre_readiness_chain_test.c"
#include "../apps/server/personal_scq_start_assurance_win32.inc"
#include "../apps/server/personal_scq_start_plan_data_win32.inc"
static SESSION_FIXTURE g0_fixtures[33];static PAPACC_SCQ_START_ASSURANCE gg[33];static PAPACC_SCQ_ADMIN_SOURCE gs[33];
static DWORD g_reply(PAPACC_SCQ_ADMIN_SOURCE *s,SESSION_FIXTURE *f,unsigned kind)
{
 static const char *prefixes[]={"OWNER_SCQ ","OWNER_SCQ_PRIVATE_ENDPOINT ","OWNER_SCQ_DIAGNOSTIC_START "};
 char reply[100];size_t n=strlen(prefixes[kind]);unsigned j;
 memcpy(reply,prefixes[kind],n);
 for(j=0;j<32U;j++){reply[n+2U*j]="0123456789abcdef"[f->plan.attempt[j]>>4];reply[n+2U*j+1U]="0123456789abcdef"[f->plan.attempt[j]&15U];}reply[n+64U]=0;
 if(kind==0U)return scqa_confirm(s,&f->administrator_owner,reply,n+64U,&f->stop);
 if(kind==1U)return scqa_endpoint_confirm(s,&f->administrator_owner,reply,n+64U,&f->stop);
 return scqa_start_confirm(s,&f->administrator_owner,reply,n+64U,&f->stop);
}
#include <stdarg.h>
static char failure_projection[4096];
static int failure_write(FILE *out,const char *format,...)
{int n;va_list args;(void)out;va_start(args,format);n=vsnprintf(failure_projection,sizeof(failure_projection),format,args);va_end(args);return n;}
#define fprintf failure_write
#include "../apps/server/personal_scq_resource_failure_win32.inc"
#undef fprintf
static int failure_projection_cases(void)
{
 PAPACC_SCQ_START_ASSURANCE g,before;unsigned i;
 for(i=0;i<8U;i++){
  memset(&g,0,sizeof(g));g.p1_handles=1;g.phase=3;g.first=ERROR_NOT_ENOUGH_MEMORY;
  scqg_selected_p1(&g.limits);g.observations.attempts=1;g.observations.samples=i==0U?0U:1U;
  g.observations.last.handles=118;g.observations.last.private_bytes=4000000;
  g.observations.last.working_set=9000000;g.observations.last.physical_available=8000000000ULL;
  g.observations.last.commit_available=9000000000ULL;
  if(i==2U){g.observations.first_error=ERROR_ACCESS_DENIED;g.observations.error_stage=3;}
  if(i==3U)g.first=ERROR_NOT_ENOUGH_QUOTA;if(i==4U)g.first=ERROR_INVALID_DATA;
  if(i==5U)g.first=ERROR_TIMEOUT;if(i==6U)g.first=ERROR_NOT_SUPPORTED;
  before=g;failure_projection[0]=0;
  scqg_failure_projection((FILE *)1,&g,g.first);
  C(!memcmp(&g,&before,sizeof(g)));
  C(strstr(failure_projection,"EXISTING_SAMPLE_NOT_NEW_QUERY")&&strstr(failure_projection,"ZERO_WITHOUT_SAMPLE_NOT_EVIDENCE"));
  C(strstr(failure_projection,"handles=118 cap=192 guard=188"));
  C(strstr(failure_projection,"commit=9000000000 commit_floor=3221225472"));
  if(i==0U)C(strstr(failure_projection,"samples=0"));
  if(i==2U)C(strstr(failure_projection,"query_error=5 query_stage=3"));
 }
 failure_projection[0]=0;scqg_failure_projection(NULL,&g,g.first);C(!failure_projection[0]);
 printf("PASS RESOURCE_FAILURE_PROJECTION cases=9 NO_NEW_QUERY_NO_STATE_CHANGE\n");return 0;
}
static int commit_floor_cases(void)
{
 PAPACC_SCQ_START_LIMITS p1,legacy,bad;PAPACC_SCQ_START_ASSURANCE q;
 PAPACC_SCM_RESOURCE_SAMPLE sample={1000000U,1000U,1010U,2000000U,3000000U,8000000000ULL,0U,128U,1U};
 BYTE current[PAPACC_SCQ_G3_PREIMAGE_BYTES],prior[PAPACC_SCQ_G3_PREIMAGE_BYTES];
 volatile LONG stop=0;int owner=0;unsigned i;ULONGLONG old_floor=8589934592ULL;DWORD e;
 scqg_selected_p1(&p1);scqg_selected(&legacy);
 C(p1.commit_floor==3221225472ULL&&legacy.commit_floor==9823326208ULL);
 C(p1.physical_floor==legacy.physical_floor&&p1.private_growth==legacy.private_growth&&p1.working_growth==PAPACC_SCQ_P1_WORKING_GROWTH&&legacy.working_growth==1994944U&&
  p1.freshness_ms==legacy.freshness_ms&&p1.duration_ms==legacy.duration_ms);
 for(i=0;i<3U;i++){
  sample.handles=128U;sample.commit_available=3221225471ULL+i;memset(&q,0,sizeof(q));
  e=scqg_query_ready_mode(&q,&owner,&sample,1010U,100U,60100U,&stop,1);
  C((e==0)==(i>0U));C(!q.source&&q.limits.commit_floor==3221225472ULL);
  if(i>0U){PAPACC_SCM_RESOURCE_SAMPLE next=sample;next.begin=1011U;next.end=1012U;C(q.phase==5U);C(!scqg_query_admit(&q,&owner,&next,1012U,101U,0,&stop));}
  /* Non-P1 QUERY retains its old handle guard AND commit floor. */
  sample.handles=100U;memset(&q,0,sizeof(q));C(scqg_query_ready_mode(&q,&owner,&sample,1010U,100U,60100U,&stop,0)==ERROR_NOT_ENOUGH_MEMORY);
  C(q.limits.commit_floor==9823326208ULL&&!q.p1_handles);
 }
 sample.commit_available=9823326208ULL;memset(&q,0,sizeof(q));
 C(!scqg_query_ready_mode(&q,&owner,&sample,1010U,100U,60100U,&stop,0));
 C(!scqg_selected_exact_mode(&p1,240000U,0));
 bad=p1;bad.max_handles=128U;C(!scqg_selected_exact_mode(&bad,240000U,0));
 C(!scqg_selected_exact_mode(&legacy,240000U,1));
 for(i=0;i<6U;i++){
  bad=p1;bad.commit_floor=i==0U?0U:i==1U?3221225471ULL:i==2U?3221225473ULL:i==3U?8589934592ULL:i==4U?9823326208ULL:~0ULL;
  C(!scqg_selected_exact_mode(&bad,240000U,1));
  C(scqg_plan_data_mode(&bad,240000U,current,1)!=0);
 }
 C(!scqg_selected_exact_mode(NULL,240000U,1));C(!scqg_selected_exact_mode(&p1,240001U,1));
 C(!scqg_plan_data_mode(&p1,240000U,current,1));memcpy(prior,current,sizeof(prior));
 for(i=0;i<8U;i++)prior[60U+i]=(BYTE)(old_floor>>(8U*i));
 C(memcmp(current,prior,sizeof(current))&&current[60]==0U&&current[63]==0xc0U&&current[64]==0U&&current[67]==0U);
 sample.commit_available=3221225472ULL;memset(&q,0,sizeof(q));
 C(scqg_query_ready_mode(&q,&owner,NULL,1010U,100U,60100U,&stop,1)!=0);
 memset(&q,0,sizeof(q));sample.frequency=0;
 C(scqg_query_ready_mode(&q,&owner,&sample,1010U,100U,60100U,&stop,1)!=0);sample.frequency=1000000U;
 memset(&q,0,sizeof(q));sample.frequency=~0ULL;
 C(scqg_query_ready_mode(&q,&owner,&sample,1010U,100U,60100U,&stop,1)!=0);sample.frequency=1000000U;
 memset(&q,0,sizeof(q));stop=1;C(scqg_query_ready_mode(&q,&owner,&sample,1010U,100U,60100U,&stop,1)!=0);stop=0;
 memset(&q,0,sizeof(q));C(!scqg_query_ready_mode(&q,&owner,&sample,1010U,100U,60100U,&stop,1));
 q.phase=4U;C(scqg_query_admit(&q,&owner,&sample,1010U,101U,0,&stop)!=0);
 printf("PASS P1_COMMIT_FLOOR BOUNDARIES_3GIB LEGACY_FLOOR_UNCHANGED EXACT_POLICY_NO_FALLBACK NULL_QUERY_OVERFLOW_STOP_UNKNOWN_NO_AUTHORITY\n");return 0;
}
static int working_growth_cases(void)
{
 PAPACC_SCQ_START_LIMITS p1,legacy,bad;PAPACC_SCQ_START_ASSURANCE q;
 PAPACC_SCM_RESOURCE_SAMPLE sample={1000000U,1000U,1010U,2000000U,3000000U,8000000000ULL,11000000000ULL,100U,1U},next;
 volatile LONG stop=0;int owner=0;unsigned i;BYTE current[84],previous[84];DWORD e;
 scqg_selected_p1(&p1);scqg_selected(&legacy);
 C(p1.working_growth==3145728ULL&&legacy.working_growth==1994944U&&p1.private_growth==1994944U);
 C(scqg_selected_exact_mode(&p1,240000U,1)&&!scqg_selected_exact_mode(&p1,240000U,0));
 bad=p1;bad.working_growth=1994944U;C(!scqg_selected_exact_mode(&bad,240000U,1));C(scqg_plan_data_mode(&bad,240000U,previous,1)!=0);
 C(!scqg_plan_data_mode(&p1,240000U,current,1));memcpy(previous,current,sizeof(current));
 for(i=0;i<8;i++)previous[44U+i]=(BYTE)(1994944ULL>>(i*8U));C(memcmp(current,previous,sizeof(current))!=0);
 for(i=0;i<3;i++){
  memset(&q,0,sizeof(q));C(!scqg_query_ready_mode(&q,&owner,&sample,1010U,100U,60100U,&stop,1));
  next=sample;next.begin=1011U;next.end=1012U;next.working_set=sample.working_set+3145727ULL+i;
  e=scqg_query_admit(&q,&owner,&next,1012U,101U,0,&stop);C((e==0)==(i<2));
 }
 memset(&q,0,sizeof(q));C(!scqg_query_ready_mode(&q,&owner,&sample,1010U,100U,60100U,&stop,1));
 next=sample;next.begin=1011U;next.end=1012U;next.working_set+=2129920U;
 C(!scqg_query_admit(&q,&owner,&next,1012U,101U,0,&stop));
 memset(&q,0,sizeof(q));C(!scqg_query_ready_mode(&q,&owner,&sample,1010U,100U,60100U,&stop,0));
 C(scqg_query_admit(&q,&owner,&next,1012U,101U,0,&stop)==ERROR_NOT_ENOUGH_MEMORY);
 memset(&q,0,sizeof(q));C(!scqg_query_ready_mode(&q,&owner,&sample,1010U,100U,60100U,&stop,1));
 next.private_bytes+=1994945U;C(scqg_query_admit(&q,&owner,&next,1012U,101U,0,&stop)==ERROR_NOT_ENOUGH_MEMORY);
 for(i=0;i<3;i++){
  memset(&q,0,sizeof(q));C(!scqg_query_ready_mode(&q,&owner,&sample,1010U,100U,60100U,&stop,1));
  next=sample;next.begin=1011U;next.end=1012U;
  if(i==0)next.working_set=~0ULL;if(i==1)next.frequency=0;if(i==2)stop=1;
  C(scqg_query_admit(&q,&owner,&next,1012U,101U,0,&stop)!=0);stop=0;
 }
 printf("PASS P1_WORKING_GROWTH 3MIB_BOUNDARIES HISTORICAL_SAMPLE_ONLY_MODEL LEGACY_PRIVATE_UNCHANGED OLD_POLICY_DENIED OVERFLOW_UNKNOWN_STOP_NO_REAL_QUERY\n");return 0;
}
int main(void)
{
 unsigned i,before=checks;DWORD e;PAPACC_PERSONAL_SCQ_ORIGINAL v;
 C(!failure_projection_cases());
 C(!commit_floor_cases());
 C(!working_growth_cases());
 {PAPACC_SCQ_START_LIMITS a={0x100000001ULL,2U,3U,4U,128U,6U,7U},b;
  BYTE original[PAPACC_SCQ_G3_PREIMAGE_BYTES],changed[PAPACC_SCQ_G3_PREIMAGE_BYTES];DWORD lifetime;
  C(!scqg_plan_data(&a,8U,original));C(!memcmp(original,"G3P1",4)&&original[4]==0xd6&&original[35]==0x1b);
  C(original[36]==1U&&original[40]==1U&&original[68]==128U&&original[80]==8U);
  /* Each quantitative choice, including high bits and authorization lifetime,
   * changes the hash preimage; the prior constant-only policy did not. */
  for(i=0;i<8U;i++){
   b=a;lifetime=8U;
   if(i==0U)b.private_growth^=0x100000000ULL;if(i==1U)b.working_growth++;
   if(i==2U)b.physical_floor++;if(i==3U)b.commit_floor++;if(i==4U)b.max_handles--;
   if(i==5U)b.freshness_ms++;if(i==6U)b.duration_ms++;if(i==7U)lifetime++;
   C(!scqg_plan_data(&b,lifetime,changed)&&memcmp(original,changed,sizeof(original)));
  }
  C(scqg_plan_data(&a,0,changed)!=0);b=a;b.max_handles=129U;C(scqg_plan_data(&b,8U,changed)!=0);
  b=a;b.physical_floor=0;C(scqg_plan_data(&b,8U,changed)!=0);
 }
 C(ConvertStringSidToSidA("S-1-5-80-111-222-333-444-555",&virtual_sid));C(ConvertStringSidToSidA("S-1-5-21-111-222-333-1001",&sam_sid));
 for(i=0;i<33U;i++){
  SESSION_FIXTURE *f=&g0_fixtures[i];PAPACC_SCQ_ADMIN_SOURCE *s=&gs[i];PAPACC_SCQ_START_ASSURANCE *g=&gg[i];
  PAPACC_SCQ_START_LIMITS limits={1000U,1000U,100000U,200000U,128U,10U,100U};
  PAPACC_SCM_RESOURCE_SAMPLE sample={1000U,100U,101U,1000U,2000U,110000U,210000U,100U,1U};
  PAPACC_SCQ_START_ASSURANCE copied;ULONGLONG now=101U;
  C(!setup_session_purpose(f,1,1U,i==2U?PAPACC_SCQ_PURPOSE_READ:PAPACC_SCQ_PURPOSE_TRANSPORT_ONLY));
  f->policy.binary=L"C:\\Program Files\\Papinho\\Accelerator";f->policy.parent=L"C:\\ProgramData\\Papinho";f->policy.data=L"C:\\ProgramData\\Papinho\\Accelerator";
  f->policy.source=s;v=f->original;v.controller=&f->administrator_owner;
  C(!scqa_capture(s,&f->administrator_owner,&f->policy,&f->controller_profile,&v,&f->stop));
  if(i==1U)memset(&limits,0,sizeof(limits));if(i==3U)limits.max_handles=129U;
  if(i==4U)s->phase=3U;if(i==31U)limits.private_growth=0;if(i==32U)limits.freshness_ms=0;
  e=scqg_capture(g,&f->administrator_owner,s,&limits,&f->stop);
  if(i==1U||i==2U||i==3U||i==4U||i==31U||i==32U){C(e!=0&&!g->phase);continue;}
  C(!e&&g->phase==1U&&!g->presented&&!s->value.controller_consumed&&!s->start_consumed&&!s->endpoint_consumed);
  C(scqg_capture(g,&f->administrator_owner,s,&limits,&f->stop)!=0);
  if(i==29U)continue;
  if(i==5U){f->policy.target[0]^=1U;C(scqg_presented(g,&f->administrator_owner,&f->stop)!=0);continue;}
  if(i!=6U)C(!scqg_presented(g,&f->administrator_owner,&f->stop)); /* native output boundary modeled */
  if(i!=28U){
   C(!scqa_present(s,&f->administrator_owner,&f->stop));C(!g_reply(s,f,0U));
   if(i!=7U){C(!scqa_endpoint_present(s,&f->administrator_owner,&f->stop));C(!g_reply(s,f,1U));}
   if(i!=8U){C(!scqa_start_present(s,&f->administrator_owner,&f->stop));C(!g_reply(s,f,2U));}
  }
  if(i==9U)f->stop=1;if(i==10U)sample.handles=124U;if(i==11U)sample.handles=129U;
  if(i==12U)sample.physical_available=limits.physical_floor-1U;if(i==13U)sample.commit_available=limits.commit_floor-1U;
  if(i==14U)sample.frequency=0;if(i==15U)sample.end=sample.begin-1U;if(i==16U)now+=20U;
  if(i==23U){C(!scqs_prepare(&f->controller,&f->administrator_owner,1U,&f->controller_profile,s,&f->controller_receiver,&f->controller_dispatch,&f->runtime));C(!scqs_take(&f->controller,&f->administrator_owner,&f->plan.administrator,NULL,f->plan.refs,&f->stop));}
  if(i==27U)sample.private_bytes=((ULONGLONG)-1)-10U;
  if(i==30U)sample.frequency=(ULONGLONG)-1;
  e=scqg_ready(g,i==22U?(const void *)&f->runtime_owner:(const void *)&f->administrator_owner,&sample,now,&f->stop);
  if((i>=6U&&i<=16U)||i==22U||i==23U||i==28U||i==30U){C(e!=0);continue;}
  C(!e&&g->phase==2U&&g->ready.private_bytes==sample.private_bytes);
  C(scqg_ready(g,&f->administrator_owner,&sample,now,&f->stop)!=0); /* cannot rebaseline */
  sample.begin=102U;sample.end=now=103U;
  if(i==17U)sample.private_bytes+=limits.private_growth+1U;if(i==18U)sample.working_set+=limits.working_growth+1U;
  if(i==19U)sample.begin=sample.end=now=1000U;
  if(i==20U)sample.frequency++;
  if(i==21U){sample.begin=99U;sample.end=100U;}
  if(i==24U)f->policy.attempt[0]^=1U;
  if(i==26U)f->stop=1;
  if(i==27U)sample.private_bytes=(ULONGLONG)-1;
  copied=*g;
  e=scqg_admit(i==25U?&copied:g,&f->administrator_owner,&sample,now,1,&f->stop);
  if((i>=17U&&i<=21U)||(i>=24U&&i<=26U))C(e!=0);
  else C(!e&&!s->value.controller_consumed&&!s->start_consumed&&!s->endpoint_consumed);
 }
 C(!mcreated&&!mopened&&!mevents&&!cs_running);
 {PAPACC_SCQ_START_ASSURANCE query[15];volatile LONG stop=0;unsigned owner=0,k;
  memset(query,0,sizeof(query));
  for(k=0;k<15U;k++){
   PAPACC_SCM_RESOURCE_SAMPLE sample={1000000U,1000U,1010U,1000000U,2000000U,8000000000ULL,11000000000ULL,100U,1U};
   ULONGLONG until=60100U,now=1010U,tick=100U;DWORD result;stop=0;
   if(k==1U)until=60101U;if(k==2U)until=100U;if(k==3U)sample.handles=124U;
   if(k==4U)sample.physical_available=7239172095ULL;if(k==5U)sample.commit_available=9823326207ULL;
   result=scqg_query_ready(&query[k],&owner,&sample,now,tick,until,&stop);
   if(k>=1U&&k<=5U){C(result!=0);continue;}C(!result&&!query[k].source&&query[k].phase==5U);
   C(scqg_query_ready(&query[k],&owner,&sample,now,tick+1U,until+1U,&stop)!=0&&query[k].global_until==60100U);
   sample.begin=1020U;sample.end=1030U;now=1040U;
   if(k==6U)now=101021U; /* query took 10us, but stale BEFORE the effect */
   if(k==7U)tick=60100U;if(k==8U)sample.private_bytes+=1994945U;
   if(k==9U)sample.working_set+=1994945U;if(k==10U)sample.handles=129U;
   if(k==11U)stop=1;if(k==12U)sample.frequency++;
   if(k==13U)sample.begin=1041U;
   result=scqg_query_admit(&query[k],&owner,&sample,now,tick,0,&stop);
   if(k>=6U&&k<=13U)C(result!=0);else C(!result&&!query[k].source);
  }
 }
 /* P1 policy is local and explicit. Legacy max alone never selects it. */
 {unsigned k;volatile LONG stop=0;int owner=0;PAPACC_SCQ_START_LIMITS p1,old;
  PAPACC_SCM_RESOURCE_SAMPLE sample={1000000U,1000U,1010U,2000000U,3000000U,8000000000ULL,11000000000ULL,128U,1U};
  PAPACC_SCQ_START_ASSURANCE q;BYTE a[PAPACC_SCQ_G3_PREIMAGE_BYTES],b[PAPACC_SCQ_G3_PREIMAGE_BYTES];
  scqg_selected_p1(&p1);scqg_selected(&old);
  C(scqg_selected_exact_mode(&p1,240000U,1)&&!scqg_selected_exact(&p1,240000U));
  C(!scqg_selected_exact_mode(&old,240000U,1)&&scqg_selected_exact(&old,240000U));
  C(scqg_plan_data(&p1,240000U,a)!=0&&!scqg_plan_data_mode(&p1,240000U,a,1));
  C(!scqg_plan_data(&old,240000U,b)&&memcmp(a,b,sizeof(a)));
  for(k=127U;k<=129U;k++){
   memset(&q,0,sizeof(q));sample.handles=k;
   e=scqg_query_ready_mode(&q,&owner,&sample,1010U,100U,60100U,&stop,1);
   C((e==0)==(k<=128U));C(q.initial_known&&q.initial_handles==k&&!q.source);
  }
  memset(&q,0,sizeof(q));sample.handles=128U;
  C(!scqg_query_ready_mode(&q,&owner,&sample,1010U,100U,60100U,&stop,1));
  for(k=187U;k<=189U;k++){sample.handles=k;C((scqg_sample(&q,&sample,1010U,1)==0)==(k==187U));}
  for(k=191U;k<=193U;k++){sample.handles=k;C((scqg_sample(&q,&sample,1010U,0)==0)==(k<=192U));}
  sample.handles=MAXDWORD;C(scqg_sample(&q,&sample,1010U,0)!=0);
  sample.handles=128U;sample.frequency=0;C(scqg_sample(&q,&sample,1010U,0)!=0);sample.frequency=1000000U;
  C(scqg_sample(&q,&sample,101001U,0)!=0); /* >100ms until effect */
  q.phase=4U;C(scqg_query_admit(&q,&owner,&sample,1010U,101U,0,&stop)!=0);
  q.phase=5U;stop=1;C(scqg_query_admit(&q,&owner,&sample,1010U,101U,0,&stop)!=0);stop=0;
  q.p1_handles=0;sample.handles=129U;C(scqg_sample(&q,&sample,1010U,0)!=0);
  sample.handles=124U;C(scqg_sample(&q,&sample,1010U,1)!=0);
  q.p1_handles=1;q.limits.max_handles=128U;sample.handles=100U;C(scqg_sample(&q,&sample,1010U,0)!=0);
 }
 printf("PASS G0_G9_ASSURANCE P1_BOUNDARIES_AND_LEGACY_ISOLATION cases=33 query_resource_cases=15 plan_field_mutations=8 checks=%u SELECTED_QUERY_G3_FRESH_UNTIL_EFFECT_NO_RENEWAL EXACT_G3_POLICY_PREIMAGE MODEL_OBSERVATIONS UNKNOWN_OPAQUE_NOT_ZERO NO_ENDPOINT_NO_START_NO_RNG_NO_KSP\n",checks-before);return 0;
}
