# Phase 3.A2B-R2 — RetroZilla NSS TLS 1.3 investigation

Status: authoritative investigation result, 2026-08-29. The investigation is
**READY**; Phase 3.A2B remains **NOT READY**. No production code, build file,
wire definition, credential or security profile was changed.

Historical-note addendum: Phase 3.A2A-R1 later completed the recommended
profile reconsideration and selected TLS 1.3 mTLS. Statements below that the
external-PSK profile remained frozen describe the state at the close of R2;
they are preserved as investigation history, not current policy. See
[Transport Security and Credential Profile](phase3-transport-security-profile.md).

Final-closeout addendum: the later
[3.A2B-R3 proof](phase3-nss-mtls-nt4-proof.md) completed VC6 modern-host and
Windows NT 4.0 SP6 TLS 1.3 mTLS validation plus normal and forced-failure
entropy proof. RetroZilla NSS/NSPR is therefore READY as a technically viable
legacy backend candidate. Conditional/NOT READY statements below describe this
R2 investigation checkpoint and are not the current 3.A2B status.

## Executive decision

RetroZilla NSS is a credible first TLS backend for legacy VC6/NT4 software,
especially for certificate-authenticated TLS and general HTTPS. It is
separable from Gecko, has a conventional C library boundary and supports the
required transport mechanics. Actual execution of its TLS 1.3 and entropy
paths on the project's NT4 VM was not available, so runtime suitability is
**LIKELY**, not proven.

It cannot satisfy the frozen Phase 3.A2A authentication profile. The exact
RetroZilla baseline has no external PSK. Upstream's later external-PSK work is
cross-cutting, moves from NSS 3.42/NSPR 4.7.7 to the NSS 3.54/NSPR 4.26
generation, and exposes only one external PSK per socket. It does not provide
the server-side multi-client credential lookup required by PapinhoAccelerator.
The frozen profile therefore remains unchanged but requires a separate,
explicit reconsideration.

## Reproducible inputs and lineage

The inspected release is RetroZilla tag `2.3-release`, commit
`2f274574d3c6ee8769914046920d649bbae9f81b`. Third-party sources and the release
ZIP were kept under ignored `build/spikes/phase3-a2b-r2`; none is tracked.

| Item | Exact result | Evidence |
|---|---|---|
| RetroZilla release | 2.3, tag commit `2f274574...` | [official release](https://github.com/rn10950/RetroZilla/releases/tag/2.3-release) |
| NSS identity | `NSS_VERSION "3.42" ... " Beta"`, 3.42.0.0 | `security/nss/lib/nss/nss.h` in that tag |
| NSPR identity | 4.7.7 | `nsprpub/pr/include/prinit.h` in that tag |
| Main NSS import | RetroZilla `a297d38466b565b48bc8cce94b03c1ab1eb0b93d`, authored 2025-04-04 | repository history; whole-tree update |
| Closest upstream base | `NSS_3_42_RTM`, upstream commit `cd6f91ae9` | version and tree comparison |
| Nature of port | upstream NSS import plus legacy/compiler adaptations, not custom Papinho or RetroZilla TLS | history and file comparison |

The tree is not byte-identical to `NSS_3_42_RTM`. Comparison found 86 changed
paths after excluding repository metadata. Material adaptations include
`coreconf/WIN32.mk`, `MSStdInt.h`, VC6-compatible SHA-512 and HACL/KreMLin
syntax, 32-bit Curve25519, disabled unsupported optimized paths, legacy Win32
randomness/loading, SQLite Win9x/NT3.51 shims and certificate-store updates.
The import itself is large because it replaced a much older NSS lineage.
Earlier traceable preparation includes RetroZilla commits `2598eb6` (VC6 fixes),
`1c9b432` (TenFourFox/Mozilla changes including a 32-bit X25519 path), and
`db2c369` (Mozilla ChaCha20-Poly1305 changes). TLS 1.3 is Mozilla/NSS protocol
code (`lib/ssl/tls13*.c`); no RetroZilla-specific TLS protocol implementation
was found.

Uncertainty: the import commit message does not name a source archive/tag and
the header still says Beta. `NSS_3_42_RTM` is the closest identifiable upstream
lineage, not a claim of a clean tag import.

## Build and NT4 evidence

RetroZilla documents release builds on Windows 2000 SP4 using Visual Studio
6.0, MozillaBuild 1.2, VC6 SP5 (explicitly not SP6), and the VC6 Processor Pack.
The build is Win32 x86 and its make/configure logic recognizes `_MSC_VER=1200`,
`WIN95`/`WINNT`, and legacy NSPR targets. NSS is part of the source tree and
the produced release contains its VC6-era DLL set; it is not merely a modern
NSS loaded by a VC6 browser shell. The source mixes C and build-time tools;
the TLS/freebl core is predominantly C, with generated C and optional x86
assembly/optimized implementations selected by the makefiles.

RetroZilla's README names Windows NT 4.0 as a target and the official 2.3
binary is built by that legacy toolchain. The release notes say NSS was updated
with TLS 1.3. This proves distribution intent and compile/link viability. It
does **not** prove that a TLS 1.3 handshake was executed on NT4. No accessible
NT4 execution endpoint or RetroZilla test log tied together OS version,
cipher/group and handshake. That axis remains **NOT PROVEN**.

## Standalone boundary and dependency graph

NSS is expressly a cross-platform client/server library, and this port retains
the normal standalone tools (`tstclnt`, `selfserv`), `NSS_NoDB_Init`,
`SSL_ImportFD`, public `ssl3.dll` exports and NSPR APIs. A standalone C program
does not require Gecko, XPCOM or browser UI. RetroZilla's `security/manager`
layer is a consumer, not a prerequisite.

```text
Papinho application
  -> future PapinhoSecureTransport (not created here)
    -> ssl3.dll
      -> nss3.dll
        -> nssutil3.dll
        -> softokn3.dll (internal PKCS#11 software token)
          -> freebl3.dll + integrity .chk files
        -> NSPR: nspr4.dll + plc4.dll + plds4.dll
        -> sqlite3.dll when persistent certificate/key DB is used
        -> nssckbi.dll for Mozilla built-in public CA roots when selected
        -> nssdbm3.dll only for legacy DB format when selected
      -> NSPR PRFileDesc/native Winsock
```

TLS cryptographic operations still travel through NSS's PKCS#11 abstraction,
softoken and freebl; a PSK profile would not reduce NSS to only `ssl3.dll`.
`NSS_NoDB_Init` avoids a persistent certificate database and SQLite DB use,
but does not remove softoken/freebl/NSPR. Certificate server/mTLS/public HTTPS
needs certificate, key and trust handling; a private CA can use an application
managed database or imported objects, while normal web HTTPS additionally
needs a maintained trust store.

The release uses DLLs, not a single static TLS archive. A future standalone
deployment could investigate static builds, but the tested artifact is the DLL
model. Symbol isolation and deployment favor a backend-private directory or
controlled loader policy; no packaging decision is made here.

## Exact release footprint

The official `retrozilla-2.3.en-US.win32.zip` is 12,677,968 bytes. Relevant
uncompressed release files are:

| File | Bytes |
|---|---:|
| `ssl3.dll` | 245,760 |
| `nss3.dll` | 811,008 |
| `nssutil3.dll` | 126,976 |
| `softokn3.dll` | 540,672 |
| `freebl3.dll` | 380,928 |
| `nspr4.dll` | 159,744 |
| `plc4.dll` | 28,672 |
| `plds4.dll` | 24,576 |
| `sqlite3.dll` | 221,184 |
| `nssckbi.dll` | 380,928 |
| `nssdbm3.dll` | 110,592 |
| two integrity `.chk` files | 1,798 |

The core SSL/NSS/softoken/freebl/NSPR DLLs total 2,318,336 bytes, excluding
SQLite, trust roots and legacy DB. Adding SQLite and built-in roots gives
2,920,448 bytes, excluding the optional legacy DB and check files. These are
disk figures, not a minimal-build proof. Per-process and per-connection RAM,
TLS buffers and stack were not measured; no RAM number is inferred.

## Transport and scheduler fit

NSS SSL is an NSPR I/O layer placed over a `PRFileDesc`. `SSL_ImportFD` returns
the layered descriptor; closing the top descriptor closes the lower layers, so
ownership must be transferred deliberately. NSPR can wrap/create native
Winsock descriptors or a private I/O layer can bridge callbacks. NSS/NSPR types
can remain entirely inside a future backend implementation.

For a nonblocking descriptor, `SSL_ForceHandshake`, `PR_Read` and `PR_Write`
return failure with `PR_WOULD_BLOCK_ERROR`; later readiness drives retry.
Conceptually, a future adapter maps the operation's pending read/write need to
`WANT_READ`/`WANT_WRITE`, successful intermediate work to `PROGRESS`, handshake
callback/completion to `COMPLETE`, and other NSS/NSPR errors to `FATAL`.
The final API is deliberately not designed here. Reads/writes can be partial;
NSS buffers handshake/record output, so the caller must preserve operation
state and retry rather than replay application bytes blindly.

NSS/NSPR use global initialization and internal locks but do not require an
application worker thread for a nonblocking handshake. A single application
thread with a `select()` scheduler is a plausible model. Thread-safety and
global shutdown ordering remain backend lifecycle responsibilities.

## TLS feature findings

| Requirement | RetroZilla NSS 3.42 result | Strength |
|---|---|---|
| TLS 1.3 client/server code | present in `tls13*.c`, suites and tests | PROVEN in source |
| TLS 1.3 only | `SSL_VersionRangeSet` can set min=max TLS 1.3 | PROVEN in API/source |
| ChaCha20-Poly1305 | suite `0x1303`, softoken/freebl implementation and release DLL | PROVEN compiled; negotiation on NT4 NOT PROVEN |
| AES-128-GCM | suite `0x1301`, portable fallback plus optional acceleration | PROVEN compiled; negotiation on NT4 NOT PROVEN |
| X25519 | group 29 and 32-bit `curve25519_32.c` legacy path | PROVEN compiled; handshake on NT4 NOT PROVEN |
| P-256 | NSS ECC/softoken path present | PROVEN compiled; handshake on NT4 NOT PROVEN |
| ALPN | client list, server callback and `SSL_GetNextProto` | PROVEN in API/source |
| SNI | supported for HTTPS; not required by TLS or PSK/certificate mechanics | PROVEN optional |
| Disable 0-RTT | `SSL_ENABLE_0RTT_DATA = PR_FALSE` | PROVEN in API/source |
| Disable resumption | `SSL_NO_CACHE`, session tickets off, cache clearing | PROVEN in API/source; policy composition not runtime-tested |
| mTLS | client/server certificates, auth hooks, PKCS#11 keys and trust | PROVEN in API/source |

Both X25519 and P-256 remain candidates. X25519 is preferred for the Papinho
profile because the tree has a dedicated 32-bit implementation and avoids a
general big-integer curve path, but this is a recommendation pending NT4
runtime and resource measurement. It uses 64-bit arithmetic where available
and a legacy 32-bit C implementation; no SSE/AES-NI/AVX instruction is
mandatory. VC6 gates disable modern AES/GCM and vectorized HACL paths; portable
C fallbacks remain. Optional x86 assembly and optimized paths are separate.

For `papacc/1`, the client can advertise the exact ALPN vector and the server
callback can select only that value. The application must reject callback
failure, `SSL_NEXT_PROTO_NO_SUPPORT`, no negotiated value or a mismatching
value. This makes missing/wrong ALPN fail-closed feasible. SNI can remain unset
for a dedicated Papinho endpoint. General HTTPS instead uses SNI, certificate
hostname verification (for example `CERT_VerifyCertName`), a web trust store
and a separately configured TLS 1.2/1.3 compatibility policy.

## Close, truncation and errors

`ssl_SecureClose`/`ssl_SecureShutdown` attempt to send `close_notify`; source
warns that the send can fail without retry in nonblocking mode. Therefore a
future adapter needs an explicit incremental shutdown state rather than relying
on `PR_Close` alone. Receipt sets `recvdCloseNotify`; the public
`SSL_AlertReceivedCallback` exposes the close alert. A raw transport EOF without
that callback is distinguishable and must map to truncation. This is
**source-feasible but not runtime-proven**.

Conceptual error mapping: `PR_WOULD_BLOCK_ERROR` -> retry; received
`close_notify` followed by EOF -> clean secure EOF; EOF/reset without it ->
truncation/transport failure; fatal TLS alerts -> peer/protocol failure;
`SSL_ERROR_BAD_MAC_READ` -> integrity/authentication failure; certificate or
unknown-CA errors -> peer authentication failure; ALPN callback/no-selection ->
profile failure; remaining SEC/SSL/PR errors -> internal/fatal with the native
code retained for diagnostics.

## Randomness on NT4

The exact `freebl/win_rand.c` dynamically loads `advapi32.dll`. It first looks
up `SystemFunction036` (`RtlGenRandom`, documented in the source as XP/2003+),
then looks up `CryptAcquireContextA`, `CryptGenRandom` and
`CryptReleaseContext`. The CryptoAPI path uses `PROV_RSA_FULL` with
`CRYPT_VERIFYCONTEXT`; source explicitly identifies CryptoAPI as available on
NT4 and Windows 95 OSR2+. This avoids a load-time dependency on newer entry
points.

There is a security caveat: when both OS PRNG paths fail, the snapshot silently
falls back to `rng_systemFromNoise`, which fills output from performance ticks,
time and environmental/file jitter. That fallback does not meet the Papinho
rule for cryptographic entropy. Selection would require fail-closed proof that
the NT4 CryptoAPI provider succeeds (and likely a narrowly reviewed policy
patch that rejects fallback), not acceptance of the fallback. No NT4 entropy
runtime was available, so CSPRNG suitability is **PARTIAL/LIKELY**, not proven.
No custom PRNG or cryptography was implemented.

## External PSK — decisive profile conflict

RetroZilla NSS 3.42 parses and emits TLS 1.3 `pre_shared_key`, but all keys are
derived from session tickets/resumption. Searches of public headers, socket
state, tests and handshake code found no external-PSK configuration API,
identity object or server lookup callback. Therefore:

```text
EXTERNAL PSK NOT PRESENT IN RETROZILLA NSS BASELINE
```

Upstream first released external PSK in **NSS 3.54** on 2020-06-26 (Bug
1603042). The authoritative commits recorded by Bugzilla are
`a2293e897889027856a26404ced424f09ecb2130` (main support) and
`c1b1112af415759e73c3219fbfbcc1004cae5bd7` (tool/test follow-up); the Git
mirror equivalents inspected were `d5cb0616b` and `5b672708f`. The main change
is 1,608 insertions/318 deletions across 30 files. It adds `tls13psk.c/.h`,
changes `tls13con.c`, extension handlers, socket/config structures, channel
information, experimental exports and 514 lines of focused PSK tests. NSS 3.54
requires NSPR 4.26 or newer.

The experimental `SSL_AddExternalPsk` API takes a `PK11SymKey`, arbitrary
byte identity and SHA-256/SHA-384 selection. A 16-byte opaque identity and a
32-byte imported secret are representable. It supports both roles when the
same PSK is configured, unknown identity fails selection, and NSS only accepts
`psk_dhe_ke` in this generation, so pure `psk_ke` is rejected. However, the
public contract explicitly accepts **one PSK per socket**. It provides no
server callback that maps an arbitrary offered identity to one of many
per-client secrets. A separate preconfigured socket per already-known client
does not solve identity discovery on a shared Papinho listener.

Backport classification: **LARGE CROSS-CUTTING BACKPORT / EFFECTIVELY REQUIRES
A NEWER NSS GENERATION**. It changes TLS binder and handshake protocol code,
not merely adapters. Although it is authoritative upstream code and therefore
not invented cryptography, transplanting it into the VC6 fork would require
auditing a broad 3.42→3.54 dependency/PKCS#11/compiler/NSPR delta and still
designing capability absent upstream for multi-client lookup. It is not a
small, plausibly auditable compatibility patch for this phase.

Sources: [NSS 3.54 release announcement](https://groups.google.com/g/mozilla.dev.tech.crypto/c/MQ5oYOEpn_Q),
[Bug 1603042 and authoritative revisions](https://bugzilla.mozilla.org/show_bug.cgi?id=1603042),
and [current experimental API contract](https://searchfox.org/firefox-main/source/security/nss/lib/ssl/sslexp.h).

## PROFILE RECONSIDERATION CANDIDATE

The profile is not changed here.

| Option | Security properties | Engineering/licensing cost |
|---|---|---|
| A. Keep external PSK | Retains frozen PSK-DHE model | Large NSS-generation backport; upstream one-PSK API still lacks required server lookup; highest protocol-maintenance risk |
| B. Switch initial profile to private-CA mTLS | Standard TLS 1.3 mutual authentication and per-client certs already implemented | Provision private CA/server/per-client certs, protect keys, maintain revocation/rotation and dependable wall clock |
| C. Server-auth TLS + application authentication | Standard TLS channel plus flexible per-client mechanism | Reintroduces a second authentication protocol and binding/replay design; must be separately standardized, not invented here |
| D. Reject NSS | Preserves profile unchanged | Returns to an unproven VC6 backend and wolfSSL licensing/toolchain blockers |

mTLS is the best evidenced NSS-native alternative. NSS already implements
X.509, server and client certificate auth, private-key handling, PKCS#11,
chain/trust validation and private trust anchors. A Papinho private CA does not
need public PKI. Certificate validity uses NSPR wall-clock time (`PR_Now` and
certificate time validation); RetroZilla's normal HTTPS validation on NT4 gives
a credible mechanical path, but clock correctness is an operational
requirement and was not runtime-proven here.

The recommended next decision is a separate 3.A2A design revision comparing a
private-CA mTLS profile against retaining external PSK with another backend.
Do not begin 3.B until that decision and an NT4 runtime/backend validation close
3.A2B.

## Licensing and redistribution

The imported NSS `COPYING` file and inspected NSS SSL, freebl and softoken
files use **Mozilla Public License 2.0**. MPL 2.0 is file-level copyleft: the
license text says NSS and modifications to covered files must remain available
under MPL 2.0, while permitting combination in a larger work. The old NSPR
4.7.7 files retain the historical **MPL 1.1 / GPL 2.0 / LGPL 2.1 tri-license**.
RetroZilla modifications inspected in NSS 3.42 files carry the same MPL 2.0
headers; no extra component-specific restriction was found. Existing notices,
source availability for modified covered files and attribution/license copies
must be preserved. This is a text-based engineering assessment, not legal
advice.

This permits a DLL/library structure without forcing the entire Papinho
ecosystem to GPLv3, subject to MPL/NSPR notice and covered-source obligations.
That contrasts with wolfSSL's GPLv3-or-commercial choice: NSS does not create
the same whole-project GPLv3/commercial-license policy dependency.

## Maintenance, HTTPS and future abstraction

NSS 3.42 is a frozen 2019-era security stack carried by a legacy browser and is
far behind current NSS. The 2025 import demonstrates that large upstream
updates can be adapted, but it also demonstrates their cost: hundreds of
thousands of changed lines plus compiler/OS shims. Selective security fixes are
possible when isolated, yet advisories may depend on newer refactors, PKCS#11
APIs, generated HACL code or NSPR. A maintained product needs an advisory
inventory, reproducible VC6 builds, NT4 regression tests and reviewed upstream
backports. “Builds once” is not a security-update strategy.

Plausible strategies are: (A) a pinned RetroZilla-derived branch with selective
upstream security backports; (B) progressively port newer NSS while retaining
VC6 shims; or (C) use it as a bootstrap backend behind a stable abstraction and
migrate later. Evidence favors C as the least coupling, but no backend or API
is selected here.

For PapinhoBrowser HTTPS, NSS has server-certificate validation, hostname
verification primitives, SNI, ALPN, TLS 1.2/1.3 and trust-store machinery, so
it could supplement or replace BearSSL after integration tests and trust policy
work. BearSSL need not be removed; both can coexist behind a future transport
abstraction during migration. Papinho transport can enforce TLS 1.3 plus
`papacc/1`, while web HTTPS can use a distinct compatibility/trust/HTTP-ALPN
profile. Applications need not see NSS/NSPR types.

The investigation teaches four constraints for a future
PapinhoSecureTransport boundary: backend-owned socket/layer lifetime;
incremental nonblocking handshake and shutdown; explicit negotiated
version/cipher/group/ALPN reporting; and distinct clean EOF, truncation,
authentication and integrity failures. No repository or final API was created.

## Test inventory and unperformed spike

The tree retains NSS SSL gtests and scripts covering TLS 1.3 handshakes,
ciphers, ALPN, certificates, resumption PSK, malformed extensions/records,
alerts and close behavior. Their presence is not evidence that they ran on
VC6/NT4. A standalone compile was not attempted: the release has runtime DLLs
but no SDK/import-library bundle, and rebuilding the full MozillaBuild/VC6 tree
would not prove NT4 runtime. The next meaningful experiment is a small
standalone client/server linked against a reproducible RetroZilla NSS build and
executed in the NT4 VM, not a Windows-modern smoke mislabeled as NT4 proof.

## Final classification

| Axis | Classification | Basis |
|---|---|---|
| TLS 1.3 capability | PROVEN | source, exports, tests and official release claim |
| VC6 build capability | PROVEN | official build recipe and distributed DLLs |
| NT4 runtime capability | LIKELY / NOT PROVEN | target claim, no exact TLS 1.3 NT4 execution log |
| Standalone-library usability | PROVEN | standard NSS tools/APIs and DLL boundary |
| Nonblocking suitability | PROVEN in source | NSPR nonblocking/would-block paths |
| ALPN suitability | PROVEN in source | client/server APIs and result query |
| Cipher/group suitability | PROVEN compiled; runtime NOT PROVEN | exact suites/groups and DLL build |
| CSPRNG suitability | PARTIAL | NT4 CryptoAPI path plus unacceptable silent fallback |
| External PSK suitability | INCOMPATIBLE | absent in 3.42; later API lacks multi-client lookup |
| mTLS suitability | LIKELY | complete stack; NT4 end-to-end not run |
| License suitability | LIKELY | MPL 2.0 plus NSPR tri-license; policy review remains |
| Footprint | PARTIAL | disk measured, RAM not measured |
| Maintainability | PARTIAL / HIGH RISK | old fork and broad upstream delta |

## Final decision table

| Question | Result | Evidence |
|---|---|---|
| RetroZilla really contains TLS 1.3 NSS | YES | NSS 3.42 TLS sources/suites/tests and release note |
| NSS compiled with VC6 | YES | documented VC6 release build plus NSS DLLs |
| TLS 1.3 path works on NT4 | NOT PROVEN | no exact runtime record |
| Standalone NSS use is practical | YES | retained NSS public tools/API, no Gecko dependency |
| NSS dependencies are manageable | PARTIAL | conventional but ~2.32 MiB core DLL graph |
| Nonblocking integration is practical | YES, source-level | `PR_WOULD_BLOCK_ERROR`, `SSL_ForceHandshake`, NSPR I/O |
| ChaCha20-Poly1305 available | YES, compiled | suite/freebl/softoken and DLL artifact |
| X25519/P-256 available | YES, compiled | group and legacy implementations |
| ALPN usable | YES | advertise/select/query APIs |
| 0-RTT can be disabled | YES | explicit socket option |
| Resumption can be disabled | YES | cache/ticket controls |
| Clean close/truncation usable | PARTIAL | alert callback distinguishes; shutdown retry unproven |
| CSPRNG/entropy path viable on NT4 | LIKELY, NOT PROVEN | CryptoAPI dynamic path; weak fallback must be rejected |
| External PSK present in RetroZilla NSS | NO | resumption-only source; no API |
| External PSK exists in later upstream NSS | YES, NSS 3.54 | release notes/Bug 1603042 |
| Upstream backport appears small/maintainable | NO | 30 files, handshake/extension/PKCS#11, NSPR generation jump |
| mTLS viable if profile changes | LIKELY | existing X.509/client-auth stack |
| License acceptable technically | LIKELY | MPL 2.0 / NSPR tri-license, no GPLv3-only condition |
| Suitable as first PapinhoSecureTransport backend | CONDITIONAL | strong TLS/legacy fit; profile, entropy and NT4 runtime blockers |

## Closure and blockers

- Profile remains valid as a frozen document: **YES**; it was not modified.
- Current profile is satisfied by RetroZilla NSS: **NO**.
- `PROFILE RECONSIDERATION REQUIRED`: **YES**.
- Exact conflict: TLS 1.3 external PSK with PSK-DHE and server-side lookup of
  distinct per-client 16-byte identities/32-byte secrets.
- Runtime blockers: TLS 1.3/ChaCha/X25519/ALPN and CryptoAPI success still need
  execution on the actual NT4 baseline; clean shutdown/truncation needs a
  standalone nonblocking harness; memory needs measurement.
- No PapinhoSecureTransport repository or API was created.
- No custom crypto, TLS protocol logic or external-PSK backport was written.
- No production PapinhoBrowser or PapinhoAccelerator source was modified.
- No third-party tree, binary, credential or test secret is tracked.

```text
RETROZILLA NSS BACKEND INVESTIGATION READY
```
