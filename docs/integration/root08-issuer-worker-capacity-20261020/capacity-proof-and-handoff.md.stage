# ROOT08/v2 issuer worker capacity — reconciliation and dependent DENY

Task PACC-PHASE6-ROOT08-ISSUER-WORKER-CAPACITY-1,2026-10-20. main / c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Initial baseline3884 files; previous carrier task manifest matches. No functional source changes, no functional patch submitted/applied. Accepted T0–T6, root/C8, issuer/C2, Model E/ED08/ED11 unchanged.

## Factual capacity

Fresh Debug/Release builds of the existing census agree: root182768, fixed graph1384376, commit105712, pending51096, committed16896, slots3456, pin tickets1024, issuer staging8192 and issuer snapshots24576. The pending packet embeds three16896-byte arrays (50688 bytes); its metadata is408 bytes. All children already belong to the containing graph; they are not added again. See raw compiled census logs and ed11-census.json. No operational working-set/commit/handle peak was measured.

Accepted ROOT08/v2 is20512..28960 bytes; carrier16384, SNT1 up to3168, CP2 already512 and LST1 up to2208 are INCLUDED in that outer size. They must not be charged a second time. Even the minimum valid v2 exceeds the current16896-byte image capacity. Four persisted/transaction images need attention: worker previous/next/observed AND root committed. Prior36192-byte widening arithmetic covered only the three worker arrays, not committed.

Current worker packet8 additionally admits version1 with no issuer only; increasing array lengths cannot qualify version2, carrier/history/CAS or authority dispatch. No version discriminator or format was altered. Root06 historical behavior and issuer bytes remain unchanged.

## Complete known scenario recalculation

The preserved conservative bound2084039 comes from the existing compiled graph/reservations and previous SELECTED-size allowance, not measured process RAM or approved full activation. It includes the old no-carrier codec prefix9376+output9408=18784. Its13113 margin is not a reserve.

For maximum v2 the standalone prefix28928+encoded output28960=57888 replaces that old pair: delta39104. This is a replacement, not charging the entire old+new pair as simultaneous. Keeping the prior conservative coexistence and four independently retained images yields:

2084039 + 4*(28960-16896) + (57888-18784) =2171399.

Known conditional excess74247 versus2097152, BEFORE new carrier/source context, caller workspace, additional complete callback/native stack, DER/CSR/CA backing, retained results and provider/kernel resources. It is not the measured current peak and was NOT allocated. All existing base reservations stay in place. Full future activation maximum remains UNKNOWN.

## Storage/lifetime matrix and alternatives

|Object|Actual consumer / lifetime|Conclusion|
|---|---|---|
|previous|Native worker exact full CAS comparison; checkpoint operations; bootstrap namespace witness/final path; retained UNKNOWN packet|Cannot remove or alias to mutable committed by capacity arithmetic alone.|
|next|Frozen before worker launch; native write and bootstrap verification; retained result/export until factual closure|Cannot overwrite after Stop/timeout/UNKNOWN or while exported consumer depends on it.|
|observed|Full native read comparison before CAS write, and bootstrap read-back after flush|Could be replaced by chunked comparison, but both entry points and failure/short-read/Stop semantics need exact proof/test. It is not currently a chunk buffer.|
|committed|Owner selected durable history, base comparison and subsequent transaction preparation|Full v2 cannot be stored in current16896. Cannot treat previous/slots as free storage.|
|issuer_snapshots / slots / pins|Immutable revisions for old Sessions and retained dependencies|Remain fully charged; no overwriting or reclaiming historical pins.|
|codec prefix/output|Caller data coexist with frozen packet under existing conservative preparation bound|Output may fit in next only after proving prepare/reset/validation ownership. Current begin clears the packet with memset before filling; arbitrary aliasing would destroy input.|
|worker stack65536 / owner scratch8192 / caller allowance131072|Existing separate reservations|No evidence that they are spare packet space. No automatic reuse or subtraction.|

A 512-byte observed chunk instead of16896 saves16384 relative to full widening. Applying it to the conditional graph above (and widening only previous/next/committed) yields2142951, excess45799. Thus this isolated optimization is insufficient; no patch was applied merely to make a partial arithmetic improvement.

Even an UNPROVED direct encode into next, eliminating the separate28960-byte output, gives2113991, excess16839. That scenario is not implemented/approved and does not prove byte lifetime or zeroing safety. Replacing previous with an immutable borrowed committed buffer might save further memory, but requires proof across every ROOT06/checkpoint/D3/bootstrap reset, poll, rollback, Stop, UNKNOWN and finish path. Bootstrap uses previous as mutable namespace/path storage; a blanket pointer substitution is not safe. No second pin/owner/registry or new borrowing architecture is proposed here.

The minimal *proven* change is therefore none: preserve capacity and DENY. This review does not claim that no possible future design can fit; it establishes that neither straightforward growth nor the examined single-buffer optimization fits the preserved known graph, and deeper sharing currently lacks lifetime evidence. Full managed/native activation credit is unavailable. Quota2097152 and legitimate reserves are unchanged.

## Validation and evidentiary boundary

Rebuilt current census, native D3 worker fixture and ROOT06 codec fixture in both configurations with the same MSVC19.51/Ninja/x64/C90 profile and normal PST0.6.5 SDK. Zero compiler warnings. Ten selected offline suites per configuration pass: previous nine issuer/LST1/ROOT8/v2 suites plus fresh memory census. Existing v2 tests cover issuer present/absent-denied, corruption, serial high-water, event-prefix/revocation preservation and model retention. No new v2 native worker path exists or is claimed.

Five local D3 modes per configuration pass: reject, success, conflict, unknown and stop. Native filesystem/TxF/CAS/handle observations are real; CURRENT/checkpoint/policy context and injected faults are synthetic. They preserve retained outcomes and do not publish operational CURRENT or prove global recovery. Separate ROOT06 --codec-only tests pass; no new root publication or real CA/certificate/signature occurred. Diagnostic fixture files and logs remain in native-fixtures; process exit is not claimed as DESTROYED. No network tests ran.

## Focal handoff

Before any capacity patch, prove one concrete shared-buffer lifetime across the existing begin/reset/native-read/write/poll/export/checkpoint/bootstrap/Stop/UNKNOWN/finish functions, price the full caller graph, and demonstrate it under2097152 without dropping serial, history, old snapshots or reserved credit. Then submit exact function hunks with preimages and both historical/native regressions. Do not proceed to TLS/X509 binding based on this census. Its factual CURRENT/pin/source lifetime and signature gates remain separate and denied. No roadmap, format extension, root, worker, journal or authority was created.
