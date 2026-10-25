# Virtual service SID — independent API/ordering diagnosis

Historical owner receipt manual-01c8f12acad745dd9ca957310913e72f.json proves eligible elevated SID ...1108, console true, parent PASS, product ACL_IDENTITY denial/closed resources, and separate .NET name-translation exception. The product did NOT reach CreateService or its post-create virtual readback. An ACL error therefore does not explain the separate name lookup exception, nor vice versa.

## API and exact evidence

The launcher calls .NET Framework NTAccount("NT SERVICE","PapinhoAccelerator").Translate(SecurityIdentifier), not the product LookupAccountNameW call. Microsoft Reference Source shows NTAccount opens local LSA policy with POLICY_LOOKUP_NAMES and, on supported systems, calls LsaLookupNames2(flags0,count1); error handling maps NTSTATUS to Win32. The original receipt stored only the exception message, not numeric NTSTATUS/GetLastError. Its historical exact numeric status cannot be claimed from that message alone.

Discriminating read-only repro (lsa-readonly.json and approved outside-sandbox lsa-external-readonly.json) uses explicit LsaOpenPolicy(NULL,POLICY_LOOKUP_NAMES) and LsaLookupNames2 with Flags0, Count1 and exact name. Target: NTSTATUS0xC000018D, mapped Win321789 ERROR_TRUSTED_RELATIONSHIP_FAILURE. Existing EventLog control: status0. The external query ran as the actual owner SID ...1108; no interactive/elevated ceremony replay. All LSA result allocations and policy handles were factually freed/closed.

Direct LookupAccountNameW with68-byte SID/256-WCHAR domain buffers and name NT SERVICE\PapinhoAccelerator returns1789, for BOTH lpSystemName=NULL and literal dot. EventLog succeeds with BOTH, 68-byte SID, domain NT SERVICE, SID_NAME_USE5. Thus changing NULL to dot is not a demonstrated correction. NULL lookup may search trusted domains after a local miss; a domain-shaped error is not proof that this application's chosen virtual account or all domain trust is broken.

SC query PapinhoAccelerator gives ERROR_SERVICE_DOES_NOT_EXIST1060; EventLog is an existing control. SC showsid PapinhoAccelerator returns S-1-5-80-3246711204-3501097961-2221176737-2866879599-4054174099, status inactive; EventLog returns its expected SID and active. showsid computes/reports the name-derived service SID without proving registration, configured logon, effective TokenUser, SID type or ACL eligibility. It is a supported candidate-preparation observation only, not a substitute factual identity or authority. No service was created to obtain it and no private SID hash implementation was written.

## Cause and remaining uncertainty

Demonstrated: target SCM registration is absent, its local name mapping fails, an installed control maps through both APIs, the derived target SID can be displayed, and the same LSA failure reproduces outside the sandbox. A preparation-time assumption that the virtual account must already resolve is unsupported on this observed unregistered target; registration/name-mapping qualification must be separate from read-only DATA preparation.

The Microsoft API contracts document service-name/SID conversion, but the reviewed pages do not provide a universal necessary-and-sufficient theorem that every unregistered name fails or that successful CreateService alone guarantees mapping on every target. No new service may be created to prove causality here. The lower-level reason this host reports trust failure instead of NONE_MAPPED for this absent mapping remains unproven; registration dependency is strongly supported, not claimed as a completed after-registration test. No machine-domain repair, secure-channel reset, ACL/UAC/SAM/provider changes or old Windows7 hotfix is justified. The historical Win7/Server2008R2 KB documenting1789 does not establish that its OS-specific bug applies here.

## Additional demonstrated native buffer-contract defect

sid-buffer-contract.json records two discriminating successful LookupAccountNameW calls for existing EventLog. With input SID capacity68, success returns cbSid68 while GetLengthSid is32. With input capacity32, both are32. The API documents cbSid as input buffer capacity and its required-size output on insufficient-buffer failure; equality with actual SID length is not guaranteed by successful lookup.

ps_virtual_readback currently rejects sidn!=GetLengthSid(sid), therefore rejects the valid68-capacity control even after successful lookup. This is an actual native adapter check defect, independent of the historical 1789 failure (which occurs earlier). Its SID namespace/subauthority checks remain appropriate; no service/SYSTEM identity fallback is proposed.

sid-native-buffer-length-PROPOSED.patch changes only that exact equality to the bounded condition GetLengthSid(sid)>sidn (reject), retains fixed-buffer upper bound, IsValidSid and namespace/subauthority checks, and stores native.sid_size=GetLengthSid(sid) rather than capacity. It changes no API, CreateService order, authority gate or quota. Patch is NOT applied. Later approval must test a valid32-byte SID in68-byte capacity, exact capacity, malformed/oversize SID, returned length smaller than actual, namespace mismatch, NULL/lookup error and retention/Stop; existing model lookup stubs must not always collapse capacity to length and hide this case.

## Existing adapter and minimum correction

The product private ps_install_virtual order already is CreateServiceW(explicit virtual account,passwordNULL) -> configure UNRESTRICTED -> QueryServiceConfig/2 readback -> LookupAccountNameW -> SID shape validation. It performs lookup AFTER successful creation and does not require this lookup before the create primitive. API and sequence align with accepted V2 and Microsoft SERVICE_SID_INFO. It still cannot run positively because authorization/binding gates stay denied. API selection and phase order are appropriate, but a separate size-contract defect is demonstrated below; no replacement API or automatic patch is justified.

The qualification wrapper, in contrast, ANDs pre-registration SID translation with its read-only data result. Minimum safe diagnostic correction: report read-only bound/closed DATA separately from name resolution/full SID qualification; label unresolved mapping BEFORE_SCM_REGISTRATION_PENDING, not identity fallback. sid-diagnostic-split-PROPOSED.patch is exact, unapplied, against the pinned launcher; adds separate read_only_data_pass and SID phase fields while preserving current full qualification/exit denial when SID unresolved. It removes no authority gate. A future change to treat DATA preparation exit independently requires explicit review of result semantics, not silent promotion to full qualification.

Correct future sequence, only after separate effect authorization and safe file binding: prepare canonical name/virtual-account plan and optional derived SID CANDIDATE -> authorized exclusive SCM registration -> configure SID type -> native configured-account/SID readback -> actual service process/token and per-resource ACL proof -> runtime admission. Preserve Stop, partial effect UNKNOWN, no retry/delete fallback. Failure after registration retains state; do not substitute sc showsid, SYSTEM, LocalService, domain account or a SID string for factual resolution/token proof.

No functional patch, ACL or SCM effect applied in this task. Next discriminating name-mapping test belongs to the separately approved registration LAB ceremony, not another identical absent-name query now. A focused manual API-only observation can be prepared if a changed hypothesis/context justifies it; elevation alone has not been proposed as a fix.

## Official references

- https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-lookupaccountnamew
- https://learn.microsoft.com/en-us/windows/win32/api/winsvc/ns-winsvc-service_sid_info
- https://learn.microsoft.com/en-us/windows/win32/api/ntsecapi/nf-ntsecapi-lsalookupnames2
- https://raw.githubusercontent.com/microsoft/referencesource/main/mscorlib/system/security/principal/ntaccount.cs
- https://learn.microsoft.com/en-us/windows/win32/services/service-user-accounts
- https://learn.microsoft.com/en-us/windows/win32/services/configuring-a-service-using-sc
- https://support.microsoft.com/en-au/topic/error-1789-when-you-use-the-lookupaccountname-function-on-a-computer-that-is-running-windows-7-or-windows-server-2008-r2-9dab7cb3-b7d7-604b-694a-c72350c35376

Primary sources only. No inaccessible search result or third-party explanation is treated as authority.
