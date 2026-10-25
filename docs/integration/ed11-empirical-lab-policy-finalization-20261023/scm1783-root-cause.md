# SCM1783 — exact caller-size defect, not an adapter or service defect

TASK=PACC-PHASE6-ED11-EMPIRICAL-LAB-POLICY-FINALIZATION-1

## Demonstrated cause

Previous inline Python collector supplied cbBufSize16384 to QueryServiceConfigW. That was an error in the collector prepared by the agent, not evidence of inadequate service identity, permissions or a broken Windows SCM configuration. Microsoft documents an8192-byte maximum; MS-SCMR RQueryServiceConfigW constrains the RPC size parameter to0..8192.16384 violates that argument range. Native1783 is preserved as observed; a packet-level/client-versus-server stub trace was not taken, so do not invent a precise internal stack frame.

Sources: [Microsoft API](https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-queryserviceconfigw), [Microsoft MS-SCMR opnum17](https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-scmr/89e2d5b1-19cf-44ca-969f-38eea9fe7f3c). Official protocol range plus the controlled test support a size-range/marshalling-contract rejection. No network operational experiment was run; documentation retrieval is evidence research.

## Discriminating native test

[Probe source](scm-readback-size-probe.py), SHA256e70e7bb0cc5c3090658605faad8c9e119dfb55fcc1ccbb09501515ae573eaf0a; [raw result](scm-readback-size-result.json),2026-10-23T20:45:16.048179Z. Exactly one paired query experiment, no Start/Stop/configuration/ACL mutation. Same local manager/service handles, token/context,16384-byte aligned allocation, pointer, CONFIG layout and output variable. Only declared cbBufSize changes:

| API size | Result | Bytes needed |
|---:|---|---:|
|16384|FALSE, Win321783|0|
|8192|TRUE|528|

OpenSCManagerW requested CONNECT1; OpenServiceW requested QUERY_CONFIG|QUERY_STATUS5 for existing PapinhoAcceleratorLab20261023. Status STOPPED1/PID0/1077, no running token claim. Context is the local executor's64-bit Python/ctypes collector, without UAC or service impersonation; TokenUser/elevation were not independently captured, so do not infer the service token or an elevated owner. Both calls run on the same thread/handles with no intervening identity-changing operation. ABI64-bit CONFIG size64; DWORD fields0/4/8, image pointer16, group24, tag32, dependencies40, account48, display56. Correct signatures/widths and aligned allocation are unchanged across paired calls. The successful second call rules out a need to change ABI/struct layout or increase rights for this observation. Sufficient real storage was present in BOTH calls; it was an invalid declared API range, not an out-of-bounds pointer allocation or buffer too small. Do not diagnose remote RPC transport/trust failure merely from the numeric error.

Successful readback: own-process16,DEMAND3,error control1, correct virtual account, image still the historical protected papacc_server_lab_host.exe with quoted absolute path plus service host. QueryServiceConfig2W(SERVICE_CONFIG_SERVICE_SID_INFO5,4 bytes) returns SID typeUNRESTRICTED1. Both query-owned service/manager handles close successfully. This does not prove an effective running token or authorize the future instrumented image.

## Minimum correction and disposition

Only the temporary collector needs cbBufSize<=8192 (or documented NULL/0 discovery plus bounded allocation). The minimal16384->8192 correction is positively demonstrated on the same handle/allocation; no need to re-run unrelated SCM probes. The paired source intentionally retains the invalid first size as a negative diagnostic case and must not be used as a positive-operation launcher.

Canonical adapter server_service_virtual_win32.inc:ps_virtual_readback_start already uses sizeof(native.query.bytes),8192, with aligned QUERY_SERVICE_CONFIGW and bounded returned string checks. No functional patch is justified. Previous collector was inline historical evidence, not a persisted product module to rewrite; preserve its failure receipt. Existing service/config/account/SID/runtime/ACL remain unchanged. Next successor must retain the canonical8192-byte buffer, not copy the faulty collector.
