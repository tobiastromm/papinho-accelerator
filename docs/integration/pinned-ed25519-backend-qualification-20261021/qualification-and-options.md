# Pinned libsodium1.0.21 qualification — STOP at VC6 gate

## Provenance

Official release1.0.21-RELEASE, signed tag object
b6b84c7d096d6779021653ab5a44277f7462508b, target commit
d24faf56214469b354b01c8ba36257e04737101e. Official source archive
libsodium-1.0.21.tar.gz SHA256
9e4285c7a419e82dedb0be63a72eea357d6943bc3e28e6735bf600dd4883feaf.
Fetched archive hash equals GitHub's release-asset digest. Saved API metadata
records tag verification verified=true/reason=valid. This is reliance on
GitHub's official HTTPS metadata/signature-verification service, not a claim
of independently verified local OpenPGP trust. No moving master pin.
Archive was inspected outside the repository; no clone/worktree or backend
installation. Selected source hashes are in provenance-and-source-hashes.json.

Official evidence URLs:

- https://github.com/jedisct1/libsodium/releases/tag/1.0.21-RELEASE
- https://api.github.com/repos/jedisct1/libsodium/releases/tags/1.0.21-RELEASE
- https://api.github.com/repos/jedisct1/libsodium/git/tags/b6b84c7d096d6779021653ab5a44277f7462508b
- https://github.com/jedisct1/libsodium/commit/d24faf56214469b354b01c8ba36257e04737101e
- https://doc.libsodium.org/advanced/point-arithmetic

License: ISC, retaining copyright/permission notices; exact release LICENSE
archived. No incompatible license found in that notice. Complete linked
closure and third-party notice audit remain pending; no library imported.

## Source-level predicate review, not executable proof

Accepted P0-P5 unchanged, ADR SHA256
3104307274ca7f98351dba79090cff86ea9f2ee1cf2b03746d1cbad2b706e844.
The release core calls ge25519_is_canonical, ge25519_has_small_order,
ge25519_frombytes, ge25519_is_on_curve and ge25519_is_on_main_subgroup.
The subgroup helper in ref10 lines1044 onward multiplies by integer L
and checks both X=0 and Y=Z, not just X=0. This is the relevant mixed-order
correction; the inspected ChangeLog includes the security fix in1.0.21.
Versions through1.0.20 are not qualified as requested; no silent backport.

Canonical helper checks y<p; decoder handles square-root validity/sign;
small-order blacklist masks sign for identity/order2/order4/order8 cases.
The decoder alone does not explicitly reject x=0/sign1, but those y=+/-1
cases must be rejected by the complete predicate. All accepted P1/P2 edges
still require independent executable tests, not inference from call names.
No actual divergence observed in this source review, no equivalence PASS.
The low-level API takes a pointer with no length: it cannot itself establish
P0. A future approved C89 adapter must reject null/non32-byte spans before
calling it. Import/public_check/signature tests are not point-validity proofs.

## Exact approved minimal compilation probe

Command vc6-compile-gate.cmd SHA256
5cdbee0e2847625fe513ed1241f0a2fff566cfed59be7ae1c03d9d09134be478
was submitted to official executor review and executed only after approval.
It uses installed VC6 VCVARS32 and cl /TC /W4 /WX /Od /Zi /c on the original
core unit, no link or execution, no local compatibility headers, no edits to
upstream or canonical source. Source SHA256
799dfef3e6ba0a63f36438dedede34a162594aa6ddd616543698a6a3d2fcfed8.

Observed exit2: fatal C1083 at the first inclusion of stdint.h (reported
line2 by this compiler). SDK VC98 has no stdint.h. Private common/export/
ref10 headers also depend on it. The release MSVC projects start at vs2010
with v100 toolset; no official VC6 project was found. Missing standard types
is the first demonstrated incompatibility, not proof that a compatibility
port is impossible or that fixing only this header suffices.

STOP honored: no further build, vector execution, shim, fallback, warning
suppression, modern toolchain substitution or integration after this failure.
MSVC Debug/Release and all runtime proofs remain NOT_RUN, not FAIL or PASS.
The approved proof generated no linked executable or running library.
WARNINGS: none emitted in this failed invocation; zero-warning successful
build has not been established. Public release retrieval is documentary,
not operational networking or NETWORK_TESTS.

## Dependency/resource gates

Inspected dependencies include fixed-width integer types, internal ref10
headers/tables and common helpers. The core translation unit also references
randombytes and utility functions for unrelated APIs; private ref10 contains
other group/scalar operations. Full source library has CRT/platform/CPU
initialization and optional implementations. Do not infer a minimal linked
closure from this single function or copy internal curve routines as a new
local primitive. Object/link-map/runtime costs cannot be reported from a
compilation that stopped before generating its object.

| Cost | Result |
|---|---|
| Compiled code/data | UNKNOWN, no object/link map |
| Initialization/heap | NOT_MEASURED; not zero |
| Stack/scratch/buffers | NOT_MEASURED, full transitive costs UNKNOWN |
| Retention/concurrency | No adapter instantiated; future multiplicities unproved |
| Existing root/issuer/Roughtime coexistence | Prior analytical graph2172856 only, not actual process peak |
| Selected known fixed / Legacy |1952624 /1790872 unchanged |
| ED11 limits | Explicit Windows3145728, Legacy2097152 unchanged |

No capacity PASS, credit, reuse discount or memory movement. ED11 PARTIAL.
No new runtime consumer or backing was added. Provider/kernel/source costs
remain UNKNOWN and v4 worker remains denied.

## Focal alternatives — proposed only

A. Audit a compatibility-only port of the same pinned upstream implementation
for VC6: explicit fixed-width types, language/compiler/ABI/alignment/intrinsic
mapping and undefined-behavior/optimization review, without arithmetic or
predicate changes. Submit exact shim/build hunks before execution. Then run
independent public positive/torsion/mixed-order/noncanonical/off-curve/length
vectors under VC6 and modern Debug/Release, inspect linked closure and measure
full ED11. This is a new qualification plan, not authorized continuation after
this STOP or proof a shim is sufficient.

B. A C89 wrapper around a modern-built in-process backend may isolate headers,
but does not qualify runtime loading, calling convention, CRT ownership or
NT4. It cannot by itself satisfy the required VC6/backend target proof.
No DLL loading, new service/process or alternate trust authority is proposed.

C. If a reviewed compatibility port is unsuitable, evaluate another existing
mature implementation under the unchanged predicate/license/portability/ED11
and independent-vector gates. None is selected here. No custom cryptography.

First factual gate: unmodified1.0.21 fails installed VC6 inclusion. Resolve
that gate explicitly before resuming the stopped qualification. No productive
source/key, CURRENT, UTC, issuer status, UPDATE or trust follows from any probe.
