# Exact same-root checkpoint implementation review

Authority: accepted issuer/C2 P3 and P6, technical profile2.2-2.5, same root/C8, Model E, ED08/ED11; owner PACC-PHASE6-ISSUER-SAME-ROOT-CHECKPOINT-LINKAGE-1. No new normative authority, accepted ADR change or backup/fencing implementation. SOURCE operation.json defines exact preimages/postimages and proposed.patch. Changes are function-specific, not text replacement at execution time.

## Private encoding

ROOT06 first16384 bytes unchanged; optional512byte version1 publication checkpoint in SAME file, total16896. Historical8192/16384 accepted by their old readers only; new explicit recovery06 accepts16384/16896 and validates extension if present. No old free bytes used. Trailer magic PACICP01, BE32 version1 at8, exact length512 at12; root/domain32 at16; BE64 R48,predecessor56,A64,P72,I80,serial highwater88; authority16 at96,generation16 at112,prefix8 at128,snapshotR at136; SHA256 whole ROOT06 at144, issuerimage at176, prior commit CAS bytes at208; bytes240..479 reserved zero; SHA256 trailer bytes0..479 at480. No secret. SHA algorithms remain approved native adapter callbacks, not new crypto.

R/I/snapshot must match verified issuer history, predecessorR-1, A/P nonzero from actual published slot. Whole root/issuer hash and immutable original root hash bind reservations/revocations and file identity stored in BODY05. Prior commit digest is derived from actual successful worker packet before it is reused, never generated request ID. Digest integrity/correlation is NOT global freshness or authenticated anti-tamper protection.

## Lifecycle / native transaction

Explicit root administrative checkpoint begin requires full current root/model/P/projection, no active issuer transition; lower begin additionally requires native preceding commit packet SUCCESS+committed and exact next bytes, current slot I/snapshot and actual postcommit A/P. Before CURRENT/A publication begin denies. Encode on serialized owner; new bounded8192byte stack/scratch allowance charged, no active resource-query collector.

Same completed worker/packet reused after handles closed. It owns no root/Session pointer, only immutable before/after bytes. Second full-byte/file-ID TxF CAS appends extension; no root revision/epoch/policy/grant changes. Poll zero-wait after signal; actual close errors/unknown retain same original semantics. Stop never waits for worker; races may commit factual historical checkpoint, never issue authority. Active writer blocks competing publication; conflict/unknown latches authority loss, no fallback/retry.

Subsequent issuer commit compares entire16896 predecessor then atomically writes/truncates to16384, invalidating old publication link. New actual CURRENT/A can then receive a fresh checkpoint. Existing read pins/snapshots remain immutable; no extra slots. Actual disk byte count tracked separately from allocation capacity. Synchronous dormant compatibility path also truncates inside its native transaction.

Data-only recovery validates trailer/history and copies historical A/P solely to explicit unsupported-recovery data output. It does NOT populate live slot A/P, publish CURRENT, pass signing proof bits or resurrect Sessions. Same-root checkpoint is not latest-proof/fencing; AUTHORITATIVE_RECOVERY=DENY always.

## Tests / budget

Fresh Debug/Release /WX, existing regressions plus native same-file round trips R2/R3/R4 under old pins, real serial reservation, predecessor/successor mismatch, stale CAS/unknown/Stop, projected policy rejection, zero/corrupt epochs, short checkpoint, historical no-checkpoint files. Synthetic issuer/REVOKED Session metadata and faults remain explicit; no network/CA/signing. Same root committed and three worker buffers each grow512 (2048 total), actual-size/mode fields/alignment measured by compiled census; explicit owner scratch8192 charged once plus existing worker65536 once. No quota increase; reject if total>2097152. Provider/kernel UNKNOWN. Native close failure and cross-installation fencing not qualified.
