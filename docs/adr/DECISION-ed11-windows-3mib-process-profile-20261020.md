---
status: accepted
revision: 1
decision-date: 2026-10-20
scope-level: project
scope-target: PapinhoAccelerator explicitly selected Windows managed-memory profile
decision-makers: [Tobias Tromm]
supersedes: []
---
# ED11 Windows Accelerator process profile — 3 MiB

Owner TASK PACC-PHASE6-ED11-WINDOWS-3MIB-PROFILE-1 explicitly approves the exact3145728-byte managed budget for the Windows Accelerator PROCESS, conditional on normative review and official executor approval. This record becomes effective only after exact official executor write approval and hash verification, before dependent source changes. No historical ADR, frozen specification, wire vector or proposal is edited.

## W1 Delimited numeric precedence

For the explicitly selected Windows Accelerator managed-memory profile ONLY, replace the2097152-byte process cap by3145728 bytes. This specializes the managed-cap sentence of ADR-0034 ED11 and PACC C2 specification revision1.0 section11.3, and only corresponding2097152 numeric process-budget constraints in local root/C8/config/issuer decisions (including D3, LST1, local bootstrap, Site TLS, server name and T0–T6 persistence). Original artifacts/hash freezes remain historical; this subsequent designated decision has numeric precedence only within the selected profile. It is not wholesale supersession or alteration of those contracts.

Default/legacy/nonselected Accelerator builds and other targets retain2097152; box/ISA/other platforms receive no increased allowance. Browser is untouched. Windows presence alone does not select3MiB. No wire/API/codec field, policy epoch, CURRENT, grants or authority identifier changes.

## W2 One shared process budget

3145728 is a controlled managed-memory ceiling of ONE PapinhoAccelerator process, not per client/Principal/Session/Connection/root/participant/operation. Existing shared-credit reservation, checked arithmetic, identity/revision ownership, commit/rollback and retained credits remain mandatory. Passing this cap does not authorize consumption, imply physical host capacity, create extra pools or reset used credits.

No individual quota increases: preserve512KiB operation-controlled allocation candidate/implemented bounds,16KiB Session reservation, two global operations/one per Session, four64KiB queues and their48/24KiB watermarks,512KiB aggregate payload,16KiB flow service and8 opportunities/wake, individual connection user/kernel/material quotas and all separately governed limits. Process-limit aliases must not replace those quantities.

## W3 Explicit bounded selection

Implement one private constant-only profile selection. Default is legacy2097152. A distinct build opt-in for the canonical Windows Accelerator candidate selects3145728; unsupported platform, malformed selector or missing required candidate/root composition rejects selection, never silently falls back. No text configuration, environment variable, runtime grant or extra registry selects the profile. Production pin0.6.4 and default build behavior remain unchanged; PST0.6.5 remains candidate until Phase6 completion. No box/ISA/NT4 qualification follows from constants/compilation.

All Windows Accelerator process-budget guards must use the same selected ceiling. Portable core/domain limits and individually sized pools/quotas remain unchanged. No Win32/CNG types are added to portable contracts. This is compatible with PapinhoEngineering ADR-0007 local deployment specialization and portable-contract/backend boundaries; no superior governance numeric ED11 ceiling was found in the locally available OrganizationEngineering/PapinhoEngineering accepted ADRs.

## W4 Accounting and qualification

Preserve every fixed/transient/caller/heap/stack/snapshot/retained/native charge and prove reuse before omitting copies. Current known conservative composition2067655 and hypothetical future ROOT08 issuer composition2142951 are scenarios, not measured process peaks or allocations. Under3MiB they leave1078073 and1002777 arithmetic bytes respectively, not automatic reservations. The isolated scratch optimization and all historical sizes remain valid; no capacity allocation or ROOT08 attachment is authorized by those sums alone.

CNG/core/arena/key metadata, CSR/CA/DER/TBS inputs/outputs, callbacks, full transitive stack and uncertain retained results require their real multiplicities and target evidence. Full activation maximum remains UNKNOWN until bounded/measured; provider/kernel UNKNOWN is not zero. ED11 total remains PARTIAL. Existing user/kernel incremental envelopes, host credit, freshness/headroom and independent target gates are unchanged.

## W5 Security/lifecycle invariants

One administrative root/worker/journal, same I/R/A/P/CAS and immutable snapshots/pins. Stop never frees retained resources or means native cancellation. Exactly-once/refund and Model E/ED08 retirement require factual quiescence/closure. No destructive eviction, CAS conflict/UNKNOWN fallback, retroactive grant, implicit admission, signature, recovery/fencing or productive bootstrap/CURRENT is permitted. Active Session revocation policy remains separately pending.

## W6 Implementation and limits

After this exact normative record is officially approved, scoped source patches still need their own exact executor approval, preimages, Debug/Release and offline/native regressions. Acceptance is not implementation/operational qualification. No Browser/PST/wire change, CA/key/certificate/private signing, network, real CURRENT, grant/admission, commit/push/tag/release or Phase6 completion is authorized.

CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
