# Strict Ed25519 TIME_SOURCE predicate acceptance

The exact P0-P5 specialization defines canonical decoding and nonidentity
prime-order-L membership. It closes the missing predicate only. This record
is effective after official executor approval and exact postimage verification.

[Accepted decision](../../adr/DECISION-time-source-strict-ed25519-predicate-20261021.md),
[conformance/vector matrix](conformance-and-vectors.md), [input hashes](input-hashes.json).
Original backend proposal and TS/AT/PT accepted bytes are preserved.
No backend, source, functional patch or executable fixture accepted by reference.

CURRENT_TASK_COMPLETED=YES
STRICT_ED25519_PREDICATE=ACCEPTED
BACKEND_REFERENCE=LIBSODIUM_1_0_21_OR_LATER_CORRECTED_CANDIDATE
BACKEND_SELECTED=NONE
BACKEND_IMPLEMENTATION_AUTHORIZED=NO
TIME_SOURCE_KEY_VALIDATION=DENY
UTC_SOURCE=NOT_QUALIFIED
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
BUILDS=NOT_RUN
TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES_EXACT_PREEXISTING_FILE_HASH_CHECKS
FIRST_UNRESOLVED_DIVERGENCE=PINNED_CORRECTED_BACKEND_AND_TARGET_ED11_NOT_QUALIFIED
NEXT=PINNED_ED25519_BACKEND_QUALIFICATION

Only documentary additions and a qualified-time live-document cross-reference
are authorized. No quotas, clocks, functional sources, LAB, Browser/PST/wire,
clones/worktrees, commit/push/tag/release or production effects. No additional
runtime memory is allocated by this normative decision. Previous analytical
ED11 scenarios are not operational measurements or reservations; all unknown
backend, initialization, stack, transitive and retention costs remain gates.
