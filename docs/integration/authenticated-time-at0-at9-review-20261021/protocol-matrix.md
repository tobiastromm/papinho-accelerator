# Verified protocol evidence and Q0-Q7 gaps

Research date:2026-10-21. Only official RFC Editor/IETF document retrieval was used;no time service was queried. Normative references are external constraints to evaluate,not newly accepted project contracts. Relevant sections,not a complete implementation certification,were checked. See verified-references.json for scope and retrieval limits.

| ID / official reference | Verified protocol facts | Project assessment (inference,not source qualification) |
|---|---|---|
| R1:[RFC10049](https://www.rfc-editor.org/rfc/rfc10049.html),Experimental,October2026;sections3-6,8-9 | Prior long-term key;Ed25519,32-byte nonce,Merkle/delegation checks;MIDP/RADI use seconds. Section8.2 describes three independent servers queried twice with chaining. Section5.4 separates validity from clock correctness. | Best first B candidate for profile study,not a selected provider. Preserve its security model and reject legacy draft/microsecond assumptions. No local PAL or deployment proof supplied. |
| R2:[RFC3161](https://www.rfc-editor.org/rfc/rfc3161.html),sections1,2.1,2.4;[RFC5816](https://www.rfc-editor.org/rfc/rfc5816.html),section2 | TSP timestamps a data imprint;genTime/accuracy and request nonce are relevant. Accuracy is optional;policy can supply it. A sent nonce must match the response. RFC5816 updates signer-certificate identification. | Document existence is not current UTC at receipt. Require finite authenticated error,causal fresh request binding and trust/credential validity. Generic token import cannot qualify a new boundary. CMS/ASN.1/certificate processing needs bounds. |
| R3:[RFC8915](https://www.rfc-editor.org/rfc/rfc8915.html),sections3-5,8.5-8.7 | NTS uses TLS key establishment plus authenticated NTP exchanges. TLS1.3 minimum. Certificate-time bootstrap and delay attacks are explicitly discussed;automatic stripping to unauthenticated NTP is a risk. | Viable alternative,not automatically Q0-Q7. Do not adopt relaxed certificate-time checks contrary to project authority. Need independent initial trust/time,source error and qualified local duration. Traffic keys/cookies/TLS are not a portable signed UTC token. |
| R4:[RFC9334](https://www.rfc-editor.org/rfc/rfc9334.html),sections10.1-10.4,AppendixA | RATS distinguishes timestamp/nonce freshness. A nonce can date signing while underlying claims predate it. | Attestation freshness cannot establish fresh UTC accuracy without additional qualified claims/source semantics. No general attestation system proposed. |
| R5:[RFC9711](https://www.rfc-editor.org/rfc/rfc9711.html),sections4.2.11-13,4.3.1 | EAT includes time/boot-related claims;claims can be older than token creation. | A signed token/boot marker is not by itself a calibrated time source or root fencing. No custom EAT time profile is invented here. |
| R6:[RFC9523](https://www.rfc-editor.org/rfc/rfc9523.html),abstract/overview | Khronos is a monitoring/selection companion for NTP with many-source assumptions and possible clock discipline. | Not the smallest fit for this no-new-clock-service task;not an authenticated observation primitive or a replacement for Q2. Detailed implementation review deferred. |
| R7:authenticated local device | No concrete device protocol/calibration/trust documentation identified. | Keep as deployment alternative only;physical locality or attestation does not establish UTC correctness. |

## Common quality matrix (project requirements)

| Q0-Q7 obligation | What must be shown before ANY candidate can be positive |
|---|---|
| Instant / source identity / purpose | Governed source instance/key/configuration and operation association,not a reply-supplied anchor |
| Uncertainty / resolution | Finite source error at the causal reading instant,with all scale,quantization and acquisition contributions |
| rho and E | Target PAL qualified bounds including zero measured duration;signature/TLS does not supply host drift or read error |
| Maximum age / D | Source/qualification lifetime and immediately pre-effect check;no arbitrary application TTL or renewed cache |
| Replay | Correct existing-protocol correlation plus current operation lifetime and qualified entropy |
| Rollback / restart | Existing owner history,invalidation and independent CURRENT/fencing;no archive restoration |
| Suspension / continuity | Qualified event coverage or denial across the event |
| Offline | Same-boundary bounded propagation only;fresh authorizing boundary still needs fresh evidence |
| Status / revocation | Independently protected applicable trust status;absence/unknown does not turn into success |
| Stop / retention | Same owner remains responsible until factual quiescence/closure |

No reviewed protocol supplies all these deployment facts automatically. Agreement among sources is not a mathematical proof of absolute accuracy without an approved fault/trust model. No maximum-age,uncertainty,concurrency or memory quota is assigned from the existence of an RFC. No NT4/box/ISA/library compatibility claim is made.

## Recommendation

Continue the owner preference B by studying R1 first,including its complete acquisition/compromise model. Keep R3 as the operational alternative if an actual qualified deployment and reusable backend makes it preferable. R2 is conditional evidence,not a drop-in now-clock. R4/R5 help analyze freshness but do not create a UTC authority. No vendor,endpoint,key list or operational protocol has been selected. Do not start a combined multi-protocol synchronizer by default.
