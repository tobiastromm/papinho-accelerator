PAPACC_RESULT papacc_server_c2_owner_win32_step(PAPACC_SERVER_C2_OWNER_WIN32*o,PAPACC_U64 connection){unsigned i;PAPACC_C2_OWNER_WRITE*w=NULL;PAPACC_CONNECTION*c;PAPACC_SIZE n=0;PAPACC_TRANSPORT_IO_STATUS status;PAPACC_RESULT result;PAPACC_BOOL closed;
 int authority_result;PAPACC_BOOL credit_result;
 PAPACC_C2_S1_FACT fact;PAPACC_U64 fact_operation=0U,fact_generation=0U;PAPACC_U16 fact_type=0U;
#define S1_RETURN(expression) do{PAPACC_RESULT s1_returned=(expression);s1_finish(o,&fact,connection,fact_operation,fact_generation,fact_type,s1_returned);return s1_returned;}while(0)
 if(!valid(o))return PAPACC_RESULT_INVALID_STATE;
 memset(&fact,0,sizeof(fact));fact.phases=PAPACC_S1_ELIGIBILITY;
 for(i=0;i<6U;++i)if(o->writes[i].pending&&o->writes[i].connection==connection)w=&o->writes[i];if(w==NULL)S1_RETURN(PAPACC_RESULT_OK);
 fact.phases|=PAPACC_S1_ELIGIBLE;fact_operation=w->operation;fact_generation=w->generation;fact_type=w->type;
 fact.offset_before=fact.offset_after=(PAPACC_U64)w->offset;
 /* Observation-only identity check: membership precedes every dereference. */
 if(w->record!=NULL&&index_of(o,w->record)>=0&&w->record->id==w->operation&&w->record->generation==w->generation){fact.phases|=PAPACC_S1_STATE_BEFORE;fact.state_before=(PAPACC_U32)w->record->state;}
 observe(o,w,PAPACC_C2_PUBLICATION_ATTEMPT,0,0);
 authority_result=authority(o,w);fact.phases|=PAPACC_S1_AUTHORITY;fact.authority=(PAPACC_U32)authority_result;
 if(!authority_result){observe(o,w,PAPACC_C2_PUBLICATION_REJECTED,0,PAPACC_RESULT_INVALID_STATE);c=papacc_connection_manager_find(o->loop->connection_manager,connection);if(w->transport_attempted){w->pending=PAPACC_FALSE;w->quarantined=PAPACC_TRUE;if(c!=NULL&&!o->secure->dispatch_active&&o->secure->stop_state!=PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL)(void)papacc_connection_try_close(c,&closed);}else retire_write(o,w);S1_RETURN(PAPACC_RESULT_INVALID_STATE);}
 fact.credits_before=o->wake_credits;credit_result=papacc_server_c2_owner_win32_take_opportunity(o);
 fact.phases|=PAPACC_S1_CREDIT;fact.credits_after=o->wake_credits;
 if(credit_result)fact.phases|=PAPACC_S1_CREDIT_GRANTED;
 if(!credit_result)S1_RETURN(PAPACC_RESULT_OK);
 c=papacc_connection_manager_find(o->loop->connection_manager,connection);w->transport_attempted=PAPACC_TRUE;
 {PAPACC_SIZE offered=w->length-w->offset;char enabled[8];if(GetEnvironmentVariableA("PAPACC_C7_FRAGMENT",enabled,sizeof(enabled))==1U&&enabled[0]=='1'&&offered>31U)offered=31U;
 fact.phases|=PAPACC_S1_IO_CALL;fact.offered=(PAPACC_U64)offered;
 result=c->transport.write_fn(c->transport.context,w->bytes+w->offset,offered,&n,&status);
 fact.phases|=PAPACC_S1_IO_RESULT;fact.io_result=result;fact.accepted=(PAPACC_U64)n;
 /* On failure an adapter need not initialize status; never read it then. */
 if(result==PAPACC_RESULT_OK){fact.phases|=PAPACC_S1_IO_STATUS;fact.io_status=(PAPACC_U32)status;}
 if(GetEnvironmentVariableA("PAPACC_C7_METRICS",enabled,sizeof(enabled))==1U&&enabled[0]=='1')fprintf(stderr,"C7_PACC_WRITE pid=%lu operation=%llu generation=%llu connection=%llu type=%u remaining=%llu offered=%llu accepted=%llu status=%d result=%d\n",GetCurrentProcessId(),(unsigned long long)w->operation,(unsigned long long)w->generation,(unsigned long long)connection,w->type,(unsigned long long)(w->length-w->offset),(unsigned long long)offered,(unsigned long long)n,(int)status,(int)result);}

 if(n>w->length-w->offset)S1_RETURN(PAPACC_RESULT_INTERNAL_ERROR);
 if(result!=0||status==PAPACC_TRANSPORT_IO_STATUS_END_OF_STREAM){if(w->record!=NULL)(void)papacc_c2_operation_revoke(o->manager,w->record,4U);observe(o,w,PAPACC_C2_PUBLICATION_REJECTED,n,result==0?PAPACC_RESULT_INTERNAL_ERROR:result);w->pending=PAPACC_FALSE;w->quarantined=PAPACC_TRUE;S1_RETURN(result==0?PAPACC_RESULT_INTERNAL_ERROR:result);}
 if(w->type==22U&&n==0U&&status==PAPACC_TRANSPORT_IO_STATUS_WOULD_BLOCK){
  unsigned fi=(w->bytes[32]-1U)*2U+w->bytes[33];
  PAPACC_PST_SECURE_TRANSPORT_ADAPTER *adapter=(PAPACC_PST_SECURE_TRANSPORT_ADAPTER*)c->transport.context;
  if(w->record->flow[fi].blocked_writes!=(PAPACC_U64)-1)++w->record->flow[fi].blocked_writes;
  w->record->flow[fi].blocked_pst_operation=adapter->processor->last_io_operation;
 }
 w->offset+=n;fact.offset_after=(PAPACC_U64)w->offset;if(n!=0U)observe(o,w,PAPACC_C2_PUBLICATION_LOWER_ACCEPTED,n,0);
 if(w->offset==w->length){PAPACC_C2_OPERATION*r=w->record;PAPACC_U16 type=w->type;unsigned role=w->bytes[16+24],flow=(w->bytes[16+16]-1U)*2U+w->bytes[16+17];PAPACC_SIZE payload_bytes=w->length>=44U?w->length-44U:0U;
  observe(o,w,PAPACC_C2_PUBLICATION_FRAME_COMPLETE,w->length,0);retire_write(o,w);
  if(type==17U&&r!=NULL){result=papacc_c2_operation_result_written(o->manager,r);SecureZeroMemory(r->result,sizeof(r->result));S1_RETURN(result);}
  if(type==19U){PAPACC_CHANNEL*ch=channel_for(o,connection);S1_RETURN(ch==NULL?PAPACC_RESULT_INVALID_STATE:papacc_c2_operation_ack_written(o->manager,r,role,ch->channel_instance_id));}
  if(type==21U)S1_RETURN(papacc_c2_operation_tls_ready_written(o->manager,r));
  if(type==22U){result=papacc_c2_operation_output_accepted(o->manager,r,flow,payload_bytes);
   if(result==0&&flow==3U){PAPACC_C3_BRIDGE*b=o->bridges[index_of(o,r)];
    if(b==NULL)S1_RETURN(PAPACC_RESULT_INVALID_STATE);
    result=papacc_c3_bridge_commit_ab(b,r->id,r->generation,payload_bytes);}
   S1_RETURN(result);}
  if(type==23U||type==24U)S1_RETURN(papacc_c2_operation_fence_written(o->manager,r,flow,type==24U?PAPACC_TRUE:PAPACC_FALSE));
  if(type==27U){r->final_written=PAPACC_TRUE;
   if(r->ownership==1U){(void)papacc_c2_tokens_revoke(&r->tokens,o->manager->owner,PAPACC_FALSE);r->revoked=PAPACC_TRUE;r->state=PAPACC_C2_OP_RELEASED;}
   S1_RETURN(PAPACC_RESULT_OK);}
 }
 S1_RETURN(PAPACC_RESULT_OK);
}

#undef S1_RETURN
