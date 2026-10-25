# ED11 provenance and lifetime reconciliation

The historical 3813017-byte figure is an intentionally unshared composition containing a test-provider CRT peak. It is neither a measured operational peak nor a proof that the operational deployment must exceed 3 MiB. Earlier closeouts correctly labelled it conservative; statements of operational impossibility based solely on that sum must be read with this qualification. No previously reserved bytes are released by this review.

## Exact arithmetic provenance

* 2103847: earlier Selected known conservative base, not just fixed storage.
* +116616: three compiled G7 backing slots, including packet/CA/metadata.
* +4896: existing three-image v3 capacity delta.
* +80: existing UPDATE cohort descriptor.
* +224: existing approval proof delta.
* =2225663: inherited conservative base, including fixed1948880 and other inherited conservative charges276783.
* +231740: named manual-package operation components.
* +68: shared M1 readonly bindings/order data.
* +1290354: TEST-provider CRT peak, includes global signing fixture.
* =3747825: inherited full synthetic/nonshared package scenario.
* +65192: proposed Roughtime charges, already including 488 observation/history, stack/transport ceilings.
* =3813017: historical combined scenario; arithmetic excess667289 over3145728.

Sources: manual-package-trust-authenticated-validation/ed11-census.json; shared-m1-factual-evidence/ed11-census.json; v3-immutable-backings-pin-lifetime/ed11-census.json; canonical-v3-transaction-completion/ed11-census.json; canonical-v3-update-cohort/ed11-census.json; v3-update-approval-producer-implementation/ed11-census.json; roughtime-exact-profile-completion/ed11-census.json (all dated20261020/21, pinned in manifest).

The isolated tests/manual_package_crypto/CMakeLists.txt builds package_crypto_test. tests/manual_package_crypto_test.c creates a fixture-only global signing context and explicitly frees it at test_only_global_signing_context_complete. The historic CRT high-water therefore cannot stand in for measured allocations of an operational Roughtime or package backend. It also contains genuine crypto/CRT work: subtracting the entire1290354 does NOT establish that all of it is fixture overhead, saved memory, or absent from deployment. Replace that incomparable measurement by separately qualified operational cost B, not zero.

Thus a useful analytical decomposition is 2522663 + B + X, where B is the actual operational backend resource graph (including retained/provider state) and X contains all other not-yet-sized costs/overlap corrections. 2522663 is an inherited conservative subtotal, NOT a verified lower bound, upper bound, measured peak or admission credit. Nominal remaining arithmetic space623065 is not available reserved capacity. The historical3813017 scenario remains preserved; this is a comparison of evidence classes, not a quota reduction.

## Lifetime matrix

| Resource | Owner/start | Coexistence and factual end | Reuse conclusion |
|---|---|---|---|
| Six requests/responses24576 | Existing administrative campaign owner; slots become live during acquisition | All needed through chained validation; uncertainty, audit need or borrow can extend retention | PT0 explicitly reserves all slots; serial sends alone do not permit overwrite |
| Random chain192 / metadata1536 | Same campaign; before nonce generation | Full campaign verification/evidence obligations and factual quiescence | No reduction demonstrated |
| Parser4096 / crypto16384 | Existing owner workspace before native use | Parsing, verification callbacks and uncertain provider completion may coexist | Future backend could prove non-overlap; none exists now; no overlap discount |
| Source descriptors1536 / external trust graph | Canonical source owner and root/policy refs before acquisition | Source revision/status and applicable external owners protected through effect/borrow | Root pin alone is insufficient; external graph remains UNKNOWN |
| Campaign control4096 / transport4096 | Same owner before I/O | Send/receive/close plus cancellation uncertainty | Deadline does not close native handles or release buffers |
| PAL observation/history488 | Same operation; collection through verification | Must remain valid/protected through issuer revalidation and every borrow | Publishing an observation does not detach proof or create independent owner |
| Adapter stack8192 | Actual executing thread when calls begin | Compiler/native/provider transitive frames; possible pending native refs | Ceiling unproved; cannot subtract existing65536 worker stack without placement/high-water proof |
| Root/issuer/G7/snapshots/pins | Existing canonical owner, already in inherited base | CURRENT/predecessor/candidate/old Sessions/retained CAS results | Fixed arrays remain allocated even when idle; do not subtract116616 or91776 again |
| Named manual-package231740+68 | Existing package operation when instantiated | May coexist with trust/time before KDF, quarantine/input, retained failures | No enforced mutual-exclusion producer integration proved; keep priced |
| Test CRT peak1290354 | Isolated test process lifetime | Measurement includes fixture/provider state | Different evidence class; operational replacement B is UNKNOWN, not zero |

The canonical cohort source enforces selected no-worker/no-pending guards in HOLD and requires no readers/retention on candidate abort. These guards protect the UPDATE lifetime; they do not establish that a not-yet-implemented Roughtime producer shares its scratch or excludes package/Sessions across the entire process. CAS UNKNOWN, failed unpin/native close and Stop preserve reachability. No fixed CURRENT/committed/previous/backing bytes may be overwritten to create temporal scratch.

## Separation assessment

Acquisition, verification, observation exposure, issuer consumption and evidence retirement are logically ordered, but there is no native Roughtime implementation proving storage separation. The profile retains all transcripts and external evidence across verification/quiescence. Issuer consumption needs qualified source/status/current continuity; no accepted compact proof permits dropping arbitrary evidence merely because the interval is copied. Stop invalidation prevents use but does not shorten ownership. The current safe sharing deduction is zero.

## Minimum safe optimization path

1. Measure operational backend allocations separately from fixture signing and caller arrays; retain all native/opaque costs. This is measurement replacement, not deleting legitimate reserves.
2. Within the same owner, prove parser/crypto workspace call graphs, no escaping references and provider closure; only then propose a union/shared workspace and differential regression hunks.
3. Prove actual adapter thread/stack placement before treating8192 as contained in an existing reserved stack. Existing idle reservation is not automatically transferable credit.
4. Preserve the current2048-byte profile. A1024-byte request/response variant would reduce12 slots by12288 bytes but changes exact PT0 and needs a separate focal normative revision and proof-size/path analysis. It would not erase opaque costs or prove capacity.
5. Acquisition is already serialized by the proposal; it yields no additional discount. Enforced separation of package KDF/AEAD and acquisition could be considered only with existing admission/owner guards and retained-graph proof; no guard or quota change is implemented here.

No deadline-based release, abandoned audit evidence, parallel owner or new journal is proposed. All alternatives remain investigation/handoff, not approved buffer reuse. Implementing a backend stays denied until a complete bounded measured graph and actual reservation satisfy the selected limit.
