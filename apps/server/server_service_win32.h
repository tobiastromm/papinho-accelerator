#ifndef PAPACC_SERVER_SERVICE_WIN32_H
#define PAPACC_SERVER_SERVICE_WIN32_H
#include <stdio.h>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include "server_service_data.h"
#include "personal_scq_inconclusive_win32.h"
#include "server_service_startup_win32.h"
/* Native private bridge only. No tokens or curve/provider types in DATA core. */
/* Borrow valid only during synchronous callback, held by SAME SCM owner. */
#include "personal_scm_registration_win32.h"
/* Acquisition from native originals already held by the canonical owner.
 * No input/approval setter; empty owner returns DENY before native effects. */
DWORD papacc_server_launcher_scq_first_controller(void);
DWORD papacc_server_launcher_scq_start_resource_current(void);
DWORD papacc_server_launcher_scq_start_namespace_prepare(void);
DWORD papacc_server_launcher_scq_prestart_close(void);
/* Direct canonical presenter/producer, never an installable callback. */
int papacc_server_launcher_scm_registration_present(const PAPACC_SERVICE_REGISTRATION_VIEW *,FILE *,FILE *);
/* Known-folder DATA resolver for the staged target, not the controller image
 * or proof of publication/authority. No object is opened by this resolver. */
DWORD papacc_server_launcher_scm_registration_target(WCHAR *,size_t);
DWORD papacc_server_launcher_scm_registration_current(const PAPACC_SERVICE_REGISTRATION_VIEW *);
DWORD papacc_server_launcher_scm_registration_take(const PAPACC_SERVICE_REGISTRATION_VIEW *);
DWORD papacc_server_launcher_scm_registration_effect(const PAPACC_SERVICE_REGISTRATION_VIEW *,unsigned,unsigned,DWORD);
DWORD papacc_server_launcher_scm_registration_finish(const PAPACC_SERVICE_REGISTRATION_VIEW *,int);
DWORD papacc_server_launcher_scm_registration_resource_report(const PAPACC_SERVICE_REGISTRATION_VIEW *,PAPACC_SCM_RESOURCE_REPORT *);
/* Same canonical controller source; no grant can be installed by callers. */
DWORD papacc_server_launcher_scq_start_carrier(PS_SCQ_CARRIER *);
DWORD papacc_server_launcher_scq_start_take(const PAPACC_SERVICE_IMAGE_VIEW *);
DWORD papacc_server_launcher_scq_start_progress(HANDLE);
DWORD papacc_server_launcher_scq_start_wait(HANDLE);
DWORD papacc_server_launcher_scq_start_wait_terminal(HANDLE);
DWORD papacc_server_launcher_scq_start_finish(HANDLE);
DWORD papacc_server_launcher_scq_start_namespace_current(void);
/* Exact live SCM-owner borrow; copying metadata cannot create this identity. */
int papacc_server_service_registration_view_current(const PAPACC_SERVICE_REGISTRATION_VIEW *);
typedef int (*PAPACC_SERVICE_PREPARE)(const char *,const char *,const PAPACC_SERVICE_IMAGE_VIEW *,FILE *,FILE *);
int papacc_server_service_cli(int,const char *const *,FILE *,FILE *,PAPACC_SERVICE_PREPARE);
/* Called only by the shared runtime at factual initialization/teardown points.
 * These receipts never authorize a listener or lift operational admission. */
int papacc_server_service_host_active(void);
/* Observed original product callback/readiness only; never SCQ authority. */
int papacc_server_service_scq_host_current(void);
/* Closed preparation is query-only, explicitly selected, never productive ready.
 * Ready obtains evidence directly from the canonical launcher, no proof setter. */
int papacc_server_service_scq_preparation_current(void);
/* Transient SCM arguments are untrusted discovery DATA, not admission. */
int papacc_server_service_scq_carrier(PS_SCQ_CARRIER *);
int papacc_server_service_scq_closed_ready(void);
int papacc_server_service_scq_closed_ready_current(void);

int papacc_server_service_runtime_ready(void);
/* Private qualification source is an owned read-only borrow, not CURRENT. */
int papacc_server_service_qualification_active(void);
const unsigned char *papacc_server_service_qualification_source(size_t *);
struct PAPACC_SERVER_IO_LOOP_WIN32;
int papacc_server_service_qualification_managers_ready(const struct PAPACC_SERVER_IO_LOOP_WIN32 *);
int papacc_server_service_runtime_stopping(void);
void papacc_server_service_runtime_closed(void);
void papacc_server_service_runtime_retained(void);
/* Exact internal entry: service host, no fallback to a console launch. */
int papacc_server_service_dispatch(int,char **,int (*)(int,char **));
size_t papacc_server_service_storage_bytes(void);
#ifdef PAPACC_SINGLE_LAB_ADMISSION
#include "server_service_admission_win32.h"
#endif
#endif
