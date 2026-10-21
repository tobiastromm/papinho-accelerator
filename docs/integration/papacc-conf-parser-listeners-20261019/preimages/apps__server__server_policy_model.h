#ifndef PAPACC_SERVER_POLICY_MODEL_H
#define PAPACC_SERVER_POLICY_MODEL_H
/* Private opt-in administrative model, not PACC wire or an auth provider. */
#include <papacc/pacc/capability_wire.h>
#define PAPACC_ADMIN_POLICY_KEYS 3U
#define PAPACC_ADMIN_POLICY_RULES 4U
#define PAPACC_ADMIN_POLICY_PRINCIPAL 1U
#define PAPACC_ADMIN_POLICY_DENY 0U
#define PAPACC_ADMIN_POLICY_ACCEPT 1U
#define PAPACC_ADMIN_OPEN 0U
#define PAPACC_ADMIN_CLOSED 1U
/* Private representation discriminator; historical0.3 remains zero. */
#define PAPACC_ADMIN_REPRESENTATION_04 4U
typedef struct PAPACC_ADMIN_PRINCIPAL16 {
 PAPACC_U8 type,length,value[16];
} PAPACC_ADMIN_PRINCIPAL16;
typedef struct PAPACC_ADMIN_POLICY_RULE {
 PAPACC_CAPABILITY_KEY key;
 PAPACC_U32 action, conditions;
 PAPACC_U8 principal[32];
 PAPACC_ADMIN_PRINCIPAL16 principal16;
} PAPACC_ADMIN_POLICY_RULE;
typedef struct PAPACC_ADMIN_POLICY {
 PAPACC_BOOL present;
 PAPACC_U32 representation;
 PAPACC_U8 identity[32];
 PAPACC_U32 profile, admission;
 PAPACC_U16 listener_port;
 PAPACC_SIZE enabled_count, rule_count;
 PAPACC_CAPABILITY_KEY enabled[PAPACC_ADMIN_POLICY_KEYS];
 PAPACC_ADMIN_POLICY_RULE rules[PAPACC_ADMIN_POLICY_RULES];
} PAPACC_ADMIN_POLICY;
#define PAPACC_ADMIN_POLICY_INITIALIZER { 0 }
typedef struct PAPACC_ADMIN_POLICY_CONTEXT {
 PAPACC_U32 profile, admission;
 PAPACC_U16 listener_port;
 PAPACC_BOOL principal_valid;
 PAPACC_U8 principal[32];
 PAPACC_U32 representation;
 PAPACC_ADMIN_PRINCIPAL16 principal16;
} PAPACC_ADMIN_POLICY_CONTEXT;
#endif
