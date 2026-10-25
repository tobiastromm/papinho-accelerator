# Existing OpenSSL strict Ed25519: offline implementation and PST handoff

## Scope and owner disposition

Task PACC-PHASE6-OPENSSL-STRICT-ED25519-ADAPTER-1 authorizes the minimum
existing-arithmetic investigation/extension and offline qualification. The
owner subsequently selected HANDOFF ONLY IN ACCELERATOR in response to the
specific PST permission question. Therefore no patch/header/build source,
manifest, import library, DLL or documentation in the PST repository is
changed. No backend is installed, distributed or registered with TIME_SOURCE.
This is a concrete reviewed patch bundle and native evaluation, not a new
canonical backend, new authority, root or worker. No libsodium is included.

## Exact sources and arithmetic

The canonical PST upstream archive SHA256 remains
 a8f84a39918ec6415ce765d9b429d313ba97b8143169c172e734b9514464f5b2.
Original crypto/ec/curve25519.c SHA256 remains
 d2ce29fa7764c6eaf083169339245643d8e10b5d4268b8e4c3cc15372c23aeda.
Downstream0001 patch SHA256
 a9108242e59b44ac67f0421dad660dc676d04141f2a12188eb4512a50c5d5566.
Downstream0002 patch SHA256
 bef8e05bfa10c514fbed06ea6822dcedb87a553018b1abb0bedd9b8622bb3890.
The exact public C89 header SHA256 is
 018942fe731192942a629602b02e9b7da81b722e531cee2eea5f9eb76f6d373a.
[Source pins](source-pins.json) records original/first-patch bytes;
[hygiene pins](hygiene-pins.json) records final evaluation postimages.
Patch0002 applies after0001, not directly to a different upstream revision.

Existing Accelerator papacc_rt_openssl_verify imports ED25519 into the
explicit pinned default provider and calls EVP_DigestVerify. Its provider
signature path reaches ossl_ed25519_verify in the original curve unit.
The extension has access to the SAME unit's static ge_frombytes_vartime,
ge_p3_tobytes, ge_double_scalarmult_vartime, ge_tobytes and fe_isnonzero.
No existing static function is called from a separately compiled consumer.
No field/group algorithm, ladder, decoder, cofactor-clearing implementation
or scalar arithmetic is introduced.

The new entry checks null/length before access, decodes P, re-encodes and
compares all32 bytes, rejects Enc(O), computes [L]P+[0]B with the existing
arbitrary-point routine, checks the projective denominator invariant, and
requires Enc(result)==Enc(O). L is the integer little-endian group order;
there is no scalar reduction, clamp, input normalization or cofactor clearing.
A failed roundtrip rejects y>=p and negative zero. Identity and pure/mixed
torsion fail the accepted P0-P2 conjunction. Validity is mathematical DATA
only, never a certificate/source authorization or proof of clock correctness.
EVP_PKEY_public_check and an artificial signature are not used as substitutes.

The code is an appended backend-local entry. Its interface exports no curve
structures. Results are VALID1, INVALID0, INTERNAL_ERROR-1 (invalid internal
projective denominator) and BACKEND_UNAVAILABLE-2 reserved at a future
unavailable consumer boundary. The present linked experimental entry is
always present and has no runtime selection/fallback or -2 execution path.
The header defining -2 is not a claim that optional loading is implemented.
Missing operational adapter continues to deny via the existing native gate.
A real consumer must reject every value except VALID, without treating a
mathematical success as authority. No wide public PST ABI is added.

## Explicit hygiene patch

Initial /W4 /WX compile found four upstream diagnostics. Patch0002 adds only
casts to uint8_t for already0/1 integer predicates, explicit zero initialization
of an existing static const object, and (void) for an unused no-op refcount
parameter. No arithmetic value, signature equation or ownership changes.
No warning suppression or toolchain substitution. Initial logs are preserved.
The final source unit and harness compile Debug/Release /W4 /WX without warnings.

## Reviewed executable evidence

All build/test commands were submitted to official executor review before
execution, exclusively in the governed external workspace. No PST file was
written. Modern source builds use the documented PST MSVC bootstrap and the
already distributed generated public OpenSSL configuration. The original
pinned libcrypto DLL/import library are used unchanged for public EVP/hash
services. The experiment links the modified curve object directly into its
TEST executable with /Gy and /OPT:REF; it does NOT replace the production DLL.
This proves that object path, not a new full OpenSSL build/package.

Focal tests:296 checks/configuration, zero failures, Debug and Release.
They cover RFC8032 TEST1 and basepoint positives, null/0..31/33..64/SIZE_MAX
lengths, immutable inputs, repeated synchronous returns, identity and negative
zero, order2/order4 both signs, noncanonical y=p..p+18 both signs, off-curve
y=2 both signs and mixed-order2L both signs. The fixed mixed point is B+T2
=(-x_B,-y_B), encoding95 followed by31*99; its exact order2L follows the
independent group-law specification, not the candidate's output. The y=2
nonsquare selection uses a fixed mathematical decoding check in metadata,
not product curve code. No new keys or private signatures are generated.
There is no claim of a public byte corpus for every4L/8L class; the implemented
[L] test mathematically rejects those classes, but independent executable
4L/8L vectors remain additional coverage for full backend qualification.

Public signature regression compares unchanged EVP verification with the
modified object's ossl_ed25519_verify for RFC8032 TEST1, plus corruption.
Existing Roughtime executables additionally pass2526 crypto and5940 transcript
checks/configuration. Those historical regressions use the original pinned
DLL; they are not mislabeled a rebuilt-downstream-DLL full regression suite.
The transcript remains LAB_ONLY, not factual UTC. No fixture regeneration.

The public header caller compiles /TC /Za /W4 /WX in VC6. This is compile-only
interface evidence, NOT VC6 backend linking, NT4 or portable deployment proof.
OpenSSL modern and VC6/NSS remain separate documented target contracts.
No requirement to compile the full OpenSSL3.5.8 source under VC6 is introduced.

Harness failures were investigated before changing anything: /TC initially
parsed .obj as source (moved after /link), VC6 warned about a function designator
(replaced the header-only caller), and the existing transcript executable
was invoked without its mandatory fixture argument (corrected command).
No cryptographic expectation was relaxed. Initial logs remain auditable.

## Lifecycle and ED11

The new entry is synchronous and has no allocation, native handles, threads,
retained pointer or escaping borrow. Caller owns immutable input through
return and must protect backend/module lifetime. Stop is enforced at the
existing owner/effect boundary; this primitive does not introduce asynchronous
cancellation, a new Stop owner or freeing by timeout. Synchronous local DATA
success cannot publish anything. No positive Stop-integration or operational
borrow proof is claimed from the standalone test.

Instrumentation observed zero OpenSSL allocator calls during the focal point
checks before provider initialization. This does not bound CRT/loader/kernel
or future provider initialization. Listing-derived wrapper frames are392 bytes
Debug and480+8 pushed bytes Release. Multiplication frames are2344 Debug and
2328+48 pushed bytes Release. Internal call-path subtotal including return
slots reaches4032 Debug and3376 Release on wrapper->multiplication->ge_add->
fe_mul. These compiled quantities supersede the earlier2588 source-object
estimate, but are NOT complete runtime stack high-water/bounds: external CRT,
CRYPTO_memcmp and security-cookie frames, caller and dynamic paths remain gates.

The TEST executable PE image sizes are98304 Debug/86016 Release; code42496/
28160 and initialized sections39936/39424. These include harness, existing
arithmetic, imports and metadata. They are neither the new adapter delta nor
a complete resident-memory census. Three new32-byte constants total96 source
bytes; linker allocation/mapping must be charged from the final actual build.
A final integrated backend may share already linked arithmetic, but no saving
or zero delta is presumed. No binary is installed to avoid these costs.

Prior fixed Selected1952624/Legacy1790872 and analytical graph2172856 are
unchanged, not an operational peak. Full downstream build/linker delta,
initialization, runtime high-water, native/source/status graphs and simultaneous
root/issuer/Roughtime retained concurrency remain unqualified. ED11 PARTIAL.
No quota, individual credit, profile default or host-headroom credit changes.
Windows3145728 explicit only, Legacy2097152 unchanged.

## Next bounded gate

Owner chose handoff-only: do not write this bundle into PST or its distribution.
A later specifically authorized PST task must approve exact downstream source/
export boundary, regenerate binaries as a NEW identified artifact, run the
complete required upstream/PST suite OFFLINE or obtain separate permission
for any network cases, audit staging/manifests and update consumer pins.
Never replace a frozen DLL/lib in place. Complete remaining independent
negative vectors, full stack/init/lifecycle/coexistence ED11 before activation.
This handoff itself authorizes none of those actions. CURRENT/UTC/issuer/
UPDATE/TRUST/signature/recovery/admission remain denied or unqualified.
