# Fronteiras de handles — preparação offline concluída

Origem histórica:136 é contagem nativa própria confirmada; sua decomposição
individual permanece UNKNOWN. Não se demonstrou fechamento seguro ou bug de
ownership. Nenhum guard, quota ou contrato foi alterado. A decisão G3/G4 não foi
reaberta. Nenhum dado de outro processo foi usado para reconstruir a sessão.

O código comprova um token original próprio até a guarda, buffers knownfolder
liberados, bridge sem HANDLE próprio devolvido e ausência de SCM/pipe/arquivos/
duplicates posteriores. CRT/loader/console inicial/herdados e recursos internos
de membership/knownfolders não têm atribuição individual nesta evidência.
Pseudo-handles não se fecham. std streams são emprestados e continuam necessários.

Como observação nova é indispensável para separar fases, foi preparado modo
temporário read-only no mesmo owner/binário, sem serviço, subsistema ou autoridade.
Só aceita --observe-cold-handle-boundaries; não recebe originais/autorizações.
Observação registra entrada POST-CRT, pós-owner, pós-console, pós-token e
pós-knownfolders/drive, antes de qualquer arquivo/RNG/SCM/SCQ. Sem output antes
das amostras; 60bytes fixos cobrados separadamente. Não há enumeração global.

Limite de suficiência expresso: não existe medição pre-CRT; slot0 contém CRT/
loader/herdados. Não prometer identificar cada handle se o excesso já existir
na entrada. Deltas localizam fases numa invocação nova, sem provar causa histórica
ou que referências possam fechar. Nenhuma coleta foi executada.

Implementação: personal_scq_cold_acquisition_win32.inc (coletor restrito),
server_launcher_scq_cold_start_win32.inc (dispatch exclusivo/close/evidência),
server_configuration_commit_win32.h (declaração privada) e scq_cold_open_test.c.
Entrada operacional anterior continua a exigir123 ou menos antes de novo handle;
observação acima desse valor NÃO inicia SCQ nem concede admissão.

Qualificação: quatro builds PASS sem warnings, três testes afetados por configuração
(censo, cold_open e cold_controller):12pares PASS. cold_open32casos/115checks,
incluindo136 somente observado, falhas/Stop e ausência de efeito. cold_controller
17casos/153checks preservados. Logs cold-boundary-*-build/tests/details.log.
Não foram repetidas campanhas não afetadas. APIs de coleta modeladas nos testes;
nenhuma execução do novo modo ou processo original de sessão.

Novo candidato diagnóstico765952bytes SHA256
714fedbff069af841766d158bd4f833eab6d066e2ebe37c8995aa161d6c3d546.
O canônico instalado permanece7d29...ca7d/763904bytes; não atualizar nem selecionar
o novo candidato para Start. Somente a coleta read-only poderá usar esse artefato
do workspace sem promover bytes ou token a autoridade administrativa.

Pedido ONE-COLD-BOUNDARY-OBSERVATION-REQUEST.CANDIDATE.md pronto para seleção/
autorização específica de uma execução manual read-only, ainda NÃO autorizado.
Nenhum comando com efeitos emitido. Esta observação não é retry da sessão,
não reutilizaef8b/af8529 e não toca documentos ou retidos.

Primeiro gate: decisão do proprietário sobre essa coleta delimitada e seus
limites/riscos. Depois preparar comando literal e aguardar saída única. Se fase
atribuir aquisição desnecessária comprovada, corrigir/qualificar apenas essa;
sem prova, nenhum fechamento. Sessão E/Q/S continuará exigindo outro pedido,
fontes legítimas e G3 fresco, sem alterar124/128. Retenção histórica, causa1816
anterior e quiescência OS UNKNOWN; readiness produtiva DENY; PST0.6.4 intacto.
