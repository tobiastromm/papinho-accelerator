#ifndef PAPACC_PRIVATE_CONFIGURATION_BODY04_WIN32_H
#define PAPACC_PRIVATE_CONFIGURATION_BODY04_WIN32_H
#ifndef PAPACC_C8_PRIVATE_STORAGE_PROFILE
#error BODY04_requires_the_private_C8_type_profile
#endif
#include "server_configuration_commit_win32.h"
#include "authentication_authorization.h"
#define PAPACC_CONFIG_BODY04_BYTES 1024U
#define PAPACC_BODY04_PRINCIPAL_NONE 0U
#define PAPACC_BODY04_PRINCIPAL_RESOLVER16 1U
typedef char PAPACC_BODY04_REQUIRES_PRINCIPAL16[(PAPACC_PRINCIPAL_VALUE_SIZE==16U)?1:-1];
/* Representation only: no resolver, configuration manager, grant or epoch.
 * base.config.policy.rules[].principal is a historical0.3-only field and MUST
 * be empty here. It is NOT a padded representation of principal16. No implicit
 * conversion of0.2/0.3 views/records or extraction into old policy evaluation. */
typedef PAPACC_ADMIN_PRINCIPAL16 PAPACC_BODY04_PRINCIPAL;
/* Typed identity travels inside the existing configuration model and root. */
typedef struct PAPACC_CONFIG_BODY04_VIEW {
 PAPACC_CONFIG_VIEW base;
} PAPACC_CONFIG_BODY04_VIEW;
PAPACC_RESULT papacc_configuration_body04_encode(const PAPACC_CONFIG_BODY04_VIEW *,PAPACC_U8 *,PAPACC_SIZE);
PAPACC_RESULT papacc_configuration_body04_decode(const PAPACC_U8 *,PAPACC_SIZE,PAPACC_CONFIG_BODY04_VIEW *);
#endif
