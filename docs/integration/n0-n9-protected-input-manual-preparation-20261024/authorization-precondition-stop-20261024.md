# N2P-20261024-01 — owner authorized, stronger protection precondition STOP

Owner authorized ONE manual execution of pinned script3968ed0d381bf2468867b69f035d04e2ce21810717ba8b883076db9ca2db1c4c and confirmation PLACE_N2_INPUTS N2P-20261024-01 75330d11cc7e4123ae62eadc9a59c2f5, conditional on effective protection against substitution/deletion/rename including parent-directory rights. No real creation/Place was run by Codex. Manual execution is now BLOCKED before effect by that condition; this is NOT a failed native placement or a request to repeat it.

[Read-only ancestor security evidence](authorization-ancestor-security-readonly.json) identifies the missing protection. Selected, its containing installation build directory and build grant Authenticated Users an effective ALLOW0x001301bf, including DELETE0x00010000 and directory creation/write rights, with no corresponding DENY in the observed DACLs. Those three directories also have owner SID S-1-5-21-896877160-3262526087-2538948522-1003, not approved operator1108/SYSTEM/Administrators. Project root and C:/Projetos also have effective AU Modify entries. Release itself remains owner1108/protected and its unsafe AU write ACE is inherit-only, but that fact does not protect movable ancestors after their handles close.

The immediate Release parent does NOT show an effective untrusted FILE_DELETE_CHILD grant. The demonstrated issue is DELETE on an ancestor object and writable/recreatable containing topology, plus unapproved ancestor ownership. Do not misreport0x001301bf as FILE_DELETE_CHILD: bit0x40 is not set. No actual destructive rename/delete/alternate-token AccessCheck test was performed. These positive untrusted grants/unapproved owners are sufficient to withhold the REQUIRED protection proof; no successful attack execution is claimed.

The pinned script locks all path components with no write/delete sharing DURING its live routine, then intentionally closes those references. It does not persistently secure their ACLs/ownership or qualify post-close topology integrity. Its review already marked chain-lock/native-positive behavior and inherited OS/admin assumptions as limitations. Under the owner's new explicit durable protection condition, temporary locks cannot be promoted to persistent protection. The exact script must NOT be run unchanged as if that condition were satisfied; no code/pin is silently updated under the old approval.

Current task only read script hash and ACLs; no new recorder, native placement, preflight, Start/Stop, SCM, ACL/owner remediation, file delete/rename or retry. Existing two protected input leaves and administrative receipt are absent at observation; no new native input IDs or cleanup receipt is fabricated. Do not consume or reuse another operation receipt as authority. Original inputs5358/326 and hashes, descriptor0fcfd057..., source pins, WIP and accepted records remain unchanged. ED11 PARTIAL; authorities DENY; production0.6.4/PST/Browser/wire unchanged.

Minimum next decision: separately review a destination with a qualified protected ancestor chain, or an explicitly scoped ancestor-protection plan; do not modify the shared parent/ancestors or create another environment by convenience. Neither is authorized here. Any changed placement procedure/target needs new exact review/pins/approval. Preserve STOP until the ownership/delete/rename/substitution property is proven.

OPERATION=N2P-20261024-01
OWNER_AUTHORIZATION=RECEIVED_CONDITIONAL_MANUAL_ONLY
SCRIPT_HASH=VERIFIED_UNCHANGED
DESTINATION_PROTECTION=STOP_ANCESTOR_DELETE_WRITE_AND_UNAPPROVED_OWNERS
PLACE_EXECUTED=NO
PROTECTED_FILES_CREATED=NO
PREFLIGHT_EXECUTED=NO
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
RETRY=NOT_RUN
PINS=UNCHANGED_EMPTY_DENY
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=PROTECTED_ANCESTOR_BOUNDARY_DECISION_BEFORE_MANUAL_PLACEMENT
