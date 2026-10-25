# V4 packet11 in the canonical worker

Task PACC-PHASE6-V4-CANONICAL-PACKET11-WORKER-1,2026-10-22.
main/HEAD913c90cf2328f7939d8f5b927640601903ac6e91 preserved.
Official executor review approved operations.json and test-only identity/retention
operations before applying their exact hash-checked postimages. Original6716-file
WIP baseline/status and staged postimages are adjacent. No accepted contract was
changed; TS0-TS8 already defines v4 in the same root/worker. Latest owner authority
permits scoped native LAB tests while ED11 remains incomplete.

## Implemented boundary

pending_packet11_image_guard explicitly recognizes3/4, rejects all other versions,
and checks their respective image bounds. The v4 calculation includes the exact
TSC1 header/row size, count0..3 and insertion offsets before LST/carrier/CP4.
All guards are bounded before dependent reads. V3 calculation and serialized
lengths remain unchanged. Packet11 is still a PRIVATE discriminator, not a new
wire/persisted format. No codec/domain/offset definition was rewritten.

pending_packet11_valid accepts CP3 for a v3 predecessor and CP4 for v4. It verifies
checkpoint/prefix/image hashes, complete predecessor digest, root identity,
revision/predecessor/base/A/P/I/issuer linkage, and an absent candidate checkpoint.
It forbids downgrade. Frozen native slice preserves full carrier16384 bytes and
all existing SNT/PDG rows byte-for-byte. For TIME_SOURCE it also preserves row
count and all records. Thus3->4 here has zero temporal records;4->4 preserves
existing records. New or changed source/status/approval/issuer/delegation remains
outside this frozen slice until factual administrative resolution is qualified.

These are packet integrity/correlation checks on images already validated by
codecs/components, NOT an alternative structural/crypto/administrative validator.
The tests use the existing full v4 base-link validator before freezing images;
kind8 administrative/certificate positives remain explicitly modelled. No DATA
checksum, temporal key, fixture or authenticated token becomes authority.

## Native transaction and lifetime

Same pending_worker/CreateThread/TxF/CreateFileTransacted/identity CAS and full
ReadFile+memcmp predecessor comparison before WriteFile/CommitTransaction.
No new owner/root/worker/registry/journal, callback, buffers, pin or lease.
No root pointer reaches the worker. Same previous/next images remain embedded
and immutable across the child operation and retained outcomes.

Apart from renaming its packet-check call, the native worker body is unchanged.
Stop remains cooperative. Confirmed rollback is distinct from UNKNOWN. A signaled
thread allows its handle to close; unresolved file/transaction references are
still retained. UNKNOWN test explicitly requires unresolved/authority_lost,
non-null transaction and file, and no claimed commit. No retry, timeout release,
or UNKNOWN-as-rollback was introduced.

Only the existing compile-isolated PAPACC_ISSUER_STORAGE_COMPONENT_TEST fixture
entry can start packet11. It refuses even a synthetic published bit. Product
canonical hold/approve/validate/commit continues to require factual CURRENT,
issuer/status/UTC/administrative sources and to return denial without them.
There is no new production begin, CLI or positive source. Publication remains
separate and denied even after a LAB native success.

## Actual qualification

MSVC Debug/Release builds pass /W4 /WX with zero warnings. Each configuration
passes18 existing codec/prepared-storage/kind8/kind9/pin/ownership/ED11 suites.
Each also passes14 native fixture processes:5 modes (success, byte conflict,
wrong native identity, injected UNKNOWN, Stop) for3->4 and4->4, plus4 original
v3 modes. Therefore28 native runs total, including20 v4 and8 v3.

Existing maximum v3 fixture is30592 bytes;3->4 with empty source collection is
30688;4->4 preserves all3 source rows at31840. Same native worker persists exact
candidate bytes on success. Wrong-identity conflict leaves every target byte
unchanged; byte conflict leaves exactly the externally changed last byte, proving
candidate writes did not precede CAS. Stop with confirmed rollback preserves the
entire predecessor. Post-poll committed/previous/candidate/carrier buffers are
compared against original immutable inputs; publication still denies.

Each v4 process includes pre-start negatives: corrupted CP, divergent predecessor,
corrupt trailer, unknown version with resealed image digest, changed issuer
history with resealed digest and (for v4 base) changed temporal row with resealed
digest. They must reject before any thread starts. Rehashed bytes cannot grant
permission to discard history or change a source. Full-codec regressions provide
broader malformed/truncated/limit coverage; packet guard is not claimed to replace
those codecs. No new key/certificate or formerly rejected fixture was generated.

The TxF/file/identity effects are real on new isolated TEMP files. Administrative
images and source dispositions are synthetic. UNKNOWN is deliberately injected,
not evidence of an observed native commit uncertainty. Fixture process exit is
not a product resource-release rule. First native invocation had a CMD path
separator error and ran no executable; corrected exact scripts were separately
reviewed before effects. Final scripts/logs identify qualified runs.

## ED11 and remaining operational gate

Same Selected known1956448 bytes, fixed graph1533568; Legacy1790952 unchanged.
No type/capacity/slot/pin/worker-count growth. Three31840-byte images,120360 bytes
of backing slots, worker65536/checkpoint8192 reserves and512 compare scratch are
already counted, never charged twice or discounted by timeout.

The guard adds bounded scalar calculations, no allocation/provider/array. Local
source variables are not a measured compiled nested stack bound. Full native,
CRT/GS/RTC, host/concurrent/retained costs and factual issuer/status/UTC/operator
source graph remain UNKNOWN. No quota change, operational margin, target exception
or total ED11 PASS. Explicit owner LAB permission enables ONLY these isolated
no-authority tests; it does not qualify operational native resource admission.

Next is factual source and operational ED11 qualification, then a separately
reviewed productive producer/handoff/publication authorization. CURRENT,
signature, recovery, UTC and operational persistence remain denied meanwhile.
