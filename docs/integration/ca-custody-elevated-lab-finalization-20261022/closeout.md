# Elevated CA custody LAB finalization — 2026-10-22

Task PACC-PHASE6-CA-CUSTODY-ELEVATED-LAB-FINALIZATION-1. Preparation/negative qualification completed; positive finalization NOT executed or qualified.

## Exact STOP and scope

The task section3 requires “confirmar ausência de colisão por consulta suportada” and “Se houver colisão ou disposição incerta do novo identificador, STOP.” The previous prepared plan explicitly describes BAD_KEYSET as provisional not-found, not factual absence. The currently qualified diagnostic reports UNKNOWN after this error, even under elevation. No supported stronger absence discriminator has been qualified in the existing owner. This task therefore does not arm generation from that result. No source, ACL, flags, policy or accepted contract is changed to weaken this gate. No negative official review occurred; this STOP comes from the task's explicit precondition and existing evidence contract.

The prior preparation remains historical. Its candidate raw creation command must NOT be run: the complete current collision/disposition gate is not satisfied. Explicit authorization for this LAB scope does not make a native UNKNOWN into absence. No request for repeated permission is made; the missing prerequisite is factual evidence.

## Prepared launcher

`tools/qualification/ca-custody-elevated-lab-uac.ps1` reuses the qualified native UAC/console/token pattern. Default Check validates executable hashes and observes only the current token/console, without KSP calls. Launch is MANUAL ONLY, native RunAs, visible new console, Windows PowerShell -NoProfile -ExecutionPolicy Bypass; no UAC policy change, credential argument, password input or fallback. Bypass is process-local execution policy selection, not an elevation bypass. Launch was NOT invoked by the executor.

Fixed new identity is exclusively `PapinhoAccelerator.CA.2345e07adf714045a20d1c8b6fdf5aaf.a2bcd6d2ed3043d083e7e50342d6fab0`, Microsoft Software KSP / MACHINE. Expected effective operator is Tobias SID S-1-5-21-785699550-1440458426-1682014700-1108. Elevated requires matching actual SID, successful token queries, elevation, enabled Administrators, no impersonation and real input/output console. Different UAC credentials do not silently rebind. Self-hash is carried to Elevated; script and executable read streams prevent writes/deletion during use, with plain canonical/reparse checks. Fixed arguments are used, not caller commands.

The launcher pins both existing binaries but contains NO invocation of --execute-new-lab. It may only query the exact NEW name with the existing read-only diagnostic; it never queries/deletes/opens the previous name. Before that query it consumes one fixed CreateNew diagnostic receipt. A preexisting or interrupted receipt blocks reexecution; receipts cannot restore authority. These are bounded LAB observations, not a product journal or authorization registry. Manual-result directory contains no started/result receipt as of preparation.

If the new object opens, disposition PRESENT/collision means STOP. If BAD_KEYSET or another open error occurs, disposition UNKNOWN means STOP. Incomplete/uncertain cleanup also means STOP. All outcomes leave generation unavailable and exit2, even if the read-only query completed normally. F10 closes the result window, without changing this exit code. This launcher does not claim to be the positive finalization launcher: a stronger supported absence test and separately reviewed exact effectful wrapper remain necessary before arming that operation.

## Existing evidence / ownership

Original FinalizeKey(SILENT) NTE_PERM 0x80090010 remains unexplained. Previous elevated read-only OpenKey NTE_BAD_KEYSET 0x80090016 is not absence. Previous LAB disposition remains UNKNOWN and its identifier is untouched. Receipt SHA-256 6f3dbbff73beda7f17ad393fb583b521437cc3f760a8f66b6cb97a7615dbb019 remains verified. See [reconciliation](../ca-ksp-state-reconciliation-20261022/closeout.md), its immutable plan, artifacts and [earlier manual observation](../ca-ncrypt-finalize-nte-perm-20261022/manual-elevated-observation-20261022.md).

No new CA owner/reference exists in this task. No key/private export/certificate/signature/root/CURRENT/bootstrap or network operation occurred. Existing canonical key owner and backend are unchanged. The prior standalone harness remains pinned and unexecuted on its effectful branch; its safe-validated same-handle LAB cleanup is not automatically authorized for any uncertain object by this launcher.

## Tests and ED11

Windows PowerShell parser PASS. Check exit0 (hashes match, current sandbox token NOT eligible, no console); Elevated without expected script pin denied2 before token/KSP use; invalid Mode rejected1 before script body. No Launch or eligible Elevated run. Test outputs in tests.json distinguish Check success from native qualification. Prior standalone MSVC Debug/Release builds /W4 /WX remain hash-identical, zero warnings; no rebuild or new native C qualification is claimed here.

LAB wrapper costs include held read streams for script and two executables, token observation/managed objects, Add-Type/CLR/PowerShell/console/runtime, bounded receipts and captured diagnostic stdout. Future diagnostic can own one KSP and at most one opened key plus existing validation scratch; cleanup failure remains retained/UNKNOWN. Previous uncertain persisted namespace debt remains independently charged even though its process handles closed. Complete nested stack, managed/native runtime, provider/kernel allocations and persisted resources are UNKNOWN, not zero. No measured operational capacity/margin, quota increase or ED11 PASS follows from this preparation. ED11 PARTIAL.

Branch main, HEAD 913c90cf2328f7939d8f5b927640601903ac6e91. WIP preimages are verified separately; only the new LAB launcher/docs and appended capability note are authorized changes. No PST, Browser, wire, historical LAB, production0.6.4, clone/worktree, commit/push/tag/release modifications.

## Result

CURRENT_TASK_COMPLETED=NO
CA_LAB_KEY_CREATION=NOT_RUN_GATE_STOP
NCRYPT_FINALIZE=NOT_RUN
CA_LAB_KEY_DISPOSITION=UNKNOWN_NOT_QUERIED
CA_CUSTODY_REFERENCE=NOT_CREATED
ELEVATED_OPERATOR=MANUAL_NEW_RUN_NOT_EXECUTED
ED11_TOTAL_STATUS=PARTIAL
BUILDS=EXISTING_DEBUG_RELEASE_HASHES_VERIFIED_NO_REBUILD
WARNINGS=NO_NEW_BUILD
NATIVE_TESTS=LAUNCHER_CHECK_AND_NEGATIVE_GATES_PASS_NO_KSP
NETWORK_TESTS=NOT_RUN
PREVIOUS_LAB_KEY_DISPOSITION=UNKNOWN
BOOTSTRAP_EXECUTION=NOT_RUN
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=NEW_LAB_ABSENCE_DISCRIMINATOR_NOT_QUALIFIED
NEXT=CA_CUSTODY_NEW_LAB_COLLISION_GATE_BEFORE_FINALIZATION

The owner's intended subsequent NEXT=PERSONAL_FIRST_INSTALL_CA_CUSTODY_BINDING remains deferred until positive LAB qualification; no readiness advancement is claimed.
