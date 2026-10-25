# First-install administrative entry: limited input implementation

Task PACC-PHASE6-FIRST-INSTALL-ADMINISTRATIVE-ENTRY-1; 2026-10-22.
main / HEAD 913c90cf2328f7939d8f5b927640601903ac6e91 preserved.
The official executor approved the exact operations.json/source preimage/postimage before application. No accepted ADR, worker, root, issuer or custody API changed.

## Implemented independent component

Existing opt-in candidate CLI now recognizes the following exact argument order:

    papacc_server.exe config first-install --declare NEW_INSTALL --source <file> --parent <directory> --container <name>

This is DECLARED_INPUT_ONLY, not an authorized administrative act or executable ceremony. Missing/other declarations, extra flags, empty/oversized spans and control bytes reject. Source and parent spans are limited to 259 bytes, container to 64, matching existing Windows input extents; this is not native path validation/canonicalization or identity proof. No arguments, paths, source text or purported identity are printed. The existing pre-runtime config frontend remains the only CLI.

All borrows belong to the synchronous argv call and end at return. No request object, persistent act, identifier, revision, UTC validity, pin, lease, ownership transfer or reusable approval receipt is issued. Thus repeated input remains denied; this does NOT claim positive replay protection for a future authorized ceremony. No candidate revision is invented. The explicit purpose is INITIAL_ROOT; UPDATE/restore declarations reject. No caller SID or authorization flag is accepted as authority.

A syntactically valid declaration returns exit 2 with OPERATOR=UNBOUND, AUTHORIZATION=ABSENT, TARGET=NATIVE_UNBOUND, CONFIGURATION=NOT_OPENED, CUSTODY=NOT_ACQUIRED and BOOTSTRAP/CURRENT_PUBLICATION=DENY. Invalid declarations return exit 1. No root, token, file, namespace, thread, worker, CA or certificate is acquired. No path probes infer NEW_INSTALL from absence. Existing config apply remains denied.

## Gate preserved

Accepted ceremony P1/P2 distinguishes declaration from native observations and requires an independently authorized actor, explicit operation and custody. These inputs cannot be supplied by command flags. The previous investigation found no reviewed complete entry producer; the owner confirmed no qualified installation is available. This independent parser does not remedy those factual gaps.

Full identity/target/configuration binding, operation revision/validity/replay, attributed audit provenance and transfer into the same owner/invocation remain NOT_IMPLEMENTED/NOT_QUALIFIED until the real authorization/custody path and participant sources can be qualified. A repeated denial is not an administrative act. No execution handoff to custody_begin is added. No alternative authorization store or synthetic producer is introduced. The accepted bootstrap, issuer/policy, root and Model E contracts remain authoritative; no normative details are accepted by this patch.

Next qualify the explicit local actor/operation against the existing native token/custody adapter and exact designated target/material references. Bind native identities and invocation in the existing owner, with revalidation, Stop, retention and disposition before enabling any effect. Initial bootstrap still requires no prior CURRENT. Do not substitute UPDATE approval. Real execution needs separate authorization.

## Validation

MSVC Debug and Release rebuilt the actual opt-in candidate server and selected tests, zero compiler warnings. Six suites each passed: config_binding.model/native_guard, phase6.memory_census, store_hash.lifetime/owner, bootstrap.scratch. Thirty real candidate CLI invocations (15 each configuration) passed. Cases cover declaration syntax, absent declaration, UPDATE/restore, extra operator/authorization flags, source/target span boundaries, changed target, repeated identical input, control input and legacy apply denial. No source/target argument is echoed. Nonexistent input remains nonexistent. All valid syntactic cases deny before acquisition.

These are native CLI negative tests and modeled declaration input, NOT positive operator/custody/administrative qualification. Operator divergence is tested as rejection of a caller identity flag, not a native identity transition. Changed target/replay remain denied, not successfully bound acts. Existing Stop/cleanup-retention regressions pass separately; this new command never acquires resources, so cannot release or disturb retained state. No bootstrap, CURRENT, certificate, network or system-clock change ran.

## ED11

No fixed owner/slot/worker/pin/managed buffer growth. New entry uses borrowed argv plus bounded scalar scan, no heap or native resources and no PC_DOCUMENT workspace construction/source parsing. No overlap with the live runtime is possible through main's pre-runtime CLI return. CRT output/process startup/argv and compiler/code costs are not free.

Adjacent actual COFF disassembly shows explicit prologue RSP changes: Debug first_install_entry 40 bytes, span 24, CLI 72; Release entry 40, span no prologue allocation, CLI 56. These are own frames only, exclude return/home/caller/nested CRT and OS attribution, and are NOT a complete stack peak. Object headers retain code/data evidence, not a new operational quota.

The build used the existing Legacy managed profile, limit 2097152 (Selected cap is not borrowed). Known fixed ledger remains 1790952 Legacy / 1956448 Selected. Full native/provider/kernel/CRT, actual source/custody graph, nested stack/host attribution and concurrent/retained credits remain UNKNOWN. No total peak or operational margin is claimed. ED11 remains PARTIAL; all operational authority gates stay denied.
