# Issuer root persistence — closeout 2026-10-19

Status: storage integration implemented and offline validated; full CURRENT/EPOCH/live publication and authoritative recovery remain incomplete. No accepted ADR was modified.

## Effective implementation

The existing application owner and PAPACC_CONFIGURATION_COMMIT now expose a dormant issuer persistence adapter. ROOT06 uses the same selected file and existing native transaction: immutable, byte-exact historical BODY0.5 image at bytes 0–8191, followed by an 8192-byte issuer image. Historical codecs and historical file lengths remain intact; old readers reject the extended length. No second root, registry or journal file was introduced.

The portable codec src/issuer/issuer_codec.c uses explicit version/endian/length, append-only transition history, event and image SHA256 integrity, immutable authority/generation/policy binding, serial prefix plus unsigned64 high-water, and certificate digest/length validation. Exhaustion denies; records are not pruned. Hash and persistence are platform adapters; no Win32/CNG headers enter the common codec/core.

Native storage compares the whole prior image and file identity inside the existing TxF transaction, flushes before commit, and rejects stale expected revision/high-water. New hash handles belong to the same root; failed release retains the handle and unresolved disposition. Historical store03 hash-release limitations remain inherited and are not qualified by this work.

## Authority and lifetime boundary

Only the existing owner thread, dormant configuration, factual current root epoch/policy and no outstanding borrowers/retained slots permit staging. A confirmed metadata commit is COMMITTED_PENDING, not normative issuer CURRENT. Production publication of ROOT06 is NOT_SUPPORTED. Root port reads export no proof bits and epoch zero as unavailable authority, not a valid operational epoch. CAS cannot acknowledge readiness to sign: even a confirmed storage commit returns UNCERTAIN until CURRENT/EPOCH acknowledgement exists. Publication requests are denied.

The concrete blocker is server_c2_configuration_associate: an already CURRENT owner accepts only the same revision/epoch. Safe generation/lease transition for live publication has not been demonstrated. Existing Sessions/leases are not invalidated or reassociated. Stop with a pending commit retains the owner; no DESTROYED or credit return is fabricated.

Recovery opens the same file read-only, validates file identity, historical prefix and issuer integrity, restores observational high-water/history and holds the read guard. It returns NOT_SUPPORTED for authoritative recovery. Integrity of a snapshot cannot establish that it is the latest snapshot. Anti-rollback/fencing and complete root CURRENT reconstruction remain required. Valid historical snapshots never grant authority.

## Validation and evidence limits

Final Debug/Release candidate builds include the canonical server and affected components: build5 logs, zero compiler warnings. Earlier failed iterations are preserved. No product listener or network was executed.

Per configuration, final codec checks=70; Stop-before-commit checks=85; portable issuer synthetic checks=117; normal-production adapter rejection checks=4; historical Site TLS codec checks=31. Native component history checks=151 and production publication-denial checks=82 passed in both configurations before the final census/Stop-only test additions. Application-root integration checks=69 passed in both configurations: actual same-owner metadata commit, original epoch preserved, publication/preflight denied, root retained.

Native TxF commit/reopen and SHA256 were exercised on new isolated files. Issuer identities, state histories, DER marker and epochs in component fixtures are synthetic: no signing, CA generation, credential issuance or revocation of real credentials occurred. The component-only publication selector is compiled solely into the isolated storage test; normal root/server targets exclude it. Crash boundaries are modeled through persisted stages and reopen, not an OS crash/power-loss proof. Close-failure dispositions are source/fixture evidence, not injected native kernel failures.

## ED11

Exact preimage/current owner sizeof: 13024 -> 29432 bytes; delta 16408 (8192 committed extension +8192 staging +24 handles/status/alignment). Compiled known managed census=1555728 bytes; limit=2097152. Application fixture combined reservation=1813435 bytes, including existing credits; do not add embedded arrays twice. Static known margin=541424; combined-reservation margin=283717. These are different measures, not operational headroom approval.

Commit previous/next stack buffers each grow from8192 to16384 (+16384 bytes together); root-port codec scratch includes8192 bytes per active frame. Full call-chain stack, backend resources, coexistence and retained-provider maxima are not qualified. Provider/kernel remain UNKNOWN; ED11 remains PARTIAL. No quota was increased.

## Result

CURRENT_TASK_COMPLETED=NO
ISSUER_ROOT_PERSISTENCE=IMPLEMENTED_DORMANT_COMMITTED_PENDING
ISSUER_PRIVATE_CODEC=VERSIONED_IMPLEMENTED
SERIAL_DURABILITY=NATIVE_STORAGE_VALIDATED_SIGN_AUTHORITY_DENIED
ISSUER_RECOVERY=READ_ONLY_HISTORY_VALIDATED_AUTHORITATIVE_RECOVERY_PENDING
REVOCATION_PERSISTENCE=SYNTHETIC_HISTORY_NATIVE_FILE_VALIDATED
LIVE_PUBLICATION=NOT_IMPLEMENTED_DENY
BUILDS=DEBUG_RELEASE_PASS
WARNINGS=0
OFFLINE_TESTS=PASS_SCOPED_NATIVE_STORAGE_AND_SYNTHETIC_ORACLES
NETWORK_TESTS=NOT_RUN
CNG_NATIVE_STATUS=UNRESOLVED
BOX_ISA=NOT_QUALIFIED
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES_PREIMAGES_AND_BASELINE_RECORDED
FIRST_UNRESOLVED_DIVERGENCE=ROOT06_CURRENT_EPOCH_PUBLICATION_AND_AUTHORITATIVE_RECOVERY_NOT_PROVEN
NEXT=INTEGRATE_ISSUER_WITH_WINDOWS_CNG_AND_LIVE_ROOT