/* Same checks and order; diagnostics never supply or relax a predicate. */
static int personal_deny(PAPACC_CONFIG_PENDING *p,DWORD stage,DWORD error,DWORD kind)
{PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;if(!n->diagnostic_first){n->diagnostic_first=stage;n->diagnostic_error=error;n->diagnostic_kind=kind;}return 0;}
static int personal_group(PAPACC_CONFIG_PENDING *p,WELL_KNOWN_SID_TYPE kind,int expected,DWORD stage)
{
 BYTE sid[SECURITY_MAX_SID_SIZE];DWORD bytes=sizeof(sid);BOOL member=FALSE;
 if(!CreateWellKnownSid(kind,NULL,sid,&bytes))return personal_deny(p,stage,GetLastError(),1);
 if(!CheckTokenMembership(NULL,sid,&member))return personal_deny(p,stage,GetLastError(),1);
 return (member?1:0)==expected?1:personal_deny(p,stage,0,2);
}
static int personal_token(PAPACC_CONFIG_PENDING *p,int capture)
{
 PAPACC_PERSONAL_NATIVE_CONTEXT *n=&p->personal_native;
 union {TOKEN_USER alignment;BYTE bytes[sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE];} u;
 TOKEN_STATISTICS current_token,t;TOKEN_ELEVATION e;TOKEN_ELEVATION_TYPE type;DWORD z,session,ps,error;TOKEN_USER *user=(TOKEN_USER *)&u;int same;BOOL queried;
 if(n->phase!=1U||p->stop||n->unexpected_thread)return personal_deny(p,1,0,2);
 if(OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&n->unexpected_thread))return personal_deny(p,2,0,2);
 error=GetLastError();if(error!=ERROR_NO_TOKEN)return personal_deny(p,2,error,1);
 if(!personal_group(p,WinBuiltinAdministratorsSid,1,3)||!personal_group(p,WinInteractiveSid,1,4)||!personal_group(p,WinServiceSid,0,5))return 0;
 if(capture&&!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&p->bootstrap_token))return personal_deny(p,6,GetLastError(),1);
 if(!p->bootstrap_token)return personal_deny(p,6,0,2);
 if(!GetTokenInformation(p->bootstrap_token,TokenStatistics,&t,sizeof(t),&z))return personal_deny(p,7,GetLastError(),1);
 if(t.TokenType!=TokenPrimary)return personal_deny(p,8,0,2);
 if(!GetTokenInformation(p->bootstrap_token,TokenElevation,&e,sizeof(e),&z))return personal_deny(p,9,GetLastError(),1);
 if(!e.TokenIsElevated)return personal_deny(p,10,0,2);
 if(!GetTokenInformation(p->bootstrap_token,TokenElevationType,&type,sizeof(type),&z))return personal_deny(p,11,GetLastError(),1);
 if(type==TokenElevationTypeLimited)return personal_deny(p,12,0,2);
 if(!GetTokenInformation(p->bootstrap_token,TokenSessionId,&session,sizeof(session),&z))return personal_deny(p,13,GetLastError(),1);
 if(!session)return personal_deny(p,14,0,2);
 if(!ProcessIdToSessionId(GetCurrentProcessId(),&ps))return personal_deny(p,15,GetLastError(),1);
 if(ps!=session||session!=WTSGetActiveConsoleSessionId())return personal_deny(p,16,0,2);
 if(!GetTokenInformation(p->bootstrap_token,TokenUser,&u,sizeof(u),&z))return personal_deny(p,17,GetLastError(),1);
 if(!IsValidSid(user->User.Sid)||GetLengthSid(user->User.Sid)>sizeof(n->sid))return personal_deny(p,18,0,2);
 if(capture){n->token=t;n->session=session;n->sid_bytes=GetLengthSid(user->User.Sid);return CopySid(sizeof(n->sid),n->sid,user->User.Sid)?1:personal_deny(p,19,GetLastError(),1);}
 if(n->check_token)return personal_deny(p,20,0,2);
 if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&n->check_token))return personal_deny(p,20,GetLastError(),1);
 queried=GetTokenInformation(n->check_token,TokenStatistics,&current_token,sizeof(current_token),&z);
 if(!queried)(void)personal_deny(p,22,GetLastError(),1);
 same=queried&&
  !memcmp(&current_token.TokenId,&t.TokenId,sizeof(LUID))&&!memcmp(&current_token.ModifiedId,&t.ModifiedId,sizeof(LUID));
 if(!CloseHandle(n->check_token)){n->phase=2U;return personal_deny(p,21,GetLastError(),1);}n->check_token=NULL;
 if(!same)return personal_deny(p,22,0,2);
 return n->session==session&&n->sid_bytes==GetLengthSid(user->User.Sid)&&EqualSid(n->sid,user->User.Sid)&&
  !memcmp(&n->token.TokenId,&t.TokenId,sizeof(LUID))&&!memcmp(&n->token.AuthenticationId,&t.AuthenticationId,sizeof(LUID))&&
  !memcmp(&n->token.ModifiedId,&t.ModifiedId,sizeof(LUID))?1:personal_deny(p,23,0,2);
}
