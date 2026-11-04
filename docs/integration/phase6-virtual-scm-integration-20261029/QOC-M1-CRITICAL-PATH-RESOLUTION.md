# M1 e caminho crítico da primeira instalação

Data: 2026-10-30. TASK: PACC-PHASE6-QOC-M1-CRITICAL-PATH-RESOLUTION.
Ref auditada: main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469, WIP preservado.
Decisão de aplicabilidade documental; não é nova decisão normativa ou autorização.

## Resultado

**M1 não é obrigatória antes da instalação. Para medir runtime de um serviço
ausente, é inaplicável.** Uma consulta de disponibilidade seria opcional e não
resolveria a ausência do processo. Não foi repetida: o último resultado conhecido
1060 continua histórico, sem afirmar estado SCM atual.

O coletor exige serviço own-process com PID vivo em RUNNING ou START_PENDING.
Registro SCM sem Start não cria esse processo. Portanto, a coleta de recursos
tem posição posterior ao registro e ao início legitimamente autorizados do
runtime diagnóstico. Não autoriza esse início nem substitui autenticação.
A cláusula M1 do MVP (segurança da CA) não é a observação QOC denominada M1.

## Fundamento accepted

- DECISION-phase6-personal-mvp-20261022: M5 prevê instalar/iniciar separadamente
  o mesmo runtime; M1 mantém CA protegida e CA_INITIALIZE distinta do startup.
- DECISION-personal-windows-optional-scm-20261022: SCM é opção de hospedagem,
  sem instalação/início implícitos ou dispensa de autoridade.
- DECISION-personal-first-install-authorization-f0-f9-20261022: F0/F1 autorizam
  primeira instalação por ato original atribuído; F3 vincula identidade do filho
  depois da criação exclusiva; F5 consome antes do efeito; F6 preserva UNKNOWN;
  F7 não confunde instalação com CA_INITIALIZE. Não exige CA/CURRENT anteriores.
- DECISION-personal-scq-closed-startup-s0-s7-20261030: permite implementação de
  preparação/readiness diagnóstica fechada, distinta da produtiva; mantém fonte
  original e autorizações operacionais separadas de endpoint/lifecycle/parent.
- DECISION-personal-scm-virtual-account-20261029 e
  DECISION-personal-binary-destination-new-install-20261029: identidade virtual e
  layout pessoal, sem prova de token real ou autorização de instalação.
- TO0–TO9 limita TRANSPORT_ONLY: ausência de KSP não dispensa originais, recursos,
  consumo, fechamento ou autorização operacional.

Nenhum desses contratos torna a execução do coletor QOC M1 pré-condição da
criação do serviço. A necessidade de fundamentar recursos continua vigente;
ela não equivale à obrigação de medir um processo inexistente.

## Código factual e lacuna funcional

`apps/server/server_service_data.c` aceita `service install`; não foi encontrado
um argumento literal `--install`. PS_INSTALL admite serviço ausente, não exige
serviço RUNNING. O nome usado no roadmap não deve produzir uma CLI inventada.

`server_service_win32.c`, papacc_server_service_cli, bloqueia toda mutação por
ps_binding_ready antes de OpenSCManager/CreateService. Em
`server_service_startup_win32.inc`, a fonte original ainda retorna
ERROR_ACCESS_DENIED: faltam original/instalação/escopo/lifetime/readiness. Esse
guard comum de startup não prova obrigação normativa de readiness pré-instalação.
Não pode ser convertido em constante positiva nem satisfeito por medição M1.

`server_service_virtual_win32.inc` já contém CreateService, consumo local antes
do efeito, imagem retida/revalidada, configuração e readback do SID type. O perfil
produto é SERVICE_DEMAND_START (`server_service_identity_win32.h`), sem Start
automático. Entretanto, ps_image_open compõe `service host`, não
`service host scq-closed`. Logo, a primitiva existente não demonstra uma sessão
diagnóstica fechada instalável: faltam composição action-specific da autoridade
original de instalação e seleção governada/verificável da entrada fechada.
Não basta remover o guard ou executar a primitiva diretamente.

O presenter pessoal é DATA/handoff; LAB prepare é somente leitura/LAB e não
concede autoridade PERSONAL_LOCAL. Getters operacionais ausentes permanecem DENY.
Há ainda entrega original e continuidade da sessão a comprovar no processo real.

## Sequência concreta sem dependência circular

1. Concluir offline o binding original da ação de registro no owner existente:
   operador, decisão específica, alvo/imagem originais, tentativa, lifetime,
   consumo antes do efeito e fechamento. Separar registro de readiness futura.
2. Integrar e testar seleção fechada da mesma imagem/runtime, com readback exato
   do comando e conta; não assumir que o comando atual seja diagnóstico.
3. Congelar artefato final e preparar pedido byte-exato de registro SCM parado.
   Se staging protegido de binários/parent for necessário, delimitar esse efeito
   antes, sob autorização própria; não presumir ausência nem adotar diretório.
4. Após aprovação operacional específica, registro exclusivo do mesmo serviço,
   conta virtual e demanda manual, configuração/readback/fechamento. Sem Start,
   CA, CURRENT ou rede. Nenhum comando executável está pronto nesta auditoria.
5. Preparar autorização separada de Start/Stop diagnóstico e, se necessário,
   endpoint privado; obter PID/token/imagem/continuidade frescos durante a sessão.
   START_PENDING não é readiness; RUNNING apenas após cadeia fechada comprovada.
6. Posicionar a observação M1 somente quando houver processo elegível, sob sua
   aprovação própria. Se o envelope da sessão bilateral ainda não estiver
   fundamentado, a mera observação não libera a sessão. Não usar picos como tetos.
7. Completar prova bilateral/close e isolamento CNG em escopos próprios quando
   necessários. NEW_INSTALL operacional, CA_INITIALIZE e publicação A/P continuam
   cerimônias distintas com autoridade própria, sem exigir CA prévia no primeiro uso.

Primeiro efeito SCM necessário: **registro governado, sem início**, depois das
correções offline e do alvo/imagem legitimamente preparados. Primeiro efeito
filesystem pode precedê-lo se staging/parent forem necessários; não foi observado
nem autorizado aqui. O próximo pacote autorizável é esse registro delimitado,
mas seu pedido ainda não está completo: não há comando com placeholders.

## Recursos e evidência

Disponíveis: layouts/quotas, limites 124/128/1024, modelos de falha e 32 amostras
Windows do próprio coletor. Não demonstram custo do runtime. Exigem processo real:
baseline runtime/token, transitivos SCM, retenção/Stop e custos ativos da troca.
Validade/freshness/duração/headroom material continuam UNKNOWN onde não fundamentados.
O primeiro ensaio real precisa de envelope justificado independentemente; não se
autoriza operação ilimitada para descobrir limites. A lacuna deve ser delimitada
no pedido correspondente, sem fazer do M1 pré-instalação uma obrigação fictícia.

Nesta tarefa: somente leituras locais e correção documental. Nenhuma consulta
SCM/M1 ou operação protegida; nenhum código alterado, build ou campanha repetida.
Proposta M1 preservada CANDIDATE/NOT_APPROVED/NOT_EXECUTED, hash
edfbe8428089699dffbaafb4e0292a20d8eb46732bf1fe1d82d1e113f72647a3.
E21 PENDING; ED11/Phase 6 PARTIAL; autoridade DENY. A prioridade histórica de M1
no handoff QOC é substituída por esta sequência, sem alterar contratos accepted.
