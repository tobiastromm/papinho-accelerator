/* Reads only inert documentary candidate; uses canonical V2 parser. */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "../../../../apps/server/server_service_admission_bd_win32.h"
#include "../../../../tools/qualification/scm-n0-context-model.h"
#define T(x) do{++n;if(!(x)){printf("FAIL case=%u line=%d\n",n,__LINE__);return 1;}}while(0)
int main(int argc,char **argv){FILE *f=NULL;char text[512],changed[512],*where;size_t bytes;unsigned n=0,i;IG_N0_FILE_RECORD record;
 T(argc==2);T(fopen_s(&f,argv[1],"rb")==0&&f!=NULL);bytes=fread(text,1,sizeof(text)-1,f);T(!ferror(f));T(fclose(f)==0);text[bytes]=0;
 T(bytes==397);T(ig_n0_parse(text,bytes,&record));T(record.volume==417614100&&record.high==327680&&record.low==1591236&&record.size==867840&&record.links==2);
 T(!strcmp(record.evidence,"bb548b4f2a866d33445d39e2bb72e0093b60b04380cc4da6fd70c4855a0038fa"));T(!strcmp(record.security,"7852b616c03c57528c4f85f843fd2f671d49dbd66a965f227c2ffbf663a177e0"));T(!strcmp(record.hash,"b02ee54fb2ec69673386d41119ee8ed083a6eab3bfca6aa2155d20ce68ef8963"));
 for(i=0;i<32;i++){memcpy(changed,text,bytes+1);where=strstr(changed,"attempt=")+8;where[i]=where[i]=='f'?'e':'f';T(!ig_n0_parse(changed,bytes,&record));}
 memcpy(changed,text,bytes+1);where=strstr(changed,"operation=")+10;*where='X';T(!ig_n0_parse(changed,bytes,&record));
 T(!ig_n0_parse(text,bytes-1,&record));memcpy(changed,text,bytes+1);changed[bytes-1]=0;T(!ig_n0_parse(changed,bytes,&record));
 T(!ig_n0_inputs_ready());T(!IG_N0_N7_ED11_SHA[0]);T(!PACC_LAB_BINDING_READY);T(ig_n0_fresh(61000,1000,60000));T(!ig_n0_fresh(61001,1000,60000));T(!ig_n0_fresh(2001,1000,1000));
 printf("Q4_INERT_INPUT_PARSER_PASS cases=%u; NO_PROTECTED_OPEN_NO_ADMISSION\n",n);return 0;}
