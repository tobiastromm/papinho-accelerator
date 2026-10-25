# Independent mathematical subset: implementation proof and handoff

Task PACC-PHASE6-INDEPENDENT-ED25519-SUBSET-EXTRACTION-1,2026-10-21. S0-S6 and P0-P5 unchanged. Official source,cast and vector document-operation hashes are retained alongside this report. No worker, Roughtime transport, codec registration, PST, Browser or wire changes.

## Independent source and scope

Initial local discovery located only PST-derived archives. No extraction used them. The owner then explicitly authorized an official download; an independent TEMP archive was fetched from https://github.com/openssl/openssl/releases/download/openssl-3.5.8/openssl-3.5.8.tar.gz, SHA256 a8f84a39918ec6415ce765d9b429d313ba97b8143169c172e734b9514464f5b2. Original archive is retained under third_party/openssl-math/source; original Apache2 LICENSE.txt and per-region extraction manifest under third_party/openssl-math/3.5.8. This is official HTTPS/hash provenance, not a claim of independent OpenPGP verification. Prior tag metadata records unknown_key, not verified signature.

33 generic math functions plus d,d2,sqrtm1,Bi are extracted; no full-library/runtime fallback. Field/group equations and integer-L multiplication unchanged. Necessary modifications: isolated width-checked PAPACC integer aliases; four64-bit literal spellings for VC6; public-data-only CRYPTO_memcmp replaced with CRT memcmp preserving zero/nonzero semantics; three explicit signed-char slide assignments for values0/1 and guarded[-15,15]. No secret inputs are permitted. Original notices/ref10 attribution preserved. Extraction manifest pins exact upstream regions and adapted hashes. Existing OpenSSL signature/hash backends remain unchanged.

Private strict_ed25519.h exposes exactly one synchronous public-data validation operation. Results1/0/-1/-2 mean VALID/INVALID/INTERNAL_ERROR/BACKEND_UNAVAILABLE; the compiled supported backend never reports unavailable. No curve structs, signing, keygen,RNG,generic crypto API or authority exported. Null/length checked before access, canonical re-encoding required, identity rejected, integer L not reduced, multiplication result identity required. Signature verification and administrative eligibility remain separate.

## Actual builds and tests

Four direct clean compiler invocations: MSVC19.51 x64 and VC6 x86, Debug/Release /TC /Za /W4 /WX /MD.296 public DATA checks/configuration pass. VC6 compiles the backend itself and its caller, not merely a header. A separate CMake/Ninja Release static-library/test build from tools/qualification/strict-ed25519 passes CTest1/1 without reading PST. No NT4/box/ISA execution/qualification claimed.

New296 count is284 inherited key-only checks plus12 new order8/mixed4L/8L checks; it is not the same composition as the old296 oracle harness, which includes signature/allocation checks. Public independent fixtures: RFC8032 TEST1/basepoint and libsodium1.0.21's already pinned ref10 order8 blacklist bytes (no libsodium code/library incorporated). Mixed4L/8L bytes were derived offline after official review using existing extracted ge operations on public B and T4/T8; not independent external mixed-order vectors. Derivation proves T's exact order4/8 and B has prime order L; B+T therefore has exact order4L/8L. Public derivation source/command preserved. No private key, signature, RNG, package fixture or UTC produced. Both sign-bit variants reject.

Original full-unit oracle296, crypto2526 and LAB transcript5940 each pass again in modern Debug/Release.284 inherited point cases agree with previous oracle expectations. This is scoped outcome comparison, not exhaustive bitwise differential testing over all points. Existing signature/transcript executables exercise unchanged production crypto, not a replacement signature backend; their passes do not independently qualify the new point validator. They cover existing Stop/replay/retention paths. Existing six LAB pairs remain unchanged and are not factual time authority.

First VC6 build failed on three implicit signed-char warnings and test do-while constant-condition warnings. Exact reviewed casts preserve values; test macro condition revised, no diagnostic suppression or weakened checks. Final four builds show zero warnings. Earlier shell harness syntax mistake produced no compile/effect and was corrected. No negative official review was bypassed.

## Dependency and lifecycle proof

Object undefined-symbol lists and executable import reports show CRT memory functions, compiler security-cookie/unwind support and VC6 64-bit integer helper routines only. No EVP/provider/OpenSSL/PST/Win32/CNG, allocator, RNG or hidden source dependency in the component. Test executables use normal platform CRT/OS startup; this is not a promise of zero OS cost. Original OpenSSL oracle necessarily uses its preexisting runtime only as a test reference; the new component and standalone build do not link it.

Synchronous immutable32-byte borrow, no stored pointers, mutable globals, heap, owner, pin table or asynchronous call. Caller retains input until return and gates new work after Stop. An in-flight call cannot be canceled or have input released by timeout; existing owner/pins/quiescence retain responsibility. No new Stop API or fabricated cancel introduced. Integration must preserve existing Model E/borrow contracts. Mathematical failure never frees caller resources or grants trust.

## ED11 and integration gate

See ed11-measurements.json and raw listings. Object section sizes are compiled raw section payloads, not loaded process peaks. Test PE sizes include harness/CRT imports and are not adapter delta. Compiled conservative internal stack subtotals: MSVC4032/3392 and VC65916/5160 Debug/Release; exclude external CRT/compiler helpers/caller/OS and are NOT full runtime bounds. No allocator/RNG call exists in component source/symbol graph; full CRT initialization and transitive operational usage remain UNKNOWN. No measured total-heap/stack highwater or operational root-v4/issuer coexistence is claimed.

No quotas or reservations changed, no presumed savings/reuse and no implicit Windows3MiB selection. Current known baseline and historical isolated Roughtime charge are recorded separately, not summed as a qualified process peak. Full operational ED11/provider/kernel/retained concurrency remain PARTIAL. The component is not registered in TSC1/TSR1/CP4/worker; next task needs exact scoped integration approval and existing factual authority/lifetime/capacity checks. Mathematical VALID cannot grant TIME_SOURCE status, CURRENT, UTC, issuer/UPDATE authority or admission.
