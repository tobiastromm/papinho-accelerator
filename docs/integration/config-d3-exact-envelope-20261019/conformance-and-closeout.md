# D3 Revision3 — exact contract preparation closeout

Task scope completed as PROPOSED binary contract only. No ACCEPTED registration, functional patch, build/test/network, native store/CA/certificate creation, CURRENT/epoch or admission change. D2/D4 were not reopened; their exact bytes remain in combined Revision3. Original Revision2 and accepted documents remain unchanged.

## Conformance matrix

| Authority/request | D3 response |
|---|---|
| Accepted papacc.conf N3/N5/N6, ADR0009 | Collection with stable IDs/order/explicit profiles, complete model entries, no singular truncation/default Legacy or alternate authority; explicit new discriminator and bounded versioned layout. |
| Accepted LST1 C1-C5 | LST1 bytes/endian/header/context remain intact; a base-only embedded fragment, absent at bootstrap, never fabricating zero A/P or new candidate authority. Candidate descriptor has its own discriminator. |
| ADR0007/PersistentID | Only valid machine-local uint64 IDs with explicit presence; no index/address/name identity; SELECTED applicability still denied. |
| ADR0028 and provider gate | Transport/admission/auth distinct; CLOSED/certificate identity remain candidate meanings but unsupported by first profile; no credential/grant/provider manufactured. |
| BODY05 | Original1024-byte model once per candidate; descriptor redundancy must match/reject. Original hash over992 retained, additional reference hashes full1024. No code/bytes changed. |
| ROOT06/issuer history | Optional existing prefix carried as immutable issuer anchor/history, not active listener policy. Existing integrity/revision/high-water/events/generation checks unchanged. Adapter origin separation remains required, not assumed implemented. |
| Historical checkpoint | PACICP01 original hash over root16384 and own480 not extended to new bytes. CP8 has a distinct magic, endian and domains. |
| Root/issuer-C2 I/R/A/P | Candidate R/base context/publication CP distinct. Epochs only actual post-commit root facts; no CURRENT positive before mandatory context/ack. No new counter or operational ID. |
| CAS/Stop/Model E/ED08 | Exclusive virgin create or exact prior-image/identity CAS; uncertainty retains refs/handles/packets/credits; no fallback/destructive retry. Existing worker/slots only, Stop independent, no pinned generation overwrite. |
| Checkpoint/fencing | Relative comparisons only, no hash-as-freshness/exclusivity. AUTHORITATIVE_RECOVERY=DENY including valid CP8. |
| ED11 | Exact serialized arithmetic and explicit buffer scenarios; no sizeof/transitive/peak/provider inference or reserve approval; quotas unchanged and UNKNOWN retained. |

No material conflict with accepted authorities identified in the proposed scope. This is normative design review, not executable validation. Proposed limits/magic/offsets are NOT accepted by existing ADRs or by this closeout. A first ROOT8 implementation would reject historical files as UPDATE predecessors; explicit ROOT06 migration and safe issuer adapter integration remain separate denied gates, not auto-bootstrap or production permission. This limitation must not be concealed by owner acceptance of format bytes.

## Source evidence inspected

Canonical main HEAD recorded in final manifest. Existing issuer_codec.c link_root/link_encode/link_decode proves historical ROOT06/checkpoint domains and original big-endian encoding. server_configuration_issuer06_win32.inc proves separate configuration-anchor/issuer validation, immutable slots and native callback ownership. Existing body05 and body04 codecs retain1024-byte model and policy_epoch-zero constraints. Existing pending owner contains three16896-byte arrays and the same worker/credit lifetime, informing the exact delta estimates. Reference historical Model E/C8 documents have isolated scope; canonical consolidation decisions preserve their contracts, not unrestricted old implementation qualification. No network used to access authorities.

## Deliverables

D3-REVISION3-PROPOSED.md is the exact replacement clause; BOUNDARY-REVISION3-PROPOSED.md reconstructs the full proposal; exact-D3-hunks.patch contains only revision metadata and D3 replacement; revision2-preimage.bin and preservation-scope.json prove unaffected clauses. offset-matrix.csv covers header512, record1536 and checkpoint512 without gaps/overlap. ed11-size-estimate.json supplies numeric scenarios; composition-and-implementation-handoff.md contains diagram/compatibility/recovery/implementation sequence. final-manifest.json hashes all artifacts and authorities/preimages. No extra roadmap or architecture is installed.

```ini
CURRENT_TASK_COMPLETED=YES
D3_BYTE_EXACT_LAYOUT=PREPARED_REVISION3_PROPOSED
ROOT_ENVELOPE=PACROOT8_VERSION1_PROPOSED_2592_TO_25792_BYTES
LST1_COMPOSITION=BASE_ONLY_ABSENT_FOR_BOOTSTRAP
CAS_BINDING=EXACT_PREDECESSOR_BYTES_IDENTITY_AND_ROOT_CONTEXT
CHECKPOINT_HASH_DOMAIN=CP8_480_CONTENT_PREFIX_Q_WHOLE_Q_PLUS_512
HISTORICAL_COMPATIBILITY=STRICT_DISPATCH_NO_AUTO_MIGRATION
ED11_ESTIMATE=MAX_25792_SERIALIZED_FOUR_ARRAYS_103168_NOT_RESERVED
NORMATIVE_STATUS=PROPOSED
IMPLEMENTATION_AUTHORIZED=NO
CONFIG_CURRENT_PUBLICATION=DENY
AUTHORITATIVE_RECOVERY=DENY
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
BUILDS=NOT_RUN
OFFLINE_TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=OWNER_ACCEPTANCE_OF_EXACT_D3_PENDING
NEXT=OWNER_ACCEPTANCE_OF_EXACT_D3_AND_CANONICAL_IMPLEMENTATION
```
