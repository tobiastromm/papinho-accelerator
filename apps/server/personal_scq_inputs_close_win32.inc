static DWORD personal_scq_inputs_close(PAPACC_PERSONAL_SCQ_INPUTS *c)
{
 unsigned i;
 if(!c||c->phase==3U)return 0;
 if(!c->phase){
  if(c->image_chain.phase){if(scqi_chain_close(&c->image_chain)){c->phase=4U;return ERROR_INVALID_STATE;}c->phase=3U;}
  return 0;
 }
 if(c->image_chain.phase&&(c->image_chain.self!=&c->image_chain||!c->image_chain.owner)){
  c->phase=4U;return ERROR_INVALID_STATE; /* ownership unknown: retain entire cohort */
 }
 if(c->thread!=GetCurrentThreadId()||c->phase==4U||c->image_chain.phase==4U||c->content_hash.active||c->content_hash.retained)return ERROR_INVALID_STATE;
 for(i=0;i<3;i++)if(c->reader[i]){if(c->close_attempts&(8U<<i))goto unknown;c->close_attempts|=8U<<i;
  if(!CloseHandle(c->reader[i]))goto unknown;c->reader[i]=NULL;}
 for(i=0;i<3;i++)if(c->file[i]){
  if(c->close_attempts&(1U<<i))goto unknown;c->close_attempts|=1U<<i;
  if(!CloseHandle(c->file[i]))goto unknown;c->file[i]=NULL;
 }
 for(i=0;i<3;i++)if(c->descriptor[i]){if(LocalFree(c->descriptor[i]))goto unknown;c->descriptor[i]=NULL;}
 if(c->checking){if(LocalFree(c->checking))goto unknown;c->checking=NULL;}
 if(scqi_chain_close(&c->image_chain))goto unknown;
 c->phase=3;return 0;
unknown:c->phase=4;return personal_scq_inputs_fail(c,10,ERROR_INVALID_HANDLE);
}
