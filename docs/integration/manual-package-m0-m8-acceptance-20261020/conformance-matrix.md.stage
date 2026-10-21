# Exact M0-M8 review and sixteen-condition matrix

Scope: normative contract acceptance only, no code/build/test/network. No material conflict found. This does NOT prove real NT4/VC6 compatibility, vetted PBKDF2 integration, full memory peak, signer delegation availability or operational CURRENT. Section4 of the owner request permits recording adaptation gates; ADR-0034 separates normative approval from target qualification. Missing target evidence remains DENY before dependent effects.

|Condition|Exact contract / authority|Finding|
|---|---|---|
|1 Version/magic/encoding/byte limits|M2/M3: PACCMP02/v2/suite1, BE92 header, ZIP32 LE, strict16/5 TLVs, no trailing bytes|Normatively determined; offsets/length arithmetic consistent|
|2 Confidentiality/authenticated content|M1/M2: ChaCha20-Poly1305 key32/nonce12/tag16; header+SPKI AAD; entire archive ciphertext; quarantine until full tag check|All client key/material archive bytes covered; header/signer are intentionally public authenticated data|
|3 KDF/password|M1: PBKDF2-HMAC-SHA256 dkLen32/salt16/exact600000;20random bytes->32Base32 ASCII and exact input normalization|No implicit parameter or user-chosen weak secret; qualified implementation/cancellation still required|
|4 Authenticity independent of password|M1/M2 dedicated RSA2048/e65537 PKCS1v1.5-SHA256, exact domain/input, external approved signer purpose/delegation|Embedded key/hash/password/CA cannot self-authorize; signer checks precede KDF|
|5 Manual private custody|M4 and accepted issuer3-4/technical3.4|Manual transient protected server custody only, separate delivery, no plaintext persistence/recovery password; automatic unchanged|
|6 Necessary contents only|M3 five fixed members CA/client leaf/client PKCS8/config/issuance|No CA/server/signer private key, grants, runtime tokens or extra files; metadata is bounded provenance|
|7 Expected server source|M0/M3C3-C4/M4 and S1-S5|Same-root approved name/profile/issuer and matching server SAN required; candidate string/hash is not authority|
|8 Package signer identity/scope|M1, technical1.1-1.3 and3.4|Separate codeSigning key/admin delegation, exact SPKI hash and256byte signature; actual approved signer context remains required before use|
|9 ZIP attacks|M3 canonical STORE-only ZIP, five names/order, contiguous local/data/central/EOCD, CRC/length/offset agreement, zero extras/comments/attrs|Traversal, duplicates, links, descriptors, overlap/hole/trailer/ZIP64/compression reject; no extraction|
|10 Rejection|M1-M3/M5|Wrong password/tag/signer/version/suite/flags/length/content/purpose/time/key rejects before publication; unauthenticated plaintext quarantined|
|11 Target size/process costs|M7 and explicit owner section4|Normatively bounded and fail-closed. Actual NT4/VC6/KDF cancellation/full peak NOT PROVEN; explicit qualification/adaptation gate, not compatible/PASS|
|12 Browser integration|M5 plus Browser ADR0007/current importer/store|Existing advanced path preserved; new simple path must reuse same immutable generations/Reference/gate/commit/leases. New package importer NOT IMPLEMENTED or qualified|
|13 Automatic custody|M0/M4, accepted issuer5|Client-local automatic generation/custody retained; no extension of manual exception|
|14 No grants/admission|M0/M4/M6, provisioning2/5/8 and ADR0028|Preparation/decode/delivery/signature never supply Principal/grants/admission/CURRENT|
|15 Revoke/renew/retain|M4-M6 plus accepted issuer7-8|Persistent future-auth revocation and explicit renewal; active Session policy remains pending; uncertain/retained resources not freed|
|16 ED11 complete accounting|M7, Windows process profile W1-W6|Account KDF/crypto/ZIP/keygen/secret and material buffers, caller/stack/source/root/issuer/generations/retention/native costs; no quota increase or automatic memory credit|

## Compatibility facts and open gates

- Browser NT4/VC6: C89/portable byte parsing and standard suite are design constraints, not host evidence. No real VC6 build, NT4 run, memory measurement or KDF latency/cancellation test was executed/permitted. NT4_VC6_COMPATIBILITY=NOT_QUALIFIED.
- Crypto backend: Browser third_party/bearssl/inc/bearssl_block.h exposes br_chacha20_ct_run and br_poly1305_ctmul32_run. Existing RSA verification/signature and HMAC primitives support candidates; no selected qualified PBKDF2 API/artifact or complete package adapter was located. M1 requires a vetted pinned/licensed adapter and vectors before integration, never a fabricated br_pbkdf2 symbol or new crypto primitive.
- Existing Browser material import accepts DER/raw Base64 and PKCS8v0 and checks local key correspondence. It is not a complete package authenticator, chain/time/EKU validator or ZIP reader. Existing material/store/generation/ActiveConfiguration/lease code is reused after separate adaptation; no advanced validation is relaxed.
- X509: accepted profile1 direct CA RSA2048/e65537/SHA256 and distinct CA/server/client/package purposes apply. ClientAuth and serverAuth/SAN remain distinct. Included channel CA does not approve origin HTTPS; Site TLS SYSTEM trust stays separate.
- Memory: archive49654/outer51042/material49152 disjoint illustrative subtotal149848, not a process allocation reservation. Generic input cap131072 and ciphertext cap65536 can require more. Default2MiB/explicit Windows3MiB per-process limits are unchanged; Browser/box/ISA do not inherit3MiB. No transitive activation or provider/kernel bound is claimed; ED11 total PARTIAL.
- Future targets: no Win32/CNG dependency in the interchange contract; actual adapters/entropy/custody/clock/crypto/retention require target qualification. Box/ISA and NT4 are not declared qualified.

## Exact incorporation and historical status

Source proposal Windows-1252 prose contains byte0x96; it is preserved as raw frozen clause bytes. Accepted registration is UTF8 and references exact intervals/hash, avoiding silent recoding or a byte-exactness claim about a transcoded rendering. The old source remains PROPOSED as history. Only M0-M8 and the two exact hash-pinned metadata paragraphs are accepted; prior comments, handoffs, CLI implementation examples, Stop-on-revoke policy and external-issuer alternatives are not promoted wholesale.

M8/historical not-accepted wording is superseded only for exact normative acceptance by the new owner-authorized record. No implementation/code, operational signer, deployment trust authority or real package follows. The earlier issuer clause9 correctly excluded format acceptance in its own registration; this subsequent record fills that deferred contract without editing it or expanding custody.
