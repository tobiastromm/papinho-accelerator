---
status: accepted
revision: 1
decision-date: 2026-10-20
scope-level: project
scope-target: PapinhoAccelerator private server TLS identity persistence
supersedes: []
decision-makers: [Tobias Tromm]
---
# Server TLS identity persistence — exact T0–T6 Revision2 acceptance

## Authority and exact incorporation

Owner conditional acceptance PACC-PHASE6-SERVER-TLS-PERSISTENCE-REVISION2-ACCEPTANCE-1. This new designated normative record takes effect ONLY after official executor approval of its exact creation and post-write hash verification. It implements the owner's conditional normative acceptance, not functional implementation permission. No prior accepted document or historical proposal is edited.

Accept ONLY T0–T6 Revision2 reproduced byte-for-byte below from docs/integration/server-tls-identity-layout-review-20261020/PERSISTENCE-LAYOUT-REVISION2-PROPOSED.md. Whole-source SHA256 0042fe329962d9f713425addf331f0bf5641f790810bc657a12fe0d328eab875; incorporated byte interval [170,23140), length22970; exact clause-byte SHA256 97989488846f67334b4eee3bc768b250e8f8db3102105951f7aa5156202430d4. The boundary starts at ## T0 and ends immediately before ## Revision provenance. Source frontmatter/title/informative provenance, original Revision1, correction hunks, review prose and implementation artifacts are NOT accepted wholesale or by functional reference. Original files and their spelling/formatting bytes remain preserved.

Historical wording PROPOSED/not-accepted in the incorporated source is superseded ONLY as to normative acceptance of these exact clauses by this record. No prohibition of implementation, signature, CURRENT, recovery, admission or qualification is lifted. References to S1–S5 mean the already accepted DECISION-server-tls-expected-name-source-20261019.md; their authority/scope is unchanged.

T1's offsets.csv reference is pinned to docs/integration/server-tls-identity-layout-review-20261020/offsets.csv, SHA256 e0a67b1332556e3dab5c475d66ba8ac678f56288a68ca1ced2af0d1e225e556c. The exact matrix is reproduced in the annex as the field map already referenced by T1, not an independent new source of semantics or authority. Fixed values/presence/relationships are those explicitly defined by T0–T6 and their unchanged accepted D3/LST1 references. No future edit of that source matrix is accepted by this hash-pinned incorporation.

## Delimited precedence and closed gates

This specializes only the NEW PACROOT8/version2, PACSNT01/PACSNI01/version1 and PACR8CP2/version2 encoding, validation and correlation rules under S1–S5. The prose label ROOT08/v2 does not define a second magic; exact bytes are ASCII PACROOT8 and version2. R1–R4 are incorporated as part of T3: X509 certificate profile is distinct from actual channel TLS security; approval covers the complete source/name/listener/TLS/issuer tuple; shared security_ref rows obey the stated coherence equality; hostname representation follows the exact bounded predicate without granting approval or claiming general IP classification.

Existing PACROOT8/version1, CP1, ROOT06, BODY0.5 and earlier BODY versions, LST1, PACICP01 and historical codecs/readers/hash domains stay byte-exact. Newly defined fields apply ONLY under the new version discriminator. No old reserved bytes, silent migration, truncation or fallback is permitted. First v1->v2 adoption still requires explicit authorized same-root transition and qualified adapter, not automatic dispatch. No second root, registry, catalog, journal, worker, epoch, counter or issuer is introduced.

The proposed format's update-only scope is accepted as a limitation: initial bootstrap/version2 remains UNSUPPORTED. Existing bootstrap contracts are unchanged. Required issuer carrier remains UNSUPPORTED IN CURRENT CODE until separate qualified adapter integration; this acceptance cannot enable it, omit its records, reset high-water, confuse its historical configuration anchor with policy or authorize native issuer work. A valid encoding or name-binding stage does not authorize a request/signature or reconstruct missing authority/material.

ADR0006/0009/0010/0028, accepted CA/profile/source/provisioning, root/C8 and issuer/C2 I/R/A/P/CAS, immutable Session snapshots/pins, Model E/ED08/ED11 retain authority. No new OPEN/BIND or other CURRENT effect may use historical approval or pins as new authority. Source leases remain distinct from root pin protection. Name/security change is not presumed issuer-only; actual operational projection determines the existing P transition. Active Session revocation disposition remains POLICY_PENDING. Serial reservation/state/history and uncertain-result retention are not replaced by association stages.

Implementation, public-source/native lifetime proof, complete managed/transient/native coexistence accounting and all native qualification remain separate. Serialized sizes20512..28960 and SNT1 limits/masks are format constraints, NOT an ED11 reservation or a claim of feasibility. Runtime storage delta of this normative task is0; cap2097152 unchanged, no automatic use of margin/stack reserve or removal of retained credit. Provider/kernel UNKNOWN; ED11 total PARTIAL. No hostname, trust, Principal/grant or actual authority is selected. No operational configuration/listener, CURRENT/epoch, CA/key/certificate, private signature, network, PST publication or Phase6 completion is authorized.

IMPLEMENTATION_AUTHORIZED=NO
CARRIER_ISSUER=UNSUPPORTED
ED11_RESERVATION=NOT_APPROVED
PROVIDER_KERNEL=UNKNOWN
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
CONFIG_CURRENT_PUBLICATION=DENY
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION

## Exact incorporated T0–T6 Revision2

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

R3 explicit coherence: within one containing root revision, rows sharing the exact security_ref must agree byte-for-byte on transport/role, certificate profile, name length/name bytes, issuer authority/generation/CA-SPKI, association stage and all conditionally present request/CSR/SPKI/certificate fields. Listener ordinal/ID and the approval revision for that listener association may differ; every row still requires its own factual association proof. The fields describe ONE normalized Local Identity at that source/revision, not per-listener alternate credentials hidden behind one reference. Any divergence rejects the whole component before exposing a binding; no first-row/default selection. Retained prior root/source snapshots are different immutable generations, not an exception allowing divergent rows in a single selected revision.

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
|32|8|name_approval_revision: existing root revision approving the exact source/name/listener/TLS/issuer tuple defined in R2; nonzero <= containing R|
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

R1 profile distinction: record20:2 identifies ONLY the X509 technical certificate profile1. It is not a TLS transport/security-profile identifier and cannot satisfy that check. In this bounded version, transport1/role1 plus the exact security_ref and its verified source/root revision must resolve the existing ADR-0006 channel security profile: TLS1.3-only, mutual certificate authentication, mandatory exact ALPN papacc/1, 0-RTT/resumption disabled and the remaining accepted security requirements. Verify these against the actual immutable normalized security configuration before RESOLVED or consumption, independently of certificate KU/EKU/algorithm checks. Certificate profile1/serverAuth alone proves none of those transport facts. Missing/incompatible/unqualified channel TLS configuration returns DENY/UNSUPPORTED, never an inferred profile. No new profile catalog, default, enum or bytes are introduced. Different future channel profiles require a separately reviewed format/support contract.

The name is the explicit public attribute selected through authorized administrative configuration, not parsed as approval from config text. The first bounded representation supports the existing offline server builder's exact byte predicate: total length1..253; each byte belongs to ASCII A-Z, a-z, 0-9, '-' or '.'; every dot separates nonempty labels of length1..63; a label must neither begin nor end with '-'; the final byte is not a dot; and at least one byte in the whole name is an ASCII letter or '-'. Thus an all-digits-and-dots name is rejected, as in the current public builder. No wildcard, embedded NUL, whitespace, non-ASCII, colon or other byte is accepted. This is a DNS-label representation check, not a resolver for every historical IPv4 spelling or a claim to detect every possible address alias; no IP literal alternative is encoded or inferred. Runtime hostname/identity applicability and target support remain separate fail-closed checks. Preserve exact bytes/case, no normalization, IDNA, DNS lookup, CN alias or implicit additional SAN. Unsupported naming representations reject, never approximate. This serialized bound reflects the already implemented target subset; it is NOT a new approval, quota, freshness or global name policy.

name_approval_revision is provenance in the SAME root's committed history, not a new monotonic counter. R2 approval tuple: the provenance covers the exact listener identity, security_ref, normalized channel TLS profile/source content, transport/role, certificate technical profile, name bytes and issuer authority/generation/CA-SPKI association. On introducing or changing ANY of those attributes it must name the transaction R that explicitly approved that new tuple; on retaining an unchanged tuple it refers to its prior verified approval revision. An unchanged hostname alone is insufficient to reuse approval after issuer rotation or source/profile replacement. The revision is not proof of the transaction: verify exact authorized content/history and actual source ownership before consumption. Request/result attachment alone does not create a new naming approval when this tuple remains unchanged; it still requires the separately authorized exact request/reservation/result transaction and its current revision. It cannot reuse old request authority, clear pending/uncertain work or change an already reserved request's CSR/SPKI/issuer/serial. Keep the old immutable association/leases and credits for any old dependent consumer until factual quiescence; a new tuple never retags an old request or Session as CURRENT. Decode checks nonzero/range only; actual authority must verify the complete approved content and its unchanged linkage through history/source snapshots. A <=R value, hash or historical pin alone cannot prove approval/current applicability. Removing/revoking approval prevents new effects; old dependency retirement remains Model E/active-Session policy, not a consequence of zeroing a row.

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

## Hash-pinned offset matrix annex

Verbatim matrix referenced by T1; interpreted with the exact T0–T6 fixed-value and relationship constraints above.

```csv
structure,offset,bytes,end_exclusive,field
checkpoint_v2,0,8,8,PACR8CP2
checkpoint_v2,8,2,10,version
checkpoint_v2,10,2,12,size
checkpoint_v2,12,4,16,presence
checkpoint_v2,16,32,48,domain
checkpoint_v2,48,8,56,R
checkpoint_v2,56,8,64,predecessor
checkpoint_v2,64,8,72,A
checkpoint_v2,72,8,80,P
checkpoint_v2,80,8,88,I
checkpoint_v2,88,32,120,C
checkpoint_v2,120,32,152,C_base
checkpoint_v2,152,32,184,W_base
checkpoint_v2,184,296,480,reserved
checkpoint_v2,480,32,512,Hcp
header,0,8,8,magic
header,8,2,10,version
header,10,2,12,header_bytes
header,12,4,16,total
header,16,4,20,flags
header,20,2,22,N
header,22,2,24,descriptor_bytes
header,24,2,26,body_bytes
header,26,2,28,hash_algorithm
header,28,2,30,checkpoint_bytes
header,30,2,32,base_lst_version
header,32,4,36,collection_offset
header,36,4,40,collection_length
header,40,4,44,lst_offset
header,44,4,48,lst_length
header,48,4,52,issuer_offset
header,52,4,56,issuer_length
header,56,4,60,Q_checkpoint_offset
header,60,4,64,checkpoint_length
header,64,4,68,digest_offset
header,68,4,72,digest_length
header,72,32,104,domain
header,104,8,112,R
header,112,8,120,predecessor
header,120,8,128,I
header,128,4,132,presence
header,132,2,134,body_codec
header,134,2,136,descriptor_version
header,136,8,144,R_base
header,144,8,152,A_base
header,152,8,160,P_base
header,160,8,168,I_base
header,168,4,172,base_presence
header,172,4,176,reserved
header,176,32,208,C_base
header,208,32,240,W_base
header,240,2,242,prior_kind
header,242,2,244,component_version1
header,244,4,248,component_offset_O
header,248,4,252,component_length_S
header,252,2,254,K
header,254,2,256,component_record_bytes768
header,256,2,258,hash_algorithm1
header,258,254,512,zero_reserved
record,0,4,4,presence
record,4,1,5,profile
record,5,1,6,admission
record,6,1,7,binding
record,7,1,8,authkind
record,8,2,10,port
record,10,2,12,id_length
record,12,2,14,security_length
record,14,2,16,auth_length
record,16,2,18,policy_length
record,18,2,20,ids_count
record,20,4,24,reserved
record,24,64,88,id
record,88,64,152,security_ref
record,152,64,216,auth_ref
record,216,64,280,policy_ref
record,280,32,312,policy_identity
record,312,32,344,BODY_sha256
record,344,8,352,PACCD001
record,352,32,384,reserved
record,384,128,512,persistent_ids
record,512,1024,1536,BODY05
sni_record,0,8,8,PACSNI01
sni_record,8,2,10,version1
sni_record,10,2,12,bytes768
sni_record,12,4,16,presence15_31_63
sni_record,16,2,18,listener_ordinal
sni_record,18,1,19,Secure1
sni_record,19,1,20,ChannelServer1
sni_record,20,2,22,certificate_profile1
sni_record,22,1,23,stage1_2_3
sni_record,23,1,24,zero
sni_record,24,2,26,listener_length
sni_record,26,2,28,security_length
sni_record,28,2,30,name_length
sni_record,30,2,32,zero
sni_record,32,8,40,name_approval_root_R
sni_record,40,16,56,issuer_authority
sni_record,56,16,72,issuer_generation
sni_record,72,16,88,request_id_or_absent
sni_record,88,32,120,csr_digest_or_absent
sni_record,120,32,152,server_spki_digest_or_absent
sni_record,152,32,184,certificate_digest_or_absent
sni_record,184,64,248,listener_id
sni_record,248,64,312,security_ref
sni_record,312,256,568,expected_name
sni_record,568,32,600,CA_spki_digest
sni_record,600,168,768,zero_reserved
snt_header,0,8,8,PACSNT01
snt_header,8,2,10,version1
snt_header,10,2,12,header64
snt_header,12,4,16,S
snt_header,16,2,18,K
snt_header,18,2,20,record768
snt_header,20,4,24,flags0
snt_header,24,32,56,containing_domain
snt_header,56,8,64,containing_R
```

## History

Revision1,2026-10-20: exact T0–T6 Revision2 accepted through owner conditional authority and official executor registration, under the15 reviewed conditions. Original proposals, correction patch, acceptance draft and accepted historical formats preserved. No functional hunks accepted by reference or applied.
