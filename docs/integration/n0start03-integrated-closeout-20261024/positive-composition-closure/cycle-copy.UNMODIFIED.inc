static DWORD cycle_copy(void)
{
 {DWORD aux_guard=lab_aux_gate();if(aux_guard)return aux_guard;}

 DWORD e,n,w;BYTE block[4096];LARGE_INTEGER zero;BY_HANDLE_FILE_INFORMATION before,after;
 cycle_stage="EXCLUSIVE_IMAGE_CREATE";
 e=GetSecurityInfo(native.image,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION|GROUP_SECURITY_INFORMATION|DACL_SECURITY_INFORMATION,NULL,NULL,NULL,NULL,&cycle_sd);if(e)return e;
 if(!GetFileInformationByHandle(cycle_source_file,&before))return lab_error();
 if((e=cycle_open_relative(1)))return e;cycle_effect=1;
 if(LocalFree(cycle_sd)){native.retained=1;return ERROR_INVALID_HANDLE;}cycle_sd=NULL;
 zero.QuadPart=0;if(!SetFilePointerEx(cycle_source_file,zero,NULL,FILE_BEGIN))return lab_error();
 for(;;){if(papacc_server_console_win32_stop_requested())return ERROR_OPERATION_ABORTED;
 if(!ReadFile(cycle_source_file,block,sizeof(block),&n,NULL))return lab_error();if(!n)break;
 if(!WriteFile(cycle_file,block,n,&w,NULL)||w!=n)return lab_error();}
 if(!FlushFileBuffers(cycle_file)||!GetFileInformationByHandle(cycle_file,&cycle_id)||!GetFileInformationByHandle(cycle_source_file,&after))return lab_error();
 if(!ps_image_same(&before,&after))return ERROR_INVALID_DATA;
 if((e=lab_hash(cycle_file,cycle_sha)))return e;
 /* Drop writer, reopen relative with read-only share, compare same native object. */
 if(!CloseHandle(cycle_file)){native.retained=1;return ERROR_INVALID_HANDLE;}cycle_file=NULL;
 if((e=cycle_open_relative(0)))return e;return cycle_files();
}
