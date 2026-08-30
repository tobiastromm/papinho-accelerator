# Phase 3.A2B-R3 RetroZilla NSS mTLS / NT4 Runtime Proof

Status: final backend closeout, 2026-08-30: **READY**. Earlier NOT READY
checkpoints remain below as explicitly historical investigation records.
No production source, CMake target, PACC message, credential, TLS integration,
PapinhoSecureTransport project or Phase 3.B work was added.

## R3A modern-host VC6 mTLS harness result

R3A continued the prerequisite investigation with an actual TLS client. A
genuine Visual C++ 6.0 (`12.00.8804`, `_MSC_VER=1200`) Win32 x86 executable
loaded the exact RetroZilla 2.3 NSS/NSPR DLL set and completed real TLS 1.3
mTLS against OpenSSL 3.6.3 on the modern Windows host. All sources, disposable
keys, databases, binaries, logs and server fixtures remain under ignored
`build\spikes\phase3-a2b-r3-nss-mtls\`; none is production material.

The client uses `NSS_InitReadWrite`, the standard `SSL_AuthCertificate`
validation path, an explicit private test-CA trust record, nickname-based
`NSS_GetClientAuthData`, a native Winsock socket imported through
`PR_ImportTCPSocket`, `SSL_ImportFD`, TLS 1.3-only version bounds, ALPN and NSS
secure-stream I/O. It contains no cryptographic implementation and no trust
bypass. The modern NSS tools used to create the disposable SQL certificate
databases were tooling only; the TLS process loaded the exact RetroZilla DLLs.

### R3A runtime evidence

| Gate | Modern-host result | Observed evidence |
|---|---|---|
| VC6 Win32 x86 client | PROVEN | runtime `_MSC_VER=1200`, `arch=x86` |
| Exact RetroZilla runtime lineage | PROVEN | copied release DLL set and previously recorded hashes |
| Trusted Client A mTLS | PASS | TLS 1.3, payload and close completed |
| Trusted Client B mTLS | PASS | separate client certificate/key and successful connection |
| Server distinguishes A/B | PASS | OpenSSL verification log records `CN=Papacc Client A` and `CN=Papacc Client B`; CN is diagnostic, not a principal mapping |
| Server certificate authentication | PASS | standard NSS validation; peer certificate returned by `SSL_PeerCertificate` |
| Untrusted server | REJECTED | `SEC_ERROR_UNKNOWN_ISSUER` during handshake |
| Missing client certificate | REJECTED | server fatal alert before application response |
| Untrusted client certificate | REJECTED | `SSL_ERROR_UNKNOWN_CA_ALERT` before application response |
| TLS 1.3-only | PASS | negotiated version `0x0304`; TLS 1.2-only server rejected with `SSL_ERROR_PROTOCOL_VERSION_ALERT` |
| Cleartext peer | REJECTED | `SSL_ERROR_RX_RECORD_TOO_LONG`; no application data accepted |
| ChaCha20-Poly1305 | PASS | cipher `0x1303`, `ChaCha20-Poly1305` |
| AES-128-GCM | PASS | cipher `0x1301`, `AES-128-GCM` |
| X25519 | PASS | negotiated group 29 |
| P-256 | PASS | negotiated group 23 |
| Forward secrecy | PASS | TLS 1.3 ephemeral X25519 and P-256 runs |
| ALPN `papacc/1` | PASS | `SSL_GetNextProto` state 3, exact eight-byte value |
| Missing ALPN | REJECTED | application policy rejects state 0/empty before payload |
| Wrong ALPN | REJECTED | fatal peer alert during handshake |
| Valid certificate | PASS | trusted server fixture accepted |
| Expired certificate | REJECTED | `SEC_ERROR_EXPIRED_CERTIFICATE` |
| Not-yet-valid certificate | REJECTED | NSS validity rejection (`SEC_ERROR_EXPIRED_CERTIFICATE`) |
| Application I/O | PASS | 56 bytes sent; authenticated HTTP response received |
| Session resumption / 0-RTT | DISABLED | `SSL_NO_CACHE`, tickets off, 0-RTT off; successful runs report `resumed=0`, `early_data=0` |
| Descriptor ownership | OBSERVED | `PR_Close` on the SSL descriptor closes SSL, NSPR descriptor and imported socket; native socket retained only as readiness handle while open |
| Basic TLS shutdown | PASS | `PR_Shutdown(PR_SHUTDOWN_BOTH)` then `PR_Close`, result 0 |
| Genuine incremental nonblocking handshake | **NOT PROVEN** | delayed loopback fixture still let `SSL_ForceHandshake` block; timeout API produced `PR_IO_TIMEOUT_ERROR`, never `PR_WOULD_BLOCK_ERROR` |
| Native `select()` readiness | **NOT PROVEN** | readiness loop exists but the required would-block transition was not obtained |
| Partial read/write retry | **NOT PROVEN** | basic secure I/O passed; deterministic partial retry was not completed |
| Nonblocking close / truncation distinction | **NOT PROVEN** | basic shutdown passed; mandatory edge cases remain |
| Actual NT4 runtime and entropy fail-closed | **NOT PROVEN** | belongs to R3B and no NT4 runtime was available |

The positive default profile was RSA-2048/SHA-256 certificates, TLS 1.3,
ChaCha20-Poly1305, X25519 and ALPN `papacc/1`. Separate constrained server
runs proved AES-128-GCM and P-256. OpenSSL 3.6.3 was only the modern-host test
peer; it is not selected as the PapinhoAccelerator TLS backend.

The harness build emits VC6 SDK-header warnings C4201 and C4514. They do not
originate in harness logic, but the requested zero-warning gate is therefore
also not claimed. No TLS library, production backend or minimum Windows
version was selected by this experiment.

R3A cannot be marked READY because genuine would-block/retry, partial secure
I/O, nonblocking close/truncation and the zero-warning build gate remain open.
R3B must not begin from a falsely promoted artifact. The earlier prerequisite
bundle is therefore not replaced, and the parent R3/3.A2B status remains open.

```text
3.A2B-R3A NOT READY
NEXT: finish the modern-host nonblocking/edge-case harness before R3B
```

## R3A Nonblocking / Readiness Closeout

Status: **READY**, 2026-08-30. This continuation preserves the historical
`VC6 NSS MTLS HARNESS NOT READY` result above and closes its modern-host
nonblocking blockers. R3 and 3.A2B remain open because full NT4 mTLS and
entropy fail-closed proof still belong to R3B.

### Root cause and exact descriptor configuration

The legacy stack was not the cause of the original blocking result. The
experimental C declaration of `PRSocketOptionData` had the wrong VC6 ABI. In
NSPR 4.7.7 its value union contains `PRNetAddr`, whose `PRIPv6Addr` contains
64-bit members; VC6 therefore aligns the union at offset 8. The old harness
wrote `non_blocking` at offset 4, so NSPR's private descriptor flag remained
false even though native `ioctlsocket(FIONBIO)` had succeeded.

The corrected sequence is:

```text
native socket/connect
  -> ioctlsocket(FIONBIO=1)
  -> PR_ImportTCPSocket
  -> exact ABI-correct PR_SetSocketOption(PR_SockOpt_Nonblocking=TRUE)
  -> SSL_ImportFD
  -> PR_GetSocketOption through the SSL layer reports TRUE
```

`SSL_ImportFD` adds an NSPR I/O layer and preserves/delegates the option to the
lower descriptor. Source inspection of the exact RetroZilla tree confirms
that `ssl_FdIsBlocking`, `ssl_SocketIsBlocking`, `SSL_ForceHandshake` and
`ssl_Poll` use that NSPR state. Native `FIONBIO` alone is insufficient because
NSPR also consults its private `nonblocking` flag.

The earlier `PR_IO_TIMEOUT_ERROR` was not an equivalent would-block signal.
The ABI defect left NSPR logically blocking, while
`SSL_ForceHandshakeWithTimeout(..., PR_INTERVAL_NO_WAIT)` explicitly installed
a zero I/O timeout. The lower receive therefore reported deadline expiration.
After correcting the option ABI and returning to `SSL_ForceHandshake`, the
same delayed fixture returned the real `PR_WOULD_BLOCK_ERROR` (`-5998`).

### Readiness, bounded work and ownership

The selected mechanism is `PR_Poll` on the SSL `PRFileDesc`. During the
handshake, `ssl_Poll` in this exact NSS lineage maps caller read/write interest
to the actual TLS need using role, `handshakeBegun` and `lastWriteBlocked`.
After establishment, the harness asks only for READ when retrying `PR_Read`
and only WRITE when retrying `PR_Write`. This removed the prior busy loop in
which native `select()` continually reported the socket writable.

The deterministic proxy accepts TCP and withholds TLS progress for 500 ms.
One handshake step returns would-block, `PR_Poll` waits, and the second bounded
step completes. No worker thread is used by the VC6 client. The scheduler can
enforce its monotonic deadline outside NSS; a timeout is distinct from
would-block.

The raw Winsock handle remains valid and observable until the layered
descriptor is closed, so native `select()` can observe transport readiness.
It is not sufficient as the only backend readiness mechanism because it does
not expose NSS's TLS read/write remapping. `PR_Poll` is therefore required by
this experimental backend; a future backend-neutral readiness abstraction can
contain that fact without exposing `PRFileDesc` or any NSS type.

Successful `PR_ImportTCPSocket` transfers close responsibility to NSPR.
`SSL_ImportFD` pushes the SSL layer. `PR_Close(ssl_fd)` closes the SSL layer,
lower NSPR descriptor and native socket exactly once. The raw socket may be
observed before that call but must never be closed separately.

### Secure I/O and closure

Before the peer sent application data, `PR_Read` returned
`PR_WOULD_BLOCK_ERROR`. The clean-response fixture then required 138 bounded
64-byte secure reads and reconstructed all 8,831 bytes correctly, including
read-would-block/`PR_Poll` retry.

A test-only slow receiver stopped reading for one second while the client sent
a bounded 2 MiB payload. The nonblocking call path completed without stalling;
this runtime did not expose a partial return or write-would-block because the
stack/OS accepted the tested data. Partial-write observation is consequently
`PARTIAL`, while source/runtime establish the normal partial-or-would-block
retry contract. No huge allocation or unbounded write was used.

`SSL_AlertReceivedCallback` and `SSL_AlertSentCallback` proved clean
`close_notify` in both directions. A separate Python TLS fixture sent the
authenticated response and then detached/closed TCP without TLS shutdown. NSS
returned EOF with no received close-notify callback, allowing this harness to
classify truncation separately from clean TLS EOF.

Source inspection establishes an important legacy limitation:
`ssl_SecureShutdown` and `ssl_SecureClose` attempt `SSL3_SendAlert` but discard
its failure before shutting down/closing. On a nonblocking socket the operation
is bounded, but close-notify is one-shot rather than retryable under write
pressure. The alert-sent callback must therefore be used to confirm graceful
closure; otherwise the backend reports an unclean close and closes transport.
No final public close-state API is defined here.

### Nonblocking closeout matrix

| Gate | Result | Evidence |
|---|---|---|
| Native socket nonblocking configured | PROVEN | `ioctlsocket(FIONBIO=1)` before import |
| NSPR descriptor nonblocking configured | PROVEN | ABI-correct `PR_SetSocketOption`; getter returns 1 |
| SSL layer preserves nonblocking | PROVEN | getter through SSL layer returns 1; runtime would-block |
| `PR_IO_TIMEOUT_ERROR` cause understood | PROVEN | zero deadline on NSPR-logically-blocking descriptor caused by bad ABI |
| `PR_WOULD_BLOCK_ERROR` observed | PROVEN | `SSL_ForceHandshake` and pre-payload `PR_Read`, error `-5998` |
| Deterministic would-block test | PROVEN | 500 ms TLS-progress delay proxy |
| Handshake bounded | PROVEN | two steps, one would-block |
| Handshake event-driven | PROVEN | step, `PR_Poll`, step; no busy loop |
| Read readiness understood | PROVEN | SSL-layer `PR_Poll`, READ/EXCEPT after handshake |
| Write readiness understood | PROVEN | SSL-layer `PR_Poll`, WRITE/EXCEPT for write retry |
| Native `select()` usable | PARTIAL | raw handle remains valid, but select alone lacks NSS interest remapping |
| `PR_Poll` usable | PROVEN | actual handshake/read progression |
| Correct readiness mechanism selected | PROVEN | SSL-layer `PR_Poll` |
| Descriptor ownership proven | PROVEN | import/layer source plus single `PR_Close` runtime |
| No double-close risk | PROVEN | native handle is never separately closed after successful import |
| Nonblocking secure read | PROVEN | pre-payload read returned would-block |
| Partial read | PROVEN | 138 reads reconstruct 8,831 bytes |
| Nonblocking secure write | PROVEN | bounded 2 MiB slow-reader test completed without caller stall |
| Partial write/retry semantics | PARTIAL | no partial/would-block observed; retry contract and bounded call path established |
| Clean `close_notify` | PROVEN | sent and received alert callbacks plus EOF |
| Nonblocking close | PROVEN | bounded one-shot legacy NSS behavior; callback verifies success |
| Truncation characterized | PROVEN | EOF without close-notify callback versus clean alert+EOF |
| No mandatory connection worker thread | PROVEN | single client application thread |
| TLS 1.3 regression | PROVEN | version `0x0304` |
| mTLS regression | PROVEN | trusted Client A and Client B pass |
| ChaCha regression | PROVEN | `0x1303`, ChaCha20-Poly1305 |
| X25519 regression | PROVEN | group 29 |
| `papacc/1` regression | PROVEN | exact ALPN state/value |
| Negative cert tests regression | PROVEN | untrusted server, missing client and untrusted client rejected |
| TLS1.2 rejection regression | PROVEN | protocol-version alert |
| Harness warnings fixed | PROVEN | zero warnings attributable to harness code |
| Remaining header warnings classified | PROVEN | two C4201 warnings in immutable VC6 `qos.h`; zero NSS-header warnings |
| NT4 bundle refreshed | PROVEN | final client executable/source copied into ignored bundle |
| No custom crypto | PROVEN | NSS operations and test orchestration only |

The build also formerly emitted five C4514 warnings for unused inline helpers
from `winnt.h`; a narrow VC6 compatibility pragma suppresses only C4514 for
the translation unit. The two remaining C4201 diagnostics are emitted by the
immutable Microsoft `qos.h` anonymous unions. There are zero harness-code,
NSS/NSPR-header and production PapinhoAccelerator warnings in this work.

### Existing manual NT4 prerequisite evidence

The manually returned prerequisite result is now recorded: Windows NT 4.0
build 1381, Service Pack 6, executed the VC6 x86 probe successfully.
`SystemFunction036` was absent; `CryptAcquireContextA(PROV_RSA_FULL,
CRYPT_VERIFYCONTEXT)` and `CryptGenRandom(32)` succeeded. `nspr4.dll`,
`nss3.dll` and `ssl3.dll` loaded; NSPR and `NSS_NoDB_Init` initialized; all
required SSL exports were present; `NSS_Shutdown` and the final probe passed.
This proves the real NT4 SP6 prerequisite only—not TLS 1.3/mTLS, algorithms,
nonblocking TLS, or entropy fail-closed on NT4.

The refreshed ignored bundle contains the final nonblocking VC6 client:

```text
papacc_nss_mtls_probe.exe
SHA-256 01C1CCBE7AE73B46DF63B49765EEFCA9B93A0887BD5301ED26A3A43BFCDF0C12
```

R3A is closed. R3B was not started.

```text
3.A2B-R3A READY
3.A2B-R3B READY (final manual NT4 evidence recorded below)
```

## 3.A2B-R3B — NT4 Full mTLS Runtime + Entropy Fail-Closed Proof

Status: **READY**, based on the final manual Windows NT 4.0 SP6 evidence
supplied for closeout. Experimental binaries remain non-production artifacts.

The R3A client in the ignored NT4 bundle still hashes exactly to the frozen
value `01C1CCBE7AE73B46DF63B49765EEFCA9B93A0887BD5301ED26A3A43BFCDF0C12`;
no client delta was made. The bundle now also contains the disposable Client A
SQL DB, an NT4-compatible `RUN_R3B_NT4.cmd`, and instructions that capture
`ver`, `date /t`, `time /t`, the complete probe output and exit status in
`R3B-RESULT.TXT`. The modern-host server launcher is
`build\spikes\phase3-a2b-r3-nss-mtls\START_R3B_SERVER.ps1`.

The final run used Windows NT 4.0 SP6, Win32 x86 and the VC6 client
(`_MSC_VER=1200`) against the modern test server. It passed NSS initialization,
TCP, nonblocking setup, bounded `PR_Poll`-driven handshake, TLS 1.3 mTLS,
authenticated payload, partial-read reconstruction and clean shutdown.

### Frozen experimental hashes

| Artifact | SHA-256 |
|---|---|
| VC6 mTLS probe | `01C1CCBE7AE73B46DF63B49765EEFCA9B93A0887BD5301ED26A3A43BFCDF0C12` |
| `nss3.dll` | `B1C26DBFD9947881BE59F85DE7498A0EA7D019FFF262E22B1156C6E177A6B77D` |
| `ssl3.dll` | `402AEBABB3B08F514D676CFF54D936C6485A4C095C040AD80A97245DC6A5FB33` |
| baseline `freebl3.dll` | `56B26EC5BDDCCFAE2C2E1F09AFA0E7BB1131B4D8CEAAA187A01DBFD59F162320` |

### Entropy policy and rebuilt variants

The exact historical fallback is
`security/nss/lib/freebl/win_rand.c:RNG_SystemRNG`: failure to load
`advapi32.dll`, missing secure exports, `SystemFunction036` failure,
`CryptAcquireContextA` failure or `CryptGenRandom` failure ultimately calls
`rng_systemFromNoise` from `sysrand.c`.

The MPL-header-preserving delta changes only this decision:
secure-provider failure sets `SEC_ERROR_NEED_RANDOM` and returns zero. Existing
`drbg.c:rng_init` already treats a zero result as `PR_FAILURE` and leaves the
global RNG unavailable. It was built with VC6 in Windows XP using VS6 SP5,
the VC6 Processor Pack, MozillaBuild 1.2, compatible legacy GNU make and exact
RetroZilla commit `2f274574d3c6ee8769914046920d649bbae9f81b`. The incremental
build rebuilt `win_rand.c` to `sysrand.obj`, relinked `freebl3.dll`, and ran
`shlibsign` to create the matching `freebl3.chk`.

No RNG, DRBG, cipher, MAC, AEAD, curve, ECDHE, certificate, signature or TLS
handshake algorithm changed. On NT4, the normal fail-closed build retained the
required fallback within secure Windows providers: unavailable
`SystemFunction036` led to `CryptAcquireContextA(PROV_RSA_FULL,
CRYPT_VERIFYCONTEXT)` and successful `CryptGenRandom`; TLS 1.3 mTLS still
passed. The rejected environmental `rng_systemFromNoise` path was removed.

A strictly test-only variant forced `RNG_SystemRNG()` to return zero without
providing fake bytes or another fallback. On NT4, `NSS_Init` failed with
`pr_error=-8023`, `SEC_ERROR_PKCS11_DEVICE_ERROR`. No TLS handshake, ALPN,
authentication, payload or successful result occurred. OpenSSL remained at
its initial `ACCEPT` waiting state, with no negotiation, client certificate or
verification evidence. Thus forced secure-RNG failure prevents TLS startup and
payload transfer: the backend fails closed. The injection binary is test-only
and must never be distributed as a production variant.

The earlier local MSYS2/VC6 rebuild failure recorded during preparation was an
environment limitation, not the final build result. The successful XP legacy
toolchain build above supersedes it for the R3B closeout while preserving that
history of the local attempt.

### R3B acceptance matrix

| Gate | Result | Evidence |
|---|---|---|
| Actual NT4 SP6 runtime | PROVEN | prior prerequisite probe: NT 4.0 build 1381 SP6 |
| Exact VC6 executable verified by hash | PROVEN | frozen SHA-256 above |
| `_MSC_VER=1200` | PROVEN | prior NT4 prerequisite and frozen VC6 client |
| NSS/NSPR lineage unchanged | PROVEN | RetroZilla tag/commit and DLL hashes |
| TLS 1.3 on NT4 | PROVEN | negotiated `0x0304` |
| TLS 1.3-only on NT4 | PROVEN | frozen client policy and R3A negative regression |
| mTLS server authentication | PROVEN | validated peer certificate present |
| mTLS client authentication | PROVEN | server accepted the client certificate |
| Client certificate loaded | PROVEN | full NT4 mTLS handshake passed |
| Peer certificate extracted | PROVEN | peer certificate reported present |
| ChaCha20-Poly1305 on NT4 | PROVEN | cipher `0x1303` |
| X25519 on NT4 | PROVEN | group 29 |
| Forward secrecy on NT4 | PROVEN | TLS 1.3 ephemeral X25519 |
| `papacc/1` on NT4 | PROVEN | ALPN exact value |
| `PR_WOULD_BLOCK_ERROR` on NT4 | PROVEN | observed during nonblocking progression |
| `PR_Poll` readiness on NT4 | PROVEN | handshake readiness path |
| Bounded handshake on NT4 | PROVEN | event-driven bounded progression |
| Single-thread viability on NT4 | PROVEN | harness architecture and runtime pass |
| Secure read/write on NT4 | PROVEN | authenticated payload and reconstruction |
| Payload exchange on NT4 | PROVEN | payload PASS |
| Clean close on NT4 | PROVEN | close-notify and CLEAN_CLOSE classification |
| Truncation behavior on NT4 | PARTIAL | R3A proven; NT4 repetition optional after core match |
| Valid certificate accepted | PROVEN | NT4 handshake and peer validation passed |
| TLS1.2 rejected | PROVEN | R3A negative; same frozen TLS1.3-only policy |
| Untrusted server rejected | PROVEN | R3A standard NSS validation path |
| Wrong ALPN rejected | PROVEN | R3A negative; same frozen policy |
| `advapi32.dll` available | PROVEN | prior real NT4 probe |
| `SystemFunction036` status known | PROVEN | absent on tested NT4 SP6 |
| `CryptAcquireContextA` succeeds | PROVEN | prior real NT4 probe |
| `CryptGenRandom` succeeds | PROVEN | prior real NT4 probe, 32 bytes |
| Secure OS entropy path proven | PROVEN | CryptoAPI/`PROV_RSA_FULL` normal path |
| Fail-closed NSS policy implemented experimentally | PROVEN | exact source rebuilt and signed with VC6 |
| Forced secure RNG failure injected | PROVEN | test-only NT4 variant |
| TLS fails after forced RNG failure | PROVEN | `NSS_Init` fatal before handshake |
| Weak environmental fallback not reached | PROVEN | forced path returned zero; no TLS/payload |
| No custom RNG | PROVEN | failure-only policy delta |
| No custom crypto | PROVEN | no algorithm change |
| Fail-closed patch small/auditable | PROVEN | policy-only `RNG_SystemRNG` delta |
| Production code untouched | PROVEN | all artifacts under ignored spike storage |

R3A and R3B together close the backend feasibility investigation. `READY`
means technically viable as a legacy TLS backend; it does not mean production
integration exists. Future integration belongs behind the independent
`PapinhoSecureTransport` boundary, with opaque backend handles and no NSS/NSPR
types in its public API. Phase 3.B remains not started.

```text
3.A2B-R3A READY
3.A2B-R3B READY
3.A2B-R3  READY
3.A2B     READY
RETROZILLA NSS LEGACY TLS BACKEND READY
```

## Original R3 prerequisite decision (historical)

The remainder of this report records the state before the R3A continuation.
Statements below that no credentialed harness or disposable PKI existed are
historical; the modern-host results are superseded by the R3A matrix above.
NT4 and entropy conclusions remain current.

The exact RetroZilla lineage remains a credible candidate, but 3.A2B cannot
close. No Windows NT 4.0 runtime or VM was accessible, and the RetroZilla
distribution contains runtime DLLs but no standalone `tstclnt`, `selfserv`,
`certutil`, `pk12util`, matching import libraries or prepared certificate DB.
At that checkpoint none of the mandatory credentialed TLS runtime gates could
honestly be marked proven.

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

## Smallest remaining mandatory proof set (historical R3 prerequisite)

The following list records the gates as they stood before R3A/R3B. The final
closeout above satisfied them and supersedes the status text in this section.

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
