# LAB-only canonical storage SHA-256 capsule

This directory is isolated qualification code, not a product target, SDK, general
crypto service or worker integration. No top-level build/issuer/root code includes
or links it. Product integration requires a separate approved task and exact patch.

## Source ownership

The Accelerator-owned snapshot contains only the original BearSSL0.6 SHA-256/
shared SHA-224 translation unit, two original range-codec units, bearssl_hash.h,
and upstream MIT license/notices. provenance.json pins official release/tag/commit,
archive hashes and every original source. adapter_include/inner.h contains three
byte-exact upstream encoding-helper function regions with original notice, the
two range declarations and a portable BR_BE_UNALIGNED=0 selection. No SHA rounds,
constants, padding or bit-count algorithm were rewritten. No TLS source is compiled
or linked. The private API exposes SHA-256 only, despite upstream shared SHA224 code.

memory_bind.h routes only the required memory copies/fills to local bounded byte
loops; these are memory glue, not new crypto. Volatile stores prevent compiler CRT
replacement. VC6 compatibility supplies fixed-width types and inline spelling;
it disables only C4127 do/while(0) and C4514 discarded-inline diagnostics. Numeric,
conversion and arithmetic warnings remain enabled. No Browser/PST source or runtime
is referenced by the build. VC6 compiler helpers are supplied by the installed
licensed toolchain, not copied from another product or reimplemented.

## Private lifecycle and API

Caller zero-initializes and owns PACC_STORAGE_SHA256 and serializes its calls.
ACTIVE -> DONE, FAILED or CANCELLED is terminal for that computation. Reinit of
live or terminal state is refused; explicit caller reset is legal only after
its own factual synchronous quiescence. No retained native resource is freed.
All pointers must refer to live, correctly typed/aligned, caller-protected spans.
The adapter checks arithmetic range/overlap/size; it is not an OS address validator.
It does not accept NULL with positive length or context/input/output overlap.
Zero-length NULL input is valid. Input/output aliasing is not rejected when safe
because output is copied only after the entire input was consumed.

The optional Stop token is a borrowed const volatile unsigned pointer read during
this synchronous call, not an arbitrary callback. No token/borrow escapes. Polls
occur before work, after each at-most1024-byte update and before/after final math.
There is no clock/deadline or asynchronous cancellation guarantee. The owner must
serialize acceptance/publication and apply its native PAL Stop ordering; a token
change after the final observation is not a new authorization boundary. Cancelled
or failed hash state is cleared, status remains caller-owned, output is unchanged.
No buffers are released by timeout; no lease/CURRENT/status/trust is produced.

Streaming length is checked against the standard byte-aligned SHA256 maximum
2^61-1 bytes, including corrupted/over-limit count rejection without wrap.
No input-sized allocation or accumulation buffer exists. The math callback shape
can exercise storage-only spans but is not wired to store03/digest/issuer today.
Unused reserved context argument has no authority or callback semantics.

## Reproduction and evidence

See docs/integration/canonical-storage-hash-closure-lab-20261022/ for four exact
build scripts, hashes, compiler logs, PE maps/imports/disassembly, stack analysis,
40 public Python hashlib oracles and separate BCrypt reference result. Scripts
use the canonical source path and TEMP output directories; create their stated
TEMP working directory before rerunning if it no longer exists. MSVC19.51 x64
and actual VC6 x86 were executed, not substituted by a newer compiler for VC6.

closure.exe is a /NODEFAULTLIB no-CRT probe linked only to the capsule (plus the
two necessary static VC6 integer helpers), with PE imports/TLS directories zero.
The richer capsule_test.exe uses expected CRT printing/test harness services;
bcrypt_oracle.exe intentionally uses BCrypt in a separate process. Neither is
proof that a whole Windows process needs no loader/kernel/CRT resources.

The scripts use /GS- for the modern isolated fixed-frame no-CRT probe, not a
production hardening policy. Stack proof is for these exact compiler flags and
binaries and excludes caller/OS startup. No NT4, box or ISA runtime qualification
or full ED11 qualification follows. Product0.6.4, PST, Browser, wire and four
previous LAB directories are untouched.
