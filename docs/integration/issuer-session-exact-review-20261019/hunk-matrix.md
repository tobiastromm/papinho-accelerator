# Exact hunk matrix — PROPOSED / NOT APPLIED

Safety/reference foundation only. NOT a complete live-publication patch; NOT submitted. Whole-file preimage SHA256 plus exact sequential snippets identify each hunk. H17 appends; no generated code was compiled or executed.

## H01 — root_cohort_preflight

File: `apps/server/server_admission_preflight_win32.inc`

Preimage SHA256: `675f92281ae8a3a2344c28d67376f3b48498dfc5e09437cb97cf6a2e1d16f3fb`

Authority: C2 revision1 section4, policy_epoch rule (line223); ADR0012; exact-review section5

Before:
```c
    root_snapshot_held(o,pin)&&
```

Proposed:
```c
    papacc_server_launcher_configuration_current(o,pin)&&root_snapshot_held(o,pin)&&
```

Static argument: CURRENT remains independently required; held pin is an additional necessary memory-lifetime check, never sufficient authority

Required regressions (NOT RUN): Old pin/new admission deny; current pin without leases deny; revoked policy deny

ED11: No new persistent storage; compiled stack verification pending

## H02 — issuer06_overlap

File: `apps/server/server_configuration_issuer06_win32.inc`

Preimage SHA256: `223f7270a8344285e7ee5ce4b2adb91f8fc7860ed4cf6ff4ca6c71ac4e41f3ef`

Authority: Model E ownership; exact-review section6

Before:
```c
PAPACC_RESULT papacc_configuration_issuer_snapshot06(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_CONFIG_PIN *p,PAPACC_U8 *out)
```

Proposed:
```c
/* Checked interval comparisons avoid address-addition overflow. */
static int issuer06_overlap(const void *a,PAPACC_SIZE na,const void *b,PAPACC_SIZE nb)
{
 ULONG_PTR x=(ULONG_PTR)a,y=(ULONG_PTR)b;
 return na&&nb&&(x<=y?y-x<na:x-y<nb);
}
PAPACC_RESULT papacc_configuration_issuer_snapshot06(PAPACC_CONFIGURATION_COMMIT *o,const PAPACC_CONFIG_PIN *p,PAPACC_U8 *out)
```

Static argument: Backend-only arithmetic; no dereference/allocation/native effect

Required regressions (NOT RUN): Overlap both directions, adjacent/identical ranges and no wrap

ED11: No new persistent storage; compiled stack verification pending

## H03 — papacc_configuration_issuer_snapshot06

File: `apps/server/server_configuration_issuer06_win32.inc`

Preimage SHA256: `223f7270a8344285e7ee5ce4b2adb91f8fc7860ed4cf6ff4ca6c71ac4e41f3ef`

Authority: Model E immutable protected snapshots

Before:
```c
 if(!owner(o)||!p||!out||p->issuer!=o||p->index>=PAPACC_CONFIG_SLOTS)return PAPACC_RESULT_INVALID_ARGUMENT;
```

Proposed:
```c
 if(!owner(o)||!p||!out||p->issuer!=o||p->index>=PAPACC_CONFIG_SLOTS||
    issuer06_overlap(out,PAPACC_ISSUER_IMAGE_BYTES,o,sizeof(*o))||
    issuer06_overlap(out,PAPACC_ISSUER_IMAGE_BYTES,p,sizeof(*p)))return PAPACC_RESULT_INVALID_ARGUMENT;
```

Static argument: Reject output aliasing live owner or input pin before writes; keep all ticket/serial guards

Required regressions (NOT RUN): Owner/input alias deny; stale serial; duplicate ticket; Stop copy; retained-slot reuse deny

ED11: No new persistent storage; compiled stack verification pending

## H04 — papacc_configuration_pin_read

File: `apps/server/server_configuration_issuer06_win32.inc`

Preimage SHA256: `223f7270a8344285e7ee5ce4b2adb91f8fc7860ed4cf6ff4ca6c71ac4e41f3ef`

Authority: Model E; data-only snapshot contract

Before:
```c
 if(!owner(o)||!p||!v||!epoch||p->issuer!=o||p->index>=PAPACC_CONFIG_SLOTS)return PAPACC_RESULT_INVALID_ARGUMENT;
```

Proposed:
```c
 if(!owner(o)||!p||!v||!epoch||p->issuer!=o||p->index>=PAPACC_CONFIG_SLOTS||
    issuer06_overlap(v,sizeof(*v),o,sizeof(*o))||issuer06_overlap(epoch,sizeof(*epoch),o,sizeof(*o))||
    issuer06_overlap(v,sizeof(*v),p,sizeof(*p))||issuer06_overlap(epoch,sizeof(*epoch),p,sizeof(*p))||
    issuer06_overlap(v,sizeof(*v),epoch,sizeof(*epoch)))return PAPACC_RESULT_INVALID_ARGUMENT;
```

Static argument: Prevent output views/epoch from overwriting slot or ticket; no proof bits returned; fail before copying

Required regressions (NOT RUN): Aliased outputs/owner/input deny; forged/stale/unpinned tokens; failed owner close denies

ED11: No new persistent storage; compiled stack verification pending

## H05 — papacc_configuration_issuer_publish06 entry

File: `apps/server/server_configuration_issuer06_win32.inc`

Preimage SHA256: `223f7270a8344285e7ee5ce4b2adb91f8fc7860ed4cf6ff4ca6c71ac4e41f3ef`

Authority: ADR0048 normative root commit; accepted CA technical profile2.2/2.5

Before:
```c
 PAPACC_U64 epoch;unsigned i;PAPACC_RESULT r=PAPACC_RESULT_INVALID_STATE;
 if(!owner(o))return r;EnterCriticalSection(&o->lock);
```

Proposed:
```c
#ifndef PAPACC_ISSUER_STORAGE_COMPONENT_TEST
 /* Normal component publication remains unavailable pending approval of
  * the complete application CURRENT/epoch and actor transaction proof. */
 (void)o;return PAPACC_RESULT_NOT_SUPPORTED;
#else
 PAPACC_U64 epoch;unsigned i;PAPACC_RESULT r=PAPACC_RESULT_INVALID_STATE;
 if(!owner(o))return r;EnterCriticalSection(&o->lock);
```

Static argument: Normal library returns before RNG or CURRENT mutation; only existing isolated storage fixture can examine the lower component

Required regressions (NOT RUN): Normal library denied before state access; no native RNG when blocked; component not operational authority

ED11: No new persistent storage; compiled stack verification pending

## H06 — papacc_configuration_issuer_publish06 exit

File: `apps/server/server_configuration_issuer06_win32.inc`

Preimage SHA256: `223f7270a8344285e7ee5ce4b2adb91f8fc7860ed4cf6ff4ca6c71ac4e41f3ef`

Authority: Same as H05

Before:
```c
done:LeaveCriticalSection(&o->lock);return r;
}
PAPACC_RESULT papacc_configuration_issuer_copy06
```

Proposed:
```c
done:LeaveCriticalSection(&o->lock);return r;
#endif
}
PAPACC_RESULT papacc_configuration_issuer_copy06
```

Static argument: Closes compile-time branch; no product-selection fallback

Required regressions (NOT RUN): Both build profiles compile; inspect symbol/call tree and unchanged application DENY

ED11: No new persistent storage; compiled stack verification pending

## H07 — papacc_server_launcher_c2_session_current locals

File: `apps/server/server_c2_root_binding_win32.inc`

Preimage SHA256: `904ea7435ffa93f70820085653ed2d3cd7ea8af97d841fab7ed8ca8a683d1d96`

Authority: Owner approved Session-reference model

Before:
```c
 PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *c;PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot;unsigned i;int owned=0;
```

Proposed:
```c
 PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *c;PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot;PAPACC_ROOT_CURRENT_PIN *admitted=NULL;unsigned i;int owned=0;
```

Static argument: Pointer to reserved existing root array only; not a new authority

Required regressions (NOT RUN): Foreign Session address deny

ED11: No new persistent storage; compiled stack verification pending

## H08 — papacc_server_launcher_c2_session_current membership

File: `apps/server/server_c2_root_binding_win32.inc`

Preimage SHA256: `904ea7435ffa93f70820085653ed2d3cd7ea8af97d841fab7ed8ca8a683d1d96`

Authority: Model E; C2 exact Session membership

Before:
```c
 for(i=0U;i<4U;++i)if(s==&o->sessions[i])owned=1;
```

Proposed:
```c
 for(i=0U;i<4U;++i)if(s==&o->sessions[i]){owned=1;admitted=&root.session_revision_pins[i];}
```

Static argument: Map actual owned object, not externally supplied index or local Session ID

Required regressions (NOT RUN): Reused slot/foreign pointer; incarnation/domain mismatch

ED11: No new persistent storage; compiled stack verification pending

## H09 — papacc_server_launcher_c2_session_current guard

File: `apps/server/server_c2_root_binding_win32.inc`

Preimage SHA256: `904ea7435ffa93f70820085653ed2d3cd7ea8af97d841fab7ed8ca8a683d1d96`

Authority: C2 section4, policy_epoch rule (line223) plus ADR0012

Before:
```c
 if(!owned||!s->initialized||!s->root_bound||s->principal!=0U||s->assurance!=1U||
```

Proposed:
```c
 if(!owned||!admitted||!root_snapshot_held(&root,admitted)||
    admitted->epoch!=s->policy_epoch||admitted->view.revision!=s->root_revision||
    memcmp(admitted->view.domain,s->root_domain,32U)||
    !s->initialized||!s->root_bound||s->principal!=0U||s->assurance!=1U||
```

Static argument: Adds bound-revision checks; all original listener CURRENT/epoch/identity/Principal/negotiation/grant checks below remain untouched. Does NOT enable old Session new operations after epoch change

Required regressions (NOT RUN): Old pin cannot authorize new operation; lost pin/current policy revoke deny; unchanged current Session behavior

ED11: No new persistent storage; compiled stack verification pending

## H10 — papacc_server_launcher_c2_adopt locals

File: `apps/server/server_c2_root_binding_win32.inc`

Preimage SHA256: `904ea7435ffa93f70820085653ed2d3cd7ea8af97d841fab7ed8ca8a683d1d96`

Authority: Owner approved binding

Before:
```c
 PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot;PAPACC_C2_SESSION *s=NULL;PAPACC_RESULT result;unsigned i;
```

Proposed:
```c
 PAPACC_SESSION_CAPABILITY_SNAPSHOT *snapshot;PAPACC_C2_SESSION *s=NULL;PAPACC_ROOT_CURRENT_PIN *admitted;PAPACC_RESULT result;unsigned i;
```

Static argument: No additional grant or callback

Required regressions (NOT RUN): Build and inspect stack

ED11: No new persistent storage; compiled stack verification pending

## H11 — papacc_server_launcher_c2_adopt acquisition

File: `apps/server/server_c2_root_binding_win32.inc`

Preimage SHA256: `904ea7435ffa93f70820085653ed2d3cd7ea8af97d841fab7ed8ca8a683d1d96`

Authority: C2 CURRENT and factual full RESULT; Model E

Before:
```c
 if(!s)return PAPACC_RESULT_LIMIT_EXCEEDED;
```

Proposed:
```c
 if(!s)return PAPACC_RESULT_LIMIT_EXCEEDED;
 admitted=&root.session_revision_pins[(unsigned)(s-o->sessions)];
 if(admitted->pin.ticket)return PAPACC_RESULT_INVALID_STATE;
 result=papacc_server_launcher_configuration_current_pin(&root,admitted);
 if(result!=PAPACC_RESULT_OK)return result;
 if(!papacc_server_launcher_configuration_current(&root,admitted)||
    admitted->epoch!=root.listener_composition.pin.epoch||
    admitted->view.revision!=root.listener_composition.pin.view.revision||
    memcmp(admitted->view.domain,root.listener_composition.pin.view.domain,32U)){
  if(papacc_server_launcher_configuration_current_unpin(&root,admitted)==PAPACC_RESULT_OK)memset(admitted,0,sizeof(*admitted));
  /* Failed release retains the root-owned ticket; no Session is exposed. */
  return PAPACC_RESULT_INVALID_STATE;
 }
```

Static argument: Acquire only after original authentication/negotiation/membership but before Session initialization or manager attachment. A leftover pin blocks reuse. CURRENT and listener epoch both mandatory

Required regressions (NOT RUN): Absent CURRENT; pin limit; stale listener; attach denied; failed pre-exposure unpin retains ticket; no grants fabricated

ED11: No new persistent storage; compiled stack verification pending

## H12 — papacc_server_launcher_c2_adopt epoch capture

File: `apps/server/server_c2_root_binding_win32.inc`

Preimage SHA256: `904ea7435ffa93f70820085653ed2d3cd7ea8af97d841fab7ed8ca8a683d1d96`

Authority: C2 section4, policy_epoch rule (line223)

Before:
```c
 s->policy_epoch=root.listener_composition.pin.epoch;
```

Proposed:
```c
 s->policy_epoch=admitted->epoch;
```

Static argument: H11 proved equality to real current/listener epoch; no caller epoch or transformation

Required regressions (NOT RUN): Racing publication before capture denies rather than substituting epoch

ED11: No new persistent storage; compiled stack verification pending

## H13 — papacc_server_launcher_c2_adopt domain

File: `apps/server/server_c2_root_binding_win32.inc`

Preimage SHA256: `904ea7435ffa93f70820085653ed2d3cd7ea8af97d841fab7ed8ca8a683d1d96`

Authority: Same factual snapshot

Before:
```c
 memcpy(s->root_domain,root.listener_composition.pin.view.domain,32U);
```

Proposed:
```c
 memcpy(s->root_domain,admitted->view.domain,32U);
```

Static argument: Domain comes from acquired pin, not untrusted peer

Required regressions (NOT RUN): Wrong root domain deny

ED11: No new persistent storage; compiled stack verification pending

## H14 — papacc_server_launcher_c2_adopt revision

File: `apps/server/server_c2_root_binding_win32.inc`

Preimage SHA256: `904ea7435ffa93f70820085653ed2d3cd7ea8af97d841fab7ed8ca8a683d1d96`

Authority: Same factual snapshot

Before:
```c
 s->root_invocation=root.invocation;s->root_revision=root.listener_composition.pin.view.revision;
```

Proposed:
```c
 s->root_invocation=root.invocation;s->root_revision=admitted->view.revision;
```

Static argument: Capture exact protected revision; original Principal and grants assignments unchanged

Required regressions (NOT RUN): Revision mismatch and slot reuse deny

ED11: No new persistent storage; compiled stack verification pending

## H15 — papacc_server_launcher_c2_adopt failure

File: `apps/server/server_c2_root_binding_win32.inc`

Preimage SHA256: `904ea7435ffa93f70820085653ed2d3cd7ea8af97d841fab7ed8ca8a683d1d96`

Authority: Model E retain uncertain ownership

Before:
```c
 if(result!=PAPACC_RESULT_OK){SecureZeroMemory(s,sizeof(*s));return result;}
```

Proposed:
```c
 if(result!=PAPACC_RESULT_OK){
  if(papacc_server_launcher_configuration_current_unpin(&root,admitted)!=PAPACC_RESULT_OK){s->state=PAPACC_C2_SESSION_RETAINED;return result;}
  memset(admitted,0,sizeof(*admitted));SecureZeroMemory(s,sizeof(*s));return result;
 }
```

Static argument: Clear Session only after factual unpin; failure leaves initialized RETAINED metadata and owned pin, no implicit retry

Required regressions (NOT RUN): Attach failure with successful/failed unpin; retained pin charged and no reuse

ED11: No new persistent storage; compiled stack verification pending

## H16 — private reaper/root declaration

File: `apps/server/server_c2_owner_win32.h`

Preimage SHA256: `2aa684c2780e72325cffe7b7e35f1f5502a42f25680b380b544716f9333817fa`

Authority: Same owner only

Before:
```c
PAPACC_BOOL papacc_server_launcher_c2_session_current(const PAPACC_C2_SESSION *);
```

Proposed:
```c
PAPACC_BOOL papacc_server_launcher_c2_session_current(const PAPACC_C2_SESSION *);
PAPACC_RESULT papacc_server_launcher_c2_session_unpin(PAPACC_SERVER_C2_OWNER_WIN32 *,const PAPACC_C2_SESSION *);
```

Static argument: No public wire/API or callback registration

Required regressions (NOT RUN): Private profile build; nonprivate build unaffected

ED11: No new persistent storage; compiled stack verification pending

## H17 — papacc_server_launcher_c2_session_unpin

File: `apps/server/server_c2_root_binding_win32.inc`

Preimage SHA256: `904ea7435ffa93f70820085653ed2d3cd7ea8af97d841fab7ed8ca8a683d1d96`

Authority: Model E; existing retire_sessions full quiescence barrier

Before:
```c
EOF
```

Proposed:
```c

/* Destruction-only authority. No CURRENT required to release a factual old
 * reference; every quiescence predicate is independently rechecked. */
PAPACC_RESULT papacc_server_launcher_c2_session_unpin(PAPACC_SERVER_C2_OWNER_WIN32 *o,const PAPACC_C2_SESSION *s)
{
 unsigned i,j;PAPACC_ROOT_CURRENT_PIN *p;PAPACC_CONFIG_VIEW v;PAPACC_U64 epoch;PAPACC_RESULT r;
 if(o!=&configuration_c2_owner||!preparation_owned(&root)||!s||!o->initialized||!o->manager||!o->loop||!o->secure||
    !o->loop->initialized||!o->secure->initialized||!o->manager->initialized||!o->loop->connection_manager||o->manager->owner!=GetCurrentThreadId()||
    o->secure->dispatch_active||o->configuration.state==PAPACC_C2_CONFIG_RETAINED||
    o->secure->stop_state==PAPACC_SERVER_SECURE_STOP_ERROR_TERMINAL||
    !o->loop->session_manager.initialized||!o->loop->channel_manager.initialized)return PAPACC_RESULT_INVALID_STATE;
 for(i=0;i<4;i++)if(s==&o->sessions[i])break;
 if(i==4||!s->initialized||s->state!=PAPACC_C2_SESSION_REVOKED||!s->incarnation||
    papacc_session_manager_find(&o->loop->session_manager,s->incarnation))return PAPACC_RESULT_INVALID_STATE;
 if(s->control_connection&&papacc_connection_manager_find(o->loop->connection_manager,s->control_connection))return PAPACC_RESULT_INVALID_STATE;
 for(j=0;j<2;j++)if(o->manager->operations[j].session==s)return PAPACC_RESULT_INVALID_STATE;
 for(j=0;j<6;j++)if(o->writes[j].denial_session==s||(o->writes[j].record&&o->writes[j].record->session==s))return PAPACC_RESULT_INVALID_STATE;
 for(j=0;j<o->loop->channel_manager.capacity;j++)if(o->loop->channel_manager.storage[j].state!=PAPACC_CHANNEL_STATE_UNINITIALIZED&&o->loop->channel_manager.storage[j].session_instance_id==s->incarnation)return PAPACC_RESULT_INVALID_STATE;
 for(j=0;j<o->loop->processor_capacity;j++){
  PAPACC_SERVER_PROTOCOL_SLOT_WIN32 *slot=&o->loop->processor_slots[j];
  if(slot->in_use&&((s->control_connection&&slot->connection_instance_id==s->control_connection)||(slot->kind==PAPACC_SERVER_PROTOCOL_SLOT_KIND_POST_CONTROL&&slot->post_control_processor.session_instance_id==s->incarnation)))return PAPACC_RESULT_INVALID_STATE;
 }
 for(j=0;j<o->secure->candidate_capacity;j++){
  PAPACC_SERVER_SECURE_CANDIDATE_WIN32 *c=&o->secure->candidates[j];
  if(c->control_close_result.session_instance_id==s->incarnation||(s->control_connection&&c->connection_instance_id==s->control_connection)){
   if(c->state!=PAPACC_SERVER_SECURE_CANDIDATE_CLOSED||c->secure_processor.connection||c->secure_processor.state==PAPACC_PST_SECURE_PROCESSOR_RETAINED_TERMINAL||c->session_security.published||c->transport_adapter.registered||papacc_connection_manager_find(o->loop->connection_manager,c->connection_instance_id))return PAPACC_RESULT_INVALID_STATE;
  }
 }
 p=&root.session_revision_pins[i];
 if(p->invocation!=s->root_invocation||!s->root_bound||
    papacc_configuration_pin_read(&root.configuration_commit,&p->pin,&v,&epoch)!=PAPACC_RESULT_OK||
    !epoch||epoch!=s->policy_epoch||epoch!=p->epoch||v.revision!=s->root_revision||v.revision!=p->view.revision||
    memcmp(v.domain,s->root_domain,32U)||memcmp(v.domain,p->view.domain,32U))return PAPACC_RESULT_INVALID_STATE;
 r=papacc_server_launcher_configuration_current_unpin(&root,p);
 if(r==PAPACC_RESULT_OK)memset(p,0,sizeof(*p));return r;
}

```

Static argument: No payload/admission authority; all native Session/Connection/operation/writer/channel/processor/candidate guards repeated; exact owned ticket; failure preserves Session and manager entry

Required regressions (NOT RUN): Each predicate individually fails; pending callbacks/borrowers retained; Stop alone insufficient; FINAL alone insufficient; stale/foreign pin; exactly-once release; no credit refund

ED11: Existing pin storage reused; local config view and epoch add stack copy, compiled call-chain census required

## H18 — papacc_server_c2_owner_win32_retire_sessions

File: `apps/server/server_c2_owner_win32.c`

Preimage SHA256: `30e0b35eb108b426194a94b115b701b7f16068f68c22e6bd0da535e151b77c7f`

Authority: Model E factual quiescence

Before:
```c
  if(borrowed)continue;
  for(j=0U;j<4U;++j)if(o->manager->sessions[j].session==s)
   SecureZeroMemory(&o->manager->sessions[j],sizeof(o->manager->sessions[j]));
  /* Native ID allocator
```

Proposed:
```c
  if(borrowed)continue;
  result=papacc_server_launcher_c2_session_unpin(o,s);if(result!=PAPACC_RESULT_OK)return result;
  for(j=0U;j<4U;++j)if(o->manager->sessions[j].session==s)
   SecureZeroMemory(&o->manager->sessions[j],sizeof(o->manager->sessions[j]));
  /* Native ID allocator
```

Static argument: New unpin comes after ALL original guards, before any manager/Session erasure; all prior checks retained; release failure returns with references intact

Required regressions (NOT RUN): Every original reference guard prevents release; unpin failure retains; duplicate cleanup rejected; no native release added

ED11: No new persistent storage; compiled stack verification pending