/* Same production entry/owner, API-mocked native IO/process/SCM/RNG only.
 * No product or protected object is executed, created, renamed or queried. */
static int utility_replacement_suite(void)
{
 FILE *out=tmpfile();BYTE pins[5][32];DWORD parent[3]={42,0,3},old[3]={42,0,4};
 PSECURITY_DESCRIPTOR saved;unsigned i;int result;
 if(!out||!ConvertStringSecurityDescriptorToSecurityDescriptorW(L"O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;GRGX;;;AU)",SDDL_REVISION_1,&good_sd,NULL))return 2;
 saved=good_sd;
 for(i=300U;i<=328U;i++){
  PAPACC_STAGED_UPDATE_STATE *u;unsigned before;int success=i==300U||i==317U||i==318U;
  fixture_reset(0);update_case=i;scm_opens=scm_closes=scm_queries=0;set_calls=old_moved=new_moved=0;
  duplicate_denied=prep_duplicate_denied=0;update_installs=update_uninstalls=0;
  T(update_pin(pins[0],4)&&update_pin(pins[1],9)&&update_pin(pins[2],7)&&update_pin(pins[3],8)&&update_pin(pins[4],6));
  if(i==317U)T(update_pin(pins[1],14)&&update_pin(pins[2],12)&&update_pin(pins[3],13)&&update_pin(pins[4],11));
  if(i==309U)pins[0][0]^=1;if(i==310U)pins[1][0]^=1;if(i==311U)pins[4][0]^=1;
  if(i==328U)stop_now=1U;if(i==313U)scenario=18U;if(i==314U)capacity_case=9U;if(i==320U)state=PAPACC_LAUNCHER_ACTIVE;
  if(i==326U&&!ConvertStringSecurityDescriptorToSecurityDescriptorW(L"O:BAG:BAD:(A;;FA;;;SY)(A;;FA;;;BA)(A;;GRGX;;;AU)",SDDL_REVISION_1,&good_sd,NULL))return 2;
  if(i==312U&&!ConvertStringSecurityDescriptorToSecurityDescriptorW(L"O:BAG:BAD:P(A;;FA;;;BA)(A;;FA;;;AU)",SDDL_REVISION_1,&good_sd,NULL))return 2;
  if(i==318U||i==319U){
   char values[5][65];const char *args[14]={"papacc_server.exe","--owner-manual-update-utility","C:\\Build\\papacc_update.exe",
    "42","0","3","42","0","4",values[0],values[1],values[2],values[3],values[4]};unsigned a,b;
   for(a=0;a<5U;a++){for(b=0;b<32U;b++)sprintf_s(values[a]+2U*b,3,"%02x",pins[a][b]);values[a][64]=0;}
   result=papacc_server_launcher_utility_update_cli(i==319U?13:14,args,out,out);
  }else result=utility_bootstrap_entry(i==317U?L"C:\\A\\B\\C\\D\\E\\F\\papacc_update.exe":i==325U?L"C:\\Build\\papacc_server.exe":L"C:\\Build\\papacc_update.exe",parent,old,(const BYTE (*)[32])pins,out,2U);
  u=staged_update_state();
  if(success!=(result==0))printf("UTILITY_UPDATE case=%u result=%d first=%lu location=%lu mode=%u phase=%u consumed=%u created=%u sets=%u chain=%u\n",i,result,(unsigned long)u->first,(unsigned long)u->first_location,u->preparation_mode,u->phase,u->consumed,u->created,set_calls,u->prep_chain_count);
  T(success==(result==0));T(set_calls<=2U&&creates<=1U);
  if(success){BYTE digest[32];
   T(u->preparation_mode==2U&&u->consumed&&u->created&&u->rename_done==3U&&old_moved&&new_moved&&u->phase==4U);
   T(scm_opens==2U&&scm_closes==2U&&scm_queries>0U&&u->written==6001U);
   T(prep_duplicate_denied==1U&&duplicate_denied==1U);
   T(pacc_storage_hash_callback(NULL,files[4].data,files[4].size,digest)&&!memcmp(digest,pins[0],32));
   before=creates;T(utility_update_apply()!=0&&utility_prep_apply()!=0&&staged_update_apply()!=0&&creates==before);
  }else if(u->effects){
   T(state==PAPACC_LAUNCHER_RETAINED);before=closes;T(utility_prep_close()!=0&&closes==before);
  }else T(!u->consumed&&!writes&&!set_calls);
  if(i==302U)T(u->first==(DWORD)0xc0000043L&&!old_moved&&!new_moved);
  if(i==303U||i==304U||i==306U)T(old_moved&&!new_moved);
  if(i==305U)T(new_moved);
  if(i==307U||i==321U)T(u->consumed&&u->created&&!set_calls);
  if(i==323U)T(!u->consumed&&!u->effects);
  if(i==312U||i==326U){LocalFree(good_sd);good_sd=saved;}
 }
 fixture_reset(0);LocalFree(good_sd);fclose(out);
 printf("PASS UTILITY_REPLACEMENT cases=29 checks=%u SAME_OWNER_BOOTSTRAP_REAL_ACT_API_MODELED SCM_STOPPED_GATES SIX_DIRECTORY_CHAIN_EPHEMERAL_READBACK_KNOWN_CLOSE_REUSE NO_SERVER_WRITE_NO_SELF_UPDATE_NO_ROLLBACK_NO_RETRY\n",checks);
 return 0;
}
