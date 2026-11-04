#include "server_ca_key_win32.h"
#include <stdio.h>
#include <string.h>
int main(int argc,char **argv) {
 PAPACC_CA_KEY o,t;BYTE a[16]={0},g[16]={0};unsigned passed=0;int from,to,wrong;
 (void)argv;if(argc!=1){puts("native mutation fixture removed; use separately authorized read-only diagnostic");return 2;}
 memset(&o,0,sizeof(o));
 if(papacc_ca_key_create(NULL,a,g,NULL,NULL)==NTE_INVALID_PARAMETER)passed++;
 if(papacc_ca_key_create(&o,a,g,NULL,NULL)==NTE_INVALID_PARAMETER)passed++;
 if(papacc_ca_key_open(&o,a,g,NULL,NULL)==NTE_INVALID_PARAMETER)passed++;
 if(papacc_ca_key_validate(NULL)==NTE_BAD_KEY_STATE)passed++;
 if(papacc_ca_key_close(NULL)==NTE_BAD_KEY_STATE)passed++;
 if(o.state==PAPACC_CA_KEY_EMPTY&&!o.provider&&!o.key)passed++;
 memset(&t,0,sizeof(t));t.thread=GetCurrentThreadId();t.state=PAPACC_CA_KEY_READY;t.key=1;t.provider=2;t.borrower=&from;
 if(papacc_ca_key_transfer_borrow(&t,&wrong,&to)!=0&&t.borrower==&from)passed++;
 if(papacc_ca_key_transfer_borrow(&t,&from,&from)!=0&&t.borrower==&from)passed++;
 t.thread++;if(papacc_ca_key_transfer_borrow(&t,&from,&to)!=0&&t.borrower==&from)passed++;t.thread--;
 t.state=PAPACC_CA_KEY_RETAINED;if(papacc_ca_key_transfer_borrow(&t,&from,&to)!=0&&t.borrower==&from)passed++;t.state=PAPACC_CA_KEY_READY;
 if(papacc_ca_key_transfer_borrow(&t,&from,&to)==0&&t.borrower==&to)passed++;
 if(papacc_ca_key_close(&t)!=0&&t.borrower==&to&&!t.close_attempted)passed++; /* no native close while borrowed */
 if(papacc_ca_key_return(&t,&from)!=0&&t.borrower==&to)passed++;
 if(papacc_ca_key_return(&t,&to)==0&&!t.borrower)passed++;
 if(papacc_ca_key_transfer_borrow(&t,&from,&to)!=0&&!t.borrower)passed++;
 printf("argument_and_lifetime_checks=%u/15; SYNTHETIC_HANDLES_NOT_DEREFERENCED; native_key_creation=NOT_RUN; owner_bytes=%zu\n",passed,sizeof(o));
 return passed==15?0:1;
}
