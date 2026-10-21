#ifndef PAPACC_PST_SOCKET_ATTACHMENT_WIN32_H
#define PAPACC_PST_SOCKET_ATTACHMENT_WIN32_H
#include <winsock2.h>
#include "pst_secure_processor.h"
/* Mechanism only: all storage belongs to the existing caller owner.
 * On wrapper creation the native alias is invalidated, irrespective of attach.
 * Rejected/unconsumed wrappers remain reachable for conservative containment. */
PAPACC_RESULT papacc_pst_socket_attach(PAPACC_PST_SECURE_PROCESSOR *,SOCKET *,pst_transport **,PAPACC_BOOL *);
#endif