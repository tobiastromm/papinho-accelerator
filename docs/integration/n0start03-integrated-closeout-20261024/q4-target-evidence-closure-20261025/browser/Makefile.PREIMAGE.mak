# Microsoft NMAKE build script for Visual C++ 6.0 (or newer cl.exe in VC6 compatibility mode)
# Produces Win32 console executables compatible with Windows NT 4.0 (x86).

CC=cl
LD=link

!include build-config\pst-targets.mak
OBJ_DIR=$(OUT_DIR)\obj
BIN_DIR=$(OUT_DIR)\bin
TCP_TARGET=$(BIN_DIR)\papinho_tcp_probe.exe
HTTP_TARGET=$(BIN_DIR)\papinho_http_probe.exe
TLS_TARGET=$(BIN_DIR)\papinho_tls_probe.exe
HTTPS_TARGET=$(BIN_DIR)\papinho_https_probe.exe
BROWSER_TARGET=$(BIN_DIR)\papinho_browser.exe
PAPINHOPACC_SMOKE_TARGET=$(BIN_DIR)\papinhopacc_smoke.exe
PST_SMOKE_TARGET=$(BIN_DIR)\papinho_pst_smoke.exe
ACCELERATOR_CLIENT_SMOKE_TARGET=$(BIN_DIR)\papinho_accelerator_client_smoke.exe
ACCELERATOR_FIXTURE_RUNNER_TARGET=$(BIN_DIR)\papinho_accelerator_fixture_runner.exe
ENDPOINT_TEST_TARGET=$(BIN_DIR)\papinho_endpoint_snapshot_test.exe

PAPINHO_COMPAT_INC=third_party\compat\vc6
PAPINHOPACC_INC=third_party\papinhopacc\include
PAPINHOPACC_PROTOCOL_INC=$(PAPINHOPACC_INC)\papacc\pacc

CANDIDATE_CLIENT_DEPS=
CANDIDATE_CLIENT_LIBS=
CANDIDATE_OWNER_OBJ=
!IF "$(PST_PROFILE)" == "CANDIDATE_065"
# Probe submission now owns a reference through the canonical credential store.
CANDIDATE_CLIENT_DEPS=$(OBJ_DIR)\papinho_credential_store.obj $(OBJ_DIR)\papinho_pst_owner.obj $(BEARSSL_LIB) $(OBJ_DIR)\c2_codec.obj $(OBJ_DIR)\papinho_c2_secret.obj $(OBJ_DIR)\papinho_site_pump_win32.obj
CANDIDATE_CLIENT_LIBS=ole32.lib user32.lib
CANDIDATE_C2_OBJS=$(OBJ_DIR)\c2_codec.obj $(OBJ_DIR)\papinho_c2_secret.obj $(OBJ_DIR)\papinho_site_pump_win32.obj
CANDIDATE_OWNER_OBJ=$(OBJ_DIR)\papinho_pst_owner.obj $(CANDIDATE_C2_OBJS) $(OBJ_DIR)\papinho_papacc_public.obj $(OBJ_DIR)\papinho_papacc_frame.obj $(OBJ_DIR)\papinho_papacc_crypto.obj $(OBJ_DIR)\papinho_papacc_content.obj $(OBJ_DIR)\papinho_papacc_material.obj $(OBJ_DIR)\provisioning_structure.obj $(OBJ_DIR)\provisioning_m1.obj $(OBJ_DIR)\provisioning_trust.obj
!ENDIF





BEARSSL_COMPAT_INC=..\compat\vc6
BEARSSL_COMPAT_FORCE_INC=..\..\compat\vc6
PAPINHO_VC6_FORCE_INCLUDE=/FI$(PAPINHO_COMPAT_INC)\vc6_compat.h
BEARSSL_VC6_FORCE_INCLUDE=/FI$(BEARSSL_COMPAT_FORCE_INC)\vc6_compat.h
BEARSSL_CFLAGS=-nologo -W2 -O2 /MD /D_WIN32_WINNT=0x0400 -I$(BEARSSL_COMPAT_INC) $(BEARSSL_VC6_FORCE_INCLUDE)

CFLAGS=/nologo /TC /W3 /MD $(PST_PROFILE_CFLAGS) /DWIN32 /D_WINDOWS /D_CONSOLE /D_CRT_SECURE_NO_WARNINGS /I. /Icore /Iplatform /I$(PAPINHO_COMPAT_INC) /Ithird_party\bearssl\inc /I$(PAPINHOPACC_INC) /I$(PAPINHOPACC_PROTOCOL_INC) /I$(PST_INC) $(PAPINHO_VC6_FORCE_INCLUDE) $(CANDIDATE_NATIVE_CFLAGS)
CANDIDATE_NATIVE_CFLAGS=
!IF "$(PST_PROFILE)" == "CANDIDATE_065"
!IF "$(CONFIG)" == "Debug"
CANDIDATE_NATIVE_CFLAGS=/DPAPINHO_C2_NATIVE_BUILD_DEBUG=1
!ELSE
CANDIDATE_NATIVE_CFLAGS=/DPAPINHO_C2_NATIVE_BUILD_DEBUG=0
!ENDIF
!ENDIF

LIBS=wsock32.lib kernel32.lib user32.lib

CORE_OBJS= \
    $(OBJ_DIR)\papinho_socket.obj \
    $(OBJ_DIR)\papinho_socket_win32.obj

TCP_OBJS= \
    $(OBJ_DIR)\papinho_tcp_probe.obj \
    $(CORE_OBJS)

HTTP_OBJS= \
    $(OBJ_DIR)\papinho_http_probe.obj \
    $(CORE_OBJS)

TLS_OBJS= \
    $(OBJ_DIR)\papinho_tls_probe.obj \
    $(OBJ_DIR)\papinho_tls.obj \
    $(CORE_OBJS)

HTTPS_OBJS= \
    $(OBJ_DIR)\papinho_https_probe.obj \
    $(OBJ_DIR)\papinho_tls.obj \
    $(CORE_OBJS)

BROWSER_OBJS= \
    $(OBJ_DIR)\papinho_browser.obj \
    $(OBJ_DIR)\papinho_credential_store.obj \
    $(OBJ_DIR)\papinho_accelerator_client.obj \
    $(OBJ_DIR)\papinho_routing_policy.obj \
    $(OBJ_DIR)\papinho_html_view.obj \
    $(OBJ_DIR)\papinho_dom.obj \
    $(OBJ_DIR)\papinho_css.obj $(CANDIDATE_OWNER_OBJ)

PAPINHOPACC_OBJS= \
    $(OBJ_DIR)\papacc_frame.obj \
    $(OBJ_DIR)\papacc_frame_parser.obj \
    $(OBJ_DIR)\papacc_control_protocol.obj \
    $(OBJ_DIR)\papacc_data_association_protocol.obj \
    $(OBJ_DIR)\papacc_capability_wire.obj \
    $(OBJ_DIR)\papacc_capability_negotiation_protocol.obj \
    $(OBJ_DIR)\papacc_network_offload_protocol.obj

all: dirs $(TCP_TARGET) $(HTTP_TARGET) $(TLS_TARGET) $(HTTPS_TARGET) $(BROWSER_TARGET) $(PAPINHOPACC_SMOKE_TARGET) $(PST_SMOKE_TARGET) $(ACCELERATOR_CLIENT_SMOKE_TARGET) $(ACCELERATOR_FIXTURE_RUNNER_TARGET) resources

dirs:
    if not exist build mkdir build
    if not exist $(OUT_DIR) mkdir $(OUT_DIR)
    if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
    if not exist $(BIN_DIR) mkdir $(BIN_DIR)

resources: dirs
    if not exist $(BIN_DIR)\resources mkdir $(BIN_DIR)\resources
    if not exist $(BIN_DIR)\resources\errors mkdir $(BIN_DIR)\resources\errors
    if not exist $(BIN_DIR)\resources\lang mkdir $(BIN_DIR)\resources\lang
    if not exist $(BIN_DIR)\resources\selftests mkdir $(BIN_DIR)\resources\selftests
    copy /Y resources\errors\*.html $(BIN_DIR)\resources\errors\ >nul
    copy /Y resources\lang\*.lang $(BIN_DIR)\resources\lang\ >nul
    copy /Y resources\selftests\*.html $(BIN_DIR)\resources\selftests\ >nul
    copy /Y $(PST_RUNTIME)\*.dll $(BIN_DIR)\ >nul
    copy /Y $(PST_RUNTIME)\*.chk $(BIN_DIR)\ >nul

$(OBJ_DIR)\papinho_tcp_probe.obj: src\papinho_tcp_probe.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_tcp_probe.c

$(OBJ_DIR)\papinho_http_probe.obj: src\papinho_http_probe.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_http_probe.c

$(OBJ_DIR)\papinho_socket.obj: core\networking\papinho_socket.c core\networking\papinho_socket.h platform\win32\papinho_socket_win32.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c core\networking\papinho_socket.c

$(OBJ_DIR)\papinho_socket_win32.obj: platform\win32\papinho_socket_win32.c platform\win32\papinho_socket_win32.h core\networking\papinho_socket.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c platform\win32\papinho_socket_win32.c

$(OBJ_DIR)\papinho_tls.obj: core\tls\papinho_tls.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c core\tls\papinho_tls.c

$(OBJ_DIR)\papinho_tls_probe.obj: src\papinho_tls_probe.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_tls_probe.c

$(OBJ_DIR)\papinho_https_probe.obj: src\papinho_https_probe.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_https_probe.c

$(OBJ_DIR)\papinho_credential_store.obj: src\papinho_credential_store.c src\papinho_credential_store.h src\papinho_credential_import.inc src\papinho_credential_lifetime.inc src\papinho_credential_finalization.inc src\papinho_papacc_provenance.h src\papinho_papacc_provenance.inc src\papinho_credential_provenance.inc
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_credential_store.c

$(OBJ_DIR)\papinho_browser.obj: src\papinho_browser.c src\papinho_routing_policy.h src\papinho_accelerator_config_generation.inc src\papinho_config_defaults_selftest.inc src\papinho_credential_store.h src\papinho_credential_options.inc src\papinho_navigation_context.h src\papinho_retry_action.inc src\papinho_navigation_security.h src\papinho_navigation_security.inc src\papinho_page_security_dialog.inc src\papinho_provenance_selftest.inc src\papinho_papacc_provenance.h src\papinho_papacc_options.inc src\papinho_papacc_public.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_browser.c

$(OBJ_DIR)\papinho_papacc_content.obj: src\papinho_papacc_content.c src\papinho_papacc_content.h src\papinho_papacc_public.h third_party\shared-provisioning-structure-snapshot1-a64a141ed2d2\structure.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_papacc_content.c

$(OBJ_DIR)\papinho_papacc_material.obj: src\papinho_papacc_material.c src\papinho_papacc_material.h src\papinho_papacc_content.h src\papinho_credential_store.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_papacc_material.c

$(OBJ_DIR)\papinho_papacc_crypto.obj: src\papinho_papacc_crypto.c src\papinho_papacc_crypto.h src\papinho_papacc_pbkdf2.inc src\papinho_papacc_public.h src\papinho_papacc_frame.h third_party\shared-provisioning-structure-snapshot1-a64a141ed2d2\structure.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_papacc_crypto.c

$(OBJ_DIR)\papinho_papacc_public.obj: src\papinho_papacc_public.c src\papinho_papacc_public.h src\papinho_papacc_frame.h src\papinho_papacc_m1.inc third_party\shared-provisioning-m1-evidence-snapshot2-4227052d83f5\m1.h third_party\shared-provisioning-m1-evidence-snapshot2-4227052d83f5\m1_internal.h third_party\shared-provisioning-m1-evidence-snapshot2-4227052d83f5\trust.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_papacc_public.c

$(OBJ_DIR)\papinho_papacc_frame.obj: src\papinho_papacc_frame.c src\papinho_papacc_frame.h third_party\shared-provisioning-structure-snapshot1-a64a141ed2d2\structure.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_papacc_frame.c

$(OBJ_DIR)\papinho_accelerator_client.obj: src\papinho_accelerator_client.c src\papinho_control_probe_submit.inc
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_accelerator_client.c
$(OBJ_DIR)\papinho_routing_policy.obj: src\papinho_routing_policy.c src\papinho_routing_policy.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_routing_policy.c

$(OBJ_DIR)\papinho_html_view.obj: src\papinho_html_view.c src\papinho_html_view.h src\papinho_navigation_context.h src\papinho_navigation_security.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_html_view.c

$(OBJ_DIR)\papinho_dom.obj: src\papinho_dom.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_dom.c

$(OBJ_DIR)\papinho_css.obj: src\papinho_css.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c src\papinho_css.c

$(OBJ_DIR)\papacc_frame.obj: third_party\papinhopacc\src\frame.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\papacc_frame.obj /c third_party\papinhopacc\src\frame.c

$(OBJ_DIR)\papacc_frame_parser.obj: third_party\papinhopacc\src\frame_parser.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\papacc_frame_parser.obj /c third_party\papinhopacc\src\frame_parser.c

$(OBJ_DIR)\papacc_control_protocol.obj: third_party\papinhopacc\src\control_protocol.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\papacc_control_protocol.obj /c third_party\papinhopacc\src\control_protocol.c

$(OBJ_DIR)\papacc_data_association_protocol.obj: third_party\papinhopacc\src\data_association_protocol.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\papacc_data_association_protocol.obj /c third_party\papinhopacc\src\data_association_protocol.c

$(OBJ_DIR)\papacc_capability_wire.obj: third_party\papinhopacc\src\capability_wire.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\papacc_capability_wire.obj /c third_party\papinhopacc\src\capability_wire.c

$(OBJ_DIR)\papacc_capability_negotiation_protocol.obj: third_party\papinhopacc\src\capability_negotiation_protocol.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\papacc_capability_negotiation_protocol.obj /c third_party\papinhopacc\src\capability_negotiation_protocol.c

$(OBJ_DIR)\papacc_network_offload_protocol.obj: third_party\papinhopacc\src\network_offload_protocol.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\papacc_network_offload_protocol.obj /c third_party\papinhopacc\src\network_offload_protocol.c

$(OBJ_DIR)\papinhopacc_consumer_smoke.obj: third_party\papinhopacc\tests\consumer_smoke.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\papinhopacc_consumer_smoke.obj /c third_party\papinhopacc\tests\consumer_smoke.c

$(OBJ_DIR)\papinho_pst_smoke.obj: src\papinho_pst_smoke.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\papinho_pst_smoke.obj /c src\papinho_pst_smoke.c

$(OBJ_DIR)\papinho_accelerator_client_smoke.obj: src\papinho_accelerator_client_smoke.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\papinho_accelerator_client_smoke.obj /c src\papinho_accelerator_client_smoke.c

$(OBJ_DIR)\papinho_accelerator_fixture_runner.obj: src\papinho_accelerator_fixture_runner.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\papinho_accelerator_fixture_runner.obj /c src\papinho_accelerator_fixture_runner.c


$(BEARSSL_LIB): third_party\bearssl\src\ec\ecdsa_i15_bits.c third_party\bearssl\src\hash\multihash.c third_party\bearssl\src\int\i15_decred.c third_party\bearssl\src\int\i31_montmul.c third_party\bearssl\src\int\i32_montmul.c
!IF "$(PST_PROFILE)" == "PRODUCTION_064"
    cd third_party\bearssl && nmake /nologo /f Makefile lib BUILD="$(PST_BEARSSL_BUILD)" CFLAGS="$(BEARSSL_CFLAGS)"
!ELSE
    cd third_party\bearssl && nmake /nologo /f Makefile lib BUILD="$(PST_BEARSSL_BUILD)" CFLAGS="-nologo -W2 /MD $(PST_OPT) /D_WIN32_WINNT=0x0400 -I$(BEARSSL_COMPAT_INC) $(BEARSSL_VC6_FORCE_INCLUDE)"
!ENDIF

$(TCP_TARGET): $(TCP_OBJS)
    $(LD) /nologo /subsystem:console /machine:I386 /incremental:no /out:$(TCP_TARGET) $(TCP_OBJS) $(LIBS)

$(HTTP_TARGET): $(HTTP_OBJS)
    $(LD) /nologo /subsystem:console /machine:I386 /incremental:no /out:$(HTTP_TARGET) $(HTTP_OBJS) $(LIBS)

$(TLS_TARGET): $(TLS_OBJS) $(BEARSSL_LIB)
    $(LD) /nologo /subsystem:console /machine:I386 /incremental:no /out:$(TLS_TARGET) $(TLS_OBJS) $(BEARSSL_LIB) $(LIBS)

$(HTTPS_TARGET): $(HTTPS_OBJS) $(BEARSSL_LIB)
    $(LD) /nologo /subsystem:console /machine:I386 /incremental:no /out:$(HTTPS_TARGET) $(HTTPS_OBJS) $(BEARSSL_LIB) $(LIBS)

$(BROWSER_TARGET): $(BROWSER_OBJS) $(PAPINHOPACC_OBJS) $(OBJ_DIR)\papinho_tls.obj $(CORE_OBJS) $(BEARSSL_LIB) $(PST_LIB)
    $(LD) /nologo /subsystem:windows /machine:I386 /incremental:no /out:$(BROWSER_TARGET) $(BROWSER_OBJS) $(PAPINHOPACC_OBJS) $(OBJ_DIR)\papinho_tls.obj $(CORE_OBJS) $(BEARSSL_LIB) $(PST_LIB) wsock32.lib kernel32.lib user32.lib gdi32.lib ole32.lib oleaut32.lib

$(PAPINHOPACC_SMOKE_TARGET): $(OBJ_DIR)\papinhopacc_consumer_smoke.obj $(PAPINHOPACC_OBJS)
    $(LD) /nologo /subsystem:console /machine:I386 /incremental:no /out:$(PAPINHOPACC_SMOKE_TARGET) $(OBJ_DIR)\papinhopacc_consumer_smoke.obj $(PAPINHOPACC_OBJS) kernel32.lib

$(PST_SMOKE_TARGET): $(OBJ_DIR)\papinho_pst_smoke.obj $(PST_LIB)
    $(LD) /nologo /subsystem:console /machine:I386 /incremental:no /out:$(PST_SMOKE_TARGET) $(OBJ_DIR)\papinho_pst_smoke.obj $(PST_LIB) wsock32.lib

$(ACCELERATOR_CLIENT_SMOKE_TARGET): $(OBJ_DIR)\papinho_accelerator_client_smoke.obj $(OBJ_DIR)\papinho_accelerator_client.obj $(PAPINHOPACC_OBJS) $(PST_LIB) $(CANDIDATE_CLIENT_DEPS)
    $(LD) /nologo /subsystem:console /machine:I386 /incremental:no /out:$(ACCELERATOR_CLIENT_SMOKE_TARGET) $(OBJ_DIR)\papinho_accelerator_client_smoke.obj $(OBJ_DIR)\papinho_accelerator_client.obj $(PAPINHOPACC_OBJS) $(PST_LIB) $(CANDIDATE_CLIENT_DEPS) wsock32.lib kernel32.lib $(CANDIDATE_CLIENT_LIBS)

$(ACCELERATOR_FIXTURE_RUNNER_TARGET): $(OBJ_DIR)\papinho_accelerator_fixture_runner.obj $(OBJ_DIR)\papinho_accelerator_client.obj $(PAPINHOPACC_OBJS) $(PST_LIB) $(CANDIDATE_CLIENT_DEPS)
    $(LD) /nologo /subsystem:console /machine:I386 /incremental:no /out:$(ACCELERATOR_FIXTURE_RUNNER_TARGET) $(OBJ_DIR)\papinho_accelerator_fixture_runner.obj $(OBJ_DIR)\papinho_accelerator_client.obj $(PAPINHOPACC_OBJS) $(PST_LIB) $(CANDIDATE_CLIENT_DEPS) wsock32.lib kernel32.lib crypt32.lib $(CANDIDATE_CLIENT_LIBS)


endpoint-test: dirs $(ENDPOINT_TEST_TARGET)
    $(ENDPOINT_TEST_TARGET)

$(OBJ_DIR)\endpoint_snapshot_test.obj: tests\endpoint_snapshot_test.c platform\win32\papinho_socket_win32.c platform\win32\papinho_socket_win32.h core\networking\papinho_socket.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\ /c tests\endpoint_snapshot_test.c

$(ENDPOINT_TEST_TARGET): $(OBJ_DIR)\endpoint_snapshot_test.obj $(OBJ_DIR)\papinho_socket.obj
    $(LD) /nologo /subsystem:console /machine:I386 /incremental:no /out:$(ENDPOINT_TEST_TARGET) $(OBJ_DIR)\endpoint_snapshot_test.obj $(OBJ_DIR)\papinho_socket.obj $(LIBS)

clean:
    if exist build rmdir /S /Q build

rebuild: clean all

# Explicit build profiles. No executable is run by these aliases.
PRODUCTION_064:
    $(MAKE) /nologo /f Makefile.nt4.mak PST_PROFILE=PRODUCTION_064 all

CANDIDATE_065:
    $(MAKE) /nologo /f Makefile.nt4.mak PST_PROFILE=CANDIDATE_065 CONFIG=$(CONFIG) all

DIAGNOSTIC_R2:
    $(MAKE) /nologo /f Makefile.nt4.mak PST_PROFILE=DIAGNOSTIC_R2 CONFIG=$(CONFIG) all
$(OBJ_DIR)\endpoint_snapshot_test.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papacc_capability_negotiation_protocol.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papacc_capability_wire.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papacc_control_protocol.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papacc_data_association_protocol.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papacc_frame_parser.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papacc_frame.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papacc_network_offload_protocol.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_accelerator_client_smoke.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_accelerator_client.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_accelerator_fixture_runner.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_browser.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_credential_store.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_css.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_dom.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_html_view.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_http_probe.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_https_probe.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_pst_smoke.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_routing_policy.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_socket_win32.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_socket.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_tcp_probe.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_tls_probe.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinho_tls.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
$(OBJ_DIR)\papinhopacc_consumer_smoke.obj: Makefile.nt4.mak build-config\pst-targets.mak $(PST_INC)\papinho_secure_transport.h
!IF "$(PST_PROFILE)" == "CANDIDATE_065"
# Staged canonical owner build; caller composition is not yet qualified.
candidate-lifecycle-owner: dirs $(OBJ_DIR)\papinho_pst_owner.obj

$(OBJ_DIR)\papinho_pst_owner.obj: src\papinho_pst_owner.c src\papinho_pst_owner.h build-config\pst-target-contract.h $(PST_INC)\papinho_secure_transport.h
    $(CC) $(CFLAGS) /WX /Fo$(OBJ_DIR)\ /c src\papinho_pst_owner.c

$(OBJ_DIR)\papinho_accelerator_client.obj: src\papinho_pst_attach_aliases.h
$(OBJ_DIR)\papinho_accelerator_client.obj: src\papinho_probe_dependencies.inc src\papinho_credential_store.h
$(OBJ_DIR)\papinho_accelerator_client.obj: src\papinho_candidate_lifetime.inc src\papinho_candidate_session_teardown.inc src\papinho_session_storage.inc src\papinho_accelerator_pst_lifetime.inc src\papinho_pst_process_root.inc src\papinho_pst_owner.h
$(OBJ_DIR)\papinho_browser.obj: src\papinho_accelerator_client.h src\papinho_candidate_controller.inc src\papinho_candidate_service_selftest.inc
$(OBJ_DIR)\papinho_accelerator_fixture_runner.obj: src\papinho_accelerator_client.h
!ENDIF

!IF "$(PST_PROFILE)" != "PRODUCTION_064"
$(OBJ_DIR)\endpoint_snapshot_test.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papacc_capability_negotiation_protocol.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papacc_capability_wire.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papacc_control_protocol.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papacc_data_association_protocol.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papacc_frame_parser.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papacc_frame.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papacc_network_offload_protocol.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_accelerator_client_smoke.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_accelerator_client.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_accelerator_fixture_runner.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_browser.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_credential_store.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_css.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_dom.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_html_view.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_http_probe.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_https_probe.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_pst_smoke.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_routing_policy.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_socket_win32.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_socket.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_tcp_probe.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_tls_probe.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinho_tls.obj: build-config\pst-target-contract.h
$(OBJ_DIR)\papinhopacc_consumer_smoke.obj: build-config\pst-target-contract.h
!ENDIF

!IF "$(PST_PROFILE)" == "CANDIDATE_065"
$(OBJ_DIR)\c2_codec.obj: third_party\pacc-c2-private\c2_codec.c third_party\pacc-c2-private\c2_codec.h third_party\pacc-c2-private\c2_token_compare.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\c2_codec.obj /c third_party\pacc-c2-private\c2_codec.c
$(OBJ_DIR)\papinho_c2_secret.obj: src\papinho_c2_secret.c
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\papinho_c2_secret.obj /c src\papinho_c2_secret.c
$(OBJ_DIR)\papinho_site_pump_win32.obj: platform\win32\papinho_site_pump_win32.c core\networking\papinho_site_pump.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\papinho_site_pump_win32.obj /c platform\win32\papinho_site_pump_win32.c
$(OBJ_DIR)\papinho_accelerator_client.obj: src\papinho_c2_s6.inc src\papinho_c2_s6.h src\papinho_c2_budget.inc src\papinho_c2_first_cause.inc third_party\pacc-c2-private\c2_codec.h
$(OBJ_DIR)\papinho_browser.obj: src\papinho_candidate_native_receipt.inc src\papinho_candidate_navigation_registry.inc src\papinho_navigation_worker_registry.h
$(OBJ_DIR)\papinho_browser.obj: src\papinho_candidate_result_publication.inc
!ENDIF

# Browser-private immutable shared structural snapshot. Candidate normal only.
$(OBJ_DIR)\provisioning_structure.obj: third_party\shared-provisioning-structure-snapshot1-a64a141ed2d2\structure.c third_party\shared-provisioning-structure-snapshot1-a64a141ed2d2\structure.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\provisioning_structure.obj /c third_party\shared-provisioning-structure-snapshot1-a64a141ed2d2\structure.c

# Private immutable M1/negative-evidence snapshot; candidate objects only.
$(OBJ_DIR)\provisioning_m1.obj: third_party\shared-provisioning-m1-evidence-snapshot2-4227052d83f5\m1.c third_party\shared-provisioning-m1-evidence-snapshot2-4227052d83f5\m1.h third_party\shared-provisioning-m1-evidence-snapshot2-4227052d83f5\m1_internal.h third_party\shared-provisioning-m1-evidence-snapshot2-4227052d83f5\trust.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\provisioning_m1.obj /c third_party\shared-provisioning-m1-evidence-snapshot2-4227052d83f5\m1.c
$(OBJ_DIR)\provisioning_trust.obj: third_party\shared-provisioning-m1-evidence-snapshot2-4227052d83f5\trust_unavailable.c third_party\shared-provisioning-m1-evidence-snapshot2-4227052d83f5\m1.h third_party\shared-provisioning-m1-evidence-snapshot2-4227052d83f5\m1_internal.h third_party\shared-provisioning-m1-evidence-snapshot2-4227052d83f5\trust.h
    $(CC) $(CFLAGS) /Fo$(OBJ_DIR)\provisioning_trust.obj /c third_party\shared-provisioning-m1-evidence-snapshot2-4227052d83f5\trust_unavailable.c
