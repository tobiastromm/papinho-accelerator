---
status: proposed
revision: 1
date: 2026-10-28
scope-level: project
supersedes: []
---
# Candidate — conditional fallback when legitimate storage hierarchy auth is unavailable

NOT ACCEPTED. No operational authority or claim that ownerAuth is factually absent. Current observation: policyOSManagedAuthLevel5; supported STORAGE_20 NULL-output query returned TBS internal error, not NOT_FOUND. External custody remains unknown. TC/W/L/cross-boot accepted are preserved.

## OA0 — Type and authority boundary

For the candidate TPM2 owner-hierarchy NV index, investigate legitimate STORAGE_20 authorization type13, not TPM1.2 FULL/ADMIN/USER or TPM2 lockoutAuth. Type13 official Tbsi_Get_OwnerAuth may expose bytes in memory IF a valid source exists and the authorized caller is entitled; current task only made a NULL-output availability query and obtained no value. Existing operator/admin/TBS membership and policy5 do not grant that credential. No direct registry extraction, empty/guessed password, Clear/hierarchy/GPO change or adoption of Windows-owned indices.

## OA1 — Preserve current safe branch

Until a source/holder and controlled delivery are factually established, disable real NV definition/increment and keep Q4readback DENY. Current root remains local ledger, not an independent witness. No repeated possession proofs, status polls or newly permissive local markers can remove this prerequisite. An inconclusive query is UNKNOWN, not source absence; a future NOT_FOUND is limited to that API/token/instant, not proof that an external holder does not exist.

## OA2 — Compatible preferred resolution if legitimate authority exists

Existing designated holder/provider may deliver original storage-hierarchy authorization directly in protected process memory to the same governed worker under a separately approved narrow provisioning ceremony. Record only holder/provenance/availability/delivery and returned status, never value/hash of secret. Do not create a second vault/service or use CA/issuer key as password storage. Original context/RAW1/root/token/leases and trust/generation/resource conditions remain mandatory. This proposal does not designate a holder or authorize retrieval of bytes, index generation, root write or NV effects.

Existing governed ownerPolicy could theoretically provide hierarchy authorization without exporting ownerAuth, but no such local policy/provider has been identified. Creating/changing that policy would be a new hierarchy operation and is outside this proposal. Do not invent empty auth or a privileged Windows broker for arbitrary application NV allocation.

## OA3 — Alternative when source is truly unavailable

W6/L2 already permit investigating an EXISTING independently governed atomic-consumption authority outside the restorable local domain. Candidate alternative is a separately designated external witness recording an exact typed root/approval/device-purpose identity and one-use consumption, with authenticated monotonic continuity, concurrency/crash/reconciliation and retention. Canonical root remains sole local ledger/owner. No new generic IPC/control service/issuer is authorized. If a suitable authority does not exist, its introduction/interface/threat model/custody requires a separate explicit architectural decision and exact implementation/operational qualification; this is a direction candidate, not a deployed service or command.

It must detect rollback of ALL local ledger/ciphertext storage against independently newer external state. Restore of both witness and ledger cannot be claimed detected. Local files/registry/DPAPI/hashchain/Git/eventlog, PCR/key signature alone or unspecified authenticator sign-counter do not satisfy the guarantee by naming them witnesses. No personal M3 fallback or reduction to local-only rollback detection. No arbitrary backend identity is invented.

## OA4 — Decision and consequence

A: identify a legitimate existing storage-authorization provider and qualify its protected delivery; may retain TPM direction but does not prove availability or grant effects.
B: if no provider exists, preserve DENY while investigating/designating an existing independent atomic-consumption witness; absent one, owner must decide whether to introduce a new narrowly governed mechanism. This avoids resetting the current TPM and preserves full required rollback guarantee, but adds real architectural/custody/resource work.
C: do not execute historical cross-boot readback; retain reservations/UNKNOWN. Immediately safe, E21 remains pending.

No branch is automatically selected by this task. Do not call actual source unavailable from generic Windows defaults or the current inconclusive query. No NV/Q4/Start/Stop/SCM/ACL/cleanup/refund/CURRENT/signature/authoritative recovery. ED11/Phase6 PARTIAL; E17-E20 independent.

[Native observations and decision path](../integration/n0start03-integrated-closeout-20261024/tpm-ownerauth-factual-resolution-20261028/CONSOLIDATED-CLOSEOUT.md).
