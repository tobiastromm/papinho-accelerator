/* Exact preserved historical predecessor; never adopt current SCM text. */
#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC
static const WCHAR cycle_previous_image[]=L"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\papacc_server_lab_host_ntstatus_0b1de869e11493b7cb08fb329578f6ce.exe";
#endif
static DWORD cycle_readback(DWORD startup,int candidate)
{
 WCHAR saved[1050];DWORD e;const WCHAR *expected=NULL;memcpy(saved,native.command,sizeof(saved));
 if(candidate)expected=cycle_path;
#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC
 else expected=cycle_previous_image;
#endif
 if(expected){native.command[0]=0;if(!lab_append(native.command,1050,L"\"")||!lab_append(native.command,1050,expected)||!lab_append(native.command,1050,L"\" service host")){memcpy(native.command,saved,sizeof(saved));return ERROR_INVALID_DATA;}}
 e=ps_virtual_readback_start(startup);
 memcpy(native.command,saved,sizeof(saved));
 if(!e&&native.query.aligned.dwErrorControl!=SERVICE_ERROR_NORMAL)e=ERROR_INVALID_DATA;
 return e;
}
