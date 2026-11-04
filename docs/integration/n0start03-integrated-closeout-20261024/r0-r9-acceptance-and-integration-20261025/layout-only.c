#define main unused_model_main
#include "../../../../tests/service_runtime_component_test.c"
#undef main
int main(void){printf("FULL_FILE_RUNTIME=%zu NATIVE=%zu TOTAL=%zu\n",sizeof(PS_RUNTIME),sizeof(PS_NATIVE),papacc_server_service_storage_bytes());return 0;}
