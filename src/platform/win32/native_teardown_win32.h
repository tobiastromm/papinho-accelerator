#ifndef PAPACC_NATIVE_TEARDOWN_WIN32_H
#define PAPACC_NATIVE_TEARDOWN_WIN32_H
#include <winsock2.h>
#include <papacc/types.h>
/* Stateless PAL mechanisms called by the existing owner. Failure preserves
 * its handle/acquisition and permanently marks uncertainty: no blind retry. */
PAPACC_RESULT papacc_native_socket_close_once(SOCKET *,PAPACC_BOOL *);
PAPACC_RESULT papacc_native_winsock_cleanup_once(PAPACC_BOOL *,PAPACC_BOOL *);
#ifdef PAPACC_NATIVE_TEARDOWN_TEST_ONLY
void papacc_native_teardown_test_results(int,int);
unsigned papacc_native_teardown_test_socket_calls(void);
unsigned papacc_native_teardown_test_cleanup_calls(void);
#endif
#endif