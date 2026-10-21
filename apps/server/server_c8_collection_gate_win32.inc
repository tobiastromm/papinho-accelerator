/* Single process-lifetime cadence authority; never reset or rebound. This is
 * observation authority only, not admission. All native collectors use it.
 * Nonwaiting CAS is held through the native query and completion bookkeeping.
 * Synthetic reducers do not touch it. No callbacks, allocation or I/O here. */
typedef struct C8_COLLECTION_GATE {
 volatile LONG locked;
 PAPACC_U64 invocation,owner,started,completed,attempts,rate_refused,clock_faults;
 PAPACC_U32 begun,incomplete,poisoned;
} C8_COLLECTION_GATE;
typedef union C8_COLLECTION_BANK {C8_COLLECTION_GATE v;PAPACC_U8 storage[PAPACC_C8_COLLECTION_STORAGE];} C8_COLLECTION_BANK;
static C8_COLLECTION_BANK collection_bank;
typedef char c8_collection_size[(sizeof(C8_COLLECTION_BANK)==128U&&sizeof(C8_COLLECTION_GATE)<=128U)?1:-1];
static PAPACC_U32 collection_begin(const PAPACC_C8_TARGET_BINDING *b,PAPACC_U64 now,PAPACC_U32 *incomplete)
{
 C8_COLLECTION_GATE *g=&collection_bank.v;PAPACC_U32 reason=PAPACC_C8_COLLECT_OK;
 *incomplete=0U;
 if(InterlockedCompareExchange(&g->locked,1L,0L)!=0L)return PAPACC_C8_COLLECT_BUSY;
 if(!b->invocation||!b->owner||(g->begun&&(b->invocation!=g->invocation||b->owner!=g->owner)))reason=PAPACC_C8_COLLECT_IDENTITY;
 else if(g->poisoned)reason=(g->incomplete&2U)?PAPACC_C8_COLLECT_OVERFLOW:PAPACC_C8_COLLECT_CLOCK;
 else if(g->begun&&now<g->completed){
  g->incomplete|=1U;
  if(g->clock_faults==(PAPACC_U64)-1){g->poisoned=1U;g->incomplete|=2U;reason=PAPACC_C8_COLLECT_OVERFLOW;}
  else {++g->clock_faults;reason=PAPACC_C8_COLLECT_CLOCK;}
 }else if(g->begun&&now-g->completed<100U){
  if(g->rate_refused==(PAPACC_U64)-1){g->poisoned=1U;g->incomplete|=2U;reason=PAPACC_C8_COLLECT_OVERFLOW;}
  else {++g->rate_refused;reason=PAPACC_C8_COLLECT_RATE;}
 }else if(g->attempts==(PAPACC_U64)-1){g->poisoned=1U;g->incomplete|=2U;reason=PAPACC_C8_COLLECT_OVERFLOW;}
 *incomplete=g->incomplete;
 if(reason!=PAPACC_C8_COLLECT_OK){InterlockedExchange(&g->locked,0L);return reason;}
 g->invocation=b->invocation;g->owner=b->owner;g->started=now;g->begun=1U;++g->attempts;
 return reason; /* lock remains held, exactly one collection in flight */
}
static void collection_end(PAPACC_U64 end,int clock_valid,PAPACC_U32 *reason,PAPACC_U32 *incomplete)
{
 C8_COLLECTION_GATE *g=&collection_bank.v;
 if(!clock_valid){g->incomplete|=1U;g->poisoned=1U;*reason=PAPACC_C8_COLLECT_CLOCK;}
 if(end<g->started){g->incomplete|=1U;*reason=PAPACC_C8_COLLECT_CLOCK;end=g->started;}
 g->completed=end;*incomplete=g->incomplete;
 InterlockedExchange(&g->locked,0L);
}

PAPACC_RESULT papacc_c8_collection_health(const PAPACC_C8_TARGET_BINDING *b,PAPACC_U32 *incomplete)
{
 C8_COLLECTION_GATE *g=&collection_bank.v;PAPACC_RESULT result=PAPACC_RESULT_OK;
 if(!b||!incomplete)return PAPACC_RESULT_INVALID_ARGUMENT;
 *incomplete=1U;
 if(InterlockedCompareExchange(&g->locked,1L,0L)!=0L)return PAPACC_RESULT_INVALID_STATE;
 if(g->begun&&(b->invocation!=g->invocation||b->owner!=g->owner))result=PAPACC_RESULT_INVALID_STATE;
 else *incomplete=g->incomplete;
 InterlockedExchange(&g->locked,0L);return result;
}

PAPACC_RESULT papacc_c8_collection_clock_fault(const PAPACC_C8_TARGET_BINDING *b,PAPACC_BOOL unavailable)
{
 C8_COLLECTION_GATE *g=&collection_bank.v;PAPACC_RESULT result=PAPACC_RESULT_OK;
 if(!b||!b->invocation||!b->owner||b->owner!=(PAPACC_U64)GetCurrentThreadId())return PAPACC_RESULT_INVALID_STATE;
 if(InterlockedCompareExchange(&g->locked,1L,0L)!=0L)return PAPACC_RESULT_INVALID_STATE;
 if(g->begun&&(b->invocation!=g->invocation||b->owner!=g->owner))result=PAPACC_RESULT_INVALID_STATE;
 else {
  g->invocation=b->invocation;g->owner=b->owner;g->begun=1U;g->incomplete|=PAPACC_C8_COLLECTION_CLOCK_INCOMPLETE;
  if(unavailable)g->poisoned=1U;
  if(g->clock_faults==(PAPACC_U64)-1){g->poisoned=1U;g->incomplete|=PAPACC_C8_COLLECTION_OVERFLOW_INCOMPLETE;result=PAPACC_RESULT_LIMIT_EXCEEDED;}
  else ++g->clock_faults;
 }
 InterlockedExchange(&g->locked,0L);return result;
}
