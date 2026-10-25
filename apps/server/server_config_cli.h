#ifndef PAPACC_SERVER_CONFIG_CLI_H
#define PAPACC_SERVER_CONFIG_CLI_H
#include <stdio.h>
/* Candidate-only synchronous frontend. Invoke before any runtime acquisition.
 * -1 means not a config command; otherwise process exit status (0 structural
 * validation only, 1 invalid/IO, 2 authority unavailable). No root is fabricated.
 * Diagnostics do not reproduce source text or references. The personal native
 * ceremony presents escaped configuration tokens only on its interactive
 * console; private key material and secrets must never be presented. */
int papacc_server_config_cli(int, const char *const *, FILE *, FILE *);
size_t papacc_server_config_cli_workspace_bytes(void);
#endif
