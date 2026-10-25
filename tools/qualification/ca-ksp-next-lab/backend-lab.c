#define _WIN32_WINNT 0x0600
/* LAB-only instrumentation of the unchanged canonical owner translation unit.
 * No new resource owner or native algorithm. No key deletion API. */
#include "server_ca_key_win32.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>
extern int papacc_lab_stopped(void);
#ifdef PAPACC_LAB_NEGATIVE
static SECURITY_STATUS injected;
static unsigned calls;
#define CREATE_CALL(p,k,a,n,l,f) (++calls,injected)
#define SET_CALL(k,n,v,c,f) (++calls,injected)
#define FINAL_CALL(k,f) (++calls,injected)
#else
#define CREATE_CALL(p,k,a,n,l,f) NCryptCreatePersistedKey(p,k,a,n,l,f)
#define SET_CALL(k,n,v,c,f) NCryptSetProperty(k,n,v,c,f)
#define FINAL_CALL(k,f) NCryptFinalizeKey(k,f)
#endif
static const wchar_t lab_name[]=L"PapinhoAccelerator.CA.2345e07adf714045a20d1c8b6fdf5aaf.a2bcd6d2ed3043d083e7e50342d6fab0";
static SECURITY_STATUS lab_phase(const char *phase,SECURITY_STATUS s)
{DWORD saved=GetLastError();printf("LAB_NATIVE phase=%s status=%08lx\n",phase,(unsigned long)s);fflush(stdout);SetLastError(saved);return s;}
static SECURITY_STATUS lab_create(NCRYPT_PROV_HANDLE p,NCRYPT_KEY_HANDLE *k,LPCWSTR a,LPCWSTR n,DWORD l,DWORD f)
{SECURITY_STATUS s;
 if(!k||!a||!n||wcscmp(a,NCRYPT_RSA_ALGORITHM)||wcscmp(n,lab_name)||l||f!=NCRYPT_MACHINE_KEY_FLAG)return lab_phase("CREATE_GUARD",NTE_INVALID_PARAMETER);
 if(papacc_lab_stopped())return lab_phase("CREATE_STOP",NTE_USER_CANCELLED);

#ifdef PAPACC_LAB_NEGATIVE
 (void)p;
#endif
 s=CREATE_CALL(p,k,a,n,l,f);return lab_phase("CREATE",s);}
static SECURITY_STATUS lab_set(NCRYPT_KEY_HANDLE k,LPCWSTR n,PBYTE v,DWORD c,DWORD f)
{const char *phase;SECURITY_STATUS s;
 if(papacc_lab_stopped())return lab_phase("PROPERTY_STOP",NTE_USER_CANCELLED);
 if(!wcscmp(n,NCRYPT_LENGTH_PROPERTY))phase="LENGTH";
 else if(!wcscmp(n,NCRYPT_EXPORT_POLICY_PROPERTY))phase="EXPORT_POLICY";
 else if(!wcscmp(n,NCRYPT_KEY_USAGE_PROPERTY))phase="USAGE";
 else if(!wcscmp(n,NCRYPT_SECURITY_DESCR_PROPERTY))phase="DACL";
 else return lab_phase("PROPERTY_GUARD",NTE_INVALID_PARAMETER);

#ifdef PAPACC_LAB_NEGATIVE
 (void)k;(void)v;(void)c;(void)f;
#endif
 s=SET_CALL(k,n,v,c,f);return lab_phase(phase,s);}
static SECURITY_STATUS lab_finalize(NCRYPT_KEY_HANDLE k,DWORD f)
{if(papacc_lab_stopped())return lab_phase("FINALIZE_STOP",NTE_USER_CANCELLED);
 if(f!=NCRYPT_SILENT_FLAG)return lab_phase("FINALIZE_GUARD",NTE_BAD_FLAGS);

#ifdef PAPACC_LAB_NEGATIVE
 (void)k;
#endif
 return lab_phase("FINALIZE",FINAL_CALL(k,f));}
#ifdef PAPACC_LAB_NEGATIVE
/* Model tests invoke the real guards with native calls replaced at compile time.
 * No owner creation routine or KSP API is called. */
int papacc_lab_negative_tests(void)
{NCRYPT_KEY_HANDLE key=0;DWORD v=2048;SECURITY_STATUS s;unsigned before;
 injected=NTE_EXISTS;calls=0;
 s=lab_create(0,&key,NCRYPT_RSA_ALGORITHM,lab_name,0,NCRYPT_MACHINE_KEY_FLAG);
 if(s!=NTE_EXISTS||calls!=1||key)return 1;
 before=calls;s=lab_create(0,&key,NCRYPT_RSA_ALGORITHM,L"DIVERGENT",0,NCRYPT_MACHINE_KEY_FLAG);if(s!=NTE_INVALID_PARAMETER||calls!=before)return 2;
 s=lab_create(0,&key,NCRYPT_RSA_ALGORITHM,lab_name,0,NCRYPT_MACHINE_KEY_FLAG|NCRYPT_OVERWRITE_KEY_FLAG);if(s!=NTE_INVALID_PARAMETER||calls!=before)return 3;
 injected=NTE_PERM;s=lab_create(0,&key,NCRYPT_RSA_ALGORITHM,lab_name,0,NCRYPT_MACHINE_KEY_FLAG);if(s!=NTE_PERM)return 4;
 s=lab_finalize(0,0);if(s!=NTE_BAD_FLAGS)return 5;
 injected=NTE_FAIL;s=lab_finalize(0,NCRYPT_SILENT_FLAG);if(s!=NTE_FAIL)return 6;
 s=lab_set(0,NCRYPT_LENGTH_PROPERTY,(PBYTE)&v,sizeof(v),0);if(s!=NTE_FAIL)return 7;
 return 0;}
#else
#define NCryptCreatePersistedKey lab_create
#define NCryptSetProperty lab_set
#define NCryptFinalizeKey lab_finalize
#include "../../../apps/server/server_ca_key_win32.c"
#endif
