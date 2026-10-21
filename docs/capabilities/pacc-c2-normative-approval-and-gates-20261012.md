# C2 Revision 3 normative approval and independent gates

2026-10-12. Current authority: [ADR-0034](../adr/ADR-0034-pacc-c2-revision3-contract-and-separated-gates.md), [normative specification1.0](../protocol/pacc-c2-specification-revision-1.0.md), [frozen hashes](../protocol/pacc-c2-contract-freeze-20261012.json). Previous review HOLD statuses remain historical, not current contract blockers. They never become runtime PASS by this update.

## Four states and causal dependency

| State | Current result | Entry/exit authority |
| --- | --- | --- |
| CONTRACT_APPROVAL | ACCEPTED | Owner authorized this conditional-closeout task; exact grammar/security/phase policy determined and documentary checks passed |
| IMPLEMENTATION_AUTHORIZATION | NOT_GRANTED; contract basis READY | Separate owner task naming scope, target, packages, security utility admission and gates |
| IMPLEMENTATION_QUALIFICATION | PENDING_IMPLEMENTATION | Actual codecs, initialized owner and target positive/negative evidence; no fixture promotion |
| PRODUCTION_ENABLEMENT | NOT_GRANTED | Separate production decision after all applicable implementation, security, resources and target gates |

Contract approval -> separate implementation authority -> build/execute implementation gates -> separate production authority. ED08 real-owner execution is a required outcome of implementation, not a prerequisite to approve the contract defining that owner. Source-pin/compiler checks for secret utilities precede integration of secret code; total target evidence precedes successful runtime admission/enablement. Missing proof is an explicit denial condition, never a fictitious positive fact.

## ED01-ED14 disposition

Each ACCEPTED entry means contractual decision, not implementation qualification. Original P/N, R2-T, CG/EW, ED08 EP and target resource oracles remain mandatory; IDs below are documentary only.

| ED | Accepted decision | Mandatory implementation/target evidence |
| --- | --- | --- |
| ED01 | Capability3/major1 requiring2/1; types16-27; old registry immutable | Canonical source/package registration matches approved allocation; API/package shipping revision separately approved |
| ED02 | Dedicated two-key C2 Session and independently authorized old-service Session under one bounded owner | Both old/new peer compatibility directions; missing grants cause zero effects; loss isolation and retained Session quota |
| ED03 | Exact twelve mandatory layouts, enum/bit/reserved/length/endian rules; fourteen illustrative byte vectors | Independent codec goldens, all min/max/overflow/reserved/trailing/split/coalesced/state negatives; VC6/MSVC |
| ED04 | DNS/IPv4 first major; canonical DNS SNI/name, immutable Browser tuple and no re-resolution | Endpoint substitution/provenance races/all-candidate Browser policy; no Accelerator public site egress |
| ED05 | Protocol-only request/op/generation/epoch identities, checked exhaustion and nonreuse | Cross-Session/Principal/op/role/stale/restart/callback/reuse negatives |
| ED06 | Modern server OS CSPRNG, independent32-byte role tokens, vetted full comparison/non-elidable erasure, no fallback and bounded pre-auth abuse | Pin exact utility source/hash/license/recipe before secret-code integration; compiler output and wipe tests; RNG/zero/duplicate/borrower/late claim/abuse negatives |
| ED07 | Exact positive public API3 CLIENT TLS/name/chain/ALPN facts captured before shutdown; unknown denies | Exact normal SDK capabilities/runtime queries; wrong trust/name/TLS/ALPN, partial fact and revoke races |
| ED08 | Normative spec11.2 audited closure partial order; COMPLETE != DESTROYED != OUTPUT_DRAINED | Both initiators, EOF-less site, pending/partial/stalled output, exact four prefix ACKs, no premature seal, real owner R4 and publication authority |
| ED09 | Local revocation, explicit irreversible lower-layer accepted prefix, no unauthorized suffix/splicing | Cancel at every boundary; actual attempted/authorized/accepted/delivered/rejected facts and borrowed-buffer lifetime |
| ED10 | Server FINAL outcome distinct from Browser native destruction; first cause and retained disposition immutable | Per-lifetime result/disposition/destruction; remote-clean/local-retained, duplicate/lost/partial summary, late callback |
| ED11 | Managed2MiB controlled limits; fail-closed checked reservation/rollback/host credits/Session lifetime/CONTROL reserves/fairness and retained quota | Real composed metadata/controller bounds; full baseline/increment/peak/provider/kernel/shared/retained ledger and target margin; real NT4 if claimed;20/32 and2/8MiB totals remain candidates |
| ED12 | Browser Internet ownership; Site TLS authorization independent from Network Egress | Actual Browser site socket and exact private Accelerator C3 tuple, policy deny before effects |
| ED13 | Secure Principal admission distinct from explicit Legacy lab lower assurance; no production Legacy grant | Separate listeners/profiles/isolation, no downgrade/cross-principal attachment; separate lab/production execution approvals |
| ED14 | Frozen ADR/RFC/layout/vector revision consistency and progressive documentary updates | Same revision in codecs/goldens/independent peer and native/target tests; runtime proof never inferred from document checks |

## Mandatory independent implementation gates

| Gate | Preconditions / required proof | Current status |
| --- | --- | --- |
| IG01 Encoding and compatibility | Accepted hashes; exact registry/layout/vector/codecs; existing-service semantics and both compatibility directions | PENDING_IMPLEMENTATION |
| IG02 Security utility and admission | Source/license/hash/portability and optimized output before secret-code integration; immutable Principal/Session/grants/token claim; all security negatives | PENDING_IMPLEMENTATION |
| IG03 Public TLS facts | Exact normal package CLIENT capability and successful public facts; no private metadata fabrication | PENDING_IMPLEMENTATION_TARGET |
| IG04 Real owner clean ED08 | Initialized real owner/manager/roles/C3, factual prescribed phases, four exact ACKs and separate Browser cleanup | PENDING_IMPLEMENTATION |
| IG05 Real owner retained R4/publication | Governed current TEST_ONLY R4 package; release result/disposition, retained leases/query-only state, explicit rejected callbacks, queue/CONTROL failures and no false clean | PENDING_IMPLEMENTATION |
| IG06 Controlled resource implementation | Managed allocation census, metadata/callback/timer/staging/controller; real rollback, Session lifetime, retained quota, CONTROL fairness and host credit | PENDING_IMPLEMENTATION |
| IG07 Total/target resource enablement | Full composed normal and retained topology, pre-auth inputs, provider/kernel measures/bounds and approved target metric/headroom/freshness; deny unknown mandatory categories | PENDING_IMPLEMENTATION_TARGET |
| IG08 Integrated ecosystem and release | Separately authorized Browser integration, NT4 when claimed, E1-E12 when authorized, public API/package/release and production decision | NOT_AUTHORIZED |

IG04/IG05 must record actual callback origin, queue admission/dequeue, authority decision, transport acceptance and peer delivery distinctly. Missing callback/attempt is not rejection proof. Real R4 results must come from governed provider control; fixture guard, synthetic consumer result and real Winsock failure remain distinct classes. No delayed grant/FINAL permits new DATA after local revocation.

## Evidence preserved and caveats

Twenty R4/current-normal component runs and28 historical ED08 runs remain valid in their original scopes, not IG04/IG05 full owner execution. ED11 managed policy's50 checks in each of3 builds remain isolated model conformance, not total composed/runtime qualification. Browser C0/C1 handoffs retain endpoint/internal site-pump scope; no full Mode C or NT4 status is inferred. PST BIO/R4 handoffs are test-only and do not replace normal packages or native-failure class.

Human-readable RFC contains full grammar, byte examples, sequence and state diagrams and normative guards. Historical drafts0.3/0.3.2/0.3.3/0.3.4 and candidates are preserved. Functional PACC registry/codecs still implement existing wire only. New normative ID allocation does not make a peer eligible before future complete negotiation/grants.

CURRENT_TASK_COMPLETED=YES
SLICE_COMPLETED=YES
C2_CONTRACT_APPROVAL=ACCEPTED_DOCUMENTARY_CONTRACT
C2_ADR_STATUS=ACCEPTED_ADR0034
C2_ENCODING_STATUS=APPROVED_EXACT_ENCODING1
PACC_REGISTRY=CAPABILITY3_MAJOR1; TYPES0x0010_TO_0x001B; NORMATIVE_ONLY
ED08_CONTRACT_STATUS=ACCEPTED_EXACT_AUDITED_PARTIAL_ORDER
ED08_REAL_OWNER_QUALIFICATION=PENDING_IMPLEMENTATION
ED11_POLICY_STATUS=ACCEPTED_CONTROLLED_POLICY; TOTAL_ENVELOPES_CANDIDATE
ED11_TOTAL_MEASUREMENT=PENDING_IMPLEMENTATION
IMPLEMENTATION_AUTHORIZATION_READY=YES
C2_IMPLEMENTATION_AUTHORIZED=NO
C3_IMPLEMENTATION_AUTHORIZED=NO
PACC_SPECIFICATION_STATUS=ACCEPTED_REVISION1.0; DRAFTS_PRESERVED
SPEC_ENCODING_TEST_CONSISTENCY=DOCUMENTARY_BYTE_EXACT_PASS; CODEC_RUNTIME_PENDING
PRODUCTION_PIN=0.6.4
PST065_PROMOTED=NO
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
FIRST_UNRESOLVED_DIVERGENCE=NONE_IN_CONTRACT; IMPLEMENTATION_TARGET_GATES_OPEN
NEXT=SEPARATE_OWNER_AUTHORIZATION_FOR_C2_C3_IMPLEMENTATION_WITH_IG01_TO_IG08
