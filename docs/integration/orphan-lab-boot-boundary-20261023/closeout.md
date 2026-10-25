# Orphan LAB boot-boundary qualification — 2026-10-23

TASK=PACC-PHASE6-ORPHAN-LAB-BOOT-BOUNDARY-1. Branch main; HEAD913c90cf2328f7939d8f5b927640601903ac6e91. The owner's already-performed Windows restart establishes a qualified boundary against continuation of the prior boot's volatile processes/handles/kernel state. No reboot or Start was executed by this task. Another reboot is NOT required for this finding.

## Evidence, epochs and scope

The result is based on a local System event lineage, not PID0/window absence, an estimated uptime alone, or a fabricated original process identifier. Collection is bounded to64 selected events. Original SCM anchor7023/Record236214 remains present with exact service/time/error matching the historical attempt evidence. Same Computer binding is verified across both epochs and stored only as a hash in reviewed evidence. No selected System-log-clear event occurs between the retained old start and new start in the collected interval; the collection reaches older records than that interval.

| Fact | Record / UTC | GMT−3 / interpretation |
|---|---|---|
| Previous kernel epoch start | Kernel-General12 /229875; StartTime2026-10-07T07:53:39.500Z |2026-10-07 04:53:39.500 |
| Previous startup metadata | Kernel-Boot20 /229877; field LastBootId=160 | retain original field label, not an invented process/boot GUID |
| Original LAB service failure anchor | SCM7023 /236214;2026-10-23T07:19:51.8934865Z |2026-10-23 04:19:51.893; PapinhoAcceleratorLab20261023, error5 |
| Owner-associated restart request | User32 1074 /236413;2026-10-23T18:35:37.3318046Z; action reiniciar |2026-10-23 15:35:37.331; event reason text is Other/unplanned, not a claim of power loss |
| OS shutdown | Kernel-General13 /236440;2026-10-23T18:36:19.2247254Z |2026-10-23 15:36:19.224 |
| Current kernel epoch start | Kernel-General12 /236441; StartTime2026-10-23T18:36:34.500Z |2026-10-23 15:36:34.500 |
| New startup metadata | Kernel-Boot20 /236443; LastBootId=161, LastShutdownGood/LastBootGood=true | raw reported fields, no invented current BootId/instance handle |
| Boot-type corroboration | Kernel-Boot27 /236445; BootType=0 | raw corroboration of restart chain; not used as sole undocumented enum oracle |

EventLog6006/6005 corroborate stop/start of Event Log. Their publication/record order is not assumed to equal all event source timestamps. The previous selected Boot27 at2026-10-10 reports raw2 without a new Kernel-General12 in this selected interval; no undocumented enum mapping is needed to establish the later explicit restart/full-start chain.

A new local GetTickCount64 observation corroborates the latest kernel StartTime to about0.385 seconds; this is a secondary consistency check, not the boundary's sole proof. Epoch identity is the machine-bound System start-event tuple, not a newly invented OS GUID. Original qualifier's exact creation-time-bound process identity remains unrecovered; PID28452 is still only a historical observation. The old attempt was observed before the proved restart; a full new kernel/process epoch excludes continuity of all such pre-restart volatile instances without inventing per-handle close receipts. Provider Event Execution.ProcessID is not substituted for a service/qualifier PID.

Evidence: [boot summary](evidence/boot-boundary-summary.json), [sanitized event lineage](evidence/boot-lineage-sanitized.json). Raw XML is retained in external governed scratch, not copied wholesale into the project.

## Supported semantics and inference

Microsoft documents that Restart performs a full boot cycle and does not use Fast Startup's hibernated kernel path. Fast Startup and hibernation can restore kernel/system state; sleep also preserves context. Therefore an off/on impression or uptime estimate alone would be insufficient. This audit combines the explicit restart action, OS shutdown/start chain, preserved older anchor, current-epoch corroboration and boot metadata. [Microsoft: Restart versus Fast Startup](https://learn.microsoft.com/en-us/troubleshoot/windows-client/setup-upgrade-and-drivers/fast-startup-causes-system-hibernation-shutdown-fail), [system power states](https://learn.microsoft.com/en-us/windows/win32/power/system-power-states), [startup-mode distinction](https://learn.microsoft.com/en-us/windows-hardware/drivers/kernel/distinguishing-fast-startup-from-wake-from-hibernation).

Inference supported by these facts: the original pre-restart process memory, stacks and kernel handle references cannot be continuing as those same volatile instances in this current epoch. This is epoch isolation, NOT proof that application Stop/close/release ran, an attempt completed cleanly, every pre-restart native call was rolled back, or every persistent/provider/external effect disappeared. Exact old exit/cleanup timing and all missing receipts remain UNKNOWN. No old owner flag/cycle_closed/cycle_quiescent is edited and no retrospective success receipt is generated.

## Persistent revalidation

Read-only SCM query shows the same persistent LAB service configuration: quoted old image + service host, virtual account NT SERVICE\PapinhoAcceleratorLab20261023, SID type UNRESTRICTED, DEMAND startup. State STOPPED/PID0, Win32 exit1077, service-specific0. Microsoft defines1077 as no Start attempted since this boot; it is not a new psr_prepare failure, zero-exit lifecycle PASS or old cleanup receipt. [Microsoft error-code reference](https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes--1000-1299-).

The persistent SCM configuration row is distinguished from old process-local SC_HANDLE values. New observation references are acquired/closed in the current epoch only. Revalidation grants no new mutation authority.

Existing read-only file/SD helper confirms all six approved descriptors and recorded original native identities/hashes. Installed candidate also receives a fresh native-ID/hash observation:417614100:589824:2690484,418304 bytes, hash62087dbe13712c468dc92c5848de684cbc7cba6f921d15259f895fa8664d2716; new observation handle close confirmed. Parent/config/original executable/DLL metadata matches prior protected plans. No owner/DACL/content/reparse/path substitution, delete or replacement is performed. Old intent/log remain unchanged and exact final receipt remains absent.

CA/KSP objects, protected roots, history and four historical LABs are not queried, removed or reclassified. Persistent side effects of earlier uncertain operations remain their owners' responsibility; reboot does not prove rollback, deletion, authority, latest state or recovery.

## Model E / contracts / ED11

O0–O5 accepted allows factual isolation review while preserving old UNKNOWN and forbids fictitious release/refund. ADR0030/0032 Stop/retirement obligations are not waived: no old owner is declared retired or safely closed. ED08 ordering/FINAL facts are not fabricated; no C2 operation takes place. The current finding is a changed factual applicability domain for old volatile coexistence, not a new cleanup mechanism or a normative exception. Existing contracts suffice; no ADR is created/modified.

The previously missing R_old native retained maximum is still historically UNKNOWN. It is no longer a necessary coexistence term for the SAME pre-boot volatile resources in the proved new kernel epoch: their continuation is excluded by evidence. Represent this as OLD_VOLATILE_OVERLAP_EXCLUDED, not as a guessed numeric zero or a quota refund. Historical ledgers remain unchanged. New kernel/SCM/LSA/provider allocations and any freshly launched program belong to the current baseline and must be measured/accounted; similarly named new instances are not historical reference continuity.

Still unqualified for the next positive test: current service/driver/launcher/receiver native peak, transitive callback/CRT/OS stack, provider/kernel/shared attribution, host margin/admission, exact candidate/arguments/binding and receiver capture. Persistent service/files and authorization/one-shot intent require separate owner review before effects. Old1077 plus epoch proof does not rearm the consumed attempt, authorize same-service reuse, qualify a new LAB profile, or supply total ED11 PASS. No new service/environment is created for convenience.

See [post-reboot procedure](manual-post-reboot-qualification.md) and [resource epoch ledger](resource-epoch-ledger.md). The old volatile-overlap blocker is resolved within scope; the next gate is current-epoch resource/operation qualification, not another demand for the missing historical cleanup receipt.

## Governance and checks

Owner already restarted; only read-only evidence collection and documentary review performed here. Both epochs/anchor, current SCM/files and candidate identity were checked. Baseline8039 WIP hashes verified before documentary changes; exact pre/post hashes govern final writes. Accepted decisions, functional sources/tests/builds, PST/Browser/wire, quotas and production0.6.4 unchanged. No builds/service/native lifecycle/network experiment run; official documentation lookup is evidence research, not application-network qualification. No automatic restart, Start/Stop/rebind, ACL/account/boot-policy change, handle release of old references, clone/worktree/commit/push/tag/release/bootstrap/key/certificate/CURRENT.

```text
CURRENT_TASK_COMPLETED=YES
BOOT_BOUNDARY=PASS_FULL_RESTART_LINEAGE_AND_CURRENT_EPOCH_CORROBORATED
OLD_TRANSIENT_RESOURCE_CONTINUITY=EXCLUDED_FOR_PREBOOT_VOLATILE_INSTANCES_NOT_CLEANUP
PERSISTENT_RESOURCE_STATUS=SCM_FILES_ACLS_CONFIG_AND_HISTORY_PRESERVED_READ_ONLY_REVALIDATED
ED11_COEXISTENCE=OLD_VOLATILE_OVERLAP_EXCLUDED_NEW_CURRENT_EPOCH_COSTS_UNQUALIFIED
MANUAL_REBOOT_REQUIRED=NO_ALREADY_PERFORMED_AND_QUALIFIED
NEW_START_AUTHORIZED=NO
HISTORICAL_ATTEMPT_DISPOSITION=UNKNOWN_PRESERVED_NO_RETROACTIVE_CLEANUP
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_EXACT_DOCUMENTARY_CHANGES_ONLY
FIRST_UNRESOLVED_DIVERGENCE=CURRENT_EPOCH_NATIVE_PEAK_RECEIVER_AND_EXACT_OPERATION_ADMISSION_NOT_QUALIFIED
NEXT=POST_REBOOT_LAB_RESOURCE_QUALIFICATION
```
