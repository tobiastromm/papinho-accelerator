#ifndef TP_JSON_DATA_H
#define TP_JSON_DATA_H
#include <stddef.h>
#define TJ_MAX 1024
#define TJ_BYTES 32768
/* Strict private bounded ASCII JSON DATA profile. No authority/persistence. */
typedef struct TJ_NODE {unsigned start,end,next,first,count;char type;} TJ_NODE;
typedef struct TJ_DOC {char bytes[TJ_BYTES];size_t length;TJ_NODE node[TJ_MAX];unsigned count,pos;} TJ_DOC;
int tj_parse(TJ_DOC *d,size_t length);
int tj_get(const TJ_DOC *d,int object,const char *key);
int tj_string(const TJ_DOC *d,int node,char *out,size_t capacity);
int tj_is(const TJ_DOC *d,int node,const char *value);
int tj_true(const TJ_DOC *d,int node);
int tj_u64(const TJ_DOC *d,int node,unsigned long long *out);
#endif
