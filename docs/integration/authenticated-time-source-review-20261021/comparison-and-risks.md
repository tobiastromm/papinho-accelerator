# Authenticated time:comparison and risks

Recommendation:B,an EXISTING signed-observation protocol on demand,as an architecture direction for owner review. It may avoid embedding a synchronizer and permits independently inspectable evidence without preserving traffic secrets for audit. It is not automatically cheaper/qualified. NTS can also be used on demand with conservative bounds;continuous internet or a background daemon is not intrinsic to either. No provider/source is selected/contacted.

## Evidence limits

Q0-Q7,local PAL/issuer code and prior Windows investigation were read;prior22-entry manifest verified. No local primary NTS/signed-time specification or authenticated hardware source contract was found in relevant project docs. External references are named for future review,not represented as fetched/read/verified now. Comparisons describe architectural categories and conditional requirements,not exact contemporary algorithms,wire versions,library support or clauses of inaccessible specifications.

## Matrix

| Criterion | A:NTS | B:existing signed observation | C:authenticated local device | D:managed OS/HTTPS |
|---|---|---|---|---|
| Authenticity | Authenticate key establishment/server and time exchanges;exact profile needed | Verify signature with independent purpose-bound trust | Authenticate device/interface/reading;presence alone insufficient | Channel/service identity is not UTC quality |
| Identity/bootstrap | Prior credential/endpoint trust;TLS date bootstrap can be circular | Prior key/delegation/status trust;may avoid live TLS dependence,profile specific | Prior device provisioning/custody/calibration | OS configuration alone does not close Q2 |
| Absolute uncertainty | Source error plus conservative acquisition delay required | Signed error still needs qualified accuracy and causal receipt bound | Initial UTC,environment/calibration/holdover error needed | Finite bound not proved in installation |
| rho/PAL E | Network auth supplies neither local bound | Same local PAL qualification | Device oscillator does not qualify host PAL | QPC frequency/unit supplies neither |
| Replay | Fresh authenticated request/session binding;verify exact mechanism | Fresh challenge,signed request relation and causal time capture | Qualified challenge/counter/reset semantics | No applicable evidence producer today |
| Delay/asymmetry | Authentication cannot prevent delay;no half RTT assumption | Full acquisition delay may widen interval | Device/bus delay also counted | HTTPS latency does not certify UTC |
| Rollback | Qualified source plus owner history still needed | Old/future signed tokens cannot bypass history | Anti-rollback device state must be proved;UTC correctness separate | Manual steps remain unqualified |
| Suspension | Target continuity or invalidate | Same | Device may continue;host mapping still must be proved | No qualified adapter proof |
| Restart | Fresh source acquisition;root/fencing separate | Signature archive never restores authority | Battery/counter is not CURRENT/latest root proof | Time service does not restore authority |
| Offline | LAN/local source possible;bounded same-operation use | Same;old file cannot start a new positive operation | Autonomous if concretely qualified | Local availability is not qualification |
| Internet | Continuous internet not inherent | Continuous internet not inherent | No internet for reading;initial upstream accuracy still matters | Neither sufficient nor mandatory |
| Portability | Protocol/backend/transport libraries need target proof | Portable parser/verifier,platform crypto adapters;support unknown | Driver/backend specific,portable Q1 output | Windows implementation cannot become core semantics |
| Implementation | TLS/key establishment,authenticated datagrams,state/cookies and parsers | Signature/proof/parser/entropy plus transport;ASN.1/proof complexity varies | Procurement,driver,calibration/tamper/holdover qualification | Small apparent change but mandatory evidence fails |
| Dependencies | PST reuse requires capability/profile review | Exact primitives must exist;no substitute algorithms | Vendor/device/backend guarantees | Substantial service qualification may still be needed |
| ED11 | TLS/AEAD/certs/datagrams/state/retention:UNKNOWN | Signature/proofs/certs/buffers/retention:UNKNOWN | Device/provider/kernel/buffers/retention:UNKNOWN | External service cost cannot be treated as zero |
| Readiness | Not implemented/qualified | Candidate only | No concrete qualified device identified | Windows local source stays NOT_SUPPORTED |

## Other candidates

Primary references to acquire later: RFC8915 (NTS),RFC5905 (NTP timestamp/scale context),RFC3161 (timestamp protocol). Roughtime is a candidate signed-time family to investigate;no current standard status/version is asserted here. Timestamp tokens are not automatically an interactive UTC source:a concrete profile must require appropriate challenge association,fresh capture semantics,finite signed error,identity/status and limits. No generic TSA response is assumed sufficient. Listing references selects no protocol.

A custom signed JSON envelope is not recommended:it would create new canonicalization/signature/replay/protocol obligations. HTTPS Date or authenticated TLS alone does not certify UTC accuracy. Existing authenticated OS/domain time can be reconsidered only if it exposes Q2 evidence;neither Type=NTP nor presumed domain membership is proof. Bare RTC,GNSS/PPS,TPM clock or secure-boot attestation alone is not authenticated civil UTC:initial accuracy,spoofing/tamper,calibration and host continuity still matter. ISA portability does not select an ISA time board;hardware possibility is not implementation.

## Risks and controls

1. An authenticated faulty/malicious source can assert false UTC/error. Require independently approved source authority,qualified accuracy,health and status;signature success is insufficient.
2. Bootstrap circularity can make the source prove its own trust. Require independent credential/validity bootstrap;no TLS date bypass,key from reply or reuse of PACKAGE_SIGNING as time authority. Canonical source-binding storage is still unproved.
3. Network messages can be delayed. Use a causal conservative full-delay envelope,not unjustified path symmetry. Wide uncertainty denies effects instead of extending certificate validity.
4. A signed nonce is not necessarily a freshly sampled timestamp. Review exact signed domain and capture semantics;clone/restart/entropy rollback need separate replay/fencing analysis.
5. Offline cache can become an unauthorized clock. Q3 only allows same-boundary/operation propagation;new authorizing boundaries need fresh observations. D is not renewed by query/export.
6. Era/leap/smear/units can introduce false precision. Require exact conservative mapping and overflow rejection.
7. Disagreeing sources cannot be averaged or selectively chosen to favor an effect. Multi-source trust/fault policy needs independent review;no silent fallback.
8. Stop may leave network/device resources retained. Same-owner factual completion/quiescence/closure required;timeout is not release. Administrative source traffic grants no client Network Egress/admission.
9. Full activation already exceeds3MiB. Nothing is free because an implementation/service/device is external. Preserve DENY pending measured lifecycle/capacity.

No functional patch,build,test,network,time setter or protocol-validation run was performed. Earlier native results are history,not rerun or promoted to qualification.
