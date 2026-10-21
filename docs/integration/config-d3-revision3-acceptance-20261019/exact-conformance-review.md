# D3 Revision3 — sixteen-condition exact normative review

Scope: accept only D3.0-D3.8 from the exact proposal identified in official-operation.json, if and only if executor approves its exact registration. No implementation or dynamic validation. New accepted record is not authority to run bootstrap, commit, listener, network or CA operations. No accepted document edited.

| Condition | Normative evidence and disposition |
|---|---|
|1 Byte-exact, no overlap/undefined fields | Header38 contiguous fields0..511; record22 fields0..1535; CP15 fields0..511. No gap/overlap in offset CSV. D3.1/2/3/5 explicitly define variable offsets, absent-region boundaries and reserved zeros. Static arithmetic/text inspection only. |
|2 Magic/version/endian/presence/count/padding/integrity | PACROOT8/version1, PACCD001, PACR8CP1; outer LE, historical components BE and LST1 LE. N1..4, zero reserved, explicit conditional flags and I/base; no padding/trailing. D3.1-5. |
|3 Size limits | T=1056+1536*N+L+J; bootstrap L=J=0 => min2592; max N=B4,L2208,J16384 =>25792. No actual buffers/code executed. |
|4 LST1 fragment | D3.0/1/3/6: historical base only, absent bootstrap, no candidate/current/grant authority. Accepted C1-C5 unchanged; no zero-epoch fragment. |
|5 Listeners collection | Ordered N complete descriptors+BODYs, unique stable IDs; target capacity rejects excess, no truncation/alias copy of singular source. |
|6 BODY05/ROOT06 historical | BODY1024 original bytes/hash, descriptor must match sole content. Optional ROOT06 prefix only as issuer anchor/history, never alternative active config. Historical readers and PACICP01 unchanged. |
|7 D2/D4 coherence | Static comparison confirms exact surrounding proposal retained. Bootstrap externally proven virgin/authorized, base absent; update actual CURRENT/pin/base required. D3 does not accept or modify D2/D4 by reference or invent bootstrap authority. |
|8 CAS uncertainty | D3.6 exact predecessor full bytes/length and native identity; UNKNOWN/conflict no old-context fallback, blind retry or bootstrap reset. Ownership retained. |
|9 CURRENT/A/P/I factual | Candidate R != CURRENT. Header stores historical base A/P; actual new A/P only CP after factual commit+complete publication. I is existing validated content revision, not copied R. No observer sees CURRENT positive with missing mandatory ack. |
|10 No freshness/fencing by hash | D3.5/7 C/Hcp/W integrity/correlation only. Valid CP insufficient; authoritative recovery DENY always. No new checkpoint/fencing service. |
|11 No ROOT06 migration | D3.6/7 prior ROOT8 only for update; historical input not silently imported/truncated/retried as bootstrap. Migration separate gate. |
|12 Issuer separate | D3.4/8 carrier origin/native identity and adapter safe separation explicitly required. No code or readiness claimed, no CA generation/serial reuse granted. |
|13 ED11 | D3.8 explicit byte-array subtotals/deltas and retained-generation example, not process/stack/provider bounds. Complete sizeof graph/reserve needed; quotas unchanged, UNKNOWN retained. |
|14 No auth/grants/admission | D3.0/3/6/8; descriptor labels and hash do not authenticate/authorize. CLOSED/certificate candidate recognized but unsupported first root profile. Provider/topology and all independent gates remain. |
|15 Historical/corruption | D3.7 explicit unknown/overflow/truncated/trailing/digest/presence/UTF8/mixed-generation rejection; no corrupt CP stripped as absent, no historical fallback. Data-only decoding. |
|16 Model E/Stop/pins | D3.6-8 immutable packets, existing worker/slots; Stop no wait/destructive retry; pinned/retained generations preserved with credits, no early release. Matches accepted issuer-C2 P4.7/P6/P7 and Model E/C8 boundaries. |

## Precedence and evidence limits

No material normative conflict identified within this exact scope. Accepting a new versioned envelope is the specific layout decision deferred by N6/F5/LST1 C1, not a silent edit to those accepted records or reserved historical bytes. ADR0007/0009/0028, provisioning, root/C8, issuer-C2, Model E and ED08/ED11 retain their authority. Existing state/build evidence does not qualify new schema/runtime.

Record incorporates source bytes starting at heading D3.0 through D3.8 only. Original PROPOSED status/history language in reproduced clauses is superseded solely as to D3 normative acceptance. Operational prohibitions remain. No other proposal/handoff/diagram/offset table, whole boundary proposal, patch or implementation is accepted by reference. Parent D2/D4 remain unchanged and are not automatically promoted; applicable bootstrap/update normative authority still must be available before dependent implementation/effects.

Static-review.json records hash and patch/text checks. The initial comparison script detected CRLF-vs-LF handling in the review script, not changed proposal clauses; the comparison was corrected to normalized text while accepted payload remains original byte-exact UTF8. No protected write was attempted at that point, no approval denial or alternative mechanism occurred. No unit/native tests/build/network were run.
