#ifndef PAPACC_SCQ_NATIVE_NAME_WIN32_INC
#define PAPACC_SCQ_NATIVE_NAME_WIN32_INC
/* Private P1 candidate-location resolver shared by cold originals and the
 * image-chain anchor. Callers prove purpose/scope and independently retain
 * and validate original objects. A device-name string is never authority. */
static DWORD scq_p1_native_name(const WCHAR *path,int root,WCHAR output[MAX_PATH+4U])
{
 WCHAR drive[3],device[MAX_PATH+4U];DWORD length,error;size_t i,k,start,n;
 if(!path||!output)return ERROR_INVALID_PARAMETER;n=wcsnlen(path,MAX_PATH);
 if(n<3U||n>=MAX_PATH||!((path[0]>=L'A'&&path[0]<=L'Z')||(path[0]>=L'a'&&path[0]<=L'z'))||
  path[1]!=L':'||path[2]!=L'\\'||(root?n!=3U:n==3U))return ERROR_INVALID_NAME;
 if(!root){start=3U;
  for(i=start;;i++)if(!path[i]||path[i]==L'\\'){
   if(i==start||(i-start==1U&&path[start]==L'.')||(i-start==2U&&path[start]==L'.'&&path[start+1U]==L'.'))return ERROR_INVALID_NAME;
   if(!path[i])break;start=i+1U;
  }else if(path[i]==L':'||path[i]==L'/'||path[i]<32U)return ERROR_INVALID_NAME;
 }
 drive[0]=path[0];drive[1]=L':';drive[2]=0;memset(device,0,sizeof(device));
 length=QueryDosDeviceW(drive,device,MAX_PATH+4U);
 if(!length){error=GetLastError();return error?error:ERROR_GEN_FAILURE;}
 if(length>MAX_PATH+4U)return ERROR_INVALID_DATA;
 k=wcsnlen(device,length);
 if(k==length||k<=8U||wcsncmp(device,L"\\Device\\",8U))return ERROR_INVALID_NAME;
 for(i=8U;i<k;i++)if(device[i]==L'\\'||device[i]==L'/'||device[i]==L':'||device[i]<33U)return ERROR_INVALID_NAME;
 if((k==9U&&device[8]==L'.')||(k==10U&&device[8]==L'.'&&device[9]==L'.'))return ERROR_INVALID_NAME;
 if(k+n-2U>=MAX_PATH+4U)return ERROR_INSUFFICIENT_BUFFER;
 wcscpy_s(output,MAX_PATH+4U,device);wcscat_s(output,MAX_PATH+4U,path+2U);return 0;
}
#endif
