# Achados, divergencias e escopo Phase6

Auditoria estatica: nao executou builds/testes/rede nem alterou fontes. Status de codigo nao e qualificacao operacional. Axx/PROV/IR/SITE sao aliases em authority-inventory.md; caminhos/linhas/funcoes em source-locations.json e requirements-matrix.json.

## GOVERNANCE CONFLICT — F01: listener implicito e cardinalidade

A09 Decisao exige: "O Shared Configuration Model representa listeners como uma colecao"; perfil explicito por listener; Regras8: "Configuracao antiga sem profile nao vira Legacy implicitamente". Fonte apps/server/server_config.h.api3.inc: PAPACC_SERVER_CONFIG contem apenas control_port/bind_selection/listener_profile; INITIALIZER usa LEGACY_ENDPOINT. apps/server/server_cli.c: papacc_server_cli_scan inicializa has_listener_profile=FALSE, mas nao exige que fique TRUE antes de retornar OK. Sem --listener-profile, port/bind validos podem resultar no perfil inicial Legacy. main.c.api3.inc help tambem exibe opcao Legacy como opcional.

Nao executar o parser para provar esse fato: e uma leitura de fluxo. Nao afirmar listener exposto: main.c.api3.inc papacc_server_run chama papacc_application_require_operational_admission antes de network_start, e a admissao do candidato permanece negada. Nao corrigir por trocar silenciosamente producao para Secure. A minima futura reconciliacao deve separar initializer sem autoridade, source com default explicitamente resolvido e requisito de opt-in Legacy. Colecao de sockets/bind targets para um control_port nao e colecao de listeners logicos com policies/perfis independentes. Implementacao atual e subset; formato completo nao pode congelar esse subset como todo o produto. A obrigacao de modelo accepted permanece; habilitacao/coexistencia operacional de perfis e gate separado, sem autorizar Legacy por esta auditoria.

## F02: corpo administrativo nao e engine completa

A43/A48 H: FIRST_TERMINAL_MATCH_WINS, todas as condicoes, DEFAULT_DENY. server_policy_model.h limita3keys/4rules, contexto profile/port/admission/Principal; unica condition bit Principal. server_config.c.api3.inc papacc_server_policy_validate rejeita bits nao suportados e papacc_server_policy_evaluate retorna a primeira terminal correspondente. Nao existe linguagem geral AND/OR/NOT, user/group/time/IP nem politica de especificidade/heranca. O tamanho de implementacao nao e novo limite publico do produto.

## F03: cobertura por caminho precisa ser demonstrada, nao generalizada

server_c2_root_binding_win32.inc usa Principal16 factual, CURRENT/P e avaliador administrativo; C2 owner liga o callback quando presente. server_io_loop_win32.c.api3.inc transition_post/establishment ainda possui allow_enabled_capability como callback basico, inclusive no job de egress. A presenca de papacc_server_policy_evaluate nao prova enforcement de todas as condicoes/per-Principal em todos os caminhos ModeB e legacy. Registrar COVERAGE_NOT_PROVEN onde nao foi demonstrada composicao completa, sem alegar exploit em admissao negada. Futuro arquivo nao pode anunciar regras per-user/per-source operacionalmente aceitas por esse callback basico.

## F04: intencao de profile e source real sao diferentes

PAPACC_EGRESS_PROFILE representa interface/source/default-route; source canonica server_egress_source_win32.c aceita apenas profile default-route e endpoints IPv4 exatos do environment. Seu resolve_dns enumera endpoints configurados por porta, nao consulta DNS do OS. Core destination.c normaliza ASCII/IP, chama resolver uma vez, canonicaliza candidatos e exige endpoint permitido; isso nao qualifica um adaptador DNS produtivo nem rota por interface. MANAGEMENT/LOCAL_SERVICE sao constants no header mas nao ha classificacao administrativa completa desses conjuntos localizada no classificador. Exact default-deny permanece; nao inventar PUBLIC_INTERNET usando apenas classificacao parcial.

## F05: seguranca de laboratorio nao e cadastro de producao

server_security_source_win32.c papacc_server_lab_principal_resolve aceita um fingerprint configurado e copia16bytes para Principal lab; papacc_server_lab_authorize permite as tres acoes de acesso para esse Principal. Nao equivale a cadastro multi-clientes, usuario/grupo, associacao estavel apos renovacao ou perfil de autorizacao administrativa. Caller canonico preserva Principal16, sem conversao da identidade historica32. Core issuer/persistencia/live/checkpoint hoje existem: relatos antigos de issuer inteiramente inexistente sao HISTORICAL. Ainda faltam CNG qualificado, cadastro operativo/CLI/autorizacao real, emissao/publicacao e recuperacao com fencing para produto.

## F06: persistencia nao e arquivo administrativo editavel

BODY0.5 guarda configuracao normalizada bounded, policy e um perfil Site TLS; ROOT06 guarda issuer e o trailer512 guarda R/A/P e correlacao. Nao ha parser completo INI/JSON/YAML nem arquivo de administracao universal definido. Refs de policy/egress/security sao identidades opacas; nao incluem automaticamente todos os snapshots resolvidos. Um futuro source deve vincular conteudo/resolver/versao/leases duravelmente, nao reler environment mutavel sob a mesma ref e afirmar configuracao identica. Nao editar BODY/ROOT/serial diretamente.

## F07: prioridades normativas nao podem ser mescladas

DENY de outra dimensao bloqueia o efeito, mas na MESMA tabela um ACCEPT terminal anterior nao e revertido por DENY posterior. Exact allowlist v1 nao possui regras DENY nem CIDR. Precedencia de Managed Policy Browser pertence ao consumidor, nao a tabela do servidor. Grupos nao possuem heranca/nesting aprovados. Fonte administrativa nao cria segunda autoridade.

## F08: tempo e identidade requerem dominios separados

Relogio UTC de validade de certificado/catalogo, monotonic deadlines e validade de observacao ModeC nao sao calendario/source-context. A28/FUT preservam semana/janela como futuro; nao definem calendarios, feriados, DST, timezone nem intervalo cruzando meia-noite. VALID mantem freshness/headroom quantitativos pendentes e exclui excecao ADR0052 CONTROL. Ausencia/tempo incoerente nega efeito dependente, nao vira TTL infinito ou threshold1000ms.

## F09: documentacao viva tem camadas historicas

architecture.md ainda cita PST0.6.1 e fases anteriores; dependency pin factual0.6.4, candidato normal0.6.5. READY de ModeB antigo nao fecha obrigações novas PROV, ModeC, C8/ED11 ou recuperacao. FUT explicita partially realized, mas preserva trechos antigos de Phase4: usar como historico/contexto, nao substituir ADRs atuais. ADRs43/44/48/51 estao na arvore historica com escopos originais: manter referencias/proveniencia e consolidacao seletiva, sem promover todos os harnesses/exception gates ao produto.

## Gaps accepted antes do fechamento integral / conformidade do modelo

- P6-G1: reconciliar modelo de listeners/perfil explicito com A09 antes de congelar source de configuracao completo; nao habilitar novos modos por esse ajuste.
- P6-G2: source/admin CLI autorizado com validar/aplicar/listar/consultar/diagnosticar/auditar e transacao mesma root; APIs privadas nao bastam para provisionamento de produto.
- P6-G3: MANUAL e AUTOMATIC, bootstrap verificavel/codigo/anti-replay, CLOSED enrollment password, AUTO/MANUAL_APPROVAL e hot cadastro obrigatorios PROV1-8; container/cerimonia/limites concretos ainda PROPOSED ou pendentes onde nao accepted.
- P6-G4: CA backend, protecao administrativa, backup/restore/checkpoint recente/fencing e emissao real sujeitos CAT; sessao ativa apos revogacao de certificado POLICY_PENDING. Persistir REVOKED nao fecha a politica.
- P6-G5: demonstrar enforcement operacional/currency por todos os consumidores e perfis suportados, sem ampliar snapshot. ModeC pair/native/ED08/ED11 ainda gates independentes. Nao fazer C8 dependencia artificial de E10 nem importar excecao CONTROL.
- P6-G6: evidencias totais ED11/freshness/headroom/provider/kernel e inputs administrativos/TLS reais, mantendo DENY. Ultimo censo disponivel known1801168, combinado1972291 incluindo attachment, e evidencia anterior de um perfil, nao PASS desta auditoria.

## Nao transformar em novas obrigacoes Phase6

RBAC/cadastro users/groups generico, grupos aninhados, calendarios/feriados/DST/horarios complexos, OR/NOT/chains/heranca, PUBLIC_INTERNET/CIDR/ranges/suffix/regex, failover, migracao de conexoes, TLS interception, proxy/conteudo, firewall/NAT, firmware box/ISA, backup.papacc.bkp completo e migracao universal NAO sao implementados por esta tarefa. Algumas direcoes sao accepted como representabilidade, outras apenas FUTURE/PROPOSED; requerem contrato proprio. Nao adiar silenciosamente as obrigacoes ja accepted acima.
