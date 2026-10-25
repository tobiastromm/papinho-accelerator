/* A synchronous borrow of already validated, still held read/share-read files.
 * Only content hashing is shared. All volatile predicates remain per entry. */
#ifndef PAPACC_N0_CENSUS_CONTENT_H
#define PAPACC_N0_CENSUS_CONTENT_H
typedef struct IG_N0_CENSUS_CONTENT {
 const void *descriptor;
 ULONGLONG began,validated,challenge;
 DWORD thread,calls;
} IG_N0_CENSUS_CONTENT;
static __inline int ig_n0_census_content_valid(const IG_N0_CENSUS_CONTENT *s,
 const void *descriptor,ULONGLONG began,ULONGLONG validated,ULONGLONG challenge,
 DWORD thread,ULONGLONG now)
{
 return s&&s->descriptor==descriptor&&s->began==began&&s->validated==validated&&
 s->challenge==challenge&&s->thread==thread&&began&&validated>=began&&
 now>=validated&&now-began<=60000ULL&&now-validated<=60000ULL;
}
#endif
