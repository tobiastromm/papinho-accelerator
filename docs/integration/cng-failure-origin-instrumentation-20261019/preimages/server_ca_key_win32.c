#define _WIN32_WINNT 0x0600
#include "server_ca_key_win32.h"
#include <bcrypt.h>
#include <sddl.h>
#include <aclapi.h>
#include <string.h>
#include <stdio.h>
/* SECURITY_STATUS/NTE values are returned, never raw key/secret diagnostics. */
static int ca_nonzero(const BYTE *p) {
 unsigned i;BYTE v=0;for(i=0;i<16;i++)v=(BYTE)(v|p[i]);return v!=0;
}
static SECURITY_STATUS ca_identity(PAPACC_CA_KEY *o,PSID admin,PSID service) {
 BYTE storage[sizeof(TOKEN_USER)+SECURITY_MAX_SID_SIZE];DWORD n=0,e;
 HANDLE token=NULL;PTOKEN_USER user=(PTOKEN_USER)storage;int same;
 if(!IsValidSid(admin)||!IsValidSid(service)||EqualSid(admin,service))return NTE_PERM;
 if(!OpenThreadToken(GetCurrentThread(),TOKEN_QUERY,TRUE,&token)) {
  e=GetLastError();if(e!=ERROR_NO_TOKEN)return NTE_PERM;
  if(!OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&token))return NTE_PERM;
 }
 same=GetTokenInformation(token,TokenUser,storage,sizeof(storage),&n)&&EqualSid(user->User.Sid,admin);
 if(!CloseHandle(token)){o->retained_token=token;o->state=PAPACC_CA_KEY_RETAINED;o->error=NTE_FAIL;return NTE_FAIL;}
 if(!same)return NTE_PERM;
 return ERROR_SUCCESS;
}
static void ca_name(wchar_t *out,const BYTE *a,const BYTE *g) {
 static const wchar_t digits[]=L"0123456789abcdef";
 const wchar_t *prefix=L"PapinhoAccelerator.CA.";unsigned i,n=0;
 while(*prefix)out[n++]=*prefix++;
 for(i=0;i<16;i++){out[n++]=digits[a[i]>>4];out[n++]=digits[a[i]&15];}
 out[n++]=L'.';
 for(i=0;i<16;i++){out[n++]=digits[g[i]>>4];out[n++]=digits[g[i]&15];}out[n]=0;
}
static SECURITY_STATUS ca_property(NCRYPT_KEY_HANDLE key,LPCWSTR name,DWORD expected) {
 DWORD value=0,n=0;SECURITY_STATUS s=NCryptGetProperty(key,name,(PBYTE)&value,sizeof(value),&n,0);
 if(s!=ERROR_SUCCESS)return s;
 return n==sizeof(value)&&value==expected?ERROR_SUCCESS:NTE_BAD_KEY_STATE;
}
static SECURITY_STATUS ca_acl(PAPACC_CA_KEY *o) {
 BYTE descriptor[1024];DWORD n=0,i;PACL acl=NULL;BOOL present=FALSE,def=FALSE;
 DWORD admin_allow=0,service_deny=0;BYTE system[SECURITY_MAX_SID_SIZE];DWORD z=sizeof(system);
 SECURITY_STATUS s=NCryptGetProperty(o->key,NCRYPT_SECURITY_DESCR_PROPERTY,descriptor,sizeof(descriptor),&n,DACL_SECURITY_INFORMATION);
 if(s!=ERROR_SUCCESS)return s;
 if(!n||!IsValidSecurityDescriptor(descriptor)||!GetSecurityDescriptorDacl(descriptor,&present,&acl,&def)||!present||!acl||
    !CreateWellKnownSid(WinLocalSystemSid,NULL,system,&z))return NTE_BAD_KEY_STATE;
 for(i=0;i<acl->AceCount;i++) {
  PACE_HEADER h;ACCESS_ALLOWED_ACE *ace;PSID sid;
  if(!GetAce(acl,i,(LPVOID *)&h))return NTE_BAD_KEY_STATE;
  if(h->AceFlags&INHERIT_ONLY_ACE)return NTE_BAD_KEY_STATE;
  if(h->AceType!=ACCESS_ALLOWED_ACE_TYPE&&h->AceType!=ACCESS_DENIED_ACE_TYPE)return NTE_BAD_KEY_STATE;
  ace=(ACCESS_ALLOWED_ACE *)h;sid=(PSID)&ace->SidStart;
  if(!IsValidSid(sid))return NTE_BAD_KEY_STATE;
  if(h->AceType==ACCESS_ALLOWED_ACE_TYPE) {
   if(!EqualSid(sid,o->operator_sid)&&!EqualSid(sid,system))return NTE_PERM;
   if(EqualSid(sid,o->operator_sid))admin_allow|=ace->Mask;
  } else if(EqualSid(sid,o->service_sid))service_deny|=ace->Mask;
 }
 if(!(admin_allow&GENERIC_ALL)||!(service_deny&GENERIC_ALL))return NTE_PERM;
 return ERROR_SUCCESS;
}
SECURITY_STATUS papacc_ca_key_validate(PAPACC_CA_KEY *o) {
 BYTE blob[512];BCRYPT_RSAKEY_BLOB *b=(BCRYPT_RSAKEY_BLOB *)blob;DWORD n=0;
 SECURITY_STATUS s;
 if(!o||!o->key||o->thread!=GetCurrentThreadId()||o->state==PAPACC_CA_KEY_RETAINED||o->state==PAPACC_CA_KEY_CLOSED)return NTE_BAD_KEY_STATE;
 s=ca_identity(o,o->operator_sid,o->service_sid);if(s!=ERROR_SUCCESS)return s;
 s=ca_property(o->key,NCRYPT_LENGTH_PROPERTY,2048);if(s!=ERROR_SUCCESS)return s;
 s=ca_property(o->key,NCRYPT_EXPORT_POLICY_PROPERTY,0);if(s!=ERROR_SUCCESS)return s;
 s=ca_property(o->key,NCRYPT_KEY_USAGE_PROPERTY,NCRYPT_ALLOW_SIGNING_FLAG);if(s!=ERROR_SUCCESS)return s;
 s=NCryptExportKey(o->key,0,BCRYPT_RSAPUBLIC_BLOB,NULL,blob,sizeof(blob),&n,0);
 if(s!=ERROR_SUCCESS)return s;
 if(n!=sizeof(*b)+3+256||b->Magic!=BCRYPT_RSAPUBLIC_MAGIC||b->BitLength!=2048||b->cbPublicExp!=3||b->cbModulus!=256||
    blob[sizeof(*b)]!=1||blob[sizeof(*b)+1]!=0||blob[sizeof(*b)+2]!=1)return NTE_BAD_KEY_STATE;
 return ca_acl(o);
}
SECURITY_STATUS papacc_ca_key_close(PAPACC_CA_KEY *o) {
 SECURITY_STATUS s;
 if(!o||o->thread!=GetCurrentThreadId())return NTE_BAD_KEY_STATE;
 if(o->state==PAPACC_CA_KEY_RETAINED)return o->error?o->error:NTE_FAIL;
 if(o->state==PAPACC_CA_KEY_CLOSED)return ERROR_SUCCESS;
 o->close_attempted=1;
 if(o->key){s=NCryptFreeObject(o->key);if(s!=ERROR_SUCCESS)goto retained;o->key=0;}
 if(o->provider){s=NCryptFreeObject(o->provider);if(s!=ERROR_SUCCESS)goto retained;o->provider=0;}
 if(o->retained_token){s=NTE_FAIL;goto retained;}
 o->state=PAPACC_CA_KEY_CLOSED;o->close_confirmed=1;return ERROR_SUCCESS;
retained:
 o->state=PAPACC_CA_KEY_RETAINED;o->error=s;return s;
}
static SECURITY_STATUS ca_begin(PAPACC_CA_KEY *o,const BYTE *a,const BYTE *g,PSID admin,PSID service,int create) {
 SECURITY_STATUS s;DWORD n;BYTE sd[1024];LPWSTR as=NULL,ss=NULL;
 PSECURITY_DESCRIPTOR converted=NULL;wchar_t text[512];int written;
 if(!o||!a||!g||!admin||!service||o->state!=PAPACC_CA_KEY_EMPTY||o->key||o->provider||o->retained_token||
    !ca_nonzero(a)||!ca_nonzero(g))return NTE_INVALID_PARAMETER;
 o->thread=GetCurrentThreadId();s=ca_identity(o,admin,service);if(s!=ERROR_SUCCESS)goto failure;
 o->operator_size=GetLengthSid(admin);o->service_size=GetLengthSid(service);
 if(o->operator_size>sizeof(o->operator_sid)||o->service_size>sizeof(o->service_sid)){s=NTE_INVALID_PARAMETER;goto failure;}
 if(!CopySid(sizeof(o->operator_sid),o->operator_sid,admin)||!CopySid(sizeof(o->service_sid),o->service_sid,service)){s=NTE_FAIL;goto failure;}
 ca_name(o->name,a,g);
 s=NCryptOpenStorageProvider(&o->provider,MS_KEY_STORAGE_PROVIDER,0);if(s!=ERROR_SUCCESS)goto failure;
 if(!create){s=NCryptOpenKey(o->provider,&o->key,o->name,0,NCRYPT_MACHINE_KEY_FLAG|NCRYPT_SILENT_FLAG);if(s!=ERROR_SUCCESS)goto failure;}
 else {
  /* No NCRYPT_OVERWRITE_KEY_FLAG. Existing key is never replaced. */
  s=NCryptCreatePersistedKey(o->provider,&o->key,NCRYPT_RSA_ALGORITHM,o->name,0,NCRYPT_MACHINE_KEY_FLAG);
  if(s!=ERROR_SUCCESS)goto failure;o->created=1;
  n=2048;s=NCryptSetProperty(o->key,NCRYPT_LENGTH_PROPERTY,(PBYTE)&n,sizeof(n),0);if(s!=ERROR_SUCCESS)goto failure;
  n=0;s=NCryptSetProperty(o->key,NCRYPT_EXPORT_POLICY_PROPERTY,(PBYTE)&n,sizeof(n),0);if(s!=ERROR_SUCCESS)goto failure;
  n=NCRYPT_ALLOW_SIGNING_FLAG;s=NCryptSetProperty(o->key,NCRYPT_KEY_USAGE_PROPERTY,(PBYTE)&n,sizeof(n),0);if(s!=ERROR_SUCCESS)goto failure;
  if(!ConvertSidToStringSidW(admin,&as)||!ConvertSidToStringSidW(service,&ss)){s=NTE_FAIL;goto failure;}
  written=_snwprintf_s(text,sizeof(text)/sizeof(text[0]),_TRUNCATE,L"D:P(D;;GA;;;%s)(A;;GA;;;%s)(A;;GA;;;SY)",ss,as);
  if(written<0||!ConvertStringSecurityDescriptorToSecurityDescriptorW(text,SDDL_REVISION_1,&converted,&n)||n>sizeof(sd)){s=NTE_FAIL;goto failure;}
  memcpy(sd,converted,n);
  s=NCryptSetProperty(o->key,NCRYPT_SECURITY_DESCR_PROPERTY,sd,n,DACL_SECURITY_INFORMATION|NCRYPT_PERSIST_FLAG);if(s!=ERROR_SUCCESS)goto failure;
  s=NCryptFinalizeKey(o->key,NCRYPT_SILENT_FLAG);if(s!=ERROR_SUCCESS)goto failure;
 }
 s=papacc_ca_key_validate(o);if(s!=ERROR_SUCCESS)goto failure;
 o->state=PAPACC_CA_KEY_READY;
failure:
 if(as&&LocalFree(as)){o->retained_local[0]=(HLOCAL)as;o->state=PAPACC_CA_KEY_RETAINED;s=NTE_FAIL;}
 if(ss&&LocalFree(ss)){o->retained_local[1]=(HLOCAL)ss;o->state=PAPACC_CA_KEY_RETAINED;s=NTE_FAIL;}
 if(converted&&LocalFree(converted)){o->retained_local[2]=(HLOCAL)converted;o->state=PAPACC_CA_KEY_RETAINED;s=NTE_FAIL;}
 if(s!=ERROR_SUCCESS){o->error=s;if(o->retained_token)o->state=PAPACC_CA_KEY_RETAINED;else if(o->state!=PAPACC_CA_KEY_RETAINED)(void)papacc_ca_key_close(o);}
 return s;
}
SECURITY_STATUS papacc_ca_key_create(PAPACC_CA_KEY *o,const BYTE a[16],const BYTE g[16],PSID admin,PSID service) {
 return ca_begin(o,a,g,admin,service,1);
}
SECURITY_STATUS papacc_ca_key_open(PAPACC_CA_KEY *o,const BYTE a[16],const BYTE g[16],PSID admin,PSID service) {
 return ca_begin(o,a,g,admin,service,0);
}