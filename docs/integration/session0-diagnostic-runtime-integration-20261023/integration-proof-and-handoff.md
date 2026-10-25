# Canonical runtime integration and unarmed next operation

Implemented profile requires PAPACC_SCM_RUNTIME_LAB + PAPACC_PSR_TRACE_LAB + PAPACC_PSR_FILE_RUNTIME_LAB. File-LAB without native recorder is a compile error. Product/default/console behavior remains outside this opt-in. No service/worker/logger/journal/registry/authority added. The existing ServiceMain calls existing psr_prepare; its file-LAB entry now binds the sink before the original prepare sequence. Default trace output remains unchanged outside file-LAB; file-LAB routes the private callback directly into the same native owner without OutputDebugString/console/stdin/stdout/desktop/PowerShell or receiver communication.

## Factual binding

Fixed existing service PapinhoAcceleratorLab20261023 and account/SID, same protected parent native417614100:1966080:79426, same planned attempt75330d11cc7e4123ae62eadc9a59c2f5. Parent opens read-only for attributes/security/synchronize, no delete share, no reparse; native ID checked and existing parent validator reused without weakening. A bounded GetKernelObjectSecurity buffer checks protected DACL/approved owner. No parent write is granted.

Diagnostic leaf is papacc-psr-75330d11cc7e4123ae62eadc9a59c2f5.diag. Runtime NEVER creates it. It opens the pre-created object exclusively with0x120083 rights, reads actual native identity and keeps the handle, verifies volume/link/type/reparse/normalized parent relationship, exact approved operator owner and protected four-ACE DACL. Operator/SYSTEM/Administrators FILE_ALL_ACCESS; specific service SID only0x120083. No inherited/additional ACE, wrong principal/right/owner or unprotected DACL accepted. AUTO_INHERITED is not treated as a string identity. Arm must be exactly1024 with same attempt/version/schema/checksum/zero payload; already-finalized2048 or malformed arm rejects.

This is binding to an observed, protected, held native object, not a textual-path grant or invented ID. The same handle/native identity and file pointer survive from validation through final append; no reopen/retarget between check and use. Future administrative preparer must additionally match its CREATE_NEW/readback native ID with the reviewed operation plan before Start and final extraction. No external file ID or created-file receipt is fabricated here. The actual diagnostic file under the protected parent still does NOT exist.

Packet associates the attempt, expected service/config hash and psr_prepare stage with existing trace API/object/access/NTSTATUS/Win32 propagation. `expected_config_sha` is explicitly a planned value, not a claim that config validation already succeeded on an early failure. Existing runtime still verifies protected config native ID/hash/token/ACL before readiness. First failure remains original native/Win32 result; recorder error has its own owner field. Missing/overflow/short/flush/close failure cannot make capture or readiness successful.

## Stop and lifecycle

Stop before bind acquires nothing. Stop after arm read prevents append and retains borrowed refs. A request while synchronous write is already in flight allows factual completion/flush/close of that operation, never forced cancellation or timeout-as-close; later readiness still observes Stop. There is one append attempt and first terminal trace event per owner. Partial/uncertain write, failed flush/close or source divergence retains the references. Existing psr_close and runtime_closed now refuse retirement/STOPPED while any diagnostic parent/file ref or retained state exists. Existing ps_park_retained containment is reused; no parallel recovery worker. No readback implies cleanup, no destructive retry/delete/rearm.

Prepare-success requires kind2 capture receipt and no diagnostic error/refs before ready=1. Native failure returns the original error even if recording also fails. Model tests drive real owner functions, not a new lifetime manager. Real VSA token and actual SS0 process are not claimed by model/no-console fixtures.

## Candidate and conditional rebind

File-LAB now selects the fixed exclusive image leaf papacc_server_lab_host_ntstatus.exe; the default historical image leaf is unchanged. Existing original proposal is preserved, not rewritten. Fresh build output is named papacc_server_lab_host.exe until an officially reviewed CREATE_NEW placement to the new fixed leaf. No historic image is overwritten. Selected Release source artifact:

```text
C:\Projetos\PapinhoAccelerator\build\session0-diagnostic-runtime-integration-20261023\Selected\Release\papacc_server_lab_host.exe
SHA256=eda84c0cf91ad1266cee462e62c89b84ffa5873853a0122215d15d666701361b
EXPECTED_EXCLUSIVE_PLACEMENT=papacc_server_lab_host_ntstatus.exe
REGISTRATION_OR_PLACEMENT=NOT_RUN
```

This pin is identity, not authority or file placement. Other profiles/hashes in evidence/instrumented-artifacts.json. Existing service remains bound to historic62087... artifact and old consumed intent remains non-replayable. Boot PASS preserved, historical orphan UNKNOWN not reopened.

ED11 native admission remains STOP per [ledger](resource-ledger.md). Therefore no runnable positive launcher/rebind command is armed. After that gate: official exact review of existing qualifier/PowerShell successor must bind SAME service/account/config, new candidate/hash/new leaf, DEMAND initial state and this one new-file plan. Owner manually creates only the exact diagnostic object with protected security AT CREATION (no existing ACL edits), verifies plan/native ID/hash/SD/arm and closes prep handle; collision or uncertainty STOP. Candidate placement CREATE_NEW only; controlled quoted image+service host args, service initially stopped, no Start before specific owner approval. Update old qualifier's pinned219d.../62087... and initial DISABLED assumptions rather than rerun it. Any partial SCM/rebind effect is retained UNKNOWN, no automatic rollback/delete/retry. No productive CA/bootstrap/CURRENT/certificate/listener authority follows.
