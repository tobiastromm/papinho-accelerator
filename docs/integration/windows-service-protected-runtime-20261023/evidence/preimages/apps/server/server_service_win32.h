#ifndef PAPACC_SERVER_SERVICE_WIN32_H
#define PAPACC_SERVER_SERVICE_WIN32_H
#include <stdio.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
/* Native private bridge only. No tokens or curve/provider types in DATA core. */
/* Borrow valid only during synchronous callback, held by SAME SCM owner. */
typedef struct PAPACC_SERVICE_IMAGE_VIEW {
 HANDLE handle;BY_HANDLE_FILE_INFORMATION identity;const wchar_t *path;
 const unsigned char *sha256;DWORD owner_thread;
} PAPACC_SERVICE_IMAGE_VIEW;
typedef int (*PAPACC_SERVICE_PREPARE)(const char *,const char *,const PAPACC_SERVICE_IMAGE_VIEW *,FILE *,FILE *);
int papacc_server_service_cli(int,const char *const *,FILE *,FILE *,PAPACC_SERVICE_PREPARE);
/* Called only by the shared runtime at factual initialization/teardown points.
 * These receipts never authorize a listener or lift operational admission. */
int papacc_server_service_host_active(void);
int papacc_server_service_runtime_ready(void);
int papacc_server_service_runtime_stopping(void);
void papacc_server_service_runtime_closed(void);
void papacc_server_service_runtime_retained(void);
/* Exact internal entry: service host, no fallback to a console launch. */
int papacc_server_service_dispatch(int,char **,int (*)(int,char **));
size_t papacc_server_service_storage_bytes(void);
#endif
