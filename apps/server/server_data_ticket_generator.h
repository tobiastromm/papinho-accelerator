#ifndef PAPACC_SERVER_DATA_TICKET_GENERATOR_H
#define PAPACC_SERVER_DATA_TICKET_GENERATOR_H
#include "data_association.h"
typedef struct PAPACC_SERVER_DATA_TICKET_GENERATOR { PAPACC_U8 counter[16]; PAPACC_BOOL initialized; } PAPACC_SERVER_DATA_TICKET_GENERATOR;
#define PAPACC_SERVER_DATA_TICKET_GENERATOR_INITIALIZER { { 0 }, PAPACC_FALSE }
PAPACC_RESULT papacc_server_data_ticket_generator_init(PAPACC_SERVER_DATA_TICKET_GENERATOR *generator);
PAPACC_RESULT papacc_server_data_ticket_generator_generate(void *context, PAPACC_DATA_ASSOCIATION_TICKET *out_ticket);
void papacc_server_data_ticket_generator_shutdown(PAPACC_SERVER_DATA_TICKET_GENERATOR *generator);
#endif
