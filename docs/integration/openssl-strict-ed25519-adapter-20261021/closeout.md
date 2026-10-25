# OpenSSL strict Ed25519 adapter — handoff only

The minimal existing-arithmetic extension is implemented and passes the scoped
native offline checks in an external evaluation copy. By the owner's explicit
response, only this handoff is recorded in Accelerator: PST sources, upstream
archive and all distributed binaries are unchanged. No adapter is installed
or registered for TIME_SOURCE. Full distribution/operational qualification is
not completed by the isolated object experiment.

[Implementation proof and PST handoff](implementation-proof-and-pst-handoff.md)
| [Validation](validation.json) | [ED11](ed11-census.json)
| [WIP verification](wip-verification.json).

CURRENT_TASK_COMPLETED=NO
OPENSSL_STRICT_ED25519_ADAPTER=IMPLEMENTED_EXTERNAL_OFFLINE_HANDOFF_ONLY
PREDICATE_EQUIVALENCE=PASS_SCOPED_NATIVE_OFFLINE_NOT_FULL_BACKEND_QUALIFICATION
BACKEND_DISTRIBUTION=NOT_AUTHORIZED_OWNER_CHOSE_HANDOFF_ONLY
VC6_COMPATIBILITY=PASS_C89_HEADER_CALLER_ONLY_BACKEND_NT4_NOT_QUALIFIED
ED11_TOTAL_STATUS=PARTIAL
BUILDS=MSVC_DEBUG_RELEASE_PASS_VC6_HEADER_COMPILE_PASS
WARNINGS=0_FINAL_BUILDS_INITIAL_DIAGNOSTICS_PRESERVED
OFFLINE_TESTS=296_FOCAL_2526_CRYPTO_5940_LAB_TRANSCRIPT_PER_CONFIGURATION_PASS
NETWORK_TESTS=NOT_RUN
TIME_SOURCE_KEY_VALIDATION=DENY_UNTIL_QUALIFIED
UTC_SOURCE=NOT_QUALIFIED
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES_PREEXISTING_HASHES_CHECKED
FIRST_UNRESOLVED_DIVERGENCE=OWNER_HANDOFF_ONLY_AND_FULL_BACKEND_DISTRIBUTION_ED11_NOT_QUALIFIED
NEXT=AUTHORIZED_PST_DOWNSTREAM_BUILD_AND_ED11_QUALIFICATION_BEFORE_V4_WORKER

No libsodium, custom curve arithmetic, new CA/root/worker/trust store,
source authority, network, production credentials, quota increase, clock
change, clone/worktree, commit/push/tag/release or production migration.
No functional canonical source or accepted contract changed.
