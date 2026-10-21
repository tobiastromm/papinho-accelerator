#include "server_config_source_binding_win32.h"
#include <stdio.h>
#include <string.h>
static PC_DOCUMENT doc;static PC_MODEL_BINDING out;
static PAPACC_SERVER_LAUNCHER_OWNER_WIN32 fake_owner;static PAPACC_ROOT_CURRENT_PIN fake_pin;
int main(void)
{PAPACC_INTERFACE_CURRENT_REPORT report;PC_DIAGNOSTIC d;unsigned checks=0U;const char *text="format_version 1\n";PC_TARGET t={1U,0};
#define CHECK(x) do{++checks;if(!(x)){printf("FAIL line=%u\n",(unsigned)__LINE__);return 1;}}while(0)
 CHECK(pc_parse((const unsigned char *)text,strlen(text),&doc,&d)==PC_OK);
 CHECK(pc_validate(&doc,&t,&d)==PC_OK);
 CHECK(papacc_server_config_source_resolve(NULL,NULL,0U,&doc,&out,&d)!=PC_OK&&!out.resolved);
 fake_pin.view.revision=1U;fake_pin.epoch=1U;fake_pin.administrative_epoch=1U;
 CHECK(papacc_server_config_source_resolve(&fake_owner,&fake_pin,1U,&doc,&out,&d)!=PC_OK&&!out.resolved);
 fake_owner.configuration_preparation.participants.interface_current.bound=PAPACC_TRUE;
 CHECK(papacc_server_launcher_configuration_interface_current(&fake_owner,&fake_pin,&report)!=PAPACC_RESULT_OK&&!report.current_bound&&!report.resolved&&!report.topology_proven);
 {PAPACC_U64 before=fake_pin.view.revision;
  CHECK(papacc_server_launcher_configuration_interface_current(&fake_owner,&fake_pin,(PAPACC_INTERFACE_CURRENT_REPORT *)&fake_pin)==PAPACC_RESULT_INVALID_ARGUMENT&&fake_pin.view.revision==before);
 }
 CHECK(papacc_server_launcher_interface_consumption_check(NULL,NULL)!=PAPACC_RESULT_OK);
 CHECK(papacc_server_launcher_interface_consumption_check(&fake_owner,&fake_pin)!=PAPACC_RESULT_OK);
 CHECK(!fake_owner.configuration_commit.initialized&&!fake_owner.network.is_active);
 fake_owner.configuration_commit.stopped=1;CHECK(papacc_server_config_source_resolve(&fake_owner,&fake_pin,1U,&doc,&out,&d)!=PC_OK&&!out.resolved);
 fake_owner.configuration_commit.unresolved=1;CHECK(papacc_server_config_source_resolve(&fake_owner,&fake_pin,1U,&doc,&out,&d)!=PC_OK&&!out.resolved);
 CHECK(papacc_server_config_source_resolve(&fake_owner,&fake_pin,2U,&doc,&out,&d)!=PC_OK&&!out.resolved);
 papacc_server_console_win32_request_stop();CHECK(papacc_server_console_win32_stop_requested()==PAPACC_TRUE);
 CHECK(papacc_server_config_source_resolve(&fake_owner,&fake_pin,1U,&doc,&out,&d)!=PC_OK&&!out.resolved);
 CHECK(!fake_owner.configuration_commit.current&&!fake_owner.configuration_commit.published);
 printf("NATIVE_GUARD_CHECKS=%u NO_CURRENT_NO_RESOLUTION_NO_NETWORK RESOLVED_STORAGE=%lu\n",checks,(unsigned long)sizeof(out));return 0;
}
