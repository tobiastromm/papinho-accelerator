#if defined(_MSC_VER) && _MSC_VER <= 1200
#define inline __inline
/* VC6 diagnoses upstream do/while(0) macros and discarded inline definitions.
 * No conversion, type, memory or arithmetic diagnostics are disabled. */
#pragma warning(disable:4127 4514)
#endif
