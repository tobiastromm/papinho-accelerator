#include <windows.h>
#include <stdio.h>
#include "server_service_win32.h"
#define T(x) do{if(!(x))return __LINE__;}while(0)
int main(void)
{
 const char *v[5]={"p","service","install","--account",".\\UnqualifiedModelAccount"};
 const char *actions[]={"start","stop","remove"};unsigned i;FILE *o=tmpfile(),*e=tmpfile();
 if(!o||!e)return 1;
 T(papacc_server_service_cli(5,v,o,e)==1);
 T(papacc_server_service_cli(3,v,o,e)==2);
 for(i=0;i<3;i++){v[2]=actions[i];T(papacc_server_service_cli(3,v,o,e)==2);}
 v[2]="startup";v[3]="automatic";T(papacc_server_service_cli(4,v,o,e)==2);
 v[3]="manual";T(papacc_server_service_cli(4,v,o,e)==2);
 v[2]="unknown";T(papacc_server_service_cli(3,v,o,e)==1);
 T(papacc_server_service_storage_bytes()>0U);
 T(fclose(o)==0);T(fclose(e)==0);
 puts("Native fixed binding gate denies all mutations before SCM acquisition PASS");return 0;
}
