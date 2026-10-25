#define main lab_registration_entry_NOT_CALLED
#include "C:/Projetos/PapinhoAccelerator/tools/qualification/scm-registration-lab.c"
#undef main
int main(void){printf("PS_NATIVE=%lu PS_HOST=%lu CLI_ATOMIC=%lu LAB_EXTRA_GLOBALS=%lu TOKEN_STATISTICS=%lu HASH_CONTEXT=%lu\n",(unsigned long)sizeof(native),(unsigned long)sizeof(host),(unsigned long)sizeof(cli_active),(unsigned long)(sizeof(lab_parent)+sizeof(lab_config)+sizeof(lab_token)+sizeof(lab_sid)+sizeof(lab_pending_sd)+sizeof(lab_aux_retained)+sizeof(lab_aux_count)+sizeof(lab_stats)+sizeof(lab_sd_hash)),(unsigned long)sizeof(TOKEN_STATISTICS),(unsigned long)sizeof(PACC_STORAGE_SHA256));return 0;}
