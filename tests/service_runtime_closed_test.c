#include <stdio.h>
#include "server_run_win32.h"
#define T(x) do{if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
static PAPACC_SERVER_NETWORK network=PAPACC_SERVER_NETWORK_INITIALIZER;
static PAPACC_SERVER_IO_LOOP_WIN32 loop=PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
static PAPACC_CONNECTION_MANAGER manager=PAPACC_CONNECTION_MANAGER_INITIALIZER;
int main(void)
{
 PAPACC_BOOL closed=PAPACC_FALSE;
 T(papacc_connection_manager_init(&manager,NULL,0U)==PAPACC_RESULT_OK);
 T(papacc_server_io_loop_win32_init_closed(&loop,&network,NULL,NULL,0U,NULL,0U,NULL,0U,NULL,0U,1U,&manager)==PAPACC_RESULT_OK);
 T(loop.initialized&&loop.session_manager.initialized&&loop.channel_manager.initialized&&loop.network_job_manager.initialized);
 T(!network.is_active&&!network.tcp_platform.initialized&&!network.listener_set.count&&!loop.enabled_capabilities.count&&!manager.count);
 T(papacc_server_io_loop_win32_poll_closed(&loop,0U)==PAPACC_RESULT_OK);
 network.is_active=PAPACC_TRUE;T(papacc_server_io_loop_win32_poll_closed(&loop,0U)!=PAPACC_RESULT_OK);network.is_active=PAPACC_FALSE;
 loop.enabled_capabilities.count=1U;T(papacc_server_io_loop_win32_poll_closed(&loop,0U)!=PAPACC_RESULT_OK);loop.enabled_capabilities.count=0U;
 T(papacc_server_io_loop_win32_poll_closed(&loop,51U)!=PAPACC_RESULT_OK);
 papacc_server_console_win32_request_stop();T(papacc_server_console_win32_stop_requested());
 papacc_server_io_loop_win32_shutdown(&loop);T(!loop.initialized);
 T(papacc_connection_manager_try_shutdown(&manager,&closed)==PAPACC_RESULT_OK&&closed);
 puts("NATIVE_CLOSED_MANAGERS=PASS; ZERO_SOCKET_WSA_LISTENER_CA_CURRENT; SHARED_INIT_MAINTENANCE_TEARDOWN; NOT_SERVICE_TOKEN_QUALIFICATION");return 0;
}
