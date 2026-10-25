/* SAME canonical owner; private auxiliary storage, no admission authority. */
static A1_OWNER lab_aux_owner;
#define lab_aux_retained lab_aux_owner.handle
#define lab_aux_count (!a1_finish(&lab_aux_owner))
static DWORD lab_aux_sync(DWORD e)
{
 if(lab_aux_owner.state&A1_STOP){
  papacc_server_console_win32_request_stop();
#ifdef PAPACC_SINGLE_LAB_ADMISSION
  if(native.admission.phase==1)(void)papacc_server_service_admission_stop();
#endif
 }
 if(lab_aux_owner.state&A1_FROZEN)native.retained=1;
 /* DO NOT terminalize canonical phase before its separately authorized safe Stop. */
 return (lab_aux_owner.state&A1_STOP)&&lab_aux_owner.first_error?lab_aux_owner.first_error:e;
}
static int lab_aux_control_thread(void)
{
 if(native.thread&&native.thread!=GetCurrentThreadId())return 0;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(native.admission.phase&&native.admission.thread!=GetCurrentThreadId())return 0;
#endif
 return 1; /* Before enter, only the synchronous qualifier entry calls here. */
}
static DWORD lab_aux_gate(void)
{
 if(!lab_aux_control_thread())return ERROR_ACCESS_DENIED;
#ifdef PAPACC_SINGLE_LAB_ADMISSION
 if(native.admission.phase==3&&!(lab_aux_owner.state&A1_FROZEN))a1_retain(&lab_aux_owner,ERROR_INVALID_STATE);
#endif
 if(native.retained&&!(lab_aux_owner.state&A1_FROZEN))a1_retain(&lab_aux_owner,ERROR_INVALID_STATE);
 if(lab_aux_owner.state&A1_FROZEN)return lab_aux_sync(ERROR_INVALID_STATE);
 if(papacc_server_console_win32_stop_requested())a1_stop(&lab_aux_owner);
 {DWORD e=a1_gate(&lab_aux_owner);if(e)(void)a1_deny(&lab_aux_owner,e);return lab_aux_sync(e);}
}
typedef struct {HANDLE target;LPCWSTR path;DWORD kind,access,share,disposition,flags;} LAB_AUX_REQUEST;
static DWORD lab_aux_api(void *arg,HANDLE *out)
{
 LAB_AUX_REQUEST *r=(LAB_AUX_REQUEST *)arg;DWORD e;
 if(r->kind==0){if(OpenThreadToken(r->target,r->access,TRUE,out))return 0;e=GetLastError();return e?e:ERROR_GEN_FAILURE;}
 if(r->kind==1){if(OpenProcessToken(r->target,r->access,out))return 0;e=GetLastError();return e?e:ERROR_GEN_FAILURE;}
 *out=CreateFileW(r->path,r->access,r->share,NULL,r->disposition,r->flags,NULL);
 if(*out!=INVALID_HANDLE_VALUE)return 0;
 e=GetLastError();*out=NULL; /* documented CreateFile failure sentinel, no real ref */
 return e?e:ERROR_GEN_FAILURE;
}
static DWORD lab_aux_open_reserved(A1_LEASE l,LAB_AUX_REQUEST *r)
{
 DWORD e=lab_aux_gate();if(e){if(!(lab_aux_owner.state&A1_FROZEN))(void)a1_cancel(&lab_aux_owner,l);return e;}
 return lab_aux_sync(a1_acquire(&lab_aux_owner,l,lab_aux_api,r,r->kind==0?ERROR_NO_TOKEN:0));
}
static DWORD lab_aux_token(A1_LEASE *l,HANDLE target,int thread,DWORD access)
{
 LAB_AUX_REQUEST r;DWORD e=lab_aux_gate();if(e)return e;
 ZeroMemory(&r,sizeof(r));r.target=target;r.kind=thread?0U:1U;r.access=access;
 e=a1_reserve(&lab_aux_owner,l);if(e)return lab_aux_sync(e);return lab_aux_open_reserved(*l,&r);
}
static DWORD lab_aux_file(A1_LEASE *l,LPCWSTR path,DWORD access,DWORD share,DWORD disposition,DWORD flags)
{
 LAB_AUX_REQUEST r;DWORD e=lab_aux_gate();if(e)return e;
 ZeroMemory(&r,sizeof(r));r.kind=2;r.path=path;r.access=access;r.share=share;r.disposition=disposition;r.flags=flags;
 e=a1_reserve(&lab_aux_owner,l);if(e)return lab_aux_sync(e);return lab_aux_open_reserved(*l,&r);
}
static DWORD lab_aux_borrow(A1_LEASE l,HANDLE *out)
{
 DWORD e=lab_aux_gate();*out=NULL;if(e)return e;
 return lab_aux_sync(a1_borrow_begin(&lab_aux_owner,l,out));
}
static DWORD lab_aux_borrow_pair(A1_LEASE a,A1_LEASE b,HANDLE *in,HANDLE *out)
{
 DWORD e=lab_aux_gate();*in=*out=NULL;if(e)return e;
 return lab_aux_sync(a1_borrow_pair_begin(&lab_aux_owner,a,b,in,out));
}
static DWORD lab_aux_borrow_end(DWORD e)
{if(!lab_aux_control_thread())return ERROR_ACCESS_DENIED;return lab_aux_sync(a1_borrow_end(&lab_aux_owner,e));}
static DWORD lab_aux_native_close(void *ignored,HANDLE h)
{DWORD e;(void)ignored;if(CloseHandle(h))return 0;e=GetLastError();return e?e:ERROR_INVALID_HANDLE;}
static DWORD lab_aux_close_preserving(A1_LEASE l,DWORD first)
{
 DWORD e;if(!lab_aux_control_thread())return ERROR_ACCESS_DENIED;(void)a1_note(&lab_aux_owner,first);
 if(native.retained&&!(lab_aux_owner.state&A1_FROZEN))a1_retain(&lab_aux_owner,first?first:ERROR_INVALID_STATE);
 e=lab_aux_sync(a1_close(&lab_aux_owner,l,lab_aux_native_close,NULL));
 return first?first:e;
}
static DWORD lab_aux_console(A1_LEASE *in,A1_LEASE *out)
{
 LAB_AUX_REQUEST r;DWORD e=lab_aux_gate();if(e)return e;
 e=a1_reserve_pair(&lab_aux_owner,in,out);if(e)return lab_aux_sync(e);
 ZeroMemory(&r,sizeof(r));r.kind=2;r.path=L"CONIN$";r.access=GENERIC_READ;r.share=FILE_SHARE_READ|FILE_SHARE_WRITE;r.disposition=OPEN_EXISTING;
 e=lab_aux_open_reserved(*in,&r);
 if(e){if(!(lab_aux_owner.state&A1_FROZEN))(void)a1_cancel(&lab_aux_owner,*out);return e;}
 r.path=L"CONOUT$";r.access=GENERIC_READ|GENERIC_WRITE;e=lab_aux_open_reserved(*out,&r);
 return e?lab_aux_close_preserving(*in,e):0;
}
