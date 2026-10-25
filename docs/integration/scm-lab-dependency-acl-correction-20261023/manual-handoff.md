# Manual two-DLL operation and independent verification

Only owner manual execution may request UAC/effects. Do not invoke previous parent or exe/conf remediation launchers. This operation does not register/rebind/start/stop/remove a service. The existing service remains DISABLED/STOPPED. Both target DLLs are under the exact existing LAB parent; there is no alternate location or productive target.

Launcher SHA256 `32a121d0221899f8f81384eeb8c085ae2ea1dea0a4d82f13cab1f0c5cfae201d`. Native Selected/Release validator SHA256 `d956daa9297ee95d1b9f9b64ae7779a9427aaab7e4db427f5952e37c18308b52`. Hashes of helper/plan are fixed within the launcher. Original accepted contracts are unchanged.

From the owner's interactive Windows terminal:

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\tools\qualification\scm-dependency-acl-uac.ps1' -Mode Launch
```

ExecutionPolicy Bypass applies only to that process; UAC/persistent policy is not changed. UAC handles any credentials directly. If already elevated, Windows may not show another credential prompt. The effective approved operator must still be S-1-5-21-785699550-1440458426-1682014700-1108 with enabled Administrators, no impersonation and interactive console. Another elevated identity is denied; no fallback.

Review both full target paths, before/after descriptors and the scope. Type the ENTIRE displayed `CORRECT_SCM_DLLS <attempt>` line, then Enter. A blank Enter, mismatched text or cancellation denies. The attempt is generated for this invocation and is not reusable. Do not type passwords. The window remains until F10. The outer result is `SCM_DLL_ACL_EXIT`; receipt paths are manual-intent.json/manual-result.json in this directory. Neither exists at handoff time.

Expected only after a successful manual operation: TWO_DLL_POST_READBACK_PASS; same IDs/hash and parent/exe/conf preserved; native SCM_DEPENDENCY_VALIDATOR=PASS; checked closes and no retained descriptor; result exit0. Any partial effect, receipt failure, mismatch or close uncertainty is STOP/UNKNOWN, not rollback. Do not retry the correction or delete files/receipts.

After manual completion, use this independent READ-ONLY verification:

```powershell
& 'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe' -NoProfile -ExecutionPolicy Bypass -File 'C:\Projetos\PapinhoAccelerator\tools\qualification\scm-dependency-acl-uac.ps1' -Mode Verify
```

Require VERIFY_PASS, native validator PASS, exact file IDs/hash/metadata, protected DACL with exact six ACEs and approved owner/group, preserved other files/parent, receipt closure and absence of unexpected effects. Before correction Verify must deny POSTIMAGE_MISMATCH; this was tested. Success does not authorize service Start. First Start still needs the separately reviewed candidate placement/rebind/lifecycle and actual token/readiness/Stop qualification.

## ED11 and limitations

Launcher controlled held handles: four read-only pins (self/helper/plan/validator), two DLLs, two preserved files and parent =9; a transient intent/result stream can coexist =10. Helper FileDigest wraps the same handle without ownership/duplication and uses an8192-byte stream buffer. SD read bound65536 bytes per helper query; at most one helper SD allocation queried at a time. Native child query has file handle plus owner/SD/SDDL LocalAlloc objects; successful frees/closes explicitly checked. Uncertain resources are never reported as zero. Read-share-only native handles persist through readback and preservation checks.

PowerShell/CLR/Add-Type/compiler subprocess, JSON/model buffers, native stack/CRT/kernel, provider/system costs and peak coexistence are not fully measured/bounded by these counters. GC lifetime is not factual release of protected native handles; native handles use explicit checked CloseHandle. No quota increase, production ED11 PASS or operating admission. ED11 PARTIAL.

Final qualification: Selected/Legacy Debug/Release builds zero warnings;20 semantic cases (P/PAI and owner/group/rights/protection/extra ACE/flags/principal/order rejection),6 launcher branches,4 native current-descriptor DENY observations,56 existing suite/configuration and72 CLI cases pass. Check exit0 means exact unsafe preimages match, not that DLL ACLs are safe. Verify before correction and no-console/pin modes deny before any effect. No interactive cancellation/positive mutation was simulated as factual. Initial harness argument error and unavailable Get-FileHash were corrected before these final results; process-only Bypass was needed for the signed-script execution policy. No automatic approval review rejection occurred.
