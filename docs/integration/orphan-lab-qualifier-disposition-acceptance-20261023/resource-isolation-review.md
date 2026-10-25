# Resource isolation and independent-attempt review — 2026-10-23

Assessment uses existing source and evidence, not new native probes or a newly allocated attempt. Historical identity remains563826bacc49449f954b20cc48ce9b36; no process absence/window/PID reuse is treated as release.

## Interference matrix

| Resource class | Existing facts / interference | Current conclusion | Necessary future evidence |
|---|---|---|---|
| Old process-local memory/stack/static owner | qualifier/launcher retain unknown lifetime; neither new process nor new address namespace proves aggregate capacity or absence of effects | do not borrow/reconstruct old owner or reset its flags | independent owner state/references, no callbacks/operations retargeting old resources; capacity evidence including retained coexistence |
| Handles | original handles are references, not underlying object identities; service handle had CHANGE_CONFIG/START/STOP as well as query access; file pins have share/access constraints | new handles to old objects are not disjoint resources | exact object identities/access modes and proof no conflicting mutation/control/retirement; no closing/duplicating/adopting unknown references |
| Existing SCM service | fixed PapinhoAcceleratorLab20261023, virtual account/SID and mutable image/startup binding; prior observation STOPPED/PID0 does not retire qualifier's rights | same-service new Start/rebind is NOT proven independent | exact governed exclusivity/control proof before effect; no two concurrent instances under this one mutable service; no claim that current status alone suffices |
| Persistent candidate/installation files | installed image417614100:589824:2690484/hash62087… remains old persistent effect; parent/config/original exe/DLL identities/descriptors are known from earlier read-only snapshots, their retained references are separate facts | replacing/rebinding an old pinned file is not isolation; known file content is not UNKNOWN handle disposition | separate exclusive owned targets or precisely proved compatible immutable sharing; unchanged old bytes/identities/ownership; separate authorization for any future new targets |
| Read-only shared dependencies | common approved immutable bytes can conceptually be shared without adopting an old handle, but retention/sharing/ownership/access and accounting still matter | no blanket ban on every shared byte; no automatic sharing PASS | prove compatible access/lifetime/readiness and no write/delete/rebind interference; count shared costs once only with evidence |
| Administrative intent/log/receipt | exact consumed intent and retained incremental log; no final attempt receipt | cannot reset/copy authorization/replay launcher or use unrelated CLOSED1 receipt | new independently authorized records only after separate approval; old evidence retained, no retroactive receipt |
| Operational identity | current fixed LAB account/SID is tied to the same service; a new textual attempt does not change TokenUser or control domain | current execution shares identity and mutable target | factual eligible identity/least privilege tied to future exact scope; readback is not live token evidence; no fallback/SYSTEM/private CA access |
| Shared host/SCM/LSA/kernel/provider/CLR | unknown old process/launcher/native costs; existing size/hash/tests do not establish available host margin | ED11 coexistence NOT_QUALIFIED | retained bounds plus new peak and proven shared attribution/host capacity; unobservable required bounds deny, not zero/refund |
| CA/KSP/history/four LABs | independent protected owners, no access or effect in current operation | preserve states, no cleanup/authority borrowed | no change or attribution to this SCM attempt |

## Existing implementation is not an independent successor

apps/server/server_service_identity_win32.h fixes the LAB name/account. tools/qualification/scm-runtime-cycle-lab.inc fixes cycle_path and accesses the same service through mutable control rights; its cycle_can_retire guard remains !cycle_start_requested || cycle_quiescent. The old intent is CREATE_NEW and the launcher rejects its presence before Launch/Elevated. Protected-runtime source pins old parent/config identities. Giving a new GUID to the same invocation would still touch these objects and fail independence; clearing intent or substituting image/hash would change/bypass guards. None is done.

A separately reviewed fixed LAB profile/target might provide its own namespace and state through the same canonical adapter/runtime, but it is not implemented, allocated, authorized or qualified here. No new service name/path/attempt ID is selected. Distinct names alone do not resolve shared-host capacity, cross-control rights, dependencies or owner lifetime.

## Model E / ED08 / ED11 proof obligations

Model E and Stop require retaining actual old references/causes and non-reuse; they do not require pretending old resources were freed to describe a separate experiment. New owner/attempt/lifetime must not replace the old owner or use its retained objects. Local cleanup/retirement and any new admission are separate proofs; no release/rebind/reuse is authorized for retained objects.

ED08 release-order/EOF/FINAL rules remain about their exact protocol ownership; no C2 operation occurs and no artificial FINAL is manufactured. ADR0030/0031/0032 scope does not supply a generic process orphan API or universal unrelated-work ban. Virtual identity V3/V5/V8 still constrains canonical service identity and effects; changing the fixed LAB scope is future exact review, not a fallback.

A sufficient shared-host admission bound would need old retained obligations + new peak + shared costs (deduplicated only with proof), within both applicable constraints and factual host margin. Old98 handles, new recorder120 bytes, binary size, per-process3MiB selection or no visible PID does not supply that proof. No required UNKNOWN is zero; no refund/quota increase. Current evidence cannot demonstrate coexistence. Therefore NEW_ATTEMPT_ELIGIBILITY=STOP_CURRENT_SHARED_RESOURCES, despite acceptance of the administrative disposition.

## Avoiding an unjustified permanent deadlock

Do not repeat the same unsuccessful PID/window/SCM queries as a condition for discussing independent work. Future review can use materially new exact-instance evidence OR demonstrate a genuinely independent resource graph and qualified coexistence/isolation. Neither option is proved or authorized here. Separate qualification need not falsely mark the historical attempt clean, retired or destroyed; its evidence remains UNKNOWN permanently if no later proof arrives.

Eligibility is blocked by specific mutable/shared dependencies and missing admission evidence, not by a rule that the word UNKNOWN anywhere prohibits every future LAB. This assessment is a set of necessary conditions, not a completed isolation proof or permission for execution. No new binding/launcher/Start is prepared.


## Subsequent focused technical census — 2026-10-23

[Focused resource audit](../orphan-lab-resource-isolation-20261023/closeout.md) qualifies the interference wording: the logged retained source branch schedules no further SCM operations, so an open control-capable handle alone does not demonstrate active mutation. It still supplies no original-instance/owner handoff or total-native coexistence bound. Fixed named host state20724 bytes includes recorder120 and CLI4 already reserved; no local accounting patch or quota increase is indicated. First missing numeric term is old qualifier/launcher retained native user/kernel upper bound. Current alternatives A–C remain unqualified, D=STOP; O0–O5 and original UNKNOWN are unchanged.
