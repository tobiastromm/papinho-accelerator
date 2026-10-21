# ED08 real-owner publication authority: implementation prerequisite audit

2026-10-12. Audit slice complete; real-owner ED08 qualification NOT EXECUTED. The [evidence inventory](ed08-real-owner-publication-authority-audit-20261012.json) records current branch/HEAD, exact source/authority hashes and all 48 preserved trace identities. All traces and previous final build log hashes verify. No qualified test was rerun.

## Factual real owner

The initialized PAPACC_SERVER_SECURE_IO_WIN32 owner in apps/server/server_secure_io_win32.c enforces owner_thread_id for Stop, progresses phase-aware teardown and reconciles secure candidates. PAPACC_SERVER_IO_LOOP_WIN32 in apps/server/server_io_loop_win32.c owns protocol slots, network stream operation/generation, framed writer and pending publication. src/runtime/connection.c owns stable manager entries; src/security/pst_secure_transport_adapter.c owns processor/scheduler removal and release. These are actual source components, not the static fixture owner used by the preceding 20-run component qualification.

Main remains pinned to PST0.6.4: src/security/pst_secure_processor.c:288 uses void pst_connection_release. API3 Model E processor, adapter and secure owner exist in the governed isolated source. Its secure owner papacc_secure_model_e_check_retention preserves ERROR_TERMINAL and original cause/deadline when a candidate or outbound processor is RETAINED_TERMINAL. ADR-0033 applies to that isolated qualification; it is absent in main. Migrating main silently would not preserve this composition or the production pin.

## Existing publication boundaries

| Fact | Actual source boundary | Proof still needed for requested integration |
| --- | --- | --- |
| Terminal event origin/acceptance | accept_owner_terminal | A real originating callback from the target R4 operation |
| Authority before enqueue | begin_network_control_payload | Active real manager Connection, slot/op/gen and legitimate event |
| Queue ownership | network_control_connection_id/operation_id/generation | Correlation to R4 Connection and live CONTROL/DATA |
| Authority before submit | progress_network_control | Post-Stop/cancel dispatch reaches this boundary and returns explicit rejection |
| Attempt | owner_terminal_emit_publication before framed_writer_step | Distinct from rejected attempt before submit |
| Accepted bytes/frame complete | framed writer and PUBLICATION_SUCCESS | Actual recipient observation; frame complete alone is not remote delivery |
| Revocation | Connection CLOSING/CLOSED plus identity guards; CONTROL_AUTHORITY_REVOKED trace | Factual Stop/cancel order and target identity |
| Model E retention | Isolated processor/adapter/manager and secure owner | Governed package R4 executed in initialized server owner |

Source progress_network_control checks Connection state and Connection/op/gen snapshot before emitting PUBLICATION_ATTEMPT. Therefore zero such events cannot prove a rejected callback. Existing authority trace CONTROL_AUTHORITY_REVOKED can locate rejection when enabled; an observation of callback entry and rejected decision must remain distinct from a native submission attempt. The standalone hook test can allow an uninitialized loop with a missing manager under test-only flags; that route is explicitly unsuitable for declaring initialized real-owner integration. Existing hook/queue component tests remain historical evidence, not this gate.

## First missing implementation boundary

An exhaustive source search in main and the governed isolated apps/src/include finds no C2_FENCE/C2_FENCE_ACK/C2_FINAL handler, RECORD role queue or C2 operation owner. The only matching TLS_READY/log RECORD strings belong to existing Network Offload trace/logging, not the proposed C2 protocol. Existing NETWORK_CLOSE/ABORT frames are not substitutes for proposed C2 FENCE/ACK/FINAL.

Specification0.3 section6 requires four exact direction fences and distinct downstream facts. Supplement0.3.2 requires COMPLETE, BA prefix/ACK, eligible release, DESTROYED, ordered C3 EOF, AB prefix/ACK and separate owner finalization. The current clean proof implements those phases only in the isolated ED08 harness; no corresponding functional owner path exists to exercise. The preceding R4 proof observes real API3 components and public provider retention, but its publication guard is a fixture and cannot be upgraded by relabeling it.

Accepted conditional-semantic decision explicitly withholds definitive encoding and C2/C3 implementation. This task also forbids production C2/C3 implementation without approved contract. These sources do not contradict each other: they establish a missing authorized prerequisite. The complete requested C2 real-owner gate cannot be implemented or truthfully qualified in this slice. No accepted ADR was changed, no proposed wire was installed, and no absence of an event was counted as rejection.

## Concrete next execution plan

First approve the exact C2 contract/encoding and separately authorize implementation of the real operation owner, two roles, C3 bridge and fences. Preserve API3 qualification and the production0.6.4 pin as separate targets. If the owner instead requests only existing Network Offload real-owner R4 integration, specify that narrower gate explicitly; it can prove those owner/queue boundaries but cannot close C2 RECORD/fence qualification.

For the resulting real target, retain an initialized server owner and actual manager/slot/Session relationships. Use real PACC CONTROL/DATA peers and the governed R4 package in a distinct target, with opt-in exact Connection/token/generation control. Instrument callback entry, authority decision/reason, queue admission/dequeue, transport accepted bytes and peer receive separately without altering behavior. Arm R4 after factual handshake and required phase barriers, never return fabricated PST outcomes.

Execute before-Stop authorized delivery and queued-after-revocation rejection, stale generation, late callback, repeated Stop/cancel, DATA after terminal, duplicate terminal, pending queue/partial publication, actual publication failure, CONTROL close and independent Connection. Require real attempts even when rejected, byte-level recipient evidence, durable owner/lease retention, query-only state and unchanged single close-boundary count. The clean gate must execute the actual C2 phase machine; timeout/reset/retention/mismatched prefix cannot pass. Then run affected Debug/Release targets with zero warnings. No E1-E12 or production qualification is implied.

CURRENT_TASK_COMPLETED=NO
SLICE_COMPLETED=YES
PUBLICATION_ATTEMPT=NOT_EXECUTED_REAL_OWNER_R4
PUBLICATION_AUTHORIZED=NOT_EXECUTED
PUBLICATION_DELIVERED=NOT_EXECUTED
PUBLICATION_REJECTED=NOT_EXECUTED
R4_REAL_OWNER=NOT_EXECUTED
ED08_CLEAN_PATH=PRESERVED_SCOPED_PROBES; REAL_C2_OWNER_PATH_ABSENT
ED08_NON_CLEAN_PATH=PRESERVED_SCOPED_R4_COMPONENT_PASS
ED08_FULL_STATUS=NOT_QUALIFIED
ED11_STATUS=PRESERVED_MANAGED_QUALIFIED_TOTAL_OPEN
C2_WIRE_READY_FOR_APPROVAL=NO
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_PROMOTED=NO
FIRST_UNRESOLVED_DIVERGENCE=C2_REAL_OWNER_RECORD_FENCE_FINAL_PATH_NOT_IMPLEMENTED
NEXT=APPROVED_C2_CONTRACT_AND_SEPARATE_REAL_OWNER_IMPLEMENTATION_AUTHORITY
