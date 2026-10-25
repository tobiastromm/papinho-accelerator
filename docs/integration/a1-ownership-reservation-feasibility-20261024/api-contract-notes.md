# API boundaries and limits

Primary Microsoft documentation consulted read-only on2026-10-24:

- [CreateFileW](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew): failure returns INVALID_HANDLE_VALUE. Only that documented sentinel is converted to absence by the candidate file adapter. Exclusive dispositions and flags remain those of each original caller. No object is created during these tests.
- [OpenThreadToken](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-openthreadtoken): successful output is a token handle; failure requires immediate GetLastError. The adapter does not assume every failure clears the output: initial NULL plus any non-null contradictory failure is retained UNKNOWN. Expected ERROR_NO_TOKEN follows the existing canonical no-impersonation policy; other failures deny.
- [CloseHandle](https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle): success/failure is explicit; double-close can cause a debugger exception. The model treats failure conservatively as UNKNOWN under Model E; it does not assert that the failed fake close proves a physically live Windows object. No retry, alternative close API, forced release or pseudo-handle close is introduced.

No claim that documentation supplies lifetime accounting for internal OS resources or makes synthetic fault outputs native evidence. Supported synchronous API adapters and their output normalization still require integration qualification after authorization. No ETW, Sysinternals download or new recorder was needed.
