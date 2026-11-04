#ifndef PAPACC_SCQ_RESOURCE_FAILURE_PROJECTION_DEFINED
#define PAPACC_SCQ_RESOURCE_FAILURE_PROJECTION_DEFINED
/* Projection of already acquired observations, emitted only after DENY.
 * Never samples, validates authority, changes state, or relaxes G3. */
static void scqg_failure_projection(FILE *out,const PAPACC_SCQ_START_ASSURANCE *g,DWORD error)
{
 const PAPACC_SCM_RESOURCE_OBSERVATION *v=&g->observations;
 const PAPACC_SCM_RESOURCE_SAMPLE *s=&v->last;
 if(!out)return;
 (void)fprintf(out,"SCQ_START_RESOURCE_FAILURE error=%lu phase=%lu first=%lu attempts=%lu samples=%lu query_error=%lu query_stage=%lu initial_known=%lu initial_handles=%lu handles=%lu cap=%lu guard=%lu private=%llu working=%llu physical=%llu physical_floor=%llu commit=%llu commit_floor=%llu ready_private=%llu ready_working=%llu private_growth=%llu working_growth=%llu begin=%llu end=%llu frequency=%llu freshness_ms=%lu duration_ms=%lu; EXISTING_SAMPLE_NOT_NEW_QUERY; ZERO_WITHOUT_SAMPLE_NOT_EVIDENCE; OPAQUE_UNKNOWN; NO_AUTHORITY\n",
  (unsigned long)error,(unsigned long)g->phase,(unsigned long)g->first,
  (unsigned long)v->attempts,(unsigned long)v->samples,(unsigned long)v->first_error,(unsigned long)v->error_stage,
  (unsigned long)g->initial_known,(unsigned long)g->initial_handles,(unsigned long)s->handles,
  (unsigned long)g->limits.max_handles,(unsigned long)PAPACC_SCQ_HANDLE_GUARD(g->p1_handles),
  s->private_bytes,s->working_set,s->physical_available,g->limits.physical_floor,
  s->commit_available,g->limits.commit_floor,g->ready.private_bytes,g->ready.working_set,
  g->limits.private_growth,g->limits.working_growth,s->begin,s->end,s->frequency,
  (unsigned long)g->limits.freshness_ms,(unsigned long)g->limits.duration_ms);
}

#endif
