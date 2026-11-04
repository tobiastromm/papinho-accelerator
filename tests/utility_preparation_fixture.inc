/* Exact owner/preparation functions above; every native effect/process API is
 * modeled by the shared fixture. Never executes papacc_server/papacc_update. */
static int utility_preparation_suite(void)
{
 FILE *out=tmpfile();BYTE pins[5][32];DWORD parent[3]={42,0,3},old[3]={42,0,4};
 PSECURITY_DESCRIPTOR saved;unsigned i;int result;
 if(!out||!ConvertStringSecurityDescriptorToSecurityDescriptorW(L"O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;GRGX;;;AU)",SDDL_REVISION_1,&good_sd,NULL))return 2;
 saved=good_sd;
 for(i=200U;i<=252U;i++){
  PAPACC_STAGED_UPDATE_STATE *u;unsigned before;int success=i==200U||i==208U||i==245U||i==252U;
  const WCHAR *source=i==252U?L"C:\\A\\B\\C\\D\\E\\F\\papacc_update.exe":i==249U?L"C:\\A\\B\\C\\D\\E\\F\\G\\papacc_update.exe":i==222U?L"C:\\Build\\papacc_server.exe":L"C:\\Build\\papacc_update.exe";
  fixture_reset(0);update_case=i;scm_opens=scm_closes=scm_queries=0;set_calls=old_moved=new_moved=0;
  duplicate_denied=prep_duplicate_denied=0;update_installs=update_uninstalls=0;
  T(update_pin(pins[0],4)&&update_pin(pins[1],9)&&update_pin(pins[2],7)&&update_pin(pins[3],8)&&update_pin(pins[4],6));
  if(i==252U)T(update_pin(pins[1],14)&&update_pin(pins[2],12)&&update_pin(pins[3],13)&&update_pin(pins[4],11));
  parent[0]=i==223U?43U:42U;parent[2]=i==241U?999U:3U;old[2]=i==242U?999U:4U;
  if(i==202U)pins[4][0]^=1;if(i==231U)pins[2][0]^=1;if(i==232U)pins[1][0]^=1;if(i==233U)pins[0][0]^=1;
  if(i==215U)scenario=15U;if(i==229U)scenario=3U;if(i==245U)scenario=6U;if(i==239U)scenario=21U;
  if(i==238U)capacity_case=9U;if(i==240U)fail_close=1U;
  if(i==220U)state=PAPACC_LAUNCHER_ACTIVE;if(i==221U)state=PAPACC_LAUNCHER_FINISHED;
  if(i==225U||i==226U||i==227U){
   if(!ConvertStringSecurityDescriptorToSecurityDescriptorW(i==225U?L"O:BUG:BAD:P(A;;FA;;;BA)":
    i==226U?L"O:BAG:BAD:P(A;;FA;;;BA)(A;;FA;;;AU)":L"O:BAG:BAD:(A;;FA;;;BA)(A;;GRGX;;;AU)",SDDL_REVISION_1,&good_sd,NULL))return 2;
  }
  if(i==200U||i==251U){
   char values[5][65];const char *args[14]={"papacc_server.exe","--owner-manual-prepare-update-utility","C:\\Build\\papacc_update.exe",
    "42","0","3","42","0","4",values[0],values[1],values[2],values[3],values[4]};unsigned a,b;
   for(a=0;a<5U;a++){for(b=0;b<32U;b++)sprintf_s(values[a]+2U*b,3,"%02x",pins[a][b]);values[a][64]=0;}
   if(i==251U)values[1][0]='Z';result=papacc_server_launcher_utility_prep_cli(14,args,out,out);
  }else result=utility_prep_entry(source,parent,old,(const BYTE (*)[32])pins,out);
  u=staged_update_state();
  if(success!=(result==0))printf("PREP case=%u result=%d first=%lu location=%lu phase=%u prepared=%u act=%u consumed=%u creates=%u effects=%u writes=%u chain=%u\n",
   i,result,(unsigned long)u->first,(unsigned long)u->first_location,u->phase,u->prepared,u->act_state,u->consumed,creates,u->effects,writes,u->prep_chain_count);
  T(success==(result==0));T(!scm_opens&&!scm_queries&&!scm_closes&&!set_calls&&!old_moved&&!new_moved);T(creates<=1U);
  if(success){BYTE digest[32];T(u->preparation_mode==1U&&u->prepared&&u->created&&u->consumed&&u->phase==4U&&staging_act()->phase==6U);
   T(prep_duplicate_denied==1U&&duplicate_denied==1U&&u->written==6001U);
   T(pacc_storage_hash_callback(NULL,files[4].data,files[4].size,digest)&&!memcmp(digest,pins[0],32));
   before=creates;T(utility_prep_apply()!=0&&creates==before);T(staged_update_apply()!=0&&!set_calls);
  }else if(u->effects){T(state==PAPACC_LAUNCHER_RETAINED);before=closes;T(utility_prep_close()!=0&&closes==before);}
  if(!u->effects)T(!u->consumed&&!creates&&!writes);
  if(i==225U||i==226U||i==227U){LocalFree(good_sd);good_sd=saved;}
 }
 fixture_reset(0);LocalFree(good_sd);fclose(out);
 printf("PASS UTILITY_PREPARATION cases=53 checks=%u SAME_OWNER_REAL_ACT_API_MODELED ONE_EXCLUSIVE_FILE FULL_SIX_DIRECTORY_CHAIN ZERO_SCM_RENAMES_START_ENDPOINT_RNG_REAL NO_RETRY_NO_DELETE_OPAQUE_UNKNOWN\n",checks);
 return 0;
}
