---
status: accepted
decision-date: 2026-10-22
scope-level: project
scope-target: canonical prepared-storage DATA only
---
# Canonical prepared-storage DATA boundary

Owner task PACC-PHASE6-V3-V4-CANONICAL-PREPARED-STORAGE-1 explicitly approves
this scoped implementation subject to official executor review. This record
freezes that owner decision before dependent source changes. The earlier
ADR-CANDIDATE-canonical-storage-sha256-backend.md remains an unchanged proposal;
its provenance and storage call-split prerequisites are resolved only for the
following boundary, not for a global issuer or platform backend replacement.

Reuse the Accelerator-owned BearSSL0.6 capsule already qualified in
../integration/canonical-storage-hash-closure-lab-20261022/implementation-proof-and-handoff.md.
Pin commit8ef7680081c61b486622f2d983c0d3d21e83caad and original release SHA256
6705bba1714961b41a728dfc5debbe348d2966c117649392f8c8139efc83ff14.
Keep original arithmetic, MIT notices, closed headers and local byte-copy helpers.
Compile the same files once in a private storage target; no Browser/PST build
input, full TLS library, new mathematical implementation or crypto service.

IN_SCOPE: store03_hash; the480-byte record digest inside storage BODY03/04/05;
storage-only root/backing/predecessor/candidate/checkpoint math callbacks.
Split these from papacc_store_hash, standalone store02_hash and the public
papacc_configuration_issuer_hash06, which remain historical native APIs.
Generic issuer DATA codecs may receive the storage callback at an explicit
storage site. Certificate/signature/status/revocation/native issuer operations
remain unchanged and independently gated. No silent native fallback.

Establish explicit version3/4 DATA preparation in the same owner/cohort/slots.
Protect CURRENT/predecessor with the real canonical pin. An unpublished candidate
has no published pin: synchronous validation uses its same-owner cohort identity
and bounded lifetime. Keep backings immutable, rehashes under existing locks,
full history, CP3/CP4 and kind8/kind9 semantics. No second owner or pin table.

Separate structural/mathematical DATA from factual source resolution,
administrative authorization and immediate pre-effect revalidation. DATA PASS
never resolves the other three gates. No positive v4 worker commit or CURRENT
publication is authorized by this task. Native identity/full predecessor byte
comparison stays inside the existing transaction before effect; UNKNOWN retains.

Do not clear/release old BCrypt references on migration. Stop, quiescence,
borrows, retention, Model E and existing quotas remain binding. Charge the actual
capsule stack/code/data and any same-owner capacity growth; no full ED11 PASS
or native/provider savings without a demonstrated reachable-call/lifetime bound.
Windows Selected3,145,728; Legacy2,097,152; no automatic target qualification.
No PST/Browser/wire/production0.6.4 change, network, commit or release.
