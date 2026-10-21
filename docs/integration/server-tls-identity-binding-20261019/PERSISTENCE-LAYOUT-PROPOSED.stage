---
status: proposed
revision: 1
scope: private same-root server TLS identity persistence only
---
# Byte-exact server identity extension â€” ROOT8 version2 / SNT1

## T0 Authority, scope and closed gates

PROPOSED, not ACCEPTED. Owner task PACC-PHASE6-SERVER-TLS-IDENTITY-BINDING-1 authorizes preparation of the missing exact storage contract, not its acceptance or dependent encoding. S1â€“S5 in DECISION-server-tls-expected-name-source-20261019.md remain accepted source requirements. This proposal changes no existing file/codec/reader/grammar or runtime. No new root, registry, catalog, journal, worker, epoch or wire identity. BYTE VALIDITY IS NOT ADMINISTRATIVE AUTHORITY.

Select a new explicitly versioned interpretation of the EXISTING PACROOT8 envelope: magic PACROOT8 with version2, not reinterpretation of version1. Existing version1, BODY0.5, ROOT06 issuer image/anchor, LST1, PACICP01 and PACR8CP1 remain byte-exact. Version2 contains the SAME ordered listener descriptors/BODY0.5 models and one public name-association component owned by the SAME administrative root participant. The component is not a separately persisted store, CURRENT, allowlist, journal or catalog. It cannot be independently adopted by a resolver.

This first proposed version2 profile is EXISTING_ROOT_UPDATE only, with an existing initialized issuer and exact carrier. INITIAL_BOOTSTRAP/version2 is UNSUPPORTED: do not invent CA identity/generation to fill it. Accepted bootstrap/version1 remains unchanged and still requires all its factual authority/custody gates. First issuer initialization, version1->version2 adoption and native carrier integration require their own exact authorized transaction/adapter proofs. This proposal alone authorizes none. CARRIER_ISSUER remains UNSUPPORTED in current code; version2 integration remains DENY until that gate is qualified. Do not remove issuer or reinterpret its historical anchor as active policy.

## T1 Regions, sizes and discrimination

Outer fields are unsigned LITTLE-ENDIAN; existing embedded BODY0.5/ROOT06 retain BIG-ENDIAN and LST1 LITTLE-ENDIAN. No native structs, padding, pointer, SID, CNG handle, private material, hostname alias or volatile interface index. Size arithmetic is checked before access, with no wrap/truncation.

N = candidate listener count1..4; B = pinned predecessor listener count1..4; K = number of SECURE_PRINCIPAL entries in the candidate collection,1..N. This version requires one association for EVERY Secure candidate and none for Legacy. It does not expand target cardinality/profile support. Current singular target still rejects N>1 before effects; Legacy remains independently unsupported where not implemented.

L =160+512*B, the unchanged BASE LST1. J=16384, the mandatory unchanged issuer carrier. S=96+768*K, complete association component. T=1056+1536*N+S+L+J. Exact minimum20512 (N=B=K=1); maximum28960 (N=B=K=4). There are no optional gaps, alignment padding, trailing bytes or partial substitutes.

Regions:
- header512 at0;
- candidate records1536*N at512;
- SNT1 component S bytes at O=512+1536*N;
- base LST1 L bytes at O+S;
- issuer carrier J bytes at O+S+L;
- checkpoint slot512 at Q=O+S+L+J;
- whole digest32 at Q+512.

Full header matrix is offsets.csv. Header fields not explicitly changed below retain their exact D3 Revision3 field encoding/rules under this NEW version only. Header length512 and candidate descriptor/BODY sizes512/1024 remain unchanged. Changes:
- offset8:u16 version2; offset12:u32 T20512..28960;
- offset16:u32 flags3 (UPDATE and ISSUER_CARRIER); offset20:u16 N;
- offset30:u16 base LST1 version1;
- offset32/36:u32 collection512/1536*N;
- offset40/44:u32 LST offset O+S and length L;
- offset48/52:u32 carrier offset O+S+L and length16384;
- offset56/60:u32 Q/512; offset64/68:u32 Q+512/32;
- offset128:u32 presence31: domain/R/predecessor/I/BASE mandatory;
- offset132/134:u16 BODY codec5 and descriptor version1;
- offset240:u16 prior_kind1=PACROOT8/version1 or2=PACROOT8/version2, exact selected predecessor format only;
- offset242:u16 association component version1;
- offset244:u32 O; offset248:u32 S;
- offset252:u16 K; offset254:u16 association record bytes768;
- offset256:u16 component hash algorithm1=SHA256; offset258:254bytes ZERO.

Offset172:4 remains ZERO. All other reserved fields remain ZERO. This is a proposed definition for version2, not permission to write into version1 reserved bytes. Root domain at72:32 is nonzero; R104:u64=predecessor112:u64+1 with no wrap; I120:u64 nonzero matches the carried issuer revision. R_base/A_base/P_base/I_base at136/144/152/160 are exact factual predecessor fields, all nonzero and base_presence168:u32=15. Header C_base176:32 and W_base208:32 refer to the same exact pinned predecessor image. No new A/P is stored in those base fields; no new counter/source epoch is introduced. Version/magic is checked BEFORE format-specific size/offset dispatch; unknown version rejects, never falls through to ROOT06/version1 parsing.

## T2 Component header64 and trailer32

Magic PACSNT01 is8 ASCII bytes. This component has no independent file/source API or commit operation.

|Offset|Bytes|Field|
|---:|---:|---|
|0|8|PACSNT01|
|8|2|version1|
|10|2|header64|
|12|4|S=96+768*K|
|16|2|K1..N|
|18|2|record768|
|20|4|flags0|
|24|32|exact containing root domain|
|56|8|exact containing candidate R|
|64|768*K|records below, in strictly increasing candidate listener ordinal|
|64+768*K|32|SHA256(component bytes[0,64+768*K))|

No separate standalone R, CURRENT or approval bit is created. Domain/R are constrained redundancy tied to containing content. Component/hash absence or failure rejects; a valid digest cannot attest administrative approval, freshness or fencing.

## T3 Association record768, explicit absence and purpose

All offsets below are relative to each record. There is exactly one row for each Secure candidate ordinal and zero for any Legacy ordinal. No duplicated ordinal or listener ID, sorting by hostname, dropped row, wildcard or cross-listener inheritance. Shared security_ref is allowed only through explicit separate associations; the application-owned resolver must prove coherent mapping to the same immutable Local Identity or deny ambiguity.

|Offset|Bytes|Field|
|---:|---:|---|
|0|8|PACSNI01|
|8|2|record version1|
|10|2|record bytes768|
|12|4|presence below|
|16|2|zero-based candidate listener ordinal0..N-1|
|18|1|transport1=SECURE_PRINCIPAL|
|19|1|role1=CHANNEL_SERVER|
|20|2|certificate technical profile1, the existing accepted RSA2048/SHA256 server profile|
|22|1|association stage1=NAME_ONLY,2=REQUEST_BOUND,3=CERT_BOUND|
|23|1|ZERO|
|24|2|listener_id length1..64|
|26|2|security_ref length1..64|
|28|2|ExpectedServerName byte length1..253|
|30|2|ZERO|
|32|8|name_approval_revision: existing root revision that approved this exact name/listener/security/role association; nonzero <= containing R|
|40|16|issuer authority ID, nonzero|
|56|16|issuer generation ID, nonzero|
|72|16|request correlation ID or ZERO when absent|
|88|32|CSR DER SHA256 or ZERO when absent|
|120|32|server full SPKI DER SHA256 or ZERO when absent|
|152|32|issued certificate full DER SHA256 or ZERO when absent|
|184|64|listener_id bytes, ZERO tail|
|248|64|exact security_ref decoded bytes, ZERO tail|
|312|256|ExpectedServerName bytes, ZERO tail including at least three bytes for maximum253|
|568|32|CA full SPKI DER SHA256, nonzero, matching applicable issuer generation|
|600|168|ZERO reserved|

Presence bits0=listener,1=security,2=name,3=issuer are mandatory. Bit4=request present iff stage2/3; bit5=certificate present iff stage3. All others zero. Thus stage1 presence15; stage2 presence31; stage3 presence63. These are data-presence/stage tags, NOT persisted administrative authorization flags. Stage1 requires offsets72..184 entirely zero; it records approved naming intent and applicable existing issuer, not an invented future key/request. Stage2 requires nonzero request/CSR/SPKI fields at72/88/120 and certificate digest152:32 zero. Stage3 requires all those plus nonzero certificate digest. Reject inconsistent stage/presence/zero fields; do not turn absence into zero-valued valid identifiers. No raw CSR/certificate, secret, grant, token, password, trust DER or private key is stored here.

The listener ID matches the selected descriptor at24:64 including length/case; its security_ref matches descriptor88:64 and decoded BODY security reference exactly. Identifier syntax and security_ref UTF8/no-control/bounds/zero-tail rules are unchanged LST1/D3 rules; no executable papacc.conf field is introduced. Role/profile/transport disagreement rejects. Security reference is still opaque; no hostname derivation from it.

The name is the explicit public attribute selected through authorized administrative configuration, not parsed as approval from config text. The first bounded representation supports the existing offline server builder's DNS ASCII subset: labels1..63, total1..253, letters/digits/hyphen, no empty or edge-hyphen label, wildcard, embedded NUL, non-ASCII, trailing dot or numeric IP representation. Preserve exact bytes/case, no normalization, IDNA, DNS lookup, CN alias or implicit additional SAN. Unsupported naming representations reject, never approximate. This serialized bound reflects the already implemented target subset; it is NOT a new approval, quota, freshness or global name policy.

name_approval_revision is provenance in the SAME root's committed history, not a new monotonic counter. On introducing/changing the exact name/listener/security/role relation, it must name that transaction R; on retaining an unchanged association it refers to the prior verified approval revision. Request/result attachment alone does not manufacture a new naming approval. Decode checks nonzero/range only; actual authority must verify the complete approved content and its unchanged linkage through history/source snapshots. A <=R value, hash or historical pin alone cannot prove approval/current applicability. Removing/revoking approval prevents new effects; old dependency retirement remains Model E/active-Session policy, not a consequence of zeroing a row.

## T4 Same-root issuer/request/certificate semantics

The mandatory ROOT06 carrier must be validated by the existing issuer owner/codec as the SAME root's selected authority/generation/CA-SPKI/history/I. Its archived configuration anchor remains only the issuer-image hash/history anchor, never active listener policy. Root domain, applicable authority/generation/SPKI and I must agree with the association and actual CURRENT/history. Preserve high-water, reservations, revocations and generation/event prefix; no absent/old issuer fallback or dropping carrier. Version2 code remains UNSUPPORTED until that adapter integration and lifetime are separately qualified; valid bytes alone do not satisfy it.

NAME_ONLY is explicit non-issuable naming data and cannot satisfy preparation requiring CSR/key/reservation. REQUEST_BOUND requires the exact same-root existing request/correlation and CSR/SPKI digests, purpose SERVER and issuer generation. An actual durable serial reservation must be proved before private effect; no serial is generated/duplicated in this component. The existing issuer record remains sole authority for counter/serial/state/validity/revocation; binding does not allocate/reset it. SIGN_REQUESTED/uncertain outcomes cannot be retried merely because a row can be read. CERT_BOUND additionally requires existing issuer result/DER digest/SPKI/purpose/profile/SAN match and factual result status; it cannot be fabricated at request creation. Digest fields are references to existing bytes/history, not substitute certificates or trust validation. PUBLISHED still requires its own factual publication, not the stage label.

At actual consumption hold both the exact root CURRENT pin and existing owning source snapshot/borrow, and verify selected listener/profile/security/name approval revision, issuer/gen/request/key/result and current administrative authority again. Inputs remain immutable through asynchronous/retained consumers. The server preparer receives only the name from that resolved authorized snapshot, never the raw component/candidate string as authority. A certificate's SAN must match that name and accepted serverAuth/KU/BC/algorithm/validity; no CN fallback or confusion with clientAuth/Principal or origin Site TLS trust. The common issuer stays platform-neutral. This proposal does not add CNG/Windows or PapinhoTrustStore dependencies to it.

## T5 Hash domains, checkpoint, CAS and recovery

Four integrity domains, all SHA256 via existing approved synchronous adapter, failure/absence rejects:
1. SNT digest at component[S-32,S) hashes its bytes[0,S-32).
2. C_v2 = SHA256(entire outer bytes[0,Q)), including exact header/version, full candidate models, association and its trailer, base LST1 and carrier. C_v2 is a document-local content-digest label, not an operational epoch, wire field or new authority. No digest field lies in its own domain.
3. Hcp2 = SHA256(checkpoint[0,480)).
4. W2 = SHA256(outer bytes[0,Q+512)), stored at Q+512:32.

To avoid accidental historical checkpoint reinterpretation, the version2 checkpoint is either all512 ZERO (absence) or magic PACR8CP2/version2/size512. Its fields and offsets follow CP1 exactly: presence12:u32=1 (issuer); root16:32; R48:u64; predecessor56:u64; factual published A64:u64/P72:u64 nonzero; I80:u64; computed content digest88:32; C_base120:32; W_base152:32;184:296 ZERO; Hcp2 at offset480, length32. CP2 is accepted only with outer ROOT8/version2; CP1 only with historical version1. There is no reinterpretation of PACICP01. Root+R+content digest is correlation, not a new generation counter or proof of latest global state.

For UPDATE, use actual CURRENT root identity, exact predecessor image and native store identity, revision/base pin and its durable matching checkpoint, plus source leases. Prior_kind explicitly matches the selected version1 or version2. Base LST1 remains the historical reference fragment for that predecessor's listener/BODY collection only; for a version2 base it does NOT carry approval rows. Compare the FULL selected predecessor image and independently preserve/validate its association/source history. No authority may be reconstructed from LST1 alone. First version1->version2 adoption is an explicit authorized same-root format/content transition after acceptance and adapter review; it is never automatic decoding/migration. Historical ROOT06 is not an admissible base for this proposal. Missing intervening history/approval, stale source, uncertain base or unsupported carrier returns INCOMPLETE/DENY, not a bootstrap/reset.

PREPARE owns complete immutable inputs, authorized source and issuer references and budget. VALIDATE requires source data actually RESOLVED, not MODEL_BOUND or codec OK. COMMIT uses the SAME worker and exact compare-and-commit against full previous bytes/native identity; first writes full version2 with ZERO CP. CAS conflict/UNKNOWN denies the new effect and retains ownership/credit/buffers, never selects an older revision or retries destructively. CURRENT then actual A/P may be published ONLY by their existing authority after confirmed durable commit. Name/security changes are not assumed issuer-only; preserving P requires identical operational projection proof. No synthetic epoch, retroactive grants or Session snapshot replacement.

Only after those facts may CP2 be copied and persisted using SAME worker/checkpoint-only CAS against the exact zero-CP image. Prefix[0,Q), content digest/R/I/association/model bytes must remain identical, not merely hash-equal. Only CP and W2 change. Failure/conflict/UNKNOWN retains both results and denies dependent further update until reconciled; no Stop wait, fabricated publication or blind retry. No replacement of an existing checkpoint by rehash is authorized. Stop never proves native I/O cancellation or destruction.

Decode checks all lengths/offsets/cardinality/ranges/zero padding/unique ordering/hash domains/embedded component and reference relations before making output available; unknown version/flags/stage/hash algorithm, overlaps/gaps/truncation/trailing bytes and invalid strings reject the entire image. Output is bounded owned data, never authority/borrow after source release. Caller owns immutable input and disjoint output/metadata for synchronous calls; bad pointer/range/overlap writes no output, later failure marks used/complete unavailable/zero after safe preconditions. No half-decoded collection. Hash/component callbacks cannot reenter/mutate/retain inputs; opaque/native costs remain priced and unknown until qualification.

Crash before/uncertain commit: no authority. Commit before CURRENT/epoch: CP absent, no authority. CURRENT before CP durable: restart denies authority. Complete CP+valid hashes: historical consistency only, still no independent freshness/fencing. Relative rollback detection compares a separately retained proven baseline: lower R, equal-R changed content, source approval/request/issuer history regression, lost CP, changed domain/predecessor, high-water/revocation rollback or missing event prefix => DENY/INCOMPLETE. No global freshness claim from these hashes/relative comparisons; AUTHORITATIVE_RECOVERY=DENY until independent approved checkpoint/backup/fencing. Never regenerate CA or reuse serial.

## T6 ED11 and separate acceptance

Runtime delta in this documentation-only task is0. Existing known base2084039/2097152 remains unchanged; margin13113 is not a reserve for this design. Provider/kernel UNKNOWN; full activation managed/physical/commit/handles/native peak UNKNOWN. No operational admission or feasibility PASS.

New component S has minimum864/maximum3168 serialized bytes. Serialized outer maximum28960 vs prior full-format maximum25792, delta3168. Under the SAME hypothetical full-size separate byte-buffer pattern:3E=86880 vs77376, delta9504;4E=115840 vs103168, delta12672;6E=173760 vs154752, delta19008. These are ARRAY SUBTOTALS ONLY, not additive deltas against the actual ALL base: the running code uses different no-carrier capacities and issuer snapshot ownership. Prove its real graph before combining. No assumption that the actual base already reserves full carrier/max E or that duplicated carrier/snapshots are free. Actual root packet capacities may reject the proposed format until a separately approved change fits the cap.

Price encoded component, decoded own copy and each retained snapshot actually instantiated, source name/reference arrays and lifetime metadata, old/new generations, pending/UNKNOWN exact-CAS images, CSR/certificate/CA backing, crypto/hash workspace, full stack/callbacks and provider/native resources. Prove sharing/exclusive phases before omitting copies; never reuse a retained/pinned generation or double-count proven shared bytes. Proposed record total includes listener/security/name arrays64+64+256=384 per row; do not add these again on top of768. Maximum component model excluding digest3136, encoded3168; coexistence if both separate=6304 BEFORE adapters/owner/other inputs. No new persistent model struct or global buffer is authorized. Missing bound/insufficient credit returns DENY under unchanged2097152. Do not reduce legitimate reservations or enlarge quota to accept this proposal.

This exact layout requires owner normative acceptance and official executor registration BEFORE codec/root implementation; later functional hunks require their own exact review and lifetime/ED11 proof. No code, CA/key/certificate/private signature, network, CURRENT, listener, trust, grant, recovery, admission or PST release is authorized now.
