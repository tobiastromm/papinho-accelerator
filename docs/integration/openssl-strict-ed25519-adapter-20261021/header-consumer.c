#include "papacc_strict_ed25519.h"
int papacc_header_probe(const unsigned char *key, size_t bytes)
{ return papacc_ossl358_ed25519_point_check(key, bytes); }
