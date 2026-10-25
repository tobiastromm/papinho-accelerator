# Windows UTC source: native investigation, qualification denied

Task PACC-PHASE6-WINDOWS-QUALIFIED-UTC-SOURCE-1. Q0-Q7 accepted and the prior portable-core delivery remain authoritative and byte-exact. No functional patch, source adapter or PAL extension is applied. This is a read-only investigation plus reproducible diagnostic, not a new clock/provider or qualification procedure accepted by inference.

## Factual installation and evidence

Baseline Accelerator main913c90cf2328f7939d8f5b927640601903ac6e91:5973 git-visible files,22 WIP lines. Browser main96f6cd2f9ec4f834999828fa13bb7d170db66a98:2155 files,clean. Prior44-entry manifest verified before investigation. Windows version reported by the process is10.0.19045.0,x64. Local Get-Service reports W32Time Running,StartType Manual; local registry Parameters.Type=NTP. These are local installation observations, not upstream identity/authentication/health or proof that synchronization is qualified. No peer names, secrets or service configuration changes were collected/applied. No w32tm sync, outbound status query, external time source or internet dependency was introduced.

A small separate-process Windows SDK diagnostic was compiled MSVC x64 Debug/Release /W4 /WX. Each execution streamed256 samples of GetSystemTimeAsFileTime, optional GetSystemTimePreciseAsFileTime and bracketed QPC reads. QPC frequency observed10000000; no QPC/wall regression observed in this small run. GetSystemTimeAdjustment succeeded: adjustment156250,increment156250 in100ns units,periodic adjustment disabled=FALSE. A current periodic adjustment parameter is neither an absolute UTC error bound nor a guarantee against a subsequent step.

GetProcAddress on the already-loaded kernel32.dll resolved GetSystemTimePreciseAsFileTime, but not GetSystemTimeAdjustmentPrecise or QueryUnbiasedInterruptTimePrecise. This describes this specific lookup/environment only, not universal Windows support or a proof that no alternate API export exists. No owned module handle was acquired and no alternate provider was loaded. No fallback observation is released. Missing optional read APIs do not get fabricated results: the precise adjustment function was not called. The earlier output fields with zero values in its diagnostic line are initialization, NOT measured quality.

SDK10.0.26100.0 sysinfoapi.h/realtimeapiset.h declarations were inspected locally and hashed in sdk-provenance.json. They identify available API shapes, not deployment security/accuracy guarantees. No online reference was fetched under the network prohibition. We do not infer an absence of all possible Windows solutions from these declarations or a short experiment.

## Qualification matrix

| Requirement | Evidence obtained | Qualification |
|---|---|---|
| Native UTC reading | FILETIME values and optional precise reading | Raw DATA only |
| Local API/backend identity | Borrowed kernel32 export and local process | Not authenticated upstream UTC identity |
| Time service configuration | W32Time Running;Type=NTP | Configuration is not source authenticity or health |
| Absolute UTC uncertainty [L,U] | No authenticated finite bound obtained | UNKNOWN: first blocking requirement |
| rho direction UTC/PAL | QPC frequency observed | No finite rate-error guarantee qualified |
| PAL additive E,including d=0 | API units and adjustment increment observed | UNKNOWN; no E=0 inferred |
| Source applicability D | No qualified source profile/health lifetime | UNKNOWN; no caller TTL assigned |
| Manual steps/rollback | No regressions in bounded sampling | No guarantee of detecting all steps before effect |
| Suspension/resume | Not induced;no qualified continuity contract | UNKNOWN; no propagation across event authorized |
| Restart | Fresh diagnostic processes only | Not a host-restart test;no authority restored |
| Stop/retention | Existing DATA core tested;probe acquires no resources to release | No new production lifecycle/source installed |

QPC is a local duration mechanism and does not authenticate civil time. A 100ns FILETIME unit, a more precise API, frequency, timer increment, successful API read or service Running state must not be used to fill rho/E/error/expiry by inference. A bracket around a read bounds a local measured acquisition interval; it cannot by itself bound the upstream UTC error or authenticate the source. Correlating two local APIs does not independently establish UTC accuracy.

The existing PAL only promises durations/relative deadlines. The existing issuer backend.utc is a scalar, and CNG delegates to administrative UTC. None supplies current qualified provenance plus finite end-to-end error and continuity evidence. No observed API/status closes Q2. The first positive-adapter STOP is therefore source authenticity/origin and finite absolute uncertainty not proved; rho/E/D and continuity also remain open. This is an evidence/implementation qualification gate, not merely lack of an elevated sandbox token, and not a normative conflict in Q0-Q7.

## Safe result and tests

No positive adapter is implemented because the section4 preconditions are unmet. papacc_issuer_qualified_utc_read remains the existing NOT_SUPPORTED boundary and leaves the output unchanged. The diagnostic verifies this with a sentinel; it never returns a raw FILETIME as a qualified observation. Source, PAL, portable core, issuer/UPDATE/CURRENT/E0 guards and wire remain byte-exact. No status positive, emission, signature, CURRENT, root, trust store, registry, journal, worker or service is created.

Both builds have zero warnings. Both diagnostic executions passed for raw sampling and denial/output preservation. Both fresh core-test executions passed11583 DATA/math/negative checks. Synthetic rollback,source mismatch,overflow,Stop,retention and restart DATA checks are explicitly not native source qualification. A valid qualified observation was NOT_RUN. Manual system clock changes were NOT_RUN without specific authorization; suspend/resume and host restart were NOT_INDUCED. We did not issue time setters, resync, service/registry/ACL/token changes, sleep/suspend/reboot or network. Production/NT4/box/ISA are not qualified.

## Minimum handoff; no new authority

Before any adapter hunk, qualify the source actually managed by the installation: approved source/backend identity and configuration revision; how current native health/provenance/authenticity are verified; finite total UTC error covering acquisition,quantization and upstream uncertainty; target-specific rho and additive E for all covered cuts including d=0; finite exclusive applicability D and invalidation criteria. Administrative authorization or written assertions alone are not a factual producer.

The qualification must account for manual adjustment,loss of synchronization,steps/slews,source/configuration changes,suspension/resume,restart and clock rollback. If guarantees cease, the same owner must invalidate before effects and require explicit requalification/new governed operation; no retry until favorable time. Event logs/notifications may be investigated later within existing ownership, but have not been shown to cover every boundary atomically and are not installed here. No numeric default or new external time protocol is proposed. Existing managed synchronization, if it can expose sufficient facts, can be qualified without a new service; its availability/authentication is not assumed from Type=NTP.

Only then prepare the smallest Windows adapter under the same issuer backend/owner,keeping Win32 out of the core. Source references must be owned/copied with measured lifetime,not protected by a root pin by inference. E0 remains synchronous borrow-only,not a place for native time acquisition. No extra pin table or source registry. Preserve Model E/Stop/retained resources and revalidate immediately before effect. Current protected predecessor/status/issuer/policy and UPDATE authorization are independent prerequisites. A qualified UTC alone never grants approval or restores authority after restart.

Full activation is separately blocked by the already-recorded conservative3747825-byte scenario over3145728. Resolve that capacity/lifetime gate before embedding new state; no quotas or reservations changed in this investigation. See ed11-census.json. No repeatable local API probe can substitute for the missing deployment guarantees. If those cannot be obtained, preserve NOT_SUPPORTED and use the owner-governed deployment qualification decision rather than weakening Q0-Q7.
