# Initial-bootstrap scratch correction: proof and handoff

Task PACC-PHASE6-INITIAL-BOOTSTRAP-SCRATCH-CORRECTION-1;2026-10-22.
Active main/HEAD913c90cf2328f7939d8f5b927640601903ac6e91 preserved.
This resolves the specific implementation nonconformance identified by the prior
factual-source investigation. Original proposal and historical report are unchanged.
No accepted contract or architecture changed. Official executor approved exact
operations.json/preimages/postimages before patch application; the test-only header
ordering/name correction has its own approved manifest. Both reviewed stages remain.

## Small production fix

The bootstrap commit binding now owns expected_body[1024] on its synchronous
stack. It passes sizeof(expected_body) to the existing BODY05 encoder and memcmp.
It no longer uses the512-byte pending_commit.observed for1024-byte work. The extra
1024 bytes are explicitly added to the existing minimum caller reservation.

No existing root/packet buffer grows; observed remains512 and native CAS still
uses min(remaining,sizeof(observed)) for ReadFile/memcmp. No quotas, formats,
hash domains, worker/owner/pin/retention or publication behavior changed. The new
scratch is used only by synchronous codec/compare; no pointer escapes to the
native begin or worker. Bootstrap source/authority/Stop checks remain intact.

Source search covers all product uses of pending_commit.observed / p->observed:
the remaining consumer is the chunk-bounded full-file CAS comparator. No second
1024-byte reuse of observed remains. BODY05->BODY04 and store04 wrappers still
operate under their1024-byte output-span contracts; this scoped review is not a
claim to validate every arbitrary external pointer in the entire repository.

## Tests and their exact authority boundary

MSVC Selected Debug/Release each passes8 relevant suites, Legacy Debug/Release
each6; all final builds emit zero warnings under /W4 /WX. The first test build
found WinSock header ordering and a shadowed fixture global; both were corrected
in test composition, without warning suppression or production semantics change.
Original diagnostic logs remain; only final runs count as qualified.

The new isolated test reuses existing root8 DATA builders and REAL BODY05/SHA
codecs. CMake extracts the EXACT current final production function definition
into a generated include, checks its boundaries and reconfigures on source edits.
There is no manually copied/forked implementation or test hook in product code.
Generated-body hashes for all4 configurations match source exactly. Test-only
preprocessor aliases replace resolver/Stop/native begin dependencies; the begin
trap ALWAYS returns NOT_SUPPORTED and never creates a worker/file/container.
This is positive buffer-path qualification with MODEL authority dependencies,
NOT a successful administrative bootstrap or evidence of factual authority.

The tests exercise:

- Valid BODY05 encode/decode at exactly1024; encoded bytes equal the existing
  independently built envelope payload.16-byte canaries on both sides remain.
- Encode capacities512/1023/1025 reject without changing the guarded output;
  decode lengths512/1023/1025 reject. No oversized/truncated span becomes success.
- The exact function body passes the payload comparison on valid input and
  reaches only the always-denying begin trap. A snapshot of the ENTIRE pending
  packet, including observed, adjacent path and identity metadata, stays equal.
- Mismatched payload, image below minimum/above maximum, insufficient reservation,
  model Stop, stopped real hash context and absent model resolver all deny before
  begin. The pending packet remains equal and no useful publication occurs.
- An additional call uses the ACTUAL public binding/resolver with an unqualified
  fixture owner; it denies without active worker/published state. No authority is
  bypassed in the product. Fixture-only Stop reset clears no real retained resource.
- Existing native guard and BCrypt ownership/failed-cleanup/retention regressions
  remain passing, as do prepared DATA and fixed-layout budget tests. Controlled
  failure models are distinguished from actual native cleanup failures.

No real bootstrap/native namespace/commit operation, certificate generation,
CURRENT publication, sockets, network, clock change or productive source ran.
These local Windows codec/negative guards do not qualify a positive ceremony.

## ED11 and readiness

No fixed layout growth: known Selected1956448, Legacy1790952 unchanged.
Local source scratch1024 is charged by minimum reservation+1024, not omitted or
claimed free through reuse. Actual Selected canonical object prologues allocate
3496 bytes Debug and3368 Release (sub rsp plus pushes). Exact object hashes and
disassembly are adjacent. These values include the local function frame, exclude
entry/caller/nested/native/CRT/OS costs and are NOT nested peak proofs. No old
compiled-frame delta is inferred merely from array size.

Ownership is synchronous through encode/compare only; Stop/error discards useful
work and does not release old native references. Existing64KiB worker/checkpoint
reserves are not increased or double-counted. Full native/source/CRT/stack/host
coexistence remains unqualified: ED11 total PARTIAL and operational DENY.

Scratch readiness is now PASS_SCOPED. Remaining initial-bootstrap prerequisites
are the real reviewed qualification entry without diagnostic bypass, designated
authorized operator and new target/custody, factual participant/material-loader
references/policy and their lifetime/reservations. Bootstrap requires no prior
CURRENT. UPDATE, publication, issuer/source qualification and signing remain
separate gates. No ceremony is authorized by this correction or its tests.
