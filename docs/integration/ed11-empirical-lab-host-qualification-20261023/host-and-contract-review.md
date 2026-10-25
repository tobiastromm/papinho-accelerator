# Single-contract host qualification review

Normative source remains the SAME B0–B6 candidate revision3. This is evidence/review, not another contract, accepted decision or resource authority.

## Host and boot evidence

Native query source pin45aa0cd41cc253f4aaedb6f500aa5383c06eea71c1126e1c4c4e3828bd6c67b7 verified against prior manifest before executing ONCE, unchanged. EvtQuery System/Kernel-General12, EvtNext(one), bounded EvtRender, checked EvtClose only. No SCM mutator, network, files/ACL changes, Start or native retry in that source. [Raw invocation](boot-query-execution.json) preserves exit2 and all output.

APIs and both close operations succeeded. Record236441 and StartTime2026-10-23T18:36:34.5000000Z match the accepted factual boundary. The query's comparison defect: it hashed the exact-case Computer text but compared a historical LOWERCASE hash. Offline parsing of preserved original System XML proves its exact-case hash is identical to the new query result:2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff. Lowercasing that SAME preserved Computer text gives expected c8d6ef2a8acad155f446bbe54499817b21207b15877ec80b0db329e486ae0e01. [Identity reconciliation](host-identity-reconciliation.json) pins the raw evidence hash. No hostname is exposed, no repeat event query and no case-normalization assumption substitutes for direct preserved raw evidence. Original exit2 is NOT rewritten to0. Minimum prospective verifier correction is matching the explicitly documented lowercase normalization; no correction/rerun of the historical query was needed to prove this observation.

GetNativeSystemInfo reports architecture9/AMD64. Current Windows API version10.0.19045, process64-bit. Historical boot-event BuildVersion19041 remains its own raw field, not a substitute for current OS version. Attempt75330d11cc7e4123ae62eadc9a59c2f5 is the existing reserved/unarmed attempt, not a newly allocated authorization. No reboot, old-instance cleanup or retrospective owner retirement is asserted.

## Existing service observation

To obtain the requested service state, a separate minimal read-only SCM query used OpenSCManagerW(CONNECT=1), OpenServiceW(QUERY_CONFIG|QUERY_STATUS=5), QueryServiceStatusEx, then QueryServiceConfigW. [Result](service-readonly-result.json),2026-10-23T20:35:22.472307Z: service PapinhoAcceleratorLab20261023 STOPPED(1),PID0,Win32 exit1077,specific exit0. Manager and service references closed without error. This is current status evidence, not proof of historical cleanup.

QueryServiceConfigW returned1783 before any configuration row or SID-type query succeeded. Its bounded16384-byte supplied buffer and requested query-only rights were not a service configuration change. No current image/account/startup/SID-type readback is fabricated; existing previous records retain their scope. Native1783 is the exact observed error; underlying RPC/provider cause is not proven, and is not attributed to an insecure ACL or virtual-account failure. No repeat or mutation. Existing config must be revalidated in the future exact-operation gate; diagnosing this independent query error is not permission for Start.

## Metrics and numeric choices

Historical resource observation2026-10-23T20:29:01.935895Z through20:29:05.389621Z remains RAM available6285791232,commit available17306345472 bytes; physical capacity21300559872,commit limit37270474752 bytes. No new capacity reading: unapproved freshness policy provides no basis to treat another transient snapshot as admitted capacity. Neither old value is current or reserved. Host identity evidence does not refresh resource snapshots.

Required metrics are explicit in B4.B.1. Hard controlled Selected3145728/Legacy2097152 remain; named service25124 includes diagnostic4400 once. Two directly owned diagnostic refs and one1024 append/final2048 logical bytes are code-backed bounds. Total process handles/threads and CRT/native/callback stack are not bounded by those counts. Future real service observation remains B4.C's OUTCOME. Known compiler stack prefixes63648 Debug/63280 Release remain partial, not new stack quotas.

Freshness: old snapshots cannot authorize another invocation. Require new metrics within the exact authorized attempt before effects; host/boot/binary/config/native identity/account/SID/token/operator/OS/filesystem/topology/environment/concurrency/Stop/query-failure invalidations remain explicit. Numeric maximum age/sample gap is not determined by host capacity or collector elapsed time and is still unqualified.

Headroom: require approved physical and commit floors, phase-aware controlled reservation plus explicitly accepted residual native risk, existing canonical host-credit ownership and retained-charge rules. No mathematical operation on a single capacity snapshot yields a safe empirical reserve. No factor, arbitrary seconds, old minimum or quota-minus-census difference is selected to force conformance. HOST_CREDITS=NONE_GRANTED; reuse/refund only after factual lifetime, no resource disappearance as release, no unknown bytes filled with zero.

The remaining gap is NOT merely a missing factual query or another premeasurement of real service peak. Exact numeric policy values and rationale/residual-risk choice must be completed/reviewed as a normative decision within this proposal. An additional memory query cannot by itself supply that choice. No extra measurement or competing ADR is prepared to conceal it. Current state is PROPOSED, not ready for conditional official acceptance. No need for another reboot or a universal kernel ceiling.

## Whole-contract review

| Clause | Conformance / remaining gate |
|---|---|
| B0 | One exact reserved attempt/LAB identity; host/boot now reconciled; no authority |
| B1 | Controlled caps/counts preserved; no nested double count or quota change |
| B2 | Owned native closure separate from kernel cache; uncertain lifetime retained |
| B3 | Residual UNKNOWN explicit; scope-specific empirical approval still pending |
| B4.A | Exact binding/successor/owned reservations remain required before operation |
| B4.B | Host identity resolved; metric/retention rules explicit; numeric freshness/headroom/credits still incomplete |
| B4.C | Real-service peak measured DURING attempt; no circular premeasurement |
| B4.D | Kernel/cache/stack/concurrency risk explicit; at most one Start/Stop/file; not yet accepted |
| B5 | Fail-before-effects, one-shot consumption, Stop and no destructive timeout cleanup preserved |
| B6 | Normative acceptance distinct from successor implementation/qualification and exact manual operation approval |

No accepted ADR/ED11/Model E/ED08 clause is silently changed. B0–B3/B4.A/B4.C/B4.D/B5–B6 text remains identical to archived revision2; only B4.B and revision/current-state metadata change. Production0.6.4, quotas, PST/Browser/wire and four historical LABs unchanged.
