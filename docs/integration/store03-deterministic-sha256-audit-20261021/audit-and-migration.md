# store03_hash deterministic SHA-256 audit

Task PACC-PHASE6-STORE03-DETERMINISTIC-SHA256-AUDIT-1, 2026-10-21.
HEAD/main 913c90cf2328f7939d8f5b927640601903ac6e91. Read-only code audit,
no build, test, source extraction, network or functional modification.

## Finding and recommendation

Recommend qualifying an Accelerator-owned, versioned minimal BearSSL SHA-256
source snapshot, using the implementation already exercised by the Browser.
This is a candidate recommendation, not accepted dependency selection or proof
of the Accelerator adapter. No deterministic standalone SHA-256 implementation
is presently compiled/incorporated in Accelerator product source. Its existing
public hash adapter uses OpenSSL EVP/provider; server hashes use BCrypt or
CryptHashCertificate2. Callback-based codecs are not independent SHA algorithms.

The Browser has concrete reusable source at
C:/Projetos/PapinhoBrowser/third_party/bearssl/src/hash/sha2small.c.
Functions: br_sha256_init, br_sha256_update (macro alias of br_sha224_update),
br_sha256_out; internally sha2small_update, sha2small_out, br_sha2small_round.
External mathematical helpers: br_range_dec32be from src/codec/dec32be.c and
br_range_enc32be from src/codec/enc32be.c. Inline br_dec32be, br_enc32be and
br_enc64be are in src/inner.h. The implementation uses memcpy/memset, fixed-width
integer operations and const IV/round constants/vtables. No malloc/free,
provider, OS handle, lazy provider initialization or crypto service is called
in this inspected source closure.

Build/header closure still needs a minimal package pin: inner.h, config.h,
bearssl.h and its transitive public headers (including bearssl_hash.h), plus
standard types/string headers. Including declarations does not require linking
all TLS/RSA objects. Prefer compiling the unchanged three source units and
preserving their original headers/license instead of copying/reimplementing
round arithmetic or importing the full BearSSL runtime. Prove actual linked
symbol closure, including compiler-generated integer helpers, before acceptance.

Browser source is available for inspection/reuse, not an Accelerator build-time
checkout dependency. Its LICENSE.txt is MIT with Thomas Pornin attribution and
redistribution notice. Preserve license and source notices. The local VC6 shim
identifies the vendored code as BearSSL 0.6; official upstream revision/archive
provenance was not established by this offline audit. Pin exact source hashes
(input-hashes.json), reconcile upstream provenance/version and authorize a
versioned Accelerator package before ingestion. No Browser file was copied into
product source and no Browser/PST library or SDK may silently supply it.

## Existing qualification and alternatives

Browser Makefile.nt4.mak explicitly builds BearSSL with VC6-compatible includes,
third_party/compat/vc6/stdint.h and forced vc6_compat.h (inline -> __inline).
The integer shim uses __int64 and exact Win32/x86 typedefs. It must NOT be
reused unchanged as a general modern 64-bit/platform stdint.h replacement.
Browser src/papinho_papacc_public.c and papinho_papacc_content.c use br_sha256;
papinho_papacc_crypto.c uses BearSSL HMAC/SHA256. Its documented actual VC6
Debug/Release crypto proof has 940 checks/configuration, including independent
PBKDF2 comparisons. This supports practical VC6 feasibility, not standalone
store03 equivalence, strict ISO C89 qualification, NT4 runtime or box/ISA proof.
No compilation or regression was run in this audit.

Accelerator alternative: independently acquired pinned OpenSSL 3.5.8 archive,
SHA256 a8f84a39918ec6415ce765d9b429d313ba97b8143169c172e734b9514464f5b2,
already available under third_party/openssl-math/source. Its crypto/sha/sha256.c
has SHA256_Init/Update/Final and a portable compression path through
crypto/md32_common.h, internal endian/config/crypto headers and architecture
selection. It could support a separately qualified no-provider subset, but it
is not extracted/qualified and has a larger header/config/assembly-dispatch
closure than the already-used BearSSL candidate. Existing accepted OpenSSL math
subset is Ed25519-only, not authority to extend it to generic hashing. Existing
EVP_Q_digest keeps provider resources and does not solve this task. No PST
archive, source modification, binary or transitive runtime is required.

No second new library is selected. An additional external implementation or
network acquisition is unnecessary to establish this local recommendation.

## Equivalence and edge contracts

Both candidates implement ordinary SHA-256, so unchanged input bytes/domains
should produce the same 32-byte output as BCrypt. This remains expected
mathematical equivalence, not differential proof. Preserve original offsets,
lengths, padding and persisted hashes in ROOT06/v2/v3/v4 and all CP/PDG/TSC
components. No format migration or on-read rehashing of historical bytes.

BearSSL supports init/update/out streaming. Zero-length update explicitly ignores
the pointer, including NULL, and empty finalization is supported. Adapter must
reject NULL with nonzero size, NULL output and invalid ownership/state before
work. Its current unsigned length must remain byte-exact; future larger streaming
inputs require checked total length below 2^64 bits (byte-aligned maximum
2^61-1 bytes), checked conversions and consistent chunk accumulation. No message
sized allocation is needed. Library void APIs do not report input/Stop errors;
the private adapter must enforce those contracts and stage output itself.

Retain existing owner exclusion/Stop/retention gates. Process bounded chunks,
observe Stop before/during/after work and before publishing the staged digest;
freeze a specific chunk/poll policy in the later patch, not by this audit.
Synchronous input borrow lasts through return; there are no backend callbacks or
native pending hash objects. Keep caller/candidate buffers and other uncertain
worker resources retained until factual quiescence. Deterministic hashing does
not authorize clearing old BCrypt retained references or disposing other owners.

Required future differential vectors: empty, abc, standard multi-block/million-a,
55/56/63/64/65-byte padding boundaries, split streaming, large lengths and
conversion limits, aliases where currently allowed, malformed spans, Stop and
no output on error. Compare against existing BCrypt and independent public KATs;
then historical codecs/CAS conflict/UNKNOWN/retention. No fixture authorizes trust.

## Deterministic ED11 components, not a qualified peak

BearSSL context fields: vtable pointer + buf[64] + count uint64 + val[8] uint32.
Field payload is pointer size +104 bytes before ABI padding. Expected x64
context 112 bytes; actual VC6 and modern sizeof/alignment must be measured.
Compression local w[64] is 256 bytes, a..h are 32 bytes, plus index/temporaries.
Finalization owns buf[64]+val[8] =96 bytes plus ptr and calls compression.
Update owns ptr/clen/pointers and calls compression; finalization has the larger
named local array set. A wrapper adds staged output[32], scalar state and any
chosen context storage. These nested live frames must be summed or covered by
a proved enclosing reserve, never called zero. No unbounded recursion or
input-sized arrays appear in the inspected closure.

Source constants: two IV arrays 64 bytes together, K[64] 256, two hash-class
vtables with ABI-dependent size/relocations. Code size, alignment, compiler spill,
CRT memcpy/memset frames, compiler 64-bit helpers, initialization and full caller
stack remain UNMEASURED. No total 512-byte or 1 KiB stack promise is made from
this source inventory. No dedicated crypto-provider heap/handles are required
by this source closure; process CRT/kernel baselines and unrelated issuers still
exist and are not zero. Runtime qualification must verify actual imported and
undefined symbols and measured code/data/stack for each target.

Local context can have synchronous stack lifetime; embedding instead requires
counting real per-owner multiplicity. Do not subtract the existing 80 bytes of
ownership contexts automatically: their gates and other users still exist.
No savings or new complete ED11 margin is claimed. Previous Selected known
1,952,704 and v4 minimum projection 1,956,448 are historical ledger starting
points, not totals with this unimplemented alternative. Quotas unchanged.

## Scope boundary and minimal migration plan

store03_hash currently delegates to papacc_store_hash. The same helper also
serves digest(record,480) and private codec users. Blindly replacing the shared
helper would exceed an exclusively store03_hash authorization. The minimum
future patch must explicitly enumerate the store03 caller closure and use a
private deterministic helper without silently rerouting issuer/legacy APIs.
Nested record validation can still invoke digest and hence BCrypt. Independent
store02_hash and issuer_hash06/CryptHashCertificate2 paths also remain. Thus
removing direct BCrypt from store03 alone does NOT establish a provider-free
whole worker or resolve all ED11 resource obligations. Broader closure needs a
separate scoped owner decision; no general crypto audit/refactor is proposed.

1. Owner decides the private backend/package boundary, exact source/provenance,
   license and treatment of shared legacy digest dependencies. Record a focused
   normative dependency decision before implementation if this durable backend
   choice is approved; existing Ed25519 acceptance does not authorize it.
2. Prepare minimal unchanged source/header snapshot, private adapter and build
   selection with hashes, no cross-checkout dependency or silent fallback.
3. Prove symbol/allocation closure, VC6/C89 and modern Debug/Release compilation,
   standard/differential vectors and historical codec/Stop/lifetime equivalence.
4. Measure full nested stack, code/data/init, simultaneous owner multiplicity and
   retained coexistence; reconcile ED11 before worker-v4 integration.

No change to the SHA algorithm or persisted format is necessary. Backend and
source ownership/distribution selection require owner approval and exact executor
review. Implementation remains unauthorized in this audit. First unresolved
choice: approve a pinned minimal BearSSL snapshot and exact store03/shared-digest
scope; standalone Accelerator equivalence and resource measurement then follow.
