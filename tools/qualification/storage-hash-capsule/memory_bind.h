#ifndef PACC_LAB_MEMORY_BIND_H
#define PACC_LAB_MEMORY_BIND_H
#include <stddef.h>
#include <string.h>
void *pacc_hash_memcpy(void *,const void *,size_t);
void *pacc_hash_memset(void *,int,size_t);
#define memcpy pacc_hash_memcpy
#define memset pacc_hash_memset
#endif
