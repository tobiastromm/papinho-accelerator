# P1 — observador focal implementado e qualificado offline

2026-11-02, America/Sao_Paulo. main, HEAD
f341985924d29870d8ff627ed37c8a0874ad6469; WIP/históricos preservados.
IMPLEMENTED_OFFLINE / QUALIFIED / OPERATION_NOT_AUTHORIZED / NOT_EXECUTED.

## Resultado e aplicabilidade

Preparada uma única prova prospectiva de nove cortes, sem UPDATE do instalado
nem nova infraestrutura. Não há redução de handles implementada ou leak
demonstrado. A origem do125 histórico continua UNKNOWN. Pedido e decisão da
sessão608b6...4441 permanecem históricos; nenhum ato consumido foi repetido.

OHI e P0–P7 continuam exigindo custódia/identidade/proteção/proveniência dos
originais, sem close do raw0 enquanto necessário; P5 só admite transferência
com continuidade provada. QUERY/G3/G4 mantêm guard124 e cap128. Nada disso é
especializado pela prova, que não ingressa em admissão administrativa ou
readiness. Referências normativas e distinção de simultaneidade estão em
P1-REQUEST-DECISION-OWNERSHIP.md; os nove cortes seguem a especificação existente.
Não houve alteração de ADR/contrato accepted ou importação de outro envelope.

## Integração efetiva

personal_scq_cold_acquisition_win32.inc compartilha raw-open e a sequência
GetSecurityInfo/IsValidSD/papacc_store_hash/LocalFree com scqc_plan real.
ObserverNULL no percurso operacional não acrescenta queries. O close raw
preexistente foi movido para personal_scq_cold_close_win32.inc, compartilhado,
sem encerramento antecipado/alternativo. A função privada
papacc_configuration_personal_request_interval_observe fica em
personal_scq_interval_observe_win32.inc e é chamada pelo launcher/owner existente.

Entrada estritamente argc2 --observe-p1-request-decision-interval. Sem argumentos
de caminho/autoridade; caminhos DATA fixos. Não chama scqc_plan, source admission,
seleção administrativa, decisão/imagem, nonce, SCM, endpoint, SCQ ou Start.
Token/console/Stop existentes; replay do packet é rejeitado. Mantém NTSTATUS,
IOSB e handle flag antes de qualquer interpretação Win32. Primeira falha imutável.
Raw0 só é fechado uma vez em encerramento diagnóstico seguro. Cleanup incerto
preserva unresolved/UNKNOWN, sem recuperação, rollback ou retry.

Nove cortes: queryQ0; lookupntdll; NtCreateFile; tipo/ID; SDválido; hash+cleanup;
LocalFree; drive da decisão; resolver/queryterminal sem decisão. Flags não
enumeram handles internos nem expõem descritor/conteúdo/referências sensíveis.
Queries extras param >128; em1/4 também >=124. São stops conservadores adicionais,
não permissão para ultrapassar guard. Final125, se observado, será DENY/1816.

## Ownership, custos e equivalência

Raw0 obrigatório retido até término; checking temporário; hash/provider possuem
cleanup público no helper original. Flags0 após cleanup não provam quiescência
interna Windows/CNG nem número de handles internos. Não há fechamento arbitrário.
144bytes de samples; estrutura784bytes x64 cobrada à reserva gerenciada existente.
Sete queries extras/pseudohandle; sem print entre cortes. Stack/heap/CRT/loader/
provider/kernel/latência podem perturbar observação, não teto demonstrado.

O seletor operacional recusa módulo workspace: essa guarda NÃO foi removida.
Observer mantém install_paths.phase0 e usa apenas resolver DATA de baixo nível.
GetModuleFileNameW continua chamado, sem fingir proveniência instalada. Mesmo
server completo/toolchain/CRT, mas outro processo/token/localização e ausência
do ritual administrativo P1: não equivalência histórica integral. Imports PE
estáticos normalizados:291nomes,nenhuma diferença contra baseline774656/hash981658...0220.
Isso não é prova de módulos dinâmicos ou imagem mapeada. Nenhum resultado125
futuro será inferido como causa histórica apenas por coincidir com a contagem.

## Qualificação e retomada após reinício

| Configuração | Build | CTest afetados |
|---|---|---|
| Selected Debug | PASS, zero warnings | 15/15 PASS |
| Selected Release | PASS, zero warnings | 15/15 PASS |
| Legacy Debug | PASS, zero warnings | 15/15 PASS |
| Legacy Release | PASS, zero warnings | 15/15 PASS |

Total60pares teste/configuração. raw-open98casos/758checks, incluindo34novos
casos modelados de nove cortes; entrypoint9casos/18checks. Sucesso, terminal125,
queryfalha e Stop em cada corte, guard124/cap128, SD/hash/cleanup/LocalFree/close
incertos, ID/reparse/pending, replay, proibição de decisão/imagem e argumentos
extras. APIs-alvo simuladas; prints P1_INTERVAL do teste de entrypoint são modelo,
NÃO coleta nativa nem evidência do host.

Após reinício informado pelo proprietário, foram conferidos logs/arquivos.
LegacyRelease encontrou CVT1107/LNK1123 em um .obj de presenter. Regenerado
somente esse derivado de build e concluídos build/testes afetados. Não se
atribui a causa da tela azul ao projeto sem evidência. Sem limpeza dos objetos
operacionais, alteração de toolchain/target ou nova coleta para investigá-la.

Logs p1-interval-*-build.log, Legacy-Release-rebuild.log e *-tests.log;
P1-INTERVAL-OFFLINE-EVIDENCE.json registra pins e comparação estática/imports.
Procedimento candidato: parserPowerShell0erros; não executado.

## Artefatos e primeiro gate

Workspace EXE779776bytes:
7b6d5e500723ba5db2f1d337cb1ca55e9fa7b3c5588ca6b31d4c7d81e693e726.
Instalado permanece baseline conhecido774656bytes/ID417614100:1638400:813992;
nenhum readback fresco instalado/SCM foi realizado ou presumido.

Pedido ONE-P1-INTERVAL-OBSERVATION-REQUEST.CANDIDATE.md, SHA256
e6644ce7505c6ae54efd4daadd51b98b3b254a397d83ae77348b472db3bd345d.
Procedimento observe-p1-interval.MANUAL.CANDIDATE.ps1, SHA256
22906613fd4ff0e9206e830160caac7109225d23c845c7aadf6020ec07e5ffd9.

Primeiro gate: autorização específica de UMA observação prospectiva read-only
com essas limitações. Nenhum comando executado/preparado como autorização já
emitida. Após aprovação: registrar ato, conferir pins e preparar comando exato;
depois receber saída manual e localizar deltas da mesma execução. Só então
avaliar correção legítima/primeira lacuna para futura sessão independente E/Q/S.
Não preparar novo Start enquanto admissão permanecer DENY. Preservar124/128/G3,
PST0.6.4, históricos, EXEs, originais e retenção/quiescência OS UNKNOWN.
