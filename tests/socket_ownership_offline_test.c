/* Synthetic boundary/failure tests only. No socket, runtime or wait-set creation. */
#include "pst_socket_attachment_win32.h"
#include "papinho_secure_transport_win32.h"
#include "native_teardown_win32.h"
#include "tcp_accept_win32.h"
#include "tcp_outbound_win32.h"
#include "tcp_platform.h"
#include <stdio.h>
#define CHECK(x) do{if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
static int create_calls,attach_calls,release_calls,create_failure;
static PAPACC_BOOL accept_ownership;
static PAPACC_RESULT attach_result;
static int wrapper_identity;
PST_RESULT PST_CALL pst_win32_socket_transport_create(pst_size value,pst_transport **out)
{(void)value;++create_calls;*out=NULL;if(create_failure)return PST_RESULT_OUT_OF_MEMORY;*out=(pst_transport *)&wrapper_identity;return PST_RESULT_OK;}
void PST_CALL pst_transport_release(pst_transport *value){(void)value;++release_calls;}
PAPACC_RESULT papacc_pst_secure_processor_attach(PAPACC_PST_SECURE_PROCESSOR *p,pst_transport *t,PAPACC_BOOL *accepted)
{(void)p;(void)t;++attach_calls;*accepted=accept_ownership;return attach_result;}
int main(void)
{
 PAPACC_PST_SECURE_PROCESSOR processor=PAPACC_PST_SECURE_PROCESSOR_INITIALIZER;
 PAPACC_TCP_ACCEPTED_SOCKET_WIN32 accepted=PAPACC_TCP_ACCEPTED_SOCKET_WIN32_INITIALIZER;
 PAPACC_TCP_OUTBOUND_WIN32 connector=PAPACC_TCP_OUTBOUND_WIN32_INITIALIZER;
 PAPACC_TCP_PLATFORM platform=PAPACC_TCP_PLATFORM_INITIALIZER;
 pst_transport *retained=NULL;SOCKET alias;PAPACC_BOOL consumed;int cases=0;
 processor.state=PAPACC_PST_SECURE_PROCESSOR_CREATED;
 /* Successful attach transfers the socket once and retains no alias. */
 alias=(SOCKET)42;accept_ownership=PAPACC_TRUE;attach_result=PAPACC_RESULT_OK;
 CHECK(papacc_pst_socket_attach(&processor,&alias,&retained,&consumed)==PAPACC_RESULT_OK);
 CHECK(alias==INVALID_SOCKET&&consumed&&retained==NULL&&attach_calls==1);++cases;
 /* Failure after consumption is still PST-owned, never closed by consumer. */
 alias=(SOCKET)43;attach_result=PAPACC_RESULT_INTERNAL_ERROR;
 CHECK(papacc_pst_socket_attach(&processor,&alias,&retained,&consumed)!=PAPACC_RESULT_OK);
 CHECK(alias==INVALID_SOCKET&&consumed&&retained==NULL);++cases;
 /* Failure before consumption preserves the actual wrapper in caller owner. */
 alias=(SOCKET)44;accept_ownership=PAPACC_FALSE;
 CHECK(papacc_pst_socket_attach(&processor,&alias,&retained,&consumed)!=PAPACC_RESULT_OK);
 CHECK(alias==INVALID_SOCKET&&!consumed&&retained==(pst_transport *)&wrapper_identity);++cases;
 /* Retained lifetime cannot be overwritten/retried or reused by another attach. */
 alias=(SOCKET)45;CHECK(papacc_pst_socket_attach(&processor,&alias,&retained,&consumed)==PAPACC_RESULT_INVALID_STATE);
 CHECK(alias==(SOCKET)45&&attach_calls==3&&create_calls==3&&release_calls==0);++cases;
 /* Independent fixture for create failure: original socket ownership remains. */
 retained=NULL;create_failure=1;
 CHECK(papacc_pst_socket_attach(&processor,&alias,&retained,&consumed)!=PAPACC_RESULT_OK);
 CHECK(alias==(SOCKET)45&&!consumed&&retained==NULL&&attach_calls==3);++cases;
 create_failure=0;attach_result=PAPACC_RESULT_OK;accept_ownership=PAPACC_FALSE;
 CHECK(papacc_pst_socket_attach(&processor,&alias,&retained,&consumed)==PAPACC_RESULT_INVALID_STATE);
 CHECK(alias==INVALID_SOCKET&&retained!=NULL&&release_calls==0);++cases;
 /* Actual accepted-owner close path, with fixed synthetic PAL close outcome. */
 accepted.native_socket=(SOCKET)51;accepted.is_open=PAPACC_TRUE;
 papacc_native_teardown_test_results(0,0);
 CHECK(papacc_tcp_accepted_socket_win32_try_close(&accepted)==PAPACC_RESULT_OK);
 CHECK(accepted.native_socket==INVALID_SOCKET&&papacc_native_teardown_test_socket_calls()==1);
 CHECK(papacc_tcp_accepted_socket_win32_try_close(&accepted)==PAPACC_RESULT_OK);
 CHECK(papacc_native_teardown_test_socket_calls()==1);++cases;
 accepted.native_socket=(SOCKET)52;accepted.is_open=PAPACC_TRUE;
 papacc_native_teardown_test_results(SOCKET_ERROR,0);
 CHECK(papacc_tcp_accepted_socket_win32_try_close(&accepted)!=PAPACC_RESULT_OK);
 CHECK(accepted.native_socket==(SOCKET)52&&accepted.close_unresolved);
 papacc_tcp_accepted_socket_win32_close(&accepted);
 CHECK(accepted.native_socket==(SOCKET)52&&papacc_native_teardown_test_socket_calls()==1);++cases;
 /* Stop/cancel repeats cannot close uncertain connectors or permit reuse. */
 connector.native_socket=(SOCKET)53;connector.state=PAPACC_TCP_OUTBOUND_CONNECTING;
 papacc_native_teardown_test_results(SOCKET_ERROR,0);
 papacc_tcp_outbound_win32_cancel(&connector);
 CHECK(connector.native_socket==(SOCKET)53&&connector.close_unresolved);
 papacc_tcp_outbound_win32_cancel(&connector);
 CHECK(papacc_tcp_outbound_win32_try_close(&connector)!=PAPACC_RESULT_OK);
 CHECK(papacc_native_teardown_test_socket_calls()==1);
 CHECK(papacc_tcp_outbound_win32_take_socket(&connector,&alias)!=PAPACC_RESULT_OK);++cases;
 platform.initialized=PAPACC_TRUE;papacc_native_teardown_test_results(0,SOCKET_ERROR);
 CHECK(papacc_tcp_platform_try_shutdown(&platform)!=PAPACC_RESULT_OK);
 CHECK(platform.initialized&&platform.close_unresolved);
 CHECK(papacc_tcp_platform_try_shutdown(&platform)!=PAPACC_RESULT_OK);
 CHECK(papacc_native_teardown_test_cleanup_calls()==1);++cases;
 platform=(PAPACC_TCP_PLATFORM)PAPACC_TCP_PLATFORM_INITIALIZER;
 platform.initialized=PAPACC_TRUE;papacc_native_teardown_test_results(0,0);
 CHECK(papacc_tcp_platform_try_shutdown(&platform)==PAPACC_RESULT_OK);
 CHECK(!platform.initialized);CHECK(papacc_tcp_platform_try_shutdown(&platform)==PAPACC_RESULT_OK);
 CHECK(papacc_native_teardown_test_cleanup_calls()==1);++cases;
 printf("PASS_OFFLINE_SYNTHETIC cases=%d NO_NATIVE_ACQUISITION_OR_RELEASE_PROOF\n",cases);
 return 0;
}