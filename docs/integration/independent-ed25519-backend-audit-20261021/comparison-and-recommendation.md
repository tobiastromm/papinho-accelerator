# Independent TIME_SOURCE Ed25519 audit

Task PACC-PHASE6-INDEPENDENT-ED25519-BACKEND-AUDIT-1, local date2026-10-21.
Research and proposal only: no functional source, dependency, build, test or
runtime registration changed. Public official-source retrieval is documentary,
not operational networking. PST_MODIFICATION=DENY, including future extension
for this purpose. Browser, wire, quotas and production0.6.4 remain unchanged.

## Recommendation

Principal: B, a bounded Accelerator-owned source-derived component containing
only the existing generic OpenSSL3.5.8 public-point arithmetic closure and the
already exercised strict predicate. No PST files, packages, generated headers,
bootstrap scripts, import libraries or DLL are dependencies of its future build
or runtime. Obtain/preserve its own exact upstream source/licensing evidence;
PST is read-only reference only. Do not create a generic cryptography project.

This is a proposed implementation direction, not a qualified extracted backend.
The current full-unit experiment still depends on OpenSSL internals and its
DLL. It must not be relabeled independent merely by moving a .obj or renaming
a library. Extraction/glue changes need explicit approval and renewed proofs.

Reserve: A/libsodium1.0.21 corrected public point-validator, conditionally if
B's verified dependency/maintenance/portability/resource gates fail and the
owner separately permits this alternative. Its direct public API avoids a
private arithmetic extraction but introduces a dependency and has a recorded
unmodified VC6 compilation failure. This audit does not resume its port, import
it or override the earlier NO_LIBSODIUM implementation direction. Reserve is
research-only until a new owner decision. D remains a heavier modern-target
option; C is not justified by line count.

Why B first: it preserves existing arithmetic and the actual predicate path
already exercised, has verified Apache-2.0 licensing and an identifiable small
source closure. Its major cost is maintaining a derived source subset and
revalidating every update. No existing off-the-shelf small API examined is both
proved equivalent and proved compatible with the requested legacy targets.

## Mathematical/API matrix

Every eligible implementation must implement exactly canonical32-byte decoding,
on-curve P, P!=O, [L]P=O with integer L, no clamp/reduction/cofactor-clearing
substitute. Signature verification is a distinct operation.

| Alternative | Concrete entry/functions | Predicate disposition |
|---|---|---|
| A Monocypher4.0.3 | crypto_eddsa_check_equation; crypto_eddsa_check; crypto_eddsa_to_x25519; scalarbase/reduce | Not a strict-key API: check_equation explicitly allows noncanonical A/R and clears cofactor; conversions and basepoint-only multiplication do not establish [L]P=O |
| A libsodium1.0.21 | crypto_core_ed25519_is_valid_point; canonical/decode/on-curve/small-order/main-subgroup helpers | Documented semantic match behind size/null wrapper; source inspected previously, independent executable qualification still absent |
| A ed25519-donna pinned commit | ed25519_sign_open/publickey, ge25519_unpack_negative_vartime and ge25519_double_scalarmult_vartime internals | No public strict-point API identified; needs another private-math adaptation and full negative tests, not a ready substitute |
| B OpenSSL math closure | ge_frombytes_vartime, ge_p3_tobytes, ge_double_scalarmult_vartime, ge_tobytes, fe_isnonzero | Existing full-unit strict hook passes scoped offline evidence; independent source subset not materialized or qualified |
| C own minimum | New field arithmetic, decode/re-encode, group operations and unreduced-L multiplication | Could specify same predicate, but no implementation/proof exists; not recommended |
| D own OpenSSL backend | Accelerator-owned full upstream build plus private strict hook, distinct build/distribution | Same existing arithmetic is feasible; ordinary EVP/public_check remains insufficient; full downstream artifact not built/qualified |

Monocypher4.0.3 release source monocypher.c lines2007-2015 permits noncanonical
encodings;2052-2064 checks [8](sum-R). An artificial equation with h=L would
not fix this: the cofactor step erases precisely the component that must be
rejected. Its default signature hash is BLAKE2b; optional Ed25519 uses SHA512,
but changing hashes does not supply subgroup validation. Internal arithmetic
could be adapted, at the cost of another unproved private extension.

## Maturity, licenses, maintenance and distribution

| Option | Evidence/licensing | Maintenance and distribution cost |
|---|---|---|
| Monocypher |4.0.3, BSD-2-Clause OR CC0;2020 Cure53 audit is historical, not proof of this release/predicate | Small upstream is attractive, but required private extension loses direct-API advantage.4.0.3 fixes signature timing leak; do not select4.0.2 as current |
| libsodium |1.0.21, ISC; official release metadata/hash and mixed-order fix recorded | Maintained public API; preserve notices and exact revision. Whole-library import for one call requires linked-closure justification |
| donna | Commit8757bd4cd209cb032853ece0ce413f122eef212c; selected source asserts public domain by Andrew M | No selected release/update qualification; full dependency attribution closure not completed, exclude from selection now |
| B | OpenSSL3.5.8, Apache-2.0, original copyright/provenance and derived-source notices | Own small vendored subset with per-function hashes, extraction review and upstream/CVE tracking; do not claim upstream public API stability for static helpers |
| C | New project code plus any reused material's own obligations | Highest security review, field-invariant/UB and maintenance burden; no inherited audit covers novel arithmetic |
| D | Same Apache-2.0 plus full upstream dependency/distribution notice audit | Whole independent source/build/package lifecycle, larger qualification surface, exact new artifact identity and complete upstream gates |

B/D license is verified in the exact archive. Apache2 section4 requires the
license, retained applicable copyright/patent/trademark/attribution notices,
prominent modified-file notices and preservation of any applicable upstream
NOTICE. No root NOTICE file was found in this archive; that observation is
not permission to discard individual-file notices. The curve source notes
SUPERCOP ref10 provenance; retain that attribution too. Exact selected-file
license review is mandatory before copying production code. Legal permission
to derive is distinct from technical qualification. No code is vendored here.

## Platform matrix

| Option | C89/VC6 interface versus backend | Modern Windows | NT4 / box / ISA |
|---|---|---|---|
| Monocypher | Public header requires stdint.h; source has C99 declarations/macros. VC6 backend not demonstrated | Upstream portable C candidate, no local build in this audit | Not qualified on any of these targets |
| libsodium | Unmodified1.0.21 VC6 core compile previously failed C1083 stdint.h; C89 facade would not repair backend | Official modern MSVC builds exist; local selected primitive not qualified | Not qualified; no port resumed |
| donna | Compiler/intrinsic/32-versus64-bit headers, including intrin.h/__emulu, need actual review | Portable32/64 upstream intent, no current local proof | Not qualified |
| B | Existing C89 public header caller passed VC6, NOT the proposed subset. Map widths via existing PAPACC_I32/I64/U8/U32/U64, review LL constants, C89 syntax, shifts/overflow/alignment | Prior full-unit MSVC Debug/Release evidence reusable as oracle only | Candidate arithmetic can avoid OS dependency; actual builds/runtime still mandatory, no NT4/box/ISA claim |
| C | Entire backend/toolchain proof is new | Unimplemented | Unimplemented/unqualified |
| D | C89 boundary possible; OpenSSL modern target does not promise direct VC6 source compilation | Existing full-unit experiment is positive only in its inspected modern target | No NT4 promise; box/ISA need their own supported backend builds |

Existing include/papacc/types.h already defines VC6 width-checked __int64
mappings. They are a possible compatibility mechanism, not evidence that
original signed-shift/overflow/literal semantics survive a port. Do not
rewrite arithmetic to pass compilation or silence diagnostics. No platform
inherits another platform's proof.

## Independent OpenSSL backend hazards (D)

An independent full backend must not import PST-owned paths or expand PST
responsibility. A second DLL merely renamed on disk is unsafe as a plan:
Windows module identity/import resolution and two libcrypto instances can
bind the wrong artifact or split allocator/provider/global state. A separately
built, deliberately isolated private export boundary and exact binary identity
would be required. Never exchange EVP objects/allocations across instances.
Static linkage is not zero memory and must audit symbol resolution/duplicate
crypto state. No release version, export or loader design is accepted here.
A full source build may maintain upstream changes more easily than B, but is
not the smallest current validation-only dependency and does not improve NT4.

## Minimum implementation plan — only after owner approval

1. Accept the bounded architecture separately; implementation remains denied.
Freeze own upstream archive/version/license and per-function source ranges.
Keep original archive immutable. No build/runtime dependency on PST checkout.
2. Materialize only verified generic field/group closure, necessary types,
constants and table Bi. Keep arithmetic bodies/invariants unchanged; private
prefix/static linkage, one size-checked result API, no signing/hash/RNG API.
3. Review minimal glue: width-checked types/literals and C89 spellings;
replace the PUBLIC-DATA-only CRYPTO_memcmp dependency with a reviewed existing
bounded comparison, preserving exact zero/nonzero semantics. Do not expose
this public-data module for secret inputs. This is a proposed glue change,
not an already tested transformation. Retain CRT memory functions and compiler
security-cookie runtime where applicable; neither is secretly zero cost.
4. Prove extracted dependency closure: no EVP/provider/Win32/CNG/PST/internal
OpenSSL headers or allocator/entropy service; no hidden external symbols.
If this cannot be shown, STOP and review reserve, never silently grow scope.
5. Reuse unchanged296 focal checks,2526 crypto and5940 LAB transcript checks
per configuration. Requalify the new component against the old full-unit
oracle, independent public vectors and exact mathematical expected results.
Add independent order8/4L/8L coverage absent from the prior byte corpus;
null/length/sign/overflow/error/output/Stop/retention tests remain required.
6. Build modern Debug/Release /W4 /WX and a real VC6 backend/caller, then
inspect linked symbols/map, heap/init/whole nested stack and retained operation
coexistence. NT4 requires actual runtime qualification; box/ISA remain gated.
7. Integrate only the existing TIME_SOURCE DATA component after scoped review
and ED11 gate. No positive source/admin authority, CURRENT, UTC, signature,
issuer/UPDATE, admission or new owner follows. Existing synchronous protected
borrow/Stop/quiescence/exactly-once contracts remain intact.
8. Ship only Accelerator-owned identified source/artifact with license notices,
manifest/build recipes, explicit target support and failed-closed unsupported
backends. Track every upstream arithmetic/security change affecting the subset;
no automatic updates, generic plugins, new authority or frozen PST replacement.

## Proof reuse and remaining gate

Original296/2526/5940 proofs and every previous fixture/log/hash are preserved.
They establish the inspected older objects/DLL paths only. A newly extracted
or replaced implementation must earn its own PASS; compilation and API names
are not equivalence. This audit ran no builds, tests, network operational
requests, keys, signatures or clock changes. Download/read/hash analysis only.

First unresolved implementation gate is an approved, pinned source boundary
and demonstrated standalone closure/C89 semantics, followed by independent
coverage and ED11. This is not reclassified as an environmental CURRENT issue.
No selected production backend exists. PST remains forbidden, not an option
waiting for another permission question.
