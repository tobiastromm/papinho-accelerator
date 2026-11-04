#ifndef PAPACC_SCQC_CLOSE_WIN32_INC
#define PAPACC_SCQC_CLOSE_WIN32_INC
static DWORD scqc_close_raw(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&o->pending_commit.personal_native;unsigned i,j;
 for(j=0;j<3U;j++){i=j==0U?2U:j-1U;
  if(n->scq_cold_raw[i]){
   if(n->scq_cold_close&(1U<<i))goto unknown;n->scq_cold_close|=1U<<i;
   if(!CloseHandle(n->scq_cold_raw[i]))goto unknown;n->scq_cold_raw[i]=NULL;
  }
 }return 0;
unknown:o->unresolved=1;return ERROR_IO_INCOMPLETE;
}
#endif
