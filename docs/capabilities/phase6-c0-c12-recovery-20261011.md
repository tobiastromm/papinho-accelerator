# Phase 6 / C0-C12 recovered roadmap and governed resume

Date: 2026-10-11. Live evidence, not a new ADR or replacement of accepted contracts.

Original Accelerator core READY is preserved for its original scope. Expanded Browser integration is NOT QUALIFIED. ADR-0022 accepted revision 1 defines placements A-D; ADR-0029 accepted revision 1 (2026-10-07) governs Mode C. Browser ADR-0007 governs Browser routing, permissions and fallback. The original C0-C12 roadmap is the joint gate section **Current-phase slices after approval**. Historical factual handoffs do not override these authorities.

There is no placement E. PST TLS A-E, Accelerator internal gates and Browser placements are separate matrices. ADR-0031 C7 and ADR-0032 candidate C11 B/C are not roadmap C7/C11. The PST feasibility alternative C1 byte transport is not roadmap C1 Browser socket pump.

## Placements A-D

| Placement | Network owner | Site TLS owner | Status | Evidence and limits |
| --- | --- | --- | --- | --- |
| A | Browser | Browser | QUALIFIED_FOR_SCOPE | Historical local HTTP/HTTPS current-host UI evidence; BearSSL authenticated-site trust debt and real NT4 remain open. |
| B | Accelerator | Accelerator | QUALIFIED_FOR_SCOPE | New scoped normal API3 Accelerator / Browser PST064 interoperability positive and negatives; Browser API3 Model E migration pending. |
| C | Browser | Accelerator | PARTIAL | Accepted ADR-0029 architecture; C0 exists, productive dual-role operation absent; C0-C12 roadmap. |
| D | Accelerator | Browser | PARTIAL | Opaque TCP_PLAIN foundation; Browser supported mixed-route composition/security qualification remains open. |

## Original C0-C12

| ID | Requirement | Authority | Status | Factual evidence / limits | Owner / next dependency |
| --- | --- | --- | --- | --- | --- |
| C0 | Endpoint Snapshot / Exact Connect | ADR-0029 section 4; Browser core/networking/papinho_socket.h | QUALIFIED_FOR_SCOPE | Blocking FIRST_IPV4 value snapshot/exact numeric connect; historical focused tests unchanged. New clean/name/truncation current-host regression evidence only. Not API3 Browser or real NT4 qualification. | Browser; full C0 closure review then C1 |
| C1 | Nonblocking/cancellable site socket pump | ADR-0029 section 5 | NOT_IMPLEMENTED | Site backend still blocking; no readiness/wake/deadline/half-close pump. PST feasibility alternative C1 is a different identifier. | Browser; next original roadmap slice under accepted contract |
| C2 | Operation + APPLICATION/RECORD PACC | ADR-0029 sections 2,3,9 | BLOCKED | Current DATA/session ticket not dual-role operation authority; wire IDs/encoding not allocated. | PACC owner; separate versioned wire decision required |
| C3 | Private C2 bridge | ADR-0029 section 8 | NOT_IMPLEMENTED | Historical socket attachment feasibility is not implemented bridge or API3 validation. | Accelerator; exact tuple/one accept/owner-only cleanup; C1/C2 |
| C4 | Secure authorization/binding | ADR-0029 sections 2,9,10 | PARTIAL | Existing Secure Session/capability/generation foundation; role-bound unpredictable single-use operation authority absent. | Accelerator/Browser; lab grant, epoch, replay/profile/swap negatives |
| C5 | Budgets/backpressure/cancel/deadline/terminal | ADR-0029 sections 5-7 | NOT_IMPLEMENTED | Experimental four-queue caps do not become production defaults. Existing Mode B queues are not Mode C evidence. | Both consumers; C1-C4, measured watermarks/fairness/terminal cause versus action |
| C6 | Deterministic E1-E12 fixtures | ADR-0029 section 12; joint gate D10 | NOT_IMPLEMENTED | Approved matrix/design only; Mode B fixtures do not qualify Mode C. | Both consumers; deterministic fragmentation/stall/cancel/egress/lifetime evidence |
| C7 | Secure Mode C E1-E12 experiments | ADR-0029 section 12 | NOT_AUTHORIZED | APPROVED_EXECUTION_NOT_AUTHORIZED; no later specific prerequisite-review authorization found. ADR-0031 C7 is unrelated. | Human owner; new gate after C0-C6 review, hashes, measured budgets |
| C8 | Production Secure Mode C integration | ADR-0029 consequences; Browser ADR-0007 Mode C debt | BLOCKED | No productive Mode C path; preserve mixed-route fail-closed guard. | Both consumers; C2 contract, C7 evidence, production decisions |
| C9 | Legacy production-security approval | ADR-0029 section 11 D9B; ADR-0028 | NOT_AUTHORIZED | D9B proposed; lab D9A does not authorize production. | Human/Accelerator security owner; accepted admission/grants/policy/quota/audit/revocation |
| C10 | Legacy implementation | Original joint gate C10; ADR-0029 section 11 | BLOCKED | Plaintext profile mechanics are not productive Mode C authority. | Both consumers; C9 and shared qualified runtime |
| C11 | Legacy validation/profile matrix | Original joint gate C11; ADR-0029 section 11 | BLOCKED | Not ADR-0032 candidate-close C11 B/C. No productive Legacy Mode C validation. | Both consumers; C9/C10, profile isolation/lower assurance/no downgrade |
| C12 | NT4 Mode C qualification | ADR-0029 section 5; Browser nt4-toolchain.md | BLOCKED | VC6/current Windows host is not actual NT4 acceptance. | Browser; implemented Mode C, target runtime and queue/budget measurements |

## Core and integration inventories

| Authority | Requirement | Status | Evidence / limitation |
| --- | --- | --- | --- |
| ADR-0015 | Bounded establishment/Network Stream lifecycle | QUALIFIED_FOR_SCOPE | Preserved Phase6 core evidence; no complete suite rerun |
| ADR-0016 | Initial capability negotiation | QUALIFIED_FOR_SCOPE | Preserved Phase6 core evidence; no complete suite rerun |
| ADR-0017 | Dedicated DATA/network framing | QUALIFIED_FOR_SCOPE | Preserved Phase6 core evidence; no complete suite rerun |
| ADR-0018 | Independent Network Egress/TLS Offload registry | QUALIFIED_FOR_SCOPE | Preserved Phase6 core evidence; no complete suite rerun |
| ADR-0019 | Server-owned Destination Policy/Egress Profile | QUALIFIED_FOR_SCOPE | Preserved Phase6 core evidence; no complete suite rerun |
| ADR-0020 | One-resolution DNS/numeric candidate policy | QUALIFIED_FOR_SCOPE | Preserved Phase6 core evidence; no complete suite rerun |
| ADR-0021 | Sensitive destination deny | QUALIFIED_FOR_SCOPE | Preserved Phase6 core evidence; no complete suite rerun |
| ADR-0022 | PST CLIENT outbound TLS profile | QUALIFIED_FOR_SCOPE | Preserved Phase6 core evidence; no complete suite rerun |
| ADR-0023 | TLS close/truncation/abort contract | QUALIFIED_FOR_SCOPE | Preserved Phase6 core evidence; no complete suite rerun |
| ADR-0024 | Concrete composition boundary | QUALIFIED_FOR_SCOPE | Preserved Phase6 core evidence; no complete suite rerun |
| ADR-0030/31/32/33 | Model E/lifecycle API3 Accelerator | QUALIFIED_FOR_SCOPE | Accepted handoff preserves P1-P12, D/E/F/G1, G3-G6/H/I, S1-S12, candidate C11 B/C; 55 distinct native lifetimes. No Browser lifetimes added. |
| Browser ADR-0007 | Independent routing/Network Egress/fallback | QUALIFIED_FOR_SCOPE | Historical A/B cold/reconnect/fallback/recovery evidence; C/D remain open. |
| Browser ADR-0007 GUI debt | Final independent capability/policy GUI | PARTIAL | Network Egress and Site TLS permissions/preferences must be exposed independently or explicitly deferred by governed version decision. |
| Browser ADR-0013 | HTTP/terminal/fallback consumer semantics | QUALIFIED_FOR_SCOPE | Complete versus incomplete HTTP, terminal/no-reuse and no inappropriate fallback; scoped new tests, no full API3 Browser claim. |
| Browser API3 consumer | Result-returning Model E release authority | IMPLEMENTED_UNQUALIFIED | Existing consumer still uses void pst_connection_release and clears contexts. API3 migration must preserve stream/session/manager/provider/runtime on retention; unsafe symbol-only replacement prohibited. |
| Browser ADR-0007 trust debt | Authenticated local Site TLS identity | IMPLEMENTED_UNQUALIFIED | Local BearSSL transport success is not authenticated site identity; documented local trust debt remains. |
| ADR-0028/0029 | Secure/Legacy capability parity | PARTIAL | Functional parity target YES, cryptographic parity NO. Production Legacy Network Egress restriction is current policy; D9B still needs accepted decision. |
| Browser NT4 target | Actual NT4 integrated runtime acceptance | IMPLEMENTED_UNQUALIFIED | VC6 build on current host does not qualify actual NT4, ownership, budgets and runtime. |

These tables overlap. Their row sum is not a mandatory-gate denominator. Full applicable Browser/governance inventory and product Debug/Release/NT4 validation remain pending; no complete Phase6 closeout is declared.

## Secure Principal and Legacy

Secure transport is TLS1.3/mTLS with role-correct trust/name/ALPN; workload grants remain separate. Legacy is plaintext/lower assurance with server-owned admission, grants, destination policy, quotas, audit and revocation. Functional parity target YES; cryptographic parity NO. Current Legacy Network Egress v1 denial (ADR-0028) is not a permanent architectural ban. D9A bounded opt-in lab architecture does not authorize production or C7 experiment execution. D9B remains proposed. No blanket Legacy functional PASS is inferred from historical explicitly scoped tests.

## Focused C0 recovery evidence

Browser was compiled in the isolated snapshot with its unchanged vendored PST0.6.4, VC6 x86 /MD. Accelerator fixture is governed normal PST0.6.5 API3/SPI4 Debug, executable SHA256 CDCB1025B524DFBAE236795C64EAE227D9483335A906266A4557B77D8F6CBB55. This is interoperability evidence, NOT Browser API3 or native-lifetime recertification.

The previous fixture Stop failure did not reproduce with the qualified fixture: it exited normally/gracefully. A separate harness mismatch was observed: terminal type13 NETWORK_CLOSE, payload0/reason0, no duplicate close, no reuse, but runner expected HALF_CLOSE. ADR-0023 case A requires CLOSE and prohibits TLS_REQUIRED half-close. Only that test-only expectation was corrected. Negative hostname passed with zero local fallback. Truncation initially failed solely because the chosen complete-CL fixture emits papacc-complete! while runner compared the clean-mode body; exact-byte expectation corrected, all terminal/no-reuse/no-fallback assertions retained. Original failed runs preserved.

| Run | Client exit | Fixture exit | Owned processes left |
| --- | --- | --- | --- |
| c0-final-positive-20261011-165234 | 0 | 0 | 0 |
| c0-lifecycle-20261011-164810 | 16 | 0 | 0 |
| c0-name-negative-20261011-165036 | 0 | 0 | 0 |
| c0-terminal-20261011-164929 | 16 | 0 | 0 |
| c0-terminal-20261011-165025 | 0 | 0 | 0 |
| c0-truncation-fixed-20261011-165207 | 0 | 0 | 0 |
| c0-truncation-negative-20261011-165039 | 16 | 0 | 0 |

Final clean positive and truncation negative PASS; hostname negative PASS. Focused VC6 compile/link emits zero warnings. Existing C0 endpoint/Mode A evidence unchanged, not rerun. Full product Debug/Release and actual NT4 NOT EXECUTED. Process exits and hardcoded counters are not proof of per-lifetime native destruction or Model E disposition.

## Preservation and next execution

All original Browser files and its full WIP status reverified against baseline.json. Isolated source has no .git; no isolated HEAD/diff claim. Three test-only edits preserve preimages, full hashes and inspected diffs in c0-checkpoints. git diff --no-index --check PASS; return1 with empty diagnostics denotes ordinary differences, not whitespace failure. No original functional source, accepted ADR, PST or wire changed. Production pin0.6.4; no promotion, commit/push/tag/release.

PST return handoff E34D555A578097C1787D249695EAA96EB5A87F1EF3C74397E6EEF1ABA7485B68 preserves accepted Accelerator API3 ADR-0032 qualification, Model E and 55 distinct lifetimes. R7 frozen FBCA33BD8A4E41F41B610C86A57DC7F1FF3042F59DBC46170BE4CD35ACF634A4 remains provenance pending, nonblocking for Browser; no replacement/supersession.

Next original implementation slice: C1, after finishing applicable governance reading and C0 scope/closure review. Implement only accepted nonblocking/readiness/wake/cancel/deadline/half-close/owner-close contract with proven NT4 primitives. Independently audit/migrate the isolated Browser consumer to API3 result-returning Model E before linking the staged normal NSS package: retain Connection/stream/Session/manager/provider/runtime until DESTROYED; retained disposition is not destructive completion. Do not blindly replace release symbols or clear retained context. PACC encoding needs a separate decision; C7 needs human prerequisite review; Legacy production needs accepted D9B. No new PST change is yet justified.

See companion JSON for exact source/trace/artifact/edit SHA256, statuses, dependencies and run identities.
