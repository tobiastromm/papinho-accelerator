/* U0-U7 native act in the SAME cohort. No callback/provider, new pin or worker.
 * This phase has no qualified issuer UTC/status source and never resolves. */
static int v3act_group(WELL_KNOWN_SID_TYPE kind,int expected)
{
 BYTE sid[SECURITY_MAX_SID_SIZE];DWORD n=sizeof(sid);BOOL member=FALSE;
 return CreateWellKnownSid(kind,NULL,sid,&n)&&CheckTokenMembership(NULL,sid,&member)&&(member?1:0)==expected;
}
static int v3act_native(PAPACC_V3_UPDATE_ACT *held,PAPACC_V3_UPDATE_ACT *out)
{
 HANDLE token=NULL,thread=NULL;TOKEN_ELEVATION elevation;TOKEN_ELEVATION_TYPE type;
 TOKEN_STATISTICS stats;DWORD n,session,process_session,error;int ok=0;
 union { TOKEN_USER alignment; BYTE bytes[sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE]; } storage;
 TOKEN_USER *user=(TOKEN_USER *)storage.bytes;
 if((held->retained_token||held->retained_process_token))return 0;
 /* An impersonated caller cannot borrow the process identity. */
 if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&thread)){
  if(!CloseHandle(thread)){held->retained_token=thread;held->native_error=GetLastError();held->state=PAPACC_V3_ACT_RETAINED;}return 0;
 }
 if(GetLastError()!=ERROR_NO_TOKEN)return 0;
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&token))return 0;
 if(!GetTokenInformation(token,TokenStatistics,&stats,sizeof(stats),&n)||stats.TokenType!=TokenPrimary||
 !GetTokenInformation(token,TokenElevation,&elevation,sizeof(elevation),&n)||!elevation.TokenIsElevated||
 !GetTokenInformation(token,TokenElevationType,&type,sizeof(type),&n)||type==TokenElevationTypeLimited||
 !GetTokenInformation(token,TokenSessionId,&session,sizeof(session),&n)||!session||
 !ProcessIdToSessionId(GetCurrentProcessId(),&process_session)||session!=process_session||
 session!=WTSGetActiveConsoleSessionId()||!v3act_group(WinBuiltinAdministratorsSid,1)||
 !v3act_group(WinInteractiveSid,1)||!v3act_group(WinServiceSid,0)||
 !GetTokenInformation(token,TokenUser,&storage,sizeof(storage),&n)||!IsValidSid(user->User.Sid)||
 GetLengthSid(user->User.Sid)>sizeof(out->sid))goto done;
 memset(out,0,sizeof(*out));out->sid_bytes=GetLengthSid(user->User.Sid);
 if(!CopySid(sizeof(out->sid),out->sid,user->User.Sid))goto done;out->statistics=stats;
 /* All effective-identity checks are bounded by the same token observation. */
 if(!GetTokenInformation(token,TokenStatistics,&stats,sizeof(stats),&n)||
 memcmp(&out->statistics.TokenId,&stats.TokenId,sizeof(LUID))||
 memcmp(&out->statistics.AuthenticationId,&stats.AuthenticationId,sizeof(LUID))||
 memcmp(&out->statistics.ModifiedId,&stats.ModifiedId,sizeof(LUID)))goto done;
 if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&thread)){
  if(!CloseHandle(thread)){held->retained_token=thread;held->native_error=GetLastError();held->state=PAPACC_V3_ACT_RETAINED;}goto done;
 }
 if(GetLastError()!=ERROR_NO_TOKEN)goto done;ok=1;
done:
 error=GetLastError();
 if(!CloseHandle(token)){
  /* Never lose a second uncertain reference. A failed thread close above
   * already retains its handle; this branch cannot release either graph. */
  held->retained_process_token=token;
  held->native_error=GetLastError();held->state=PAPACC_V3_ACT_RETAINED;return 0;
 }
 if(!ok)held->native_error=error;return ok;
}
static int v3act_ready(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_V3_UPDATE_COHORT *u=&o->v3_update;unsigned j;
 if(!owner(o)||u->phase!=PAPACC_V3_UPDATE_HELD||!u->invocation||!u->candidate_serial||
 o->stopped||o->unresolved||o->authority_lost||o->committed_pending||pending_blocks(o)||
 o->pending_commit.thread||o->pending_commit.transaction||o->pending_commit.close_error||
 o->pending_commit.packet_format||o->pending_commit.result!=PAPACC_COMMIT_IDLE||(u->act.retained_token||u->act.retained_process_token)||
 u->candidate_index>=PAPACC_CONFIG_SLOTS||u->predecessor_pin.index>=PAPACC_CONFIG_SLOTS||
 u->predecessor_pin.index!=o->current||u->candidate_index==o->current||!o->prepared||o->pending!=u->candidate_index||
 o->slots[u->candidate_index].serial!=u->candidate_serial||o->slots[u->candidate_index].readers||
 o->slots[u->candidate_index].retained||o->v3_backings[u->candidate_index].serial||
 o->slots[o->current].serial!=u->predecessor_pin.serial||!o->slots[o->current].readers)return 0;
 for(j=0;j<32U;j++)if(o->tickets[j].ticket==u->predecessor_pin.ticket&&u->predecessor_pin.ticket&&
 o->tickets[j].serial==u->predecessor_pin.serial&&o->tickets[j].index==u->predecessor_pin.index)break;
 if((o->v3_backings[o->current].packet[8]!=3U&&o->v3_backings[o->current].packet[8]!=4U)||
 (o->v3_backings[u->candidate_index].packet[8]!=3U&&o->v3_backings[u->candidate_index].packet[8]!=4U)||j==32U||o->v3_backings[o->current].packet_bytes<PC_ROOT8_V3_MIN||
 o->v3_backings[o->current].packet_bytes>PC_ROOT8_V4_MAX||
 o->v3_backings[u->candidate_index].packet_bytes<PC_ROOT8_V3_MIN||
 o->v3_backings[u->candidate_index].packet_bytes>PC_ROOT8_V4_MAX||
 o->committed_bytes!=o->v3_backings[o->current].packet_bytes||
 o->pending_commit.previous_bytes!=o->committed_bytes||
 memcmp(o->committed,o->v3_backings[o->current].packet,o->committed_bytes)||
 memcmp(o->pending_commit.previous,o->committed,o->committed_bytes))return 0;
 return 1;
}
PAPACC_RESULT papacc_configuration_v3_update_act_match_data(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_V3_UPDATE_COHORT *u;PAPACC_CONFIG_V3_BACKING *b,*base;BYTE digest[32];
 if(!o||!v3act_ready(o))return PAPACC_RESULT_INVALID_STATE;u=&o->v3_update;
 if(u->act.state!=PAPACC_V3_ACT_CAPTURED)return PAPACC_RESULT_INVALID_STATE;
 b=&o->v3_backings[u->candidate_index];base=&o->v3_backings[o->current];
 if(b->packet_bytes<PC_ROOT8_V3_MIN||b->packet_bytes>PC_ROOT8_V4_MAX||
 base->packet_bytes<PC_ROOT8_V3_MIN||base->packet_bytes>PC_ROOT8_V4_MAX||
 o->committed_bytes!=base->packet_bytes||o->pending_commit.previous_bytes!=base->packet_bytes||
 memcmp(o->committed,base->packet,base->packet_bytes)||memcmp(o->pending_commit.previous,base->packet,base->packet_bytes)||
 !papacc_configuration_storage_hash(o,b->packet,b->packet_bytes,digest)||memcmp(digest,u->act.candidate_hash,32)||
 !papacc_configuration_storage_hash(o,base->packet,base->packet_bytes,digest)||memcmp(digest,u->act.predecessor_hash,32))return PAPACC_RESULT_INVALID_STATE;
 return PAPACC_RESULT_OK; /* Correlation ONLY, no native identity or status. */
}
PAPACC_RESULT papacc_configuration_v3_update_act_capture(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_U8 *selected,PAPACC_SIZE bytes)
{
 PAPACC_V3_UPDATE_COHORT *u;PAPACC_CONFIG_V3_BACKING *b;PAPACC_V3_UPDATE_ACT observed;
 if(!o||o->v3_update.data_active||!v3act_ready(o))return PAPACC_RESULT_INVALID_STATE;u=&o->v3_update;b=&o->v3_backings[u->candidate_index];
 if(u->act.state!=PAPACC_V3_ACT_EMPTY||!v3b_range(selected,bytes)||
 issuer06_overlap(o,sizeof(*o),selected,bytes)||bytes!=b->packet_bytes||
 bytes<PC_ROOT8_V3_MIN||bytes>PC_ROOT8_V4_MAX||memcmp(selected,b->packet,bytes))return PAPACC_RESULT_INVALID_ARGUMENT;
 if(u->reserved>PAPACC_ED11_PROCESS_LIMIT||bytes+4096U>PAPACC_ED11_PROCESS_LIMIT-u->reserved)return PAPACC_RESULT_LIMIT_EXCEEDED;
 memset(&observed,0,sizeof(observed));
 if(!v3act_native(&u->act,&observed))return PAPACC_RESULT_INVALID_STATE;
 if(!v3act_ready(o)||!papacc_configuration_storage_hash(o,b->packet,bytes,observed.candidate_hash)||
 !papacc_configuration_storage_hash(o,o->committed,o->committed_bytes,observed.predecessor_hash))return PAPACC_RESULT_INVALID_STATE;
 observed.state=PAPACC_V3_ACT_CAPTURED;observed.status=PAPACC_V3_STATUS_SOURCE_ABSENT;u->act=observed;
 return papacc_configuration_v3_update_act_revalidate(o);
}
PAPACC_RESULT papacc_configuration_v3_update_act_revalidate(PAPACC_CONFIGURATION_COMMIT *o)
{
 PAPACC_V3_UPDATE_ACT observed,*a;
 if(!o||!owner(o)||o->v3_update.data_active)return PAPACC_RESULT_INVALID_STATE;a=&o->v3_update.act;
 if(papacc_configuration_v3_update_act_match_data(o)!=PAPACC_RESULT_OK){if(a->state==PAPACC_V3_ACT_CAPTURED)a->state=PAPACC_V3_ACT_INVALID;return PAPACC_RESULT_INVALID_STATE;}
 memset(&observed,0,sizeof(observed));
 if(!a->sid_bytes||a->sid_bytes>sizeof(a->sid)||!IsValidSid(a->sid)||GetLengthSid(a->sid)!=a->sid_bytes||
 !v3act_native(a,&observed)||a->sid_bytes!=observed.sid_bytes||!EqualSid(a->sid,observed.sid)||
 memcmp(&a->statistics.TokenId,&observed.statistics.TokenId,sizeof(LUID))||
 memcmp(&a->statistics.AuthenticationId,&observed.statistics.AuthenticationId,sizeof(LUID))||
 memcmp(&a->statistics.ModifiedId,&observed.statistics.ModifiedId,sizeof(LUID))||
 papacc_configuration_v3_update_act_match_data(o)!=PAPACC_RESULT_OK){if(!a->retained_token&&!a->retained_process_token)a->state=PAPACC_V3_ACT_INVALID;return PAPACC_RESULT_INVALID_STATE;}
 return PAPACC_RESULT_OK; /* Native ACT only, never qualified UTC/status. */
}
