#ifndef PAPACC_SERVER_SERVICE_DATA_H
#define PAPACC_SERVER_SERVICE_DATA_H
/* Private C89 DATA. No OS identity/authorization or resource release. */
enum { PS_INSTALL=1,PS_START,PS_STOP,PS_STATUS,PS_AUTOMATIC,PS_MANUAL,PS_REMOVE };
enum { PS_EMPTY=0,PS_START_PENDING,PS_RUNNING,PS_STOP_PENDING,PS_RETAINED,PS_STOPPED };
typedef struct PS_REQUEST { unsigned action,prepare_lab; char account[128],installation[260],configuration[65]; } PS_REQUEST;
typedef struct PS_LIFETIME { unsigned phase,stop,terminal; } PS_LIFETIME;
/* -1 not this CLI, 0 malformed, 1 complete. No borrowed argv retained. */
int ps_parse(int,const char *const *,PS_REQUEST *);
int ps_begin(PS_LIFETIME *);
int ps_running(PS_LIFETIME *);
int ps_stop(PS_LIFETIME *);
/* quiescent is a caller fact, never inferred from timeout/SCM status. */
int ps_finish(PS_LIFETIME *,int quiescent,int retained);
/* Observed state is DATA, not authority. Install only absent; remove stopped. */
int ps_action_allowed(unsigned action,int exists,unsigned observed);
/* Exact immutable read-only LAB receipt, never SCM effect authorization.
 * Callers zero-initialize facts; native producers own all references. */
typedef struct PS_BINDING_FACTS {
 unsigned long installation[3],configuration[3],executable[3],token[6];
 unsigned char attempt[32],operator_id[32],config_hash[32],image_hash[32];
 unsigned char parent_acl[32],config_acl[32],image_acl[32];
 unsigned configuration_bytes;
} PS_BINDING_FACTS;
typedef struct PS_LAB_BINDING { unsigned phase;PS_BINDING_FACTS facts; } PS_LAB_BINDING;
int ps_lab_binding_capture(PS_LAB_BINDING *,const PS_BINDING_FACTS *);
int ps_lab_binding_complete(PS_LAB_BINDING *,const PS_BINDING_FACTS *,int stopped);
void ps_lab_binding_invalidate(PS_LAB_BINDING *);
#endif
