#ifndef PAPACC_PERSONAL_CONSOLE_TOKEN_DEFINED
#define PAPACC_PERSONAL_CONSOLE_TOKEN_DEFINED
static int personal_console_token(HANDLE h,const char *s)
{
 static const char hex[]="0123456789abcdef";char b[5];const unsigned char *p=(const unsigned char *)s;
 if(!personal_console_write(h,"\""))return 0;
 while(*p){if(*p>=32U&&*p<127U&&*p!='"'&&*p!='\\'){b[0]=(char)*p;b[1]=0;}
 else{b[0]='\\';b[1]='x';b[2]=hex[*p>>4];b[3]=hex[*p&15U];b[4]=0;}
 if(!personal_console_write(h,b))return 0;++p;}
 return personal_console_write(h,"\"");
}
#endif
