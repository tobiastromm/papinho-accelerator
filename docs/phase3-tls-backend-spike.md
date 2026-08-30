# Phase 3.A2B Crypto/TLS Backend and Legacy Compatibility Spike

Status: authoritative compatibility report, 2026-08-29. Result: **NOT READY**.
The frozen [Transport Security profile](phase3-transport-security-profile.md)
was not weakened. This spike added no production integration, dependency or
PACC wire change.

The follow-up [RetroZilla NSS investigation](phase3-retrozilla-nss-investigation.md)
is authoritative for subphase 3.A2B-R2. It does not replace this historical
result or close 3.A2B.

Phase 3.A2A-R1 subsequently revised the normative profile to TLS 1.3 mTLS; see
[Transport Security and Credential Profile](phase3-transport-security-profile.md).
All external-PSK requirements, comparisons and conclusions below are retained
as historical spike evidence and are no longer current profile requirements.

## Evidence vocabulary

- **Documented:** asserted by an upstream primary source.
- **Source-inspected:** verified in the exact local source snapshot.
- **Compiled:** compiled with the named actual compiler.
- **Linked/runtime/interoperable:** respectively linked, executed, or completed
  the required cross-peer handshake.
- **Not tested:** no stronger claim is made.

## Environment and reproducibility

PapinhoBrowser was inspected read-only at `C:\Projetos\PapinhoBrowser`. It is
not a Git checkout, and neither PapinhoBrowser nor PapinhoAccelerator currently
has a project-level LICENSE file. Dependency compatibility therefore requires
a project licensing policy decision rather than an assumed license.

The actual client build uses Microsoft Visual C++ 6.0 `cl.exe`/NMAKE, `/TC`,
`/W3`, `_WIN32_WINNT=0x0400`, Win32 x86, `wsock32.lib`, and a local `stdint.h`
plus `inline -> __inline` compatibility header. The installed compiler at
`C:\MSVC600-master\VC98\Bin\cl.exe` was used. PapinhoBrowser already vendors
and builds BearSSL for unrelated TLS 1.0--1.2 HTTPS functionality. That code is
blocking/socket-oriented and does not establish suitability for this profile.
The documented NT4 workflow is manual copy/run in an NT4 VM; no automated VM
execution endpoint was available to this spike.

Temporary sources and results were kept under ignored
`build\spikes\phase3-a2b`. Archives inspected:

| Candidate | Exact snapshot | Origin | Local SHA-256 |
|---|---|---|---|
| wolfSSL | tag `v5.9.1-stable` | official GitHub tag archive | `D5CA7AF48CD2D9A91D539E9BAEDEBA55A0605A28D7AC8B01DC3D5254A13CA341` |
| Mbed TLS | tag `mbedtls-4.1.1` | official GitHub tag archive | `88AEFCD81012EB7713224E4CF66BC035EB1B6533A9086D3CCB51201132BE369C` |
| BearSSL | PapinhoBrowser vendored snapshot, file dates 2018 | existing repository copy | version not independently pinned |
| OpenSSL | 4.0.0 evaluated from upstream docs | no source downloaded | not applicable |
| picotls | current upstream documentation/source metadata | no source downloaded | not applicable |

The GitHub Mbed TLS tag archive uses submodules and is not the complete official
release bundle; this was sufficient for source/language inspection but not
treated as a failed library build.

## Candidate result summary

| Candidate | Classification | Technical result | Legacy result | Maintenance |
|---|---|---|---|---|
| BearSSL | D — profile-incompatible | TLS 1.3 absent | Existing VC6 build is irrelevant to missing protocol | Upstream site still labels TLS 1.3 unimplemented; no recent release evidence |
| Mbed TLS 4.1.1 | E/G — legacy-incompatible/insufficient evidence | Documents TLS 1.3 client/server, external PSK-ephemeral, ChaCha/AES, ALPN and selectable TLS 1.3-only build | Requires C99, VS2017 baseline and PSA dependency; VC6 build not proven | Active releases and SECURITY process |
| wolfSSL 5.9.1 | B/G/F — likely small/moderate port, insufficient proof, license decision | Source exposes TLS 1.3 PSK callbacks, PSK-DHE logic, ChaCha, X25519, ALPN and custom I/O | Header parsed after small shims; core source compilation failed; no link/runtime | Active releases/advisories; newer 5.9.2 exists |
| OpenSSL 4.0.0 | C — server-only viable (documented) | Full modern TLS 1.3 external-PSK, BIO, ALPN/version/ticket controls | Current OpenSSL is not a realistic VC6/NT4 client build | Active, 4.0 supported through 2027-05-14 |
| picotls current | G — insufficient/profile concern | TLS 1.3 and resumption PSK-DHE; external-PSK/multiple-identity issues remain visible upstream | No VC6 proof; modern dependencies/build | Active repository, but not selected infrastructure |

BearSSL's own status states that RFC 8446 is not implemented, so adding TLS 1.3
on top would be forbidden custom TLS. Mbed TLS documents both TLS 1.3 sides and
an external PSK-ephemeral-only build that omits certificates, but its current
baseline is C99, CMake 3.20, PSA Crypto and VS2017; rewriting it for VC6 would
be a **large/unsustainable fork**. wolfSSL is the only candidate found with
explicit current C89/MSVC6 branches in source and is therefore the strongest
client candidate, but it did not pass the required gate.

Primary sources: [BearSSL TLS 1.3 status](https://bearssl.org/tls13.html),
[Mbed TLS repository and requirements](https://github.com/Mbed-TLS/mbedtls),
[Mbed TLS TLS 1.3 support](https://github.com/Mbed-TLS/mbedtls/blob/development/docs/architecture/tls13-support.md),
[wolfSSL repository/releases](https://github.com/wolfSSL/wolfssl),
[wolfSSL licensing](https://www.wolfssl.com/license/),
[OpenSSL 4.0 release](https://openssl-library.org/post/2026-04-14-openssl-40-final-release/),
and [picotls repository](https://github.com/h2o/picotls).

## Security capability matrix

`D` is documented, `S` source-inspected, `P` compiled/proven, `N` absent, and
`?` not proved.

| Requirement | BearSSL | Mbed TLS 4.1.1 | wolfSSL 5.9.1 | OpenSSL 4.0.0 | picotls current | Evidence |
|---|---:|---:|---:|---:|---:|---|
| TLS 1.3 client/server | N | D/S | D/S | D | D | Upstream docs/source |
| External PSK, both roles | N | D/S | D/S | D | ? | APIs/source; picotls focus is ticket PSK |
| PSK-DHE | N | D/S | D/S | D | D | Mode/config source |
| Disable pure `psk_ke` | N/A | S | S, not runtime-proved | Configurable, not tested | Context flag, not tested | Source/docs |
| ChaCha20-Poly1305 | primitive only, no TLS 1.3 | D/S | D/S | D | D | Source/docs |
| AES-128-GCM | primitive only | D/S | D/S | D | D | Source/docs |
| X25519 | primitive availability insufficient | D/S | D/S | D | D | Source/docs |
| P-256 | primitive availability insufficient | D/S | D/S | D | D | Source/docs |
| ALPN `papacc/1` | no TLS 1.3 profile | D/S API | D/S API | D API | D | Exact value not handshaken |
| TLS 1.3-only | N/A | D selectable | S macros `WOLFSSL_NO_TLS12`/`NO_OLD_TLS` | D min/max API | inherently TLS 1.3 | Not runtime-proved |
| Disable 0-RTT | N/A | D/config | S, feature omitted | D/config | configurable | Not runtime-proved |
| Disable resumption | N/A | D/config | S by omitting session tickets | D cache/ticket controls | callback omission | Not runtime-proved |
| Nonblocking retry | TLS 1.2 only | D WANT_READ/WRITE | D WANT_READ/WRITE | D BIO retry | D incremental | No harness proof |
| Custom transport | callbacks | D BIO callbacks | D/S `SetIORecv/Send` context | D BIO | direct handshake API | No Papinho integration |
| Clean close/truncation distinction | TLS 1.2 only | API documented | APIs documented | APIs documented | ? | Not tested |

No row marked only documented/source-inspected is promoted to runtime proof.

## Legacy compiler matrix

| Candidate | Declared language baseline | VC6 header parse | VC6 library build | VC6 link | NT4 runtime | Patch size | Result |
|---|---|---:|---:|---:|---:|---|---|
| BearSSL vendored | C89-oriented | PASS (existing project) | PASS for TLS 1.2 library (existing project) | Existing browser build | Existing unrelated browser tests | Existing small shim | PROFILE-INCOMPATIBLE |
| Mbed TLS 4.1.1 | C99; tested VS2017+ | FAIL/incomplete dependency graph | NOT TESTED | NOT TESTED | NOT TESTED | Large fork expected | E/G |
| wolfSSL 5.9.1 | C with `WOLF_C89` paths | PASS after two shims | FAIL | NOT TESTED | NOT TESTED | Unknown; beyond trivial header shim | B/G/F |
| OpenSSL 4.0.0 | Modern supported toolchains | NOT TESTED | NOT TESTED | NOT TESTED | NOT TESTED | Expected unsustainable | C server-only |
| picotls current | Modern C/dependencies | NOT TESTED | NOT TESTED | NOT TESTED | NOT TESTED | Unknown | G |

The wolfSSL header probe used the real VC6 compiler and passed after defining
`SIZEOF_LONG_LONG=8` and disabling variadic macros. The subsequent direct
compile of TLS 1.3 core and required crypto produced only 3 objects before
failure. Observed blockers included VC6-invalid `ULL` preprocessing tokens,
`long long`, `min/max` declarations, `strtok_s`, `vsnprintf`, and syntax in the
included `misc.c`. These are plausibly portability rather than cryptographic
changes, but their complete scope was not established. Therefore “small port”
is a hypothesis, not a validated result. There is no VC6 link evidence.

## License matrix

| Candidate | License | Static/dynamic observation | Project compatibility | Commercial option | Decision |
|---|---|---|---|---|---|
| BearSSL | MIT | Permissive with notice | Generally permissive, but project license absent | Not needed | Technically unusable for profile |
| Mbed TLS | Apache-2.0 OR GPL-2.0-or-later | Apache option is permissive with conditions | Likely usable subject to project policy | Not required by upstream model | Technically poor legacy fit |
| wolfSSL | GPLv3 or commercial | Linking/redistribution under GPL requires project compliance; exact obligations need policy/legal review | Project license absent, so not accepted automatically | Available | F — policy decision required |
| OpenSSL 4.0 | Apache-2.0 | Permissive conditions/notice | Likely usable subject to project policy | Support available | Provisional server candidate |
| picotls core | MIT; bindings carry additional licenses | Depends on selected crypto binding | Needs binding-by-binding review | Not identified | Not selected |

This is a compatibility observation, not legal advice. The technical client
leader and licensing leader are not yet the same decision: wolfSSL has the best
legacy signals but requires GPLv3-compatible distribution or a commercial
license.

## Interoperability and negative-test matrix

| Server | Client | TLS 1.3 | External PSK | PSK-DHE | ChaCha | Group | ALPN | Handshake tested | Result |
|---|---|---|---|---|---|---|---|---:|---|
| OpenSSL 4.0 | wolfSSL 5.9.1 VC6 | Documented | Documented | Documented | Documented | X25519 documented | APIs documented | NO | BLOCKED: VC6 library did not build |
| wolfSSL 5.9.1 modern | wolfSSL 5.9.1 VC6 | Source | Source | Source | Source | X25519 source | Source | NO | BLOCKED: both modern harness and VC6 link absent |
| Mbed TLS 4.1.1 modern | wolfSSL 5.9.1 VC6 | Documented | Documented | Documented | Documented | candidates documented | APIs documented | NO | BLOCKED: VC6 library absent |

Because no client executable linked, the required handshake and negative tests
were **not tested**: correct PSK, wrong secret, unknown identity, TLS 1.2-only
peer, pure `psk_ke`, missing/wrong ALPN, disabled 0-RTT, disabled resumption,
`close_notify`, truncation, partial I/O and cross-backend interoperability.
Generic connect success is deliberately not substituted.

Consequently no final ECDHE group is selected. X25519 remains preferred and
P-256 remains the compatibility candidate. Neither is proven under the VC6
TLS client configuration.

## Entropy, platform and resource audit

The future client backend must accept an application/platform entropy callback.
`rand()`, time, ticks, runtime IDs and the DATA ticket counter remain forbidden.
Windows CryptoAPI exposes `CryptAcquireContext`/`CryptGenRandom`; contemporary
Microsoft documentation describes cryptographic output but now lists XP as the
supported minimum, so that page alone does not prove the NT4 baseline. Historic
NT4 cryptographic-module material indicates CryptoAPI/CSP availability, but an
actual VC6 probe on the target VM is still required. The credible candidate is
`PROV_RSA_FULL` plus `CryptGenRandom`, dynamically or statically bound only
after proving the exact NT4 service-pack/provider baseline. Until that runtime
probe succeeds, NT4 CSPRNG is **credible but not proven**. The server entropy
backend remains independently selectable.

Relevant sources: [Microsoft CryptGenRandom documentation](https://learn.microsoft.com/en-us/windows/win32/api/wincrypt/nf-wincrypt-cryptgenrandom)
and [Microsoft NT4 CSP security policy archived by NIST](https://csrc.nist.gov/csrc/media/projects/cryptographic-module-validation-program/documents/security-policies/140sp68.pdf).

wolfSSL exposes custom I/O, allocator/heap abstractions and single-threaded
configuration; Mbed TLS exposes BIO/RNG callbacks; OpenSSL exposes BIO and
retry states. These meet the architecture conceptually, but bounded work in
Papinho's scheduler was not measured. TLS record buffers, per-connection RAM,
stack, code size and CPU were not quantified because no representative minimal
client linked. The three wolfSSL objects emitted before failure total 51,799
bytes and are not a library-footprint measurement.

ChaCha20-Poly1305 remains the preferred old-x86 software cipher; AES-GCM without
AES-NI may be less attractive. X25519 is normally smaller/simpler than general
P-256 implementations, but neither claim replaces measurement. Certificate,
X.509, RSA/ECDSA and CA-store code is theoretically omittable for PSK-only
Mbed TLS/wolfSSL configurations; omission was source-inspected but not proven
in a linked binary.

## Maintenance and security process

Mbed TLS, wolfSSL and OpenSSL have active repositories, releases and published
security channels/advisories. Past CVEs are not treated as automatic rejection;
the availability of fixes/process is positive. wolfSSL 5.9.1 was the downloaded
test tag, but upstream 5.9.2 was current by report completion and includes later
fixes, so any resumed spike must restart on 5.9.2 rather than ship 5.9.1.
OpenSSL 4.0.0 is non-LTS and has a relatively short standard support horizon;
a later dependency decision should prefer an appropriate supported/LTS branch
available at integration time. BearSSL's stale TLS 1.3 status makes it an
unacceptable new Phase 3 transport foundation despite its excellent existing
legacy compile behavior.

A permanent large VC6 patchset would make security updates too risky. Only
socket/allocator/entropy callbacks, fixed-width typedefs, CRT substitutions and
small compiler syntax shims are acceptable; TLS handshake, HKDF, ChaCha,
Poly1305, ECDHE, binder or record-layer rewrites are prohibited.

## Recommendation and blockers

```text
Preferred modern server backend: OpenSSL 4.x family, provisional/documented only
Preferred NT4 client backend:    none proven; wolfSSL is the next candidate
Same/different backend:          different backends are expected and acceptable
Compatibility shim:             not yet bounded
Frozen profile:                 unchanged
Preferred group:                not selected; X25519 remains first probe
```

There is no production dependency version plan because no backend pair is
selected. If future evidence selects OpenSSL server plus wolfSSL client, source
must be fetched from exact signed/tagged upstream releases, hash-recorded and
vendored through a separately reviewed dependency process—not copied from this
ignored spike. Minimal features remain TLS 1.3, external PSK, PSK-DHE,
SHA-256/HMAC/HKDF, ChaCha20-Poly1305, one approved ECDHE group, ALPN, CSPRNG and
records; TLS 1.2 can remain in third-party source only if runtime/build policy
makes it unreachable. Certificate and obsolete-protocol stacks should be
compiled out where supported.

### Required continuation before READY

1. Repeat wolfSSL on current 5.9.2 and fully inventory the VC6 patch, proving it
   is small and non-cryptographic.
2. Build and link the minimal VC6 client with external PSK-DHE, ChaCha, X25519
   (then P-256 if necessary) and ALPN.
3. Run it on the actual NT4 baseline and prove CSP entropy initialization.
4. Build a modern server backend and execute all positive/negative, close,
   truncation, partial-I/O and cross-backend tests.
5. Measure client code/RAM/record buffers/stack and decide wolfSSL GPLv3 versus
   commercial licensing in the context of explicit project licenses.

No 3.A2A profile reconsideration is justified by current evidence: the blocker
is unproven backend portability, not proof that TLS 1.3 PSK-DHE is impossible.

## Inputs reserved for Phase 3.B after this spike closes

Once a backend pair is proven, 3.B needs a backend-independent context boundary;
post-handshake principal output; 16-byte credential lookup callback; CSPRNG
abstraction; security-specific result taxonomy; Connection Security Context
lifecycle; secret ownership/cleanup; incremental WANT_READ/WANT_WRITE; selected
profile/version/cipher/group/ALPN reporting; and distinct clean-close,
truncation and fatal-integrity results. 3.B must not begin while this report is
NOT READY.

```text
CRYPTO/TLS BACKEND SPIKE NOT READY
```

## RetroZilla NSS investigation — 3.A2B-R2

The exact RetroZilla 2.3 release tag (`2f274574d3c6ee8769914046920d649bbae9f81b`)
contains a VC6-adapted NSS 3.42 Beta snapshot and NSPR 4.7.7. RetroZilla's
release notes explicitly advertise the NSS update with TLS 1.3, and its build
instructions use Visual Studio 6.0 SP5 plus Processor Pack on Windows 2000 SP4
for binaries targeting Windows 95 and Windows NT 4.0. Source inspection found
TLS 1.3, ChaCha20-Poly1305, AES-GCM, X25519/P-256, ALPN, version-range controls,
nonblocking NSPR I/O, session/0-RTT controls and standalone NSS entry points.

This is substantially stronger legacy evidence than the first-spike
candidates, but it does **not** satisfy the frozen authentication profile.
RetroZilla NSS 3.42 has only resumption PSKs; it has no external-PSK API or
identity lookup. Upstream first added TLS 1.3 external PSK in NSS 3.54
(Bug 1603042, commits `a2293e897889027856a26404ced424f09ecb2130` and
`c1b1112af415759e73c3219fbfbcc1004cae5bd7`). The principal upstream change
touches 30 files with 1,608 insertions and 318 deletions, including the TLS
handshake, extension, socket, PKCS#11-key and test paths. NSS 3.54 also requires
NSPR 4.26, while RetroZilla carries NSPR 4.7.7. The experimental API accepts
only one PSK per socket, not the required dynamic lookup among distinct
per-client identities. This is a **LARGE CROSS-CUTTING BACKPORT** and still
does not provide the frozen server credential model.

Therefore the investigation reached a defensible result, while the parent
backend selection remains open:

```text
RETROZILLA NSS BACKEND INVESTIGATION READY
PROFILE RECONSIDERATION REQUIRED
CRYPTO/TLS BACKEND SPIKE NOT READY
```

The complete lineage, dependency graph, capability matrix, licensing,
footprint, entropy, I/O and alternatives analysis is in the linked R2 report.

## Post-investigation profile revision — 3.A2A-R1

The requested profile reconsideration is complete. TLS 1.3 mTLS with a
private/administrator CA and individual client-device certificates supersedes
the external-PSK profile. External PSK is therefore no longer a 3.A2B gate.

The next 3.A2B-R3 spike must instead prove certificate-authenticated TLS 1.3 on
legacy and modern targets; client/server key and trust-anchor handling; full
independent CONTROL/DATA mTLS; ALPN and algorithm policy; certificate wall-clock
validation; fail-closed entropy across the complete platform matrix;
nonblocking I/O and close/error distinctions; structured browser errors;
footprint, licensing and cross-backend interoperability. The authoritative,
complete gates are in the revised profile. This addendum selects no backend and
does not change the historical `CRYPTO/TLS BACKEND SPIKE NOT READY` result.
