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
 if(p1&&(v->max_handles!=PAPACC_SCQ_P1_CAP||v->commit_floor!=PAPACC_SCQ_P1_COMMIT_FLOOR))return ERROR_INVALID_DATA;
 memcpy(out,"G3P1",4);
 for(i=0;i<32U;i++){b=0;for(j=0;j<2U;j++){char ch=norm[2U*i+j];b=(BYTE)((b<<4)|(ch<='9'?ch-'0':ch-'a'+10));}out[4U+i]=b;}
 values[0]=v->private_growth;values[1]=v->working_growth;values[2]=v->physical_floor;values[3]=v->commit_floor;
 values[4]=v->max_handles;values[5]=v->freshness_ms;values[6]=v->duration_ms;values[7]=authorization_ms;
 for(i=0;i<8U;i++){width=i<4U?8U:4U;for(j=0;j<width;j++)out[offset++]=(BYTE)(values[i]>>(8U*j));}
 return 0;
}
static DWORD scqg_plan_data(const PAPACC_SCQ_START_LIMITS *v,DWORD authorization_ms,BYTE out[PAPACC_SCQ_G3_PREIMAGE_BYTES])
{return scqg_plan_data_mode(v,authorization_ms,out,0);}
#endif
