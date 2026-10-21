# ED08 OpenSSL x64: factual C3 probe closeout

Date: 2026-10-12. Scope: isolated test harness, PST 0.6.5 API 3.0.0 / SPI 4.0 normal package. C2 specification 0.3 and supplement 0.3.2 remain proposals. No production C2/C3 implementation or wire approval.

## Evidence and result

The [machine-readable evidence](pacc-c2-ed08-openssl-c3-factual-evidence-20261012.json) records 28 passing executions: seven cases on normal Debug/Release and Combined Debug/Release. Cases are peer-close with pending external output, locally initiated close, active wait-set release precondition, cancel with escaped partial prefix, premature EOF, reset, and connection isolation. Combined AUTOMATIC selection is checked through the public provider query: both endpoints bind OpenSSL, with two registered providers. It is not inferred from package name.

The standalone consumer build uses documented MSVC 19.51 x64, /MD and /W4 /WX. All four builds pass with zero compiler/linker warnings. It consumes package libraries without rebuilding PST. Existing qualified suites were not rerun.

## Causal boundaries

The harness establishes real TLS 1.3 and mutual certificate authentication, then holds the external C3 reader until an actual public PST application-write operation reports NEED_WRITE and interest includes WRITE. This is application-write backpressure, not a claim of shutdown NEED_WRITE. Resuming the same outstanding write produces exactly the accepted plaintext at the peer.

For the peer-close case, public CLOSED with clean provenance precedes reciprocal COMPLETE. Actual bytes remain readable outside PST after COMPLETE. BA admission freezes with its already-admitted prefix accounted for; the proposed BA FENCE/ACK phase completes before public release. Membership is removed before release eligibility. Release result and disposition are distinct trace fields. DESTROYED precedes actual external ordered EOF; the bridge remains alive until that EOF and exact AB prefix acceptance. COMPLETE, DESTROYED and OUTPUT_DRAINED are separate facts. Local-close uses the corresponding public reciprocal flow without asserting that output must still be pending after COMPLETE.

Counters are scoped by operation ID, generation and client/server lifetime. BA received/accepted and AB received/native-site accepted prefixes must match; rolling byte fingerprints provide additional consistency checks. They are not cryptographic commitments. Native send acceptance does not establish remote TCP delivery. The site fixture's public TLS peer observation is an additional fact, not a replacement for Browser C1 production evidence.

The precondition probe observes INVALID_STATE with PRECONDITION_RETAINED while membership is active, preserves the Connection, then removes membership and observes DESTROYED. No release attempt is interpreted as destruction.

## Candidate fences and negatives

The harness transports candidate 48-byte frames through a private fixture socket, including fragmented receiver assembly and APPLICATION/RECORD in both directions. This exercises candidate layouts and phase guards; it does not exercise production PACC Session/grant authorization, CONTROL queues or Browser C1. BA seal authority is frozen producer plus exact accepted prefix. AB seal authority is ordered external EOF plus exact prefix accepted by the site-emulation socket. FENCE_ACK is downstream acceptance of that prefix, not Internet delivery.

Negative checks reject premature producer completion, premature downstream ACK, stale generation, cross-operation identity, prefix mismatch, cancel, and a late actual fixture byte after sealing without dereferencing a destroyed PST Connection. Cancel demonstrates received prefix 2 / native accepted prefix 1, preserving the escaped byte and rejecting further forwarding. This is a partial stream prefix; syscall-short-write counts are not qualified. EOF/reset before close_notify must produce nonclean/truncated failure. A second Connection sharing the runtime remains unaffected by first-operation cancel.

## Model E and limitations

Normal OpenSSL DESTROYED and PRECONDITION_RETAINED are factually observed. Normal OpenSSL CLOSE_UNRESOLVED_RETAINED is NOT EXECUTED: no governed safe failure mechanism was available. The source includes an explicitly synthetic consumer guard oracle; it is auxiliary and contributes no native PST retention qualification.

The recovered NSS test-only native retention package is separately preserved. Its ZIP, library, executable, canonical patch, manifest and internal checksums are verified in the JSON. It proves historical NSS x86 native failure/retention, not OpenSSL retention, output drain, or a future corrected ssl3.dll. No historical native test was rerun or substituted.

The [reconstructed NSS U08 audit](pst-nss-u08-source-reconstruction-20261012.json) confirms that PR_Shutdown success does not prove submission of close_notify ciphertext. No normal PST or third-party NSS source was changed. A safe fix needs explicit governed third-party patch/rebuild authority, pending-ciphertext drain and retry semantics, followed by affected ownership/retention gates on the new DLL. Merely checking SSL3_SendAlert's result is insufficient.

## Qualification boundary

ED08_OPENSSL_STATUS=PASS_FOR_SCOPED_NORMAL_NATIVE_PREFIX_EOF_PROBES; FULL_GATE_INCOMPLETE
C3_PREFIX_ACCOUNTING=PASS_BY_OPERATION_DIRECTION_GENERATION
C3_ORDERED_EOF=PASS
BA_SEAL_AUTHORITY=TEST_ONLY_CANDIDATE_PHASE_PASS
AB_SEAL_AUTHORITY=ORDERED_EXTERNAL_EOF_AND_EXACT_NATIVE_ACCEPTED_PREFIX_PASS
MODEL_E_RETENTION=PRECONDITION_PASS; OPENSSL_CLOSE_UNRESOLVED_NOT_EXECUTED; NSS_HISTORICAL_SEPARATE
WIRE_READY_FOR_APPROVAL=NO

ED11 total resource measurements/admission remain open. No runtime/global C2 qualification is declared from these probes. E1-E12 were not executed. Production pin remains 0.6.4. PST, functional Accelerator, Browser and production PACC wire remain unchanged.

NEXT: obtain specific upstream NSS third-party correction authority and governed corrected DLL; revalidate impacted native retention and shutdown/drain tests. Obtain a governed OpenSSL R4 failure mechanism if that target's complete retention matrix is required. Complete real Browser/C3/PACC integration and ED11 measurement before wire/runtime qualification.
