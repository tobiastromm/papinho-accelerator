#include "../src/core/ed11_process_profile.h"
#include "server_c8_private_storage_layout_win32.h"
#include <stdio.h>
int main(void)
{
 printf("ROOT_LAYOUT=%llu PARTICIPANT=%llu C2_OWNER=%llu C3_BRIDGE=%llu FIXED_LAYOUT=%llu BASE_ARRAYS=%llu SECURE_ARRAYS=%llu SCHED=%llu BIND=%llu KNOWN_MANAGED=%llu LIMIT=%llu\n",
 (unsigned long long)sizeof(PAPACC_SERVER_LAUNCHER_OWNER_WIN32),(unsigned long long)sizeof(PAPACC_SERVER_C2_PARTICIPANT),
 (unsigned long long)sizeof(PAPACC_SERVER_C2_OWNER_WIN32),(unsigned long long)sizeof(PAPACC_C3_BRIDGE),
 (unsigned long long)sizeof(PAPACC_C8_PRIVATE_STORAGE_LAYOUT),(unsigned long long)PAPACC_C8_BASE_ARRAY_BYTES,
 (unsigned long long)PAPACC_C8_SECURE_ARRAY_BYTES,(unsigned long long)PAPACC_C8_SCHED_ARRAY_BYTES,
 (unsigned long long)PAPACC_C8_BIND_LISTENER_BYTES,(unsigned long long)PAPACC_C8_KNOWN_MANAGED_BYTES,(unsigned long long)PAPACC_ED11_PROCESS_LIMIT);
 puts("PROVIDER_KERNEL=UNKNOWN NATIVE_ADMISSION=DENY NO_NETWORK_OR_MEASUREMENT");
 return PAPACC_C8_KNOWN_MANAGED_BYTES>PAPACC_ED11_PROCESS_LIMIT?1:0;
}