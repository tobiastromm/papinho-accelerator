# Subsequent owner elevated read-only observation — 2026-10-22

Task PACC-PHASE6-CA-NCRYPT-FINALIZE-NTE-PERM-1. This subsequent observation updates the previously prepared/unexecuted manual step; earlier probes and original statements remain historical.

Owner supplied screenshots of the elevated window and its normal return UAC_READONLY_EXIT=0. Receipt [607304b978194007a40ab1132474878a.json](manual-results/607304b978194007a40ab1132474878a.json), SHA2566f3dbbff73beda7f17ad393fb583b521437cc3f760a8f66b6cb97a7615dbb019, was read and verified against the actual script/executable hashes. Receipt UTC annotation is only a log timestamp, not qualified time evidence.

The effective operator is the expected Tobias SID S-1-5-21-785699550-1440458426-1682014700-1108. Elevated=true, elevation_type2 (Full), Administrators enabled, no impersonation and an interactive console. This is factual elevated read-only qualification, not synthetic eligibility or proof that the previous failed creation used the same token.

The exact previous October22 LAB name was queried in Microsoft Software Key Storage Provider/MACHINE. NCryptOpenKey returned NTE_BAD_KEYSET0x80090016, first failure function2/stage12/API3. Child diagnostic exit4. The provider was opened and then factually closed: stateCLOSED3, close_confirmed1, no key/provider handles owned at the recorded end. query_completed_and_closed=true.

Outer exit0 means the read-only diagnostic/receipt flow completed and the owner closed its result window normally; it does not override the child query failure4 or indicate CA success. The non-elevated and elevated observations agree that this exact name was not openable in their contexts. Under the preserved diagnostic contract this is NOT a proof of global absence or safe name reuse: PREVIOUS_LAB_KEY_DISPOSITION=UNKNOWN.

The immediate lack of elevation is now removed as an explanation for the CURRENT read-only query result. No new creation/finalization was executed, so this does not prove whether elevation would repair the ORIGINAL NCryptFinalizeKey=NTE_PERM. No product patch, permission/ACL/KSP change, key creation/retry/deletion, certificate, bootstrap or CURRENT followed. The receipt and old key names remain preserved. ED11 PARTIAL; all operational authority remains DENY.

CURRENT_TASK_COMPLETED=NO
ELEVATED_READ_ONLY_QUALIFICATION=PASS
OUTER_LAUNCHER_EXIT=0
NATIVE_KEY_QUERY_EXIT=4
NATIVE_KEY_QUERY_STATUS=0x80090016
PREVIOUS_LAB_KEY_DISPOSITION=UNKNOWN
NCRYPT_FINALIZE_CAUSE=UNRESOLVED
KEY_CREATION=NOT_RUN
KEY_DELETION=NOT_RUN
CURRENT_PUBLICATION=DENY
ED11_TOTAL_STATUS=PARTIAL
NEXT=CA_CUSTODY_NATIVE_LAB_QUALIFICATION
