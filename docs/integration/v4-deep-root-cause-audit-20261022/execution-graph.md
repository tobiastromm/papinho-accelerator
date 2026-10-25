# Execution, ownership and resource graph

All paths refer to current canonical source. Rows marked MISSING are not a
positive v4 execution. P = PAPACC_CONFIGURATION_COMMIT; U = its v3_update;
W = pending_commit; B = v3_backings[3]. The same types are used for v4 DATA.

| Stage / actual function | Input -> output / status | Owner and lifetime | Resources, hashes, callbacks, failures and persistent effect | Obligation |
| --- | --- | --- | --- | --- |
| Administration: launcher v3_update_hold / v3_update_approve | candidate bytes, reserved bound, CURRENT/source -> HELD/native act; not RESOLVED | launcher singleton/invocation; root owner thread, U holds prior pin/source ticket | local pin/view, operator token queries in v3act_native; failure closes or retains tokens; no store effect; native credentials are external facts | D2.2/D4, U0-U7; authentication is not approval/status |
| Candidate preparation: pc_root8_v4_*_encode | approved-shape fields -> encoded public DATA image/fragments | caller owns nonoverlapping buffers for synchronous call | fixed codec locals, caller output; generic hash/component callbacks; malformed/truncated/unsupported fail; no store effect | TS0-TS8 offsets/CP4; no positive authority |
| Cohort: papacc_configuration_v3_update_hold_data | base pin + candidate -> slot index/serial0 backing + retained predecessor ticket | P/U and same free B slot; source input borrowed until copied | candidate packet/CA copy in existing slot; pending.previous copy; invokes v3_checkpoint_match/v3_borrow and issuer_hash06; rejects version4 currently; no native commit | G7 immutable material, no candidate pin fabrication |
| DATA capture: backing_capture_data_version / v4_capture_data | packet+CA+decode workspace -> B with serial, hashes | root P protects B; external workspace borrowed synchronously | codec v4 validation, component callback, issuer image validation, DER decode/encode/SPKI hash, repeated BODY05 decode; hashes via issuer_hash06 and store helper; no escaped workspace pointers; slot rechecked before memcpy; no persistent effect | G7/TS lifetime/corruption, preserve issuer/history |
| Canonical kind8: pinned_signer -> papacc_root8_signer_data | protected carrier/PDG row -> PUBLIC DATA only | real predecessor pin with exact backing ranges; native DER/signature work uses caller arenas | v3 borrow required, header version3 required; native certificate validation and hashes remain. MISSING v4/candidate-owned bridge, especially D>0 | F/G issuer/delegation data; mathematical validity not authority |
| Factual resolution: v3_update_current_sources / update_validate | same CURRENT/source ticket + act -> NOT_SUPPORTED | prior pin, source transaction ticket and native act retained in U | current identity and act token revalidation/hash calls; no qualified issuer status/UTC source; not RESOLVED; no worker effect | D4, U/Q, E0-E8; UNKNOWN remains deny |
| Predecessor: v3_checkpoint_match_data; pc_root8_v4_base_link DATA | complete base CP3/CP4 + next -> correlations only | caller borrows; pin protects published root bytes; previous copy persists through worker | slot R/A/P/I, full image/CP hashes and issuer prefix checks; no CURRENT created. Canonical v3 checkpoint path accepts CP3, so v4 attachment MISSING | complete predecessor, no digest-only fencing |
| Worker prepare: pending packet11 test entry / future reviewed canonical entry | immutable previous/next, file identity, Stop -> pending packet | W by value; one worker/thread; no root pointer/Session/authority callback in worker | three image capacities are v3 maximum30592; thread reservation65536. packet11 and pending_v3_image_guard reject v4; production v4 preparation MISSING | native identity, frozen arrays, pre-effect validation, bounded capacity |
| Validate: pending_v3_packet_valid | frozen arrays -> accept/reject | W until factual completion | repeated image trailer/CP3 hashes through W.store_hash; same carrier/rows preserved; v3-only shape; no private signing or write | exact CP/predecessor/current mode invariants |
| CAS: pending_worker -> pending_compare_file | native transaction file+expected identity/size+previous -> equality/conflict/error | W holds transaction/file, owner holds thread | CreateTransaction/CreateFileTransacted; native file identity/size then ReadFile+memcmp every previous byte, scratch512 reused after each sync read. No hash substitution for compare | serial native CAS, no TOCTOU across unprotected close/reopen |
| Durable commit: pending_worker | validated comparison+Stop -> WriteFile/Flush/CommitTransaction | same exclusive transaction file and W; signals result only after worker completion | native I/O failure/Stop/close/commit uncertainty retain references and both images; conflict distinct; no partial CURRENT publication; persistent effect begins here | Model E, CAS UNKNOWN not rollback |
| Poll/current: pending_poll06; configuration_transaction_publish / issuer publish, checkpoint paths | native result -> committed_pending, then only authorized factual publication | P slots/tickets/epochs; no worker authority | exact entry paths differ by old format; no complete canonical v4 path. CP4 population and checkpoint transaction remain dependent integration, not inferred from codec | COMMIT -> CURRENT -> factual A/P/checkpoint |
| Retain/abort/restart: v3_update_abort / abort_data / configuration_stop/finish | Stop/terminal state -> retention or factual release | existing root/U/B/W/source owners keep responsibilities | uncertain token/file/transaction/hash prevents abort/reuse/finish. Sync borrows end factually, pins unpin once. Historical integrity may persist, operational authority does not restore on restart | U6, G5/G7, Model E/ED08 |

The request's linear COMMIT -> CAS order is not the actual safety order. CAS
verification precedes native write/commit, and no CURRENT can be published from
the diagnostic DATA or an incomplete result.

## Hash dependency edges (source-proved)

- store03_hash -> papacc_store_hash -> BCrypt Open/Create/Hash/Finish/Destroy/Close.
- digest(record,480) -> papacc_store_hash; BODY04/05 and old record validation use it.
- store02_hash -> its own local BCrypt handle pair, not the new retained helper.
- backing_capture_data_version -> pc_root8_v4_validate -> codec hash callbacks;
  then issuer_image_validate, v3b_ca, packet/CA hashes -> issuer_hash06 -> BCrypt.
- backing_borrow_data_version -> hash(packet), hash(CA) inside P.lock;
  v3_checkpoint_match_data and pinned_signer invoke borrow again.
- update_act_capture/revalidate/match -> hash candidate/predecessor through issuer_hash06.
- pinned_signer -> signer DATA -> native DER/signature validators ->
  CryptHashCertificate2/CryptVerifyCertificateSignatureEx as applicable.
- pending_compare_file -> ReadFile+memcmp, not SHA; these edges must remain intact.

Provider-free SHA at the first edge alone leaves all later native paths.
EOF/cleanup/timeouts do not prove native resources disappeared.
