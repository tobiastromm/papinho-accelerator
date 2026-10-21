#define main papacc_secure_fixture_embedded_main
#include "secure_fixture_server_win32.c"
#undef main

static int parse_port(const char *text, PAPACC_U16 *port)
{
    char *end = NULL;
    unsigned long value;
    if (text == NULL || port == NULL || text[0] == '\0') return 0;
    value = strtoul(text, &end, 10);
    if (end == text || *end != '\0' || value == 0UL || value > 65535UL)
        return 0;
    *port = (PAPACC_U16)value;
    return 1;
}

int main(int argc, char **argv)
{
    PAPACC_U16 http_port = 0U, https_port = 0U;
    if (argc != 9 || strcmp(argv[1], "--http-port") != 0 ||
        !parse_port(argv[2], &http_port) ||
        strcmp(argv[3], "--https-port") != 0 ||
        !parse_port(argv[4], &https_port) || http_port == https_port ||
        strcmp(argv[5], "--ready-file") != 0 || argv[6][0] == '\0' ||
        strcmp(argv[7], "--stop-event") != 0 || argv[8][0] == '\0') {
        fprintf(stderr, "usage: %s --http-port <1..65535> --https-port "
            "<1..65535> --ready-file <path> --stop-event <name>\n", argv[0]);
        return 2;
    }
    return run_controlled_targets_on_ports(argv[6], argv[8], http_port,
        https_port);
}
