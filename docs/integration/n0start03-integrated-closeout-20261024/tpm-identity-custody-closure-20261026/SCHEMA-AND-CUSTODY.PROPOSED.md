# Q4 candidate component and custody review — NOT ACCEPTED / NOT APPLIED

This exact review is technical preparation under accepted L1/W1. It does not amend ROOT8, issuer events, CURRENT, epochs or any accepted format. The isolated Python codec is not installed in the canonical worker. The existing 88-byte DATA binding remains the sole functional integration. A durable component must pass the existing root participant/worker and format review before implementation/publication; the fixtures do not authorize a second journal.

## Proposed record

`Q4DRAFT1` is an intentionally incompatible candidate marker, version1. Exactly372 bytes: big-endian header52 + nine fixed32-byte binding digests288 + SHA256 checksum32. Header: magic8, version1, state1, reserved-zero2, baseline u64, expected u64, PID u64 (1..UINT32_MAX), creation FILETIME u64 nonzero, first-error u64. State1 PREPARED,2 CONSUMED,3 RESULT,4 UNKNOWN. Expected must be baseline+1 with no overflow. Error codes1..11 correspond in order to IDENTITY, WITNESS_BASELINE, PREPARE_PERSIST, INCREMENT_UNKNOWN, COUNTER_DIVERGED, COMMIT_UNKNOWN, CALLBACK_UNKNOWN, LEASE, WITNESS_CURRENT, STOP, CLOSE_UNKNOWN;255 denotes an unknown model error. These are model error categories, not fabricated native TPM/Windows error codes.

Nine digests bind root identity, exact native predecessor, new approval, historical attempt, authenticated device/generation, current operator/token/boot/live invocation, exact original-object manifest, source/dependencies, and custody designation. Each digest is nonzero32 bytes. The underlying typed manifests must retain native volume/FileID, original handles, ordered RAW1 bytes, hashes/sizes and provenance. Hashing a manifest is not the original-handle proof. Existing owner/pin/current-state checks remain necessary. The checksum detects corruption only; it is not authenticity, rollback protection or authority. No accepted schema is silently extended.372 is serialized candidate size, not a RAM/stack/managed heap bound or ED11 credit.

## Worker protocol and failure review

1. Existing root owner holds its governed exclusive/live lease; obtain and validate original root/native predecessor, approved request, operator/token, actual device generation and original object proofs. No borrowed handle escapes its owner.
2. Same-worker exact-native-preimage CAS persists PREPARED before irreversible witness consumption. Any failure or Stop before increment denies without a hardware write. Tested Stop from inside the prepare callback is rechecked before increment.
3. Revalidate authenticated device/index public area/Name, credential and expected baseline. Increment once. A TPM counter is not CAS: root serialization and all-authorized-writer exclusion are prerequisites. A process-local Python lock is not that exclusion.
4. Ambiguous response, intervening increment, unknown close, wrong identity or persist failure freezes; preserve first failure and dependencies, never resend. No adoption/rebaseline.
5. Same-owner CAS records CONSUMED, then revalidates lease/context/object/witness immediately before the read. Stop during commit cannot grant read. Only original PID/creation live invocation can use one model read slot. Restarted/querying CONSUMED is always denied.
6. Persist RESULT/UNKNOWN using the reviewed worker; loss of result never refunds. FINAL RESULT publication is not implemented by this prototype. Query-only reconciliation cannot grant a new invocation.

The current implementation is a state/codec fixture with injected persistence, not durable disk publication. Its simulated witness is assumed independently retained. The test deliberately demonstrates that restoring both simulated witness and ledger evades detection; native independent continuity still needs evidence. No claim of arbitrary anti-rollback, hypervisor resistance, power-loss proof or deployment.

## Concrete custody proposal — operational designation NOT GRANTED

| Capability | Proposed holder and constraint |
|---|---|
| Define candidate index | Owner-approved elevated provisioning ceremony using authorized OWNER hierarchy access; administrator membership/TBS access alone is insufficient. Actual authorization availability remains UNKNOWN. No extraction, empty fallback, guessing, hierarchy change or retries. |
| Increment | Existing canonical owner/worker, exact approved purpose/root/device/index binding, live lease, no impersonation, current TokenId/AuthId/ModifiedId revalidation, parameter-protected authenticated session. AUTHWRITE grants whoever possesses auth; the OS/user custody boundary must be explicitly accepted, not advertised as TPM-enforced process isolation. |
| Read public area | Supported read-only TBS access; no ownership/consumption inferred. |
| Read counter value | AUTHREAD with the same designated per-index credential/session; not an unauthenticated public read. No extra ADMINREAD/OWNERREAD permission added silently. |
| Per-index secret | Future32 random bytes only at separately authorized provisioning, kept in memory under owner lifetime. Never argv/log/report/chat/plain evidence. No actual secret generated here. |
| At-rest secret | Proposed CurrentUser DPAPI ciphertext within the SAME reviewed root participant with exact native RAW1 custody; authenticated entropy binds actual root/device generation/index/public profile/purpose/custodian. Existing synthetic10-process tests are reused, not real custody proof. No second vault/authority/store. |

The observed operator SID is a candidate custodian identity only, not appointment by observation. CurrentUser DPAPI does not exclude other software as that user or a privileged attacker, does not authenticate hardware and does not resist local blob rollback. Binding entropy is public context, not secret authorization. Custody review must explicitly delimit these trusted actors. Same-root publication/native RAW1/foreign-user rejection and actual credential recovery after restart have not been demonstrated.

Loss of profile/master protection/credential, TPM replacement/Clear, index disappearance/redefinition, untrusted generation or unavailable hierarchy access => DENY/UNKNOWN and preserve reservations; no automatic recreation, adoption or refund. Same EK after restart does not prove uninterrupted NV provisioning generation. Keep original dependencies until factual close; managed buffer clearing is best effort, not guaranteed physical erasure. NO_DA is clear in0x00040014: real authorization errors can affect lockout, so no guessing or retry.
