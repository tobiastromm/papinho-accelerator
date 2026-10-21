# C2 revision2 closeout and progressive protocol governance

Task: PAPINHOACCELERATOR-PACC-C2-REVISION2-CLOSEOUT-AND-PROTOCOL-GOVERNANCE.
Date: 2026-10-11. Documentation-only review completed.
Semantic gaps resolved AS PROPOSED CANDIDATE; wire approval/implementation absent.

## Baseline and preservation

Current main HEADs: Accelerator c50b8fb9a7e3293b5e6355fc08459bf85b56857a;
Browser1eb028554f622672377953225e162ed9b6564a40. Dirty WIP preserved.
Isolated Browser source has no .git; no independent HEAD claimed.
Prior review inputs/hashes and original revision1/revision2/spec0.1 unchanged.
Seven C1 qualification hashes, normal SDK53 hashes and seven shared codecs
reverified; no qualification rerun. C0/C1 remain qualified only for recorded
endpoint/component/current-host scopes; no productive Mode C or real NT4 claim.
Model E normal/test-only evidence stays distinct and scoped.

Read baseline: AGENTS, ADR-0022/0029, existing PACC framing/CONTROL/DATA/
capability/security/Principal/Legacy contracts, prior joint review/JSON,
full revision2 and human draft, Browser C0/C1/Model E handoffs.
Unchanged accepted contracts/read evidence from the prior review remain applicable.
No accepted ADR modified; no functional Browser/Accelerator/PST/PACC change.

## Contract closeout and deliverables

[Revision3](../future/pacc-c2-versioned-contract-proposal-revision-3.md)
resolves revision2's semantic alternatives and exposes trade-offs. It is a
new proposed candidate, not owner-approved policy or a replacement design.
The [separate ADR candidate](../adr/ADR-CANDIDATE-pacc-c2-operation-and-progressive-specification.md)
records decisions/rationale; it does not contain the whole specification.

| Boundary | Proposed resolution | Acceptance/evidence remaining |
| --- | --- | --- |
| Session/grants | Dedicated C2 two-key Session + preserved existing-service Session, one owner | Owner review, consumer implementation and resource/native tests |
| CONTROL | Per-state/type finite maxima; reserved notifications; bounded priority/deadline | Grammar-derived numerical bounds and encoding approval |
| Partial revocation | Local owner serialization; irreversible escaped prefix; abandon incomplete DATA | Security review of observation latency, byte-offset negatives |
| ACK | Role claim/write/peer validation/activation/TLS-ready distinct | Exact schemas, partial/lost ACK and wrong-generation tests |
| Fences | Four directional role prefixes; matching downstream ACKs; no global socket order | Counts/enums/layout, finite drainage and required fence failure matrix |
| Terminal/Model E | TLS subfact not clean operation outcome; CONTROL final outcome; retention preserved | Exact cause/action/disposition grammar and per-lifetime evidence |
| Compatibility | Old IDs1-15/bytes untouched; no third offer key or silent growth | Allocations/package metadata and old/new goldens |
| Secure/Legacy | Principal+Session+role binding; explicit lower assurance | Token entropy/limits/security review; no D9B production enablement |
| Site TLS vs egress | Mode C needs TLS/operation, not remote NETWORK_EGRESS | Local/all-candidate policy integration and zero public site dial proof |
| Budgets/backpressure | Four queues plus staging/CONTROL/retained accounting | Numeric encoding bounds, measured caps and NT4 evidence |
| Progressive specification | ADR-linked clauses/diagrams/planned conformance | Owner acceptance; encoding/codecs/vectors/runtime consistency later |

There is no newly established conflict between accepted ADRs. A real conceptual
ambiguity was clarified: revocation on independent CONTROL/DATA cannot promise
instantaneous peer knowledge. The candidate defines each owner's actual
linearization/observation and preserves escaped bytes; this is for security/
owner approval, not a silent weakening of existing oracles.

[Draft0.2](../protocol/pacc-c2-protocol-specification-draft-revision-0.2.md)
updates the human specification, original sequences/state conditions and adds
Session/revocation/fence diagrams. Existing CONTROL/DATA vectors remain the only
byte examples. [Conformance and encoding plan](pacc-c2-revision3-conformance.md)
maps22 documentary checks to R3-I01-I12 and original P/N/R2 oracles; none executed.

[Progressive PACC ledger](../protocol/pacc-progressive-specification-ledger.md)
keeps existing normative fragments and the new candidate separately traceable.
[Global ecosystem principle](../future/papinho-ecosystem-protocol-specification-governance-proposal.md)
is a forwardable proposed policy for PapinhoEngineering, NOT global accepted.
No central repository edit or external message was performed.
[Owner handoff](../integration/pacc-c2-revision3-wire-approval-handoff.md)
presents concrete semantic/security/encoding decisions without permission to code.

## Evidence and qualification boundaries

Document cross-links, version/status labels, ADR/spec/invariant/test mappings,
unchanged old vectors and before/after SHA256 are validated.
This is DOCUMENTARY consistency only. Complete C2 encoding, codecs/goldens,
independent-peer compatibility and native/NT4 qualification are NOT READY/NOT RUN.
Missing exact IDs/widths/layouts/token parameters cannot be guessed to report
WIRE_ENCODING_READY=YES. Global governance acceptance cannot be inferred.

Only new documents added. Full pre-existing nonignored file hashes/status and
isolated visible files preserved; companion audit JSON records exact inputs,
outputs and counts. New-file whitespace checks and default git diff --check
pass; existing Git line-ending notices are not compiler warnings.
No build or runtime scenario, E1-E12, credential/config/HKCU, commit/push/tag/
release or PST promotion. No test processes launched.

## Result

CURRENT_TASK_COMPLETED=YES
SLICE_COMPLETED=YES
C2_CONTRACT_STATUS=SEMANTIC_CANDIDATE_RECONCILED_OWNER_REVIEW_PENDING
REVISION=3
SESSION_GRANTS=RESOLVED_PROPOSED_BOUNDED_SEPARATE_SESSIONS_ONE_OWNER
CONTROL_LIMITS=RESOLVED_PROPOSED_STATE_TYPE_BOUNDS_ENCODING_MAXIMA_PENDING
PARTIAL_SEND_REVOCATION=RESOLVED_PROPOSED_LOCAL_LINEARIZATION_ESCAPED_PREFIX
ACK_SEMANTICS=RESOLVED_PROPOSED_DISTINCT_WRITE_RECEIPT_ACTIVATION
OPERATION_FENCES=RESOLVED_PROPOSED_SCOPED_PREFIX_AND_DOWNSTREAM_ACK
PACC_SPECIFICATION=DRAFT_0_2_ADR_LINKED
PACC_ADR_PROPOSAL=PROPOSED_NO_CANONICAL_NUMBER_ASSIGNED
RFC_DIAGRAMS=UPDATED_VERSIONABLE_PROPOSED_SEQUENCES_AND_STATES
SPEC_ENCODING_TEST_CONSISTENCY=DOCUMENTARY_PASS_ENCODING_RUNTIME_PENDING
ECOSYSTEM_PROTOCOL_PRINCIPLE_PROPOSAL=PREPARED_PROPOSED_CENTRAL_REVIEW
WIRE_ENCODING_READY=NO
C2_IMPLEMENTATION_AUTHORIZED=NO
C3_IMPLEMENTATION_AUTHORIZED=NO
C0_STATUS=PRESERVED_QUALIFIED_FOR_SCOPE
C1_STATUS=PRESERVED_QUALIFIED_INTERNAL_CURRENT_HOST
PST_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
FIRST_UNRESOLVED_DIVERGENCE=EXACT_ENCODING_SECURITY_PARAMETERS_AND_OWNER_ACCEPTANCE_PENDING
NEXT=OWNER_SEMANTIC_REVIEW_THEN_EXACT_VERSIONED_ENCODING_SECURITY_DECISION
