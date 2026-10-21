# Binding/persistence gate — conformance and implementation handoff

## Factual state

main / c50b8fb9a7e3293b5e6355fc08459bf85b56857a. S1–S5 source relationship is ACCEPTED, but the accepted decision explicitly excludes byte layout/persistence/functional hunks. Existing channel_tls parses only security_ref; normalized server security snapshot lacks the approved-name/source/issuer tuple. ROOT8 version1 embeds BODY0.5; neither it nor LST1 represents the missing tuple. Existing private issuer request likewise does not carry it. The public server X509 preparer takes an explicit candidate string and cannot supply administrative authority.

Therefore no functional patch can yet safely connect the approved name to X509 consumption. Task §4's preparation branch is executed; task §9's persistence-without-accepted-layout/source-authority STOP blocks dependent implementation. All independent components previously completed are preserved. No source/codec modification, build, test, network or key/certificate operation runs here.

## Proposed minimal versioned content

PERSISTENCE-LAYOUT-PROPOSED.md T0–T6 defines ROOT8 version2 with the same magic and explicit version, unchanged candidate BODY/descriptor collection, and one root-owned SNT1 component. No standalone store or registry. It also defines a distinct CP2 checkpoint discriminator to avoid interpreting CP1 under changed domains. Existing v1/CP1/LST1/BODY/ROOT06 bytes/readers remain unchanged. offsets.csv contains complete static region coverage; composition.json documents exact formulas. All40 N/B/K shape combinations and offsets were checked as DOCUMENT ARITHMETIC ONLY, not codec tests/native proof.

There are three association data stages so an already-issued certificate is not fabricated: NAME_ONLY has approved name plus existing issuer but no request/key/certificate; REQUEST_BOUND associates existing actual CSR/SPKI/request/reservation; CERT_BOUND adds actual result digest. None is an authority flag. Exact request/private-effect states remain in the existing issuer core/history, including serial burning and uncertainty. Source approval revision is an existing root revision, not a parallel counter.

| Authority | Conformance of proposed boundary / separate gate |
|---|---|
| S1–S5 | Same-root public name, explicit listener/security/profile/issuer, true approval/current/lifetime checked externally; raw decode never RESOLVED. |
| Technical profile §1.3/§1.5 | SERVER purpose, SAN equal approved name, CSR not identity authority, separate client/server/CA roles; no certificate/issuer invented. |
| ADR0009/0010 | listeners[] preserved; every Secure ordinal has explicit row; existing source transaction owns data and existing leases protect it. No global default/alias. |
| papacc.conf | No new directive/syntax/source implementation; grammar/AST unchanged. |
| D3/LST1 historical | New version only; unchanged embedded bytes, complete explicit version dispatch, no reserved-byte use in old versions or silent migration. |
| root/C8/I/R/A/P | Same worker/CAS/commit/CURRENT/epoch sequence; exact predecessor/base/source checks; no old authority fallback or synthetic epoch. |
| Issuer persistence | No carrier/record loss; mandatory same-root issuer relation must be validated by existing owner. Native carrier integration currently UNSUPPORTED, so dependent v2 integration remains DENY. |
| Model E/ED08 | Immutable owned snapshots, independent source borrow/root pin, Stop not cancellation/destruction; uncertainty and old dependencies retained. |
| ED11 | Per-component/array arithmetic separated from full actual graph; no quota increase/automatic margin use. Activation DENY until compiled/metered reservation. |

No material conflict with accepted contracts was identified in this PROPOSED boundary. Accepting it would specialize a NEW format/version only, not supersede D3 Revision3/version1 or approve a functional patch. Owner and official normative acceptance remain required; documentary official review alone is not normative adoption.

## Smallest implementation after acceptance

1. Implement only the portable data model/codec for the accepted new version, reusing existing hash/byte helpers and canonical embedded validators; disjoint owned outputs, no authority or new allocations by hidden convention. Keep both historical and new decoders explicitly separated by version. No runtime registration.
2. Reconcile real existing packet/model/input/output capacities, issuer carrier adapter and pinned source/root generations. If unsupported or over cap, preserve earlier components and deny dependent integration; do not widen blindly. No carrier work is authorized by this handoff alone.
3. Integrate tuple into the SAME administrative participant/source owner, using its existing source lease plus root CURRENT pin. Only factual admin approval/committed history produces positive name authority. A fixture can test function behavior, never produce production approval.
4. Consume name only from that resolved immutable association: compare listener/security/TLS role/revision/current, issuer authority/generation, CSR/SPKI/reservation and selected result. Recheck before passing to existing server preparer and before any later effect; mismatches/Stop/source loss deny. Signature authority remains a separate gate even on a valid association.

Required later tests: all field/stage/presence/name limits, unknown version/flags/padding, exact bounds/round-trip/integrity/truncation/trailing/overlap; valid/missing listener/security/profile/name; mismatched SAN/CSR/SPKI/issuer; no CURRENT/stale source/approval history; root v1 compatibility/explicit adoption; carrier absent/unsupported; CAS success/conflict/UNKNOWN without fallback; Stop/pending/retained snapshots/buffers; compiler Debug/Release zero warnings; actual transitive ED11. Native authorization/name-source/lifetime proof must remain separate from synthetic fixtures. No real private operations in those offline tests.

## Remaining gates and resources

New bytes are PROPOSED only. Existing known managed base2084039/2097152 is unchanged, not a complete activation peak. Source tuple/string copies, all retained generations, buffers, callbacks, carrier and issuer snapshots, signer/CA public backing/provider/kernel require actual census. The proposed encoded/model pair max6304 and full-size array deltas are not approved credit or a complete owner price. Existing source stack allowance is not automatically reusable.

First gate: owner acceptance/official registration of T0–T6 exact layout. Then exact reviewed component code plus actual capacity/lifetime proof; carrier issuer remains a separate unsupported integration boundary. CURRENT/operational publication, signature, native issuer recovery and all admission remain denied. No proposed encoding boolean can stand in for administrative approval.
