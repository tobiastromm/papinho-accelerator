static int fixture_role_evidence(TP_PLATFORM *p,HANDLE process,unsigned role){TOKEN_STATISTICS stats;DWORD used=0;unsigned char ids[24];TP_OWNER *o=&p->proof;
 if(!fixture_folder[0])return hp_admin_evidence(p,process,role);
 /* Test actors are unsigned native fixtures, not approved PowerShell tokens.
  * Model role AUTHORITY ONLY; original process/console/metrics and helper file are real. */
 if(o->frozen||p->token)return 0;if(!OpenProcessToken(process,TOKEN_QUERY,&p->token))return tp_fail(o,"MODEL_ROLE_TOKEN_OPEN",GetLastError());o->controlled_refs++;
 if(!GetTokenInformation(p->token,TokenStatistics,&stats,sizeof(stats),&used)||used!=sizeof(stats)){tp_fail(o,"MODEL_ROLE_TOKEN_QUERY",GetLastError());goto done;}
 memcpy(ids,&stats.TokenId,8);memcpy(ids+8,&stats.AuthenticationId,8);memcpy(ids+16,&stats.ModifiedId,8);
 if(p->role_token_bound[role]&&!tp_eq(ids,p->role_token_ids[role],24)){tp_fail(o,"MODEL_ROLE_TOKEN_CHANGED",0);goto done;}memcpy(p->role_token_ids[role],ids,24);p->role_token_bound[role]=1;
 if(!tp_sample(o,"MODEL_ROLE_TOKEN_HELD"))goto done;
 done:if(!close_handle(p,&p->token))return 0;if(o->frozen)return 0;
 return role?hp_role_file(p,process,1):hp_role_file(p,NULL,2);
}
