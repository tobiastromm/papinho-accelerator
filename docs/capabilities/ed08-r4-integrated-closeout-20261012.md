# ED08 / OpenSSL R4 integrated component qualification

2026-10-12. Controlled provider-boundary class only. No native Winsock failure claim; no production C2 implementation or runtime qualification.

## Authority and package-only composition

PST branch homologacao/pst-0.6.5 / HEAD 4dbd2783ebaf135114da2b7247e36c0ad6bc550a confirmed. R4 ZIP F897F162DB9B52D38BA97EEFE44540B5D724C6017D428F6561A834BAD6BF2F17, library 1450C01C2C1132566CDAC2A64BE8E84B72478800BA164207B7C7F592CAE9DED5, upstream handoff E10ABACAFF49BC2A9DD30C654D18220B30EE4E5B9D4B499021AC8A807344D863 verified. All internal checksums verified before and after isolated staging.

The package oracle-authority.md withdraws the earlier unlocated native-Winsock-only R4 assertion and accepts the minimum governed provider-boundary class. It preserves real-native failure as a separate obligation, consistent with PST ADR-0008/0009 and the consumer matrix. Earlier assessment/report remain historical; this evidence supersedes their unavailable-mechanism status for the controlled class only.

Normal current-commit OpenSSL ZIP 79E53A2D47AD6D509EC893BD1C6D9B8C26B98C5A8A9ECD75E4ADB33A240B132F supplies verified public headers and OpenSSL import libraries missing from the R4 archive. The R4 target links only TEST_ONLY_R4 PST static library; the separate normal target links only normal PST. OpenSSL runtime DLLs match both packages byte-exactly. No PST source rebuild. Normal library symbol audit finds no private R4/NEED_WRITE symbols.

ADR-0033 exists only in the governed isolated API3 Accelerator source, not main. Byte-exact Connection/manager, transport, processor, adapter and scheduler sources from that tree are copied under tests/diagnostic/ed08_r4_probe/consumer, with origin/full hashes. No accepted ADR is transplanted or edited and no functional main-tree component is changed. ADR-0022/0029/0032 remain binding. C2 spec0.3/supplement0.3.2 stay proposed.

## Executed evidence

Twenty final executions PASS: Debug and Release each run R4, gate-OFF positive/precondition controls, and seven current-normal ED08 cases. Final consumer builds MSVC19.51 x64 /MD /W4 /WX PASS, zero warnings. The new current normal artifact justifies focal regression execution; original 28 traces remain unchanged and were hash-verified without rerun.

The [machine-readable matrix](ed08-r4-integrated-evidence-20261012.json) records each run ID, operation/generation, executable and trace hashes, source provenance, corrections, package identities and negatives. Initial fixture missing header/source includes caused ordinary C1083/LNK2019 errors; complete byte-exact endpoint dependencies resolved them. Initial custom adapter probe is diagnostic history only; final qualification uses real processor/adapter/scheduler release logic with synthetic consumer hooks disabled.

## Causal sequence and retained outcome

Real TLS1.3/mTLS handshake, application-write backpressure and exact plaintext delivery precede public peer CLOSED and reciprocal COMPLETE. COMPLETE still has external ciphertext pending. Candidate fixture BA prefix/FENCE/ACK completes before release eligibility. The fixture adopts the factually established PST connection into the real consumer processor and moves the same membership into its scheduler. Real adapter PRE_CLOSE confirms scheduler removal; real manager invokes real adapter/processor release. The observation wrapper records attempt separately from public result/disposition.

Both R4 runs observe RESOURCE_FAILURE=6, CLOSE_UNRESOLVED_RETAINED=3 and zero/invalid native fields. Real processor enters RETAINED_TERMINAL; adapter remains open with its processor/reference, manager preserves the same Connection/context/count and denies slot reuse. Runtime, credentials, trust, C3 peers and scheduler remain reachable in a static durable fixture owner until explicit process containment. Private query after all negatives reports one provider-close attempt. Canonical provider patch branches before closesocket, so the controlled lane makes no target native call; the count is not presented as a measured Winsock-failure counter. Query survival proves retained backend reachability; internal provider/transport preservation also relies on the verified Model E implementation/contract, not invented destruction trace events.

Repeated manager Stop/remove and processor release do not invoke another PST release. A separately labeled direct public repeated-release negative tests core containment; it is not consumer retry policy. Raw PST read/write/shutdown/wait/rebind and real adapter read/write reject retention. Wrong token, stale generation, duplicate association, non-target arm and late arm are actually executed. The independent server reaches DESTROYED without changing the target's retained disposition.

R4 is non-clean. No ordered target C3 EOF, AB seal/FENCE/ACK, false OUTPUT_DRAINED or clean terminal is recorded. Retained C3 peer remains open. BA acceptance and COMPLETE are never converted into AB drain. Candidate FENCE validation rejects retained/cancelled authority. A late callback reaches the isolated fixture publication boundary: one attempt, zero delivery. This is a fixture negative, not execution of production Session/grant/queue/G1 publication machinery.

## Qualification boundary and remaining work

ACCELERATOR_R4=PASS_FOR_GOVERNED_ISOLATED_COMPONENT_INTEGRATION
RESOURCE_FAILURE=PASS
CLOSE_UNRESOLVED_RETAINED=PASS
OWNER_RETENTION=PASS_SCOPED_DURABLE_FIXTURE_AND_REAL_MANAGER_ADAPTER_PROCESSOR
QUERY_ONLY=PASS
NO_SECOND_CLOSE=PASS_PROVIDER_BOUNDARY; NATIVE_FAILURE_NOT_PROVEN
ED08_R4_NON_CLEAN=PASS
ED08_CLEAN_REGRESSION=PASS_CURRENT_NORMAL_AND_GATE_OFF
ED08_STATUS=SCOPED_PROBES_PASS; FULL_PRODUCTION_INTEGRATION_OPEN
NEGATIVES=PASS_SCOPED_INTEGRATED_MATRIX; PRODUCTION_PUBLICATION_NOT_EXECUTED
DEBUG_BUILD=PASS
RELEASE_BUILD=PASS
WARNINGS=0
ED11_STATUS=PRESERVED_MANAGED_QUALIFIED_TOTAL_OPEN
C2_WIRE_READY_FOR_APPROVAL=NO

This closes the controlled-provider R4 component lane. It does not certify real Winsock failure, full server owner/Session publication, Browser C1 native drain, production C2/C3, E1-E12 or the whole ED08 runtime gate. No new R4 failure recipe is needed for this class. Final wire approval and subsequent authorized implementation/integration gates remain separate.

PRODUCTION_PIN=0.6.4
PST065_PROMOTED=NO
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
FIRST_UNRESOLVED_DIVERGENCE=FULL_PRODUCTION_OWNER_AND_PUBLICATION_INTEGRATION_NOT_IMPLEMENTED_IN_THIS_PROBE
NEXT=OWNER_REVIEW_OF_SCOPED_R4_EVIDENCE; SEPARATE_ED11_AND_C2_FINAL_GATE
