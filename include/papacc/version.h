#ifndef PAPACC_VERSION_H
#define PAPACC_VERSION_H

#define PAPACC_VERSION_MAJOR 0
#define PAPACC_VERSION_MINOR 1
#define PAPACC_VERSION_PATCH 0

#ifdef __cplusplus
extern "C" {
#endif

int papacc_version_major(void);
int papacc_version_minor(void);
int papacc_version_patch(void);
const char *papacc_version_string(void);

#ifdef __cplusplus
}
#endif

#endif
