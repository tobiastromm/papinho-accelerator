---
status: accepted
revision: 1
decision-date: 2026-10-20
scope-level: project
scope-target: exact manual package policy delegation and private root version3 representation
decision-makers: [Tobias Tromm]
supersedes: []
---
# Package signer delegation and persistence — F1–F5 / G0–G7

## Authority and exact incorporation

Owner task PACC-PHASE6-PACKAGE-DELEGATION-PERSISTENCE-ACCEPTANCE-1 conditionally authorizes exact acceptance of F1–F5 and G0–G7, subject to conformity review and official executor approval. This designated record becomes effective only after official approval of these exact bytes and post-write hash verification. No functional patch or operational permission follows from acceptance.

Accept ONLY F1-F5 from `docs/integration/manual-package-factual-trust-integration-20261020/SOURCE-EXTENSION-PROPOSED.md`, whole-source SHA256 `9387d8ef45316aad15b0bc336fa693ef476fe5420f82ace9576794d22cea1cbd`, byte interval[502,8808), length8306, clause SHA256 `7d8ca8636b91423e980577a555a63f7828f69961e248cc2f32fb896a94070c85`. Exact clause bytes are incorporated through [F1-F5-frozen-clause-bytes.txt](../integration/package-delegation-persistence-acceptance-20261020/F1-F5-frozen-clause-bytes.txt), with no encoding or line-ending normalization. Source frontmatter, title and preparatory introduction are excluded; original proposals remain unchanged.

Accept ONLY G0-G7 from `docs/integration/manual-package-f1-f5-acceptance-20261020/PERSISTENCE-LAYOUT-PROPOSED.md`, whole-source SHA256 `d59815b2028188714a5f96d7b84aed20aa203edafac5f50f73b2d58773acde39`, byte interval[191,17490), length17299, clause SHA256 `ec17ecb182d81b7535a4ad9853d69768120d6895434ae75b890744f929403307`. Exact clause bytes are incorporated through [G0-G7-frozen-clause-bytes.txt](../integration/package-delegation-persistence-acceptance-20261020/G0-G7-frozen-clause-bytes.txt), with no encoding or line-ending normalization. Source frontmatter, title and preparatory introduction are excluded; original proposals remain unchanged.

G2's offset-matrix reference is pinned ONLY to `docs/integration/manual-package-f1-f5-acceptance-20261020/offset-matrix-proposed.csv`, SHA256 `748193b82bf50485bfb3f56267b68ce4383746d066a0a3d3d402fb1f316ba61f`, length3517. Its exact bytes are incorporated as [offset-matrix-frozen.csv](../integration/package-delegation-persistence-acceptance-20261020/offset-matrix-frozen.csv), interpreted with the fixed values, dynamic-region formulas and hash domains of G0–G7. No future edit is accepted by reference.

The inherited T0–T6 Revision2 rules mean the existing accepted record `docs/adr/DECISION-server-tls-identity-persistence-revision2-20261020.md`, SHA256 `c67fec877f602d9122680c6e0a685b2e653fa2220e24930d6142d2e3b22229cb`, including its already frozen components/offset reference. This registration does not modify that record or its formats.

## Delimited precedence

Historical PROPOSED/not-accepted wording inside the incorporated clauses is superseded ONLY as to acceptance of these exact semantic and binary contracts. All prohibitions on implementation, publication, private signatures, recovery, admission, qualification and resource reservation remain effective. F5's prohibition of inferred layouts is satisfied specifically by the jointly accepted G0–G7 and frozen matrix; no other layout/API/field is accepted.

G1 expressly selects the first private version3 subset: D0..K<=N<=4, at most one delegation per Secure listener. F1 alone had selected no cardinality; this specific joint decision does so ONLY for that representation, not for individual client quotas or historical formats. Insufficient representability is UNSUPPORTED, without truncation, first-match selection or fallback. Actual target support remains independent and may still reject N>1.

Precedence is limited to NEW PACROOT8/version3, PACPDG01/PACPDL01/version1 and PACR8CP3/version3 bytes and source/consumption obligations. ROOT08/v2, CP2, version1/CP1, ROOT06/BODY0.5 and earlier formats, SNT1, LST1, issuer serial/history codecs and PACCMP02 remain byte-exact. New version3 defines its own formerly reserved header interpretation explicitly; it never authorizes writing those fields under version2. Version3 is EXISTING_ROOT_UPDATE only from factual version2/version3 predecessor with complete checkpoint, CURRENT and pin; no bootstrap, silent migration, downgrade dropping delegation, new root, registry, trust store, journal, worker or authority.

D3 first-contact trust, M0–M8 cryptography/package/custody, internal CA profile1, S1–S5, root/C8 and issuer/C2 meanings retain authority. PDG1 is public policy data of the same root, not a second source of certificate status or signing authority. Certificate issuance, purpose PACKAGE, codeSigning and valid RSA never by themselves delegate trust. Certificate and delegation validity/revocation/status remain distinct and mandatory.

## Three authorities and mandatory order

The Accelerator may produce only factual source observations under actual canonical policy/issuer/TLS/CURRENT and protected lifetime. The recipient's independent D3 ceremony belongs to its existing operation/owner; the Accelerator cannot certify the recipient's recognition or local installation right. Local BrowserProfile authorization to install follows complete authenticated validation and is separate from both the server's delegation and the recipient's trust confirmation. No local confirmation or reusable attestation is persisted as root authority; no new packet/member, provider or signed-attestation format is created. Browser sources/ADRs remain unchanged and require their own governed handoff/implementation.

STRUCTURE → SIGNATURE → TRUST → KDF → AEAD → CONTENT → RELEASE remains mandatory. Final applicability/status/lifetime revalidation remains required. Before TRUST, use the approved signer certificate/source to compare complete SPKI plus full fingerprint against the package; self-supplied key/password/hash is insufficient. Missing or uncertain policy, status, CURRENT, pins or independent confirmation means DENY before KDF. Quarantine and all confidential output remain private through full content/material/profile/key/identity validation. No partial success is authorized.

## Transaction, lifecycle and historical readiness

Same PREPARE → VALIDATE → factual durable COMMIT → CURRENT → EPOCH authority, exact full-predecessor/native-identity CAS, no CAS_CONFLICT/UNKNOWN fallback or destructive retry. R/A/P/I are distinct; no caller-selected epoch or publication from digest. P preservation still requires identical operational projection proof, not a label on delegation. CP3 records post-commit/post-publication facts only, never freshness, fencing or recovery permission. No serial reset/reuse, issuer data loss or active-Session policy change is authorized.

Existing owners retain snapshots/pins/source leases/immutable input/crypto buffers and credits until factual quiescence and closure. Root pins do not automatically protect external source objects. Stop/timeout/process exit/FINAL do not establish cancellation or DESTROYED. Retained slots are not overwritten; uncertain results retain their graph/credit. Model E, ED08 and exactly-once remain unchanged.

## ED11 and separate implementation gate

Serialized image20608..30592, PDG96..1632, header512, record384 and CP3512 are representation bounds, not reservations or evidence of fit. Independent full-buffer subtotals3E91776/4E122368/6E183552 and deltas4896/6528/9792 against version2 are hypothetical arrays only. Source/pin/history/observation/snapshot/retention/native/provider/stack coexistence must be censused before effects; no costs may be omitted or shared without proof. The earlier conservative fixture-provider scenario is not an operational peak or allowance. No quota/reservation is increased/reduced, default2097152 and explicit Windows3145728 remain per-process ceilings; provider/kernel UNKNOWN and ED11 total PARTIAL.

No source adapter, factual CURRENT, independent confirmation, code/target/native lifetime, operational storage reservation, signing isolation, backup/fencing or complete authenticated package validation is qualified by this record. Accepted layout is not implemented support. Later source patches require separate owner authorization, exact official executor review, preimages and appropriate offline/native qualification. No functional hunks are accepted by reference.

## Result and preserved limits

F1_F5_STATUS=ACCEPTED
G0_G7_STATUS=ACCEPTED
PACROOT8_V3_LAYOUT=ACCEPTED_NORMATIVE_ONLY
PDG1_CP3_LAYOUT=ACCEPTED_NORMATIVE_ONLY
DELEGATED_SIGNER_TRUST=DENY_UNTIL_IMPLEMENTED
BROWSER_FIRST_CONTACT=SEPARATE_LOCAL_AUTHORITY
IMPLEMENTATION_AUTHORIZED=NO
AUTHENTICATED_PACKAGE_VALIDATION=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION

No builds, tests, network, real packages/keys/certificates, functional changes, Browser/PST/wire change, production bootstrap/CURRENT/private signature, commit/push/tag/release or Phase6 closure is authorized or executed by this normative task.

## Evidence and history

[Joint conformance review](../integration/package-delegation-persistence-acceptance-20261020/conformance-matrix.md), [incorporation/preimages](../integration/package-delegation-persistence-acceptance-20261020/incorporation-and-preimages.json), [closeout](../integration/package-delegation-persistence-acceptance-20261020/closeout.md). Earlier partial reviews remain historically correct before this joint acceptance; no original proposal or prior accepted artifact is rewritten.

Revision1,2026-10-20: F1–F5 and G0–G7 plus exact matrix accepted through conditional owner authority and official executor registration only; operational gates unchanged.
