# Canonical Roughtime source binding — proposed extension

Task PACC-PHASE6-ROUGHTIME-CANONICAL-SOURCE-BINDING-1,2026-10-21. Existing accepted issuer/PDG/root purpose/layouts cannot represent TIME_SOURCE without a new contract. Accordingly no functional patch was applied. [TS0-TS8 PROPOSED](TIME-SOURCE-BINDING-PROPOSED.md) defines the focal administrative purpose and versioned same-root representation; [findings and handoff](conformance-and-handoff.md) distinguish representation from factual authority. Exact input/output hashes are in [manifest](manifest.json).

```ini
CURRENT_TASK_COMPLETED=NO
ROUGHTIME_SOURCE_BINDING=PROPOSED_NORMATIVE_EXTENSION_ONLY
CANONICAL_TIME_SOURCE=NOT_IMPLEMENTED
TIME_SOURCE_STATUS=DENY_UNTIL_FACTUAL_SOURCE_QUALIFIED
TIME_SOURCE_KEY_AUTHORITY=DENY
UTC_SOURCE=NOT_QUALIFIED
BUILDS=NOT_RUN_NO_FUNCTIONAL_PATCH
WARNINGS=NOT_MEASURED
OFFLINE_TESTS=NOT_RUN_NO_FUNCTIONAL_PATCH
NETWORK_TESTS=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ISSUER_STATUS_BINDING=DENY
UPDATE_U0_U7_BINDING=DENY
ED11_TOTAL_STATUS=PARTIAL
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=TIME_SOURCE_PURPOSE_AND_PERSISTENCE_CONTRACT_NOT_ACCEPTED
NEXT=ROUGHTIME_NATIVE_TRANSPORT_AFTER_CANONICAL_SOURCE_BINDING
```

Immediate dependency before that NEXT: normative review of TS0-TS8, followed by authorized canonical binding implementation and factual source/lifetime/ED11 qualification. Documentation preparation is complete; requested functional binding is not complete. No accepted clause was changed, and this proposal was not registered ACCEPTED. No native transport, network, loopback, key, certificate, fixture generation, CURRENT, clock change, quota, clone/worktree, commit/push/tag/release or production effect occurred. Both projects' WIP/HEAD and four LAB remain preserved.
