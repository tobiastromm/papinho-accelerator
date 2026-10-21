#ifndef PAPACC_ED11_PROCESS_PROFILE_H
#define PAPACC_ED11_PROCESS_PROFILE_H
/* Private compile-time PROCESS ceiling. No authority, credit, per-client quota
 * or platform API. Default/other targets preserve the historical ceiling. */
#define PAPACC_ED11_LEGACY_PROCESS_LIMIT 2097152U
#if defined(PAPACC_ED11_WINDOWS_3MIB)
#if PAPACC_ED11_WINDOWS_3MIB != 1
#error Invalid_ED11_Windows_profile_selector
#endif
#if !defined(_WIN32)
#error Windows_ED11_profile_requires_Windows_target
#endif
#define PAPACC_ED11_PROCESS_LIMIT 3145728U
#else
#define PAPACC_ED11_PROCESS_LIMIT PAPACC_ED11_LEGACY_PROCESS_LIMIT
#endif
#endif
