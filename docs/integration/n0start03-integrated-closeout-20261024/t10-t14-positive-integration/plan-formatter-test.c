#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "../../../../apps/server/server_service_admission_bd_win32.h"
#include "../../../../tools/qualification/scm-n0-context-pins.h"
static unsigned ig_n0_positive,invalid;static char seen[2304];static size_t seen_bytes;
static const char *ig_n0_approval_sha="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
static struct {BY_HANDLE_FILE_INFORMATION dossier_id,evidence_id;} ig_n0;
static int pacc_storage_hash_callback(void *unused,const BYTE *text,size_t n,BYTE out[32]){(void)unused;if(n>=sizeof(seen))return 0;memcpy(seen,text,n);seen[n]=0;seen_bytes=n;memset(out,0,32);return 1;}
#include "plan-formatter.CURRENT.inc"
static void bad_parameter(const wchar_t *a,const wchar_t *b,const wchar_t *c,unsigned line,uintptr_t reserved){(void)a;(void)b;(void)c;(void)line;(void)reserved;invalid=1;}
int main(void){PS_BD_DESCRIPTOR d;char digest[65];unsigned i;_set_invalid_parameter_handler(bad_parameter);
#ifdef _DEBUG
 _CrtSetReportMode(_CRT_ASSERT,0);
#endif
 memset(&d,255,sizeof(d));memset(&ig_n0,255,sizeof(ig_n0));
 memset(d.attempt,'a',32);d.attempt[32]=0;
 {char *v[]={d.config_hash,d.image_hash,d.policy,d.host,d.auxiliary.file_hash,d.auxiliary.security_hash,d.auxiliary.provenance_hash};for(i=0;i<7;i++){memset(v[i],'a',64);v[i][64]=0;}}
 for(i=0;i<2;i++){ig_n0_positive=i;if(ig_n0_plan(&d,digest)||invalid||strlen(digest)!=64||seen_bytes>=sizeof(seen))return 1;
  if((strstr(seen,"T10_T14=1f293812e9fca107c7fa8ba20abe20fd8ffbca3cdf6e95e58465a95dadbe4dd5")!=NULL)!=(i!=0))return 2;
  printf("PLAN_FORMATTER_BOUND mode=%s bytes=%llu capacity=2304 HASH_MOCK_ONLY_NO_AUTHORITY\n",i?"positive":"readonly",(unsigned long long)seen_bytes);
 }return 0;}
