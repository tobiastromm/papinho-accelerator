# ED11 — qualificação da composição positiva e gate real

TASK=PACC-PHASE6-ED11-POSITIVE-COMPOSITION-CLOSURE — 2026-10-25.

## Resultado

Duas correções funcionais foram aplicadas e qualificadas offline: o dispatcher stdout foi movido antes dos helpers positivos; três referências de validação live fecham após o último uso somente no ramo positivo N0. Default EMPTY_DENY, entrada positiva compilada OFF e launcher/pins operacionais permanecem preservados. **A composição isolada foi qualificada; a capacidade positiva operacional completa não foi demonstrada e nenhuma admissão foi concedida.**

O gate concreto não é repetir a observação consumida: T0/T8 accepted restringem a integração atual a opt-in read-only/positiva DENY. A nova proposta [T10–T14](../../../adr/ADR-CANDIDATE-n0start03-positive-private-transport-composition-20261025.md), SHA256 `0cb75462607462cf1d719eed016883f811dae59676c0a08822fbe581e4bfeb6c`, explicita a extensão estrita necessária, sem alterá-las silenciosamente ou conceder operação. Após aceitação, executar o [plano restante](remaining-integration-and-operation-plan.md) sob a autonomia técnica vigente e preparar uma única aprovação diagnóstica positiva. Não há comando positivo executável nesta entrega.

## Autoridade e preservação

Branch main, HEAD `913c90cf2328f7939d8f5b927640601903ac6e91`, sem mutação SCM/Git ou ACL existente. Continuidade dos registros e revisões de governança N0–N9/B+D/ED11/L/H/P3/RAW1/N7/Model E/ED08 preservada; T0–T9 foi relida e sua restrição é explícita em T0/T8. AGENTS exige registro normativo aprovado antes de código que dependa de nova decisão e proíbe alterar accepted autonomamente. Nenhum accepted foi editado. A emenda é somente PROPOSED, não aprovação criada pelo agente.

Os sete hashes reconciliados continuam byte-exatos; [conferência](seven-reconciled-pins-preserved.json). Recibo original SHA8502efdfd876c01f753457eebcb95da8d07b7cdb2ee4dc7aa158fc67f2212601 e aprovação read-only consumida preservados, sem reutilização. O binário read-only aprovado NÃO foi sobrescrito: os builds novos estão em outra pasta de artefatos da mesma árvore canônica, sem clone/worktree. Os quatro fontes e as live docs alteradas têm preimagens; [patch final](applied-source.patch), baseline e verificação de WIP identificam os deltas próprios. Produção0.6.4/PST/Browser/wire, dossiê e efeitos02 não foram modificados.

## Correções e prova de ownership

1. `scm-registration-lab.c` inclui o adapter antes de `scm-runtime-cycle-lab.inc`; o include tardio em `scm-instrumented-cycle-lab.inc` foi removido. A tradução compilada mostra printf/puts/fflush dos helpers, inclusive ACL/módulos/token/endpoints, no stream explícito. Stderr, WriteConsole, stdin e confirmações não são remapeados. O adapter executável não mudou, apenas sua posição e comentário. Falha de escrita continua freeze/Stop/retention sem novas aquisições. Não se afirma Unicode arbitrário: o binding mantém rejeição de bytes não ASCII e protocolo atual ASCII/UTF16hex.
2. `cycle_live_close_last()` fecha snapshot de threads, token duplicado de acesso e token primário consultado somente após TODAS as validações bem-sucedidas e seu último uso, com `ig_n0_positive`. Cada sucesso anula seu slot; close incerto conserva o original, primeiro erro e A1 freeze/Stop/retenção. O helper recusa nova tentativa após freeze. Processo original, imagem, intent, exclusividade e dependências permanecem. Falhas anteriores preservam o cleanup/retention original. Rota legada e read-only não executam esse fechamento antecipado.

Nenhum array/limite aumentou. O estado de modo4bytes foi deslocado, não duplicado. Não há novo estado persistente de produto para C1; o custo de código/stack não é zero. No objeto Release do fixture: helper subrsp0x20+push8 (40bytes locais observáveis), cycle_live0x4B18+push16, endpoint0x40068+push32. São frames DIRETOS dessa composição compilada, não limites transitivos de stack nem custo total de memória; DLL/CLR/PSS/CRT/OS permanecem contabilizados por seus contratos. Ver compiled-frames.log e ledger anterior do transporte4464bytes.

## Experimento integrado e limites

O novo experimento REUTILIZA o corpo A–J anterior e extrai byte-textualmente os callers C atuais; não cria outro owner de produto. Compõe transporte privado real, cópia sintética FILE_CREATE relativa ao pai original, A1 real, current-token real, PSS/context/stack metadata, processo sintético e endpoint tables IPv4/IPv6 reais. AccessCheck real é exercitado em negação contra arquivo/token sintéticos. Módulos esperados, ACL de sucesso do serviço e proveniência/RAW1 do LAB continuam substituições explícitas, não provas operacionais. Não há SCM real, serviço LAB, intent operacional ou mutex canônico nesse experimento.

[Ledger por fase](phase-ledger.csv), [coexistência](coexistence-and-ownership.csv), [APIs](native-api-boundaries.csv), [closes](close-api-samples.csv), logs por campanha e identidades/tempo preservados.

| Medida | Resultado final | Alcance |
|---|---|---|
| Pico raw amostrado limpo |99 nas quatro configurações|fixture, não processo LAB|
| Pico de referências correspondentes próprias |33;34 com evento Stop do fixture|ledger explícito, sem cobrar aliases duas vezes|
| Binding / após retorno live |26 /29 correspondentes|intent+imagem+processo persistem; três refs live já fechadas|
| Sem C1 no ramo legado do teste |32 após live|mesma semântica anterior; não usar raw entre processos como causalidade|
| PSS do coletor |+1 captura,+1 thread; volta ao precedente|amostrado; picos entre amostras UNKNOWN|
| Concorrência durante imagem copiada |10 concorrentes por configuração;40 total;zero matches do objeto stdout privado|comparação por objeto, não por número de handle|
| Primeiro GetExtendedTcpTable IPv4 |+1 em cada limpo; consultas seguintes delta0|real nesse fixture; lifetime interno não dispensável por inferência|
| CreateProcess do trabalhador |+4 no retorno;2 refs retornadas explicitamente|+2 residuais do fixture; NÃO atribuir ao OpenProcess/Start via SCM real|

Após cleanup do fixture, zero referências NOMINAIS próprias não significa zero handle interno: raw54→65 permanece no limpo Release. Nenhum leak, extinção física universal ou refund é inferido. Os casos UNKNOWN terminam por contenção isolada com saída2; isso NÃO comprova CloseHandle, quiescência do serviço ou release Model E. Recursos/arquivos sintéticos não são apagados ou sobrescritos. Nenhum processo foi morto pelo executor.

## Coexistência e projeções condicionais

O read-only118/pico120/admin1005 é histórico, sem autorização reutilizável ou orçamento futuro. Para entender somente os owners conhecidos: base positiva poderia ser117 pelo self-token; pré-Start+2=119; primeiro processo+1=120 e PSS amostrado+2=122; dentro do live+3+thread1+primeira consulta TCP1=125; após C1 retornam120+TCP1=121, cujo PSS amostrado seria123. Essas são PROJEÇÕES condicionais, não observação LAB, máximo, nova quota ou crédito. A thread transitória live e o PSS seguinte NÃO coexistem; fechar C1 não reduz o pico anterior dentro do live. Não adicionar o processo de serviço à tabela do qualificador ou somar seus handles com PowerShell/auxiliar fora do orçamento correto.

Custos ainda não demonstrados: rebind/SCM-RPC positivo atual; contexto e token reais do serviço virtual; endpoints/ACL/módulos do serviço; inicializações/transientes fora das amostras; crescimento administrativo durante efeitos e Stop. Os 19 handles administrativos históricos não são uma reserva garantida. Não inferir custo zero de substituições, nem transferir a diferença de teto para outro orçamento.

B4.C/D aceitos estabelecem que o pico real do primeiro diagnóstico é SAÍDA futura, sem exigir prova circular de um serviço ainda não iniciado. Não há teto universal novo nem waiver de cap controlado: orçamento fresco dos participantes já vivos, compatibilidade de reservas retidas, headroom5/9GiB, freshness60s/1s e limites128/1024 continuam gates antes de efeitos. H0–H9 não dispensa UNKNOWN controlado. Somente a operação positiva exata futura, separadamente aprovada, poderá medir as fronteiras remanescentes; outra observação read-only equivalente não as resolve.

## Builds, testes e hashes

Quatro builds PRODUTO Selected/Legacy Debug/Release e quatro builds FIXTURE finais MSVCx64/W4/WX: PASS, zero warnings. Positiva OFF conferida nos quatro caches. Nenhum executável operacional foi invocado. [68 resultados nativos isolados](final-native-composition-results.json) e [60 regressões](regression-results.json) PASS. Casos: limpo, concorrência, AccessCheck negado, aquisição falha, close incerto, saturação, Stop, stale/replay, token divergente, processo vivo/terminado, self-close incerto, legacy preservado e cada um dos três closes novos incertos. Tentativa direta de repetir o helper após freeze não fecha novamente. Falhas freshness/replay são negativas modeladas; não são admissão factual. Reutilizadas as provas anteriores válidas de autenticação recíproca, peer errado/impersonation, pipe loss e 28 casos de console/forwarding: seu código não mudou. Não repetir campanhas equivalentes sem mudança não reduz essas obrigações.

Histórico de correções do fixture preservado: declaração antecipada ausente gerou C4013 na primeira compilação e foi corrigida; o verificador inicialmente classificou legacy-live como saída2, mas a execução correta retornou0. A expectativa foi corrigida e a campanha final completa passou. Não eram falhas do produto e não foram convertidas em prova operacional.

Selected Release qualificador NOVO, staged: `f32d304692bdbba59300cbe6f79f8b0f796404021aceb8c205367afe78cb7f39`.
Native DLL NOVA compilação, staged: `0228825ae364011ce58d3ad2ea99f3bb1e76d7ed6a936688e599e7a3772e9268`.
Launcher preservado: `b65571abc3b5335b79f60ab0cca055d08c71dff749b1b947c5dcf55ca03cd6b3`.
Demais fontes/artefatos e quatro configurações: [pins derivados NÃO ativados](final-derived-pins.NOT-ACTIVATED.json). O launcher continua apontando para os artefatos read-only históricos; não foi repinado para execução por inferência. Não há novo registro executável de aprovação.

## Matriz ED11 e primeiro gate pendente

E02/E10/E11 incorporam o ledger e as medições limitadas; E14 incorpora C1/UNKNOWN. E15 tem implementação/helper offline melhor qualificado, mas efeitos ainda NOT_RUN. E16–E18 (diagnóstico/serviço/Stop/quiescência) continuam NOT_RUN; E20 composição Browser/C2/C3 continua fora da prova deste LAB. ED11 total e Phase6 permanecem PARTIAL. Nenhum accepted/número de gate foi substituído por um PASS global.

Primeiro gate: decisão explícita sobre T10–T14, por conflito real com o escopo de T0/T8. Depois: concluir entrada positiva específica nos mesmos artefatos/owner, negativos offline e novo pedido positivo. Próxima execução útil: UMA tentativa diagnóstica positiva manual exata03, com admissão fresca antes de consumo e no máximo um Stop quando aceito; não outra observação idêntica. A aceitação da proposta não autoriza essa execução. Não apresentar comando executável antes de hashes/registro aprovados.

TECHNICAL_CORRECTIONS=OFFLINE_QUALIFIED
POSITIVE_COMPOSITION=ISOLATED_QUALIFIED_WITH_EXPLICIT_SUBSTITUTIONS
POSITIVE_OPERATIONAL_CAPACITY=NOT_DEMONSTRATED
DEFAULT_PROFILE=EMPTY_DENY
POSITIVE_ENTRY=OFF
NORMATIVE_EXTENSION=PROPOSED
OPERATIONAL_APPROVAL=ABSENT
INDEPENDENT_BUDGET=NOT_GRANTED
PREPARE_EXECUTED_BY_CODEX=NO
START_EXECUTED=NO
STOP_SERVICE_EXECUTED=NO
INTENT_OPERATIONALLY_CONSUMED=NO
SCM_MUTATIONS=NONE
EXISTING_ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=OWNER_NORMATIVE_T10_T14_DECISION_THEN_EXACT_POSITIVE_INTEGRATION_AND_MANUAL_APPROVAL
