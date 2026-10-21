# Browser handoff — minimal Mode C receipt correlation
Status: implementation handoff, NOT applied to Browser. No Browser write/build occurred.
Authority: PACC-PHASE6-MODE-C-MINIMAL-PAIR-RECEIPT-CORRELATION-1, section9.

Canonical Browser main HEAD 1eb028554f622672377953225e162ed9b6564a40.
Preimages to recheck before editing:
- src/papinho_c2_s6.h SHA256 5067B585EA582E4319DD4886C316636BA44F8E9110A3349F99AAA47CB1670486
- src/papinho_c2_s6.inc SHA256 7D5197B880604CA7ED46FE3071E97E34FFEB4B33084E12BED8E331B3FB645E52
If changed, reconcile the actual WIP; never replace with this baseline.

## Exact boundaries and data

1. Extend the existing papinho_c2_s6_facts receipt, not the owner/collector. Add explicitly present request, epoch, navigation/attempt and endpoint.resolution_generation copied from c->request, c->epoch, c->navigation, c->endpoint.resolution_generation. Existing operation/generation/prefixes remain authoritative local facts. Do not mint new IDs. Copy before context disposal on both success (currently lines369–370) and failure (currently line387). Absence before validated OPEN_RESULT must remain absent; zero is not authorization.
2. c2_accept_open_result (currently line151): only its successful validated return witnesses OPEN_RESULT receipt. Sending OPEN is not that fact. Match request/attempt/resolution and nonzero server tuple; never export bytes44..107 tokens.
3. c2_role_attach (currently line184): use existing successful c2_accept_bind_ack for role receipt. Record separate role1/2 references for actual fully written BIND and validated ACK, retaining local sequence/Connection context. Do not infer receipt from a write or alias local Session identity to server Session.
4. c2_handle TLS_READY: reference only successful validated 0x0304/mask7/selected http/1.1 receipt. Use actual selected ALPN, never the offered constant.
5. Reuse existing c->count[4], fences[4], fence_sent/seen, ack_seen/written and existing C2 first-cause/publication sequence. Preserve APP BA, APP AB, RECORD BA, RECORD AB order. An accepted prefix is not delivery; map the exact same count semantic on both sides. Missing boundary sequence stays missing; do not assign a new retrospective causal timestamp.
6. FINAL validation is remote summary only. Keep the existing local cleanup path at lines369–371 separate, with actual Session/Site/native-result/lease/borrower proofs. Existing facts->local_destroyed alone is an aggregate reference, not all individual destruction evidence. Export individual proof references from the existing registries; retain original charge on uncertainty. Do not change any refund routine.
7. Reuse existing build manifests and own-process/run provenance. No Principal16 is required from Browser if not factually known. No root epoch is issued locally. Preserve absence and report missing process-creation/image binding, ED11 metrics or producer rather than fabricate them.

## Offline consumer interface

Accelerator tools/offline/mode_c_pair.py provides consume_mode_c(browser_raw, accelerator_raw, artifacts) and historical_joint preserving the existing v4 joint verifier's behavior. The new function validates normalized PUBLIC receipt references, NOT arbitrary native logs and NOT their authenticity. Existing source adapters must export actual facts; no new collector is permitted. Unsupported/unavailable projection remains INCOMPLETE. No complete native projection or actual Browser artifact was exercised in this slice.

Each envelope is {format:"MODE_C_RECEIPT_REFERENCES",version:1,body,body_bytes,sha256}. body_bytes/hash cover exact canonical body (ASCII JSON, sorted keys, compact separators). Body exact keys: role,run,process,creation,image,sdk,build_ref,origin,join,refs,incomplete. Run IDs are preserved up to127 printable ASCII chars. Numeric IDs are exact checked Python integers up toU64, never bool/float; serialization from VC6 must retain full U64 bytes/limbs. join is null or {request,operation,generation,epoch,attempt,resolution}; attempt/resolution fitU32. image/sdk/build_ref reference actual supplied bytes by SHA256. Expected hashes are not native attestation. Local clock and Session IDs are not compared between processes.

refs are {artifact:<sha256>,sequence:<existing local sequence>,kind:<stage>}. Referenced artifact is a public normalized projection of the existing receipt, with exact fields role,run,process,creation,origin,join,facts. Each fact: {sequence,kind,action,values}; no token/payload. Actual source identity must equal its referencing receipt. References are strictly ordered by local sequence, no duplicates, max32. Artifact input is bounded256KiB, total16 entries/1MiB, no owner buffer allocation is implied.

Required stage vocabulary:
- OPEN_RESULT, ACK_APP, ACK_RECORD, TLS_READY, FINAL: Accelerator FULL_WRITE / Browser VALIDATED_RECEIVE.
- BIND_APP, BIND_RECORD: Browser FULL_WRITE / Accelerator VALIDATED_RECEIVE.
- PREFIXES, FENCES, ACKS, STOP, TEARDOWN: LOCAL_FACT.
Values: TLS_READY={version:772,verification:7,alpn:"http/1.1"}; PREFIXES/FENCES/ACKS are four exact counters; FINAL={outcome,ownership}; TEARDOWN={disposition,callbacks,leases}; other values null.
The supported complete case is one clean operation with matching final four-flow counters. Retained, non-clean, missing or incompatible evidence remains INCOMPLETE (not a claim that every non-clean trace is malformed). Contradictions, forged hashes, duplicate references and impossible causal order are INVALID. Capture must cover all required stages; a decodable file is insufficient. Repeated/partial events must be referenced by their actual complete boundary, not compressed into invented facts.

CORRELATED means matching REPORTED artifacts only. The consumer always returns native_qualified=false, ED11_PASS=false, admission=DENY, provenance=REPORTED_ARTIFACTS_NOT_AUTHENTICATED. It cannot authorize native execution. Synthetic fixtures explicitly remain SYNTHETIC. No cross-process host-credit reservation or admission publisher is added.

## Accelerator source counterpart

server_c2_owner_win32.h adds two56-byte public OPEN_RESULT mirrors to the existing collector; server_c2_owner_win32.c fills only at the existing FRAME_COMPLETE/GRANTED boundary. Slot reuse with a different operation poisons completeness instead of overwriting. server_c2_pair_receipt.c projects only public bytes0..43. server_c2_e10_observe.inc exposes papacc_server_c2_pair_copy only after existing seal, owner-thread checks, no active dispatch/overflow/reentrancy; aliasing the owner as output is rejected. Existing event-copy API supplies other sequence references; TLS_READY/FINAL mirrors remain unchanged. There is no automatic file I/O or positive producer enablement. The external existing export orchestrator must supply actual process/build provenance and normalized references; absent sources cannot produce native CORRELATED evidence.

## Required Browser validation, not executed here

Use governed VC6 Debug/Release normal SDK, isolated new outputs; zero warnings. Verify full IDs/absence, ACK-before-receipt denial, generation/epoch mismatch, partial write/WOULD_BLOCK, normal/retained Stop, FINAL without local destruction, duplicate source references and no credit mutation. Recompile actual Browser sizeof/scratch/copy/retained coexistence. No new quota, historical512MiB margin, ADR0052 extension, PST/wire change or network execution. CLIENT/SITE_TLS/ED11 gates remain DENY. PST065 publication waits for integral Phase6 closeout.