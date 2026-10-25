# Integrated diagnostic resource ledger

Same native runtime owner, same service and one proposed attempt. Current complete native admission remains STOP; logical integration is implemented and independently tested. Historical UNKNOWN remains historical, boot boundary PASS retained, old volatile overlap excluded without zero/refund/cleanup.

| Resource | Measured/static fact | Charge/lifetime |
|---|---|---|
| PSRF_FILE |1120 bytes after bound/append-attempted flags; includes frame1024 | Nested in PS_DIAGNOSTIC, never add separately again |
| PS_DIAGNOSTIC |4400 bytes | Includes file1120, parent handle, SD buffer1024, WCHAR arrays512+600 (2224 bytes), outcome state/padding |
| PS_RUNTIME |12080 bytes, previously7680 | Includes complete diagnostic4400; no additional pool |
| PS_NATIVE |24904 bytes, previously20504 | Includes PS_RUNTIME; don't add12080 again |
| Service storage |25120 =24904+host96+trace120 | Existing storage_bytes function returns actual sizeof composition |
| Existing CLI atomic |4 bytes already separately reserved | Total named composition25124, prior20724; delta4400, not an allowance/quota increase |
| Diagnostic handles |One parent + one exclusive file, both pinned during psr_prepare | At most two new explicitly-owned refs; prep/capture close precedes runtime readiness on success; acquired failure retains actual refs |
| Descriptor/query |Caller-owned1024-byte SD buffer, bounded GetKernelObjectSecurity | No new application LocalAlloc/GetSecurityInfo/reallocation in binder. Too large/error => retained/deny, not growth |
| File |1024 immutable arm + one1024 append; max2048 own logical bytes | CREATE_NEW occurs only in separately approved preparer; runtime is OPEN_EXISTING-only. Cache/metadata/physical sectors not bounded by2048 |
| Formatting |Existing trace record/formatter, sink formats into owned frame payload960 | No second960-byte payload copy, no new CRT instance/worker/queue/receiver |
| Stack |Descriptor validator has four68-byte SID buffers plus fixed SID tails; binder has BY_HANDLE_FILE_INFORMATION52; finish has52; existing768-byte trace formatting remains live during callback | Lexical sizes are not compiler/native/CRT/Windows transitive peak; no full stack bound inferred from PE reserve |
| Stop/retention |No retry/forced close; diagnostic2 refs remain part of existing owner until factual close | Handler signals only; in-flight synchronous I/O is not cancelled by a timeout. Failed flush/close retains state and prevents STOPPED |
| SCM/token/runtime |Same pre-existing profile/owners/quotas | Full process/native/provider/kernel and simultaneous host capacity still independently unqualified |

Actual selected/legacy model outputs agree in Debug/Release: DIAGNOSTIC4400 RUNTIME12080 NATIVE24904 STORAGE25120. This is actual sizeof evidence and automatic reservation via the existing owner census, not manually resetting credit. Selected3145728/Legacy2097152 caps and all operation/session/queue quotas unchanged. Existing memory-census tests PASS; nominal arithmetic margin is not host admission.

Phase graph: administrative preparer creates/flushes/closes arm before Start. It retains its plan/native-file-ID evidence in the existing qualifier. Service binding holds diagnostic parent/file concurrently with psr_prepare resources; final append/flush/checked close releases both before ready. Runtime later owns its ordinary SCM/token/config/resources. On uncertainty, charge retained diagnostic and existing runtime resources together until factual quiescence; never drop the retained term on timeout. Qualifier/launcher may coexist with the service and cannot be charged as four parallel build configurations. Persistent files/history are not volatile-resource reuse.

The72 no-effect suite/configuration combinations retain process observations in JSON using owned child handles and CREATE_NO_WINDOW. Eight real file fixtures across two rounds remain in scratch, handles closed by their successful fixture receipts; fake integrated handles are model-only. Process working-set/pagefile high-water and sampled private/handle maxima include fixture/CRT baseline and are not actual service/receiver envelopes. No observed zero stands for invisible provider/kernel/shared allocation. Test-agent overhead is not omitted from whole-host reasoning.

First concrete native unknown: peak kernel/file-system/cache-manager charge through the local-file open/read/append/FlushFileBuffers/close path, including allocations not attributable by current process counters. Full transitive Win32/CRT stack and simultaneous host admission are also unproved. No extra diagnostic receiver, process or worker is necessary; do not add one to measure this away. The caller-owned SD buffer and in-place formatting reduce the explicit graph but do not manufacture an opaque kernel ceiling.

Smallest additional measurement: one separately reviewed no-service fixture on the SAME local volume, with the exact creation/SD and bind/read/append/flush/close sequence, original owned process/file handles, phase timestamps and factual close receipts. Collect process baseline/peak private/working-set/commit/handle/thread/stack evidence and supported process quota/native counters; collect kernel/cache/commit host snapshots with shared attribution explicitly marked. Never attribute a global delta solely to the sink or infer an upper bound from one idle sample. If required kernel attribution/bound remains unavailable, retain UNKNOWN and STOP; a native measurement/proof strategy requires separate exact review, not a guessed reserve or quota increase. Actual VSA/service-token/SCM/SS0 qualification remains a later manual operation, not supplied by this fixture.
