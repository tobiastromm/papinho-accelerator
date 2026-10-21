#include "config_source.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static PC_DOCUMENT doc;
static unsigned cases,failed;
static void copy_text(char *dst,const char *src){while((*dst++=*src++)!='\0'){} }
static void append_text(char *dst,const char *src){copy_text(dst+strlen(dst),src);}
static FILE *open_read(const char *path)
{
#if defined(_MSC_VER)
 FILE *file=NULL;if(fopen_s(&file,path,"rb")!=0)return NULL;return file;
#else
 return fopen(path,"rb");
#endif
}
static const char *base=
"format_version 1\n"
"binding input\n mode ALL\nend\n"
"channel_tls tls\n security_ref \"real-ref-required\"\nend\n"
"access_auth auth\n method CERTIFICATE_IDENTITY\n provider_ref \"resolver-required\"\nend\n"
"capability_policy policy\n enable 2 1\n rule yes ACCEPT 2 1\n rule no DENY 2 1\nend\n"
"listener front\n binding_ref input\n port 7443\n transport SECURE_PRINCIPAL\n channel_tls_ref tls\n admission CLOSED\n authentication_ref auth\n policy_ref policy\nend\n";
static void check(int yes,const char *name)
{++cases;if(!yes){++failed;printf("FAIL %s\n",name);}}
static PC_RESULT run(const char *s,unsigned max)
{PC_DIAGNOSTIC d;PC_TARGET t;PC_RESULT r;t.max_listeners=max;t.legacy_supported=0;
 r=pc_parse((const unsigned char *)s,strlen(s),&doc,&d);
 if(r==PC_OK)r=pc_validate(&doc,&t,&d);
 if(r!=PC_OK)check(pc_view(&doc)==NULL,"invalid has no view");
 return r;}
static void changed(const char *old,const char *replacement,PC_RESULT expected,const char *name)
{char buffer[PC_SOURCE_MAX+1U];const char *at=strstr(base,old);size_t n;
 if(!at){check(0,"fixture substitution missing");return;}n=(size_t)(at-base);
 memcpy(buffer,base,n);copy_text(buffer+n,replacement);append_text(buffer,at+strlen(old));
 check(run(buffer,1U)==expected,name);}
int main(int argc,char **argv)
{PC_DIAGNOSTIC d;PC_TARGET t;PC_RESULT r;unsigned b;char buf[PC_SOURCE_MAX+2U];FILE *f;size_t n;
 check(run("",1U)==PC_MISSING,"empty");
 check(run("# comments\n ; invalid \" unclosed\n",1U)==PC_MISSING,"comments only");
 check(run(" ;garbage\nformat_version 1 # inline\n",1U)==PC_OK,"disabled no AST");
 check(doc.line_count==1U&&doc.block_count==0U,"disabled nodes absent");
 check(run("format_version 1\nclient_admission DISABLED\nsite_tls_admission DENY\ned11_admission DENY\nnetwork_egress DISABLED\n",1U)==PC_OK,"negative gates");
 check(run("format_version 1; comment\n",1U)==PC_INVALID,"semicolon inline");
 check(run("format_version 1\nclient_admission ENABLED\n",1U)==PC_INVALID,"no admission");
 check(run("format_version 1\nwat x\n",1U)==PC_UNKNOWN,"unknown");
 check(run("format_version 1\nsite_tls x\nend\n",1U)==PC_UNSUPPORTED,"unsupported block");
 check(run("format_version 2\n",1U)==PC_INVALID,"unknown version");
 check(run("format_version 1\nformat_version 1\n",1U)==PC_UNKNOWN,"duplicate version rejected");
 check(run(base,1U)==PC_OK,"listener valid synthetic refs only");
 check(doc.listener_count==1U&&doc.listeners[0].profile==1U&&doc.listeners[0].security!=PAPACC_CONFIG_NO_INDEX,"explicit profile and collection");
 b=doc.listeners[0].policy;
 check(pc_policy_accepts(&doc,b,2U,1U,NULL)==1,"first ACCEPT");
 check(pc_policy_accepts(&doc,b,3U,1U,NULL)==0,"disabled key default deny");
 changed("rule yes ACCEPT 2 1\n rule no DENY 2 1","rule yes DENY 2 1\n rule no ACCEPT 2 1",PC_OK,"first DENY candidate");
 check(pc_policy_accepts(&doc,doc.listeners[0].policy,2U,1U,NULL)==0,"first DENY decision");
 changed("rule yes ACCEPT 2 1\n rule no DENY 2 1","rule yes ACCEPT 2 1 principal \"abcdefabcdefabcdefabcdefabcdefab\"",PC_OK,"Principal condition");
 b=doc.listeners[0].policy;
 check(!pc_policy_accepts(&doc,b,2U,1U,NULL),"absence not identity");
 check(!pc_policy_accepts(&doc,b,2U,1U,"00000000000000000000000000000000"),"divergent Principal");
 check(pc_policy_accepts(&doc,b,2U,1U,"ABCDEFABCDEFABCDEFABCDEFABCDEFAB"),"Principal exact bytes case independent");
 changed("rule yes ACCEPT 2 1","rule yes ACCEPT 2 1 source_ip x",PC_UNSUPPORTED,"ACL unsupported");
 changed("rule yes ACCEPT 2 1","rule yes ACCEPT",PC_INVALID,"partial rule");
 changed("enable 2 1","enable 2 1\n enable 02 01",PC_DUPLICATE,"numeric duplicate");
 changed("enable 2 1","enable 9 1",PC_UNSUPPORTED,"unimplemented capability");
 changed("rule yes ACCEPT 2 1","rule yes ACCEPT 2 1 ; principal",PC_INVALID,"no partial predicate");
 changed("transport SECURE_PRINCIPAL\n","",PC_MISSING,"profile absent no Legacy");
 changed("transport SECURE_PRINCIPAL","transport AUTO",PC_UNSUPPORTED,"profile unknown");
 changed("transport SECURE_PRINCIPAL","transport LEGACY_ENDPOINT",PC_UNSUPPORTED,"Legacy target deny");
 changed("channel_tls_ref tls\n","",PC_REFERENCE,"security missing");
 changed("security_ref \"real-ref-required\"","security_ref \"\"",PC_MISSING,"empty security ref");
 changed("binding_ref input","binding_ref missing",PC_REFERENCE,"unresolved bind");
 changed("mode ALL","mode SELECTED",PC_INVALID,"selected without refs");
 changed("mode ALL","mode SELECTED\n interface_ref \"machine-local-id\"",PC_OK,"selected structural ref");
 changed("mode ALL","mode UNKNOWN",PC_INVALID,"bind invalid");
 changed("port 7443","port 0",PC_INVALID,"port zero");
 changed("port 7443","port 65536",PC_INVALID,"port overflow");
 changed("port 7443","port 9999999999999999999",PC_INVALID,"unsigned overflow");
 changed("port 7443","port 7443\n port 7443",PC_DUPLICATE,"duplicate field");
 changed("method CERTIFICATE_IDENTITY\n provider_ref \"resolver-required\"","method NONE",PC_INVALID,"CLOSED NONE invalid");
 changed("channel_tls tls","channel_tls tls\nend\nchannel_tls tls",PC_DUPLICATE,"duplicate id");
 changed("mode ALL\nend","mode ALL\n;end",PC_UNKNOWN,"partially disabled end");
 changed("binding input",";binding input",PC_UNKNOWN,"partially disabled opener");
 changed("port 7443","; port 7443",PC_INVALID,"disabled required field");
 changed("security_ref \"real-ref-required\"","security_ref \"a;#b\\\\c\\\"d\" # ok",PC_OK,"literal and approved escapes");
 changed("security_ref \"real-ref-required\"","security_ref \"bad\\n\"",PC_INVALID,"unknown escape");
 changed("security_ref \"real-ref-required\"","security_ref \"open",PC_INVALID,"unclosed quote");
 changed("mode ALL","mode AL\rL",PC_INVALID,"bare CR inside line");
 check(run(base,0U)==PC_LIMIT,"target no listener support");
 copy_text(buf,base);append_text(buf,"listener second\n binding_ref input\n port 8443\n transport SECURE_PRINCIPAL\n channel_tls_ref tls\n admission CLOSED\n authentication_ref auth\n policy_ref policy\nend\n");
 check(run(buf,1U)==PC_LIMIT,"collection not truncated");
 check(run(buf,2U)==PC_OK&&doc.listener_count==2U,"collection two offline");
 copy_text(buf,base);append_text(buf,"listener second\n binding_ref input\n port 7443\n transport SECURE_PRINCIPAL\n channel_tls_ref tls\n admission CLOSED\n authentication_ref auth\n policy_ref policy\nend\n");
 check(run(buf,2U)==PC_CONFLICT,"ambiguous endpoints conservative denial");
 check(run("\357\273\277format_version 1\r\n",1U)==PC_OK,"UTF8 BOM CRLF");
 check(run("format_version 1\n;\300\200\n",1U)==PC_INVALID,"UTF8 malformed disabled");
 buf[0]='\0';check(pc_parse((const unsigned char *)buf,1U,&doc,&d)==PC_INVALID,"NUL");
 memset(buf,'x',sizeof(buf));check(pc_parse((const unsigned char *)buf,sizeof(buf),&doc,&d)==PC_LIMIT,"source capacity");
 copy_text(buf,"format_version 1\n");for(b=0U;b<96U;++b)append_text(buf,"client_admission DISABLED\n");
 check(run(buf,1U)==PC_DUPLICATE,"duplicate globals");
 copy_text(buf,"format_version 1\nchannel_tls x\n security_ref \"");n=strlen(buf);memset(buf+n,'x',65U);buf[n+65U]='\0';append_text(buf,"\"\nend\n");
 check(run(buf,1U)==PC_LIMIT,"string capacity");
 copy_text(buf,"format_version 1\n");for(b=0U;b<16U;++b){char part[100];copy_text(part,"binding bA\n mode ALL\nend\n");part[9]=(char)('A'+b);append_text(buf,part);}
 check(run(buf,1U)==PC_OK,"exact block capacity");append_text(buf,"binding extra\n mode ALL\nend\n");check(run(buf,1U)==PC_LIMIT,"block overflow");
 check(run("format_version 1\nbinding a\n mode ALL\nend\n",1U)==PC_OK,"no implicit listener");
 check(doc.listener_count==0U,"zero listener not default");
 t.max_listeners=1U;t.legacy_supported=1;
 copy_text(buf,base);{char *at=strstr(buf,"SECURE_PRINCIPAL");size_t tail=strlen(at+16U);memmove(at+15U,at+16U,tail+1U);memcpy(at,"LEGACY_ENDPOINT",15U);at=strstr(buf," channel_tls_ref tls\n");memmove(at,at+21U,strlen(at+21U)+1U);}
 r=pc_parse((const unsigned char *)buf,strlen(buf),&doc,&d);if(r==PC_OK)r=pc_validate(&doc,&t,&d);
 check(r==PC_OK,"Legacy explicit offline support");if(r==PC_OK)check(doc.listeners[0].profile==2U&&doc.listeners[0].security==PAPACC_CONFIG_NO_INDEX&&(doc.listeners[0].presence&128U)==0U,"Legacy absence preserved");
 check(run("format_version 1\r",1U)==PC_INVALID,"bare terminal CR rejected");
 check(run("format_version 1\n; ignored\n\tlistener a\nend\n",1U)==PC_REFERENCE,"visual indent and physical lines");
 r=pc_parse((const unsigned char *)"format_version 1\n\tunknown a\n",strlen("format_version 1\n\tunknown a\n"),&doc,&d);
 check(r!=PC_OK&&d.line==2U&&d.column>0U,"diagnostic location available");
 copy_text(buf,"format_version 1\ncapability_policy many\n");
 for(b=0U;b<95U;++b)append_text(buf," rule a DENY 2 1\n");
 append_text(buf,"end\n");r=pc_parse((const unsigned char *)buf,strlen(buf),&doc,&d);
 check(r==PC_LIMIT,"active line capacity");
 check(run("format_version 1\ncapability_policy a\n rule a DENY 2 1 principal \"00000000000000000000000000000000\" extra extra\nend\n",1U)==PC_LIMIT,"token capacity");
 copy_text(buf,"format_version 1\nchannel_tls a\n security_ref \"");n=strlen(buf);memset(buf+n,'x',64U);buf[n+64U]='\0';append_text(buf,"\"\nend\n");
 check(run(buf,1U)==PC_OK,"exact string capacity");
 check(run("format_version 1\nchannel_tls a\n security_ref \"a\"\nend\n",1U)==PC_OK,"owned copied strings");
 check(doc.lines[2].tokens[1].text[0]=='a',"input lifetime not borrowed");
 check(pc_parse(NULL,0U,&doc,&d)==PC_MISSING,"NULL empty safe");
 check(pc_parse(NULL,1U,&doc,&d)==PC_INVALID,"NULL nonempty rejected");
 if(argc==2){f=open_read(argv[1]);check(f!=NULL,"distributed sample present");if(f){n=fread(buf,1U,sizeof(buf),f);check(!ferror(f),"sample read");check(fclose(f)==0,"sample close");r=pc_parse((const unsigned char *)buf,n,&doc,&d);if(r==PC_OK)r=pc_validate(&doc,&t,&d);check(r==PC_OK&&doc.line_count==5U&&doc.listener_count==0U&&doc.block_count==0U,"distributed sample fail closed");}}
 printf("CASES=%u FAILURES=%u\n",cases,failed);
 printf("ED11_DOCUMENT=%lu LINE=%lu TOKEN=%lu LISTENER=%lu DIAGNOSTIC=%lu TARGET=%lu\n",(unsigned long)sizeof(PC_DOCUMENT),(unsigned long)sizeof(PC_LINE),(unsigned long)sizeof(PC_TOKEN),(unsigned long)sizeof(PC_LISTENER),(unsigned long)sizeof(PC_DIAGNOSTIC),(unsigned long)sizeof(PC_TARGET));
 return failed?1:0;
}
