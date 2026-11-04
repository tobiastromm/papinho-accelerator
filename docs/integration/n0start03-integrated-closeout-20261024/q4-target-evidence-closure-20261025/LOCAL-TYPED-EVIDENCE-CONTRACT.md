# Private typed projections of existing target observations

These are two implementation-private version1 by-value schemas, NOT a common wire/API or a new policy. Do not serialize native structs, compare their numeric enums, cast between them or compare participant clocks. The existing pair/receipt protocol remains unchanged. Accepted ModeC validity rules, ModelE and existing resource owners constrain the implementation.

| Property | Accelerator | Browser |
|---|---|---|
| Original owner/producer | canonical launcher root target_producer | existing C2 resource_phase native receipt slots |
| Schema/version | PAPACC_C8_TARGET_VIEW /1, sizeof checked | C2_TARGET_VIEW /1, sizeof checked |
| Origin | existing native raw2 only | existing native observation1 only |
| Source identity | PID/creation/thread, root invocation/owner/revision/epoch/domain, series/sequence | PID/creation/thread creation, mapping/root, request/operation/generation/epoch/phase/witness |
| Scope | own-process baseline/prepared; native counters host/process; provider/kernel perConnection UNKNOWN | existing first-reached local operation slots carrying process/host metrics, NOT peroperation attribution |
| Time | original PAL local monotonic ns-to-ms64 and recorded C8 raw expiry; original validation timestamp | original local GetTickCount U32 interval and explicit presence; no applicable expiry supplied |
| Freshness meaning | coherence of recorded raw diagnostic interval only; not an approved ModeC maximum age | wrap/regression/incomplete interval denies complete observation; quantitative freshness still absent |
| Units | bytes or counts explicitly, host/process/connection scopes | bytes/handles explicitly, process/OS peak/host scopes |
| SDK/backend | compiled OpenSSL/x64 normal package inventory; runtime provider UNKNOWN | compiled NSS/VC6/x86 normal package inventory; runtime provider UNKNOWN |
| Image meaning | checked OS process/mapped paths, NOT byte attestation | existing mapping/build metadata, NOT attestation of mapped provider bytes |
| Eligibility result | coherent view still RESOURCE_UNKNOWN | OBSERVATION_ONLY never admission; positive helper always false |
| Missing policy | ABSENT, never a zero/unlimited maximum | policy/expiry/recipe presence all absent; forged presence invalid |

The root exports from its existing producer on the original owner thread outside dispatch and before activation. It rejects output aliasing, wrong source/config/series/generation/SDK/architecture/metric/unit/scope, incomplete APIs, clock regression and Stop. The copied sequence must equal the current original producer; an old view is not current proof after that producer advances or closes. Validation rechecks native process image identity without rewriting original recorded timestamps. No new collector/history/global slot/heap/handle is introduced.

Browser integrates the immutable view into existing receipt peak aggregation. Historical live/retained/stopped/destroyed observations can remain readable without granting positive reuse. The new validator is not a global anti-replay registry and does not authorize arbitrary cross-thread publication; existing owner serialization remains mandatory. Both owners preserve retained resource obligations. Independent test threads are joined and checked closed; no claim that sampled counters prove global quiescence.

Zero is a factual metric value when its API and presence/classification say it was observed. Missing/UNKNOWN remains missing even if its storage is zero. Fields identify scope; process totals from multiple operation receipts must not be added once per operation, nor private/WS/managed bytes counted as disjoint totals. No measured peak is promoted to a hard ceiling.

A future quantitative policy/recipe can be implemented only after its exact authority and supported artifacts/identities are established. This version contains NO setter, environment toggle or automatic recipe that can turn UNKNOWN into positive capacity. Existing target_admission/c2_capacity remain closed. No LAB approval is applicable to either private schema.

The Accelerator preserves the validation instant returned by its existing native provenance call, and separately records verified_ms from the actual local clock after the native identity checks complete. Neither observation nor recorded expiry is rewritten. Revalidation also checks completion against that same expiry; time spent blocked in identity observation cannot extend validity. This is diagnostic coherence only, with ModeC numerical policy still absent.
