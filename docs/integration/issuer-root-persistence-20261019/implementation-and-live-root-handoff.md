# CNG and live-root handoff

Read closeout.md and codec-and-lifetime-review.md first. Keep production ROOT06 publication denied and root-port proof bits absent. Do not enable the component-test selector in any product target.

1. Demonstrate generation lifetime and borrower/lease disposition for a CURRENT root update; existing C2 association is immutable once CURRENT. Reuse the same application owner and transaction. Preserve existing sessions and retained dependencies.
2. Complete PREPARE -> VALIDATE -> COMMIT -> CURRENT -> factual EPOCH acknowledgement. Only then may CAS acknowledge serial readiness to the signer or PUBLISHED. Do not infer authority from committed bytes alone.
3. Establish authoritative restart selection, stale-writer fencing and rollback/high-water consistency with the same root. Current recovery is observational only. Preserve uncertain reservations and signing outcomes.
4. Bind the accepted authority/SPKI/prefix identity to the approved platform signer; no unqualified fixture metadata may become operational identity. CNG native state remains unresolved; this task executed no key operation.
5. Repeat offline stale-writer, crash/recovery, retained-generation and active-lease tests for the actual publication composition. Reconcile complete stack/coexistence/backend ED11 before admitting operations. No network or CA initialization is authorized by this handoff.

Files: src/issuer/issuer_codec.*, apps/server/server_configuration_issuer06_win32.inc, existing configuration commit/root transaction/launcher headers, tests/issuer_root_store_win32_test.c and tests/canonical_root_transaction_test.c. Build recipes and exact preimages accompany this handoff.