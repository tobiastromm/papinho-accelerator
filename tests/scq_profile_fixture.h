#ifndef PAPACC_SCQ_PROFILE_FIXTURE_H
#define PAPACC_SCQ_PROFILE_FIXTURE_H
#include "personal_scq_profile_win32.h"
#include <sddl.h>
#include <string.h>
/* Explicit MODEL DATA. No source acquisition, authority or protected input. */
static ULONGLONG scq_fixture_window=10000ULL;
static int scq_fixture_plan(PAPACC_SCQ_PLAN_DATA *p)
{
 PSID a=NULL,b=NULL;unsigned i;int ok=0;ULONGLONG now=GetTickCount64(),until=now+scq_fixture_window;
 memset(p,0,sizeof(*p));p->valid_from[0]=(DWORD)now;p->valid_from[1]=(DWORD)(now>>32);p->valid_until[0]=(DWORD)until;p->valid_until[1]=(DWORD)(until>>32);p->version=1;p->profile=PAPACC_SCQ_PROFILE;p->purpose=PAPACC_SCQ_PURPOSE_READ;p->schema=PAPACC_SCQ_NON_SECRET_SCHEMA;p->service=PAPACC_SCQ_SERVICE_PAPINHO;
 memset(p->attempt,1,32);memset(p->policy,2,32);memset(p->request,3,32);memset(p->decision,4,32);memset(p->target,5,32);
 strcpy_s(p->key,sizeof(p->key),"PapinhoAccelerator.CA.11111111111111111111111111111111.22222222222222222222222222222222");
 if(!ConvertStringSidToSidA("S-1-5-21-1-2-3-1001",&a)||!ConvertStringSidToSidA("S-1-5-80-1-2-3-4-5",&b))goto done;
 p->administrator.sid_bytes=GetLengthSid(a);p->runtime.sid_bytes=GetLengthSid(b);
 if(!CopySid(68,p->administrator.sid,a)||!CopySid(68,p->runtime.sid,b))goto done;
 p->administrator.pid=101;p->administrator.creation[0]=1001;p->administrator.token_id[0]=11;p->administrator.authentication[0]=12;p->administrator.modified[0]=13;
 p->administrator.groups=p->administrator.enumerated=2;p->administrator.elevated=1;p->administrator.elevation_type=TokenElevationTypeFull;
 p->administrator.matches=1;p->administrator.attributes=SE_GROUP_ENABLED;p->administrator.token_type=TokenPrimary;p->administrator.wait_status=WAIT_TIMEOUT;p->administrator.exit_code=STILL_ACTIVE;p->administrator.session=1;p->administrator.logon=2;p->administrator.administrator_attributes=SE_GROUP_ENABLED;
 p->runtime.pid=202;p->runtime.creation[0]=2002;p->runtime.token_id[0]=21;p->runtime.authentication[0]=22;p->runtime.modified[0]=23;
 p->runtime.groups=p->runtime.enumerated=7;p->runtime.elevation_type=TokenElevationTypeDefault;p->runtime.token_type=TokenPrimary;p->runtime.wait_status=WAIT_TIMEOUT;p->runtime.exit_code=STILL_ACTIVE;p->runtime.logon=1;p->runtime.service_type=SERVICE_WIN32_OWN_PROCESS;p->runtime.service_state=SERVICE_RUNNING;p->runtime.service_sid_type=1;
 for(i=0;i<3;i++){p->refs[i].role=i+1;p->refs[i].rights=PAPACC_SCQ_FILE_READ_RIGHTS;p->refs[i].volume=42;p->refs[i].low=501+i;
  p->refs[i].size_low=4096;p->refs[i].attributes=FILE_ATTRIBUTE_ARCHIVE;p->refs[i].links=1;memset(p->refs[i].digest,(int)i+1,32);memset(p->refs[i].descriptor,(int)i+4,32);}
 p->administrator.image[0]=42;p->administrator.image[2]=900;memset(p->administrator.image_digest,9,32);
 p->runtime.image[0]=p->refs[2].volume;p->runtime.image[1]=p->refs[2].high;p->runtime.image[2]=p->refs[2].low;memcpy(p->runtime.image_digest,p->refs[2].digest,32);
 ok=1;
done:if(a&&LocalFree(a))ok=0;if(b&&LocalFree(b))ok=0;return ok;
}
#endif
