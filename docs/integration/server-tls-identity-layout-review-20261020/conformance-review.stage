# Focal T0–T6 review — Revision1 findings and Revision2 proposal

Review date2026-10-20. Owner explicitly requested review BEFORE acceptance. main / c50b8fb9a7e3293b5e6355fc08459bf85b56857a. No functional patch or normative acceptance is authorized by this review. The original Revision1 remains byte-exact.

## Conclusion

No overlapping offset, static-region gap, size-equation error, hash self-reference or direct contradiction of accepted root/history formats was found. However Revision1 should NOT be accepted unchanged: four semantic points need exact clarification to avoid permissive implementations. A corrected Revision2 remains PROPOSED, with IDENTICAL offsets, sizes, presence masks and hash domains. This review does not prove a native source, carrier, CURRENT, signer, recovery or memory qualification.

The actual discriminator is the eight-byte ASCII PACROOT8 plus uint16 version2. The owner's prose label ROOT08 is not an alternate magic/encoding and must not cause normalization or alternate dispatch. SNT1 component/records are PACSNT01/PACSNI01 version1; checkpoint is PACR8CP2/version2. Unknown alternatives reject.

## Findings and smallest exact corrections

| Finding | Exact Revision1 location | Risk / smallest Revision2 correction |
|---|---|---|
| R1 TLS/certificate profile distinction | T3 record20:2 and “Role/profile/transport disagreement rejects” | Profile1 is the certificate technical profile, not proof of channel TLS. Explicitly require actual immutable source to resolve ADR0006 TLS1.3 mTLS, ALPN papacc/1 and other accepted requirements; reject missing/mismatched support. No new profile enum/bytes/catalog. |
| R2 approval provenance tuple | T3 “On introducing/changing the exact name/listener/security/role relation” and field32 description | Same name could mask issuer/source/profile replacement. Enumerate the approved tuple including channel source/profile, certificate profile, issuer authority/generation/CA-SPKI; changing any requires factual new approval revision. Separate request/result transactions remain required; old dependencies/uncertain reservations are never retagged or cleared. |
| R3 shared security_ref coherence | T3 “Shared security_ref is allowed ... coherent mapping” | Define the exact equality fields for one selected immutable Local Identity; reject conflicting rows before exposure instead of choosing first/default. Distinct listener association approval revisions remain independently verified. No cross-generation memory reclamation implied. |
| R4 exact DNS representation | T3 “numeric IP representation” | That phrase is not an exact predicate and could diverge from the implemented candidate builder. Specify exact ASCII/label and all-digits/dots rejection, without claiming to classify every legacy IP spelling or perform resolution/normalization. No hostname is selected/approved by this representation check. |

These are proposed normative clarifications, not retroactive edits to accepted contracts. exact-normative-corrections.patch and correction-matrix.json preserve exact preimage/replacement text. Original proposal SHA256: 8f84533e42ea7a6273a1209af6ebf4685fff19fc8cad1610af8d1672848a131f. Revision2 full artifact SHA256: 0042fe329962d9f713425addf331f0bf5641f790810bc657a12fe0d328eab875.

## Ten requested conditions

| # | Condition | Review outcome / evidence and remaining gate |
|---|---|---|
|1|Single administrative root|CONFORMANT_DESIGN. T0/T5 use same participant/store/worker/exact CAS; SNT1 cannot be independently persisted/adopted as authority. No second registry/catalog/owner introduced. Native integration not implemented.|
|2|Byte-exact SNT1/CP2|STATIC_LAYOUT_CONFIRMED. Five complete matrices header512/record1536/CP512/table-header64/association768; 40 N/B/K shapes; total20512..28960; explicit masks15/31/63 and reserved zeroes. Four acyclic SHA256 domains; CP2 and v2 discriminator explicit. This is document arithmetic, not codec/test qualification.|
|3|S1–S5 relation|REVISION1_NEEDS_R1_R2_R3; REVISION2_PROPOSED_CONFORMANT. Distinct listener/security/actual channel TLS/name/issuer, exact CSR/SPKI/result references and separate source borrow/root pin. R4 makes lexical behavior exact, not approval.|
|4|Historical formats|PRESERVED. No BODY0.5/ROOT06/LST1/ROOT8v1/CP1/PACICP01 bytes/readers/hash domains changed. New v2/CP2 interpretation only after separate acceptance. Full predecessor bytes retained; no truncation/reinterpretation/fallback. Compatibility is a design requirement, not executed new-version regression evidence.|
|5|Update-only|CONFORMANT_BOUNDED_SCOPE. v2 initial bootstrap explicitly UNSUPPORTED; existing initialized issuer/base I required. No missing-CURRENT-as-bootstrap; recovery always DENY. Existing accepted bootstrap unchanged.|
|6|CURRENT/A/P/I, predecessor/CAS|CONFORMANT_DESIGN. R=pre+1; factual pinned predecessor R/A/P/I/C/W and native identity; base LST1 plus whole predecessor/source history. Postcommit CURRENT/epoch only; exact zero-CP->CP2 transition preserves prefix; no issuer-only/P assumption or conflict/UNKNOWN fallback. No publication in review.|
|7|Issuer carrier|EXPLICIT_UNSUPPORTED_GATE. J16384 is required for proposed v2, but current native carrier path remains UNSUPPORTED. No issuer loss, anchor-as-policy, high-water reset or hidden carrier implementation permission. This gate cannot be marked PASS by valid serialization.|
|8|Negative/corrupt inputs|REQUIRED_FAIL_CLOSED. Explicit magic/version/size/offset/presence/order/strings/zero-padding/hash checks and request/source/issuer cross-validation; all-or-nothing owned output. Native/semantic checks remain mandatory beyond parse. R3/R4 remove permissive ambiguity. No parser/codec tests run.|
|9|Stop/lifetime/Model E|CONFORMANT_REQUIREMENTS_ONLY. Source borrow separately protected from CURRENT pin; immutable snapshots/old Sessions/pins retained; no buffer/credit release on Stop/timeout/unknown. R2/R3 explicitly preserve old dependencies. Actual caller/callback/native latency/lifetime proof remains implementation qualification.|
|10|ED11|ACCOUNTING_BOUNDARY_CONFIRMED_NOT_APPROVED. S864..3168; owned model max3136, separate pair6304; Emax28960; hypothetical3/4/6-array deltas9504/12672/19008. These cannot be added blindly to known base2084039 or consume margin13113 by inference. Full caller/source/root/issuer/signature/native coexistence UNKNOWN; cap2097152 unchanged.|

## Implementation and acceptance boundary

Revision2 addresses all four identified interpretation gaps within the accepted architectural direction. No material conflict remains identified at the DOCUMENT DESIGN level after these corrections. That is not approval of these new proposed constraints by the owner. acceptance-record-draft.md prepares only a future DELIMITED registration after owner concurrence on Revision2 and official executor review. It is not an ACCEPTED ADR and does not accept the original Revision1, implementation artifacts, schemas by analogy, carrier integration or a reserve.

After exact layout acceptance, the next independent executable slice is the portable OFFLINE codec/model/version dispatch and negative data tests, subject to exact functional review and actual memory/lifetime budget. Existing source-owner attachment and carrier-issuer integration remain separately denied until proven; no raw row/candidate can be passed as positive authority to X509 merely because decode succeeded. No productive apply/CURRENT, signer/key/CA/certificate or network follows from any acceptance.

## Preservation/evidence

No functional sources, accepted ADRs, original proposal/offsets/census, Browser, PST, PACC wire or production pin0.6.4 changed. No build/test/network or private/native key operation executed. Arithmetic analysis is limited to the proposed document fields; it is not code, synthetic runtime testing or native qualification. Hash checks and the baseline map verify preservation; the final manifest covers corrected proposed documents and exact review evidence. Gates remain SIGNATURE_AUTHORITY=DENY, CURRENT=DENY, AUTHORITATIVE_RECOVERY=DENY, CLIENT_ADMISSION=DISABLED, SITE_TLS_ADMISSION=DENY, ED11_ADMISSION=DENY, provider/kernel UNKNOWN. PST0.6.5 remains deferred until integral Phase6 completion.
