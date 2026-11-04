# Primary sources checked 2026-10-26

* [Microsoft Tbsi_GetDeviceInfo](https://learn.microsoft.com/en-us/windows/win32/api/tbs/nf-tbs-tbsi_getdeviceinfo): device version query and error-return contract.
* [Microsoft TPM_DEVICE_INFO](https://learn.microsoft.com/en-us/windows/win32/api/tbs/ns-tbs-tpm_device_info): four UINT32 layout, separate struct/TPM versions.
* [Microsoft TBS command blocking](https://learn.microsoft.com/en-us/windows/win32/tbs/command-blocking): software access is restricted; Windows10 1809+ TPM2 allowed commands are fixed in driver. No policy modification performed.
* [Microsoft TPM reference constants](https://raw.githubusercontent.com/microsoft/ms-tpm-20-ref/main/TPMCmd/tpm/include/public/TpmTypes.h): property0x116 NV_COUNTERS_MAX;0x20a NV_COUNTERS;0x20b NV_COUNTERS_AVAIL. Actual reported quantities are observations, not reserved capacity or granted index authority.
* [Microsoft reference NV_Increment](https://raw.githubusercontent.com/microsoft/ms-tpm-20-ref/main/TPMCmd/tpm/src/command/NVStorage/NV_Increment.c): access checks, counter type and NV error paths; orderly update behavior needs explicit review. Not a per-request CAS API.
* [Microsoft reference Clear](https://raw.githubusercontent.com/microsoft/ms-tpm-20-ref/main/TPMCmd/tpm/src/command/Hierarchy/Clear.c) and [NV_UndefineSpace](https://raw.githubusercontent.com/microsoft/ms-tpm-20-ref/main/TPMCmd/tpm/src/command/NVStorage/NV_UndefineSpace.c): destructive/reset and deletion paths must be excluded/detected; a version query is not lifecycle continuity.

TCG PDF fetches failed through the available web tool; no fetched full-PDF review claimed. Microsoft reference implementation and official Windows documentation support the scoped findings above. Upstream branch links are research sources, not pinned deployed firmware attestation.
