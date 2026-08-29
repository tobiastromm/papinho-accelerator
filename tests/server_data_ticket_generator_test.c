#include "server_data_ticket_generator.h"
#include <string.h>
int main(void)
{
    PAPACC_SERVER_DATA_TICKET_GENERATOR g =
        PAPACC_SERVER_DATA_TICKET_GENERATOR_INITIALIZER;
    PAPACC_DATA_ASSOCIATION_TICKET a = PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
    PAPACC_DATA_ASSOCIATION_TICKET b = PAPACC_DATA_ASSOCIATION_TICKET_INITIALIZER;
    if (papacc_server_data_ticket_generator_generate(&g, &a) !=
            PAPACC_RESULT_INVALID_STATE ||
        papacc_server_data_ticket_generator_init(&g) != PAPACC_RESULT_OK ||
        papacc_server_data_ticket_generator_generate(&g, &a) != PAPACC_RESULT_OK ||
        papacc_server_data_ticket_generator_generate(&g, &b) != PAPACC_RESULT_OK ||
        papacc_data_association_ticket_is_valid(&a) != PAPACC_TRUE ||
        papacc_data_association_ticket_equal(&a, &b) == PAPACC_TRUE)
        return 1;
    memset(g.counter, 0xFF, sizeof(g.counter));
    if (papacc_server_data_ticket_generator_generate(&g, &a) != PAPACC_RESULT_OK ||
        papacc_data_association_ticket_is_valid(&a) != PAPACC_TRUE ||
        a.bytes[15] != 1)
        return 2;
    papacc_server_data_ticket_generator_shutdown(&g);
    papacc_server_data_ticket_generator_shutdown(&g);
    return g.initialized == PAPACC_FALSE ? 0 : 3;
}
