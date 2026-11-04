#ifndef PAPACC_SERVER_SERVICE_DATA_H
#define PAPACC_SERVER_SERVICE_DATA_H
/* Private C89 DATA. No OS identity/authorization or resource release. */
enum { PS_INSTALL=1,PS_START,PS_STOP,PS_STATUS,PS_AUTOMATIC,PS_MANUAL,PS_REMOVE };
/* Explicit command selection is DATA, never permission to register/start. */
enum { PS_INSTALL_PRODUCT=0,PS_INSTALL_SCQ_CLOSED=1 };
enum { PS_START_PRODUCT=0,PS_START_SCQ_CLOSED=1 };
/* Private transient discovery DATA. Never an approval, token or remote ref.
 * No runtime PID is encoded before that process exists. Fixed lower-case hex
 * avoids console/argv quoting ambiguity; no persisted/PACC representation. */
#define PS_SCQ_CARRIER_BYTES 308U
#define PS_SCQ_CARRIER_CHARS (5U+2U*PS_SCQ_CARRIER_BYTES+1U)
typedef struct PS_SCQ_CARRIER {
 unsigned controller_pid,controller_creation[2],until[2],runtime_sid_bytes;
 unsigned char attempt[32],policy[32],target[32],request[32],decision[32],image[32];
 unsigned char endpoint[16],runtime_sid[68];
 unsigned global_until[2]; /* transient Start argument; NOT encoded in SCQ1 */
} PS_SCQ_CARRIER;
int ps_scq_carrier_encode(const PS_SCQ_CARRIER *,char *,unsigned);
int ps_scq_carrier_decode(const char *,unsigned,PS_SCQ_CARRIER *);
int ps_scq_carrier_equal(const PS_SCQ_CARRIER *,const PS_SCQ_CARRIER *);
/* G3 DATA-only timeout arithmetic: earlier original/global expiry, no renewal.
 * Does not validate provenance, grant permission or imply quiescence. */
int ps_scq_remaining_ms(const PS_SCQ_CARRIER *,unsigned,unsigned,unsigned *);
enum { PS_EMPTY=0,PS_START_PENDING,PS_RUNNING,PS_STOP_PENDING,PS_RETAINED,PS_STOPPED };
typedef struct PS_REQUEST { unsigned action,prepare_lab,install_profile,start_profile; char account[128],installation[260],configuration[65]; } PS_REQUEST;
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
