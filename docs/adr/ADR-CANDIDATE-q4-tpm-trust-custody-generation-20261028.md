---
status: proposed
revision: 1
date: 2026-10-28
scope-level: project
scope-target: Q4 TPM witness trust, custody and generation designation only
supersedes: []
---
# Candidate — Q4 trust/custody/generation designation

NOT ACCEPTED. No TPM call, NV write, secret creation, Q4 read, root publication, hierarchy/SCM/ACL change, cleanup/refund or productive authority follows. This proposal does not modify accepted W0-W7/L0-L7/L1/cross-boot. Each decision below is independent; absent a precise owner decision and factual prerequisites, DENY.

## TC0 — Existing facts and limits

EK PKCS1 SHA256406874753bd60db238226c888765b49940cc6f0229b8a0296f4ad1547e92aba2, canonical SPKI1c643066b17bdf9e81c09870661aeb7c9cfff1f2ffbef71b43532c14a25c474e, EK certificate965f56aa0d4c5b5772b423d65d8a06a623109f5828c0eddcb8ec4ae8fa3de761. Observed chain signatures Intel and native salted mathematical proof PASS do not alone designate a trusted device generation or custody. CRL device query403 remains UNKNOWN; no nonrevocation claim. Actual old native proof approval94f9 is consumed, never renewed by this proposal.

## TC1 — Trust-anchor decision A

Candidate sole anchor for this purpose: Intel EK root DER SHA2562e1b3ba79af56d758be51697621bc4b9e8cee0983db3e749c55eb9b37c6d2ae0, with exact reviewed chain/intermediate and pinned EK certificate/key. Recommend an explicit local-purpose designation; no OS-wide trust-store installation, AIA automatic trust, wildcard manufacturer allowlist or other certificate purpose. Preserve the immutable decision, chain bytes, verification time provenance and invalidation history. Root source URL alone is not designation. Missing/corrupt/changed chain, key, algorithms, constraints, certificate validity or applicable policy => DENY. Root self-signature is integrity, not independent justification for choosing it.

## TC2 — Revocation decision A, distinct from anchor

Recommended strict profile: UNKNOWN may remain recorded as mathematical evidence, but cannot satisfy Q4 witness admission or authorize NV. Obtain acceptable revocation evidence under an approved policy, or separately decide an exact bounded diagnostic-only exception. No timeout,403/offline failure or no OCSP turns into PASS.

Alternative requiring explicit owner acceptance: allow UNKNOWN only for ONE isolated index/persistence experiment under exact pinned EK/chain and declared current-host trust, without Q4 reading, hardware admission, durable production use, credits or closure. Such an exception changes the admissible diagnostic evidence, not the revocation status. Its duration/bindings and resource limits must be specified in an exact later request; none is fabricated here. This alternative does not make the non-restorable boundary or ownerAuth available. No generic revocation waiver is proposed. A decision merely accepting the anchor does not choose this exception.

## TC3 — Custody decision B

Proposed custodian SID is the already observed operator S-1-5-21-785699550-1440458426-1682014700-1108, ONLY if independently designated by the owner; observation/admin membership is not appointment. Original Windows token SID/elevation/no impersonation/LUID/lifetime checks remain mandatory. The canonical configuration owner/worker remains the only product participant and serializer, not this tooling.

Definition authority: a legitimate previously governed holder/provider of TPM_RH_OWNER authorization. Before any request can be executable, document the source, holder and controlled in-memory delivery without exposing the value in chat/files/argv/log/ENV. Windows-admin/TBS access, lockoutAuth, or this normative decision do not imply ownerAuth. If no legitimate ownerAuth exists, STOP; no extraction, empty/guessed attempt, Clear, reset, hierarchy or GPO change. Replacing this requirement needs a separate architectural proposal, not a fallback policy.

Increment/counter read: independently designated32-byte per-index credential under AUTHWRITE/AUTHREAD and authenticated protected sessions; exact root/device-generation/index/purpose/live invocation bindings, no impersonation or foreign owner. Public-area query does not authorize counter read or write. Per-index credential generation is prohibited until separately approved provisioning.

## TC4 — Secret custody boundary B

Use the accepted L1 design only: a ciphertext backing owned by the SAME root participant under the existing protected parent, exclusive original native identity/RAW1/content/size/descriptor manifest, CurrentUser DPAPI. Versioned binding entropy includes root domain/device-generation/index/public profile/purpose/custodian/credential version. No CA/issuer key reuse, second vault/service/ledger, plaintext persistence or secret evidence. Same-user/admin processes are trusted within this candidate boundary; DPAPI does not isolate application processes or stop such privileged actors. If this trust assumption is rejected, this candidate cannot pass: propose a separately reviewed TPM policy/custody design, do not claim process isolation from AUTHWRITE.

Revocation of custodian approval stops all future leases immediately in governed authority; ACL change/credential rotation/deletion is not automatic. Actual32-byte NV secret may still be known to prior trusted software: documentary revocation alone cannot cryptographically erase it or revoke TPM access. Any stronger access-revocation requirement needs an explicit separate auth-change/migration design and approval. No new command is authorized here.

## TC5 — Generation designation C

EK is device public identity, NOT provisioning generation. The candidate index0x01804143/8bytes/SHA256/attrs0x00040014 is not reserved or owned. A future exact ceremony must bind original qualified hardware endpoint/EK/chain, authorized physical/firmware/storage inventory and explicit independently persisted, non-co-restored hardware boundary to exact root/predecessor/approval/custody and index public profile. No label/hash in a local manifest manufactures that boundary. Record the original creation/initialization/readback event and the actual first baseline; do not assume0/1. An inert future generation identifier, if approved, records that ceremony, not a hardware monotonicity proof by itself.

After first write, WRITTEN changes profile/Name to0x20040014; pin the exact approved state transition and authenticated Name before every command. Same Name on a recreated index is not original-generation proof. Baseline+1, original credential-controlled access, full ledger lineage and live exclusive lease are all mandatory; no rebaseline on mismatch. Missing/redone index, Clear/device/firmware discontinuity, lost root, changed auth, ambiguous generation, failed persistence or unexpected counter => DENY/UNKNOWN. Do not re-create or adopt matching bytes to repair history.

## TC6 — Threat boundary and proof C

Target guarantee is detection of rollback of ALL restorable local ledger/ciphertext state while an authenticated SAME hardware witness remains independently advanced. Hardware/TPM/vTPM state restored jointly with disk is outside that guarantee and MUST NOT be claimed protected. A local test with two restored files must expose this limit. Trust in approved host/firmware/TPM and the designated hierarchy/custody actors is explicit; malicious actors able to reset/redefine/authenticate as those authorities are not magically excluded by hashes or DPAPI. If that boundary conflicts with intended W4 guarantees, STOP and seek an exact revised threat-model decision before execution. No M3 assumption is inherited.

Native increment/read/reboot/persistence and actual rollback comparison still require evidence; neither a certificate nor acceptance of C can assert these facts. Resource costs remain measured samples/UNKNOWN, never credits.

## TC7 — Ledger and crash protocol

Use accepted L1 carrier5/component512/same-root worker14 and exact-original-preimage CAS only. Authenticate original root/raw ancestry/context/custody/witness -> persist PREPARED -> one irreversible increment -> verify exact expected -> persist CONSUMED -> fresh original invocation guards -> one reader -> RESULT/UNKNOWN. Only original live lease/PID/creation may read. Reboot/reconciliation of CONSUMED cannot grant a replacement invocation. Crash, missing state, auth failure, rate/retry code, ambiguous response/close or drift freezes and retains dependencies/first failure; never resend/rebaseline/refund. Query-only recovery cannot consume anew. No local CREATE_NEW marker substitutes for witness.

## TC8 — Independent operational decision D

A/B/C owner decisions, factual hierarchy/custody/boundary data, qualified native authenticated NV sender, exact artifacts and fresh budgets must precede a request for effects. Current candidate limits remain at most1definition and2increments total, no undefine/Clear/AK/hierarchy/rollback/cleanup. These are candidate ceilings, not present authorization; no future baseline/Name/secret/generation/hashes fabricated. Experiment may not publish root/CURRENT or read Q4. Reboot and later Q4 require their own allowed effects/approval. Any executable request must state actual custody, trust profile and device/index generation facts; command remains null until complete.

## TC9 — Decision record and alternatives

Record decisions A/B/C separately under exact proposal SHA and their explicit choices, before dependent integration. No bulk ACCEPTED promotion by task title or broad technical autonomy. A: strict trust/revocation or narrow explicitly bounded diagnostic exception. B: designate custodian only if legitimate authority delivery and same-user/admin boundary accepted, otherwise retain DENY/propose stronger custody. C: designate only evidenced nonrestorable boundary and original provisioning lifecycle, otherwise no TPM witness/Q4 use. D: later exact manual operational request; no automatic operation or retries. Safe alternative for all is preserve current DENY and historical reservations. ED11/Phase6 PARTIAL; E21 pending; E17-E20 independent.

[Technical evidence and remaining facts](../integration/n0start03-integrated-closeout-20261024/tpm-trust-custody-resolution-20261028/CONSOLIDATED-CLOSEOUT.md).
