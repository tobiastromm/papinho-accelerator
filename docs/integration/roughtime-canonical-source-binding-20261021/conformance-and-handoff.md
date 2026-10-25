# Source representation audit and focal handoff

No accepted TIME_SOURCE representation was found. AT2 explicitly requires a minimal separately accepted layout if canonical fields are insufficient. Evidence:

| Existing mechanism | Limitation |
|---|---|
|Issuer core purpose CLIENT/SERVER/PACKAGE|Certificate purposes; no bare Ed25519 temporal identity or status authority|
|PDG1/G3|Purpose1 exclusively PACKAGE_SIGNING; certificate/issuer/listener semantics cannot be renamed|
|PACROOT8/v3 G2|Bytes274..512 are ZERO; no uncontracted extension fields|
|U0-U7|Approval for existing v3 SNT/PDG candidate; not implicit authorization for TIME_SOURCE|
|E0-E8|Package trust evidence/lifecycle; not an independent temporal source producer|
|PAL monotonic|Durations, not authenticated UTC, rho/E or independent administrative source status|

The proposed TS0-TS8 addresses the missing representation with new version4, TSC1/TSR1 and CP4 within the same root, not a sidecar or second authority. It defines a separate purpose and explicit operator/authorization/provenance references. Those references still require factual protected producers; persisted IDs/status/fingerprint cannot authenticate themselves. No source is selected and no positive source is claimed.

TS3/TS4/TS5 enumerate bytes and hash domains. Original v3/CP3/PDG1 remain intact. A source change requires complete predecessor, CAS and protected immutable generations under the existing owner; restart remains historical-only. First bootstrap status must be independent of the response under validation, otherwise DENY even after implementation. Three independently controlled sources/calibration and qualified PAL remain independent factual gates.

TS6 preserves root vs external-evidence ownership, synchronous protected borrows, Stop retention, quiescence and RELEASE_INVALID. TS7 provides representation arithmetic only; no capacity proof, native test or reservation. No test ran because no functional patch is authorized without normative closure.

Next review: exact TS0-TS8 acceptance, including administrative authorization scope and scope-free address subset. Then existing-owner codec/binding integration with prepatch census, offline source absent/key divergence/status unknown/revocation/stale revision/Stop/retention tests. Only afterward consider native transport; source/pin/CURRENT/quality/lifetime/capacity facts remain required. No functional API, enum or field was added by inference.
