# Byte limits, security and rationale (PROPOSED)

The PT profile uses the unmodified RFC packet, not a new envelope. Official reference: https://www.rfc-editor.org/rfc/rfc10049.html . The exact local restrictions below are project choices, not claims that RFC requires these maxima. Bounds can reject a valid packet and therefore reduce availability; no fallback is enabled to hide that tradeoff.

## Request instantiation

Offsets are zero-based octets from the start of the 2048-byte UDP payload. All integer fields retain RFC little-endian encoding. Outer magic at [0,8), message length at [8,12) is 2036. Map N at [12,16) is 5. Four relative value offsets at [16,32) are 4,36,68,72. Five tags at [32,52) are VER,SRV,NONC,TYPE,ZZZZ in numeric order. Values start at 52: VER [52,56) is uint32 1; SRV [56,88) is the RFC long-term-key identifier; NONC [88,120) is the 32-byte campaign nonce; TYPE [120,124) is uint32 0; ZZZZ [124,2048) is 1924 zero octets. Neither a private header nor additional fields are present. No request reconstruction may replace retained transmitted bytes for proof verification.

## Local limits

| Item | Exact ceiling |
|---|---:|
| Sources / independent parties | 3 / 3 |
| Queries / sends per campaign | 6 / 6 |
| Campaigns alive including retained | 1 |
| Outstanding exchange | 1 |
| Request / response bytes, outer header included | 2048 / 2048 |
| Parsed maps / depth / entries per map / total entries | 4 / 3 / 32 / 128 |
| PATH nodes / bytes | 32 / 1024, also packet bounded |
| Stored request slots / response slots | 6 / 6 |
| Packet transcript bytes | 24576 |
| Source descriptor bytes total | 1536 |
| Parser / crypto / control workspace bytes | 4096 / 16384 / 4096 |
| Local stack budget for new adapter call chain | 8192 |
| Local owned transport/control buffers budget | 4096 |
| Width / post-verification age cap | 30 UTC seconds / strictly less than 1 PAL second |
| Send / response from send / exchange / campaign deadlines | 1 / 3 / 6 / 60 PAL seconds |
| Campaign start spacing | at least 60 PAL seconds |
| Failed campaign retry interval | 1,2,4,...,86400 PAL seconds, capped |

No finite wall-time bound is asserted for uncertain completion/retention: it is event-bounded, occupies the sole slot, stays charged, and blocks replacement until factual quiescence. Stop is nonblocking. No preemption, crypto completion, cancellation success or memory release is inferred from deadline expiry.

## Security and availability rationale

Three distinct administrative parties and six chained exchanges preserve the selected campaign model; no actual provider is authorized. The at-least-one-correct-source premise and calibration remain independent factual requirements. A hull prevents a bad source from narrowing a correct interval, while width and causal checks fail closed; denial of service by a bad source remains possible. This is intentional, not a claim that signatures determine truth.

2048-byte requests allow bounded proof headroom beyond the customary smaller request while keeping all six transcripts finite. UDP may fail on constrained/fragment-filtered paths; this profile returns unsupported rather than silently adding TCP. Deadlines and spacing restrict administrative resource occupation and request load; they are policy caps, not measurements of deployed capacity. The width/age caps strictly reject imprecise/stale observations and never widen accepted certificate validity.

Bare-key bootstrap avoids a circular certificate-date check, but still needs independent operator authorization, trust/status evidence and factual CURRENT. Temporary key delegation is protocol evidence only. No new time-trust record is implemented or serialized. A missing canonical representation requires a separate versioned layout decision, not inferred spare fields.

## Review limitations

No builds, native qualification or operational network were run. The official errata search was unavailable through documentary browsing; exact errata disposition remains a named acceptance-review limitation, not a default of no errata. Q accepted clauses and their required rho/E/source/current premises are unchanged. All new numbers and trust choices remain PROPOSED. No platform is qualified by C89-compatible prose.
