---
status: accepted
revision: 1
decision-date: 2026-10-21
scope-level: project
scope-target: Accelerator-only public TIME_SOURCE strict Ed25519 architecture
decision-makers: [Tobias Tromm]
supersedes: []
---
# Independent strict Ed25519 validation architecture

Owner task PACC-PHASE6-INDEPENDENT-ED25519-BACKEND-ACCEPTANCE-1 approves this preferred architecture subject to official review. Effective only after official executor approval of exact document operations and verified post-hashes.

Accept ONLY [frozen S0-S6](../integration/independent-ed25519-backend-acceptance-20261021/S0-S6-frozen-clause-bytes.txt), 2590 bytes, SHA256 250364259bdd76d59f230e9e9f928c9467d99dd4acb63be7a091034494bec8df, candidate byte range [523,3113). Original candidate SHA256 20037e4a3b471e778cd064c7478c75882672220191400af96417c7447ecbcf7e is preserved unchanged. Introductions, navigation, audit plans and functional patches are excluded. Future edits are not accepted by reference.

Preferred architecture: bounded Accelerator-owned pinned OpenSSL3.5.8 mathematical source subset, exclusively public strict TIME_SOURCE DATA validation. P0-P5 remains unchanged: canonical valid32-byte point, P != O, integer [L]P = O. No reduction of L, cofactor-clearing substitute or artificial signature test.

Source, recipes, distribution and maintenance belong exclusively to Accelerator. No PST modification, extension, internal export, build/runtime dependency or renamed object/DLL shortcut. Immutable upstream archive and retained licenses/attributions required. One private C89 boundary, no internal curve ABI, signing, keygen, generic framework or implicit authority.

Extraction needs separate authorization and exact patch approval: pinned source, preserved arithmetic, reviewed types/constants/public-data glue and independent symbol closure without EVP/provider/internal OpenSSL/PST dependencies. New component must pass modern Debug/Release, actual VC6 backend/caller, independent mixed-order vectors and differential regressions. Old296/2526/5940 proofs are oracles only; NT4/box/ISA remain unqualified.

S4 permits only a reserve after separate future owner decision: no libsodium port resumes or backend is selected. This specializes the implementation direction left open by predicate P4, without changing its mathematical/provenance/lifecycle/qualification gates. TS0-TS8, AT/PT, Q0-Q7, Model E and ED08/ED11 remain unchanged. Historical wire/layouts unchanged.

Existing synchronous protected borrows, ownership/pins, Stop, retention and factual quiescence remain required. No timeout release or DATA-to-authority promotion. Later ED11 measurement must include code/data, heap, full nested stack, initialization, buffers, retention, coexistence and UNKNOWN costs. No quota increase, presumed savings or source-count capacity claim.

CURRENT_TASK_COMPLETED=YES
INDEPENDENT_ED25519_ARCHITECTURE=ACCEPTED
BACKEND_SELECTED=NONE_UNTIL_QUALIFIED
IMPLEMENTATION_AUTHORIZED=NO
PST_MODIFICATION=DENY
TIME_SOURCE_KEY_VALIDATION=DENY
UTC_SOURCE=NOT_QUALIFIED
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_PREEXISTING_HASHES_VERIFIED
FIRST_UNRESOLVED_DIVERGENCE=SEPARATE_EXTRACTION_AUTHORIZATION_AND_BACKEND_QUALIFICATION
NEXT=ACCELERATOR_OWNED_ED25519_SUBSET_EXTRACTION
