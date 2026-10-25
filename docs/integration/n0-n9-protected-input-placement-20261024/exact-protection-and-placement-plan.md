# Exact protected creation plan — manual approval pending

## Review result and smallest operation

Use the SAME existing parent and TWO original proposed leaf names. Do not create another directory or change the shared parent, its owner/group/DACL/SACL, existing files or historical LABs. Proposed correction is a protected creation descriptor on each NEW file, supplied atomically with CREATE_NEW. It is not SetSecurityInfo on existing objects and not a repair after an unsafe write window. A protected explicit child DACL avoids inheriting the unsafe parent ACE; actual child protection must still be verified before treating inputs as qualified. No operation has been executed.

Native original-parent identity417614100:1966080:79426 was checked through the existing PaccFileAcl read-only helper. GetSecurityInfo(SE_FILE_OBJECT,mask7) returned264 original bytes, SHA256 `6483a0d3acb883e2af96b684fcaa5e39d0aeed696f2bb117fb4b388ddce70868`; [native raw preimage](parent-before.raw) and [identity/control/ACE evidence](parent-before.json) are preserved. Two same-handle raw samples matched; original handle closed successfully. Current token SID1003 is NOT approved operator1108. SACL is not queried. First formatting attempt failed in PowerShell RawSecurityDescriptor constructor argument binding, after native read; finally closed its original handle. No first successful JSON evidence is invented: the first null artifact was replaced by the corrected query's evidence, and this paragraph preserves that error. Correction used the existing .NET constructor only to inspect RAW bytes, not reserialize native evidence. No security API write was called.

Parent owner is approved operator1108; DACL protected, all recorded ACEs explicit (no INHERITED_ACE). It includes SY/BA full access, users/sandbox read/execute and AU read/execute, plus AU OI|CI|INHERIT_ONLY generic read/write/execute+DELETE mask0xe0010000. That last ACE does not apply on the parent itself, but can propagate to children. No parent FILE_DELETE_CHILD grant to untrusted principals was found in these observed masks. This is scoped ACL evidence, not proof against a compromised administrator/kernel, malicious existing handle or all ancestor/path races. Future operation must retain/revalidate original parent and sources, reject substitution/reparse/unknown state, and stop if it cannot prove the path targets the retained object. No blanket ACL normalizer or whitelist is permitted.

## Exact new-object descriptor — PROPOSED, not applied

[Creation input](proposed-new-file-creation.json), [exact prepared binary](proposed-new-file-creation.raw), SHA256 `0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814`, 152 bytes.

```
O:S-1-5-21-785699550-1440458426-1682014700-1108G:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;0x12019f;;;S-1-5-21-785699550-1440458426-1682014700-1108)
```

Owner approved SID S-1-5-21-785699550-1440458426-1682014700-1108, group Builtin Administrators. DACL protected; exactly three explicit ALLOW ACEs: SYSTEM and Administrators retain existing administrative full control; approved operator gets FILE_GENERIC_READ|FILE_GENERIC_WRITE mask0x0012019f for one-time content write/readback/flush and subsequent qualifier reads. No operator DELETE/WRITE_DAC/WRITE_OWNER ACE is added; owner still has Windows owner semantics, which this plan does not claim to remove. No AU/BU/sandbox/other principal gets an ACE, no inherited grant, no deny-Everyone ACE that could override necessary owner/system access. Service does not consume these administrative inputs and needs no new grant. Group ownership alone grants nothing. SACL/integrity labels unqueried remain UNKNOWN; no privilege enablement or policy waiver.

This binary is an exact PROPOSED CREATION INPUT assembled in memory. It is not the RAW1 observation of an existing file. Windows-assigned new-object native SD bytes/control must be captured as fact and validated against the intended owner/group/protected ordered ACEs and writer set; never call the .NET serialization a native RAW1 sample or silently accept a changed ACL. The opened OS-file RAW1 digest remains unchanged and distinct.

## Exact two input effects proposed

Parent: `C:\Projetos\PapinhoAccelerator\build\scm-protected-installation-binding-20261022\Selected\Release`.

| New leaf | Bytes | SHA256 |
|---|---:|---|
| papacc-n2-75330d11cc7e4123ae62eadc9a59c2f5.evidence |5358|babab3ea393b605802933054ab011e6fcb66b21bffcc80923448715e911f27e0|
| papacc-n2-75330d11cc7e4123ae62eadc9a59c2f5.approved |326|5a2b7203afdece0adca77be278d6b86c18cc2cf457cab87474ee7dce0311c294|

Source review bytes remain at [previous preparation](../n0-n9-protected-dossier-placement-20261024/candidate-pins.json). They retain original attempt75330d11cc7e4123ae62eadc9a59c2f5; no new attempt and no old receipt as authorization. RAW1 decision8b042e2b..., catalog provenance/content/native file ID and target/policy pins unchanged. Future new input native IDs are absent until actual creation. Live process/token/metrics/challenge remain future existing-owner facts, not invented fields.

## Future manual procedure and fail-closed requirements

This is an approval package, not an executable authorization or a preflight command. Exact implementation/command hash must be reviewed before any manual execution. The existing helper provides read/apply but no qualified protected CREATE_NEW two-file workflow; do not use its Apply method or an unreviewed shell-copy command to improvise one.

1. Bind approved elevated operator1108, no impersonation, exact host/boot236441, attempt/policies/plan and explicit confirmation; validate pinned source bytes through retained handles. Hold original parent query handle and require exact native ID and RAW preimage264/hash above. Recheck both leaf names, but only CREATE_NEW determines collision. Stop before effect suppresses all creation.
2. For each exact leaf once, use CreateFileW with GENERIC_READ|GENERIC_WRITE|READ_CONTROL, share0, CREATE_NEW, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OPEN_REPARSE_POINT, SECURITY_ATTRIBUTES with bInheritHandle=FALSE and the approved protected creation descriptor. No overwrite/open-existing fallback. Any collision or native failure preserves first error and stops; do not continue with the other file after uncertainty. No service/SCM/intent effect.
3. Through the original returned file handle validate FILE_TYPE_DISK, non-reparse/non-directory, native volume/file ID, single link, owner/group/DACL/control and authorized writer set. Fail closed if unsafe inheritance, descriptor mismatch, unexpected owner or unobservable mandatory fact. Verify the final parent/path relationship and revalidate parent RAW identity/security before further effect. A textual path alone is insufficient. No dynamic ACL repair.
4. Write exact fixed bytes once with checked byte count; partial/error is failure with no retry or appending loop. Flush; set position and read back exact bytes/hash using same original handle; recheck native identity/SD and parent. Record new native IDs and actual RAW descriptors, size/hash and first error. No adoption of foreign/old file IDs.
5. No delete, rename, rollback cleanup or retry. If Stop arrives after one file, do not create the second; preserve the first and report partial result. Check each CloseHandle/LocalFree once; uncertain result retains original handle/buffer/identity and charge in live owning context; do not terminate owner to manufacture cleanup. UNKNOWN cannot be reused, retried or promoted. Successful closure yields a factual placement receipt only, not resource admission or permission to activate pins.
6. Active pins/adapter/launcher stay unchanged and DENY. Separate source/pin qualification remains required, including explicit RAW1 revision/self-relative guards and fixed protected paths; any future source patch needs applicable four builds. Then separately request one read-only preflight. No Start/Stop/rebind, ACL changes to existing objects, CA/cert/bootstrap/CURRENT, extra roles/credits or production authority.

## Tests and residual limitations

Three existing isolated suites SD13-case, BD73-case and PSRF model PASS ([results](tests-and-input-verification.json)); original binaries pinned, no source/build change. SD/BD exercise owner/security/digest/identity/substitution/stale/replay/Stop/retention/UNKNOWN; PSRF exercises collision/denied/partial/flush/close/retention/checksum/identity/Stop in a synthetic sink. These are reused components/models, not a tested complete new placement workflow. Unauthorized-write analysis of the proposed DACL finds no ALLOW grant outside SY/BA/operator; no actual alternate-token write/AccessCheck/child protection experiment occurred. A missing permission or unsupported check is DENY, not a synthetic PASS for Windows effective access. Two inputs and child descriptors NOT created. Parent unchanged.

[Microsoft CreateFileW](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew) documents CREATE_NEW collision and SECURITY_ATTRIBUTES; [inheritance documentation](https://learn.microsoft.com/en-us/windows/win32/secauthz/automatic-propagation-of-inheritable-aces) describes protected child boundaries. No universal race exclusion, exact serialization stability or SACL coverage follows. Existing trusted administrator/OS assumptions and Model E/ED08 factual closure/ED11 retained charges remain in force. Quotas/four roles/128/shared budget/NONE_GRANTED unchanged.

## Approval requested

Approve ONLY the protection plan for these TWO new objects using the exact proposed creation descriptor hash and exact input bytes above, with the shared parent untouched. This approval would permit preparation/qualification of the corresponding manual creation operation; execution still requires a reviewed pinned command and explicit manual confirmation. No current ACL mutation or creation is performed by this package. If this protection choice is not approved, maintain DENY without touching the parent or inputs.
