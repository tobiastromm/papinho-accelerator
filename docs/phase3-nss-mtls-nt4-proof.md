# Phase 3.A2B-R3 RetroZilla NSS mTLS / NT4 Runtime Proof

Status: focused backend closeout report, 2026-08-30. Result: **NOT READY**.
No production source, CMake target, PACC message, credential, TLS integration,
PapinhoSecureTransport project or Phase 3.B work was added.

```text
NT4 RUNTIME EXECUTION REQUIRED
RETROZILLA NSS LEGACY TLS BACKEND NOT READY
```

## Decision

The exact RetroZilla lineage remains a credible candidate, but 3.A2B cannot
close. No Windows NT 4.0 runtime or VM was accessible, and the RetroZilla
distribution contains runtime DLLs but no standalone `tstclnt`, `selfserv`,
`certutil`, `pk12util`, matching import libraries or prepared certificate DB.
Consequently none of the mandatory credentialed TLS runtime gates can honestly
be marked proven.

A reproducible VC6-built prerequisite bundle was prepared under ignored
storage at `build\spikes\phase3-a2b-r3-nss-mtls\nt4-bundle\`. It proves on the
available build host that a genuine `_MSC_VER 1200` Win32 x86 program can load
the distributed NSS/NSPR DLL graph, initialize NSPR and `NSS_NoDB_Init`,
discover required SSL entry points and use Windows CryptoAPI. It deliberately
does not pretend to be the missing mTLS handshake proof.

## Exact lineage and origin

| Item | Evidence |
|---|---|
| RetroZilla tag | `2.3-release` |
| RetroZilla commit | `2f274574d3c6ee8769914046920d649bbae9f81b` |
| NSS | RetroZilla snapshot approximately NSS 3.42 Beta; closest upstream lineage `NSS_3_42_RTM` |
| NSPR | 4.7.7 |
| Source/binary origin | Existing ignored R2 checkout and unmodified RetroZilla 2.3 release archive/distribution |
| Release archive SHA-256 | `186579FFE158D892AABE68CFF8AF620FFC8FF4207DE37D1D2C25A5AC70121A76` |
| `nss3.dll` SHA-256 | `B1C26DBFD9947881BE59F85DE7498A0EA7D019FFF262E22B1156C6E177A6B77D` |
| `ssl3.dll` SHA-256 | `402AEBABB3B08F514D676CFF54D936C6485A4C095C040AD80A97245DC6A5FB33` |
| `nspr4.dll` SHA-256 | `B94AF0D9BCA4B1016F281144A5EBF6CC1AFA3AA9FDC9A53D9E4E26214AB4D1DE` |
| License lineage | NSS MPL 2.0; NSPR MPL 2.0/GPL 2.0/LGPL 2.1 tri-license as recorded by R2 |

The checked-out tag and commit match the R2 baseline. No modern NSS binary was
substituted. The tested lineage is old/frozen; successful mechanics would not
prove it contains current security fixes or settle future maintenance policy.

## Build and prerequisite execution

| Property | Result |
|---|---|
| Build host | Windows 10 Pro 22H2, build 19045, AMD64 |
| Compiler | Microsoft 32-bit C/C++ Optimizing Compiler 12.00.8804 for 80x86 |
| VC6 proof | Runtime output reports `_MSC_VER=1200`; object/executable built by the VC6 compiler/linker |
| Boundary | C89-compatible application source, Win32 x86, `_WIN32_WINNT=0x0400` |
| Build warnings | Zero after locally suppressing VC6 SDK header warning C4201 around `wincrypt.h` |
| Available-host execution | PASS, but this is not an NT4 result |
| Actual NT4 version/SP | NOT AVAILABLE / NOT PROVEN |

The available-host prerequisite result was:

```text
BUILD compiler_msc_ver=1200 arch=x86 boundary=C89
ENTROPY dll=advapi32.dll result=LOADED
ENTROPY api=SystemFunction036 result=PRESENT
ENTROPY api=SystemFunction036 call=SUCCESS
ENTROPY api=CryptAcquireContextA provider=PROV_RSA_FULL flags=CRYPT_VERIFYCONTEXT result=SUCCESS
ENTROPY api=CryptGenRandom bytes=32 result=SUCCESS
LOAD module=nspr4.dll result=SUCCESS
NSPR_INIT result=SUCCESS
LOAD module=nss3.dll result=SUCCESS
NSS_INIT mode=NSS_NoDB_Init result=SUCCESS code=0
LOAD module=ssl3.dll result=SUCCESS
SSL_ImportFD / SSL_VersionRangeSet / SSL_SetNextProtoNego present
SSL_GetNextProto / SSL_AuthCertificateHook present
SSL_GetClientAuthDataHook / SSL_ForceHandshake / SSL_PeerCertificate present
NSS_SHUTDOWN result=SUCCESS code=0
FINAL result=PASS
```

`GetVersionExA` reported the compatibility-manifest view `6.2` on this modern
host; it is not used as proof of the host or target OS. The NT4 run instructions
require independently recording edition and service pack.

## Intended complete test path

```text
actual Windows NT 4.0
    -> VC6 x86 standalone client
    -> RetroZilla NSPR 4.7.7 + NSS ~3.42 DLLs
    -> TLS 1.3-only mTLS + ALPN papacc/1
    -> NSS test server, then an optional OpenSSL 3.6.3 server
```

The intended client must use its own key/certificate DB, validate the server
chain, present an individual client certificate, drive `SSL_ForceHandshake`
nonblockingly, reject missing/wrong ALPN before application framing, exchange
`PAPACC NSS MTLS PROBE` / `OK`, and close cleanly. The server must require and
validate the client certificate. Client A and B must be separately observable
through authenticated peer-certificate data without defining Papinho principal
semantics or using subject CN alone.

The complete credentialed harness was not fabricated from incomplete evidence.
The distributed release lacks NSS command tools and import libraries needed to
prepare and link that proof directly. Rebuilding the exact source lineage or
producing reviewed import libraries plus certificate tooling remains required.

## PKI, algorithms and protocol results

No disposable PKI was generated because there was no executable mTLS harness
or NT4 target on which it could satisfy a gate. Therefore server/client key
algorithms, certificate signature algorithms and NSS DB behavior remain **NOT
PROVEN**. RSA-2048/SHA-256 and ECDSA P-256/SHA-256 remain candidates, not
selections. Certificate authentication keys remain distinct from ephemeral
groups such as X25519 and P-256.

OpenSSL 3.6.3 (Cygwin x86_64) was locally available, but no cross-backend result
is claimed because the RetroZilla client handshake side was absent. OpenSSL is
not selected as the server backend.

## Entropy result

Source evidence remains unchanged: `security/nss/lib/freebl/win_rand.c`
dynamically loads `advapi32.dll`, tries `SystemFunction036`, then
`CryptAcquireContextA`/`CryptGenRandom` with `PROV_RSA_FULL` and
`CRYPT_VERIFYCONTEXT`. If secure OS paths fail, the historical implementation
can continue through `rng_systemFromNoise`; that environmental fallback is
forbidden by the Papinho profile.

The prerequisite probe calls both API families and reports only status, never
random data. On Windows 10 both succeeded. This does not prove which path the
unmodified NSS DLL took internally on NT4. No entropy patch or injection hook
was made, so forced failure, fail-closed behavior and absence of fallback are
**NOT PROVEN**. A later isolated source experiment must make secure-provider
failure fatal without changing or replacing the RNG algorithm. If that cannot
be a small policy patch, it is a security blocker.

Win95/98/Me, NT 3.x, Windows 3.11/Win32s and Windows 2000/XP remain separate
future entropy/runtime audits. R3 does not expand its NT4 gate to those systems.

## Bundle inventory and execution

The ignored bundle is 2,717,032 bytes including source/object/instructions. It
contains the 40,960-byte VC6 probe, RetroZilla `nspr4`, `plc4`, `plds4`, `nss3`,
`nssutil3`, `ssl3`, `smime3`, `softokn3`, `freebl3`, `sqlite3` DLLs, checksum
companions, source, object, command file and instructions. RAM usage was not
measured. It contains no certificate DB, certificate, key or secret.

To execute the prerequisite gate:

1. Copy the complete `nt4-bundle` directory to the actual NT4 machine.
2. Record NT4 edition/service pack.
3. Run `RUN_NT4_TEST.cmd`.
4. Preserve and return `NT4-RESULT.txt` unchanged.
5. Treat anything other than `FINAL result=PASS` and exit 0 as a classified
   loader/API/provider/NSS prerequisite failure.

The expected lines and security caveats are included in
`README-NOT-FOR-GIT.txt`. This bundle is a prerequisite probe only. A second
bundle with the complete mTLS client and disposable credential DB is still
required before READY.

## Acceptance matrix

| Gate | Result | Evidence |
|---|---|---|
| Exact RetroZilla NSS lineage used | PROVEN | tag/commit and release hashes |
| VC6-compatible client build | PARTIAL | VC6 prerequisite probe built; mTLS client not built |
| Actual NT4 runtime | NOT PROVEN | no accessible NT4 runtime |
| NSS initialization on NT4 | NOT PROVEN | modern-host `NSS_NoDB_Init` only |
| TLS 1.3 handshake | NOT PROVEN | no credentialed runtime harness |
| TLS 1.3-only enforcement | NOT PROVEN | API/export/source only |
| TLS 1.2 downgrade rejected | NOT PROVEN | not executed |
| Cleartext peer rejected | NOT PROVEN | not executed |
| mTLS server authentication | NOT PROVEN | not executed |
| mTLS client authentication | NOT PROVEN | not executed |
| Missing client cert rejected | NOT PROVEN | not executed |
| Untrusted client rejected | NOT PROVEN | not executed |
| Untrusted server rejected | NOT PROVEN | not executed |
| Client A/B distinguishable | NOT PROVEN | peer-certificate export present only |
| Peer certificate extraction | PARTIAL | `SSL_PeerCertificate` export present; no authenticated runtime peer |
| ChaCha20-Poly1305 negotiated | NOT PROVEN | source/compiled presence is insufficient |
| AES-128-GCM tested | NOT PROVEN | not executed |
| X25519 negotiated | NOT PROVEN | source/compiled presence is insufficient |
| P-256 tested | NOT PROVEN | not executed |
| Forward-secret exchange | NOT PROVEN | no handshake |
| ALPN `papacc/1` negotiated | NOT PROVEN | set/get exports present only |
| Missing ALPN rejected | NOT PROVEN | not executed |
| Wrong ALPN rejected | NOT PROVEN | not executed |
| 0-RTT disabled/not available | NOT PROVEN | source/API evidence only |
| Resumption disabled | NOT PROVEN | no two-connection runtime test |
| Nonblocking handshake | NOT PROVEN | no handshake |
| `PR_WOULD_BLOCK_ERROR` observed | NOT PROVEN | not executed |
| Select-driven compatibility | NOT PROVEN | not executed |
| No mandatory worker thread | NOT PROVEN | not runtime-observed |
| Partial secure read/write | NOT PROVEN | no secure stream |
| Standalone application I/O | NOT PROVEN | no secure stream |
| Clean `close_notify` | NOT PROVEN | not executed |
| Nonblocking close | NOT PROVEN | not executed |
| Truncation distinguished | NOT PROVEN | not executed |
| Certificate expiry rejected | NOT PROVEN | no PKI/runtime test |
| Not-yet-valid certificate rejected | NOT PROVEN | no PKI/runtime test |
| NT4 secure RNG path proven | NOT PROVEN | no NT4 execution |
| `CryptGenRandom` succeeds | PARTIAL | modern host success; NT4 not run |
| Weak entropy fallback rejected | NOT PROVEN | unmodified DLL retains historical path |
| Forced RNG failure is fail-closed | NOT PROVEN | no injection/policy patch |
| Disk footprint measured | PROVEN | bundle inventory totals 2,717,032 bytes |
| License lineage confirmed | PROVEN | exact R2 tag/distribution lineage |
| No custom crypto | PROVEN | loader/API probe only |

## Constraints learned for a future transport component

Evidence supports, but does not yet freeze, the need to represent NSS global
initialization/shutdown, explicit DLL/provider diagnostics, `PRFileDesc`
ownership, nonblocking retry/readiness, exact ALPN result, authenticated peer
certificate extraction, credential DB/loading, clean-close/truncation state,
entropy-policy initialization and backend error translation. No public API or
project was created.

## Smallest remaining mandatory proof set

1. Build a complete VC6 x86 client against the exact RetroZilla DLL lineage and
   prepare disposable CA/server/client-A/client-B credentials.
2. Execute on actual NT4 and prove TLS 1.3-only mTLS, trust failures, certificate
   validity, ALPN success/failures, forward secrecy and at least one approved
   cipher, preferably ChaCha20-Poly1305 plus X25519.
3. Prove nonblocking handshake/read/write/close, `PR_WOULD_BLOCK_ERROR`, native
   readiness behavior, application I/O and no cleartext/TLS 1.2 fallback.
4. Prove the actual NT4 secure provider path and a small fail-closed NSS policy
   enforcement with injected OS RNG failure and no environmental fallback.

Until all four are backed by runtime evidence, the revised mTLS profile does
not conflict with reality, but its legacy feasibility remains unproven.
3.A2A does not need another revision on current evidence. Phase 2 remains
`PHASE 2 READY`; 3.A2B remains open, and Phase 3.B must not begin.

```text
RETROZILLA NSS LEGACY TLS BACKEND NOT READY
```
