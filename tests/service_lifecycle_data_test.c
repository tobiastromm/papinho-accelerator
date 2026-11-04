#include "server_service_data.h"
#include <stdio.h>
#include <string.h>
#define T(x) do{if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
int main(void)
{
 PS_REQUEST q;PS_LIFETIME s;char big[140];const char *v[5]={"p","service","install","--account",".\\LocalOperator"};
 T(ps_parse(3,v,&q)==1&&q.action==PS_INSTALL&&!strcmp(q.account,"NT SERVICE\\PapinhoAccelerator"));
 T(q.install_profile==PS_INSTALL_PRODUCT);
 v[3]="--scq-closed";T(ps_parse(4,v,&q)==1&&q.action==PS_INSTALL&&q.install_profile==PS_INSTALL_SCQ_CLOSED&&!q.prepare_lab);
 T(!ps_parse(5,v,&q));v[3]="--account";T(ps_parse(3,v,&q)==1);
 T(ps_action_allowed(q.action,0,PS_EMPTY));T(!ps_action_allowed(q.action,1,PS_STOPPED));T(!ps_parse(5,v,&q));
 v[2]="start";T(ps_parse(3,v,&q)==1&&q.action==PS_START);
 v[3]="--scq-closed";T(ps_parse(4,v,&q)==1&&q.start_profile==PS_START_SCQ_CLOSED);T(ps_parse(3,v,&q)==1&&!q.start_profile);
 T(ps_action_allowed(q.action,1,PS_STOPPED));T(!ps_action_allowed(q.action,0,PS_EMPTY));
 T(!ps_action_allowed(q.action,1,PS_RUNNING));T(!ps_action_allowed(q.action,1,PS_RETAINED));
 v[2]="stop";T(ps_parse(3,v,&q)==1&&q.action==PS_STOP);T(ps_action_allowed(q.action,1,PS_RUNNING));
 T(!ps_action_allowed(q.action,1,PS_STOP_PENDING));T(!ps_action_allowed(q.action,0,PS_EMPTY));
 v[2]="status";T(ps_parse(3,v,&q)==1&&q.action==PS_STATUS);T(ps_action_allowed(q.action,0,PS_EMPTY));
 v[2]="startup";v[3]="automatic";T(ps_parse(4,v,&q)==1&&q.action==PS_AUTOMATIC);
 v[3]="manual";T(ps_parse(4,v,&q)==1&&q.action==PS_MANUAL);
 T(ps_action_allowed(q.action,1,PS_RUNNING));T(!ps_action_allowed(q.action,0,PS_EMPTY));
 v[3]="delayed";T(!ps_parse(4,v,&q));v[2]="remove";T(ps_parse(3,v,&q)==1&&q.action==PS_REMOVE);
 T(ps_action_allowed(q.action,1,PS_STOPPED));T(!ps_action_allowed(q.action,1,PS_RUNNING));
 T(!ps_action_allowed(q.action,1,PS_RETAINED));T(!ps_action_allowed(999,1,PS_STOPPED));
 v[2]="install";v[3]="--account";v[4]="LocalSystem";T(!ps_parse(5,v,&q));
 v[4]=".\\bad name";T(!ps_parse(5,v,&q));v[4]=".\\a\"b";T(!ps_parse(5,v,&q));
 memset(big,'a',sizeof(big));big[0]='.';big[1]='\\';big[sizeof(big)-1]=0;v[4]=big;T(!ps_parse(5,v,&q));
 v[4]=".\\ordinary";T(!ps_parse(4,v,&q));T(!ps_parse(5,v,NULL));
 v[2]="bogus";T(!ps_parse(3,v,&q));v[2]=NULL;T(!ps_parse(3,v,&q));
 v[1]="config";T(ps_parse(2,v,&q)==-1);T(ps_parse(0,NULL,&q)==-1);
 memset(&s,0,sizeof(s));T(!ps_running(&s));T(!ps_stop(&s));T(!ps_finish(&s,1,0));
 T(ps_begin(&s));T(!ps_begin(&s));T(ps_running(&s));T(!ps_running(&s));T(ps_stop(&s));
 T(!ps_running(&s));T(!ps_finish(&s,0,0));T(s.phase==PS_STOP_PENDING);T(ps_finish(&s,1,0));
 T(s.phase==PS_STOPPED&&s.terminal);T(!ps_finish(&s,1,0));T(!ps_stop(&s));T(!ps_begin(&s));
 memset(&s,0,sizeof(s));T(ps_begin(&s));T(ps_stop(&s));T(!ps_running(&s));
 T(!ps_finish(&s,0,1));T(s.phase==PS_RETAINED);T(!ps_finish(&s,1,0));T(!ps_stop(&s));
 memset(&s,0,sizeof(s));T(ps_begin(&s));T(ps_finish(&s,1,0)); /* startup failure, no resources */
 T(!ps_begin(NULL));T(!ps_running(NULL));T(!ps_stop(NULL));T(!ps_finish(NULL,1,0));
 {
 PS_BINDING_FACTS f,g;PS_LAB_BINDING b;const char *a[]={"p","service","install","--prepare-lab","--installation","C:/Lab","--configuration","papacc.conf"};
 T(ps_parse(8,a,&q)&&q.prepare_lab&&!strcmp(q.configuration,"papacc.conf"));
 a[7]="../escape.conf";T(!ps_parse(8,a,&q));
 memset(&f,0,sizeof(f));memset(&b,0,sizeof(b));T(!ps_lab_binding_capture(&b,&f));
 f.configuration_bytes=128;f.installation[2]=1;f.configuration[2]=2;f.executable[2]=3;
 memset(f.attempt,1,32);memset(f.operator_id,2,32);memset(f.config_hash,3,32);memset(f.image_hash,4,32);
 memset(f.parent_acl,5,32);memset(f.config_acl,6,32);memset(f.image_acl,7,32);
 T(ps_lab_binding_capture(&b,&f));T(!ps_lab_binding_capture(&b,&f));g=f;g.config_hash[0]++;
 T(!ps_lab_binding_complete(&b,&g,0)&&b.phase==3);T(!ps_lab_binding_complete(&b,&f,0));
 memset(&b,0,sizeof(b));T(ps_lab_binding_capture(&b,&f));g=f;g.operator_id[0]++;T(!ps_lab_binding_complete(&b,&g,0));
 memset(&b,0,sizeof(b));T(ps_lab_binding_capture(&b,&f));g=f;g.image_hash[0]++;T(!ps_lab_binding_complete(&b,&g,0));
 memset(&b,0,sizeof(b));T(ps_lab_binding_capture(&b,&f));g=f;g.installation[2]++;T(!ps_lab_binding_complete(&b,&g,0));
 memset(&b,0,sizeof(b));T(ps_lab_binding_capture(&b,&f));T(!ps_lab_binding_complete(&b,&f,1));
 memset(&b,0,sizeof(b));T(ps_lab_binding_capture(&b,&f));T(ps_lab_binding_complete(&b,&f,0));T(b.phase==2);T(!ps_lab_binding_complete(&b,&f,0));ps_lab_binding_invalidate(&b);T(b.phase==3);
 }
 puts("DATA lifecycle, replay, Stop-before-running and retained-quiescence negatives PASS");return 0;
}
