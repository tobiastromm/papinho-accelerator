# Pinned Ed25519 backend qualification closeout

Official release provenance was pinned and source-level checks inspected.
The officially approved minimal VC6 compile failed on missing stdint.h;
STOP preserved. No backend selected, installed or integrated. No mathematical
vectors executed and no executable/library linked or run. MSVC/VC6 successful
Debug/Release qualification, exact predicate equivalence and full ED11 remain
pending. This report does not claim libsodium cannot be ported.

[Qualification and focal options](qualification-and-options.md),
[provenance/hashes](provenance-and-source-hashes.json),
[exact command](vc6-compile-gate.cmd), [log](vc6-compile-gate.log).

CURRENT_TASK_COMPLETED=NO
STRICT_ED25519_BACKEND=NOT_QUALIFIED_STOP_VC6
BACKEND_VERSION=LIBSODIUM_1_0_21_CANDIDATE_ONLY
BACKEND_COMMIT=d24faf56214469b354b01c8ba36257e04737101e
BACKEND_SHA256=9e4285c7a419e82dedb0be63a72eea357d6943bc3e28e6735bf600dd4883feaf
PREDICATE_EQUIVALENCE=SOURCE_REVIEW_ONLY_NOT_PROVEN
VC6_COMPATIBILITY=FAIL_UNMODIFIED_SOURCE_STDINT_H_MISSING
ED11_TOTAL_STATUS=PARTIAL
BUILDS=VC6_MINIMAL_COMPILE_FAILED_MSVC_NOT_RUN
WARNINGS=NONE_EMITTED_FATAL_C1083_BUILD_NOT_QUALIFIED
OFFLINE_TESTS=NOT_RUN_STOP_AT_COMPILE_GATE
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
WIP_PRESERVED=YES_EXISTING_HASHES_CHECKED_NO_FUNCTIONAL_WRITES
FIRST_UNRESOLVED_DIVERGENCE=VC6_UNMODIFIED_SOURCE_REQUIRES_STDINT_H
NEXT=REVIEW_VC6_COMPATIBILITY_ONLY_BACKEND_QUALIFICATION_PLAN

No sources, accepted clauses, quotas, Browser/PST/wire or production changed;
no clones/worktrees, commits/push/tags/releases, productive keys/certificates,
clock changes or operational network. Upstream archive and compiler artifacts
stay in the governed external temporary workspace, not a new canonical root.
WIP and LAB untouched. Qualification options are PROPOSED, not accepted.
