# AT0-AT9 review,bootstrap and minimum plan

## Conformance/corrections

| Clause | Review | Revision2 proposed change |
|---|---|---|
| AT0 | Direction B remains reasonable,lowest footprint unproved | Reference the protocol evidence index;preserve full assurance model |
| AT1 | One root/no parallel registry preserved | Existing source-binding layout is still an explicit gate |
| AT2 | Independent bootstrap required | Clarify source key status,compromise,rotation and administrative withdrawal |
| AT3 | Nonce/signing is not necessarily reading freshness | Separate acquisition campaign,packet association and causal reading evidence |
| AT4 | Conservative envelope is conditional | Make quantization,endpoint conversion,checked arithmetic and source-truth premises explicit |
| AT5 | Same-operation offline limit is correct | Preserve fresh acquisition for new authorizing boundaries |
| AT6 | Restart never restores authority | Clarify source disagreement/compromised-key risk and separate fencing |
| AT7 | Existing owner/Stop/E0 boundaries correct | Preserve external buffer protection and retained work |
| AT8 | Protocol evidence gap partly resolved | Relevant sections verified;full profile/errata,bytes,crypto and capacity gates remain |
| AT9 | PROPOSED must remain PROPOSED | Separate revision/patch;no incorporation into accepted authority |

No accepted Q0-Q7 clause is changed or contradicted by an implemented effect. External peers are not authorized by the accepted Windows-local adapter slice;this proposal is future architecture only. Operational source transport and source-key representation need separately accepted scope and explicit execution authorization. No protocol is implemented or contacted.

## Trust bootstrap and status

First source trust must arrive through the existing explicit administrative process and protected canonical root/policy. Do not learn it from the first response,public server-list URL,TLS handshake or package signer. The configuration must identify its time purpose,applicable protocol/key/source revision,validity/status/revocation and owner lifetime. The current code has no proved representation for this binding;do not add fields by inference. A minimal versioned layout decision may be needed before persistence,not a new catalog.

Cryptographic verification under a pretrusted identity is distinct from establishing actual accuracy. Protocol-local delegation validity is not administrative revocation status. Temporary key rotation cannot enroll a replacement long-term identity;changes/withdrawal require governed revision. Long-term compromise invalidates applicability;uncertain status remains DENY. Offline use cannot assert fresh revocation information it lacks. Independent bootstrap must also address any credential validity check needing prior UTC;no TLS date bypass or using the same response as its sole authority proof.

The same owner binds root,CURRENT revision/pin,source configuration,process lifetime,invocation,candidate and acquisition campaign. Fresh qualified entropy and request identity must survive through verification/retention. Clone/restart must not reuse an old operation/challenge/approval. Neither hardware boot counters nor signed archives establish latest root history. No new journal or pin table is authorized.

## Conditional uncertainty inclusion proof (project analysis)

This is an analysis of accepted Q1/Q2/Q3 premises,not a claim any fetched server satisfies them. Let g be the actual reading instant;independently qualified source evidence encloses true UTC at g in [Ls,Us]. Let m0 precede creation/transmission of the unpredictable request and m1 follow final verification. The selected protocol/source semantics must prove g lies between those cuts. With d=m1-m0,qualified PAL supplies the Q2 upper elapsed bound du=ceil(d*(rho_den+rho_num)/rho_den)+E. If conversion/units/era/leap handling are unambiguous and every arithmetic step is checked,UTC at m1 lies conservatively in [Ls,Us+du]. Lower endpoint uses nonnegative elapsed time,not half RTT. Authentication without these source/causality/PAL facts gives no inclusion proof.

This uses the FULL measured interval,including acquisition,parsing and verification. It remains conservative under arbitrary nonnegative path asymmetry because no symmetric delay estimate is subtracted. Source quantization and calibration must already be included in [Ls,Us];local conversion must widen/reject as needed. Source-native open intervals can be outward-enclosed,never tightened. Preserve native delegation endpoint semantics separately from issuer/X509 [notBefore,notAfter). Overflow,missing E/rho,bad scale or unknown continuity means DENY,not saturation.

For several replies,each has its own bracket and evidence lifetime. Do not collapse times from different local cuts or discard earlier transcripts to claim single-packet storage. Any common-cut conversion/disagreement policy needs proof;no favorable averaging or automatic trusted majority. At effect,require m1<=m<D and unchanged source/root/pin/operation/status. D must be a qualified monotonic applicability cut,not a civil timestamp compared directly to monotonic time or a free TTL. A source-protocol rate model cannot silently replace accepted rho/E orientation.

Stop invalidates effects but retains bytes/resources. Suspension/resume without qualified continuity invalidates the operation. New query cannot extend D;new operation requires new observation. Nothing here creates a reusable software clock,changes the system clock or restores authority after restart.

## Ordered implementation gates

1. Owner reviews R1 versus R3 and the declared trust/compromise/independence model;no provider selection by agent. All candidate documents remain PROPOSED.
2. Archive/review exact official specification,updates/errata,license/provenance and test vectors before freezing an implementation profile. Current browser retrieval verifies relevant sections,not complete parser correctness or an original-byte archive.
3. Select actual source(s) administratively and prove identity,status,calibration/error,independence where required,health and availability. No public examples or runtime-discovered endpoint is enrolled.
4. Resolve canonical root binding/versioned persistence if current fields are insufficient,without changing historical layouts or creating a registry.
5. Fix exact existing-protocol version,transport,algorithms,wire validation,entropy,request/response limits,deadlines,backoff,age/uncertainty policy and status/rotation behavior. Do not substitute existing package algorithms for protocol-required ones or modify PST/PACC wire by reference.
6. Audit backend primitive availability and C89/VC6 APIs on every target;no mandatory Win32/CNG in the core. Qualify PAL rho/E and event continuity;verify the conditional acquisition proof against actual source semantics.
7. Prove full ED11 graph/stack/provider/kernel/coexistence/retention BEFORE source hunks. Current conservative activation scenario exceeds3MiB. No numeric profile reservation or quota increase here.
8. Only after acceptance and exact patch approval,implement the minimal adapter in existing administrative ownership;acquire before E0. Then separately bind issuer/status/U4,CURRENT/predecessor/pins and immediate revalidation. None becomes positive in this task.
9. Future tests must distinguish exact protocol vectors and mathematical simulations from native qualified observations. Cover bad signatures,wrong keys/status,compromise/rotation,replay/causality,delay/asymmetry,scale/overflow,unknown rho/E,rollback/restart/suspension,Stop/retention and ED11. Network/time changes require separate authorization.

## Research limitations

Documentary HTTPS fetches only;no network-operational test/build/time query. Official RFC3161 info page and a subsequent RFC9523 body request returned HTTP429;available primary text/update and official overview still support the limited R2/R6 review. Full errata closure is not claimed. Source-byte hashes were not obtained through the browser;verified-references.json hashes our local review records,not remote RFC files. No target or deployment is qualified.
