#define main historical_size_entry_not_run
#include "C:/Projetos/PapinhoAccelerator/tests/phase6_memory_census_test.c"
#undef main
#include "config_root8_v4.h"
typedef struct PROPOSED_BACKING { unsigned char packet[31840],ca[8192]; unsigned char packet_hash[32],ca_hash[32]; PAPACC_SIZE packet_bytes,ca_bytes; PAPACC_U64 serial; } PROPOSED_BACKING;
int main(void){size_t delta=3U*(sizeof(PROPOSED_BACKING)-sizeof(PAPACC_CONFIG_V3_BACKING));size_t fixed=PAPACC_C8_KNOWN_MANAGED_BYTES+delta;size_t callers=3U*31840U+30592U+8192U+32768U+256U;size_t scoped=fixed+callers+52904U;
 printf("BEFORE_FIXED=%Iu OLD_SLOT=%Iu PROPOSED_SLOT=%Iu SLOT_COUNT=3 DELTA=%Iu PROPOSED_FIXED=%Iu CALLER_SCENARIO=%Iu PT_EXPLICIT=52904 KNOWN_SCENARIO=%Iu LIMIT=%Iu\n",(size_t)PAPACC_C8_KNOWN_MANAGED_BYTES,sizeof(PAPACC_CONFIG_V3_BACKING),sizeof(PROPOSED_BACKING),delta,fixed,callers,scoped,(size_t)PAPACC_ED11_PROCESS_LIMIT);
 puts("SIZE_ONLY_NO_RESOURCES NO_OPERATIONAL_RESERVATION PROVIDER_KERNEL_TRANSITIVE_STACK_UNKNOWN WORKER_NOT_ENABLED");return scoped>PAPACC_ED11_PROCESS_LIMIT;}
