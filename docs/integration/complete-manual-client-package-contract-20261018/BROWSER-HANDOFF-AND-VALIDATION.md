# Browser handoff and validation matrix — PROPOSED

Use only after accepted scoped custody exception and concrete technical contract, then separate implementation authorization. Current task writes only Accelerator documentation. [Exact contract](CONTRACT-PROPOSED.md); [owner request](OWNER-REQUEST.txt). Prior PCP1/CSR-local manual handoff is preserved history, not the intended complete-manual design. Automatic local-key path remains separate and mandatory.

## Browser minimum changes

Preserve “Configurar segurança — importação local”, existing advanced CA/cert/key selection and draft Apply/OK/Cancel. Add primary “Importar pacote de configuração...”. One operation asks file, independent issuer fingerprint/server confirmation, protected unlock, validation summary and final confirmation. No prior CSR, key, configured Browser or companion material files required. Trust reference must come from an independent authenticated admin channel; inability to confirm is a hard stop, not a hidden TOFU checkbox. Password unlock never supplies issuer identity.

Frontend in `src/papinho_credential_options.inc`; bounded envelope reader/crypto adapters adjacent to existing `papinho_credential_import.inc`, not another credential library/store. Reuse `papinho_credential_material_validate`, `papinho_credential_store_prepare`, existing candidate/reference, draft and `papinho_credential_commit_metadata` pipeline. Existing `papinho_accelerator_config_generation.inc` publishes ActiveConfiguration before candidate_committed. Extend that same generation metadata with public package/import provenance and replay identity atomically. Do not implement second journal commit that could diverge from ActiveConfiguration.

Authenticate envelope/signature using independent approved signer, KDF+AEAD, and only then parse bounded ZIP in reserved memory. Existing AEAD routine decrypts in place before caller verifies tag: plaintext must remain quarantined until full tag check. No disk extraction. Perform issuer/chain/name/EKU/time/cert-key validation beyond R1's framing/key-correspondence checks. Do not load PST/use trust or create candidate publication from unauthenticated material. Keep child-provider leases through validation/import failure and retirement as required.

Expose distinct errors: unsupported version/suite/backend, malformed/oversize, untrusted signer/server, unlock/authentication failed (no useful secret oracle), certificate invalid/time unavailable, stale revision, duplicate/no-op, cancelled, resource denied, committed/reconciliation required, retained. No password or PKCS8 bytes in UI diagnostics, CLI, Registry, exception strings or logs. Masked edit controls and clipboard handling require explicit implementation review; do not promise OS-global secret erasure.

After decrypt/material copy, wipe all temporary archive/key/password/KDF buffers using existing reviewed erasure utility once borrowers quiesce. Store persists approved plaintext private material under its existing replaceable protection backend; this does not magically add encryption at rest. Where backend unavailable, deny Secure persistent import and preserve previous generation, never fallback to Legacy.

## Compatibility matrix — factual static assessment, no new build/run

| Item | Existing evidence / source | New proof required |
|---|---|---|
| C89/VC6 integration | Makefile.nt4.mak existing BearSSL library recipe, vc6_compat forced include, /TC, NT4 Win32 target | Compile new parser/adapters Debug/Release with real VC6, zero warnings, static endian/64-bit overflow paths |
| AEAD | BearSSL `inc/bearssl_block.h` br_poly1305_ctmul32_run and br_chacha20_ct_run; pure32bit candidate, no SSE2 dependency required | Actual archive exports/link, official AEAD vectors including bad tags, optimized comparison/wipe review; no claim new suite already qualifies |
| PBKDF2 | HMAC/SHA256 primitives present; KDF header documents HKDF; no selected qualified PBKDF2 adapter located | Pin vetted artifact/license/hash, standard vectors and cross-backend vectors, 600000-iteration real NT4 latency/cancellation/memory gate. No invented br_pbkdf2 symbol |
| Package signature | R1 uses br_rsa_pkcs1_vrfy/sign for local RSA correspondence | Exact package signature vectors, strict PKCS1 parsing/rejection, independent signer trust binding, no assumptions about selected deployed signer |
| ZIP | Existing credential importer has no ZIP package parser | Implement only bounded STORE subset; fixed5 names/central-local consistency/CRC/overflow cases; no general extractor or decompressor dependency |
| Credential material | R1 accepts DER/raw Base64 and unencrypted PKCS8v0, matching RSA keys | Decoded package path feeds same material validator/store; no PEM/PKCS12 automatic compatibility claim |
| Atomic config/leases | store/reference/candidate, ActiveConfiguration, credential_lifetime/finalization source exist | New metadata crash/stale-writer/cancel/lease/coexistence regression; historical committed credentials unchanged |
| NT4/backend | Legacy build compatibility direction exists; current R1 backend has target qualification limits | Real NT4 CPU, approved entropy at server only, Browser crypto performance/storage protection/LockFileEx/runtime acceptance; SOURCE_AVAILABLE != NT4_PASS |
| Accelerator | Existing modern MSVC/OpenSSL composition and same-root transaction components | Approved provider artifact/ABI for keygen/PBKDF2/AEAD/sign, live registry transaction and issuer adapter, no PST modification |

No compile/run was performed; actual compatibility of new complete package remains NOT_QUALIFIED. Existing primitive source/API availability is weaker evidence than linked target support and much weaker than complete import qualification.

## Security and regression matrix for subsequent authorized work

| Risk | Required controls | Future tests |
|---|---|---|
| Weak ZIP/encryption downgrade | Suite/version exact; authenticated envelope; no ZipCrypto | Reject plaintext ZIP, encrypted ZIP other profile, PCP1, unknown/altered suite/flags |
| Password mistaken for issuer | Independent signer SPKI and server confirmation; external signature | Known-password attacker-generated package/wrong signer/key/CA/server; no trust install |
| KDF DoS/offline guessing | Signature before KDF, fixed bounded work, random160bit unlock, bounded UI attempts/cancel | Huge iterations/invalid lengths, wrong password3times, Stop/cancel responsiveness; no offline-lockout claim |
| AEAD misuse | Fresh salt/nonce/key per new package, full tag, quarantine | Official vectors, modified header/SPKI/ciphertext/tag, truncation, nonce reuse guard, unauthenticated plaintext never parsed |
| Archive attack | Five STORE entries only; no paths/extraction/decompression | Duplicate/local-central mismatch/overlap/hole/trailer/ZIP64/descriptor/traversal/ADS/symlink/oversize/CRC/method changes |
| Secret exposure | Protected staging only; no plaintext temp/export/log; checked wipe/close | Allocation/issue/encrypt/sign/write/flush/close failures and crash evidence; no private-key/password traces; distinguish simulated failures |
| Wrong certificate/key | Approved issuer/cert usage/time/name plus key match | Wrong key, CA, clientAuth, serial, expired/notYetValid, implausible clock, disallowed chain, signature substitution |
| Copied/lost package | No device-binding claim, persistent server revoke/renew with new key | Duplicate import no-op; same package identity/different bytes rejected; old package revoked denies new auth; package deletion not revoke |
| Partial publication | Same-root durable intent/live commit/recovery; issuer lookup | Issue success/commit failure, uncertain issuer, crash before/after CURRENT; never issue duplicate on retry |
| Retention/lifecycle | Existing owners/leases/quiescence, original credit | Stop during KDF/import/export; callbacks/retained generations; no double release, resurrection, early reuse or fabricated DESTROYED |

Never promote synthetic fault fixtures to native failure proof, component crypto vectors to operational issuance, or import success to ED11/mTLS/HTTPS PASS.

## Minimum implementation slices after acceptance

1. Register only approved manual custody exception and exact package contract in designated normative record; select real issuer/signer, validity/retention policy, protected staging and reviewed crypto artifacts. Keep automatic decisions not needed by manual deferred.
2. Accelerator: same-root client registry/live update/recovery plus issuer query/idempotent revoke/renew; bounded key generation/protected package and local admin CLI. No second listener/registry or dormant guard bypass. Compile/account costs before claiming capacity.
3. Browser Codex: bounded envelope+ZIP import UI and same existing validator/store/generation/leases; protect transient secrets; preserve advanced import and historical readers.
4. Authorized offline cross-product vectors, export/import negative/cancel/crash/replay/retention tests; real VC6/NT4 and MSVC targets; compiled ED11 census/peak/credit. Operational issuer/PKI actions, network/admission need separate authorization.

Automatic and all six Phase6 provisioning gates remain OPEN. No new roadmap or collector/supervisor.