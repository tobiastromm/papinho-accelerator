#include <stdio.h>
#include "server_service_data.h"
#include "personal_native_win32.h"
#include "config_source.h"
int main(void){printf("REQUEST=%lu FACTS=%lu BINDING=%lu NATIVE_CONTEXT=%lu AST=%lu\n",(unsigned long)sizeof(PS_REQUEST),(unsigned long)sizeof(PS_BINDING_FACTS),(unsigned long)sizeof(PS_LAB_BINDING),(unsigned long)sizeof(PAPACC_PERSONAL_NATIVE_CONTEXT),(unsigned long)sizeof(PC_DOCUMENT));return 0;}
