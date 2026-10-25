---
status: proposed
revision: 1
scope-level: project
scope-target: canonical Roughtime source policy and private persistence extension only
---
# TS0–TS8 — canonical TIME_SOURCE binding (PROPOSED)

Owner task PACC-PHASE6-ROUGHTIME-CANONICAL-SOURCE-BINDING-1 authorizes preparation only where existing representation is insufficient. These clauses are NOT ACCEPTED. No implementation, migration, source, endpoint, provider, credentials, CURRENT, temporal quality, budget reservation or activation is approved. Existing AT/PT, Q0-Q7, U0-U7, E0-E8, root/C8, Model E and ED08/ED11 retain authority. This is a separate focal layout/administrative-purpose proposal, not an extension of RFC wire or PACKAGE_SIGNING.

## TS0 Authority and minimal boundary

TIME_SOURCE is an explicit administrative policy purpose for a bare Ed25519 verification key, distinct from all issuer certificate purposes and PACKAGE_SIGNING. It is not a certificate, CA, issuer generation, capability, grant or user identity. The existing canonical administrative root/policy owns configuration and disposition; the same owner/producer/worker/CAS/snapshot/pin machinery must be extended after acceptance. No registry, trust store, journal, clock service, worker or independent status authority is added. No data is read from discovered lists or Roughtime replies to enroll a source.

The proposed first representation is PACROOT8/version4 with a public TSC1 source collection and CP4, preserving all older versions byte-for-byte. Only explicit UPDATE from factual version3 or version4 with complete checkpoint/predecessor/CURRENT/pin is supported. No direct version2 upgrade, bootstrap, authoritative recovery, automatic migration or downgrade dropping sources is authorized. Version2 must first follow its separately accepted path to version3. All missing dependencies deny. Native transport remains unimplemented until a later exact approved patch and gates.

## TS1 Source tuple and administrative act

Each source tuple comprises root identity, source ID, independent administrative-party ID,32-byte raw long-term Ed25519 key and SHA256 fingerprint, TIME_SOURCE purpose, RT1-UDP-2048 profile, exact numeric unicast endpoint/port5319, policy identity, disposition root revision, validity interval, disposition status, provenance-evidence reference, operator identity and administrative-act reference. All IDs/fingerprints are exact binary identifiers, never authority merely because nonzero. Identity references must resolve unambiguously to protected existing administrative evidence. Missing producer/reference resolution denies; no new evidence store is implied.

An existing authenticated administrative entry must obtain explicit TIME_SOURCE configuration authorization for the entire candidate tuple and complete predecessor. Authentication alone, bootstrap authorization, U0-U7 approval limited to SNT/PDG, an issuer key, a package signature, a digest or a boolean does not authorize this act. This proposal adds only that focal operation to the existing administrative authorization flow; no general identity-provider API is created. The exact native producer/binding patch remains separately reviewed. A currently unavailable source of operator/authorization/provenance is not filled by test data.

The initial key and expected SHA256 fingerprint arrive through an independently authenticated administrative ceremony, with protected evidence of operator authority, origin, source independence and current applicable root/policy. Compare SHA256 of exactly32 raw key bytes before proposing installation. Mathematical validity/correspondence is checked with an existing qualified primitive backend; no custom crypto. The source does not authenticate its own initial key. No TOFU, candidate response, password, OS clock or LAB key establishes production trust.

Statuses0 UNKNOWN,1 AUTHORIZED,2 WITHDRAWN,3 REVOKED describe persisted policy disposition only. UNKNOWN, absent/expired/revoked/withdrawn or unresolved evidence denies. AUTHORIZED bytes still require live authority/status and applicability. Source calibration/health/revocation freshness are independently checked, not inferred from this enum or a valid signature. Retained history cannot restore an old favorable disposition.

## TS2 Validity, bootstrap and status without circularity

Stored not_before/not_after are unsigned UTC seconds with0<not_before<not_after, applicability [not_before,not_after). Actual source effects require full inclusion of the conservatively qualified UTC interval in this validity domain, and separate current administrative status/provenance. No skew, backdating or saturation.

First-contact mathematical RFC delegation verification uses the authenticated response timestamp under the independently installed bare key, as AT2 already permits. It does not establish administrative status, freshness or actual UTC. Initial source authorization/status must be independently applicable without relying solely on that response. If checking administrative expiry requires UTC, obtain independently qualified evidence; otherwise DENY. This proposal deliberately does not manufacture a timeless status, alternate local clock, bootstrap approval or date bypass. Persistent validity fields and intact CP4 are not that independent evidence. This may leave initial positive source qualification blocked even after codecs exist.

Rotation/replacement/revocation/withdrawal requires explicit new canonical disposition and candidate-bound authorization. Change of key, endpoint, purpose, party, policy, validity or evidence applicability invalidates affected operations before activation. No automatic key overlap, fallback or selecting an older authorized tuple. Source ID may remain stable under explicitly recorded rotation; complete predecessor/history preserves the old tuple. Reintroduction after withdrawal/revocation requires a new act, not reuse of a historic revision. Removed rows deny future use; existing root history must remain preserved, without sidecar logging. Restart restores history only; status, CURRENT, pins, observation, entropy and administrative applicability do not return automatically. UNKNOWN remains DENY.

## TS3 Byte-exact TSC1 collection and TSR1 records

All new integers are unsigned LITTLE-ENDIAN. No native struct serialization, pointers, handles, padding, private keys or platform IDs. Intervals have exclusive end. Check every length/offset/product/sum before access. Hash is SHA256 using an existing approved public adapter. Hash failure denies; integrity never confers authority.

Let H=number of source records0..3; F=96+384*H (96..1248). Header64:0:8 ASCII PACTSC01;8:u16=1;10:u16=64;12:u32=F;16:u16=H;18:u16=384;20:u32=0;24:32=root domain;56:u64=R. Both domain/R equal the containing version4 image. Records at64+384*j, j0..H-1. SHA256(collection[0,F-32)) at[F-32,F). No gaps or trailing bytes. H0 retains96 bytes, means no configured temporal source, and denies acquisition.

Each TSR1 record is exactly384 bytes:

| Offset | Bytes | Field |
|---:|---:|---|
|0|8|ASCII PACTSR01|
|8|2|version1|
|10|2|record bytes384|
|12|4|presence1, all other bits zero|
|16|2|status0 UNKNOWN /1 AUTHORIZED /2 WITHDRAWN /3 REVOKED|
|18|2|purpose1 TIME_SOURCE in this private component only|
|20|2|profile1 RT1-UDP-2048, accepted PT0-PT7|
|22|2|address family4 IPv4 /6 IPv6|
|24|32|source identity|
|56|32|administrative-party identity|
|88|32|raw Ed25519 long-term public key|
|120|32|SHA256 of exactly raw key bytes[88,120)|
|152|16|numeric address bytes in network order|
|168|2|UDP port5319|
|170|6|ZERO|
|176|32|policy identity|
|208|8|disposition root revision,1..R|
|216|8|not_before UTC seconds|
|224|8|not_after UTC seconds|
|232|32|provenance-evidence identity reference|
|264|32|administrative-act identity reference|
|296|32|operator identity reference|
|328|56|ZERO|

Every identity/key/digest/evidence/act/operator reference is nonzero; exact key/fingerprint correspondence is mandatory. Rows sort lexicographically by unsigned source ID, strictly increasing, no duplicates. The first subset uses scope-free numeric unicast only: IPv4 first4 address bytes followed by12 ZERO, no unspecified/multicast/limited broadcast; IPv6 uses all16 bytes, no unspecified/multicast/IPv4-mapped or scope-required link-local addresses. Directed broadcast and deployment-specific endpoint admissibility require factual administrative endpoint validation; syntax is insufficient. No DNS, URI, address fallback or port override. A deployment requiring scoped IPv6 is UNSUPPORTED under this first proposed representation, not converted to another address. This is a proposed availability subset, not a change to RFC wire.

Policy ID must refer to the actual canonical policy containing the disposition, not an issuer policy alias. Source/party/evidence/act/operator IDs must be resolved by the existing administrative mechanisms with protected lifetime; matching32-byte strings does not resolve them. A campaign requires exactly three applicable authorized sources with distinct source IDs, full keys, endpoints and administrative-party identities, plus actual proof of independent operation/calibration. Merely different IDs do not prove independence. Acquisition uses accepted randomized A,B,C order and exactly six exchanges, not record-order authority. H<3 or any invalid source denies; no substitution/retry/favorable selection.

## TS4 PACROOT8/version4 composition and complete header

Retain G1 definitions N1..4, B1..4, K1..N, D0..K and all descriptor/BODY/SNT/PDG/issuer predicates. Define L=160+512*B, J=16384, O=512+1536*N, S=96+768*K, U=O+S, V=96+384*D, X=U+V, F as TS3, Q=X+F+L+J, T=Q+544. Image size20704..31840. Regions: header[0,512), descriptor/BODY collection[512,O), SNT1[O,U), PDG1[U,X), TSC1[X,X+F), predecessor LST1[X+F,X+F+L), issuer[X+F+L,Q), CP4 or512 ZERO[Q,Q+512), final digest[Q+512,T). Regions are contiguous without padding, overlap or trailing bytes.

Header512 is enumerated by the accepted G2/version3 matrix, with the following complete overrides ONLY for version4; every inherited field retains its stated size, endian, purpose and zero rules:

*0:8 PACROOT8;8:u16=4;10:u16=512;12:u32=T;16:u32=3 UPDATE+ISSUER;20:u16=N;22:u16=512;24:u16=1024;26:u16=1;28:u16=512;30:u16=1.
*32:u32=512;36:u32=1536*N;40:u32=X+F;44:u32=L;48:u32=X+F+L;52:u32=16384;56:u32=Q;60:u32=512;64:u32=Q+512;68:u32=32.
*72:32=domain;104:u64=R;112:u64=predecessor;120:u64=I;128:u32=31;132:u16=5;134:u16=1;136:u64=R_base;144:u64=A_base;152:u64=P_base;160:u64=I_base;168:u32=15;172:4 ZERO;176:32=C_base;208:32=W_base.
*240:u16=prior_kind3 or4 matching the full predecessor;242:u16=1;244:u32=O;248:u32=S;252:u16=K;254:u16=768;256:u16=1;258:u16=1;260:u32=U;264:u32=V;268:u16=D;270:u16=384;272:u16=1.
*274:u16=1 TSC version;276:u32=X;280:u32=F;284:u16=H;286:u16=384;288:u16=1 SHA256;290:u16=0 flags;292:220 ZERO.

R=predecessor+1 without wrap; root domain and R/base R/A/P/I/C/W retain all G2/G4 factual relations. I>=I_base and equal to actual carrier revision, never fabricated for a policy update. The inherited source is the immutable accepted G0-G7 frozen clauses and offset matrix, not a future edit of a proposal. SOURCE identity is independent of issuer identity; issuer carrier, high-water, reservations, revocation and history remain complete and mandatory. Source rows neither replace nor alter issuer records. No old reserved bytes are used under version3,2 or1. Historical readers reject version4, without stripping TSC1 or rewriting as version3.

## TS5 Hashes, CP4, predecessor and transaction

C4=SHA256(image[0,Q)), including complete TSC1 and every inherited component. W4=SHA256(image[0,Q+512)), stored at[Q+512,T). TSC1 digest is specified in TS3; SNT1/PDG1/ROOT06/BODY/LST bytes and their digest rules remain historical. No digest includes itself.

Absent CP is512 ZERO. Present CP4 is exactly512 bytes:0:8 ASCII PACR8CP4;8:u16=4;10:u16=512;12:u32=1;16:32=domain;48:u64=R;56:u64=predecessor;64:u64=A;72:u64=P;80:u64=I;88:32=C4;120:32=C_base;152:32=W_base;184:296 ZERO;480:32=SHA256(CP4[0,480)). Epochs A/P are factual nonzero post-publication values only. C_base/W_base select the same complete predecessor version3/CP3 or version4/CP4, not a reconstructed fragment or transfer hash. No checksum establishes CURRENT/latest revision/fencing.

The existing producer PREPARE holds full immutable candidate, source dispositions/evidence, authorization, predecessor and actual CURRENT pin. VALIDATE resolves all inherited and source references, status and lifetime with provenance. DATA/MODEL_BOUND never becomes RESOLVED by codec success. Existing worker commits CP ZERO using full-predecessor/native-identity CAS. Only a separately authorized factual durable commit and publication can later produce CURRENT/A/P and checkpoint-only CAS. P preservation still requires accepted operational-projection proof, never a label that time settings are nonoperational. No positive commit/publication is authorized here. CAS_CONFLICT and UNKNOWN never fallback; UNKNOWN retains both generations/credits until factual closure. Restart/archive integrity never restores authority or authorizes recovery.

## TS6 Binding, pins and lifetime

Runtime binding is a synchronous borrow of previously protected immutable source tuples and separately protected operator/policy/status/provenance objects under the existing owner. The caller already owns protection before asking for a view. Root pin protects root-owned source bytes, not external evidence automatically. Binding must identify actual root, CURRENT/R/A/P/I, source/profile/disposition revisions, owner/operation, exact endpoint/key, status applicability and every external protection. Missing references, expired/stale pin, changed CURRENT/revision, divergent key/endpoint/purpose, revocation or UNKNOWN denies. No fabricated lease, boolean proof or new asynchronous acquire exists.

Stop prevents new sends/effects immediately and invalidates use, while owners retain buffers/backings/pins/credits and external borrows until factual quiescence/closure. Timeout, failed cancel or process exit does not release resources or prove DESTROYED. Existing Model E/exactly-once and E0-E8 RELEASE_INVALID (no attempt/effect; borrow preserved) remain intact. E0-E8's package-purpose evidence is not repurposed into positive TIME_SOURCE authority; only its lifecycle constraints are reused. A later private API patch must make its distinct purpose and source provenance explicit before implementation. No general trust-provider API is defined by this proposal.

## TS7 ED11, errors and compatibility

TSC1 representation96..1248 bytes, each full version4 buffer grows by F versus the otherwise equivalent version3 image. Maximum serialized image31840, checkpoint512 unchanged. Independent3/4/6 full-image buffers are95520/127360/191040; deltas versus version3 maxima are3744/4992/7488. These are conservative array arithmetic, not runtime deltas/reservations. Embedded TSC1 bytes are already included in the image; add only actual separate copies. Source evidence, key/backend storage, snapshots/pins, old/candidate generations, hash/encode/decode/CAS staging, stack, crypto, native handles/queues and retained concurrency are separately censused before hunks/effects. Existing PT source-descriptor1536 ceiling is not free credit or proof that external evidence fits.

Runtime additional allocation in this proposal=0. No codec, owner slot or worker buffer grows now. Prior isolated406300 heap measurement and analytical2522663/2876059 scenarios do not qualify version4 coexistence. Windows3145728 only explicit selection, legacy2097152 and individual quotas unchanged. Provider/kernel/transitive stack/source costs remain UNKNOWN, ED11 PARTIAL. Missing bound or over-budget graph stops dependent implementation.

Reject wrong/unknown magic/version/status/purpose/profile/family/flags, invalid presence/zeros/length/count/order/endpoint, malformed key, hash mismatch, overflow, gap/overlap/truncation/trailing bytes, mismatched root/revision/policy/evidence, incomplete predecessor/history or unsupported shape. UNKNOWN disposition is structurally recognized but semantically DENY; it is never AUTHORIZED by default. No partial usable result, rehash repair, issuer omission, ancestor fallback or silent truncation. Old formats and wire remain byte-exact, no Browser/PST dependency or changes. C89/VC6 portability is a later build/implementation proof, not granted by this layout.

## TS8 Review and implementation gates

Review TS0-TS8 including its new administrative purpose, first scope-free address subset and version4/CP4 before acceptance. Preserve this proposal and all prior accepted bytes. No automatic ACCEPTED or functional patch follows. After exact official normative acceptance, independently reviewed source codecs/owner/producer patches, prepatch lifetime/capacity proof, offline negative tests and protected factual source/environment qualification are still required. Lab fixtures cannot establish positive authority. Native transport/UTC/issuer/UPDATE/CURRENT/signature/recovery/admission remain DENY/NOT_QUALIFIED. This proposal selects no deployment source or address and generates no key/certificate/packet.
