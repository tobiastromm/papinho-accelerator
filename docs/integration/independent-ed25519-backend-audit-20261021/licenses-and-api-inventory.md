# Pinned evidence, APIs and dependencies

Exact versions/commits/archive/file hashes: [evidence-provenance.json](evidence-provenance.json).
OpenSSL source functions/ranges/per-body hashes: [openssl-math-inventory.json](openssl-math-inventory.json).
No downloaded source is incorporated into product code. No provider selected.

## Official references consulted

- OpenSSL3.5.8 release: https://github.com/openssl/openssl/releases/tag/openssl-3.5.8
- Tag metadata: https://api.github.com/repos/openssl/openssl/git/tags/090eec6d3628aa0520bdf2cf97b063fafc34e7be
- Source archive: https://github.com/openssl/openssl/releases/download/openssl-3.5.8/openssl-3.5.8.tar.gz
- Monocypher download/checksum: https://monocypher.org/download/ and https://monocypher.org/download/monocypher-4.0.3.tar.gz.sha512
- Monocypher API: https://monocypher.org/manual/eddsa
- Monocypher code: https://github.com/LoupVaillant/Monocypher/blob/4.0.3/src/monocypher.c
- Monocypher license: https://github.com/LoupVaillant/Monocypher/blob/4.0.3/LICENCE.md
- Monocypher changes: https://github.com/LoupVaillant/Monocypher/blob/4.0.3/CHANGELOG.md
- Historical Monocypher audit: https://monocypher.org/quality-assurance/audit
- libsodium API warning: https://doc.libsodium.org/advanced/point-arithmetic
- libsodium1.0.21 code: https://github.com/jedisct1/libsodium/blob/1.0.21-RELEASE/src/libsodium/crypto_core/ed25519/core_ed25519.c
- libsodium change record: https://github.com/jedisct1/libsodium/blob/1.0.21-RELEASE/ChangeLog
- libsodium release: https://github.com/jedisct1/libsodium/releases/tag/1.0.21-RELEASE
- donna header: https://github.com/floodyberry/ed25519-donna/blob/8757bd4cd209cb032853ece0ce413f122eef212c/ed25519.h
- donna internals/notice: https://github.com/floodyberry/ed25519-donna/blob/8757bd4cd209cb032853ece0ce413f122eef212c/ed25519-donna.h

Monocypher4.0.3 commit ab2b16dd619ad5f6979a4fbe69cfa324a6fcc35f,
archive SHA2568cc9bc341a66249016db9bd70e9142d8d0aef9945973744b1ac05dbc55d8ee66;
SHA512 matches the official download checksum. Source/core/header/LICENCE and
ChangeLog hashes retained. BSD-2-Clause or CC0-1.0 choice is explicit; BSD
requires source/binary notice retention. Prior timing correction is not
mistaken for subgroup-validation support.

libsodium1.0.21 commit d24faf56214469b354b01c8ba36257e04737101e,
archive SHA2569e4285c7a419e82dedb0be63a72eea357d6943bc3e28e6735bf600dd4883feaf;
ISC notice retained by the earlier investigation. Corrected API checks y
canonicality, decode, curve, small order and main subgroup. Prior mixed-order
acceptance affects versions through1.0.20 as documented; exact stable revision
matters. Existing VC6 failure is historical evidence, not rerun here. This
research does not authorize a port or reverse NO_LIBSODIUM implementation scope.

OpenSSL3.5.8 commit f4dc4d58b48d346a8270183f89acf826d459b0ca,
archive SHA256a8f84a39918ec6415ce765d9b429d313ba97b8143169c172e734b9514464f5b2.
Local archive matches the expected canonical hash. Official GitHub tag API
reports verified=false/reason=unknown_key; do not claim independent signature
verification. That observation is not evidence the source is malicious or
that the recorded accepted hash changed. Before a new distribution, explicitly
approve provenance/signature verification under its own supply-chain gate.
Apache2 license text was read from this exact archive, not inferred from name.

## B exact arithmetic inventory

The33 bodies include field load/from/to bytes, zero/one/copy/add/sub/neg/mul/
sq/sq2/invert/pow22523/isnegative/isnonzero; ge decode/encode/cached/projective
conversion/add/sub/madd/msub/double; slide and double-scalarmult. See JSON
rather than copying a new component in this audit. Keep the generic int32[10]
field implementation, d/d2/sqrtm1, four64-bit masks and Bi[8]. The huge
k25519Precomp[32][8] basepoint-signing table is not referenced by this closure;
its exclusion must be verified mechanically, not rewritten or assumed from
linker stripping. Preserve necessary types, constant definitions and licenses.

Current compiled externals: CRYPTO_memcmp plus CRT memcmp/memmove/memset and
compiler security-cookie helper. Current full unit also includes ecx.h,
ec_local.h, evp.h, sha.h and internal headers for unrelated operations. Simply
copying curve25519.c without removing that coupling is not alternative B.
Proposed subset uses only known generic math/types/constants and reviewed
public-data comparison glue; no signature, hashing, RNG, provider/context,
Win32/CNG or PST is needed by the mathematical predicate. Demonstrate that
claim on the final link before any integration. This is a future verification
gate, not an undocumented dependency silently treated as present.

Monocypher public check_equation is unsuitable; private ge_frombytes_neg_vartime,
ge_tobytes, ge_add/ge_double/cache and slide machinery exist but need a new
reviewed entry without cofactor clearing. No ready strict API identified.
Its stdint/C99 requirements remain distinct from a C89 facade. donna's public
API is signing/verification/basepoint operations; internals bring platform
identification, 32/64-bit arithmetic tables and compiler intrinsics, and full
attribution closure was not qualified. No backend selected on incomplete
license or dependency evidence. Own implementation C would need field carry/
reduction/inversion/sqrt, Edwards group law, integer scalar multiplication,
canonical decode and result checks plus independent proofs; this is the
maintenance/security burden avoided by preferring existing arithmetic.

Unavailable/failed web paths were not interpreted as semantic absence:
Monocypher advanced/eddsa and intro paths were unavailable via the web tool;
manual/eddsa and exact official release source resolved the API question.
TweetNaCl20140427 official source returned502 and was not used as a substitute
or a selected candidate. donna root LICENSE path returned404; individual-file
notices were examined, without declaring the entire dependency closure licensed.
