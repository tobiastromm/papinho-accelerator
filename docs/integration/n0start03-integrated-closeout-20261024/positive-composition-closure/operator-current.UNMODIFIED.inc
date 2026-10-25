static DWORD lab_operator_current(void){
 HANDLE token=NULL;A1_LEASE token_lease,thread_lease;TOKEN_STATISTICS s;DWORD n=0,e=0;
 e=lab_aux_token(&thread_lease,GetCurrentThread(),1,TOKEN_QUERY);if(!e)return lab_aux_close_preserving(thread_lease,ERROR_ACCESS_DENIED);
 if(e!=ERROR_NO_TOKEN)return e;
 if((e=lab_aux_token(&token_lease,GetCurrentProcess(),0,TOKEN_QUERY)))return e;
 if((e=lab_aux_borrow(token_lease,&token)))return lab_aux_close_preserving(token_lease,e);
 if(!GetTokenInformation(token,TokenStatistics,&s,sizeof(s),&n)||memcmp(&s.TokenId,&lab_stats.TokenId,sizeof(LUID))||
 memcmp(&s.AuthenticationId,&lab_stats.AuthenticationId,sizeof(LUID))||memcmp(&s.ModifiedId,&lab_stats.ModifiedId,sizeof(LUID)))e=ERROR_ACCESS_DENIED;
 return lab_aux_close_preserving(token_lease,lab_aux_borrow_end(e));
}
