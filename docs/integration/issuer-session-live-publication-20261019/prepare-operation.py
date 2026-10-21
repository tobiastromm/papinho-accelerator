from pathlib import Path
import json,hashlib,difflib
r=Path.cwd();d=r/'docs/integration/issuer-session-live-publication-20261019';ops=[];diff=[]
for f in sorted((d/'proposed').iterdir()):
 p=f.name.replace('__','/');before=(d/'preimages'/f.name).read_bytes();after=f.read_bytes();assert (r/p).read_bytes()==before,p
 ops.append(dict(path=p,before=hashlib.sha256(before).hexdigest(),after=hashlib.sha256(after).hexdigest()))
 diff.extend(difflib.unified_diff(before.decode().replace("\r\n","\n").splitlines(True),after.decode().replace("\r\n","\n").splitlines(True),fromfile='a/'+p,tofile='b/'+p))
(d/'operation.json').write_text(json.dumps(ops,indent=2));(d/'proposed.patch').write_bytes(''.join(diff).encode())
proof='''# Exact reconciled operation â€” not yet applied

Authority: accepted issuer/C2 I/R/A/P relation P1â€“P4, P6.4/P7; C2 current epoch and negotiated grants; ADR0012; Model E/ED08/ED11. The historical 18 foundation hunks are not accepted or replayed. This operation is reconciled against source after the pending persistence adapter and stack deduplication.

## Function-level proof

- Native snapshot/pin_read: exact owner/ticket/index/serial/readers; conditional I presence; A and P separate. Reject alias of output/input/owner. No new native query or callbacks. Old bytes are data, never CURRENT.
- root_operational_reference_current: require actual selected root model, current source/leases, C2 P exactly equal captured P, full pure semantic operational projection, no Stop/unresolved/authority_lost/active worker. No ordinal/less-than generation fallback. Generic root CURRENT also requires actual selected R/index/A and no commit pending.
- New Session adoption: unchanged real negotiated RESULT/grants/Principal checks; acquire root CURRENT pin before exposing Session; capture its R/I/A/P and actual incarnation. No historical pin manufactures new authority. Failed acquisition denies, failed release retains Session/pin. No policy/grant expansion.
- Existing Session check: exact owned Session address/incarnation and its own admitted reference; actual CURRENT operational P/complete projection/source/leases checked separately. Original authenticated control/negotiation/capability/current-policy checks remain. A former R can protect memory while P remains genuinely current; no stale P accepted.
- Site template/listener: exact Session's reference or existing protected listener reference plus actual CURRENT operational projection. Original strict P, Site TLS profile, trust, revision/time, mTLS/Principal/grants and admission checks remain. Profile/Principal/policy changes deny this issuer-only path.
- Issuer prepare: same root/owner, durable participants/source, bounded reservation, actual CURRENT/P, full operational equality and image provenance. No caller epoch. CAS expected R/high-water guards remain. Flags bind one staged operation to R/P.
- Issuer publish: begin existing worker once, poll existing worker on later owner calls, including Stop completion. No synchronous commit or additional worker. Only factual CAS_SUCCESS plus post-completion revalidation can call root selection/A emitter. Whole selection/model acknowledgement protected by existing lock with no callbacks. A generated after commit; P preserved from factual current operational slot; no grants rewritten. Conflict/UNKNOWN never fall back. Terminal failures cannot restart commit; publication attempt is latched to prevent blind retry after postcommit failure.
- Reaper: every original guard precedes unpin, and exact private helper independently repeats absence of native Session/Connection, operations (even EMPTY refs), writes/denials, channels, processors and matching candidate/PST/security/adapter dependencies. Typed A/P, original R/domain/incarnation/ticket checked. No CURRENT requirement for destruction-only unpin. Failure returns before clearing manager/Session/pin. Exactly-once metadata clearing only after successful factual unpin. No native close, credit refund or alternate cleanup.

No checkpoint/fencing/high-water recovery source is fabricated. Normal PUBLISHED/REVOKED issuer record activation guard remains unchanged, as do signing proof bits and active-certificate-revocation gate. No Browser/PST/wire/network/CA/key change. Administrative metadata publication is not real credential issuance qualification.

## Proposed regression evidence

New normal-library application-root cases use real native root, actual initial P, TxF and existing runtime/wait-set attachment with no listener/Connection. REVOKED Session metadata is explicitly synthetic and nonauthorizing; not native authentication. Pin acquisition/selection and reaper execute actual product functions. Cover old/new revision references, immutable I snapshot, separate A/P, identical/divergent projection, stale R, active worker denial, duplicate begin, native file conflict and native pre-CAS access failure UNKNOWN, Stop before publication, three pinned slots, each borrower class blocking reaper, invalid pin, duplicate release and retained root. Existing pending adapter/core/storage/catalog tests retain native versus injected-fault distinction. Debug/Release fresh outputs + compiled censo are required after approval. No existing PASS is reused for new source.

ED11: per-slot tags and existing Session pin descriptors grow within same root, no new slots or worker. Stack and embedded packet charged once by existing reservation; quotas unchanged. Exact compiled census required. Provider/kernel UNKNOWN; all admission remains DENY.

## Exact files

'''
for e in ops:proof+=f"- `{e['path']}` before `{e['before']}` after `{e['after']}`.\n"
(d/'function-level-proof.md').write_text(proof);print('Exact operation',len(ops),'files',len(diff),'diff lines')
