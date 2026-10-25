/* T0-T9 transport resource, not an admission owner. One session per process. */
#ifndef PACC_PRIVATE_STDOUT_H
#define PACC_PRIVATE_STDOUT_H
#include <windows.h>
#include <stdarg.h>
#ifdef PACC_TRANSPORT_DLL
#define PACC_TAPI __declspec(dllexport)
#else
#define PACC_TAPI
#endif
PACC_TAPI DWORD WINAPI PaccTransportOpen(void);
PACC_TAPI DWORD WINAPI PaccTransportCreate(const WCHAR *exe,const WCHAR *arguments);
PACC_TAPI DWORD WINAPI PaccTransportAuthenticate(void);
PACC_TAPI DWORD WINAPI PaccTransportRead(BYTE *out,DWORD capacity,DWORD *length);
PACC_TAPI DWORD WINAPI PaccTransportWait(DWORD *exit_code);
PACC_TAPI DWORD WINAPI PaccTransportClose(void);
PACC_TAPI DWORD WINAPI PaccTransportStatus(DWORD *pid,ULONGLONG *creation,DWORD *first,DWORD *retained);
PACC_TAPI DWORD WINAPI PaccTransportClient(const WCHAR *name,DWORD pid,ULONGLONG creation);
PACC_TAPI int PaccTransportVPrintf(const char *format,va_list args);
PACC_TAPI int PaccTransportPrintf(const char *format,...);
PACC_TAPI int PaccTransportPuts(const char *text);
PACC_TAPI int PaccTransportFlush(void);
PACC_TAPI DWORD PaccTransportClientClose(void);
#endif
