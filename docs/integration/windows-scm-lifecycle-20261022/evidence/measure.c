#include <stdio.h>
#include "C:/Projetos/PapinhoAccelerator/apps/server/server_service_win32.c"
int main(void)
{
 printf("NATIVE=%lu HOST=%lu ATOMIC=%lu NAME_RO=%lu REQUEST_STACK=%lu LIFE=%lu STATUS_STACK=%lu STORAGE_API=%lu\n",
  (unsigned long)sizeof(native),(unsigned long)sizeof(host),(unsigned long)sizeof(cli_active),
  (unsigned long)sizeof(ps_name),(unsigned long)sizeof(PS_REQUEST),(unsigned long)sizeof(PS_LIFETIME),
  (unsigned long)sizeof(SERVICE_STATUS_PROCESS),(unsigned long)papacc_server_service_storage_bytes());
 return 0;
}
