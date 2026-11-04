/* Administrative UPDATE adapter only. Same canonical owner and U1-U7/U2 act;
 * this executable does not install/protect itself or grant authority by name.
 * An independently qualified protected original remains mandatory at runtime.
 */
#include <stdio.h>
#include <string.h>
#include "server_launcher_owner_win32.h"

static int papacc_update_entry(int argc,const char *const *argv,FILE *out,FILE *err)
{
 int result;
 if(argc<2||!argv||!argv[1]||!out||!err||strcmp(argv[1],"--owner-manual-binary-update"))return 2;
 result=papacc_server_launcher_binary_update_cli(argc,argv,out,err);
 return result==0?0:2;
}

int main(int argc,char **argv)
{return papacc_update_entry(argc,(const char *const *)argv,stdout,stderr);}
