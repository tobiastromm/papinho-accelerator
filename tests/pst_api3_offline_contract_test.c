/* Offline SDK ABI contract only: no runtime, Connection, wait-set or socket. */
#include <papinho_secure_transport.h>
#include <stddef.h>
#include <stdio.h>
#if PST_API_VERSION != 0x00030000UL
#error This isolated candidate requires PST API 3.0.0
#endif
int main(void)
{
    PST_RELEASE_INFO info;
    if (sizeof(info) != 28U || offsetof(PST_RELEASE_INFO, result) != 12U)
        return 1;
    if (pst_release_info_init(&info, (pst_u32)sizeof(info)) != PST_RESULT_OK)
        return 2;
    if (info.api_version != PST_API_VERSION ||
        info.disposition != PST_RELEASE_DISPOSITION_NONE)
        return 3;
    if (pst_connection_try_release(NULL, &info) != PST_RESULT_INVALID_ARGUMENT)
        return 4;
    puts("PASS_OFFLINE_API3_ABI_ONLY; NO_OWNER_OR_NATIVE_RELEASE_PROOF");
    return 0;
}