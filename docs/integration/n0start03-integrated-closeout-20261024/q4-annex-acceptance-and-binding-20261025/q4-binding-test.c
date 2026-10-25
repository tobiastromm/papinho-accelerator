/* Existing canonical parser/transport helpers; synthetic data, no effects. */
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <sddl.h>
#include "../../../../apps/server/server_service_admission_bd_win32.h"
#include "../../../../tools/qualification/scm-n0-context-model.h"
#include "../../../../tools/qualification/scm-n0-private-stdout-profile.h"
#define T(x) do{++n;if(!(x)){printf("FAIL line=%d case=%u\n",__LINE__,n);return 1;}}while(0)
int main(void){unsigned n=0;IG_N0_FILE_RECORD v;char record[512];int size;PS_BD_DESCRIPTOR d;SID_IDENTIFIER_AUTHORITY nt=SECURITY_NT_AUTHORITY;PSID sid=NULL;
 char *args[]={"fixture","--cycle-n0",PACC_LAB_ATTEMPT,"10","100","11","12","101","aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa","--private-stdout","\\\\.\\pipe\\papacc-n0-stdout-bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb","10","100"};
 T(!strcmp(PACC_LAB_ATTEMPT,"10584f02e11f92c92b43cddf0d9d316f"));T(!PACC_LAB_BINDING_READY);T(!IG_N0_DOSSIER_SHA[0]);T(!IG_N0_EVIDENCE_SHA[0]);T(!IG_N0_N7_ED11_SHA[0]);T(!IG_N0_DOSSIER_VOLUME);T(!IG_N0_EVIDENCE_VOLUME);
 T(ig_transport_bootstrap(13,args));args[2]="0b1de869e11493b7cb08fb329578f6ce";T(!ig_transport_bootstrap(13,args));args[2]="75330d11cc7e4123ae62eadc9a59c2f5";T(!ig_transport_bootstrap(13,args));args[2]=PACC_LAB_ATTEMPT;
 size=sprintf_s(record,sizeof(record),"PACC_N2_OPENED_FILE_V2\nattempt=%s\noperation=%s\nvolume=1\nhigh=0\nlow=2\nsize=3\nlinks=1\nsha256=%064d\nsecurity_sha256=%064d\nwindows_evidence_sha256=%064d\n",PACC_LAB_ATTEMPT,PACC_LAB_OPERATION,1,2,3);
 T(size>0);T(ig_n0_parse(record,(size_t)size,&v));record[40]^=1;T(!ig_n0_parse(record,(size_t)size,&v));record[40]^=1;
 T(ig_n0_fresh(60100,100,60000));T(!ig_n0_fresh(60101,100,60000));T(!ig_n0_fresh(99,100,1000));T(!ig_n0_fresh(1101,100,1000));
 ZeroMemory(&d,sizeof(d));d.actor.handle=(HANDLE)1;d.actor.pid=1;d.actor.creation=1;d.control_thread=(HANDLE)2;d.control_tid=2;d.configuration=(HANDLE)3;d.image=(HANDLE)4;d.owner_pid=2;d.owner_creation=2;d.boot=236441;d.challenge=1;
 strcpy_s(d.attempt,sizeof(d.attempt),PACC_LAB_ATTEMPT);memset(d.plan,'a',64);memset(d.config_hash,'a',64);memset(d.image_hash,'a',64);memset(d.policy,'a',64);memset(d.host,'a',64);
 T(AllocateAndInitializeSid(&nt,1,SECURITY_LOCAL_SYSTEM_RID,0,0,0,0,0,0,0,&sid));d.sid_size=GetLengthSid(sid);T(CopySid(sizeof(d.sid),d.sid,sid));FreeSid(sid);T(!ps_bd_descriptor(&d));strcpy_s(d.attempt,sizeof(d.attempt),"0b1de869e11493b7cb08fb329578f6ce");T(ps_bd_descriptor(&d)==ERROR_INVALID_DATA);
 printf("Q4_BINDING_CASES=%u PASS; SYNTHETIC_NO_OPERATION\n",n);return 0;}
