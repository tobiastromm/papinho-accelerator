# Candidate manual read-only procedure — NOT YET AUTHORIZED

Exact draft request SHA256: `8bbb9647f92cf7ac4c4ea0a5f1b37db0b21a0deb2ad5f8e80dbc89c9b4eb4de6`.
This is a protected historical inventory request, not Q2 application or Q4/new LAB admission. Obtain specific owner approval of this request/hash first. Do not run automatically or reuse a consumed approval.

Use one existing Windows PowerShell5.1 x64 process already elevated by the owner, with the authorized primary token and enabled Administrators, no impersonation and no preloaded reader types. Do not launch a child PowerShell from this script. Do not bypass UAC/GPO or change ACLs. The pinned host/boot must still match; mismatch means STOP without repin. No concurrent project operation should alter these objects during inspection; sharing/identity/read errors deny, no automatic repeat.

Script SHA256: `64070a6525472ce95defce3b941961f30f4d11d5ffb03ee3baebefb9dc2fa307`.
C# inspection source SHA256: `6fe1d262291f0343be4d316fcafa9c70898c676263d3a37d4ab4074d9ea5d117`.
Qualified native helper SHA256: `f52242fe74932d1539e83a742430ea523eda746ce39d8815b1b977cf6bb6ac75`.
Unchanged qualified03 dossier reader SHA256: `1ec26dc9bd04a468543836b42c574a8b872515b5f596398a1901613edf8b9cc0`.
All dependencies are checked before in-memory compilation. The current compiled product/launcher/pins remain unchanged. The helper's pre-existing mutation methods are never invoked by this reader; the Verify call graph contains only existing-object read and checked own close operations.

After approval ONLY, the complete candidate command is:

```powershell
& "C:\Projetos\PapinhoAccelerator\docs\integration\n0start03-integrated-closeout-20261024\q2-q4-protected-readback-and-disposition-20261025\q2-protected-readback.ps1" -Mode Verify -ExpectedScriptSha256 64070a6525472ce95defce3b941961f30f4d11d5ffb03ee3baebefb9dc2fa307 -Confirmation "READ_ONLY_Q2 N0START-20261024-03 0b1de869e11493b7cb08fb329578f6ce"
```

This script does not change execution policy. If policy or elevation prevents it, stop; no remediation or retry is implied.

It reads the installation chain through original parent handles using NtCreateFile/FILE_OPEN/read-only/noninheritable/shareREAD/no-reparse; logs original NTSTATUS and IO status; requires exact synchronous FILE_OPENED, bounded identity/size/content/security and checked close. The four target files are canonical consumed intent, historical image, final sink and configuration. It then reuses the unchanged native03 N0 reader in the SAME process to revalidate existing dossier IDs/content/RAW1. No protected file, intent, sink or image is created; no service API mutation or preflight runs. Existing native IDs absent from old receipts are explicitly UNKNOWN, never assigned retroactively.

RAW1 is GetSecurityInfo mask7 on the original file handle, original bytes/digest/base64, native owner/group/ordered DACL; no sorting or normalization. Historical exact descriptor pins are compared only where actually available (root/dossier); other historical RAW continuity remains unproved and must be reviewed. Separate source-compatible DACL checks reject untrusted writers; intent creation's GA mask and mapped file-FA representation are logged as distinct raw bytes, not treated as RAW1 byte equality. SACL/audit/labels are not queried and remain UNKNOWN.

For the sink: require original creation native ID417614100:196608:2922916,2048bytes, immutable arm and kind1 result; validate native-format frame binding/padding/checksum,525byte record and payload SHA256cab4f25f9e95257222e8f64b9fadc98f13d040b5681e5d253481cb2b1f09c437 reconstructed from the archived ASCII record with native LF line endings. That expected payload is source-derived, not a claimed already-read full-sink SHA. Actual sink digest will be observed by the owner, not invented here.

Escape requests Stop between new inspector calls. Synchronous calls are not canceled by timeout; abrupt host loss remains UNKNOWN. Any error stops further stages. On uncertain close/descriptor retention, keep the process/reference state for governed review; the wrapper reports retention and waits. Do not close again, kill, retry, delete, restore or release mutexes to force completion.

Provide ALL console output, including original failures, raw descriptors, native IDs, FIRST_ERROR, dossier result, controlled close/retention and final exit. Visible console text is not itself persistent evidence; the returned original output must be archived byte-consciously and reconciled. Do not infer Q2 application or ED11 PASS from exit0. A success records readback checks, not historical native-ID continuity where no old ID exists, physical refund, global quiescence, a service-token fact or operational admission.

After factual result, review Q2's joint evidence and request separate documentary application approval only if mandatory conditions are proved. If continuity/protection/pending-work evidence remains insufficient, preserve UNKNOWN and STOP. Q4 requires its own owner-selected exact identity/annex; no GUID/name/intent or new service observation is authorized here.
