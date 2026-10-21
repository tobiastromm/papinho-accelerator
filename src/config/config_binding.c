#include "config_binding.h"
#include <string.h>
static int same(const char *a,const char *b){return strcmp(a,b)==0;}
static const PC_LINE *field(const PC_DOCUMENT *d,unsigned b,const char *name)
{unsigned i;for(i=d->blocks[b].opener+1U;i<d->blocks[b].end;++i)if(same(d->lines[i].tokens[0].text,name))return &d->lines[i];return NULL;}
static PC_RESULT reject(PC_MODEL_BINDING *o,PC_DIAGNOSTIC *diag,PC_RESULT r,unsigned line,const char *why)
{memset(o,0,sizeof(*o));if(diag){diag->code=r;diag->line=line;diag->column=1U;diag->reason=why;}return r;}
static unsigned dec(const char *s){unsigned n=0U;while(*s){n=n*10U+(unsigned)(*s-'0');++s;}return n;}
static unsigned nibble(char c){return c>='0'&&c<='9'?(unsigned)(c-'0'):c>='A'&&c<='F'?(unsigned)(c-'A'+10):(unsigned)(c-'a'+10);}
static int policy_equal(const PC_DOCUMENT *d,unsigned block,const PAPACC_ADMIN_POLICY *p)
{
 unsigned i,j,k,en=0U,rules=0U;const PC_LINE *r;const PAPACC_ADMIN_POLICY_RULE *rule;
 if(!p->present||p->representation!=PAPACC_ADMIN_REPRESENTATION_04)return 0;
 for(i=d->blocks[block].opener+1U;i<d->blocks[block].end;++i){r=&d->lines[i];
  if(same(r->tokens[0].text,"enable")){
   if(en>=p->enabled_count||dec(r->tokens[1].text)!=p->enabled[en].id||dec(r->tokens[2].text)!=p->enabled[en].major)return 0;++en;
  }else{
   if(rules>=p->rule_count)return 0;rule=&p->rules[rules++];
   if(dec(r->tokens[3].text)!=rule->key.id||dec(r->tokens[4].text)!=rule->key.major||
      (same(r->tokens[2].text,"ACCEPT")?PAPACC_ADMIN_POLICY_ACCEPT:PAPACC_ADMIN_POLICY_DENY)!=rule->action)return 0;
   if(r->count==7U){if(rule->conditions!=PAPACC_ADMIN_POLICY_PRINCIPAL||rule->principal16.type!=1U||rule->principal16.length!=16U)return 0;
    for(j=0U;j<16U;++j){k=nibble(r->tokens[6].text[j*2U])*16U+nibble(r->tokens[6].text[j*2U+1U]);if(k!=rule->principal16.value[j])return 0;}
   }else if(rule->conditions)return 0;
  }
 }
 return en==p->enabled_count&&rules==p->rule_count;
}
/* Decimal representation already used by --interface-id. Presence is separate
 * from zero. Reject overflow before arithmetic; never infer aliases or IDs. */
static int persistent_decimal(const char *text,PAPACC_U64 *out)
{
 PAPACC_U64 value=0U,maximum=~(PAPACC_U64)0,digit;
 if(!text||!*text)return 0;
 while(*text){if(*text<'0'||*text>'9')return 0;digit=(PAPACC_U64)(*text-'0');
  if(value>(maximum-digit)/10U)return 0;value=value*10U+digit;++text;}
 *out=value;return 1;
}
static PC_RESULT bind_selection(const PC_DOCUMENT *d,unsigned block,
 const PAPACC_SERVER_CONFIG *c,const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *snapshot,
 PC_BOUND_LISTENER *bound)
{
 const PC_LINE *mode=field(d,block,"mode"),*line;unsigned i,j,count=0U;
 PAPACC_NETWORK_INTERFACE_PERSISTENT_ID ids[8];PAPACC_U32 instances[8];
 PAPACC_PERSISTENT_BIND_SELECTION persistent=PAPACC_PERSISTENT_BIND_SELECTION_INITIALIZER;
 PAPACC_BIND_SELECTION runtime=PAPACC_BIND_SELECTION_INITIALIZER;
 PAPACC_NETWORK_DISCOVERY_SNAPSHOT empty=PAPACC_NETWORK_DISCOVERY_SNAPSHOT_INITIALIZER;
 PAPACC_SIZE required;PAPACC_RESULT result;
 if(!mode)return PC_REFERENCE;
 if(same(mode->tokens[1].text,"ALL")){
  if(c->bind_selection.mode!=PAPACC_BIND_SELECTION_ALL_INTERFACES)return PC_REFERENCE;
  runtime.mode=PAPACC_BIND_SELECTION_ALL_INTERFACES;snapshot=&empty;
 }else{
  if(!snapshot)return PC_UNSUPPORTED;
  if(c->bind_selection.mode!=PAPACC_BIND_SELECTION_SELECTED_INTERFACES)return PC_REFERENCE;
  for(i=d->blocks[block].opener+1U;i<d->blocks[block].end;++i){line=&d->lines[i];
   if(!same(line->tokens[0].text,"interface_ref"))continue;
   if(count==8U)return PC_LIMIT;
   if(!persistent_decimal(line->tokens[1].text,&ids[count].value))return PC_REFERENCE;
   ids[count].is_valid=PAPACC_TRUE;
   for(j=0U;j<count;++j)if(ids[j].value==ids[count].value)return PC_DUPLICATE;
   ++count;
  }
  if(!count||count!=c->bind_selection.interface_persistent_id_count)return PC_REFERENCE;
  for(i=0U;i<count;++i)if(papacc_network_interface_persistent_id_equal(
    &ids[i],&c->bind_selection.interface_persistent_ids[i])!=PAPACC_TRUE)return PC_REFERENCE;
  persistent.mode=PAPACC_BIND_SELECTION_SELECTED_INTERFACES;
  persistent.interface_persistent_ids=ids;persistent.interface_persistent_id_count=count;
  result=papacc_persistent_bind_selection_resolve(&persistent,snapshot,instances,8U,&runtime,&required);
  if(result!=PAPACC_RESULT_OK)return result==PAPACC_RESULT_LIMIT_EXCEEDED?PC_LIMIT:PC_REFERENCE;
 }
 result=papacc_bind_targets_resolve(&runtime,snapshot,bound->targets,2U,&bound->target_count,&required);
 return result==PAPACC_RESULT_OK?PC_OK:result==PAPACC_RESULT_LIMIT_EXCEEDED?PC_LIMIT:PC_REFERENCE;
}
PC_RESULT pc_bind_existing_models_snapshot(const PC_DOCUMENT *d,const PAPACC_SERVER_CONFIG *const *models,
 PAPACC_SIZE model_count,const PAPACC_NETWORK_DISCOVERY_SNAPSHOT *snapshot,PC_MODEL_BINDING *out,PC_DIAGNOSTIC *diag)
{
 unsigned i,j,matched;PAPACC_SIZE k;PC_RESULT selection_result;const PC_LISTENER *l;const PC_LINE *r,*method,*security;
 const PAPACC_SERVER_CONFIG *c;PC_BOUND_LISTENER *bound;
 if(!out)return PC_INVALID;memset(out,0,sizeof(*out));
 if(!pc_view(d)||!models||!model_count||model_count>PC_LISTENERS_MAX)return reject(out,diag,PC_MISSING,0U,"validated input and bounded existing model required");
 /* Reuse canonical validator, never install candidate policy or synthesize identity. */
 for(k=0U;k<model_count;++k)if(!models[k]||papacc_server_config_validate(models[k])!=PAPACC_RESULT_OK)
  return reject(out,diag,PC_REFERENCE,0U,"existing model invalid or policy absent");
 /* Even unused declared objects must resolve; no apparently applied orphan. */
 for(i=0U;i<d->block_count;++i){matched=0U;
  for(j=0U;j<d->listener_count;++j){l=&d->listeners[j];if(i==l->block||i==l->binding||i==l->security||i==l->authentication||i==l->policy)matched=1U;}
  if(!matched)return reject(out,diag,PC_REFERENCE,d->lines[d->blocks[i].opener].line,"object not bound to a listener context");
 }
 if(!d->listener_count)return reject(out,diag,PC_MISSING,0U,"no listener to bind");
 for(i=0U;i<d->listener_count;++i){l=&d->listeners[i];r=&d->lines[d->blocks[l->block].opener];
  if(l->profile!=1U)return reject(out,diag,PC_UNSUPPORTED,r->line,"Legacy has no implicit administrative resolution");
  method=field(d,l->authentication,"method");
  if(!method||!same(method->tokens[1].text,"NONE")||l->admission!=1U)return reject(out,diag,PC_UNSUPPORTED,r->line,"named access-auth provider authority not established");
  security=field(d,l->security,"security_ref");if(!security)return reject(out,diag,PC_REFERENCE,r->line,"security reference absent");
  matched=0U;c=NULL;
  for(k=0U;k<model_count;++k){const PAPACC_SERVER_CONFIG *m=models[k];
   if(m->control_port==l->port&&m->listener_profile==PAPACC_SERVER_LISTENER_PROFILE_SECURE_PRINCIPAL){++matched;c=m;}
  }
  if(matched!=1U)return reject(out,diag,matched?PC_CONFLICT:PC_REFERENCE,r->line,"listener model absent or ambiguous");
  if(c->policy.admission!=PAPACC_ADMIN_OPEN||
     !policy_equal(d,l->policy,&c->policy))return reject(out,diag,PC_REFERENCE,r->line,"policy or binding does not match existing administrative content");
  if(!c->security_configuration_ref_valid||strlen(security->tokens[1].text)!=c->security_configuration_ref_length||
     memcmp(security->tokens[1].text,c->security_configuration_ref,c->security_configuration_ref_length))return reject(out,diag,PC_REFERENCE,r->line,"security reference does not match existing model");
  bound=&out->listeners[i];memcpy(bound->id,r->tokens[1].text,strlen(r->tokens[1].text)+1U);
  bound->profile=l->profile;bound->admission=l->admission;bound->port=l->port;
  memcpy(bound->security_ref,c->security_configuration_ref,c->security_configuration_ref_length);bound->security_length=c->security_configuration_ref_length;
  memcpy(bound->policy_identity,c->policy.identity,32U);
  selection_result=bind_selection(d,l->binding,c,snapshot,bound);
  if(selection_result!=PC_OK)return reject(out,diag,selection_result,r->line,"PersistentID/model/snapshot or bounded targets rejected; no fallback");
 }
 out->count=d->listener_count;out->model_bound=1U;
 if(diag){diag->code=PC_OK;diag->line=diag->column=0U;diag->reason="model-bound only; native CURRENT and security lease still required";}
 return PC_OK;
}

PC_RESULT pc_bind_existing_models(const PC_DOCUMENT *d,const PAPACC_SERVER_CONFIG *const *models,
 PAPACC_SIZE count,PC_MODEL_BINDING *out,PC_DIAGNOSTIC *diag)
{return pc_bind_existing_models_snapshot(d,models,count,NULL,out,diag);}
