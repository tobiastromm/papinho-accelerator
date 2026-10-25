#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <bcrypt.h>
#include "../../../../apps/server/server_service_admission_bd_win32.h"
#include "../../../../tools/qualification/scm-n0-context-model.h"
static unsigned ig_n0_positive=1;static const char *ig_n0_approval_sha="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
static struct {BY_HANDLE_FILE_INFORMATION dossier_id,evidence_id;} ig_n0;
static size_t rendered;
static char captured[2304];
static int pacc_storage_hash_callback(void *c,const BYTE *b,size_t n,BYTE out[32]){(void)c;rendered=n;if(n>=sizeof(captured))return 0;memcpy(captured,b,n);captured[n]=0;memset(out,1,32);return 1;}
static void ig_n0_hex(const BYTE b[32],char out[65]){unsigned i;for(i=0;i<32;i++){out[2*i]="0123456789abcdef"[b[i]>>4];out[2*i+1]="0123456789abcdef"[b[i]&15];}out[64]=0;}
#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC
#define IG_N0_DISPOSITION_PLAN "Q0=97763395f521d0a8d15ee0afe22b61705897d6d97c2c1f8905ad9114e8b1b9ca;Q4=590aa1c695adce55d6ba17e9ce508d747090c05c2d61cd44a7ced603cf6c4db4;Q2=5794e17265af6e34d335591b0fed42e5d9048aec81e751578fd9f4132cf524e1;"
#else
#define IG_N0_DISPOSITION_PLAN "L1=2d91d64eace4613f64b2d17188642e85eb5ac41af5f6a5afa1f8e746eaeff27e;H=23d509674be807f01d2c6301cd990f2212a05d6d4d707e56f9f924c6afbd7218;P3=892d51b6a0033b3211c6483adb56d03d728c5f8285dfc8520cf73574c3ee5fad;"
#endif
static DWORD ig_n0_plan(const PS_BD_DESCRIPTOR *d,char out[65])
{
 BYTE digest[32];char text[2304];int n;
 n=sprintf_s(text,sizeof(text),
 "%s;%s;%s;%s;%s;%s;%llu;%lu;%llu;%lu;%llu;%lu;%lu;%lu:%lu:%lu:%lu:%lu:%lu;%llu;"
 "config=%lu:%lu:%lu:%lu:%lu;image=%lu:%lu:%lu:%lu:%lu;aux=%lu:%llu:%lu:%lu;file=%lu:%lu:%lu:%lu:%lu:%lu;%s;%s;%s;"
 "APPROVAL=%s;INPUT_IDS=%lu:%lu:%lu:%lu,%lu:%lu:%lu:%lu;INPUT_PROFILE=N2-SD-RAW1-OWNER-GROUP-DACL;" IG_N0_DISPOSITION_PLAN "SERVICE=PapinhoAcceleratorLab20261023;SID=S-1-5-80-3567404667-26456103-3018599297-139793421-3962028451;OPERATOR=S-1-5-21-785699550-1440458426-1682014700-1108;ASSOCIATION=UNPROVEN;CREDITS=NONE_GRANTED;N7_ED11=%s",
 ig_n0_positive?"" PACC_LAB_OPERATION ";CREATE_IMAGE;CREATE_SINK;REBIND;START1;STOP_MAX1;T10_T14=1f293812e9fca107c7fa8ba20abe20fd8ffbca3cdf6e95e58465a95dadbe4dd5":"N0_READ_ONLY",d->attempt,d->config_hash,d->image_hash,d->policy,d->host,d->boot,(unsigned long)d->owner_pid,d->owner_creation,
 (unsigned long)d->actor.pid,d->actor.creation,(unsigned long)d->actor.session,(unsigned long)d->control_tid,
 (unsigned long)d->statistics.TokenId.LowPart,(unsigned long)d->statistics.TokenId.HighPart,
 (unsigned long)d->statistics.AuthenticationId.LowPart,(unsigned long)d->statistics.AuthenticationId.HighPart,
 (unsigned long)d->statistics.ModifiedId.LowPart,(unsigned long)d->statistics.ModifiedId.HighPart,d->challenge,
 (unsigned long)d->config_id.dwVolumeSerialNumber,(unsigned long)d->config_id.nFileIndexHigh,(unsigned long)d->config_id.nFileIndexLow,(unsigned long)d->config_id.nFileSizeHigh,(unsigned long)d->config_id.nFileSizeLow,
 (unsigned long)d->image_id.dwVolumeSerialNumber,(unsigned long)d->image_id.nFileIndexHigh,(unsigned long)d->image_id.nFileIndexLow,(unsigned long)d->image_id.nFileSizeHigh,(unsigned long)d->image_id.nFileSizeLow,
 (unsigned long)d->auxiliary.instance.pid,d->auxiliary.instance.creation,(unsigned long)d->auxiliary.instance.session,(unsigned long)d->auxiliary.parent,
 (unsigned long)d->auxiliary.file_id.dwVolumeSerialNumber,(unsigned long)d->auxiliary.file_id.nFileIndexHigh,(unsigned long)d->auxiliary.file_id.nFileIndexLow,(unsigned long)d->auxiliary.file_id.nFileSizeHigh,(unsigned long)d->auxiliary.file_id.nFileSizeLow,(unsigned long)d->auxiliary.file_id.nNumberOfLinks,
 d->auxiliary.file_hash,d->auxiliary.security_hash,d->auxiliary.provenance_hash,ig_n0_approval_sha?ig_n0_approval_sha:"NONE",
 (unsigned long)ig_n0.dossier_id.dwVolumeSerialNumber,(unsigned long)ig_n0.dossier_id.nFileIndexHigh,(unsigned long)ig_n0.dossier_id.nFileIndexLow,(unsigned long)ig_n0.dossier_id.nFileSizeLow,
 (unsigned long)ig_n0.evidence_id.dwVolumeSerialNumber,(unsigned long)ig_n0.evidence_id.nFileIndexHigh,(unsigned long)ig_n0.evidence_id.nFileIndexLow,(unsigned long)ig_n0.evidence_id.nFileSizeLow,IG_N0_N7_ED11_SHA);
 if(n<=0||!pacc_storage_hash_callback(NULL,(const BYTE *)text,(size_t)n,digest))return ERROR_INVALID_DATA;ig_n0_hex(digest,out);return 0;
}
int main(void){PS_BD_DESCRIPTOR d;char out[65];ZeroMemory(&d,sizeof(d));
 memset(d.attempt,'a',32);memset(d.config_hash,'b',64);memset(d.image_hash,'c',64);memset(d.policy,'d',64);memset(d.host,'e',64);
 memset(d.auxiliary.file_hash,'f',64);memset(d.auxiliary.security_hash,'a',64);memset(d.auxiliary.provenance_hash,'b',64);
 d.boot=d.owner_creation=d.actor.creation=d.challenge=d.auxiliary.instance.creation=~(ULONGLONG)0;
 d.owner_pid=d.actor.pid=d.actor.session=d.control_tid=d.auxiliary.parent=d.auxiliary.instance.pid=d.auxiliary.instance.session=MAXDWORD;
 memset(&d.statistics.TokenId,0xff,sizeof(LUID));memset(&d.statistics.AuthenticationId,0xff,sizeof(LUID));memset(&d.statistics.ModifiedId,0xff,sizeof(LUID));
 memset(&d.config_id,0xff,sizeof(d.config_id));memset(&d.image_id,0xff,sizeof(d.image_id));memset(&d.auxiliary.file_id,0xff,sizeof(d.auxiliary.file_id));memset(&ig_n0,0xff,sizeof(ig_n0));
 if(ig_n0_plan(&d,out)||!strstr(captured,"Q0=97763395")||!strstr(captured,"Q4=590aa1c6")||!strstr(captured,"Q2=5794e172")||!strstr(captured,"N7_ED11=1d6bca53")||strstr(captured,"L1=")||strstr(captured,"P3="))return 1;
 printf("ACTUAL_PLAN_MAX_WIDTH_BYTES=%zu CAPACITY=2304; Q4_Q0_Q2_N7_PRESENT; NO_L1_P3_INHERITANCE\n",rendered);
 return 0;}
