/* Private immutable projection of existing receipt slots. Not wire or authority.
 * U32=Win32 unsigned long; U64=existing PAPACC_U64. No resources acquired. */
#ifndef C2_TARGET_EVIDENCE_PRIVATE_H
#define C2_TARGET_EVIDENCE_PRIVATE_H
typedef struct C2_TARGET_VIEW {
 unsigned long version,bytes,origin,clock,metric,unit,scope,phase;
 unsigned long process,thread,mapping,root,root_state,witness,lifetime;
 unsigned long begin,end,presence,policy_present,expiry_present,recipe_present;
 PAPACC_U64 creation,thread_creation,request,operation,generation,epoch,value;
} C2_TARGET_VIEW;
/* Result: 0 INCOMPLETE; 1 OBSERVATION_ONLY; 2 INVALID. Never admission.
 * origin1=native; clock1=local GetTickCount U32; unit1=bytes,2=handles;
 * scope1=process,2=OS-reported peak,3=host available;
 * lifetime1=live,2=retained,3=stopped,4=destroyed. */
static int c2_target_view_classify(const C2_TARGET_VIEW *v,const C2_TARGET_VIEW *o){
 if(!v || !o)return 0;
 if(v->version!=1U || v->bytes!=sizeof(*v) || v->origin!=1U || v->clock!=1U ||
  v->metric>5U || v->unit!=(v->metric==2U?2U:1U) ||
  v->scope!=(v->metric<3U?1U:(v->metric==3U?2U:3U)) ||
  v->phase>=8U || v->lifetime<1U || v->lifetime>4U)return 2;
 if(v->process!=o->process || v->thread!=o->thread || v->creation!=o->creation ||
  v->thread_creation!=o->thread_creation || v->mapping!=o->mapping ||
  v->root!=o->root || v->root_state!=o->root_state || v->request!=o->request ||
  v->operation!=o->operation || v->generation!=o->generation || v->epoch!=o->epoch ||
  v->phase!=o->phase || v->witness!=o->witness || v->metric!=o->metric || v->value!=o->value ||
  v->lifetime!=o->lifetime || v->begin!=o->begin || v->end!=o->end || v->presence!=o->presence)return 2;
 /* No applicable policy/expiry/recipe has been installed. Forged presence is invalid. */
 if(v->policy_present || v->expiry_present || v->recipe_present)return 2;
 if((v->presence&3U)!=3U || !v->process || !v->creation || !v->mapping || v->end<v->begin)return 0;
 return 1;
}
static int c2_target_view_admission(const C2_TARGET_VIEW *v,const C2_TARGET_VIEW *o){
 (void)c2_target_view_classify(v,o);return 0;
}
#endif
