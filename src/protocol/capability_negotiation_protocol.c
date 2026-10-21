#include "capability_negotiation_protocol.h"

static void put_u16(PAPACC_U8 *p, PAPACC_U16 v)
{ p[0]=(PAPACC_U8)(v>>8); p[1]=(PAPACC_U8)v; }
static void put_u32(PAPACC_U8 *p, PAPACC_U32 v)
{ p[0]=(PAPACC_U8)(v>>24); p[1]=(PAPACC_U8)(v>>16); p[2]=(PAPACC_U8)(v>>8); p[3]=(PAPACC_U8)v; }
static PAPACC_U16 get_u16(const PAPACC_U8 *p)
{ return (PAPACC_U16)(((PAPACC_U16)p[0]<<8)|p[1]); }
static PAPACC_U32 get_u32(const PAPACC_U8 *p)
{ return ((PAPACC_U32)p[0]<<24)|((PAPACC_U32)p[1]<<16)|((PAPACC_U32)p[2]<<8)|p[3]; }

static PAPACC_BOOL set_valid_canonical(const PAPACC_CAPABILITY_SET *set)
{
    PAPACC_SIZE i;
    if (set == NULL || set->initialized != PAPACC_TRUE || set->count > set->capacity ||
        (set->count > 0U && set->storage == NULL)) return PAPACC_FALSE;
    for (i=0U;i<set->count;++i) {
        if (papacc_capability_key_valid(&set->storage[i]) != PAPACC_TRUE) return PAPACC_FALSE;
        if (i>0U && papacc_capability_key_compare(&set->storage[i-1U],
            &set->storage[i]) >= 0) return PAPACC_FALSE;
    }
    return PAPACC_TRUE;
}
static PAPACC_BOOL subset(const PAPACC_CAPABILITY_SET *a,
    const PAPACC_CAPABILITY_SET *b)
{
    PAPACC_SIZE i;
    for(i=0U;i<a->count;++i)
        if(papacc_capability_set_contains(b,&a->storage[i])!=PAPACC_TRUE) return PAPACC_FALSE;
    return PAPACC_TRUE;
}
static void put_key(PAPACC_U8 *p,const PAPACC_CAPABILITY_KEY *k)
{ put_u32(p,k->id); put_u16(p+4,k->major); }
static PAPACC_RESULT read_keys(const PAPACC_U8 *p,PAPACC_SIZE count,
    PAPACC_CAPABILITY_SET *set)
{
    PAPACC_SIZE i;
    if(count>set->capacity) return PAPACC_RESULT_LIMIT_EXCEEDED;
    for(i=0U;i<count;++i) {
        PAPACC_CAPABILITY_KEY k;
        k.id=get_u32(p+i*6U); k.major=get_u16(p+i*6U+4U);
        if(papacc_capability_key_valid(&k)!=PAPACC_TRUE) return PAPACC_RESULT_PROTOCOL_ERROR;
        if(i>0U && papacc_capability_key_compare(&set->storage[i-1U],&k)>=0)
            return PAPACC_RESULT_PROTOCOL_ERROR;
        set->storage[i]=k;
    }
    set->count=count; return PAPACC_RESULT_OK;
}

PAPACC_RESULT papacc_capability_negotiate_encode(const PAPACC_CAPABILITY_SET *s,
    const PAPACC_CAPABILITY_SET *p,PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *written)
{
    PAPACC_SIZE i,need;
    if(written!=NULL)*written=0U;
    if(out==NULL||written==NULL||set_valid_canonical(s)!=PAPACC_TRUE||
        set_valid_canonical(p)!=PAPACC_TRUE||subset(p,s)!=PAPACC_TRUE||
        s->count>65535U||p->count>65535U) return PAPACC_RESULT_INVALID_ARGUMENT;
    need=4U+(s->count+p->count)*6U;
    if(cap<need) return PAPACC_RESULT_LIMIT_EXCEEDED;
    put_u16(out,(PAPACC_U16)s->count); put_u16(out+2,(PAPACC_U16)p->count);
    for(i=0U;i<s->count;++i)put_key(out+4U+i*6U,&s->storage[i]);
    for(i=0U;i<p->count;++i)put_key(out+4U+(s->count+i)*6U,&p->storage[i]);
    *written=need; return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_capability_negotiate_decode(const PAPACC_U8 *in,
    PAPACC_SIZE len,PAPACC_CAPABILITY_SET *s,PAPACC_CAPABILITY_SET *p)
{
    PAPACC_SIZE sc,pc; PAPACC_RESULT r;
    if(in==NULL||s==NULL||p==NULL||s->initialized!=PAPACC_TRUE||p->initialized!=PAPACC_TRUE)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    s->count=0U;p->count=0U;if(len<4U)return PAPACC_RESULT_PROTOCOL_ERROR;
    sc=get_u16(in);pc=get_u16(in+2);
    if(len!=4U+(sc+pc)*6U)return PAPACC_RESULT_PROTOCOL_ERROR;
    r=read_keys(in+4U,sc,s);if(r!=PAPACC_RESULT_OK)return r;
    r=read_keys(in+4U+sc*6U,pc,p);if(r!=PAPACC_RESULT_OK){s->count=0U;return r;}
    if(subset(p,s)!=PAPACC_TRUE){s->count=0U;p->count=0U;return PAPACC_RESULT_PROTOCOL_ERROR;}
    return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_capability_negotiate_result_encode(const PAPACC_CAPABILITY_SET *e,
    PAPACC_U8 *out,PAPACC_SIZE cap,PAPACC_SIZE *written)
{
    PAPACC_SIZE i,need;
    if(written!=NULL)*written=0U;
    if(out==NULL||written==NULL||set_valid_canonical(e)!=PAPACC_TRUE||e->count>65535U)
        return PAPACC_RESULT_INVALID_ARGUMENT;
    need=2U+e->count*6U;if(cap<need)return PAPACC_RESULT_LIMIT_EXCEEDED;
    put_u16(out,(PAPACC_U16)e->count);for(i=0U;i<e->count;++i)put_key(out+2U+i*6U,&e->storage[i]);
    *written=need;return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_capability_negotiate_result_decode(const PAPACC_U8 *in,
    PAPACC_SIZE len,PAPACC_CAPABILITY_SET *e)
{
    PAPACC_SIZE count;if(in==NULL||e==NULL||e->initialized!=PAPACC_TRUE)return PAPACC_RESULT_INVALID_ARGUMENT;
    e->count=0U;if(len<2U)return PAPACC_RESULT_PROTOCOL_ERROR;count=get_u16(in);
    if(len!=2U+count*6U)return PAPACC_RESULT_PROTOCOL_ERROR;
    return read_keys(in+2U,count,e);
}
static PAPACC_FRAME_HEADER hdr(PAPACC_U16 type,PAPACC_U32 n)
{ PAPACC_FRAME_HEADER h={1U,0U,16U,0U,0U,0U};
  h.message_type=type; h.payload_length=n; return h; }
PAPACC_FRAME_HEADER papacc_capability_negotiate_frame_header(PAPACC_U32 n)
{return hdr(PAPACC_MESSAGE_TYPE_CAPABILITY_NEGOTIATE,n);}
PAPACC_FRAME_HEADER papacc_capability_negotiate_result_frame_header(PAPACC_U32 n)
{return hdr(PAPACC_MESSAGE_TYPE_CAPABILITY_NEGOTIATE_RESULT,n);}
