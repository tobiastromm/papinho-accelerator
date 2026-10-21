# Checkpoint consumers and administrative CLI handoff

No new backup/fencing/backend or administrative authentication mechanism is selected here. Existing accepted technical profile2.4/3.5-3.6 and issuer/C2 P6 require approved latest-state and fencing proofs before recovery/signing. This checkpoint supplies historical linkage only.

## Existing-root administrative call sequence

1. Future authenticated administrative caller uses existing issuer_prepare_live and issuer_publish_live (begin/poll) through same root. Preserve its factual actor/request, original R/P and immutable bytes; no caller epoch.
2. Only after issuer_publish_live completes successfully and confirms actual CURRENT/A, call papacc_server_launcher_configuration_checkpoint_begin(root). This is a distinct acknowledgement of persisted publication provenance, not automatic signing or operational authorization. No new scheduler/callback path was installed.
3. Poll papacc_server_launcher_configuration_checkpoint_poll(root,&result) on existing administrative owner turns. Do not spin in functional owner, block Stop or call begin repeatedly. PENDING retains packet; SUCCESS confirms durable trailer; conflict/unknown/error retains exact disposition, denies dependent effects and never retries or falls back automatically. Begin cannot be replayed after success/failure.
4. Data query papacc_configuration_checkpoint_data07 returns NOT_SUPPORTED with populated link only for validated historical data; INVALID_STATE means no usable output. Never treat NOT_SUPPORTED as recovery PASS. Its output is cleared on failed validation. Root/issuer buffers are not exported secrets; no private key involved. Checkpoint hashing/encoding is serialized administrative work, not an active resource collector or claim of universal native API latency bounds.
5. Read-only restart uses existing recovery_init02 then issuer_recovery06; validated linked data may be inspected, but slots A/P remain absent and recovery returns NOT_SUPPORTED. Do not use it to resume signing/credit/Sessions. Reconstruct actual live context only after a later approved authoritative recovery implementation.

## Future proof source

Select approved protected backup/recovery artifact and independently retained newest checkpoint/history, reconcile serial/reservations/uncertain signatures/external certificates, and prevent EVERY earlier same-generation installation from signing. Local file identity/hash/CAS/mutex/PID/time is insufficient. Bind source/root/CA generation/key/history and exact commit correlation; missing/uncertain proof means DENY. No service/network/distributed authority or new CA generation is introduced automatically.

Future CNG integration must retain exact diagnostic key disposition, approved operator/token/protected KSP/key correspondence/backup gates, durable reservation-before-sign and uncertainty reconciliation. It cannot set current/fencing proof flags just because this trailer decodes. Active Session revocation policy is still separate POLICY_PENDING.

Normal 0.6.5 candidate remains unpromoted; production pin0.6.4, Browser/PST/wire and all denied admissions unchanged. New CLI/user interface requires its own authorization/audit/lifetime validation; no real issuance or network is authorized by this handoff.

The three serialized revision fields retain distinct meanings: R (CURRENT), I (issuer content), snapshot revision. Current ROOT06 implementation binds their numeric revisions by its existing per-event transaction mapping; format1 rejects unsupported mappings rather than inferring authority from equal numbers. Future independent model/issuer revision mapping needs explicit codec/adapter review, never aliasing IDs.
