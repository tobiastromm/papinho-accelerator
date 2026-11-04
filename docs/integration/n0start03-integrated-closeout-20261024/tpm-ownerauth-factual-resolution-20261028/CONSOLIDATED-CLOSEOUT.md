# ownerAuth factual resolution — evidence and exact remaining limit

TASK=PACC-PHASE6-ED11-TPM-OWNERAUTH-FACTUAL-RESOLUTION;2026-10-28.

## New factual result, not another DENY model campaign

Read exact nonsecret Windows policy OSManagedAuthLevel:5; OS10.0.19045.0/x64. Win32_Tpm class schema query and ComputerSystem domain-membership query returned ACCESS_DENIED in the current tool token. No method enumeration/state absence/AD-escrow conclusion inferred. No Get-Tpm broad object, ownerAuth property, credential registry key/value, password buffer, private material or secret hash was read. Exact metadata output preserved in LOCAL-NONSECRET-METADATA.json.

Compiled one minimal native C Release collector /W4 /WX /GS /sdl, zero warnings. It only loads System32 tbs.dll, opens context and invokes the supported Tbsi_Get_OwnerAuth STORAGE_20/type13 with NULL output buffer, length initialized0. No secret buffer exists, no bytes requested/returned to program. NULL describes output sizing metadata, NOT empty authorization/password submitted to TPM. No Tbsip_Submit_Command import/call, NV/auth command, ownership/reset/clear/hierarchy operation or Get-Tpm call exists in this source.

Sandbox execution: context creation0x80284001, query NOT attempted. A separately reviewed read-only execution outside sandbox tested the different context rather than retrying any consumed TPM operation: context creation0, availability query0x80284001, context close0, FreeLibrary returned. Collector exit0 means collection/closure completed, NOT query success. Exact native results/raw outputs retained separately; historical proof authorization was not reused. No automatic UAC elevation or permissions bypass. The owner-elevated token was NOT observed.

Fact: local STORAGE_20 source availability remains UNKNOWN due to INTERNAL_ERROR. It did NOT return0x80284015 OWNERAUTH_NOT_FOUND (official token TBS_E_OWNERAUTH_NOT_FOUND); cannot conclude absent, empty, discarded or unavailable everywhere. Nor can we assert source present/accessible. External custody/escrow not identified, not proved nonexistent. Current policy5 is consistent with Windows lockout-only retention defaults but is not credential inventory or proof of what happened at provisioning. No repeated status polling/secret-retrieval attempt follows.

Own handles48->56 in sandbox and57->65 outside before/after module load. Preobserver124/own128 checked before context path. These are limited samples, not full-process peak, administrative admission or proof of NV resource capacity. Controlled context close returned0 and library release returned; no retention observed on successful collector path. Internal Windows/TBS resource/TPM activity UNKNOWN; explicit direct Submit count0 is not a claim that Windows internally performed zero commands. The collector retains context/module indefinitely on uncertain close; that branch was not triggered or destructively injected.

## Correct distinction between TPM2 authorities

Microsoft official TPM2 policy table separates StorageOwnerAuth from LockoutAuth and documents OSManagedAuthLevel5 behavior. TPM1.2 FULL/ADMIN/delegation terminology is not a substitute for TPM2 storage hierarchy. Official API type13 STORAGE_20 is the relevant candidate source, not type1 FULL by inference, type2 legacyADMIN or lockoutAuth. Existing reports that summarize generic 'owner password discarded/full' must be read within this type distinction; no accepted clause was rewritten.

Supported API may deliver authorization to a caller buffer in memory IF the source/permissions actually exist. That is a legitimate potential interface, not proof of entitlement or a deployed custodian. Current task did not retrieve the value or call the API with a value buffer. Windows/service-managed authorization can remain unavailable to the operator; TBS command access/admin/operatorSID/TPM readiness do not transfer its secret or hierarchy rights. No supported no-secret availability method was identified in Win32_Tpm's documented method set; IsOwned only reports ownership, not authority available to caller.

Older documentation discusses registry/AD backup under applicable policies, but current endpoint/domain/escrow facts do not establish such a source here. No AD query, password file search, registry secret probing or fallback was performed. Whether the human owner has another legitimate governed holder/provider remains a nonsecret custody fact to supply, not a password to paste.

## What can run without exporting ownerAuth

Existing mathematical salted EK proof already passed without hierarchy auth; do not repeat it. Native public metadata, GetCapability/ReadPublic/NV_ReadPublic are information, not index ownership/counter access. The current candidate AUTHREAD/AUTHWRITE counter needs its index credential once defined; definition under TPM_RH_OWNER needs legitimate storage hierarchy authorization or a valid already-governed policy. Neither a Windows key-provider private-key operation nor TBS submission automatically authorizes arbitrary app NV. No documented generic Windows broker granting this application's NV definition without its legitimate hierarchy authority was identified.

A legitimate provider could keep authorization inside controlled process memory without export/logging and authorize a separately approved ceremony, but its identity/source/availability/entitlement must first be proved. Existing ownerPolicy is another conceptual authorization route; none found/queried as available. Installing/changing it or using platformAuth would be new hierarchy authority/operations, not current-scope work. No empty password attempt, ownerAuth guess, Clear, reset or GPO change is legitimate remediation here.

## Specific designation readiness

TC framework ACCEPTED, do not ask for it again. Local Intel root candidate2e1b3ba79af56d758be51697621bc4b9e8cee0983db3e749c55eb9b37c6d2ae0 still needs exact purpose designation; revocation remains UNKNOWN/strict DENY. No diagnostic exception activated. A template decision can designate that exact local purpose without installing OS trust; it cannot assert revocation PASS or authorize NV.

SID1108 remains a custody candidate, not a proven holder. L1 supports same-root protected CurrentUser DPAPI backing with native RAW1/IDs/entropy bindings, but real backing/secret/restart recovery do not exist. Same-user/admin trusted boundary is explicit; DPAPI does not authenticate a device or stop privileged same-user credential use. Effective custodian designation alone cannot establish storage hierarchy custody. No secret was generated or installed.

Generation: existing EK cert/possession/INTC/firmware/interface3 and boot metadata do not demonstrate storage independent of disk/vTPM restoration, original NV provisioning lifecycle, exclusion of Clear/substitution/redefinition or global quiescence. No new hardware claim inferred from an inaccessible WMI provider. No counter/index state inspected or altered in this task. W4/L2 continue requiring factual independently advanced SAME generation; all joint-restore limitations/UNKNOWN remain.

## Concrete next observation and alternatives

MANUAL-ELEVATED-METADATA-ONLY.txt contains a REAL hash-pinned command for the same NULL-output query in an ALREADY elevated owner PowerShell console. That distinct token context is the minimum remaining local observation capable of clarifying whether the current error was access/context-related. It has not been executed automatically and does not ask for secret values, TPM commands, NV effects or new operational approval. Same error remains UNKNOWN; NOT_FOUND would only establish source not found by this interface/token/instant; positive sizing would not establish custody or authorization. No second call with a value buffer or retry is built in. No need to repeat if an existing legitimate holder is already known: supply only its nonsecret provenance/controlled-delivery mechanism, never authorization value.

If legitimate storage auth is actually unavailable, STOP NV. Conditional focal proposal ADR-CANDIDATE-q4-witness-without-local-ownerauth-20261028.md preserves full rollback guarantee and presents: A legitimate existing provider; B separately designated EXISTING external atomic-consumption witness outside local restore domain under W6/L2, canonical root remains local ledger; C do not perform cross-boot readback. No external authority/provider/service is claimed to exist. Introduction of one would need its own explicit architecture/interface/custody/resource/operational decision; no generic service, local-only marker, signing key/PCR, DPAPI/Git/eventlog fallback or personalM3 shortcut. Proposal is NOT_ACCEPTED and no branch silently selected.

The current source is not proved unavailable, so no fallback activation or new NV attempt is warranted. Next fact is the owner-elevated metadata result OR nonsecret identity/provenance of an existing legitimate authorization holder. That fact is more discriminating than another proof of EK possession or another synthetic DENY test.

## ED11 parallel and closeout

Existing E17-E20 source/evidence reviewed/reused without repeating campaigns. E17 requires service readiness/identity/effective access under future eligible operation; E18 real lifecycle/Stop/quiescence. E19 D4/D5 target age/validity/duration/physical/commit policy values not justified or selected; E20 root/CURRENT/epoch/SiteTLS/backend/destination bindings and one-operation Browser/C2/C3/provider active costs pending. No ownerAuth status or TPM trust choice can provide those target measurements. No new falsifiable defect in these unchanged sources justifies rebuild/retests or invented quantitative values.

E21 updated with actual nonsecret policy/API observations, not PASS for custody or persistence. E22 dependent; ED11/Phase6 PARTIAL. No product source change, no existing protocol/binary/operational pin changed, no repeated tests; single new read-only metadata collector build/observations only. HEAD/WIP/histories/UNKNOWN/reservations preserved, no commit/clone/worktree/cleanup/refund/CURRENT/signature/recovery. No NV/Q4/Start/Stop/SCM/ACL effects.

## Primary sources

- https://learn.microsoft.com/en-us/windows/win32/api/tbs/nf-tbs-tbsi_get_ownerauth — supported nullable output, type13, NOT_FOUND.
- https://learn.microsoft.com/en-us/windows/security/hardware-security/tpm/trusted-platform-module-services-group-policy-settings — TPM2 authority mapping, policy5 and distinction from TPM1.2.
- https://learn.microsoft.com/en-us/windows/win32/secprov/win32-tpm — methods, no caller-authority proof from IsOwned.
- https://learn.microsoft.com/en-us/windows/win32/tbs/command-blocking — command permission separate from hierarchy authorization; no changes made.

No cited documentation proves local credential state. FINAL-VERIFICATION.json and SHA256SUMS.json contain exact artifacts/status. Source unavailable is NOT the factual conclusion; no authorized source demonstrated is the conclusion.
