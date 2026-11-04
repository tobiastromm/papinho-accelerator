---
status: proposed
revision: 1
scope-level: project
scope-target: standalone native C mathematical EK proof; explicit console host binding only
supersedes: []
---
# Candidate — C0–C9 explicit real console host binding

No owner acceptance is inferred from technical investigation. No existing accepted ADR is edited. This proposal does NOT authorize TPM, Q4, NV, service, SCM/ACL, credits, CURRENT, signing or recovery. It requires specific owner normative approval and a later exact manual request after qualification.

## Context and precise change

Exact previous request2f225db7 requires window-process distinct from operator and proof. Actual parent17204 and HWND process17204 are identical, while original-handle native discovery finds separate process20144/conhost with stable creation/token/session/file identity. Readonly pair samples977 then884 handles; neither is a budget grant. Classic and ConPTY fixtures prove that HWND owner varies and is not the locator of every real console host. The proposed change replaces the window-PID role locator, NOT the requirement to identify and count actual distinct operator/native/auxiliary processes.

Accepted [B+D](DECISION-canonical-lab-console-topology-bd-20261023.md), [N7 exact historical scope](DECISION-n0start03-n7-and-ed11-evaluation-plan-20261024.md), [W](DECISION-q4-w0-w7-20261026.md), [L](DECISION-q4-l0-l7-20261026.md), [L1](DECISION-q4-l1-single-root-component-20261027.md), Q0–Q9/R0–R9/RAW1/H0–H9 remain intact. No LAB N7 acceptance transfers to this standalone proof. The exact original native proof request remains spent, not retrospectively amended.

C0. Scope is one future exact owner-manual transient mathematical proof using the existing native C protocol. No service/owner/IPC/intent/authority added. Candidate process IDs below are evidence only, never reusable admission pins by themselves.

C1. Bind operator parent, native proof and actual auxiliary by ORIGINAL process handles plus PID/creation. Enforce approved operator SID, elevation/no impersonation, token continuity, session and lifetime. File volume/ID/size/hash/protection/provenance are separate from mapped-image provenance. Reused PID, object replacement, missing mandatory data or changed identity denies.

C2. HWND owner and console client list are metadata. HWND owner may equal operator or native client without merging the REAL auxiliary into them. A complete contemporary client list must still include the authorized parent and proof and reject unexpected operation participants. This revision never authenticates by PID/name/HWND alone.

C3. NtQueryInformationProcess class49 may DISCOVER a candidate only in a qualified Windows x64 ABI profile. Preserve NTSTATUS, width and raw low bits; do not treat every masked value as a host. In the observed profile, qualified forward flag1 and nonzero aligned masked PID lead to an independently opened candidate; flag2 backlink and flag0 metadata are not additional hosts. Require repeated matching observations from original operator and native handles, original auxiliary identity and fresh census. Unsupported width/status/flags, ambiguous host, change or missing member evidence => DENY. Never call a setter, bypass privilege or rewrite Windows association state. This internal API may change; version/dependency changes invalidate eligibility.

C4. Specific owner risk acceptance would acknowledge that this native discovery plus identity checks does NOT cryptographically authenticate complete console association or mapped image. It is permitted only for the standalone mathematical experiment, with no trust/hardware-generation/admission inference. This is a NEW narrow decision, not implicit reuse of historical N7. If that residual risk is not accepted, retain DENY and require stronger authenticated linkage. Missing controlled process identity, token, file provenance, resource or lifetime evidence is NOT waived.

C5. Count by unique original process identity, not role labels. Operator + distinct actual helper share1024 handles, each process observed separately with live overlap; native proof remains128 and preobserver124. Shared/alias roles of the SAME process object count once; same PID with different creation denies. All actually operation-related additional UI/broker/helper processes must be identified and covered by the applicable existing limits or DENY. Explorer ancestry alone is not an administrative actor: observed desktop baseline stays explicit, not zero/credit; do not exclude any operation-related cost merely because it has an ancestor role. No quota, headroom, freshness, four-role or retention relaxation.

C6. Keep the inherited interactive console for exact confirmation and Escape/Stop, no artificial helper, automatic console-host selection, flag mutation or generic IPC. Context changes/lost console/failed query deny. A diagnostic FreeConsole/AttachConsole of the OBSERVER only is not a future proof topology or a substitute for observed inherited membership. Manual elevated readonly collection must first resolve the remaining client-list/context gap.

C7. Integrate only after acceptance into the existing host's guard/ownership path, preserving two temporary observer references when applicable, first error/freeze, checked close and original-reference retention. Measure added scalar storage/frames and lookup overlap before use; no presumed zero cost or budget credit. Validate concurrency, query failure, role omission, PID reuse, partial acquisition, close uncertainty, Stop and UNKNOWN. Reuse qualified crypto/protocol tests unless affected.

C8. Before any new TPM request require qualified actual topology/role coverage, file/security/identity pins, original handles, fresh124/128/1024 and other applicable obligations, exact current host/boot/operator context and separate owner operational approval. Real EK possession/revocation/custody/NV/Q4 remain separate; read-only topology success does not close E21/ED11.

C9. Prior approval4a5f was used once; no retry. Do not modify consumed request/approval/artifacts or use absence of receipt as authorization. No cleanup/refund/Stop/SCM/ACL/CURRENT/signing/recovery follows from acceptance. ED11/Phase6 PARTIAL until applicable factual gates pass.

## Evidence and remaining gate

[New factual evidence, native tests and alternatives](../integration/n0start03-integrated-closeout-20261024/real-console-role-binding-20261027/CONSOLIDATED-CLOSEOUT.md). Real seed/auxiliary identities and per-process resources observed; current collector could not join elevated console (AttachConsole5) despite FreeConsole success/no clients after detach. A single inherited read-only collector in the existing elevated owner console is prepared, not automatically executed. No new TPM request is ready.
