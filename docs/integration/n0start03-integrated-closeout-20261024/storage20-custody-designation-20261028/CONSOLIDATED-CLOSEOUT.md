# STORAGE_20 — separate designations and controlled-memory delivery qualification

TASK=PACC-PHASE6-ED11-TPM-STORAGE20-CUSTODY-DESIGNATION;2026-10-28. Canonical main/HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preserved. Preparation and isolated implementation completed; NO actual credential read or operational authority.

## Two exact independent proposed decisions

CUSTODIAN-DESIGNATION.PROPOSED.json proposes SID S-1-5-21-785699550-1440458426-1682014700-1108 as Q4-purpose custodian only, with live token/process/primary SID/elevation/no-impersonation/lease checks and TC/L1 same-user/admin boundary. WINDOWS-STORAGE20-SOURCE-DESIGNATION.PROPOSED.json separately proposes official Windows Tbsi_Get_OwnerAuth/type13 STORAGE_20 as the controlled-memory source, not TPM1.2 FULL/ADMIN, lockoutAuth or an empty-password fallback.

Both bind the same Q4 purpose, observed EK SPKI1c643066b17bdf9e81c09870661aeb7c9cfff1f2ffbef71b43532c14a25c474e/certificate965f56aa0d4c5b5772b423d65d8a06a623109f5828c0eddcb8ec4ae8fa3de761 references and accepted TC framework. Original NV-generation/root-domain operational facts are NOT fabricated; their bindings are null pending actual scope. Each owner_decision is null and credential_access_authorized/NV_Q4_authorized false. No designation recorded as ACCEPTED and no approval created in owner's name.

Historical STORAGE20 receipt is preserved as owner-reported metadata source-present only. Displayed hash check after execution, unknown preceding variable value, and missing operator/PID/creation attestation remain explicit. Current matching artifact hash does not reconstruct historical original-object identity. No metadata/proof rerun requested or performed.

## Implementation actually completed

New delivery.c/.h implement isolated callback-based memory delivery using the existing TP_OWNER interface. ST20_DELIVERY is a borrowed scratch/lifetime descriptor, not a new authority/owner/service/vault/registry. No product root owner instance, serialized format or operational source resolver was changed. The module does not open/close TBS, load/resolve Tbsi_Get_OwnerAuth or send TPM commands. Its function-pointer ABI matches the supported Windows source; tests bind ONLY get_mock, never the real API.

Caller provides immutable descriptor/API table and a disjoint owned scratch slot. Windows VirtualLock locks it before any source call; one typed STORAGE_20 value call writes directly into that slot. No intermediate plaintext copy, argv/ENV/file/log/report/secret hash or output-value API. Capacity profile is bounded at64bytes for this qualification, NOT an asserted universal TPM limit or operational capacity grant; zero/oversize lengths and insufficient-buffer/API failure deny without resize/retry. A future exact access plan must justify the buffer bound against live metadata; no allocation increase by inference.

Guards run before lock/get/use and after synchronous use. Future guards must resolve original context/Windows dependency/operator/designation/source/TPM-reference/purpose/root/lease/resource facts; this isolated prototype does not install a producer or turn booleans into authority. API_SUCCESS means bytes returned, not validity for NV. The consumer receives a const synchronous borrow and returns known complete/rejected/UNKNOWN; no callback may retain/copy it outside this contract. Same-user/admin hostile code is not prevented by a C callback or VirtualLock and remains within the explicit TC trust boundary.

Known API failure, absent/invalid credential, reject/Stop or known completed consumer => SecureZeroMemory of entire controlled capacity before checked VirtualUnlock. Sync source failure may have partially written, so whole slot is wiped. Wiping affects only controlled memory, not unknown Windows/TBS/CPU/other-process copies. VirtualLock is not secrecy from administrators or a new at-rest custody system. Hierarchy secret is not persisted; per-index secret/backing remains a distinct future L1 provisioning concern.

Owner-thread affinity and existing BUSY cover the entire borrow. Reentry freezes before another source acquisition. Foreign threads are rejected without state mutation. Once descriptor is used or owner frozen/retained, no new query. First failure/native status is preserved. Context replacement records the original borrowed context and freezes/retains dependencies even if the guard simultaneously rejects.

UNKNOWN consumer borrow preserves original buffer/length/lock/context dependency and owner BUSY/freeze/retained; no wiping or unlock while use is uncertain. Failed unlock preserves its original descriptor/lock and dependencies with BUSY retained; known completed plaintext was already wiped. Existing tp_finish refuses cleanup while these states persist. No timeout, second-close/retry, background cleanup or new context. Caller must retain the original descriptor/buffer/owner lifetime until factual resolution; process loss remains UNKNOWN, never proof of closure.

## Qualification and corrections

Debug/Release MSVC x64 /std:c11 /W4 /WX /GS /sdl PASS, ZERO warnings.75checks per configuration,150executions of same set, not150independent properties. Logs TEST-RESULTS.json and per-config build/test.log.

Source API and consumer outcomes are simulated with public fixed fixture patterns, never real ownerAuth. VirtualLock/VirtualUnlock/SecureZeroMemory/VirtualAlloc and two concurrent Windows threads are real. Cases include source NOT_FOUND, partial-write/API error, zero credential, oversize length, wrong context before/after query, divergent custodian/source guard, invalid fixture credential rejected by consumer, guard rejection with simultaneous context swap, Stop before query/after borrow, reentry, overlapping buffer/owner/descriptor/API table, repeat use, two foreign threads, uncertain borrower and uncertain unlock. All source mock calls enforce type13/original context; no legacy/auth-empty fallback. Model invalid-credential test is NOT a native proof of correct/incorrect TPM authorization.

Final review fixed context-swap handling even when guard rejects and preserved BUSY for retained UNKNOWN states. Added affected cases and recompiled only these fixtures. Prior passing intermediate logs retained, no warnings disabled. Existing1929checks native adapter and494checks root DATA policy scopes remain valid, sources/binaries unchanged; no redundant full campaigns or product builds.

Layout compiled: descriptor56bytes, API table40bytes, maximum caller secret slot64bytes. These do not represent total memory/stack/locked-page cost. Fixture requests4096-byte virtual regions and intentionally retains two original UNKNOWN pages/owners until process loss; actual allocation granularity/transitive costs are not a product envelope or credit. No product arrays/quotas/owner structs changed and no budget transferred. Native TBS/hierarchy resource peak is NOT observed.

## Current factual custody

Availability remains observed metadata, not credential possession/validity. Real source holder/interface is proposed Windows management; effective custodian and source designations still pending. No hierarchy/index secret generated, retrieved, installed or revealed. Endpoint/device generation/trust/revocation gaps stay unresolved. Strict revocation UNKNOWN continues DENY for witness/NV/Q4. A possible separately authorized memory-only diagnostic would not itself constitute witness admission or prove an original unprovisioned NV generation; do not fabricate index facts as a precondition or conclusion of memory delivery.

Same-root CurrentUser DPAPI L1 design for a future index credential remains unchanged; it does not create actual ciphertext custody by naming the API. No new plaintext vault or reuse of issuer/CA key. Credential validity for TPM commands is a separate factual property; this module deliberately provides no NV sender and no positive authority field.

## Next exact decision and first real proof

Owner must decide the two proposed designations independently under their exact SHA256s; TC framework itself is already accepted and need not be reapproved. Designations do NOT permit reading the credential. After valid decisions and actual root/operator/context/artifact/lifetime bindings, prepare an exact request for ONE controlled-memory access only, with permitted synchronous consumer behavior, no secret output/persistence, capacity/resources/Stop/UNKNOWN and manual handling. Require specific authority for actual value access; never include NV provisioning in that approval.

CONTROLLED-ACCESS-READINESS.NOT-EXECUTABLE.json keeps request/command null now. First real proof still missing is a specifically authorized controlled delivery from the legitimate bound source to the same governed owner under valid custody, including factual cleanup/retention; it is not another NULL-output query or EK possession proof. Only after further trust/generation/custody/sender/resource prerequisites could a separate NV experiment be requested. NV1definition/2increments candidate limits confer no permission here.

## ED11 and preservation

E21 updated only with isolated delivery/designation readiness, still PENDING. E17 readiness/identity/access and E18 governed Stop/lifecycle remain operational facts; E19 D4/D5 quantitative policy and E20 target root/CURRENT/epoch/SiteTLS/backend/destination/composition remain independent. Source presence/delivery cannot invent provider/kernel costs or close these gates. E22 derived pending, ED11/Phase6 PARTIAL; UNKNOWN/reservations retained.

No ownerAuth real access, TBS/TPM/NV/Q4/Start/Stop/SCM/ACL/current/signature/recovery; no cleanup/refund, secret logs, commit/clone/worktree. Product sources/binaries/pins/HEAD/WIP and consumed approvals verified unchanged. New package contains only independent proposed designation records, isolated mechanism/fixtures and evidence. SHA256SUMS.json records final hashes; operational approval remains absent.
