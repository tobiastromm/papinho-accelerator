# Owner selection and read-only observations — 2026-10-22

The owner selected .\papinho and %ProgramFiles%\PapinhoAccelerator, and explicitly required optional SCM installation with preserved console execution. See ../../adr/DECISION-personal-windows-optional-scm-20261022.md. This supersedes only the preceding task report's statement that no account/destination name had been supplied; other factual gates remain open.

Read-only observations in the executor's current host context:
- CIM Win32_UserAccount query returned access denied. This is not account absence.
- net user papinho returned success, canonical displayed name Papinho, account active, local group Users (localized output). This is an observation, not proof of exclusive dedication, complete privilege eligibility or future operational token.
- Windows NTAccount using the current machine name resolved Papinho to S-1-5-21-896877160-3262526087-2538948522-1006. This is the account SID in this host context, not an SCM service SID, historical operator SID or qualified TokenUser of a service. Do not carry it to a different host by inference.
- ProgramFiles resolves to C:\Program Files. C:\Program Files\PapinhoAccelerator does not currently exist. No directory was created; no configuration path/file or native installation identity is yet supplied.
- Existing Selected/Release executable --help returned 0 and shows existing direct console CLI. main.c.api3.inc dispatches explicit service commands and otherwise falls through to papacc_server_entry. No SCM is required for help/direct command parsing. Operational runtime still has its existing admission DENY; this is not proof of an operational listener.

No source or accepted prior ADR was changed. No service/account/ACL/password/key/certificate/root/CURRENT was created or changed, no elevation or network used. No new build is needed for this documentation-only update; the preceding four builds and tests remain the implementation evidence. Next factual work: inspect this specific account through a bounded native adapter, protect installation/config/executable references, qualify SCM credential handoff and actual runtime readiness/quiescence. No positive service LAB operation has been authorized by the location/account selection.
