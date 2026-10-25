#ifndef PAPACC_SERVER_SERVICE_WIN32_H
#define PAPACC_SERVER_SERVICE_WIN32_H
#include <stdio.h>
/* Native private bridge only. No tokens or curve/provider types in DATA core. */
int papacc_server_service_cli(int,const char *const *,FILE *,FILE *);
/* Exact internal entry: service host, no fallback to a console launch. */
int papacc_server_service_dispatch(int,char **,int (*)(int,char **));
size_t papacc_server_service_storage_bytes(void);
#endif
