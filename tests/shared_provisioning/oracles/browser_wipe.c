/* Browser-private C89 non-elidable erasure; no public API/PST change.
 * Caller must own n valid bytes and end all borrows before physical erasure. */
#include <stddef.h>
void papinho_c2_wipe(void *value, size_t n)
{
    volatile unsigned char *p = (volatile unsigned char *)value;
    while (n != 0U) { *p++ = 0U; --n; }
}
