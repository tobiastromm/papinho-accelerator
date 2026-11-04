# ED11 — Reconciliação Q4 e D4/D5

TASK `PACC-PHASE6-ED11-Q4-READBACK-RECONCILIATION-AND-D4D5`, 2026-10-26.
Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469. WIP anterior preservado. Documentação viva/evidência técnica, sem nova decisão normativa.

## Q4: primeira falha e fechamento

O owner reportou uma tentativa manual Verify do script aa9b398dfe29443be1a51faa48eaf4c4418720b531c21505a744e5843daff5e4 para N0TOKEN-20261025-01 / 10584f02e11f92c92b43cddf0d9d316f. Primeira falha: HOST_BOOT_DENY, linha48. Transcrição separada é evidência reportada, não captura independente; sem horário/tuple atual inventado.

O guard combina três comparações por OR: hash do host, boot RecordId e boot StartTime. A saída NÃO identifica qual divergiu. Reboot é hipótese, não diagnóstico demonstrado. Expected host2b9bba4c16c1010239890f61ad2247544186e928623446ad40470cba28fcbcff, boot236441, StartTime2026-10-23T18:36:34.5000000Z, operador S-1-5-21-785699550-1440458426-1682014700-1108 permanecem pins históricos intactos.

Pela ordem do script inalterado, a falha sucede script/runtime/source checks, compilação, confirmação e checks de console/token/operador/impersonação. Precede os checks posteriores de receipt/decision, PaccQ4EffectsRead.Run(), PaccQ4InputRead.Run() e ambos os sumários finais. Assim, condicionado à execução reportada, nenhum leitor protegido foi alcançado e nenhum objeto protegido foi adquirido por esses leitores. Isto NÃO é receipt close0/RETAINED=False/exit final bem-sucedido. A exceção explica os sumários ausentes; prompt retornado não atesta saída do processo nem todo cleanup interno/helper/runtime. Não inventar falha de CloseHandle protegido também.

Identidades originais, RAW1 e conteúdo NÃO foram verificados novamente. Sink417614100:10354688:7367, inputs com IDs/digests da colocação e hashes esperados de imagem/payload permanecem expectativas/evidência histórica. IDs históricos não impressos de intent/imagem continuam UNKNOWN; SACL/labels, resíduo opaco e quiescência global continuam UNKNOWN/UNPROVEN. Start1/Stop0/intent consumido, primeiras falhas, prova anterior de processo original terminado/fechamento local e reservas indisponíveis não mudam.

Não executar/repetir Verify, re-pin boot, consultar efeitos protegidos ou criar identidade sucessora. A autoridade não é descrita como ainda não tentada: houve tentativa manual reportada e negada; nenhum retry é inferido.

## Disposição e E21

R6–R8 accepted permite avaliação; R7 exige readback autorizado por handles originais, native IDs/RAW1/conteúdo e revalidação nos mesmos handles. D05 da matriz histórica continua insatisfeita. D06 daquela matriz é texto histórico anterior à aceitação R0–R9; arquivo arquivado preservado. D01–D04 são evidência anterior limitada; D07/aprovação exata de aplicação ausente.

Candidata: CLOSED_ORIGINAL_LOCAL_REFERENCES_WITH_PERSISTENT_EFFECTS_AND_UNKNOWN_RETAINED, somente para a identidade consumida acima. Q4-DISPOSITION-READINESS.json vincula falha e lacunas: APPROVABLE=false, aplicação DENY. Não há base para pedir aprovação aplicável da disposição com R7 ausente. O próximo owner decision precisa resolver explicitamente a incompatibilidade host/boot e o caminho permitido para evidência; não há novo comando protegido aqui. Nenhum pedido de waiver, retry, refund ou aprovação com placeholders.

E21 permanece PENDENTE_READBACK_E_DISPOSICAO_DOCUMENTAL, com evidência corrente da tentativa negada. E22 dependente; ED11 4 demonstrados/12 limitados/6 pendentes; Phase6 PARTIAL; autoridades produtivas DENY. Linhas históricas, classificações, reservas e UNKNOWN preservados.

## D4: parâmetros demonstrados e propostas fundamentadas

| Parâmetro | Fato | Proposta/lacuna |
|---|---|---|
| Maximum age | C8 exige coleta<=100ms e registra expiry=start+1000ms; mecanismos, não policy Mode C. | Escolher A por métrica/ponto de validação com latências reais de coleta/scheduling/uso. Exigir age dentro de A E expiry original no domínio local original; sem renovação ao exportar. A numérico não justificado ainda. |
| Collection validity | Cadência, duração de coleta e reuso são dimensões diferentes; expiry C8 não aprova aplicabilidade. | Definir V e seus pontos de início/fim; janela aceita também precisa caber na validade original. Ampliar expiry exige evidência/decisão, não cópia de timestamp. Sem comparação de clocks de processos. V numérico pendente. |
| Duração | Setup/connect/drain5s, handshake10s, idle reiniciado por progresso30s, C3 setup5s são guards locais. | Separar deadline absoluto de observação da operação e prazo de observação de Stop/drain. Incluir preparação, chamadas nativas, scheduling e finalização. Não somar fases sobrepostas. Retenção sobrevive ao prazo; total não demonstrado. |
| Site pump | Caller passa60000ms; site_active compara GetTickCount-started>=duration desde criação. | É duração absoluta de uma fase checada no uso, não apenas idle; não limita preparação anterior, bloqueios, cleanup ou retenção. |
| Browser | Charge1858020/cap2097152: residual239132 (11,40%); duas charges3716040 excedem cap por1618888 bytes. | No máximo uma charge ordinária cabe nesse cálculo restrito; zero operações admitidas hoje. Shared/outros/retidos reduzem residual. Não é headroom físico/commit/crédito opaco. |
| Accelerator | Selected1965128/3145728 residual1180600; default1799632/2097152 residual297520 bytes. | Preservar profiles. C2manager527824/C3bridge132448; subset payload393216/op já embutido. Não somar novamente nem transferir quota. |
| Physical headroom | Sem observação fresca do host composto/workload vinculado; managed bytes não são resident growth total. | Piso exige crescimento residente simultâneo restante+shared/kernel/transientes+concorrência externa no horizonte coleta/uso/Stop, sem sobreposição. Termos opacos impedem valor numérico. Não importar5GiB LAB. |
| Commit headroom | Private/commit/working set diferem e se sobrepõem. | Commit limit-total atual versus commit adicional justificado até falha/retenção e demanda concorrente. Não somar private+commit nem picos por processo como bound global. Não importar9GiB LAB. |

Relação quantitativa proposta, NÃO política adotada: headroom adicional requerido = crescimento controlado restante demonstrado/disjunto + crescimento provider/shared/kernel/transiente justificado separadamente + demanda concorrente baseada em evidência no horizonte aceito. Retidos entram uma vez no owner/baseline real e continuam indisponíveis; não duplicar custo presente como crescimento futuro. Termo ausente torna a relação indeterminada, não zero. Ledgers físico/commit separados. Não inventar multiplier/zero/infinito/samplemax como teto.

Experimento anterior:20processos/500samples/240lifetimes e records initialized0. Não mede TLS/network ativo nem par retido; crescimento amostrado zero não cobre unsampled peaks nem renova observações históricas. Snapshot4648bytes e metadata60bytes não são envelopes provider. Quantidades anteriores: ../composite-target-quantitative-closure-20261026/QUANTITATIVE-RECIPE-AND-ACCOUNTING.md e ../d0-d9-bounded-diagnostic-policy-20261026/QUANTITATIVE-ANALYSIS.md. Nenhum valor de policy foi instalado/aceito aqui.

## D5: bindings/proveniência/shared/retidos

| Categoria | Estado factual | Completar antes de efeitos |
|---|---|---|
| Root/CURRENT/policy/epoch | Guards de configuração committed/CURRENT existem; não houve aquisição administrativa/publicação nesta tarefa. Código não é receipt atual. | Fonte/owner/lifetime administrativo original, revisão aplicável e epoch factual no ponto autorizado; mudanças invalidam reuso. D2 não fabrica configuração. |
| SiteTLS/destino | Exige profile separado; workload prospectivo um HTTPS text/plain sem redirect/subrecursos/retry. | Profile/trust/nome/SNI/ALPN aprovado, hostname/porta/endpoint resolvido exatos; não usar destino LAB nem criar origem. |
| Host/boot/operador/token | Tuple Q4 esperado histórico; guard falhou sem imprimir operandos atuais. | Bindings próprios factuais no futuro escopo autorizado; não rebatizar tuple Q4 como atual ou reutilizar Q4/Q7. |
| PID/criação/threads | IDs históricos não identificam par futuro; quatro roles LAB não são census normal. | Processos/threads originais, lifetimes e borrow windows reais; não PID isolado ou Session-ID local como identidade global. |
| Módulos | Hash build/SDK/fontes e provider_id/version/initialized não atestam objetos mapeados. | Associar módulos efetivamente mapeados a objetos/backings originais/bytes pela evidência permitida. Não carregar/inicializar provider implicitamente. |
| Topologia/shared | Browser CONTROL/APP/RECORD+Site socket+parent/runtime; C3listener/connector/accepted peer transientes, dois loopback endpoints estáveis. | Census/coexistência por owner uma vez; três sockets C3 não limitam todos os handles do par. Parent existente não implica custo incremental zero. |
| Retenção | Dependências/charges incertas e obrigações históricas Q4 permanecem. | Ledger por owner/lifetime/fase e compatibilidade de reservas; controlled UNKNOWN/incompatibilidade negam. Deadline/remote FINAL/saída local não refundam. |

## Divergências, decisões e caminho crítico

Relatórios arquivados dizem Q4 não aprovado/não executado em fases anteriores; handoff D2 posterior registrou confirmação do owner e este trabalho acrescenta tentativa negada. E21 ainda dizia binding da aprovação anterior não reconciliado; documentação corrente corrigida. Divergência temporal documental, sem reescrita de arquivos históricos/decisões accepted. Nenhum conflito normativo novo estabelecido na comparação focal; não é nova auditoria exaustiva de conformidade do repositório.

1. Q4: preservar falha. Decisão explícita do owner sobre incompatibilidade de binding/caminho de evidência antes de nova ação protegida. Não re-pin/retry. Somente com R7 completo preparar pedido exato e aprovação SEPARADA de disposição para E21.
2. D4/D5: justificar latências/janelas/horizonte absoluto, pisos físico/commit distintos e bindings administrativos/Site/processo/módulos por procedimentos autorizados. Completar anexo numérico antes de efeitos. D2 accepted SOMENTE para preparação; não perguntar novamente por B/D2.
3. Não há correção funcional justificada para recusa pré-leitor. Script/pins intactos; nenhuma nova campanha/build de produto por alteração documental. Preparação técnica passiva permanece autorizada; mudanças concretas devem ter builds/regressões pertinentes.
4. Operação correlacionada futura exige D4/D5 normativos completos e pedido manual exato separado. E19/E20 precisam composição ativa/provider/kernel reais; initialized0 ou initialized1 simulado não passam. E17/E18 precisam prova real própria de serviço/acesso/lifecycle/Stop, sem replay Q4/Q7.
5. Resolver E17–E21/limitações antes de derivar E22. Sem admissão/rede/Start/Stop/SCM/ACL/CURRENT/assinatura/recuperação autoritativa/limpeza/refund/identidade operacional nova/commit/push.

Verificação: VERIFICATION.json, somente fontes/evidências do repositório; nenhum efeito protegido aberto. Consistência/fluxo não substituem prova nativa ausente.
