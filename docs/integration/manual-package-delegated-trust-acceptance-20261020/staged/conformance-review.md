# Conformance review - delegated package signer trust

Task PACC-PHASE6-MANUAL-PACKAGE-DELEGATED-TRUST-ACCEPTANCE-1. main at c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Review only: no source change, build, test, network, package/key/certificate creation or private signature. Original proposal, all accepted records and previous evidence remain byte-exact.

## Acceptance disposition

ACCEPTANCE PENDING. D0-D2/D4-D6 conform at the normative boundary, with their stated implementation/environment/codec/ED11 gates. D3 retains a material FIRST-CONTACT decision: its second paragraph assumes an independently authenticated administrator, and its third paragraph says the confirmation action, owner, source transport and status/freshness policy remain to be identified. These are not merely tests of an already selected mechanism. They determine the basis on which a new Browser accepts a previously unknown signer.

Exact original D3 paragraph3:

> Before implementation, its exact existing confirmation action, owner and source transport must be identified and demonstrated.

It additionally requires an explicit applicable status/freshness policy without selecting one. Section4 of the current task expressly requires dependent acceptance to remain pending when that ceremony needs an additional normative decision. Therefore no ACCEPTED record or partial acceptance of D1 alone is created. This is an incomplete proposal, not a finding that same-root delegation contradicts accepted architecture.

## Conformance matrix

|Clause/requirement|Authority and finding|Disposition|
|---|---|---|
|D0/M1 sequence|M1 requires authenticated independent approved signer before KDF; corrected sequence verifies structure/signature/delegation before derivation and releases only after all checks|CONFORMANT; no implementation inferred|
|D1 policy authority|Same canonical root policy explicitly approves exact issuer PACKAGE record/key; issuance itself grants no delegation; no second root/registry/journal|CONFORMANT_SEMANTICS; actual policy representation/versioned persistence not approved here|
|D1 identity/scope|Existing request/record, CA authority/generation, exact certificate digest/SPKI, package version/suite/domain and root/server scope are bound to factual administrative revision|CONFORMANT; no concrete trusted signer instantiated|
|D1 CURRENT/revocation/validity|Factual committed/published approval required; cert and delegation validity/status separate; missing facts deny|CONFORMANT; status evidence must still reach consumer independently|
|D2 lifetime/ownership|Read-only immutable view under existing owner/pin/lease; historical pin is not CURRENT; no authority boolean|CONFORMANT; concrete lifetime proof remains an implementation gate|
|D2 purpose/CA separation|Technical profile1.3 requires dedicated codeSigning-only signer, not CA/TLS/client key; chain/RSA/EKU alone are insufficient|CONFORMANT|
|D3 first Browser identity basis|M1 allows independently authenticated full fingerprint/server confirmation, ADR0006 forbids silent TOFU; D3 does not yet select the independent authentication procedure|MATERIAL_GAP; cannot accept by interpreting a string or local UI click as independent trust|
|D3 signer purpose/delegation evidence|M2 has public signer SPKI but no signer certificate; ZIP has ca.der/cliente.der/cliente.pk8/config.bin/issuance.bin. Those certificate roles cannot be reinterpreted as signer-purpose evidence|MATERIAL_GAP; determine whether independently recognized admin attestation is sufficient approved context or another existing independent public evidence source is required|
|D3 local installation authority|RSA correctness, accepted signer identity and permission to replace BrowserProfile configuration are separate. D3 does not explicitly select the local approval step/actor|CORRECTION_REQUIRED; preserve existing profile policy, no inferred admin powers|
|D3 freshness|An offline package or CA signature cannot prove latest delegation/revocation status. Original D3 defers the policy|DECISION_PENDING; no invented TTL/current/fencing or mandatory network|
|D4 quarantine/Stop/retention|Whole-result release only after all checks; hold references/credits through factual quiescence/uncertainty; no timeout destruction or destructive retry|CONFORMANT; complete integration unqualified|
|D5 ED11|Explicit Windows3MiB/default2MiB remain per-process limits, not per-client quotas; provider/kernel UNKNOWN; no credit from fitting estimate|CONFORMANT; operational reservation NOT_APPROVED|
|D6 acceptance boundary|Owner choice and official executor review separate from source qualification/activation; proposals/history preserved|CONFORMANT; no functional patch approval|
|Browser compatibility|Permanent manual import plus advanced separate-file import converge on existing candidate/generation/reference/store/leases/ActiveConfiguration; no PapinhoTrustStore or Win32/CNG obligation in common contract|PRESERVED; Browser-specific contract/implementation remains its own governed work|

## Three distinct claims at first contact

1. Mathematical signature: proves that the supplied key verifies the signed bytes. The embedded key/hash has no independent identity authority.
2. Signer identity and delegation: requires independent approved full-SPKI reference plus purpose/scope/status facts. A CA included with client credentials cannot authenticate itself or delegate an unrelated signing role.
3. Local installation authorization: must come from an operator already allowed to change that BrowserProfile, after complete validation. Neither remote administrative approval nor password possession grants that local right. Successful validation is not Apply/commit.

The freshly installed Browser need not have prior issuer trust, a prior CSR, or a second mandatory material file. M1 allows an independent human confirmation path. However the independent identity/source and purpose evidence must be selected explicitly; the package contents and an unauthenticated connection cannot establish them. Original D3 leaves that substantive selection open.

## Smallest correction prepared, not accepted

D3-CORRECTION-PROPOSED.md and exact-D3-correction.patch replace only original D3 paragraphs2-3. D3 paragraph1 and all other clauses stay intact. The proposed concrete minimum is an in-person handover by an administrator whose identity/role is already recognized independently, reading the actual same-root approval and current status. It proposes that explicit administrative attestation plus full fingerprint comparison can establish the first approved signer context, without claiming that Browser locally validated an unseen signer certificate. This human trust assumption is NEW PROPOSED decision text, not inferred from M1 or already authorized by this acceptance task.

It also proposes same-operation contemporaneous status confirmation with no cached-reuse privilege or numerical TTL, and separates later local profile installation approval. If the owner requires different independent evidence or status policy, revise that specific text before acceptance. None of these proposed choices is applied to original D3, accepted as policy, implemented or tested. No transport, crypto suite, format, new persistent trust record or runtime source is added.

M1 already permits the class of independent confirmation; choosing its concrete authentication/attestation semantics is the missing decision. Selecting a UI transport and proving existing owner/lifetime later are implementation qualifications; they must not be used to hide an undecided trust basis. An explicit acceptance must state whether the proposed human attestation is an allowed source of purpose/delegation evidence, or select another governed source. Until then all dependent acceptance and validation deny.

## ED11 reconciliation - no new measurement

This review adds no production storage/copies/contexts/threads: source delta0. The proposed trust context must eventually account for exact SPKI/reference, cert/delegation/status evidence, immutable input, fingerprint/digest scratch, signature/provider/KDF/key state, password, AEAD quarantine/plaintext/ciphertext, ZIP/TLV views, validated material, candidate old/new coexistence and retained borrowers/credits. Borrowed aliases are counted once; actual extra copies and retained state cannot be omitted. No numeric maximum for unimplemented trust metadata is fabricated.

Prior known selected base2103847 and legacy2067655 are not full operational maxima. Prior test peak1290354 includes a fixture-only global signing context; private-adapter/material phase569962 is not a complete runtime peak. Conservative selected base+full-test peak3394201 exceeds3MiB by248473 before caller buffers. It is a DENY scenario, not proof of deployed usage; attribution/shared provider/fixture differences remain unqualified. No repeated measurement/build/test in this task. WINDOWS_LIMIT3145728 applies only by explicit selection; default/legacy2097152, individual quotas and all credits remain unchanged. Provider/kernel and full transitive stacks UNKNOWN; ED11 totalPARTIAL, activation/operational reservationDENY.

## Historical evidence and cross-project boundary

Reviewed local ADR0006, accepted manual provisioning/custody/CA profile, frozen M0-M8 and narrow metadata incorporation, issuer/C2/root transaction/pins and ED08/ED11 retained-resource boundaries. Browser ADR0007 permanent provisioning/candidate Apply and safe-retirement sections preserve advanced import, authorized Profile changes, immutable generations, leases and conservative retention. Browser files are read-only references; this Accelerator review cannot amend them. No Browser local-TLS trust debt or SYSTEM trust is used to bootstrap package signer trust.

Source SHA256, exact replacement offsets and candidate bytes are in normative-preimages.json. No accepted ADR is modified and the original proposal is preserved. Official review of this documentary result is not acceptance of the new ceremony or approval of functional patches.
