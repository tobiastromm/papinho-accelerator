#ifndef PAPACC_EXACT_POLICY_H
#define PAPACC_EXACT_POLICY_H

#include "destination.h"

#define PAPACC_EXACT_EGRESS_RULE_CAPACITY 16U

typedef struct PAPACC_EXACT_HOST_RULE {
    char hostname[PAPACC_DESTINATION_HOSTNAME_CAPACITY + 1U];
    PAPACC_SIZE hostname_size;
    PAPACC_U16 port;
} PAPACC_EXACT_HOST_RULE;

typedef struct PAPACC_EXACT_ENDPOINT_RULE {
    PAPACC_NETWORK_ENDPOINT endpoint;
} PAPACC_EXACT_ENDPOINT_RULE;

typedef struct PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT {
    PAPACC_EXACT_HOST_RULE host_rules[PAPACC_EXACT_EGRESS_RULE_CAPACITY];
    PAPACC_SIZE host_rule_count;
    PAPACC_EXACT_ENDPOINT_RULE endpoint_rules[PAPACC_EXACT_EGRESS_RULE_CAPACITY];
    PAPACC_SIZE endpoint_rule_count;
    PAPACC_BOOL valid;
} PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT;

#define PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT_INITIALIZER { {{0}},0U,{{0}},0U,PAPACC_FALSE }

PAPACC_RESULT papacc_exact_egress_policy_init(
    PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT *snapshot);
PAPACC_RESULT papacc_exact_egress_policy_add_host(
    PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT *snapshot,
    const char *hostname, PAPACC_SIZE hostname_size, PAPACC_U16 port);
PAPACC_RESULT papacc_exact_egress_policy_add_endpoint(
    PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT *snapshot,
    const PAPACC_NETWORK_ENDPOINT *endpoint);
PAPACC_RESULT papacc_exact_egress_policy_publish(
    PAPACC_EXACT_EGRESS_POLICY_SNAPSHOT *snapshot,
    PAPACC_EGRESS_POLICY *runtime_policy);

#endif
