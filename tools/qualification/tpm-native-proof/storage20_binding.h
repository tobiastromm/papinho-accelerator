#ifndef ST20_ROOT_BINDING_H
#define ST20_ROOT_BINDING_H
#include "storage20_delivery.h"
#include "server_configuration_commit_win32.h"
/* Immutable borrowed binding; no secret or authority producer. Actual owner
 * boundary MUST independently authenticate all runtime/native/approval facts. */
typedef struct ST20_ROOT_BINDING {
 PAPACC_CONFIGURATION_COMMIT *root_owner;const PAPACC_CONFIG_PIN *pin;
 PAPACC_Q4_ST20_DESIGNATION_DATA expected;
 ST20_GET designated_source;
 char current_sid[80];
 unsigned char current_ek[32],current_certificate[32],request_digest[32],approval_digest[32];
} ST20_ROOT_BINDING;
/* Metadata/export resolution only; caller retains original module and must
 * authenticate original file/provenance in its owner boundary. No API call. */
ST20_GET st20_windows_source(HMODULE);
int st20_root_guard(TP_OWNER *,const ST20_DELIVERY *,const char *);
#endif
