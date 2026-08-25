#include "papacc/version.h"

#define PAPACC_STRINGIFY_IMPL(value) #value
#define PAPACC_STRINGIFY(value) PAPACC_STRINGIFY_IMPL(value)

int papacc_version_major(void)
{
    return PAPACC_VERSION_MAJOR;
}

int papacc_version_minor(void)
{
    return PAPACC_VERSION_MINOR;
}

int papacc_version_patch(void)
{
    return PAPACC_VERSION_PATCH;
}

const char *papacc_version_string(void)
{
    return PAPACC_STRINGIFY(PAPACC_VERSION_MAJOR) "."
           PAPACC_STRINGIFY(PAPACC_VERSION_MINOR) "."
           PAPACC_STRINGIFY(PAPACC_VERSION_PATCH);
}
