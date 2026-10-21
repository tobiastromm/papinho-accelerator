# Canonical transaction integration — exact blocked boundary

No functional patch applied. This is a concrete implementation-boundary review, not a general Phase6 audit. HEAD main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a; baseline/preimages in final manifest. Accepted LST1 and historical source preserved.

## Existing executable boundaries

| Requested stage | Existing function and factual condition | Result |
|---|---|---|
| SOURCE/PARSED | pc_parse, src/config/config_source.c | owned AST, no root authority |
| VALIDATED | pc_validate | explicit supported subset; structural references only |
| MODEL_BOUND | pc_bind_existing_models[_snapshot] | exact existing model projection; no resolved bit; borrowed discovery cannot grant CURRENT |
| RESOLVED | papacc_server_config_source_resolve, apps/server/server_config_source_binding_win32.c | requires actual configuration_current pin, expected revision nonzero, owner thread, inactive runtime, published validated Secure source, correct leases/ALPN, reserve and revalidation |
| PREPARED participants | participant_candidate / configuration_prepare_listener in server_configuration_participants_win32.inc | owned initial/recovery candidate and provider; commit must be uninitialized unless exact recovery candidate; no initial CURRENT yet |
| prepare_store | server_configuration_root_transaction_win32.inc:43-60 | root_dormant_owned; expected must be0; configuration_commit must be uninitialized; not normative_committed; dormant_applied; existing participants validated |
| COMMIT/CURRENT | existing transaction functions | NOT_EXECUTED; no patch/command publishes |

For the ordinary new-installation path, RESOLVED requires initialized/published CURRENT while prepare_store forbids initialized store. There is no evidenced direct composition of those calls. Recovery is a distinct guarded path, not permission to bypass either precondition. Existing component prepare05 can stage against a published store, but calling it directly would bypass application-level participants/authority/lifetime; it is not a ready-made root transaction integration.

## Second boundary: LST1 provenance vs candidate

Accepted C2 encodes nonzero historical R/A/P and predecessor R-1 plus digest of exact1024-byte BODY05 with matching domain/R/predecessor. It does not encode a whole multi-listener candidate. BODY04/05 requires body policy_epoch zero; A/P come from actual root context, not this field. C4 says codec is no authority stage. Reusing old CURRENT as the candidate's future CURRENT, or assigning prospective R/A/P to make encode pass, would violate accepted semantics. A value-only RESOLVED export is not owned/pinned transactional state nor a candidate constructor.

Existing participant owns listener target snapshot and runtime IDs, not LST1 collection/model persistence. Existing ROOT06 uses the same8192-byte store plus issuer8192 and optional checkpoint512 (linked16896); no accepted listener-section extension exists. No free bytes, second file, alternate registry or parallel journal can fill the gap. Copying a decoded LST1 into root memory with a PREPARED flag would not prove participant validation or versioned persistence. Positive hash/roundtrip is not permission to do so.

## Stop/ownership/ED11

No references are acquired or new buffers embedded. Existing snapshot ownership, root pin checks, Stop, retained states and credit remain untouched. The exact normative-preparation blocker precedes any new lifetime/reaper/worker change. No new blocking query, hash call or worker is installed in owner/Stop.

Compiled existing census: model2176, codec test models6528, test buffers4418, native adapter context105632; AST59880 and max input8192 are types/limits, not instantiated by codec test. Existing source resolver charges AST+input+binding+diagnostic+models+16384 in addition to combined reservation. Future joint pipeline must count LST1 decode/encode/model/body1024/hash scratch and retention on top of actual overlap, not sum test fixtures blindly or price already embedded root twice. No new product allocation/reservation, quotas unchanged; provider/kernel UNKNOWN, total PARTIAL. Tests of the old composition do not qualify a new pipeline.

## Minimal next implementation decision

See DECISION-BOUNDARY-PROPOSED.md. Preserve LST1 as historical provenance; define the candidate-to-base relation and same-root collection envelope before a source writer/participant mutation. No accepted document changed. This is not executor denial; no functional operation was submitted because its normative prerequisites are missing. Positive PREPARED, root-positive RESOLVED and CAS/reaper/stale-writer tests for a new composition remain NOT_IMPLEMENTED/NOT_RUN. Existing codec/model/negative CURRENT tests only validate their existing boundaries.
