/* Closed bootstrap is not operational authority. Body still validates every pin. */
#include "scm-n0-private-stdout-profile.h"
int main(int argc,char **argv){WCHAR name[96];ULONGLONG pid=0,created=0;DWORD error;int result;
 if(!ig_transport_bootstrap(argc,argv)||!ig_transport_decimal(argv[11],MAXDWORD,&pid)||!ig_transport_decimal(argv[12],~(ULONGLONG)0,&created)||!MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,argv[10],-1,name,96)){fprintf(stderr,"PRIVATE_STDOUT_BOOTSTRAP_DENY; NO_PREFLIGHT\n");return 2;}
 error=PaccTransportClient(name,(DWORD)pid,created);
 if(error){fprintf(stderr,"PRIVATE_STDOUT_AUTH_DENY error=%lu; NO_PREFLIGHT\n",(unsigned long)error);if(PaccTransportClientClose())ig_transport_write_failed();return 2;}
 result=ig_transport_body(argc-4,argv);
 if(PaccTransportClientClose())ig_transport_write_failed();return result;
}
