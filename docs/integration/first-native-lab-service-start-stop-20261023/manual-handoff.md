# Manual first LAB lifecycle: one attempt

Exact official-reviewed [plan](exact-manual-plan.json) and [source/evidence](closeout.md). This is the first lifecycle attempt, not another registration/remediation. No effects have been executed at handoff. Do NOT rerun old ACL/registration launchers. Service currently DISABLED/STOPPED with original image; all prior descriptors/bytes remain preserved.

Qualifier SHA256 `219d13960ac3bfdc98cb94c5e3baa8e834d8f5b52afe731006cc47ec1e02387d`. Candidate SHA256 `62087dbe13712c468dc92c5848de684cbc7cba6f921d15259f895fa8664d2716`. Launcher SHA256 `d2773859973a4d2f924c47ac3d5717328fc14e964654f3e2698669845354666d`. Absolute fixed paths, arguments, actual native IDs/configuration and DLL hashes/descriptors are bound in the native executor; the launcher also holds its pins for the whole invocation.

Only the owner runs this in an interactive Windows terminal:

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\tools\qualification\scm-lab-first-cycle-uac.ps1' -Mode Launch
```

Bypass is process-only execution policy, not UAC bypass/persistent policy. Native UAC directly handles credentials if needed; the already-approved effective operator SID must match. No password parameter/Read-Host/log. Running from an elevated console may not show another UAC prompt. The real elevated identity, not the original user name, is verified.

Read the ENTIRE native plan. It creates ONE new protected papacc_server_lab_host.exe under the bound original parent, copying the already-approved original image owner/group/DACL at creation; existing target collision is STOP, no overwrite/adoption. It explicitly rebinds only the existing LAB image command to quoted new path plus service host and startup DISABLED→DEMAND. Account/SID/dependencies remain unchanged. No existing ACL/security descriptor or binary byte is modified. No product service is installed or started.

Type the complete displayed `CYCLE_SCM_LAB <attempt>` line, then Enter. A blank Enter or mismatch cancels; Esc/mismatch before effect denies. The text is specific to that native invocation, never reusable read-only/F10 authorization. F10 is only for closing the completed result window.

Approved cycle: one Start -> factual closed-runtime RUNNING -> status/PID + actual token/groups/privileges + effective file accesses + imported module paths + endpoint/resource snapshots -> one Stop -> factual STOPPED/zero exit + process signaled + checked close. A single safety Stop is included if evidence capture fails after RUNNING, retaining first error. No retry or fabricated quiescence. No listener/admission/CA/key/cert/bootstrap/CURRENT/signature/KSP-private acquisition. Actual KSP denial remains untested; not calling CA is not proof of effective KSP isolation.

Expected success: `LAB_CYCLE=RUNNING_TOKEN_ACCESS_STOP_QUIESCENCE_PASS`, final ERROR0/CLOSE0/CLOSED1/EFFECT2/START_REQUESTED1/STOP_REQUESTED1, `CYCLE_PASS=True`, outer `SCM_FIRST_CYCLE_EXIT=0`. Assess receipt/output and factual observations together, not merely exit0. Service remains DEMAND/STOPPED with new candidate retained after success; no automatic binding/startup restore, file deletion or service removal. These persistent effects are part of the confirmed plan.

On failure, do not repeat. Intent remains `manual-native-intent.txt` in this directory. Incrementally flushed `manual-<attempt>.log` records nonsecret output. On completed return the launcher writes `manual-<attempt>.json` including hashes/operator/native output/result. If `LAB_CYCLE_RETAINED` appears, live disposition or close is UNKNOWN: qualifier and parent launcher intentionally remain alive with pins/refs, no final completion receipt. Preserve the window/logs; do not force-close/kill/retry or claim rollback. Further handling needs factual diagnosis and explicit authorization. A timeout never means STOPPED.

After a returned result, retain evidence and close with F10. Independent post-result read-only review must verify identity/configuration, process disposition/Stop, original files/descriptors, token/ACL observations and resource limitations before marking the cycle qualified. Do not run Launch again to perform verification. Product listener/live apply remains a subsequent separately governed slice.
