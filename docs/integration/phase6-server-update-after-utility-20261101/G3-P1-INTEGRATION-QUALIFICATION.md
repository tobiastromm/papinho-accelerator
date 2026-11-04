# G3-P1 — integração e qualificação offline

2026-11-02. main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469.
IMPLEMENTED_OFFLINE / QUALIFIED / NO_OPERATIONAL_AUTHORITY.

## Decisão e resultado

Proposta original byte-exata 9559fbb0f82e875ad33a6dfc80c52fa4572ae3267338d6ff4401c2a901881abf preservada. Alternativa B aceita
por incorporação em [DECISION-personal-scq-p1-handle-envelope-20261102.md](../../adr/DECISION-personal-scq-p1-handle-envelope-20261102.md),
antes do código dependente. Precedência focal sobre números G4/QUERY-B3-G3/P5/
SSO-B-S6/TO7/ONA-B7, preservando os contratos anteriores e todos os demais gates.
Nenhuma divergência normativa residual identificada nesse escopo.

P1 PERSONAL_LOCAL TRANSPORT_ONLY: entrada<=128, aquisição count<188,
contagem total<=192, por participante. C44/U16/folga4 permanecem composição de
risco, não máximos nativos nem reserva física. READ/NEW_INSTALL e demais
perfis não recebem ampliação. P0 seleciona objeto existente, não novo destino.

## Integração efetiva

- Política privada central em personal_scq_handle_policy_win32.h; política
  antiga permanece padrão. Campo max_handles e preimage/digest quantitativo
  existentes reutilizados; nenhum tamanho/versão de SCQ1/SQB1/SQB2 mudou.
- CLI fixa --owner-manual-first-scq-start-to-existing exige192; entrada antiga
  exige128. Nenhuma entrada universal, teto escolhido por peer ou fallback.
  Seleção P1 continua passando pelo token original e seletor TO/AMD64 existente.
- scqc_plan exige seleção quantitativa exata do perfil; raw-open usa primeira
  query EXISTENTE após as inicializações necessárias e preserva initial_known/
  initial_handles. Entrada129 nega antes de abrir; consultas seguintes não
  sobrescrevem essa prova. Queries e handles não foram deslocados/subtraídos.
- Duplicatas locais, cadeia original e readers usam188 somente no contexto
  tipado selected-object P1/TO. Owner/DACL, identidade/proveniência, namespace
  nativo/OBJ_DONT_REPARSE, ancestrais e referências retidas permanecem obrigatórios.
- Owner captura G3-P1 somente com paths P1 e transfere a primeira contagem ao
  assurance; sem essa prova a amostra P1 nega. Fonte, três atos e consumos
  independentes permanecem requisitos anteriores aos efeitos correspondentes.
- Receptor estabelece contexto runtime fechado original antes da primeira
  admissão QUERY; política LOCAL192 e digest exato são conferidos antes de
  abrir endpoint. A primeira amostra local exige<=128. Nenhum PID/token futuro
  é exigido antes do processo; carrier/policy DATA não constituem autoridade.
- Eventos/pipe/peer/entrega usam G3 do mesmo owner, count total, freshness até
  efeito, deadlines mínimos e sem renovação. QUERY sem scope nega; phase UNKNOWN
  não permite nova aquisição. Receptor continua sem admissão administrativa
  até provas originais e autenticação bilateral.
- Antes de CADA DuplicateHandle remoto, query completa da tabela destino pela
  referência delivery_process já retida e correlacionada ao runtime original,
  sem novo OpenProcess para medir; count>=188, falha, Stop ou prazo negam.
  A query usa QPC/frequência/freshness100ms. Não exclui os IDs remotos da tabela
  destino nem promete ausência de alteração concorrente após a query.

Censo44 revalidado: raw3 + cadeia9 + inputs6 + transporte14 + identidade3 +
operador receptor2 + views operador fonte2 + SCM normal2 + console1 + token1 +
token temporário1. Mesmos arrays/lifetimes, sem slot HANDLE novo. Adicionados
somente escalares de política/primeira contagem e queries pelo processo retido.
Aliases/baseline/ramos continuam conservadoramente sobrecontados; recursos
internos e SC_HANDLE/RPC/provider/kernel continuam opacos.

## Qualificação

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings, mesma receita
MSVC x64/W4/WX, mesmas dependências/PST0.6.4. Receita build-g3-p1-offline.cmd.
**56 pares teste/configuração PASS**:13 CTests focais + install_paths por build.
Logs finais g3-p1-*-build.log, g3-p1-*-tests.log e g3-p1-*-paths-tests.log;
hashes em G3-P1-INTEGRATION-EVIDENCE.json. Não foi repetida campanha operacional
ou campanha inalterada do updater. Os testes usam APIs protegidas simuladas.

Fronteiras explícitas: entrada127/128/129; preaquisição187/188/189;
cap191/192/193; MAXDWORD/overflow CLI; query indisponível; perfil/seleção ausente
ou divergente; política antiga não aceita192 e P1 não cai para128; Stop,
freshness vencida, retenção UNKNOWN, readback/proveniência/namespace/replay e
readiness prematura. Modelo completo P1 QUERY/SQB2 com três atos, entrega remota,
autenticação, receipt/close passa; demais casos continuam exercitando política
antiga e negativos. Coexistência raw/cadeia/cópias/readers preservada; o teste
que esperava negação124 no P1 agora demonstra aquisição dos três readers sem
fechar originais. READ e NEW_INSTALL continuam exercitados pelos testes de
inputs/proteção e install_paths; nenhuma quota LAB/BD/UPDATE foi modificada.

Limites restantes G3 preservados: private/WS1994944 bytes por participante,
físico7239172096 e commit9823326208 bytes, freshness100ms,
janela240000ms, duração global60000ms. Memória managed3MiB: census PASS nas
quatro configurações. Não se afirma teto nativo, capacidade ou latência.

## Artefato e primeiro gate

Novo Selected/Release: **783360 bytes**, SHA256
**2d056518dd4da617e747e0e79b8e78acf5aab54b8b694118bfec95b75deb4d13**.
Delta+2048 do instalado HISTÓRICO781312, ID417614100:8716288:38019,
SHA256dc4855c4644cc0255e86aeeea3b5799ae15c4741cf8468cd00a489bb63b43076.
Cópias congeladas workspace antiga/nova preservadas. DLLs byte-idênticas aos
pins originais. Não consultados nem modificados imagem instalada ou SCM agora.

Pedido [ONE-G3-P1-IMAGE-UPDATE-REQUEST.CANDIDATE.md](ONE-G3-P1-IMAGE-UPDATE-REQUEST.CANDIDATE.md),
SHA256 **bafa1b8a58c8eed646f00998597c1f4b053262967a78c3ac3603af860270dda0**, pronto para seleção/autorização específica do owner.
Procedimento candidato update-g3-p1-image.MANUAL.CANDIDATE.ps1,
SHA256 **fa96e9c9cc5ee61cec79ca0599ecd5105dbe1092f2b141518d41b05bc6e3f318**; AST válida, seis helpers byte-idênticos ao anterior,
somente deltas de seleção/IDs/tamanhos/pins/nome do pedido/banner. Não executado.

Primeiro gate: autorização operacional específica desse UPDATE. Identidade,
proteção/continuidade, DLLs, executor e SCM STOPPED/PID0 devem ser readquiridos
em gates FRESCOS na futura chamada; estado histórico não foi promovido a PASS.
Retenção histórica/OS UNKNOWN podem impedir o ato e exigem STOP, não cleanup.

Caminho mínimo: aprovação do UPDATE -> procedimento manual único -> reconciliar
novo canônico/anterior/readback/SCM/close -> novos originais e pedido E/Q/S
independente com max192 -> autorização específica -> sessão manual única.
Entrada real e recursos, autenticação bilateral, receipt/close e término finito
continuam provas pendentes; não presumir que o novo envelope assegura Start.
Nenhum retry, coleta, UPDATE, E/Q/S/Start/Stop, ACL/SCM, CA/CURRENT, rede,
TPM/NV/Q4, cleanup/refund foi executado. DENY produtivo, UNKNOWN, históricos,
pins, atos consumidos, HEAD/WIP/EXEs/PST0.6.4 preservados. Phase6/ED11 PARTIAL.
