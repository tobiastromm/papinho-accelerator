# G0–G9 — resultado factual da integração offline

Task PACC-PHASE6-G0-G9-ACCEPTANCE-AND-COLD-CHAIN-COMPLETION.
Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469. WIP preexistente
preservado, sem commit/reset/stage. **Resultado parcial, não cadeia fria completa.**

## Decisão e código ligado

SHA256 original conferido literalmente:
`d65da3287ac5c1da7f59e6a467498ae2198f0896db801584b4b6c5fedad0ad1b`.
[Decisão accepted](../../adr/DECISION-personal-first-diagnostic-start-assurance-g0-g9-20261031.md)
incorpora B/G0–G9 sem editar a proposta ou ampliar autorização operacional.
G1 especializa custos opacos; G2 conserva origem; G3 não seleciona números.

Caller real: main.c.api3.inc → launcher privado frio → owner canônico vazio →
papacc_configuration_personal_scq_cold_controller → scqc_plan → aquisição
original existente → captura do plano G3 → first_controller existente.
Não é somente uma API sem caller. O seletor privado aceita paths/pins/parâmetros
como DATA e exige aquisição nativa e três confirmações separadas SCQ/endpoint/Start.
Não foi emitido comando operacional para essa entrada.

scqc_plan utiliza token/grupos/restrições atuais, known folders, abertura NT
somente FILE_OPEN com direitos de leitura, SHARE_READ/OBJ_DONT_REPARSE,
identidade/descritor/conteúdo dos originais, correlação da imagem protegida,
tentativa CSPRNG e lifetime explícito. Não inventa PID/token futuros. A resolução
da conta virtual não é autenticação do runtime. A associação de imagem não é
atestação do módulo mapeado nem uma autoridade. Request/decisão/READ permanecem
estritos; guarda TO da imagem permanece a anteriormente aceita.

Os handles brutos temporários fecham uma vez, após obtenção das referências
retidas. Falhas conhecidas anteriores também fecham; pending nativo/hash/SD e
falha de close conservam ownership/UNKNOWN sem repetir o fechamento. Testados
erro após primeira aquisição, origem divergente e aquisição/hash incertos.

O ator G0 no mesmo packet congela limites antes das confirmações, exige plano
mostrado e três atos originais, captura READY sem rebaseline e verifica QPC,
freshness/duração, handles completos, crescimento privado/working set e margens
física/commit. A ponte launcher/SCM consulta o ator original antes de efeitos.
Guard124/cap128 preservados no SCQ; nenhum limite transplantado de staging.
LIMITED_RISK_ELIGIBLE não apaga resource_pre_missing=63 nem obrigações posteriores=7.

## Corte da cadeia fria

[Corte preciso](COLD-RECEIVER-CUT.md): controller-local integrado; receptor nativo
inicial continua sem fonte independente. O getter não recebe aprovação derivada
de carrier/peer/hash/arquivo. O relatório expõe initial_receiver_source_unavailable=1
e conserva o bit DELIVERY_CONTINUATION: nega namespace/Start antes dos efeitos.
Não foi alterado scqb_open para abrir pipe sem essa fonte. A cadeia de modelo
preexistente com seleção receptora fornecida não prova um producer frio.

G0–G9 não contém a autorização que faltaria a uma abertura anterior à origem,
nem dispensa B4/B5/S1/S4. A hipótese de promover QUERY a partir de peer/imagem
foi rejeitada na revisão: elegibilidade/bytes não demonstram confirmação e
consumo originais. Nenhuma aprovação foi reconstruída ou registrada por ela.

## Qualificação final

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings nos logs finais.
27 CTests afetados por configuração, **108 pares teste/configuração PASS**.
Não somar execuções intermediárias ou campanhas históricas.

Novos testes: cold_open 13 cenários/28 checks; cold_controller 17/153;
start_assurance 33/506, por configuração. Orquestração/atores reais; APIs NT,
token, arquivos, RNG, CNG, console, SCM e endpoint sensíveis modelados nesses
positivos. Não houve endpoint/Start/KSP/RNG protegido real. Helpers de SID/thread
usados pelas regressões não provam bilateralidade nativa.

Regressões incluem fonte ausente, replay/consumo, endpoint/Start independentes,
Stop, peer perdido, receipt/close incompletos, retenção e readiness prematura.
Falhas iniciais de compilação de modelos corrigidas, logs iniciais preservados.
[EVIDENCE.json](EVIDENCE.json) lista hashes e logs; [qualify.ps1](qualify.ps1)
repete apenas a suíte afetada com os efeitos modelados.

## Recursos, imagem e próximo gate

Censo x64: FIXED_GRAPH=1569504, KNOWN=1994944, ROOT=367896,
COMMIT=289600, PENDING=97288 bytes; componentes embutidos, não somar novamente.
São storage compilado, não heap/stack/RPC/loader/kernel máximos ou margem real.
G1 admite somente o risco residual especificado; custos opacos continuam UNKNOWN.
Plano G3 operacional numérico e observações frescas das duas pontas pendentes;
não foram copiadas escolhas B3/R0–R9. Contagem real do runtime é prova posterior.

[Delta de imagem](STAGED-IMAGE-DELTA.md): novo EXE de workspace 701440 bytes,
SHA256 `3c90c3adb8e427a16b65670b4ecad07912c62b2f86f17508af3cbdbb22ac530d`;
DLLs sem delta contra pins históricos. EXE staged histórico não atualizado nem
reconsultado. Não congelar artefato operacional enquanto ingresso incompleto.

Primeiro bloqueio: fonte independente do receptor antes do primeiro I/O; depois,
plano G3 concreto das duas pontas, atualização governada e autorizações separadas
de endpoint/Start/Stop. Pedido operacional **NOT_READY**, autoridade **DENY**.
Nenhuma operação protegida ou consulta operacional executada nesta tarefa.
OS_QUIESCENCE UNKNOWN; E21 PENDING; ED11/Phase6 PARTIAL; PST0.6.4 preservado.

[Próximo prompt executável](NEXT-RECEIVER-ORIGINAL-ADMISSION.md).
