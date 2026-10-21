# ED11 final owner handoff: scoped policy qualified, total gates open

2026-10-12. PREPARED_LOCAL_NOT_SENT. NSS and R4 are separate PST-owner work; no duplication or requested implementation of them.

Accepted decision: incremental envelopes beyond factual baseline, never whole-process ceilings or minimum RAM. [Normative record](../adr/DECISION-ed11-preserve-incremental-resource-envelopes-20261012.md).

## Delivered evidence

[Complete ledger](../capabilities/ed11-total-resource-ledger-reconciled-20261012.md), [measurement report](../capabilities/ed11-total-resource-closeout-20261012.md), [immutable measurements](../capabilities/ed11-total-resource-measurements-20261012.json), [final50-check conformance and mapping supplement](../capabilities/ed11-total-resource-final-conformance-20261012.json).

Final policy50/50 PASS in x64 Debug/Release and VC6/C89 x86 on current Windows. Normal OpenSSL lifecycle676 cycles/719 logical operations plus two virtual-map runs; exact old normal SDK, not upcoming reissued package. Existing C1 x86 executable externally observed without modification; extracted layout compiledVC6 pump412 bytes. Zero warnings in final applicable builds. Historical23 probes/4 models/28 ED08 executions preserved.

Corrections in isolated candidate policy: monotonic closed-operation identity; late callback fence; host snapshot credit cannot be double-spent; partial allocation/cancel rollback; persistent terminal quota; independent CONTROL Session lifetime at zero ops; Session-generation and borrower guards. No production code was integrated.

## Decisions still required

1. Approve/revise controlled slab layout512KiB core,16KiB Session, Browser1MiB application storage,2MiB managed/process. Future full structs must fit before product activation; model sizes alone are not proof of C2 runtime.
2. Define per-target headroom reserves, sample freshness, process/address-space/handle limits and shared attribution. Require incremental reservation AND available host/process margin. The standalone fixture64MiB safety floor and synthetic policy inputs are not accepted production thresholds.
3. For total user/kernel: require supported hard allocation/input/kernel bounds, or separately authorize an explicitly restricted empirical lab policy. Current full enablement remains denied. Monitoring after allocation cannot provide fail-before-effects for opaque PST/provider/kernel growth.
4. Obtain actual Browser/C2 Session/CONTROL/DATA topology and baseline after separate implementation/encoding authority. No Browser/C3 production changes authorized here. Windows10 raw C1 cannot substitute for the full Browser or NT4.
5. After governed new normal SDK is returned, verify commit/header/lib/DLL/CRT/manifests and reexecute only impacted measurements. Current upstream homologacao/pst-0.6.5 HEAD4dbd2783ebaf135114da2b7247e36c0ad6bc550a was read-only confirmed. Measurements consume normal staged commit da7190930700f54ecc92611f5edbcb0932caa5ec. Version/API/SPI equality does not merge their evidence.

## Smallest ED11 upstream/target dependency

Initially request existing supported normal-package bounds and input restrictions for core/SSL/provider/credential/peer-info allocations, per-context/global retained allowances and failure boundaries. Ask which can be reserved before effects and which remain empirical; no private pointer-size estimate or SDK rebuild authorized. If a new API/contract is necessary, return a separate decision request rather than modifying PST.

Socket/TCP/driver total has no certified bound from SO_SNDBUF/SO_RCVBUF or process pool quota. Target owner must provide a supported accounting/limit method or accept explicit empirical uncertainty. Host available counters and fixed finite socket count alone do not certify an8MiB kernel ceiling.

NSS correction/R4 do not solve this resource-bound dependency automatically. Do not wait for them to accept or reject the independent ED11 policy scope.

## Reproduction and return

Standalone sources at tests/diagnostic/ed11_total_probe; instructions in README. Artifact root build/ed11-total-20261012 holds all logs, source diffs, pre/post hashes, run traces and final audit. Public fixture inputs and exact normal SDK copied unchanged; no PST rebuild.

Owner return should name approved scope, metric definition, candidate numeric caps/headroom policy, explicit unsupported categories, accepted residual risk if any, required target evidence and future SDK identity. No wire approval, C2/C3 production authority, E1-E12, Legacy production, PST promotion or pin change follows automatically.
