# TS0-TS8 normative conformance review

Task PACC-PHASE6-TIME-SOURCE-TS0-TS8-ACCEPTANCE-1,2026-10-21. Result: no material normative conflict or undefined byte range found. No correction to the original proposal is necessary. PASS here means contractual conformity, not implementation, native capability, factual source authority or operational capacity.

| Requirement | Clauses/evidence | Finding |
|---|---|---|
|Same root/policy, distinct TIME_SOURCE|TS0/TS1; AT1/AT2; F1/G3|PASS: separate bare-key purpose, no reuse of issuer/PDG enum semantics or parallel store|
|Identity/key/endpoint/status/provenance|TS1/TS3|PASS: full tuple,32-byte raw key and hash, exact address/port, bounded IDs/status; protected references still must resolve factually|
|Complete layout/version/presence|TS3/TS4/TS5; documentary review JSON|PASS: TSC1 header64, TSR1 record384, root header512, CP4512, full coverage and explicit zero tails/endian/counts|
|Sizes/hash domains|F96..1248; T20704..31840; TS5|PASS: contiguous regions, component/C4/W4/CP4 domains exclude stored digest; no native padding or trailing bytes|
|Historical compatibility|TS0/TS4/TS7; G0-G7|PASS: new v4 only; v3/CP3/PDG and all older bytes unchanged; no downgrade or direct v2 adoption|
|Update/current/predecessor/CAS|TS4/TS5; D2/D4/G4/G5|PASS: factual complete v3/v4 base, pin, native identity/full CAS; CP ZERO at commit, CP4 only after factual publication; conflict/UNKNOWN deny/retain|
|Administrative authorization vs DATA|TS1/TS2/TS6; U0-U7|PASS: explicit distinct TIME_SOURCE act, no inherited bootstrap/SNT/PDG permission, no digest/boolean/current from DATA|
|First temporal trust/rotation/revocation|TS1/TS2; AT2/PT1|PASS: independent ceremony/status, response cannot approve itself, no TOFU/fallback; initial positive may remain denied without independent qualified evidence|
|Restart/rollback/history|TS2/TS5; U6/G5|PASS: history only after restart, no restored CURRENT/approval/status/UTC or authoritative recovery; changed/removed dispositions cannot revive ancestral authority|
|Snapshots/pins/borrows|TS6; G7/E6/E7|PASS_CONTRACT_ONLY: immutable prior-protected root/external evidence, synchronous borrow; root pin not protection of external objects; no second table or manufactured lease|
|Stop/quiescence/release|TS6; AT7/Q6/U5/E7/Model E|PASS_CONTRACT_ONLY: no effect after Stop, retain until factual quiescence/closure; RELEASE_INVALID no attempt/effect, timeout/exit not release|
|Portability|TS3/TS7|PASS_CONTRACT_ONLY: numeric byte layout, no mandatory CNG/Win32/platform IDs; scope-free IPv6 subset explicit, other shapes unsupported; no VC6/NT4/box/ISA qualification claimed|
|ED11|TS7; ED11 Windows/incremental envelope decisions|PASS_GATED: serialized arithmetic only, full-buffer/coexistence/retention/native/stack UNKNOWN counted as unknown, no quota or reservation;3MiB explicit only|
|No productive activation|TS8|PASS: implementation/source/UDP/UTC/issuer/UPDATE/CURRENT/signature/recovery/TRUST/admission remain denied or unqualified|

The first scope-free endpoint subset is an explicit new representation restriction, not a change to RFC wire or silent fallback. Exactly three independent sources are a use gate; H0..3 is only storage cardinality. Distinct IDs/keys/parties do not prove independence/calibration. UNKNOWN disposition is structurally known and semantically DENY; other unrecognized enum values reject.

E0-E8 package authority is not generalized to temporal authority. Only lifecycle rules carry over. Runtime concrete types/producer binding, external evidence resolution and factual pin/coexistence proofs require later exact patch review. An accepted validity interval does not solve first UTC by circular reasoning: AT2 remains fail closed when independent status/expiry evidence is missing.

Original proposal and all accepted source artifacts remain byte-exact. No builds, functional tests, native probes, network, key generation, clock changes or functional hunks were executed. The only arithmetic check enumerates document-defined offsets and bounds. Normative acceptance must be officially approved before incorporation; no functional patch is accepted by reference.
