# Inventário focal e matriz de referências

Status: evidência factual; não é decisão accepted. HEAD main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a.

## Interface

| Conceito | Fonte canônica | Limite |
|---|---|---|
| Identidade persistente | ADR-0007; src/network/network_interface.h | token opaco machine-local, is_valid separado; zero pode ser válido |
| Representação decimal existente | apps/server/server_cli.c, papacc_server_cli_parse_u64 e --interface-id; tests/server_cli_test.c | uint64 decimal, overflow rejeitado; não define ainda interface_ref no arquivo |
| Produtor Windows | src/platform/win32/network_interfaces_win32.c, papacc_network_adapter_persistent_id | IP_ADAPTER_ADDRESSES.Luid.Value apenas quando Length inclui Luid; não hash de nome |
| Snapshot local | papacc_network_discover_local_snapshot | armazenamento caller-owned; uma descoberta coerente por resolução |
| Resolução persistente | src/network/persistent_bind_selection.c | identidade -> instância atual; rejeita inexistência, ambiguidade e colisão; sem fallback |
| Endereço e endpoint | src/network/bind_target.c | instância -> endereços bindáveis; IP não substitui PersistentID |
| Modelo administrativo | PAPACC_SERVER_CONFIG.bind_selection | reutilizável, não criar registro de aliases |
| Bridge textual atual | src/config/config_binding.c | SELECTED negado antes de lookup; resolução textual não implementada |

Nome administrativo do bloco identifica a declaração, não uma interface física. PresentationName, índice volátil, endereço IP e interface_instance_id não são PersistentID. O token não determina sozinho interface física/lógica nem política de entrada/saída. Listener ingress e Network Egress são contextos distintos; identidade comum não transfere autorização. Não foi executada enumeração nativa de interfaces nesta tarefa.

Correção focal do handoff anterior: existe representação decimal no CLI. A lacuna não é ausência de identidade ou resolver: é a adoção normativa exata dessa representação em interface_ref e seu binding ao snapshot na bridge.

## Autenticação

| Fonte | Fato | Não autoriza |
|---|---|---|
| ADR-0010; server_security_configuration.h/.c | security_configuration_ref resolve agregado de identidade/trust/callbacks com lifetime | usar security_ref automaticamente como provider_ref |
| authentication_authorization.h/.c | PeerEvidence -> PrincipalResolver -> Authorization; negação por ausência/falha | callback inventado, grants implícitos ou provider nomeado |
| server_security_source_win32.c | source explicitamente laboratorial, allowlist fingerprint | catálogo produtivo ou importação de fixture |
| composition_inputs.provider_id=openssl | backend criptográfico PST | provider de autenticação de cliente |
| backend issuer authorize / SID administrativo | autorização administrativa separada | autenticação de usuário/cliente |
| ADR-0028 | transporte, admissão, AuthN, Principal e capability distintos | OPEN=mTLS dispensado; CLOSED=mTLS automaticamente suficiente |

Não localizado catálogo administrativo versionado de providers nomeados com identidade/revisão/pin. Não localizado enforcement governado de usuário/senha/grupo/enrollment reutilizável como provider_ref. Principal autenticado não é usuário nem grant. Backend de certificado existente é componente reutilizável, não registro nominal aprovado.

## Bridge preservada

VALIDATED -> MODEL_BOUND -> RESOLVED. PC_OK estrutural não basta. A entrada nativa exige pin CURRENT factual, owner/thread/estado/revisão/source e revalidação; a saída é valor, não ticket. SELECTED/provider nomeado permanecem explicitamente rejeitados. Nenhuma configuração, policy, grant ou CURRENT publicado.
