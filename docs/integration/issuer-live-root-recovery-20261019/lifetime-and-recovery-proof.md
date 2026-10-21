# Issuer generation lifetime and remaining authority gates

Task PACC-PHASE6-ISSUER-LIVE-ROOT-AND-RECOVERY-1. Implementation evidence, not an accepted normative amendment.

## Implemented safe subset

The same PAPACC_CONFIGURATION_COMMIT owns three fixed issuer image copies, one for each existing configuration slot. No second owner, registry, journal, thread, listener or heap is created. prepare06 selects a free non-current slot; reader counts and retained flags prevent reuse. Validation, expected revision/high-water and native whole-image compare-and-commit are unchanged. The native committed-pending image is copied into its reserved slot only after CommitTransaction succeeds. An uncertain transaction or failed close never enables snapshot access; unresolved denies the new reader.

issuer_snapshot06 requires owner thread, initialized existing root, exact issuer pointer/index/serial/ticket and a nonzero reader count. It performs a synchronous memory copy under the existing lock, without resource query, hash, callback, wait loop or allocation. Caller owns the destination. No reference to mutable latest committed bytes escapes. Existing pin/unpin enforce exactly-once ticket release. Stop denies new pins and effects; an existing pin can still copy immutable data, preventing teardown until released. A retained slot cannot be reused or finished. Capacity exhaustion returns LIMIT_EXCEEDED without overwriting old data, waiting for readers or increasing quotas.

The application wrapper explicitly retains its prior no-reader/no-retained-slot restriction and root_dormant_owned validation. Production ROOT06 publication remains NOT_SUPPORTED. The component-only selection macro is still confined to the isolated test target. Neither retained issuer bytes nor component selection creates CURRENT or an epoch.

## Concrete live lifetime barrier — STOP before dependent activation

apps/server/server_configuration_root_transaction_win32.inc root_snapshot_live accepts only p.pin.index == c.current and p.epoch == t.current_policy_epoch. apps/server/server_listener_composition_win32.inc root_listener_check_mode requires that pin to be CURRENT and agree with the C2 configuration revision. apps/server/server_c2_root_binding_win32.inc papacc_server_launcher_c2_session_current compares each Session's admitted policy_epoch/root_revision to the one listener pin. apps/server/server_c8_policy_adapter_win32.c allowed compares the effective epoch pointer with the admitted epoch on each authorization.

Consequently a naive revision/epoch replacement invalidates the listener cohort and existing Sessions. Overwriting those Session fields would silently reinterpret their original authority. Retaining the old fields alone is also insufficient because the current predicate rejects them. The new byte snapshots solve data lifetime, NOT this authorization continuity. The task's STOP for lifetime not proven applies to changing this dependent publication path. No guard is removed and no Session/grant/Principal/Connection/callback is modified.

The next implementation must preserve an explicitly protected policy cohort for admitted Sessions while a separately identified issuer-only metadata revision advances in the same root; prove unchanged policy and grant membership rather than ignoring revision mismatch. The root authority alone must issue the post-commit context epoch. Exact relationship between issuer metadata revision and the existing operational policy-cohort predicates must be demonstrated against C2/C8 contracts before changing them. This is not permission to introduce a second epoch authority or to change policy by issuance. Active-Session revocation remains POLICY_PENDING in the accepted issuer decision.

## Authoritative recovery barrier

Accepted technical profile sections2.4 and3.5–3.6 require latest high-water proof, old-instance fencing and separately retained administrative recovery checkpoint, with protected backup/restore evidence. No implemented approved checkpoint/fencing producer was located in the issuer/root sources. ROOT06 stores a hash chain and an immutable base-file identity: a valid older whole-file image can still satisfy integrity. A local hash, clock, revision literal, exclusive read lease or successful reopen cannot prove latest history or fence a restored clone.

Current recovery therefore stays observational/NOT_SUPPORTED and exports no positive proof bits or usable epoch. It restores visible high-water and records without granting sign/publication authority. No fabricated checkpoint, raw proof-bit toggle, guessed current epoch or shadow journal is added. Binding a concrete approved recovery source remains required before authoritative restart can be enabled. CNG remains excluded and unresolved; no certificate/key generation or signing was attempted.

## Offline evidence

Fresh Debug/Release directories were configured with normal governed x64 PST0.6.5, /W4 /WX, no R2 or toolchain substitution. Initial build failed on an incorrect test enum (NOT_FOUND instead of the existing ABSENT); logs are retained. Final build3 passes including the canonical server, zero compiler warnings.

Per configuration: immutable-generation component117 assertions; native-file historical state/revocation history153; normal-library publication denial84; Stop before commit86; portable issuer synthetic117; production test-adapter denial4; historical catalog31; application root69. Counts are not claims of equivalent distinct oracle coverage. Actual new files use native TxF commit/reopen and SHA256. Request identities/epochs/DER marker and component state selection are synthetic. No OS crash/power-loss test, real Session traffic, network, provider key operation or real certificate was run. Core error/uncertainty tests are simulated, not injected native API failures.

Application root test confirms the existing pin still prevents dormant prepare, then same-owner metadata commit succeeds after unpin, original epoch stays unchanged, ROOT06 publication/preflight deny, and root remains retained. Its fixture reads existing test TLS material but does not bind/listen/handshake/issue.

## ED11 compiled reconciliation

Pre-task native owner29432 ->54024 bytes: +24592 (3*8192 snapshots +3*4 valid flags +4 padding). The pre-persistence historical comparison helper reports13024 ->54024, cumulative41000; do not confuse this cumulative delta with this task's24592 delta. Application root47464 ->72056. Known managed1555728 ->1580320, limit2097152, known-static margin516832. Actual fixture combined reservation1813435 ->1838027, remaining259125. Embedded snapshots are charged once through the existing owner; do not separately add them again. Copies are bounded and no new stack image is introduced by snapshot06. Existing commit two16KiB stack buffers and codec/caller/backend stack remain separately unqualified. Provider/kernel UNKNOWN; total ED11 PARTIAL. No quota or operational headroom/freshness threshold changed.