# Browser implementation handoff — PROPOSED / awaiting owner acceptance

No Browser edits authorized or performed by Accelerator Codex. Consume [CONTRACT-PROPOSED.md](CONTRACT-PROPOSED.md) only after the concrete choices are accepted and implementation separately authorized. Do not infer an implemented package parser or CSR generator from the existing granular import.

## Exact delta and reuse matrix

| Component | Existing source / behavior | Minimum proposed delta and proof |
|---|---|---|
| Key/request preparation | R1 imports supplied unencrypted PKCS8; no qualified CSR generation path | Explicit client-local pending request/key reference through current replaceable credential backend; bounded PKCS10 CSR and approved entropy/keygen backend. Do not expose pending as usable CredentialReference; integrate with same gate/generation ownership, not another identity store |
| Manual import UI | `src/papinho_credential_options.inc`, existing advanced three-file import and draft Apply/OK/Cancel | Add explicit import-package action, verify independent signer/server confirmation and pending recipient reference; keep granular workflow/history intact |
| Envelope reader | `src/papinho_credential_import.inc` only DER/raw Base64, no container/authenticity parser | Bounded PCP1 binary reader, exact length/order/signature and clock checks from proposal; no archive extraction, code execution or file-controlled trust bootstrap |
| Crypto | Existing BearSSL X509/key decoder, RSA local correspondence sign/verify | Reuse vetted RSA SHA256 primitive for package verification; verify signature against independently approved signer SPKI. Current local match proof does not validate issuer chain, EKU, expiry or recipient identity; add those checks under existing crypto boundary, not hand-coded crypto or PST APIs |
| Material assembly | `papinho_credential_material_validate`, trust/cert/private-key fields | Assemble approved public anchor/client leaf with the actual matching local key. No key from package. Hold references and bounded secret buffers; use existing wipe/retention paths |
| Staging and publication | `papinho_credential_store_prepare`, config draft and `papinho_credential_commit_metadata`; ActiveConfiguration flip; candidate_committed afterward | Put request/package provenance and consumed-request/replay status into same atomic configuration generation; avoid a second commit point or standalone replay DB. Validate stale writer before publication |
| Duplicate/cancel/crash | Existing generation/lease and retirement coordination | Exact committed duplicate is no-op; conflicting bytes require explicit replacement; cancelled draft preserves current generation. Crash/readback reconciles committed metadata and candidate ownership, not automatic reissuance |
| Runtime/Stop | Existing reference leases, PST runtime users and safe retirement | No package branch in PST, routing, scheduler, reconnect or lifetime. Stop cancels pending work via existing mechanisms. RETAINED dependencies and credit survive until factual closure |
| Legacy compatibility | Historical granular DER/base64 import and generation references | No automatic migration/reinterpretation; retain formats/readers. Unknown package version fails explicitly. VC6/C89 and real NT4 acceptance remain separate gates |

No cross-process Session comparison or Browser assertion of Accelerator Principal. Package policy references are provenance/display only; actual server CURRENT/grants and authenticated Principal remain factual server authority. Do not import Site TLS trust into channel trust or vice versa. No network needed to parse/import a public package, but offline import cannot attest current remote revocation.

## Offline regression contract — define now, execute only under later authorization

1. Accepted exact known-answer serialization/signature, min/max bounds, streaming splits, byte-exact repeat export; C89 endian/64-bit handling independent of native layouts.
2. Tamper every signed field; wrong/unapproved signer, file-supplied signer substitution, invalid RSA padding/hash/suite/key, certificate parser failures and verify/use file mutation.
3. Wrong server name/anchor, unknown issuer, expired/not-yet-valid/implausible clock, unsupported chain and purpose/EKU, wrong local key/CSR/SPKI/recipient, substituted CSR approval context.
4. Duplicate/unknown/out-of-order/truncated TLV, overflow, oversized/empty values, NUL/control/invalid UTF8/DNS, wrong field count/flags/version/length/trailing bytes, forbidden private material custody model.
5. Exact duplicate without new generation; same CSR/different bytes; stale writer; cancel before flip, crash before/after flip, readback recovery, retired generation and outstanding lease. No replay by metadata deletion.
6. Stop during parsing/validation/import; failed allocation/store/crypto/write/flush/close; no secret logger/temp leakage, retained candidate with preserved ownership/credit; no double release/early reuse/resurrection.
7. Historical granular imports and existing configuration/lease tests preserved; separate synthetic tests, cryptographic known answers, actual current-host store tests and real NT4/backend qualification. No synthetic result qualifies mTLS/admission.

## Accelerator handoff dependencies

Same existing admin root needs real client registry publication/recovery without restart; first dormant transaction is insufficient. Issuer must be operationally selected, approved and queried idempotently. Package public signature/trust and local request lifetime require concrete acceptance; the provided proposal is not an available signer backend. No external/fixture CA creation is allowed by this handoff.

ED11 implementation must measure concurrent parser/crypto/material/store staging, old/new generation coexistence, pending request and bounded replay metadata, rollback and retention. Never put a 64KiB package on the owner stack or import it outside reserved credit. Exact additional bytes remain NOT_COMPILED; public package size cap is not a budget increase. Provider/kernel remain UNKNOWN.

NEXT=OWNER_ACCEPTANCE_OF_EXACT_MANUAL_PACKAGE_AND_ISSUER_CONTRACT
IMPLEMENTATION_AUTHORIZED=NO