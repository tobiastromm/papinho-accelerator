# Canonical issuer carrier — proof and exact remaining integration gate

Task PACC-PHASE6-CANONICAL-ISSUER-CARRIER-INTEGRATION-1, 2026-10-20. Branch main, HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a. Prior task final manifest matched before any change; baseline3842 files recorded. Accepted decisions are unchanged.

## Contract / format determination

T1/T4 of DECISION-server-tls-identity-persistence-revision2-20261020 already define mandatory J16384: historical ROOT06 anchor8192 plus issuer image8192, at the exact outer offset48/length52. No extension is necessary and none was made. The anchor remains archived issuer hash/history context, never active listener policy. No ROOT06/BODY05/D3/LST1/ROOT8/SNT1/CP2 byte layout, hash domain or epoch semantics changed. PACICP01 is not appended inside the carrier. Initial CA creation, v2 bootstrap and operational migration remain outside this integration.

## Function-level implementation

|Function|Owner/lifetime and input|Effect/rejection|
|---|---|---|
|papacc_configuration_issuer_carrier_inspect06|Existing configuration owner, actual writer thread; immutable caller-owned16384 bytes; disjoint caller workspace/view output; no borrowed input into owner storage|Reuses issuer06_binding and read-only store05_select to inspect the COMPLETE historical journal plus issuer image. Rejects bad size/range/overlap, pending work, stopped/unresolved/lost authority, malformed journal, anchor hash or root mismatch. Returns data only with epoch/proofs0. No file open/adoption, publication, new owner or persistent object.|
|papacc_root8_issuer_carrier_data, kind6|Existing owner + caller workspace + immutable complete carrier floor, all borrowed for the synchronous call|Validates floor and candidate, exact root/I, byte-identical archived anchor and existing checkpoint_assess event-prefix/high-water continuity. Missing floor is INCOMPLETE. A data-valid report is explicitly RECOVERY_DENIED, never CURRENT.|
|same callback, kind7|Same enclosing carrier and SNI row; no source/pin created|Checks authority/generation/CA-SPKI and SERVER request CSR/SPKI. NAME_ONLY is data only. REQUEST_BOUND accepts reserved/sign-requested data, never authorizes replay. Revoked and inappropriate states reject. CERT_BOUND remains INCOMPLETE pending actual DER/profile/SAN and source authority proof.|
|papacc_root8_body05_data|Historical callback unchanged|Still rejects carrier kind6 as UNSUPPORTED. No caller or production configuration automatically switches to the new data callback.|

The new context contains pointers to the SAME existing owner/workspace and a borrowed comparison floor, not a second registry or authority. Caller retains exact snapshots and references through synchronous inspection. No input pointer escapes, no pin is removed, no stored event is rewritten. Codec success/completeness does not promote MODEL_BOUND to RESOLVED. Inspecting embedded file IDs checks serialized consistency only; it does not prove actual file identity, native custody or CURRENT. Source leases remain separate from root pins.

Stop is checked before new inspection. This code is not installed into the active owner event loop or worker; no claim is made that synchronous hashing/legacy decoding is bounded for active Stop. The existing pending worker, slot pins, Sessions, exactly-once/credits, CAS and uncertain-result retention paths are unchanged. No native hash-failure/cleanup-injection qualification is claimed for the reused historical codec; failure cannot establish authority. Positive native source/lifetime qualification remains a separate gate.

## History preservation and tests

New positive carrier cases build a complete historical PACL0501 journal fixture using BODY05 encoding and real native SHA, then the existing issuer-image init/append codec. No store/file, CA, key, private signature or real certificate is created. Embedded file IDs, epochs/approvals, CA identity and predecessor are synthetic. A three-byte model result is deliberately not X509. This is native public-codec execution over synthetic administrative history, not proof of a committed/current product installation.

Tests verify full-envelope ROOT8/v2 validation with the new callback; same carrier bytes remain intact. Reserved serial high-water is2, a SIGN_REQUESTED extension preserves the complete prior prefix, and a later model PUBLISHED/REVOKED history remains readable with high-water unchanged. The revoked request cannot create a new REQUEST_BOUND association. Restoring an older valid image against the retained newer floor rejects: no serial reuse or loss of revocation. Corrupt anchor digest, wrong carrier size, wrong issuer revision, missing floor, wrong owner thread and Stop reject. Existing suites cover core/backend denial, checkpoint rollback, uncertain outcomes, model retention, LST1 and historical ROOT8 behavior. CAS conflict/UNKNOWN and worker retention were not altered; no new native v2 CAS/commit proof is claimed.

Debug/Release compile the server and specified test targets with /WX and zero warnings. Nine selected offline suites pass each; new model41500 assertions each. No network tests ran. See build scripts/logs, test-results.json and final-manifest.json.

## ED11 and dependent STOP

No owner/slot/packet struct grew; no permanent buffer, heap, worker, registry or new root was instantiated. The prior runtime managed base2084039 is unchanged, not a new full activation measurement. Native provider/kernel remain UNKNOWN. New context is32 bytes on this x64 target; supplied existing config-view workspace is1104 bytes (compiled immediate0x450), issuer data view176. Context/workspace reuse must be proved in the actual caller graph before crediting it; the offline fixture reuses its existing view. Its static test graph remains227905 bytes and is not the product budget.

Compiled Debug/Release RSP subtractions: inspector360/304, carrier callback848/736, historical journal selector1384/1248, checkpoint assessment488/240, issuer image validation560/456. These exclude saved registers, nested calls, hash providers, input/output storage and retained graphs. They are evidence, not a complete stack bound. Raw disassembly and frame excerpts are retained.

The existing pending worker still has three16896-byte images and the same historical commit mechanics. ROOT8/v2 needs20512..28960 per image. Naively widening all three would add36192 bytes: conditional base2120231, exceeding2097152 by23079 before other activation costs. This was NOT applied or treated as the measured current peak. Separately allocated maximum prefix+output needs57888 bytes before decoded copy/floor, workspace, stack, source leases or retention; historical margin13113 is not an approved reserve. Sharing with the existing three issuer snapshots or retained slots is unproved and cannot be assumed. There is no qualified full activation graph or credit; dependent worker/persistence attachment remains DENY. No quota or legitimate reserve was reduced.

## Implementable handoff

The offline adapter is ready for a separately reviewed SAME-root caller. Before productive attachment, prove an owned CURRENT source with root R/A/P/I/base C/W, real pin ticket and source lease; provide the complete selected predecessor carrier and verify same-root journal/issuer history; reconcile storage and full callback stack with the existing worker/slots within2097152. Preserve the exact retained bytes on CAS UNKNOWN/Stop and never fall back to carrier-free ROOT8. No productive format migration or packet widening is authorized by this data callback.

Then connect the exact listener/security_ref/channel TLS profile/name approval/issuer tuple to the SERVER X509 preparer, holding both source and root references and requiring matching SAN/serverAuth/KU/BC/validity. CERT_BOUND needs the actual result DER/profile/CSR/SPKI proof. Raw decoded names, floor bytes or proof-free issuer views cannot authorize that call. Actual CURRENT absent/stale authority rejects at the authority layer; this callback intentionally supplies no authority layer.

Authoritative recovery, backup/fencing, native signing authority and active-Session revocation policy remain separately gated. No CNG/Win32 dependency was added to the portable issuer core; Windows-specific adapter remains Windows-only. Box/ISA/NT4 are not qualified. Browser/PST/wire and production0.6.4 were untouched; PST0.6.5 remains candidate.
