#define _WIN32_WINNT 0x0600
#include <windows.h>
#include <shlobj.h>
#include <knownfolders.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
static unsigned fault,calls,frees,checks,data_calls;static volatile LONG path_stop;
static WCHAR model_program[300],model_data[300];
static HRESULT WINAPI known(REFKNOWNFOLDERID id,DWORD flags,HANDLE token,PWSTR *out)
{
 unsigned i;int first=IsEqualGUID(id,&FOLDERID_ProgramFiles);calls++;if(!first)data_calls++;
 if(flags!=KF_FLAG_DONT_VERIFY||token)return E_INVALIDARG;
 if(first)wcscpy_s(model_program,300,fault==4?L"\\\\host\\share":fault==7?L"E:\\Programs":L"C:\\Programs");
 else wcscpy_s(model_data,300,L"D:\\Data");
 *out=first?model_program:model_data;
 if((fault==1&&first)||(fault==2&&!first))return E_FAIL;
 if(fault==3&&first)*out=NULL;
 if(fault==5&&first){for(i=0;i<280;i++)model_program[i]=L'A';model_program[280]=0;}
 if((fault==6&&first)||(fault==8&&!first))InterlockedExchange(&path_stop,1);
 return S_OK;
}
static void WINAPI release(void *p){if(p==model_program||p==model_data)frees++;}
static DWORD WINAPI model_image(HMODULE module,LPWSTR out,DWORD cap)
{const WCHAR *value=fault==10U?L"C:\\Other\\papacc_server.exe":L"C:\\Program Files\\Papinho\\Accelerator\\papacc_server.exe";
 (void)module;if(fault==9U)return cap;wcscpy_s(out,cap,value);return (DWORD)wcslen(value);}
#define GetModuleFileNameW model_image
#define SHGetKnownFolderPath known
#define CoTaskMemFree release
#include "../apps/server/personal_install_paths_win32.inc"
#define C(x) do{checks++;if(!(x)){printf("PATH_FAIL line=%u fault=%u\n",(unsigned)__LINE__,fault);return 1;}}while(0)
static void reset(PAPACC_PERSONAL_INSTALL_PATHS *p,unsigned f)
{memset(p,0,sizeof(*p));fault=f;calls=frees=data_calls=0;path_stop=0;}
int main(void)
{
 PAPACC_PERSONAL_INSTALL_PATHS p;unsigned i,before;WCHAR copy[MAX_PATH];
 reset(&p,0);C(papacc_personal_paths_new(&p,&path_stop)==0&&p.phase==2);
 C(!wcscmp(p.binary,L"C:\\Programs\\Papinho\\Accelerator"));C(!wcscmp(p.parent,L"D:\\Data\\Papinho"));C(!wcscmp(p.data,L"D:\\Data\\Papinho\\Accelerator"));C(!wcscmp(p.leaf,L"Accelerator"));C(calls==2&&frees==2);
 C(papacc_personal_paths_current(&p,&path_stop)==0&&calls==4&&frees==4);
 before=calls;C(papacc_personal_paths_new(&p,&path_stop)!=0&&calls==before);
 wcscpy_s(copy,MAX_PATH,p.binary);fault=7;C(papacc_personal_paths_current(&p,&path_stop)!=0&&!wcscmp(p.binary,copy));
 for(i=1;i<=8;i++){
  if(i==7)continue;reset(&p,i);C(papacc_personal_paths_new(&p,&path_stop)!=0&&p.phase==4);
  C(frees==(i==2||i==4||i==5||i==8?2U:i==3?0U:1U));
  before=calls;C(papacc_personal_paths_new(&p,&path_stop)!=0&&calls==before);
 }
 reset(&p,0);path_stop=1;C(papacc_personal_paths_new(&p,&path_stop)!=0&&calls==0&&frees==0);
 reset(&p,0);C(papacc_personal_paths_existing(&p,L"E:\\Original\\PapinhoAccelerator",L"E:\\OriginalData",L"Historical") ==0);
 C(!wcscmp(p.binary,L"E:\\Original\\PapinhoAccelerator")&&!wcscmp(p.data,L"E:\\OriginalData\\Historical")&&calls==0&&frees==0);
 C(papacc_personal_paths_current(&p,&path_stop)==0&&calls==0);path_stop=1;C(papacc_personal_paths_current(&p,&path_stop)!=0);
 reset(&p,0);C(papacc_personal_paths_existing(&p,L"%ProgramFiles%\\Papinho",L"D:\\Data",L"x")!=0&&calls==0);
 reset(&p,0);C(papacc_personal_paths_existing(&p,L"C:\\Original",L"D:\\Data\\..\\Other",L"x")!=0);
 reset(&p,0);C(papacc_personal_paths_existing(&p,L"C:\\Original",L"D:\\Data",L"..")!=0);
 reset(&p,0);wcscpy_s(p.binary,MAX_PATH,L"C:\\Original");C(papacc_personal_paths_existing(&p,p.binary,L"D:\\Data",L"x")!=0&&!p.phase);
 reset(&p,2);C(papacc_personal_paths_binary(&p,&path_stop)==0&&p.mode==PAPACC_PERSONAL_PATHS_BINARY_ONLY&&calls==1U&&frees==1U&&!data_calls);
 C(!wcscmp(p.binary,L"C:\\Programs\\Papinho\\Accelerator")&&!p.parent[0]&&!p.data[0]&&!p.leaf[0]);
 C(papacc_personal_paths_binary_current(&p,&path_stop)==0&&calls==2U&&frees==2U&&!data_calls);
 before=calls;C(papacc_personal_paths_current(&p,&path_stop)!=0&&calls==before); /* cannot authorize full install */
 C(papacc_personal_paths_binary(&p,&path_stop)!=0&&calls==before); /* no recapture */
 fault=7;wcscpy_s(copy,MAX_PATH,p.binary);C(papacc_personal_paths_binary_current(&p,&path_stop)!=0&&!wcscmp(copy,p.binary));
 for(i=0;i<3U;i++){
  reset(&p,0);C(papacc_personal_paths_binary(&p,&path_stop)==0);
  if(i==0U)p.parent[0]=L'X';if(i==1U)p.data[0]=L'X';if(i==2U)p.leaf[0]=L'X';
  C(papacc_personal_paths_binary_current(&p,&path_stop)!=0&&!data_calls);
 }
 for(i=1;i<=6U;i++)if(i!=2U){reset(&p,i);C(papacc_personal_paths_binary(&p,&path_stop)!=0&&!data_calls);}
 reset(&p,0);path_stop=1;C(papacc_personal_paths_binary(&p,&path_stop)!=0&&!calls&&!frees);
 reset(&p,0);C(papacc_personal_paths_new(&p,&path_stop)==0&&data_calls==1U);before=calls;C(papacc_personal_paths_binary_current(&p,&path_stop)!=0&&calls==before);
 {PAPACC_SCQ_EXISTING_SELECTION selected;memset(&selected,0,sizeof(selected));selected.architecture=0x8664;
  memcpy(selected.provenance,pip_existing_provenance,32);for(i=0;i<3U;i++){selected.identity[3U*i]=7;selected.identity[3U*i+2U]=i+1U;}
  reset(&p,0);C(!papacc_personal_paths_selected(&p,&selected,1U,&p,&path_stop)&&!calls&&p.mode==PAPACC_PERSONAL_PATHS_SELECTED_OBJECT);
  C(papacc_personal_paths_current(&p,&path_stop)!=0&&papacc_personal_paths_binary_current(&p,&path_stop)!=0&&!calls);
  C(papacc_personal_paths_selected(&p,&selected,1U,&p,&path_stop)!=0); /* no replay */
  reset(&p,0);C(papacc_personal_paths_selected(&p,NULL,1U,&p,&path_stop)!=0);
  reset(&p,0);C(!papacc_personal_paths_selected(&p,NULL,2U,&p,&path_stop)&&!p.selected.identity[0]&&!calls);
  reset(&p,0);C(papacc_personal_paths_selected(&p,&selected,2U,&p,&path_stop)!=0);
  reset(&p,0);selected.provenance[0]^=1;C(papacc_personal_paths_selected(&p,&selected,1U,&p,&path_stop)!=0);selected.provenance[0]^=1;
  reset(&p,0);C(!papacc_personal_paths_selected_machine_read(&p,&selected,1U,&p,&path_stop,selected.provenance)&&p.read_diagnostic==1U&&!calls);
  C(papacc_personal_paths_current(&p,&path_stop)!=0&&papacc_personal_paths_binary_current(&p,&path_stop)!=0&&!calls);
  C(papacc_personal_paths_selected_machine_read(&p,&selected,1U,&p,&path_stop,selected.provenance)!=0);
  reset(&p,0);C(papacc_personal_paths_selected_machine_read(&p,NULL,1U,&p,&path_stop,selected.provenance)!=0);
  reset(&p,0);C(!papacc_personal_paths_selected_machine_read(&p,NULL,2U,&p,&path_stop,selected.provenance)&&p.read_diagnostic==1U&&!calls);
  reset(&p,0);selected.provenance[0]^=1;C(papacc_personal_paths_selected_machine_read(&p,&selected,1U,&p,&path_stop,selected.provenance)!=0);selected.provenance[0]^=1;
  reset(&p,9);C(papacc_personal_paths_selected(&p,&selected,1U,&p,&path_stop)!=0&&!calls);
  reset(&p,10);C(papacc_personal_paths_selected(&p,&selected,1U,&p,&path_stop)!=0&&!calls);
 }
 printf("PASS INSTALL_PATHS checks=%u MODELED_KNOWN_FOLDER_APIS FREE_CALLS_RECORDED NO_DIRECTORY_OR_OBJECT_PROOF bytes=%zu\n",checks,sizeof(p));return 0;
}


