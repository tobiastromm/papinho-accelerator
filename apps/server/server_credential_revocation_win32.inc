/* Effect-only part of the existing owner. The canonical root, not this
 * routine, selects/authorizes an exact credential cohort. No native close. */
static PAPACC_RESULT credential_stop_cohort(PAPACC_C2_OPERATION_MANAGER *m,PAPACC_C2_SESSION *sessions,unsigned mask,PAPACC_CREDENTIAL_REVOCATION_RECEIPT *receipt)
{
 unsigned i,j;PAPACC_RESULT result;PAPACC_C2_OPERATION *operation;
 if(!m||!sessions||!receipt||!m->initialized||m->owner!=(PAPACC_U32)GetCurrentThreadId()||(mask&~15U))return PAPACC_RESULT_INVALID_STATE;
 /* Validate every selected Session before invalidating the first one. */
 for(i=0;i<4U;i++)if(mask&(1U<<i)){
  int attached=0;
  if(!sessions[i].initialized||sessions[i].owner!=m->owner)return PAPACC_RESULT_INVALID_STATE;
  for(j=0;j<4U;j++)if(m->sessions[j].session==&sessions[i])attached++;
  if(attached!=1)return PAPACC_RESULT_INVALID_STATE;
 }
 for(i=0;i<4U;i++)if(mask&(1U<<i)){
  result=papacc_c2_session_revoke(&sessions[i],m->owner,PAPACC_FALSE);
  if(result==PAPACC_RESULT_OK)receipt->sessions_invalidated++;
  else if(!receipt->first_error)receipt->first_error=result;
  for(j=0;j<2U;j++){
   operation=&m->operations[j];
   if(operation->session!=&sessions[i]||operation->state==PAPACC_C2_OP_EMPTY||operation->state==PAPACC_C2_OP_RELEASED)continue;
   /* Never convert a retained graph back to TERMINATING or release it. */
   if(operation->state==PAPACC_C2_OP_RETAINED){
    result=operation->revoked?PAPACC_RESULT_OK:PAPACC_RESULT_INVALID_STATE;
   }else result=papacc_c2_operation_revoke(m,operation,2U);
   if(result==PAPACC_RESULT_OK)receipt->operations_stop_requested++;
   else if(!receipt->first_error)receipt->first_error=result;
  }
 }
 if(receipt->first_error){receipt->uncertain=1U;return receipt->first_error;}
 return PAPACC_RESULT_OK;
}
