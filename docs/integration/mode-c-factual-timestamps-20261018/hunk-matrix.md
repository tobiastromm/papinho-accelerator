# Exact correction review — approved operation
Task PACC-PHASE6-MODE-C-FACTUAL-TIMESTAMP-EXPORT-CORRECTION-1.
Official executor review APPROVED the five focal hunks; the rejected operation was not reapplied.
Preimages are exact bytes under preimages/. exact-source-hashes.json records pre/post source identity; final manifest will record later test additions and evidence.

| Hunk | Function/boundary | Before | Approved change | Authority and regression |
|---|---|---|---|---|
| H1 | PAPACC_C8_PROCESS_PROVENANCE | No exported temporal fields | Caller-owned original observed/validated/recorded-until times plus explicit presence/clock descriptor | Owner exact export decision; sizeof and native copy equality |
| H2 | papacc_c8_target_process_provenance | Time already observed/checked but not returned | Copy existing sampled_ms, existing validation instant now, existing expires_ms; no new query, threshold, sampler or rule | Same producer and clock/lifetime; original bytes and identity preserved |
| H3 | process_provenance_offline_test | Native JSON omitted times | Assert exact original copy; serialize local domain with factual PID/creation, original sequence/invocation; policy ref null | Native own-process test, no Browser/network/auth |
| H4 | recorded_time_coherence / validate_producer_binding | No temporal-coherence report; producer consistency could correlate | Strict local reference/domain checks, explicit null absence; classify regression/inverted/outside/coherent without threshold; FRESHNESS_POLICY NOT_ESTABLISHED and VALIDITY_RESULT INCOMPLETE always | ED11 requires separate target approval; ADR0052 remains C8 CONTROL-only; no policy accepted by input string |
| H5 | process_provenance_consumer_test | Producer fixtures could return CORRELATED without Mode C validity | Preserve reported identity checks; require INCOMPLETE without applicable policy; time/absence/domain/reference/cross-process negatives | Synthetic only, no native promotion |

The producer's pre-existing validity/cadence implementation is untouched. Exporting its recorded expiry is not accepting that rule for Mode C. There is no approved/applicable Mode C freshness policy identified; coherent timestamps alone do not produce PASS. Zero remains a real numeric observation when present, not missing. No direct comparison between participant clocks. No operational authority, credit, grant, policy, epoch, wire or quota changes.