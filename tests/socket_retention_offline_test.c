/* Actual owner entry points with synthetic retained records; no native IO. */
#include "server_secure_io_win32.h"
#include "pst_outbound_tls.h"
#include "c3_bridge_win32.h"
#include <windows.h>
#include <stdio.h>
#define CHECK(x) do{if(!(x)){printf("FAIL line=%d\n",__LINE__);return 1;}}while(0)
static PAPACC_SERVER_SECURE_IO_WIN32 secure=PAPACC_SERVER_SECURE_IO_WIN32_INITIALIZER;
static PAPACC_SERVER_IO_LOOP_WIN32 loop=PAPACC_SERVER_IO_LOOP_WIN32_INITIALIZER;
static PAPACC_SERVER_SECURE_CANDIDATE_WIN32 candidate=PAPACC_SERVER_SECURE_CANDIDATE_WIN32_INITIALIZER;
static PAPACC_OUTBOUND_TLS tls=PAPACC_OUTBOUND_TLS_INITIALIZER;
static PAPACC_C3_BRIDGE bridge;
static int wrapper;
int main(void)
{
 PAPACC_BOOL closed=PAPACC_TRUE;
 secure.protocol_loop=&loop;secure.owner_thread_id=GetCurrentThreadId();
 secure.candidates=&candidate;secure.candidate_capacity=1U;secure.initialized=PAPACC_TRUE;
 candidate.owner=&secure;candidate.unconsumed_transport=(pst_transport *)&wrapper;
 candidate.state=PAPACC_SERVER_SECURE_CANDIDATE_FAILED;
 CHECK(papacc_server_secure_io_win32_request_stop(&secure)!=PAPACC_RESULT_OK);
 CHECK(secure.stop_state==PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL);
 CHECK(!papacc_server_secure_io_win32_is_stopped(&secure));
 CHECK(papacc_server_secure_io_win32_try_shutdown(&secure)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_secure_io_win32_try_shutdown(&secure)!=PAPACC_RESULT_OK);
 CHECK(candidate.unconsumed_transport==(pst_transport *)&wrapper&&secure.initialized);
 CHECK(papacc_server_secure_io_win32_request_stop(&secure)!=PAPACC_RESULT_OK);
 tls.unconsumed_transport=(pst_transport *)&wrapper;
 CHECK(papacc_outbound_tls_try_close(&tls,&closed)!=PAPACC_RESULT_OK&&!closed);
 papacc_outbound_tls_close(&tls);
 CHECK(tls.unconsumed_transport==(pst_transport *)&wrapper);
 bridge.initialized=PAPACC_TRUE;bridge.owner_thread=GetCurrentThreadId();
 bridge.retained=PAPACC_TRUE;bridge.unconsumed_transport=(pst_transport *)&wrapper;
 CHECK(papacc_c3_bridge_release_child(&bridge)!=PAPACC_RESULT_OK);
 CHECK(papacc_c3_bridge_close_peer(&bridge)!=PAPACC_RESULT_OK);
 CHECK(papacc_c3_bridge_reset_destroyed(&bridge)!=PAPACC_RESULT_OK);
 CHECK(bridge.unconsumed_transport==(pst_transport *)&wrapper&&bridge.retained);
 puts("PASS_OFFLINE_SYNTHETIC actual_Stop_shutdown_outbound_containment NO_NATIVE_RELEASE_PROOF");
 return 0;
}