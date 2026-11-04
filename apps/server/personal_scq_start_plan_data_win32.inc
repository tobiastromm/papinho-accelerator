#ifndef PAPACC_SCQ_START_PLAN_DATA_DEFINED
#define PAPACC_SCQ_START_PLAN_DATA_DEFINED
#include "personal_scq_start_assurance_win32.h"
/* Stack-only hash preimage of the explicit G3 view, not a wire/persisted
 * record or an authority. Do not hash C padding or native pointer values. */
#define PAPACC_SCQ_G3_PREIMAGE_BYTES 84U
static DWORD scqg_plan_data_mode(const PAPACC_SCQ_START_LIMITS *v,DWORD authorization_ms,
 BYTE out[PAPACC_SCQ_G3_PREIMAGE_BYTES],int p1)
{
 static const char norm[]="d65da3287ac5c1da7f59e6a467498ae2198f0896db801584b4b6c5fedad0ad1b";
 ULONGLONG values[8];unsigned i,j,offset=36U,width;BYTE b;
 if(!v||!out||!authorization_ms||!v->private_growth||!v->working_growth||
  !v->physical_floor||!v->commit_floor||!v->max_handles||v->max_handles>PAPACC_SCQ_HANDLE_CAP(p1)||
  !v->freshness_ms||!v->duration_ms)return ERROR_INVALID_DATA;
 if(p1&&(v->max_handles!=PAPACC_SCQ_P1_CAP||v->commit_floor!=PAPACC_SCQ_P1_COMMIT_FLOOR||v->working_growth!=PAPACC_SCQ_P1_WORKING_GROWTH))return ERROR_INVALID_DATA;
 memcpy(out,"G3P1",4);
 for(i=0;i<32U;i++){b=0;for(j=0;j<2U;j++){char ch=norm[2U*i+j];b=(BYTE)((b<<4)|(ch<='9'?ch-'0':ch-'a'+10));}out[4U+i]=b;}
 values[0]=v->private_growth;values[1]=v->working_growth;values[2]=v->physical_floor;values[3]=v->commit_floor;
 values[4]=v->max_handles;values[5]=v->freshness_ms;values[6]=v->duration_ms;values[7]=authorization_ms;
 for(i=0;i<8U;i++){width=i<4U?8U:4U;for(j=0;j<width;j++)out[offset++]=(BYTE)(values[i]>>(8U*j));}
 return 0;
}
static DWORD scqg_plan_data(const PAPACC_SCQ_START_LIMITS *v,DWORD authorization_ms,BYTE out[PAPACC_SCQ_G3_PREIMAGE_BYTES])
{return scqg_plan_data_mode(v,authorization_ms,out,0);}
static DWORD scqk_policy_data(const PAPACC_SCQ_START_LIMITS *limits,BYTE out[PAPACC_SCQ_G3_PREIMAGE_BYTES])
{
 static const BYTE approved[32]={0x84,0x89,0xc0,0x09,0xdf,0xa6,0x2d,0xf0,0x83,0x54,0x15,0xd4,0x54,0x4c,0x06,0x32,0xab,0xd8,0x50,0x92,0x4a,0x35,0x21,0x7b,0x2e,0xc4,0x51,0x71,0x81,0x47,0xad,0x38};
 DWORD e;if(!out)return ERROR_INVALID_PARAMETER;
 if(!limits||limits->private_growth!=1994944U||limits->working_growth!=PAPACC_SCQ_P1_WORKING_GROWTH||limits->physical_floor!=7239172096ULL||limits->commit_floor!=PAPACC_SCQ_P1_COMMIT_FLOOR||limits->max_handles!=PAPACC_SCQ_P1_CAP||limits->freshness_ms!=100U||limits->duration_ms!=60000U)return ERROR_INVALID_DATA;
 e=scqg_plan_data_mode(limits,240000U,out,1);if(e)return e;
 memcpy(out,"G3KR",4);memcpy(out+4,approved,32);return 0; /* distinct policy from TO */
}

#endif
