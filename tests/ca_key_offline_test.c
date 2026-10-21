#include "server_ca_key_win32.h"
#include <stdio.h>
#include <string.h>
int main(int argc,char **argv) {
 PAPACC_CA_KEY o;BYTE a[16]={0},g[16]={0};unsigned passed=0;
 (void)argv;if(argc!=1){puts("native mutation fixture removed; use separately authorized read-only diagnostic");return 2;}
 memset(&o,0,sizeof(o));
 if(papacc_ca_key_create(NULL,a,g,NULL,NULL)==NTE_INVALID_PARAMETER)passed++;
 if(papacc_ca_key_create(&o,a,g,NULL,NULL)==NTE_INVALID_PARAMETER)passed++;
 if(papacc_ca_key_open(&o,a,g,NULL,NULL)==NTE_INVALID_PARAMETER)passed++;
 if(papacc_ca_key_validate(NULL)==NTE_BAD_KEY_STATE)passed++;
 if(papacc_ca_key_close(NULL)==NTE_BAD_KEY_STATE)passed++;
 if(o.state==PAPACC_CA_KEY_EMPTY&&!o.provider&&!o.key)passed++;
 printf("invalid_argument_checks=%u/6; native_key_creation=NOT_RUN; owner_bytes=%zu\n",passed,sizeof(o));
 return passed==6?0:1;
}