#ifndef PAPACC_PERSONAL_SERVICE_SID_DEFINED
#define PAPACC_PERSONAL_SERVICE_SID_DEFINED
static DWORD scqc_runtime_sid(PAPACC_SCQ_PEER_DATA *runtime,WCHAR domain[128])
{
 DWORD capacity=sizeof(runtime->sid),domain_capacity=128U,length,encoded;SID_NAME_USE use;
 if(scqp_nonzero((const BYTE *)runtime,sizeof(*runtime)))return ERROR_INVALID_STATE;
 if(!LookupAccountNameW(NULL,L"NT SERVICE\\PapinhoAccelerator",runtime->sid,&capacity,domain,&domain_capacity,&use)||
  (use!=SidTypeWellKnownGroup&&use!=SidTypeUser))return ERROR_INVALID_SID;
 if(capacity<8U||capacity>sizeof(runtime->sid)||runtime->sid[1]>SID_MAX_SUB_AUTHORITIES)return ERROR_INVALID_SID;
 encoded=8U+4U*runtime->sid[1];
 if(encoded>sizeof(runtime->sid)||encoded>capacity||!IsValidSid((PSID)runtime->sid))return ERROR_INVALID_SID;
 length=GetLengthSid((PSID)runtime->sid);
 if(length!=encoded||length>capacity)return ERROR_INVALID_SID;
 runtime->sid_bytes=length;
 return scqp_unbound(runtime)?0:ERROR_INVALID_SID;
}
#endif
