# Server TLS persistence / X.509 binding — implementation boundary

Status: PARTIAL, 2026-10-20. HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a, branch main. Normative authority: DECISION-server-tls-identity-persistence-revision2-20261020, T0–T6 Revision 2. No normative decision changed.

## Implemented independent component

`src/config/config_root8_v2.c/.h` implement a private portable offline data codec for PACROOT8/version 2, contained SNT1 and CP2. Header512, descriptors512+BODY05/1024 per listener, base LST1, carrier16384, CP512, final digest32 preserve the accepted domains and offsets. SNT1 record768, header64 and trailer32; CP2 hashes its first480 bytes. Whole hash covers all bytes before its final32. CP content digest covers the prefix before CP. Unknown versions, inconsistent presence, reserved bytes, impossible counts/offsets, padding, corruption, truncation and trailing bytes reject. Total20512..28960, candidate1..4, base1..4, Secure rows1..N. Historical code paths are untouched.

The caller supplies already-existing revisions/epochs and immutable data. Encode does not invent publication or authority; decode owns a caller-provided copy and reports data completeness only. Fragment encoders require the full containing context and never provide independent storage. ZERO512 CP means absent checkpoint. A valid digest is not freshness, fencing, CURRENT or signature authority.

SNI validation distinguishes NAME_ONLY / REQUEST_BOUND / CERT_BOUND, the server certificate-profile identifier from channel transport profile, exact DNS bytes and shared security_ref coherence. Existing BODY05 validation is reused. The component callback kinds5/6 validate BODY05 and carrier; private kind7 checks the row/carrier association. Kind7's contract is data validation, not a new owner, catalog or authority.

## Evidence and its limits

Debug and Release built the canonical server and listed offline targets with /WX and zero compiler warnings. Nine selected CTest suites passed per configuration: issuer offline, production gate, checkpoint, LST1, historical ROOT8, new ROOT8/v2, Windows public validation, X509 model and CNG production gate. Each new test execution reports41450 assertions. Logs and build scripts are retained here.

Positive v2 results use a visibly synthetic MODELANC carrier anchor, real issuer-image codec metadata, synthetic request/result bytes, and native SHA/BODY05 public codecs. The three-byte model certificate is not X.509 and is not evidence of certificate issuance. The model's R/A/P/I and predecessor are not a real committed history. The 16 N/B combinations and stages1/2/3 exercise structural composition, not native CURRENT/source authority. Every-byte mutation and all shorter file lengths reject. Rehashed malformed fields, association/issuer divergence, shared security_ref mismatch, malformed DNS, hash failure, missing component validator, overlap and capacity guards reject. The historical version1 decoder rejects v2. Existing issuer suites preserve synthetic Stop/retention/uncertain-result regressions; no new native v2 Stop/lifetime proof is claimed.

The ordinary native `papacc_root8_body05_data` returns UNSUPPORTED for carrier kind6; a regression verifies that v2 cannot bypass this. The new test callback is confined to its test executable, with no productive configuration selector. No test creates a key, private signature, CA, certificate, socket or committed store. Executable/public hashing evidence is native Windows; carrier/source/CURRENT positives remain synthetic or unavailable.

## Ownership, Stop and retention

The codec has no static mutable storage, heap, process handle, worker, pin or publication. Caller owns immutable input and disjoint output/metadata for the synchronous call. Callbacks must be synchronous, nonreentrant, nonmutating and must not retain pointers. Buffers cannot be recycled while their owning operation or callback is pending/uncertain. Codec return does not finalize a root generation or release a Model E credit. The codec is not registered in product startup or the worker; no claim is made that an arbitrary hash callback is bounded or qualified for Stop.

## First unresolved divergence / exact next integration gate

Native carrier is expressly UNSUPPORTED. Full carrier/history validation, a same-root owned source snapshot with factual CURRENT/revision/pin/lifetime, channel-profile proof, and the activation ED11 budget are absent. Existing worker packet capacity16896 cannot contain maximum v2 size28960. Do not widen it or discard issuer. This task preserves the independent codec and stops the dependent attachment.

The existing public SERVER X509 preparer remains unchanged. No raw decoded string is routed to it as approved identity. Operational binding must compare the exact listener, security_ref, TLS13/mTLS/papacc1 transport profile, ExpectedServerName, approval revision, issuer generation/CA SPKI and applicable reservation/CSR/SPKI/result against that factual source, retaining its pin through preparation and rejecting stale/Stop/invalidation. The resulting SAN must exactly match the approved name. No caller bool, candidate name or historical pin may substitute CURRENT authority. SERVER signer remains denied.

Before attachment, select a separately authorized carrier/source integration slice, prove the full base C/W/history link and Model E lifetime, and census all caller graphs and retained generations. Do not implement carrier by inference under this task. Recovery/fencing, native signing qualification and production initialization remain separate DENY gates. No new roadmap or architecture is introduced.
