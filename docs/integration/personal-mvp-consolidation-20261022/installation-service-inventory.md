# Inventário factual — instalação/serviço

main HEAD913c90cf2328f7939d8f5b927640601903ac6e91 com WIP; hashes no manifesto. Sem comandos SCM/conta/chave. Ausência limitada às árvores/símbolos pesquisados, não fontes externas.

| Caminho/função | Reutilização factual | Lacuna |
| --- | --- | --- |
| apps/server/main.c, main.c.api3.inc:main/papacc_server_run | API2/candidateAPI3, configCLI antes da rede, logger/console/runtime e lifetime | Sem SCM dispatcher/ServiceMain; papacc_application_require_operational_admission nega antes do listener |
| server_config_cli.c:papacc_server_config_cli/first_install_entry | validate/diff/show-effective/apply/first-install | apply DENY, diff/effective semCURRENT; first-install read-only semCA_INITIALIZE |
| server_launcher_personal_win32.inc | Parent/token/plano/console/confirm visível/mesma tentativa; F10 histórico positivo | Falta produtor same-invocation factual de CA; read-only não autoriza efeitos |
| personal_native/pending_win32.inc:personal_target_worker, packet12/13 | Parent protegido/workerúnico/child exclusivo mecanismoLAB, Stop/UNKNOWN | personal_native_ceremony_ready0; integração factual pendente |
| server_ca_key_win32.c:create/validate/borrow/return/close | KSP MACHINE/propriedades/diagFinalize/lifetime, LABpositivo | Não é fonte/isolamento produtivos; manter mesmo owner |
| server_configuration_personal_ca_win32.inc | match/bind/current/release e exclusão das duas identidadesLAB | Precisa produtor/owner factual durável, nenhum keyname arbitrário |
| config_source.c/.h; server_config_source_binding_win32.c | Parserbounded e binder à root/pin | Sem retention key, settings-only export/restore, apply operacional completo |
| pending/root_transaction/prepared_storage | Worker/CAS/backings/pins, packet11v4LAB e hashesDATA determinísticos | CURRENT/autoridade e ED11 operacional não qualificados |
| launcher_owner_win32.h/.c, server_console_win32 | Root/owners/Stop/lifetime | EntradaSCM deve reutilizar, não duplicar runtime ou exigir console no serviço |
| tools/qualification/*uac.ps1 | Launchers manuais pinados/receipt/UAC | Qualificação, não instalador/registrador de serviço ou conta produtiva |

Busca apps/src/tools por CreateService/OpenSCManager/StartServiceCtrlDispatcher/ServiceMain/ChangeServiceConfig não encontrou host/registrador SCM. LAB ServiceSid exclui LocalService e não seleciona conta operacional. Não localizado installer/uninstaller completo no corpus examinado.

Evidências existentes preservadas: F10/manualUAC; personal-first-install-target-ca-custody2; ca-lab-finalization-launcher-completion/manual-elevated-result; personal-first-install-ca-custody-binding; v4-canonical-packet11-worker; v3-v4-canonical-prepared-storage. Não repetir geração/fixture nem tomar handoff antigo por estado factual mais novo. Serviço, conta, binários/instalação e CA exigem operações próprias autorizadas.

