/* R0-R9 accepted, prospective fixed LAB only. No new handles/authority. */
#if !defined(PAPACC_SCM_REGISTRATION_LAB) || !defined(PAPACC_SCM_RUNTIME_LAB) || !defined(PAPACC_PSR_TRACE_LAB)
#error R0-R9 requires fixed LAB runtime and original status recorder
#endif
#define PSR_R0R9_DECISION_SHA "5bd7e99d48c46490e6c7a3d95765c01093e51cb85b277d7cb83eeac40603e331"
static int psr_r4_stats_same(const TOKEN_STATISTICS *a,const TOKEN_STATISTICS *b)
{return a->TokenType==b->TokenType&&!memcmp(&a->TokenId,&b->TokenId,sizeof(LUID))&&!memcmp(&a->AuthenticationId,&b->AuthenticationId,sizeof(LUID))&&!memcmp(&a->ModifiedId,&b->ModifiedId,sizeof(LUID));}
static int psr_r4_allowed(void){return !native.runtime.retained&&!native.retained&&!host.stop&&!papacc_server_console_win32_stop_requested();}
static BOOL psr_r4_query(HANDLE h,TOKEN_INFORMATION_CLASS c,void *b,DWORD cap,DWORD *used)
{if(!psr_r4_allowed()){SetLastError(native.runtime.r4_error?native.runtime.r4_error:ERROR_OPERATION_ABORTED);return FALSE;}return GetTokenInformation(h,c,b,cap,used);}
#define PSR_QUERY psr_r4_query
static DWORD psr_r4_current(void)
{
 static const struct {BYTE revision,count,authority[6];DWORD sub[6];} expected={1,6,{0,0,0,0,0,5},{80,3567404667UL,26456103UL,3018599297UL,139793421UL,3962028451UL}};
 PT_META m;TOKEN_STATISTICS st;DWORD n;BOOL member=FALSE;
 union {TOKEN_USER align;BYTE bytes[128];} user;
 if(!psr_r4_allowed())return PSRT_RETURN(ERROR_OPERATION_ABORTED,"r4_stop_or_retained");
 if(native.runtime.r4_busy){native.runtime.retained=1U;if(!native.runtime.r4_error)native.runtime.r4_error=ERROR_INVALID_STATE;return PSRT_RETURN(native.runtime.r4_error,"r4_reentrant_borrow");}native.runtime.r4_busy=1U;
 if(!PSRT_PRED("R0_R9_POLICY_" PSR_R0R9_DECISION_SHA,native.readback&&native.sid_size==sizeof(expected)&&EqualSid(native.service_sid,(PSID)&expected))||
 !PSRT_PRED("r4.original_owned_tokens",native.runtime.token&&native.runtime.access_token&&!native.runtime.unexpected&&!native.runtime.check_token))return PSRT_RETURN(ERROR_ACCESS_DENIED,"r4_profile_or_ownership");
 m=pt_token(native.runtime.token,&native.runtime.statistics,native.runtime.user.bytes,native.runtime.user_bytes,native.runtime.groups.bytes,native.runtime.group_bytes,native.service_sid);
 if(m.error){SetLastError(m.error);PSRT_VALUES(m.error,m.known);(void)PSRT_BOOL("GetTokenInformation:R4BoundedObservation",TOKEN_QUERY,FALSE);return PSRT_RETURN(m.error,"r4_original_token_query");}
 if(!PSRT_PRED("r4.complete_original_token",m.complete&&m.known==15U)||!PSRT_PRED("r4.unfiltered_unrestricted",!m.filtered_ever&&!m.restricting_count)||
 !PSRT_PRED("r4.specific_group_shape",m.matches==0U||(m.matches==1U&&(m.attributes&SE_GROUP_ENABLED)&&!(m.attributes&SE_GROUP_USE_FOR_DENY_ONLY))))return PSRT_RETURN(ERROR_ACCESS_DENIED,"r4_token_evidence");
 if(!PSRT_BOOL("GetTokenInformation:R4DuplicateStatistics",TOKEN_QUERY,PSR_QUERY(native.runtime.access_token,TokenStatistics,&st,sizeof(st),&n)))return PSRT_RETURN(ps_last_error(),"r4_duplicate_query");
 if(!PSRT_PRED("r4.duplicate_identity_before",n==sizeof(st)&&st.TokenType==TokenImpersonation&&psr_r4_stats_same(&st,&native.runtime.access_statistics)))return PSRT_RETURN(ERROR_ACCESS_DENIED,"r4_duplicate_identity");
 if(!PSRT_BOOL("GetTokenInformation:R4DuplicateUser",TOKEN_QUERY,PSR_QUERY(native.runtime.access_token,TokenUser,&user,sizeof(user),&n)))return PSRT_RETURN(ps_last_error(),"r4_duplicate_query");
 if(!PSRT_PRED("r4.duplicate_user",n<=sizeof(user)&&n>=sizeof(TOKEN_USER)&&psr_inside_sid(user.bytes,n,user.align.User.Sid)&&EqualSid(user.align.User.Sid,native.service_sid)))return PSRT_RETURN(ERROR_ACCESS_DENIED,"r4_duplicate_identity");
 if(!PSRT_BOOL("CheckTokenMembership:R4SpecificSid",TOKEN_QUERY,CheckTokenMembership(native.runtime.access_token,native.service_sid,&member)))return PSRT_RETURN(ps_last_error(),"r4_membership_query");
 if(!PSRT_PRED("r4.effective_specific_membership",member))return PSRT_RETURN(ERROR_ACCESS_DENIED,"r4_membership_denied");
 if(!PSRT_BOOL("GetTokenInformation:R4DuplicateStatisticsAfter",TOKEN_QUERY,PSR_QUERY(native.runtime.access_token,TokenStatistics,&st,sizeof(st),&n)))return PSRT_RETURN(ps_last_error(),"r4_duplicate_query");
 if(!PSRT_PRED("r4.duplicate_identity_after",n==sizeof(st)&&psr_r4_stats_same(&st,&native.runtime.access_statistics)))return PSRT_RETURN(ERROR_ACCESS_DENIED,"r4_duplicate_identity");
 if(host.stop||native.runtime.retained||papacc_server_console_win32_stop_requested())return PSRT_RETURN(ERROR_OPERATION_ABORTED,"r4_stop_before_current_primary");
 if(PSRT_BOOL("OpenThreadToken:R4Current",TOKEN_QUERY,OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&native.runtime.unexpected)))return PSRT_RETURN(ERROR_ACCESS_DENIED,"r4_unexpected_impersonation");
 if(GetLastError()!=ERROR_NO_TOKEN)return PSRT_RETURN(ps_last_error(),"r4_thread_token_query");
 if(!psr_r4_allowed())return PSRT_RETURN(ERROR_OPERATION_ABORTED,"r4_stop_before_current_acquisition");
 if(!PSRT_BOOL("OpenProcessToken:R4Current",TOKEN_QUERY,OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&native.runtime.check_token)))return PSRT_RETURN(ps_last_error(),"r4_current_primary_query");
 if(!PSRT_BOOL("GetTokenInformation:R4CurrentStatistics",TOKEN_QUERY,PSR_QUERY(native.runtime.check_token,TokenStatistics,&st,sizeof(st),&n)))return PSRT_RETURN(ps_last_error(),"r4_current_primary_query");
 if(!PSRT_PRED("r4.current_primary_continuity",n==sizeof(st)&&st.TokenType==TokenPrimary&&psr_r4_stats_same(&st,&native.runtime.statistics)))return PSRT_RETURN(ERROR_ACCESS_DENIED,"r4_current_primary_identity");
 if(!CloseHandle(native.runtime.check_token)){DWORD e=ps_last_error();native.runtime.retained=1U;if(!native.runtime.r4_error)native.runtime.r4_error=e;return PSRT_RETURN(native.runtime.r4_error,"r4_current_close_unknown");}native.runtime.check_token=NULL;
 native.runtime.r4_busy=0U;return ERROR_SUCCESS;
}
