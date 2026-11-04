#ifndef PAPACC_SCQ_NATIVE_SD_WIN32_H
#define PAPACC_SCQ_NATIVE_SD_WIN32_H
/* Private acquisition diagnostics, never a wire format or authority. */
typedef struct PAPACC_SCQ_NATIVE_SD_STATUS { DWORD calls,status,needed,capacity; } PAPACC_SCQ_NATIVE_SD_STATUS;
#define PAPACC_SCQ_FILE_SD_BYTES 65536U /* existing SCQ chain bound; NTFS SD limit */
#endif
