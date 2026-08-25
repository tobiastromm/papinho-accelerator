#include <string.h>

#include "papacc/version.h"

int main(void)
{
    if (papacc_version_major() != PAPACC_VERSION_MAJOR) {
        return 1;
    }
    if (papacc_version_minor() != PAPACC_VERSION_MINOR) {
        return 2;
    }
    if (papacc_version_patch() != PAPACC_VERSION_PATCH) {
        return 3;
    }
    if (strcmp(papacc_version_string(), "0.1.0") != 0) {
        return 4;
    }

    return 0;
}
