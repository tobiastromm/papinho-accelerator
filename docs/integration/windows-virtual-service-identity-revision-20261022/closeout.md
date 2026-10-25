# Windows virtual service identity — closeout, 2026-10-22

Owner-selected direction reviewed and accepted after official exact document-write approval. [Normative V0–V8](../../adr/DECISION-windows-virtual-service-identity-20261022.md); [preserved proposal](VIRTUAL-SERVICE-IDENTITY-PROPOSED.md); [conformance, official references, source handoff, test design and ED11](review-and-handoff.md). verification.json records source/authority preimages and postimage/body hashes. Original accepted decisions and proposals preserved.

This completes the normative review, not a functional adapter or target qualification. Windows7+ is the selected backend floor, not a tested-target assertion. Set logon account and service SID policy separately; verify actual token and ACLs. No password or manual SAM account is required for the chosen profile. No positive mutation, real service lifecycle or new code tests occurred. Operational setup still requires protected installation binding, native configuration/token/ACL readback, same-runtime readiness/final teardown receipts and ED11 proof. Functional authorization remains separate under request section9; no patch applied. Positive LAB installation is a further exact approval gate.

CURRENT_TASK_COMPLETED=YES
WINDOWS_SERVICE_IDENTITY_CONTRACT=ACCEPTED
WINDOWS_SERVICE_MIN_VERSION=7
VIRTUAL_SERVICE_ACCOUNT=NT_SERVICE_PapinhoAccelerator_SELECTED_NOT_IMPLEMENTED
SERVICE_SID=UNRESTRICTED_SELECTED_DISTINCT_FROM_LOGON_SELECTION_TOKEN_NOT_QUALIFIED
ACCOUNT_CREATION_REQUIRED=NO
SERVICE_PASSWORD_REQUIRED=NO
OPTIONAL_SCM=PRESERVED
DIRECT_EXECUTION=PRESERVED_WITH_EXISTING_ADMISSION_AND_ISOLATION_GATES
CA_CUSTODY_ACCESS=PRIVATE_OPEN_USE_SIGN_EXPORT_DENY_TO_OPERATIONAL_PRINCIPAL
SCM_IMPLEMENTATION_READINESS=DOCUMENTED_API_PATH_SOURCE_AUTHORIZATION_AND_FACTUAL_GATES_PENDING
IMPLEMENTATION_AUTHORIZED=NO_UNTIL_SEPARATE_EXPLICIT_FUNCTIONAL_APPROVAL
BUILDS=NOT_RUN_DOCUMENTATION_ONLY
TESTS=DESIGN_PREPARED_NOT_EXECUTED
ED11_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=SCOPED_BASELINE_HASHES_VERIFIED_ONLY_EXPECTED_LIVE_APPEND
FIRST_UNRESOLVED_DIVERGENCE=SOURCE_STILL_LOCAL_ACCOUNT_PARSE_AND_NO_CREATESERVICE_VIRTUAL_BINDING
NEXT=WINDOWS_VIRTUAL_SERVICE_ACCOUNT_SCM_INTEGRATION
