# ERROR_BUSY stage diagnostics

Original proposed patch applied in staging with preimage fbfeaffaa76bb34e1978c41ce4b143896909ebef64f2efc1a6b38487f8c59038 and proposed postimage b7cf3e84fd066e90863a1961b6a37a5d63fc517133c7a5e69cf48403e97abf19 verified. Historical proposal preserved. Diagnostic extensions add original API/result/resource and bounded census detail.

IG_DIAGNOSTIC stores first failure only and restores GetLastError. CANONICAL_ADMISSION distinguishes CreateMutexExW/GetSecurityInfo failure, WAIT_TIMEOUT native258 mapped by existing owner to170, abandonment and WAIT_FAILED. No holder identity fabricated. PREFLIGHT_CENSUS records PID,parent,predicate child/project_prefix and up to64 UTF16 basename units. Snapshot name is not creation identity proof. Snapshot/iteration/close failures are separate from predicate rejection. Existing filters unchanged.

PREFLIGHT_METRICS records API, native result, resource, PID and role for process/thread/PSS/host observations. Intermediate stages and gate_or_uninstrumented_api fallback preserve localization limits. CLEANUP retains first error. No retry, takeover, forced release, threshold, lock, authority or admission policy change.

Isolated tests cover injected GetPerformanceInfo/170, census conflict and real cross-process mutex timeout, first error, LastError, Stop, retention and close. Four final /W4 /WX builds pass after correcting intermediate compiler diagnostics. Launcher AST parsed only; no launcher entry executed. Historical real Win32 170 source remains UNKNOWN.
