/* Bounded caller-span memory primitives, not cryptographic arithmetic.
 * Volatile byte stores prevent compiler substitution by opaque CRT calls. */
#include <stddef.h>
void *pacc_hash_memcpy(void *dst,const void *src,size_t n)
{volatile unsigned char *d=dst;const unsigned char *p=src;void *ret=dst;while(n--)*d++=*p++;return ret;}
void *pacc_hash_memset(void *dst,int value,size_t n)
{volatile unsigned char *d=dst;void *ret=dst;while(n--)*d++=(unsigned char)value;return ret;}
