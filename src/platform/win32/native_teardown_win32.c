#include "native_teardown_win32.h"
#ifdef PAPACC_NATIVE_TEARDOWN_TEST_ONLY
static int socket_result,cleanup_result;
static unsigned socket_calls,cleanup_calls;
void papacc_native_teardown_test_results(int s,int w){socket_result=s;cleanup_result=w;socket_calls=cleanup_calls=0U;}
unsigned papacc_native_teardown_test_socket_calls(void){return socket_calls;}
unsigned papacc_native_teardown_test_cleanup_calls(void){return cleanup_calls;}
#endif
PAPACC_RESULT papacc_native_socket_close_once(SOCKET *value,PAPACC_BOOL *unresolved)
{
 int result;
 if(!value||!unresolved)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(*unresolved!=PAPACC_FALSE)return PAPACC_RESULT_INVALID_STATE;
 if(*value==INVALID_SOCKET)return PAPACC_RESULT_OK;
#ifdef PAPACC_NATIVE_TEARDOWN_TEST_ONLY
 ++socket_calls;result=socket_result;
#else
 result=closesocket(*value);
#endif
 if(result!=0){*unresolved=PAPACC_TRUE;return PAPACC_RESULT_INTERNAL_ERROR;}
 *value=INVALID_SOCKET;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_native_winsock_cleanup_once(PAPACC_BOOL *initialized,PAPACC_BOOL *unresolved)
{
 int result;
 if(!initialized||!unresolved)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(*unresolved!=PAPACC_FALSE)return PAPACC_RESULT_INVALID_STATE;
 if(*initialized==PAPACC_FALSE)return PAPACC_RESULT_OK;
 if(*initialized!=PAPACC_TRUE)return PAPACC_RESULT_INVALID_STATE;
#ifdef PAPACC_NATIVE_TEARDOWN_TEST_ONLY
 ++cleanup_calls;result=cleanup_result;
#else
 result=WSACleanup();
#endif
 if(result!=0){*unresolved=PAPACC_TRUE;return PAPACC_RESULT_INTERNAL_ERROR;}
 *initialized=PAPACC_FALSE;return PAPACC_RESULT_OK;
}