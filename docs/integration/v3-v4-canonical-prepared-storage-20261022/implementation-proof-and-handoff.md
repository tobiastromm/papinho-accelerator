# V3/V4 canonical prepared-storage: implementation proof and handoff

Task PACC-PHASE6-V3-V4-CANONICAL-PREPARED-STORAGE-1; 2026-10-22.
main / HEAD913c90cf2328f7939d8f5b927640601903ac6e91 preserved.
The owner-authorized DATA integration is implemented. Native v4 effect and factual
source qualification remain denied. No CURRENT was created or published.

## Decision, exact review and preserved source

The explicit owner direction is recorded before implementation in
../../adr/DECISION-canonical-prepared-storage-data-20261022.md.
The original backend candidate and all prior accepted contracts remain unchanged.
Official executor review approved decision-ops, patch1/2/3/4/5 and the two test
correction manifests before each corresponding mutation. Pre/post SHA256 and exact
reviewed postimages are retained. Historic TEMP paths in manifests resolve by
basename under reviewed-files for durable verification. The patch files record
scoped changes against the active WIP, rather than attributing all Git differences
to this task. Baseline contains6598 existing nonignored files.

The SAME Accelerator-owned BearSSL0.6 source files from the qualified LAB are
compiled in private target papacc_storage_sha256. No arithmetic/vendor/header file
was changed or copied from another product. Original MIT/provenance/hash evidence
continues in tools/qualification/storage-hash-capsule/provenance.json and the LAB
handoff. No TLS library or build/runtime Browser/PST checkout dependency was added.

## Exact call boundary

- store03_hash now calls store_data_hash, using the SAME PAPACC_STORE_HASH owner
  for active/Stop/old-reference guards and a synchronous local capsule context.
- storage_digest handles the480-byte digest only at BODY03/04/05 storage sites.
  Historical digest/record_valid and standalone store02_hash remain native.
- papacc_configuration_storage_hash is a separate owner-thread guarded math port.
  Backing capture/reborrow, public canonical-SPKI digest, candidate/predecessor
  binding and kind8 signer DATA receive this callback.
- Pending storage link/ROOT08/checkpoint/preimage math uses the storage callback.
  Generic issuer codecs retain their algorithms and receive it only at explicit
  storage DATA sites. storage_carrier_inspect returns epoch/proofs0.
- papacc_configuration_issuer_hash06, its issuer binding/inspection/stage paths,
  signing/certificate/status/revocation APIs and native root administrative
  callbacks remain unchanged. No global issuer replacement or silent fallback.

The new bridge bounds spans and rejects output/context overlap, preserves staging,
checks Stop between1024-byte updates and finalization, and relinquishes active
only after synchronous quiescence. It never clears old algorithm/hash references,
retained state or native error history. Existing retained references deny reuse.
Actual retained BCrypt handles remain unchanged in the owner regression.

The product math archive has no BCrypt/CryptHashCertificate2/EVP/PST/Browser or
allocator symbol. Product builds retain GS and Debug RTC instrumentation: their
explicit runtime symbols are recorded, NOT silently priced zero. The old LAB
no-CRT/no-import binary remains a different, fully identified qualification scope.
Native certificate verification still has its existing crypto/CRT dependencies;
the whole root/worker is not claimed provider-free.

## One owner, explicit versions and candidate lifetime

The canonical launcher hold entry explicitly dispatches version3 or4 while keeping
its real CURRENT/source/dormant guards. Existing v3 API remains version3-only;
private v4 hold uses the SAME implementation/cohort/slots. V4 accepts a protected
v3/v4 predecessor; no downgrade from4 to3. CP3/CP4 correlation, full committed
preimage, root/revision/A/P/I/issuer/history correlation are preserved.

Selected committed/previous/next capacity becomes31840 each. Wire/codec offsets,
versions, domains and historical lengths do not change. Legacy stays16896.
Kind8 header accepts3/4 without changing PDG semantics; published v3/v4 use the
same canonical pin table. Kind9 remains the independent strict point DATA adapter.

Candidate backing serial staysZERO. A synchronous prepared_storage_validate_data
window uses the existing cohort invocation, candidate slot serial and retained
predecessor pin. It rehashes candidate/CA and the pinned predecessor under the
existing lock, validates the versioned codec, then rechecks lifetime/integrity.
A candidate view is available only in that active window; no published pin or
lease is fabricated. Inputs/arenas remain borrowed through return, never stored
by the capsule. Ports are synchronous/nonreentrant and may not retain pointers.

Abort while the DATA window is active has no effect. The launcher also refuses
source unborrow during that window; native act capture/revalidation cannot start
there. Stop invalidates useful work but does not release backing bytes. The window
ends after synchronous return; later abort still proves existing slot/pin/child
quiescence and respects retained/UNKNOWN state. No timeout or new pin manager.

DATA OK leaves phase HELD and status unresolved. Factual issuer/status/UTC source
resolution, UPDATE authorization and immediate pre-effect revalidation remain
independent deny gates. The canonical approve/validate/commit entry still returns
NOT_SUPPORTED without those sources and does not start packet11.

## Worker, history and qualification

No packet11 v4 format guard was opened. Native identity CAS and complete previous
byte comparison remain inside the transaction before WriteFile/CommitTransaction.
CAS UNKNOWN retains resources; it is not treated as rollback. The existing v3
native worker regression now asserts the explicit buffer capacity while testing
unchanged v3 serialized lengths. No new worker/journal/root/registry exists.

MSVC Selected Debug/Release each passes18 relevant regressions; Legacy each10,
with zero final emitted warnings. Actual VC6/C89 Debug/Release and MSVC capsule
builds each pass350879 checks. VC6 here qualifies the portable capsule, NOT the
modern Win32 owner, NT4, box or ISA. No such native compatibility is asserted.
Sixteen storage-length digests compare byte-for-byte against the existing issuer
SHA backend; the capsule also reruns fragmented/boundary/large/Stop vectors.

Prepared tests cover3->3,3->4,4->4, protected predecessor, candidate identity,
corruption, no borrow outside the window, blocked in-window abort, Stop and
quiescent release. Kind8 positive routing is explicitly MODEL (real same-owner
ranges and native public CA decoding); native certificate verification rejects the
existing synthetic3-byte certificate. Kind9 uses actual strict math. Neither is
factual delegation/TIME_SOURCE authority. Full native positive package trust is
not claimed or fabricated.

Each MSVC configuration also passes the real v3 worker success/conflict/UNKNOWN/
Stop cases and ROOT06 codec/pending-success/conflict/UNKNOWN cases in fresh TEMP
files, using existing synthetic administrative data. UNKNOWN is an injected
fixture outcome, not a claimed observed native commit uncertainty. No private
keys, real certificates, CURRENT, network or clock changes were generated.

Intermediate diagnostics are preserved: localized stale include objects required
a clean rebuild; one test include renamed main twice and was corrected; the v4
base test now uses the existing next4 builder; a preexisting malformed-kind9
expectation was stale (UNSUPPORTED instead of INVALID). Only final configurations
are counted. No validation predicate was relaxed to make a test pass.

## ED11 and next handoff

Measured known Selected1956448 bytes, +3744 for3*(31840-30592); fixed graph1533568.
Cohort data_active consumes former padding (sizeof remains304). Legacy1790952 is
unchanged. Three G7 slots120360, pins1024, slot metadata3456, issuer snapshots24576,
stage8192, existing worker65536/checkpoint8192 reserves are counted once.

The nominal Selected difference1189280 and Legacy306200 are NOT admission credit.
Local context128/staging32, caller context growth, nested product stack/GS/CRT,
code/data and retained native coexistence remain separately charged. Object
sections/symbols and exact census are adjacent; archive section sums are not a
loaded-process peak. LAB stack bounds cannot be transplanted across compiler
flags or enclosing call graphs. Complete native/provider/CRT/stack/host/source
terms remain UNKNOWN and ED11 total PARTIAL. No quota or capacity exception.

Next: qualify/implement the existing worker's v4 packet handoff only after its
remaining resource/factual gates; source B/C/D, native budgets and runtime/host
credits need their own evidence. No further DATA checksum can supply CURRENT,
authorization, qualified UTC or administrative status. Preserve DENY meanwhile.
