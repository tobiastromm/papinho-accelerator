#include "authentication_authorization.h"

#include <string.h>

#define CHECK(x,n) do { if (!(x)) return (n); } while (0)
typedef struct FIXTURE { PAPACC_U8 disabled; PAPACC_U8 mode; } FIXTURE;

static PAPACC_RESULT resolve(void *p, const PAPACC_PEER_EVIDENCE *e,
    PAPACC_PRINCIPAL_RESOLUTION *o, PAPACC_PRINCIPAL *principal)
{
    FIXTURE *f=(FIXTURE *)p; PAPACC_U8 key=e->certificate_sha256[0];
    if(f->mode==2U){*o=PAPACC_PRINCIPAL_RESOLVER_ERROR;return PAPACC_RESULT_INTERNAL_ERROR;}
    if(key==9U){*o=PAPACC_PRINCIPAL_NOT_ENROLLED;return PAPACC_RESULT_OK;}
    if(key==f->disabled){*o=PAPACC_PRINCIPAL_CREDENTIAL_DISABLED;return PAPACC_RESULT_OK;}
    *principal=(PAPACC_PRINCIPAL)PAPACC_PRINCIPAL_INITIALIZER;
    principal->valid=PAPACC_TRUE; principal->value[0]=(key==1U||key==2U)?7U:key;
    *o=PAPACC_PRINCIPAL_RESOLVED; return PAPACC_RESULT_OK;
}
static PAPACC_RESULT authorize(void *p,const PAPACC_PRINCIPAL *principal,
    PAPACC_AUTHORIZATION_ACTION action,PAPACC_AUTHORIZATION_DECISION *d)
{
    FIXTURE *f=(FIXTURE *)p; (void)action;
    if(f->mode==3U){*d=PAPACC_AUTHORIZATION_ERROR;return PAPACC_RESULT_INTERNAL_ERROR;}
    *d=(principal->value[0]==7U&&f->mode==0U)?PAPACC_AUTHORIZATION_ALLOW:PAPACC_AUTHORIZATION_DENY;
    return PAPACC_RESULT_OK;
}
static PAPACC_PEER_EVIDENCE evidence(PAPACC_U8 key)
{
    PAPACC_PEER_EVIDENCE e=PAPACC_PEER_EVIDENCE_INITIALIZER;
    e.certificate_present=e.chain_validated=e.peer_authenticated=e.certificate_sha256_valid=PAPACC_TRUE;
    e.certificate_sha256[0]=key; return e;
}
int main(void)
{
    FIXTURE f={3U,0U}; PAPACC_CONNECTION_SECURITY_CONTEXT c=PAPACC_CONNECTION_SECURITY_CONTEXT_INITIALIZER;
    PAPACC_SESSION_SECURITY_CONTEXT s=PAPACC_SESSION_SECURITY_CONTEXT_INITIALIZER,s2=PAPACC_SESSION_SECURITY_CONTEXT_INITIALIZER;
    PAPACC_PRINCIPAL_RESOLUTION r; PAPACC_AUTHORIZATION_DECISION d; PAPACC_PEER_EVIDENCE a=evidence(1U),b=evidence(2U),bad=evidence(9U); PAPACC_PRINCIPAL p;
    CHECK(papacc_authenticate_and_authorize(&c,&a,resolve,&f,authorize,&f,PAPACC_AUTHORIZATION_CREATE_CONTROL_SESSION,&r,&d)==PAPACC_RESULT_OK&&c.state==PAPACC_SECURITY_CONTEXT_AUTHORIZED,1);
    p=c.principal; CHECK(memcmp(a.certificate_sha256,p.value,PAPACC_PRINCIPAL_VALUE_SIZE)!=0,2);
    CHECK(papacc_session_security_context_publish(&s,&c)==PAPACC_RESULT_OK,3);
    papacc_connection_security_context_release(&c);
    CHECK(papacc_authenticate_and_authorize(&c,&b,resolve,&f,authorize,&f,PAPACC_AUTHORIZATION_CREATE_CONTROL_SESSION,&r,&d)==PAPACC_RESULT_OK&&papacc_principal_equal(&p,&c.principal),4);
    CHECK(papacc_session_security_context_publish(&s2,&c)==PAPACC_RESULT_OK&&papacc_principal_equal(&s.principal,&s2.principal),5);
    CHECK(papacc_authenticate_and_authorize(&c,&bad,resolve,&f,authorize,&f,PAPACC_AUTHORIZATION_CREATE_CONTROL_SESSION,&r,&d)==PAPACC_RESULT_OK&&c.state==PAPACC_SECURITY_CONTEXT_FAILED&&!c.principal.valid,6);
    f.disabled=2U; CHECK(papacc_authenticate_and_authorize(&c,&b,resolve,&f,authorize,&f,PAPACC_AUTHORIZATION_CREATE_CONTROL_SESSION,&r,&d)==PAPACC_RESULT_OK&&r==PAPACC_PRINCIPAL_CREDENTIAL_DISABLED&&c.state==PAPACC_SECURITY_CONTEXT_FAILED,7);
    f.disabled=3U; f.mode=1U; CHECK(papacc_authenticate_and_authorize(&c,&a,resolve,&f,authorize,&f,PAPACC_AUTHORIZATION_CREATE_CONTROL_SESSION,&r,&d)==PAPACC_RESULT_OK&&d==PAPACC_AUTHORIZATION_DENY&&c.state==PAPACC_SECURITY_CONTEXT_FAILED,8);
    f.mode=3U; CHECK(papacc_authenticate_and_authorize(&c,&a,resolve,&f,authorize,&f,PAPACC_AUTHORIZATION_CREATE_CONTROL_SESSION,&r,&d)==PAPACC_RESULT_OK&&c.state==PAPACC_SECURITY_CONTEXT_FAILED,9);
    CHECK(papacc_authenticate_and_authorize(&c,&a,NULL,&f,authorize,&f,PAPACC_AUTHORIZATION_CREATE_CONTROL_SESSION,&r,&d)==PAPACC_RESULT_OK&&c.state==PAPACC_SECURITY_CONTEXT_FAILED,10);
    a.chain_validated=PAPACC_FALSE; CHECK(papacc_authenticate_and_authorize(&c,&a,resolve,&f,authorize,&f,PAPACC_AUTHORIZATION_CREATE_CONTROL_SESSION,&r,&d)==PAPACC_RESULT_OK&&c.state==PAPACC_SECURITY_CONTEXT_FAILED,11);
    papacc_connection_security_context_release(&c); papacc_session_security_context_release(&s); papacc_session_security_context_release(&s2); return 0;
}
