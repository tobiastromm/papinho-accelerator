from pathlib import Path
import json
r=Path.cwd();d=r/'docs/integration/issuer-session-live-publication-20261019';h=json.loads((r/'docs/integration/issuer-live-session-exact-integration-20261019/candidate-hunks.json').read_text())
def load(p):return (d/'proposed'/p.replace('/','__')).read_text()
def save(p,s):(d/'proposed'/p.replace('/','__')).write_text(s)
# Reconcile CRLF-only conflicts against exact current snippets.
for idx in [24,29]:
 e=h[idx];s=load(e['file']);a=e['before'].replace('\r\n','\n');b=e['after'].replace('\r\n','\n');assert s.count(a)==1 or b in s;save(e['file'],s.replace(a,b,1) if a in s else s)
p='apps/server/server_configuration_root_transaction_win32.inc';s=load(p);a=s.index('static int root_snapshot_held(');b=s.index('PAPACC_RESULT papacc_server_launcher_configuration_current_pin(',a);replacement=h[15]['after'].replace('\r\n','\n');s=s[:a]+replacement+s[b:]
s=s.replace('!c->published||c->stopped||c->unresolved||c->recovery_selected||','!c->published||c->stopped||c->unresolved||c->recovery_selected||c->pending_commit.active||c->authority_lost||',1)
s=s.replace('return c->initialized&&c->published&&!c->stopped&&!c->unresolved&&!c->recovery_selected&&','return c->initialized&&c->published&&!c->stopped&&!c->unresolved&&!c->recovery_selected&&!c->authority_lost&&',1)
s=s.replace('configuration_c2_owner.manager&&!configuration_c2_owner.manager->stopping','configuration_c2_owner.manager&&configuration_c2_owner.manager->initialized&&!configuration_c2_owner.manager->stopping',1)
a=s.index('PAPACC_RESULT papacc_server_launcher_configuration_issuer_publish_live(');b=s.index('/* Same normative application',a)
s=s[:a]+'''/* Begin exactly once, then poll on subsequent owner turns. The worker never
 * publishes the model. Stop/unknown must still permit factual completion query. */
PAPACC_RESULT papacc_server_launcher_configuration_issuer_publish_live(PAPACC_SERVER_LAUNCHER_OWNER_WIN32 *o,PAPACC_ISSUER_CAS_RESULT *out)
{
 PAPACC_CONFIGURATION_COMMIT *c;PAPACC_ROOT_CONFIGURATION_PREPARATION *t;PAPACC_RESULT result;
 PAPACC_ISSUER_CAS_RESULT observed;
 if(!preparation_owned(o)||!out||!o->issuer_projection_validated)return PAPACC_RESULT_INVALID_STATE;
 c=&o->configuration_commit;t=&o->configuration_preparation;
 if(issuer06_root_output_overlap(o,out,sizeof(*out)))return PAPACC_RESULT_INVALID_ARGUMENT;
 *out=PAPACC_COMMIT_IDLE;
 if(!o->issuer_commit_started){
  if(!issuer_root_live_ready(o)||c->pending_commit.active||c->committed_pending||
     o->issuer_expected_revision!=c->slots[c->current].view.revision||o->issuer_expected_policy_epoch!=t->current_policy_epoch||
     !c->issuer_stage_prepared||!papacc_configuration_issuer_projection06(&c->slots[c->pending].view.config,&c->slots[c->current].view.config))return PAPACC_RESULT_INVALID_STATE;
  o->issuer_commit_started=PAPACC_TRUE;
  result=papacc_configuration_issuer_begin06(c);
  *out=result==PAPACC_RESULT_OK?PAPACC_COMMIT_PENDING:c->pending_commit.result;
  return result;
 }
 if(c->pending_commit.active){
  result=papacc_configuration_issuer_poll06(c,&observed);*out=observed;
  if(result!=PAPACC_RESULT_OK||observed==PAPACC_COMMIT_PENDING)return result;
 }else{observed=c->pending_commit.result;*out=observed;}
 /* Terminal failure is latched: no second commit or guessed CURRENT. */
 if(observed!=PAPACC_CAS_SUCCESS||!c->committed_pending||!issuer_root_live_ready(o)||
    c->unresolved||c->authority_lost||c->stopped||
    o->issuer_expected_revision!=c->slots[c->current].view.revision||o->issuer_expected_policy_epoch!=t->current_policy_epoch||
    !papacc_configuration_issuer_projection06(&c->slots[c->pending].view.config,&c->slots[c->current].view.config))return PAPACC_RESULT_INVALID_STATE;
 EnterCriticalSection(&c->lock);
 result=papacc_configuration_issuer_publish06(c);
 if(result==PAPACC_RESULT_OK){
  t->model=c->slots[c->current].view;t->model.config.bind_selection.interface_persistent_ids=t->model.config.bind_selection.interface_persistent_id_count?t->model.ids:NULL;
  o->issuer_projection_validated=PAPACC_FALSE;o->issuer_commit_started=PAPACC_FALSE;o->issuer_expected_revision=0;o->issuer_expected_policy_epoch=0;
 }
 LeaveCriticalSection(&c->lock);return result;
}

'''+s[b:]
# The root's public outputs must not overwrite root storage. Session pins are
# intentional root-owned destinations only for current_pin, not this result.
idx=s.index('static PAPACC_ROOT_CURRENT_PIN *root_session_reference(')
s=s[:idx]+'''static int issuer06_root_output_overlap(const void *o,const void *out,PAPACC_SIZE n)
{ULONG_PTR a=(ULONG_PTR)o,b=(ULONG_PTR)out;return a<=b?b-a<sizeof(root):a-b<n;}
'''+s[idx:]
s=s.replace('o->issuer_projection_validated=PAPACC_TRUE;}','o->issuer_projection_validated=PAPACC_TRUE;o->issuer_commit_started=PAPACC_FALSE;}',1)
s=s.replace('if(c->prepared||c->committed_pending||!papacc_configuration_issuer_projection06','if(c->prepared||c->committed_pending||c->pending_commit.active||!papacc_configuration_issuer_projection06',1)
save(p,s)
p='apps/server/server_launcher_owner_win32.h';s=load(p).replace('PAPACC_BOOL issuer_projection_validated;','PAPACC_BOOL issuer_projection_validated,issuer_commit_started;',1).replace('papacc_server_launcher_configuration_issuer_publish_live(PAPACC_SERVER_LAUNCHER_OWNER_WIN32*);','papacc_server_launcher_configuration_issuer_publish_live(PAPACC_SERVER_LAUNCHER_OWNER_WIN32*,PAPACC_ISSUER_CAS_RESULT*);',1);save(p,s)
p='apps/server/server_configuration_issuer06_win32.inc';s=load(p);s=s.replace('!o->slots[o->current].operational_epoch||','pending_blocks(o)||!o->slots[o->current].operational_epoch||',1);save(p,s)
print('Reconciled three conflicts, retained pending guards, replaced synchronous publication draft with begin/poll')
