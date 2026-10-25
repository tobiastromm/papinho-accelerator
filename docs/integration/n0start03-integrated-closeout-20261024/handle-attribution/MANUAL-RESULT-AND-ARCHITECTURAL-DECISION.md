# N0HT01 — reconciliação factual e conclusão B

## Conclusão

**B. Capacidade positiva ainda não demonstrada.** A observação terminou com exit0/first0/close0, sem Start/Stop/intent/efeitos. Ela localizou as fases de crescimento e identificou os27 slots controlados finais; não identificou a origem de cada entrada não mapeada. Não há base para fechar referências desconhecidas ou garantir uma redução real. Não se pede outra captura equivalente.

Original preservado byte-exato: MANUAL-N0HT01-OUTPUT.txt,47404bytes,SHA256f57d9fbf8b8990714856ee9c26795d19e92d64feb2828c0c23ed39a498bbda7e. Fonte: arquivo de console fornecido pelo owner, não uma captura independente do Codex.302 entradas foram parseadas; contagens, PID24492/criação31286313:1746997232 e finais conferem. Não há marcador de truncamento.

## Atribuição no alcance comprovado

| Checkpoint | Total | Slots controlados | Entradas não mapeadas | CapturaPSS | Após free |
|---|---:|---:|---:|---:|---:|
| Entrada após CRT/console/aprovação |71|1|70|72|71|
| Após arquivos/owner inicial/SCM |105|9|96|106|105|
| Após bindingN0 |126|27|99|127|126|

Primeiro intervalo: +34 = +8 controlados e +26 não mapeados. Os8 são token self, pai/config/imagem original, duas dependências, mutex e imagem fonte. O intervalo inclui operador/boot, arquivos, exclusividade e SCM; não é uma amostra exclusiva de OpenSCManager/QueryServiceConfig. Os26 adicionais incluem8Semaphore,4ALPC Port,2Event,2Key,1Thread,1IoCompletion,1TpWorkerFactory,2IRTimer,2 prefixosWaitCompletionPa e3 entradas sem tipo. O padrão é compatível com inicialização/transporte Windows, mas não prova qual chamada o causou nem dispensabilidade.

Segundo intervalo: +21 = +18 controlados e +3 não mapeados. Os18 são5roots,dossiê/evidência,4políticas e7 referências canônicasBD. Os3 não mapeados são1File e2 sem tipo. O handoff já fecha suas5 referências temporárias originais; não encontrámos cópias esquecidas. Correspondência numérica/metadados entre capturas foi preservada como correlação, não prova de identidade contínua.

Em cada checkpoint aparecem12 entradas com flagINHERIT: stdin76,stdout2972,stderr88 e9 candidatas não stdio (Section608/612/616/620/2596/2964;Event1216;File84/1396). Elas têm os mesmos valores/metadados observados nos três pontos, sem prova de que sejam os mesmos objetos durante todos os intervalos. Flag herdável no filho não prova origem no pai, e nenhuma dessas9 foi provada dispensável. Não tocar, sinalizar, alterar flags ou fechar objetos reais para testar isso.

O snapshot de entrada já ocorre depois de CRT, console e arquivo de aprovação;70 entradas não mapeadas não significam70 herdadas nem70 alocadas pelo Windows/SCM. A tabela final99 não mapeadas inclui as candidatas e estado anterior/interno: não somar12+99 nem rotular99 como SCM. Mapas completos estão em MANUAL-N0HT01-HANDLE-TABLES.csv e MANUAL-N0HT01-RECONCILIATION.json.

## Restrição de capacidade

PowerShell711 + auxiliar273 =984/1024 (margem40 no conjunto observado). Não usar essa margem no guard do qualificador. Qualificador126 antes de PSS de threads,128 no stack-VA e126 após fechamento; margem zero no ponto amostrado. PSS de tabela acrescentou1 nos checkpoints; o PSS de threads/thread aberto acrescentou2 nos pontos observados. Capturas são sequenciais, não somar1+2 como se coexistissem. Pico entre pontos e kernel/transitivos continuam UNKNOWN.

A primeira restrição ainda não resolvida é coexistência obrigatória positiva após binding: o self-token encerra1 no ramo positivo, mas intent e imagem acrescentam2 referências mantidas até fechamento. Se todo restante fosse igual,126−1+2+2=129 préStart; ao adicionar o processo do serviço,130 no primeiro monitoramento. São projeções condicionais, não picos LAB. No ciclo posterior,131 estáveis após3 referências cycle_live,132 com thread temporária, ou133 no PSS posterior são projeções de fases SEQUENCIAIS; não somar todas as parcelas como pico134. O máximo pré-bind/handoff também não foi amostrado; não declarar que toda trajetória read-only respeitou um pico universal128.

Nenhuma das27 referências pode ser fechada genericamente para produzir PASS: self-token já é candidato implementado somente positivo; originais protegidos/canônicos ainda são revalidados; intent/imagem/processo continuam necessários; C1 após cycle_live não resolve as primeiras fases. Somente memória disponível não demonstra capacidade. first0/close0 comprovam o alcance dos closes locais do recibo, não quiescência global; quiescent0 permanece UNPROVEN.

## Proposta concreta e decisão necessária

Recomendo como direção a criação do filho com **herança explicitamente limitada aos canais necessários**, dentro do mesmo PowerShell e sem outro owner/processo. Não há implementação ativa dessa alternativa nem alegação de que eliminará9 handles reais. Sua segurança exige stdio/console, processo original, Stop e retenção demonstrados e falha fechada para dependência implícita não prevista.

A proposta está em docs/adr/ADR-CANDIDATE-n0start03-explicit-child-handle-inheritance-20261024.md, revisão1I0–I6, statusPROPOSED. SHA256457f79568a251622e7f7b53db1d7bbb384e9e3e03afc80b1ca9d92339379d20a. Ela submete ao owner uma escolha durável do contrato de capacidades no nascimento, incluindo o risco de retirar dependência implícita desconhecida; não afirma que o contrato vigente proíbe toda implementação seletiva nem que é inevitável alterar a arquitetura. Nenhuma decisão accepted foi editada ou criada.

A alternativa de um único objeto protegido de políticas poderia reduzir3 handles controlados, mas troca a cadeia de provas dos originais e não resolve o ciclo completo. Aumentar quotas, mover custos para PowerShell ou dispensar PSS/RAW1 não são alternativas aceitas. Se a direção de herança explícita for recusada ou não qualificada, manter DENY.

Próxima decisão concreta: selecionar/aprovar ou rejeitar a direçãoI0–I6. Após seleção aprovada, qualificar o mecanismo seletivo no fixture existente e a supervisão antes de qualquer ativação/pin ou autorização operacional nova. Não fazer uma nova rodada igual de checkpoints e não usar a aprovação56aa3bea novamente. Não entregar pacote Start como se estivesse qualificado.

## Preservação e gates

Nenhum fonte funcional, helper, launcher ou pin foi modificado nesta reconciliação. Qualificador2bf1746ae9de3f5d559b4fc989d9da3c3f9dacfe47539f8b0b067e2fe496ae15 e launcher3451d4d5e2b2dd6bfae8ac64fc97293b63a9d7a3658fd0d00cba805be35bf2ab continuam iguais. Builds/testes32/30/8 anteriores foram reaproveitados; novos checks são de parse/integridade da evidência recebida, não nova prova nativa. Nenhum Prepare, SCM, ACL, Start/Stop ou efeito foi executado pelo Codex. Aprovação usada virou histórica, sem rearmar. H/P3/N7 e UNKNOWN preservados; reservas reais não liberadas; NONE_GRANTED.

MatrizED11 atualizada somente nas linhas afetadas e roadmapPhase6 continua separado. Read-only não encerra serviçoB4.C/psr_prepare/NTSTATUS/Stop/quiescência, C2/C3/Browser/lifecycle/revogação. ED11 e Phase6 permanecem PARTIAL.

CURRENT_TASK_COMPLETED=YES
CLOSURE_CONCLUSION=B_CAPACITY_NOT_DEMONSTRATED
HANDLE_ATTRIBUTION=PARTIAL_FACTUAL_PHASE_AND_CONTROLLED_SLOT_ATTRIBUTION
INHERITED_REAL_HANDLES=UNKNOWN;12_FLAGGED;9_NONSTDIO_CANDIDATES
CONTROLLED_HANDLES=27_AT_FINAL_CHECKPOINT
INTERNAL_WINDOWS_SCM_HANDLES=UNKNOWN;26_UNMAPPED_NET_INCREASE_IN_INITIAL_SCM_INTERVAL
SAFE_REDUCTION=NOT_DEMONSTRATED
POSITIVE_PRESTART_CAPACITY=NOT_DEMONSTRATED
FIRST_MONITOR_CAPACITY=NOT_DEMONSTRATED
READONLY_EXECUTION=MANUAL_RESULT_RECEIVED_SINGLE_USE_HISTORICAL
START_EXECUTED_BY_CODEX=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
UNKNOWN_CHARGES=RETAINED
INDEPENDENT_BUDGET=NOT_GRANTED
ED11_TOTAL_STATUS=PARTIAL
PHASE6_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
FIRST_UNRESOLVED_GATE=EXPLICIT_CHILD_CAPABILITY_BOUNDARY_DIRECTION_AND_QUALIFICATION
NEXT=OWNER_DECISION_ON_I0_I6_EXPLICIT_CHILD_INHERITANCE_PROPOSAL
