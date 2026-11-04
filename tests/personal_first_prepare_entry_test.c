#include "server_launcher_owner_win32.h"
#include <stdio.h>
#include <string.h>
static unsigned prepare_calls,legacy_calls,parent_calls,prepared_calls;
static int entry_prepared(const char *s,const char *p,const char *l,FILE *e)
{(void)s;(void)p;(void)l;(void)e;prepared_calls++;return 44;}
#define papacc_server_launcher_personal_prepared_validate entry_prepared
static int entry_parent(const char *source,const char *parent,const char *leaf,FILE *err)
{(void)source;(void)parent;(void)leaf;(void)err;parent_calls++;return 43;}
static int entry_prepare(const char *source,const char *parent,const char *leaf,FILE *err)
{(void)source;(void)parent;(void)leaf;(void)err;prepare_calls++;return 42;}
static int entry_legacy(const char *source,const char *parent,const char *leaf,FILE *err)
{(void)source;(void)parent;(void)leaf;(void)err;legacy_calls++;return 41;}
#define papacc_server_launcher_personal_first_prepare entry_prepare
#define papacc_server_launcher_personal_parent_prepare entry_parent
#define papacc_server_launcher_personal_first_install entry_legacy
#include "../apps/server/server_config_cli.c"
#define C(x) do{if(!(x)){printf("FCA_ENTRY_FAIL %u\n",(unsigned)__LINE__);return 1;}}while(0)
int main(void)
{
 const char *args[]={"papacc_server","config","first-install","--declare","NEW_INSTALL_CA_PREPARE","--source","C:\\Protected\\initial.conf","--parent","C:\\ProgramData\\Papinho","--container","Accelerator",NULL};unsigned i;
 C(papacc_server_config_cli(11,args,stdout,stderr)==42&&prepare_calls==1U&&!legacy_calls);
 args[4]="NEW_INSTALL";C(papacc_server_config_cli(11,args,stdout,stderr)==41&&prepare_calls==1U&&legacy_calls==1U);
 for(i=0;i<6U;i++){static const char *invalid[]={"READ","TRANSPORT_ONLY","CA_INITIALIZE","NEW_INSTALL_CA_PREPARE_EXTRA","","NEW_INSTALL_CA_PREPARE "};args[4]=invalid[i];C(papacc_server_config_cli(11,args,stdout,stderr)==1&&prepare_calls==1U&&legacy_calls==1U);}
 args[4]="NEW_INSTALL_CA_PREPARE";C(papacc_server_config_cli(12,args,stdout,stderr)==1&&prepare_calls==1U);
 C(papacc_server_config_cli(10,args,stdout,stderr)==1&&prepare_calls==1U);
 args[6]="";C(papacc_server_config_cli(11,args,stdout,stderr)==1&&prepare_calls==1U);
 args[6]="C:\\Protected\\initial.conf";args[3]="--profile";C(papacc_server_config_cli(11,args,stdout,stderr)==1&&prepare_calls==1U);
 args[3]="--declare";args[4]="NEW_INSTALL_PARENT_PREPARE";args[8]="C:\\ProgramData";args[10]="Papinho";
 C(papacc_server_config_cli(11,args,stdout,stderr)==43&&parent_calls==1U&&prepare_calls==1U&&legacy_calls==1U);
 args[4]="NEW_INSTALL_PARENT_PREPARE_EXTRA";C(papacc_server_config_cli(11,args,stdout,stderr)==1&&parent_calls==1U);
 args[4]="PREPARED_INSTANCE_VALIDATE";C(papacc_server_config_cli(11,args,stdout,stderr)==44&&prepared_calls==1U&&prepare_calls==1U&&parent_calls==1U);
 args[4]="PREPARED_INSTANCE_VALIDATE_EXTRA";C(papacc_server_config_cli(11,args,stdout,stderr)==1&&prepared_calls==1U);
 puts("FCA_PDP_FCR_ENTRY_MODEL 14 cases PASS; ROUTING_ONLY_NOT_AUTHORITY; NO_FILE_TOKEN_RESOURCE_OR_THREAD_QUERY");return 0;
}
