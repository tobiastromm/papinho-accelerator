#include "host_platform.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static TP_PLATFORM platform;
int main(int argc,char **argv){FILE *f=NULL;size_t n;int ok;
 if(argc!=3||strlen(argv[2])!=64)return 2;
 hp_init(&platform);strcpy_s(platform.exe_hash,sizeof(platform.exe_hash),argv[2]);
 if(fopen_s(&f,argv[1],"rb")||!f)return 2;n=fread(platform.request.bytes,1,TJ_BYTES-1,f);
 if(ferror(f)||!feof(f)){fclose(f);return 2;}if(fclose(f))return 2;
 ok=tj_parse(&platform.request,n)&&hp_request_contract(&platform);
 printf("REQUEST_DATA_ONLY %s nodes=%u bytes=%Iu first=%s NO_APPROVAL_NO_TPM\n",ok?"PASS":"DENY",platform.request.count,n,platform.proof.first);
 return ok?0:1;
}
