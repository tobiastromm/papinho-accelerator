# ACL_IDENTITY — exact independent diagnosis

2026-10-23, diagnostic/preparation only. Exact paths come from the pinned launcher and its manual receipt, not leaf-name inference. Parent417614100:1966080:79426 remains verified PASS (parent-preserved.log). native-readonly.json records file native IDs, hashes, native descriptor, SD hash, ordered ACE fields and checked closure. Files were opened by NtCreateFile FILE_OPEN relative to the held parent, OBJ_DONT_REPARSE|OBJ_CASE_INSENSITIVE, non-directory/open-reparse/synchronous options, READ_CONTROL/read attributes/data, shareREAD. No CREATE/overwrite/security-write operation. Native IDs/size/mtime remained identical through hashing on the SAME handles; no file or parent reparse observed.

| File | Current native ID | SHA256 | First failed source predicate |
|---|---|---|---|
| papacc.conf | 417614100:393216:1120240 | a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44 | Applicable ALLOW foreign writer ACE0 |
| papacc_server.exe | 417614100:262144:1119989 | 3737e83be95613a2616b9fb764fe95d72796912746e9440e37b3ed2a77e01abb | Owner is neither approved operator, SYSTEM nor Administrators |

These current native IDs form the pre-effect pins for the proposed correction. The failed historical binding receipt omitted leaf IDs; byte-identical pinned hashes and parent identity corroborate path/content continuity but cannot prove historical leaf identity continuity absolutely. No historical ID is fabricated. A later effect MUST re-open these exact IDs and revalidate descriptors/hash/mtime, denying any substitution.

## Predicate comparison, each file

Actual scm_descriptor in apps/server/server_launcher_scm_prepare_win32.inc is unchanged. The isolated C harness includes/calls it on both real handles; leaf-native-validation.log records expected DENY for each and checked closes. Final MSVC /W4 /WX build passed; source included existing capsule header only for declarations, no new cryptography/product build. Python source reconstruction identifies the precise first failed clause, not a generic WIN32=5 assumption.

Both descriptors are valid, owner SID present, DACL present/non-null, six ordinary ALLOW ACEs, each INHERITED_ACE0x10 and parent/file-applicable (no INHERIT_ONLY). No DENY or explicit ACE is observed. DACL has AUTO_INHERITED, not PROTECTED. Group is informational for this predicate and must remain unchanged: conf ...785699550-1440458426-1682014700-513; executable ...896877160-3262526087-2538948522-513.

Configuration owner is operator ...785699550-1440458426-1682014700-1108: PASS. Executable owner ...896877160-3262526087-2538948522-1003: FIRST FAIL. If that owner alone were corrected, its DACL would still fail independently.

For BOTH files, six ACEs in native order:

| Index | Trustee | Mask | Predicate |
|---|---|---|---|
| 0 | S-1-5-21-896877160-3262526087-2538948522-1002 | 0x001301BF | FAIL foreign writer; first config failure |
| 1 | S-1-5-21-1212450354-1333653649-3686394325-941509878 | 0x001301BF | FAIL foreign writer |
| 2 | S-1-5-32-544 | 0x001F01FF | Allowed administrator trustee, not a new grant |
| 3 | S-1-5-18 | 0x001F01FF | Allowed SYSTEM trustee, not a new grant |
| 4 | S-1-5-32-545 | 0x001200A9 | PASS read/execute/synchronize |
| 5 | S-1-5-11 | 0x001301BF | FAIL Authenticated Users writer |

0x1301BF includes file write/append, write EA/attributes and DELETE. It does not include WRITE_DAC0x40000 or WRITE_OWNER0x80000. FullControl admin/SYSTEM includes those, but is not itself required by the normative contract. FILE_DELETE_CHILD0x40 is not present in these Modify ACEs and should not be conflated with DELETE. For files, write and deletion exposure can permit tampering/replacement subject to full Windows/parent access rules. Owner can normally control DACL even without a separate explicit WRITE_DAC ACE; foreign owner is therefore distinct from a write mask. No destructive access test was run. Inheritance itself is not rejected; the applicable rights/principal combination is.

Accepted virtual identity V3 requires protected configuration/executable identity against substitution through effect; V4 requires operational least privilege and no operational writes to binary/admin authority. The exact three-owner/writer allowlist is current adapter policy, not a newly inferred accepted universal rule. No weakening is proposed.

## Exact correction prepared, NOT APPLIED

acl-correction-PROPOSED.json pins each identity, content hash, native descriptor hash, owner/group and full before/after SDDL. All SIDs are numeric in the proposed postimage to preserve actual group identity rather than depend on domain-relative SDDL alias expansion.

- Configuration: retain current owner; preserve group and six ACEs/order; reduce ACE0/1/5 masks from0x1301BF to0x1200A9.
- Executable: change owner to the approved operator; preserve its group and six ACEs/order; reduce ACE0/1/5 identically.
- For each file ONLY, protect its DACL and retain current inherited ACEs explicitly. Otherwise inheritance from upstream could reintroduce the rejected masks. No parent/ancestor/other-file ACL change. Preserve existing Users and Authenticated Users read/execute, admin/SYSTEM rights; no new FullControl grant. Actual service-token access still requires later real-token proof; preserved broad read rights are not that proof.

Later mutation requires exact official approval and manually authorized scoped effect, descriptor/identity recheck immediately before setting, same retained handle through effect/readback, checked cleanup and UNKNOWN retention. Verify owner, group, all six ordered ACEs and PROTECTED; only AUTO_INHERITED variation allowed as already qualified. Never retry or assume rollback. These files have no descendant propagation scope. No executable mutation launcher is enabled by this task, and no mutation was performed. Build tooling may lose write access to these artifacts; that is an explicit consequence to review, not reason to grant unsafe writes or move the target silently.
