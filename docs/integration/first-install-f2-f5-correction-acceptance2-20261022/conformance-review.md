# F2/F5 correction: exact review and STOP

Task PACC-PHASE6-FIRST-INSTALL-F2-F5-CORRECTION-ACCEPTANCE-2; 2026-10-22.
main/HEAD 913c90cf2328f7939d8f5b927640601903ac6e91 unchanged.
No accepted record or functional patch produced. Original proposal/correction preserved.

## Separation achieved and remaining ambiguity

The proposed F2 correction correctly distinguishes externally authenticated full-request decision (A) from independently authenticated authorization scope plus protected local association (B). It forbids creating competence or broadening that scope. The proposed F5 correction preserves atomic one-time consumption, replay denial, no automatic rearm, Stop and retained UNKNOWN. These parts require no corporate product, Active Directory, Configuration Manager, cloud, new CA or prior CURRENT.

A material coherence point remains in original F3, unchanged by the proposed patch:

> A mesma decisão vincula a configuração validada e suas referências protegidas.

In profile B, the external decision may authorize a bounded class of configurations while the adapter binds the exact configuration. The sentence does not say whether it means external source decision, the protected aggregate, or final Accelerator admissibility. Automatically treating those as equivalent would weaken the separation requested in this task. The previous correction omitted that necessary cross-reference; the adjacent minimal replacement addresses it without changing target custody or P1.

F2 also does not explicitly distinguish a source-issued approval identifier from a protected policy/rule reference when no individual approval is issued; original F2 says origin/revision/status but does not state a factual revocation result separately. A local nonce must never masquerade as a source-issued approval ID, nor may absent revocation evidence become NOT_REVOKED. The adjacent addendum closes the source-versus-adapter projection explicitly. These are documentary corrections still PROPOSED, not a finding that any deployed source satisfies them.

## Required projection

Source authenticates authorizer/competence, operator or authorized operator predicate, target scope, NEW_INSTALL purpose, allowed configuration scope, source reference/revision, applicable validity, and status/revocation evidence. It authenticates the full request only in profile A. Adapter derives no new authority; it authenticates/correlates native operator and retained parent, tests every request element against protected source scope, and binds exact configuration/invocation/nonce only where profile B permits it. Accelerator makes the final admissibility decision, including P1, custody, Stop and ED11. Source approval or association alone is not final admission.

## Scenarios

Individual/local: F1 permits a legitimately designated owner/administrator and preexisting independent native administrative policy. It does not define how ownership/designation and competence are proven for an ordinary standalone installation. Token, elevation, local ACL, possession of equipment and CLI confirmation remain insufficient alone. No particular source/ceremony was supplied or selected. LOCAL_INSTALLATION_PROFILE=NOT_DEFINED_NOT_QUALIFIED_DENY. A future focal local ceremony must identify the actual independent proof and fault model; do not invent a second approver or enterprise service as a universal requirement.

Institutional: an existing authenticated deployment-approval process remains a candidate class, with exact scope/provenance/validity/revocation and local binding needing qualification. No deployment is presumed available and no source selected. INSTITUTIONAL_INSTALLATION_PROFILE=CANDIDATE_ONLY_NOT_QUALIFIED. An institution's product is optional, not a portable-core dependency. No new external documentation query or operational network was needed for this review.

The absence of a qualified source is distinct from these wording gaps. A generic contract can deny unsupported local targets; it cannot claim to have completed their actual ceremony. No universal impossibility or mandatory corporate infrastructure is inferred.

## Lifecycle, compatibility and ED11

F4/F5/F6 event/context validity, one live act, indivisible consume, invalidation, no retry/rearm, quiescence, restart denial and UNKNOWN retention remain intact. Identity of the child still follows exclusive native creation; before that only retained parent/scope/relative name exist. CA custody/init remains separate under issuer clause6; no key generation or borrowing authority is conferred. Initial bootstrap needs no prior CURRENT. No historical formats, root, worker or pins change.

Proof/association objects, source references, token/target/CA, buffers, stack/CRT/native costs and retention remain subject to complete ED11 accounting. No budget or opaque-resource bound is proved here; UNKNOWN is not zero. ED11 PARTIAL, unchanged quotas and 0.6.4. Builds/tests/effects NOT_RUN: document review only. Exact acceptance and executor write approval must wait for the remaining wording correction; no registration was attempted.
