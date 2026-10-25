#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <ncrypt.h>
#include <stdio.h>
static int stop_now;
int papacc_lab_stopped(void){return stop_now;}
#include "../tools/qualification/ca-ksp-next-lab/backend-lab.c"
int main(void){NCRYPT_KEY_HANDLE k=0;DWORD v=2048;unsigned before;int rc=papacc_lab_negative_tests();if(rc)return rc;
 stop_now=1;before=calls;
 if(lab_create(0,&k,NCRYPT_RSA_ALGORITHM,lab_name,0,NCRYPT_MACHINE_KEY_FLAG)!=NTE_USER_CANCELLED||calls!=before)return 8;
 if(lab_set(0,NCRYPT_LENGTH_PROPERTY,(PBYTE)&v,sizeof(v),0)!=NTE_USER_CANCELLED||calls!=before)return 9;
 if(lab_finalize(0,NCRYPT_SILENT_FLAG)!=NTE_USER_CANCELLED||calls!=before)return 10;
 puts("LAB_NEGATIVE_MODEL_PASS; NATIVE_KEY_OPERATIONS=NOT_RUN");return 0;}
