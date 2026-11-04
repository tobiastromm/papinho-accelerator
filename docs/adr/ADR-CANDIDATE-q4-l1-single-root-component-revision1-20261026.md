---
status: proposed
revision: 1
date: 2026-10-26
scope-level: project
supersedes: []
---
# L1 bounded amendment candidate — one Q4 component in the sole root

This document is NOT ACCEPTED and grants no publication, root write, NV command, Q4 read, cleanup/refund or productive authority. It makes the exact schema/participant decision reserved by accepted L1 reviewable. W0–W7/L0–L7 accepted and the historical cross-boot decision remain controlling. Original ROOT8 v3/v4 bytes and validators remain unchanged.

## Decision proposed

Permit a new private root carrier version5 solely to carry one typed Q4 documentary component through the EXISTING canonical root owner, retained generations and exact-native-preimage CAS/worker. No sidecar authority, issuer event, generic journal, service, registry, IPC or new owner. Never reinterpret nonzero v4 reserved bytes as Q4. Existing v3/v4 codecs must still reject this representation as unsupported; absence of the component is INCOMPLETE for Q4, not an unused authorization or adoption permission.

## Exact component1 candidate

Exactly512 bytes. Offsets0..371 use the prior372-byte candidate record with production candidate tag PACQ4D01 instead of Q4DRAFT1. Offset0 magic8;8 version byte1;9 state byte (PREPARED1/CONSUMED2/RESULT3/UNKNOWN4);10..11 zero. Big-endian u64 at12 baseline,20 expected=baseline+1 (no overflow),28 PID (1..UINT32_MAX),36 creation FILETIME nonzero,44 first factual/model-error category. Bindings at52..339 are nine nonzero32-byte fields: root domain; exact predecessor whole-image digest; new approval digest; historical-identity manifest digest; authenticated witness/device-generation manifest; current host/boot/operator/token/live-invocation manifest; original objects/RAW1 manifest; scripts/dependencies manifest; custody designation manifest. SHA256 at340..371 covers0..339;372..511 must be zero. Digest fields reference complete frozen typed manifests and do not replace original handles, native File IDs, RAW1, trust or authority. Checksum is integrity DATA, not authentication or antirollback.

Error categories1..11 retain the previous candidate ordering IDENTITY/WITNESS_BASELINE/PREPARE_PERSIST/INCREMENT_UNKNOWN/COUNTER_DIVERGED/COMMIT_UNKNOWN/CALLBACK_UNKNOWN/LEASE/WITNESS_CURRENT/STOP/CLOSE_UNKNOWN;255 is uncategorized fixture failure. Production evidence must separately retain exact Windows/TPM first native status, API/stage and closure results; an error category cannot substitute for that receipt or turn UNKNOWN into CLOSED.

## Carrier5 placement proposed

Preserve existing v4 sections/order, insert exactly512 Q4 bytes immediately before its512-byte checkpoint, retain trailing whole-carrier SHA256. Version field8 becomes5. Header292..295 little-endian u32 Q4 offset;296..299 u32 length512;300..301 u16 component version1;302..303 u16 count1;304..511 zero. Existing offset56 points to shifted checkpoint; total/digest offsets12/64 advance512; other unchanged sections remain byte-identical. Candidate size interval21216..32352, fields maximum32320 (v4 bounds+512), NOT a quota/capacity grant. Version5 checkpoint tag PACR8CP5/version5 and link validation must use exact new carrier; old checkpoints remain validated by their original version. New private DATA discriminator10 can validate the component; it is not a wire message or authority namespace.

This is an exact extension proposal, not implemented in product. Header/checkpoint/source-link/native-worker code currently accepts only original versions/sizes. Activation requires implementing/testing the reviewed v5 codec, native identity/link/CAS discriminator and same-owner worker together; no generic permissive version check or bypass. Q4-only transitions preserve configuration, issuer/listener/security/temporal sources, A/P and operational CURRENT; only documented root lineage/Q4 facts change. No automatic migration/adoption. First component creation needs an exact separately approved documentary transition with a factual predecessor/witness baseline, not bootstrap by missing-file assumption.

## Protocol and ownership

Existing exclusive root/live pin owns the operation. Revalidate binding_current and original native predecessor/RAW1 at each boundary; snapshot equality is not CAS. Persist PREPARED -> validate authentic witness/profile/baseline -> ONE irreversible increment -> verify expected next -> persist CONSUMED -> final fresh live/objects guard -> one protected read -> RESULT or UNKNOWN. Known PREPARED/CONSUMED on a different invocation denies. Ambiguous store/increment/close retains first failure and dependencies; no repair, adoption, resend, refund or resumed execution. Restore detection is against an independently advanced SAME hardware generation; unchanged witness does not attest that no earlier pre-consumption preparation ever occurred. Known missing/inconsistent history never becomes virgin. Missing/redefined/untrusted witness denies.

Do not release original pin, mutex, store/TPM dependencies or uncertain handles to manufacture spare slots. Stop acceptance/quiescence/close remain factual. If existing slots, pending storage, memory, freshness or exclusivity cannot hold the required coexistence, STOP; no increase to128/1024, reservations, quotas or credit. Each instantiated carrier increases serialized size512; full owner/pending/retained/backing/stack/provider layout must be measured after implementation. This proposal does not assert a total512-byte RAM delta or approved budget.

## Credential custody backing (DATA only)

The512-byte component stores a custody manifest reference, not an inline secret or generic vault. A future dedicated DPAPI CurrentUser ciphertext backing may be owned by the SAME existing root participant under the already governed protected parent, with exclusive native creation/RAW1/native FileID/hash/size held in that manifest. It is secret DATA, never a ledger or independent authority. No concrete filename/native ID/blob/secret is fabricated here, and no protected object may be created without its own exact provisioning approval. The existing qualified protected-file creator must be reused; collision means STOP, not adoption/overwrite. Same-user/admin trust limitations remain explicit.

Binding entropy uses an exact reviewed purpose/version, root domain, approved device-generation/public-profile/index, custodian SID and credential version; entropy is public binding data, not secret strength. The root manifest binds the ciphertext's original object/descriptor and current auth profile. The secret never appears in approval JSON/argv/logs, and no issuer/CA key is reused as a password vault. Unavailable custody, profile loss, device replacement, Clear or ambiguous index generation denies; no automatic recovery/recreation. This authorizes only a future reviewed implementation design if accepted, NOT actual creation, secret generation, hierarchy access or TPM writing.

## Acceptance and gate

Owner must explicitly approve this exact amendment before dependent product-format implementation. After approval, code/test qualification is technical work; native TPM provisioning and eventual Q4 readback still require separate exact decisions/evidence. Prior528 root checks/46 challenge checks are preserved in scope, not converted into native persistence proof. The512-byte codec and worker fault tests in the associated package are isolated candidate evidence only.
