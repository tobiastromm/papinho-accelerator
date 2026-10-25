# PRE_METRIC_REVALIDATION — correção de redundância e qualificação

TASK=PACC-PHASE6-ED11-PRE-METRIC-TIMEOUT-RESOLUTION. main/HEAD `913c90cf2328f7939d8f5b927640601903ac6e91`. Trabalho técnico concluído no escopo offline. Nenhum Prepare/Cycle operacional, Start/Stop, consumo, SCM/ACL, input protegido, imagem ou sink foi executado/criado nesta tarefa.

## Diagnóstico factual e limites

A execução manual anterior falhou1460 em PRE_METRIC_REVALIDATION/bd_census_predicate/process_snapshot; intent/effect/diagnostic/starts/stops0, close0 e transporte local fechado/retained0. O PID6380 era a entrada do snapshot, não prova de quinto papel. A aprovação anterior foi utilizada, preservada e não pode ser reutilizada. Sem métricas finais nesta saída. Quiescência global não comprovada; UNKNOWN históricos não convertidos em zero.

Cadeia real: `ig_project_census` → current inicial → inventory → snapshot → PARA CADA entrada `ps_bd_census` → inventory → console → `psa_bd_aux_current` → `ig_n0_current`. Antes das métricas, este último refazia conteúdo/configuração/imagens/dependências/dossiê/proveniência/políticas/aprovação, além de identidade/token/RAW1/SCM. Fonte prova N+2 validações completas de conteúdo para N entradas, após B1. Há múltiplos censos entre binding e preflight: inicial, PRE_METRIC e readbacks posteriores; a composição antiga repetia o trabalho em cada um.

Os relógios reais são `ig_n0.began`, `validated_at`, proof.console.observed, gate.preflight e sample.oldest; não há valores históricos suficientes para reconstruir duração de cada hash/SCM/RAW1. B+D e a confirmação positiva também ocorrem dentro de lifetimes já iniciados; espera do operador conta. `ig_n0_current` tem retorno1460 por content freshness, e por idade global depois de completar validação. Outros erros são propagados. O estágio/api do recibo não identifica qual comparação ou API interna produziu o1460. Portanto NÃO se declara comprovado que foi exatamente a janela60s, nem que a correção garante eliminação de qualquer timeout futuro. A causa demonstrada corrigida é a repetição de hashing estável, não uma duração histórica inventada.

## Menor correção selecionada

A proposta B2 anterior de retirar inventário por entrada continua REJEITADA: seus endpoints perdiam alteração transitória e a ordem da primeira falha. O corpo do censo permanece byte-equivalente, salvo o nome `_body`; ele conserva TODOS os callbacks por entrada, inclusive token, liveness, sessão, contexto do console, RAW1/ancestrais, identidade, SCM/recovery/estado, plano, Stop e membership. Callers diretos não são removidos.

A nova composição afeta SOMENTE o ramo positivo N0 pré-métricas. O wrapper faz uma validação completa, empresta a observação de CONTEÚDO durante a execução síncrona do mesmo censo e faz validação completa novamente ao final limpo. Durante esse intervalo, `ig_n0_current` continua todas as verificações voláteis e compartilha apenas hashes de bytes nos MESMOS originais retidos, adquiridos por leitura com FILE_SHARE_READ, sem share-write/delete. Nenhum handle é fechado/reaberto ou transferido, nenhum novo owner/cache global/registry. Os callbacks que consultam esse borrow executam no mesmo thread original e descriptor/challenge/began/validated devem coincidir; qualquer expiração/alteração invalida sem fallback para reutilização. Reentrância do censo DENY. Falha/Stop/close incerto limpa o ponteiro de borrow antes de retornar, sem novas aquisições ou liberação das dependências incertas. A1 e seus dois slots permanecem intocados.

O ponteiro é global somente para resolver a callback síncrona do owner existente; aponta para40bytes de stack local e nunca representa autoridade persistente. Fora do wrapper é NULL. Não sobrevive a retorno, falha, snapshot ou geração posterior. A cada censo novo exige nova validação integral; não é uma nova admissão ou retry de freshness. `validated_at` NÃO é atualizado quando conteúdo é compartilhado; `began`, proof.observed, preflight e amostra1s não são resetados. O relógio global60s cobre inclusive o custo da validação completa final. Metric_window e read-only/legados continuam no caminho anterior.

Para N entradas, o corpo mantém N+2 callbacks voláteis; o wrapper acrescenta duas fronteiras, total N+4 callbacks e apenas DUAS rodadas integrais de hashing. Isto não reduz handles estáveis nem dispensa os checks da última linha/censo. Mutações de segurança reversíveis continuam observadas nos mesmos pontos. Writes/deletes/mapeamentos graváveis são excluídos pelo lifetime das aberturas read/share-read, no escopo de OS confiável/operador autorizado já aceito. Não se alega resistência a kernel comprometido, acesso bruto privilegiado ou destruição arbitrária do processo. A documentação oficial descreve os conflitos de abertura e mapeamento: [CreateFileW](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew). A proteção é dos objetos originais retidos, não de um nome textual que será reaberto.

## Evidências novas

Quatro builds Selected/Legacy Debug/Release, /W4 /WX, PASS zero warnings. Mesma toolchain MSVCx64/Ninja/dependências e quotas. [Resultados](build-results.json). A primeira compilação encontrou ordem de declaração do novo wrapper; foi corrigida posicionando-o após o contexto. Outro build tentou relinkar DLL de transporte inalterada e recebeu LNK1104 por uso do arquivo; nenhum processo foi fechado/morto. O target inalterado foi retirado da campanha necessária, mantendo sua DLL existente/pin; os quatro builds finais afetados passaram. Logs iniciais preservados. Não se trocou backend para contornar falha.

[76 regressões/modelos atuais](regression-results.json) PASS (72 anteriores +4 execuções do novo fixture canônico). Incluem A1, B1, self-token, Stop, retenção, replay, RAW1, default/opt-in pins e60s/1s. Novo teste inclui o header e wrapper REAIS e simula callbacks/APIs:441 verificações em cada configuração, incluindo32–1024 entradas, fronteiras60000/60001, relógio regressivo, descriptor/challenge/thread/observação alterados, expiração antes/durante/depois, falha de leitura, Stop, reentrância, fechamento incerto e alterações voláteis transitórias. Não é prova de token/SCM/ACL LAB reais; seis variantes de falha volátil usam o mesmo predicate simulado. A preservação dos callers reais é adicionalmente conferida pela comparação do corpo do censo e contagem de chamadas de segurança em [source-equivalence-review](source-equivalence-review.json).

[27 gates puros](launcher-gates.log) PASS, e Review ESTÁTICO de ambos os perfis PASS; sem entrada nativa. Quatro leituras da função de aprovação REAL atual extraída com APIs simuladas passaram16casos cada, inclusive novo endereço exato, record type e erro nativo original. Uma execução inicial apontou o harness antigo; foi detectada na revisão. O harness novo inicialmente rejeitou o endereço novo porque ainda esperava o antigo; expectativa atualizada para exigir o novo endereço, seguida de quatro builds/runs finais. Resultados finais não se baseiam no harness antigo. Transporte/forwarding/console/recibos não mudaram; suas48provas PowerShell e autenticação adversarial anteriores continuam aplicáveis por fonte, sem repetir campanha equivalente.

Native isolated: um mapeamento gravável preexistente bloqueou a abertura original read/share-read com erro32. Durante outro lifetime read/share-read, novas aberturas WRITE/DELETE/READ_WRITE foram negadas32; depois de fechar o original, WRITE passou. Sem setter ACL/objeto protegido. Contagem do coletor Python107→108→107 nesse ensaio, não do qualificador LAB; picos não amostrados/transitivos permanecem UNKNOWN.

Dezoito amostras temporais equivalentes (32/256/1024 entradas,3 repetições,2 métodos) com leitura/hash real de arquivo sintético e identidade por callback. Para1024 entradas:1026hashes antes,2depois; medianas1692.967ms e8.307ms. São medidas do callback substituto, sem RAW1/SCM/censo LAB completo; não são previsão ou garantia de duração positiva. [Dados brutos](native-sharing-and-time-results.json). O fixture é execução via runner Python/ctypes stdin registrado na sessão; os JSON preservam os resultados, não se apresenta esse modelo como processo nativo de admissão.

## Instrumentação e recursos

Opt-in `PAPACC_N0_CENSUS_TIMING`, ON nos builds candidatos: uma linha agregada por wrapper terminado (calls, fronteiras completas realmente tentadas, elapsed e idade global) e linha no retorno explícito de timeout de contexto (point, now/began/validated/window/scope). Usa stdout privado existente, não cria recorder/logger/handle, não altera retorno/limite/admissão e não tenta compensar tempo de logging. A linha de erro ocorre APÓS a decisão de negar; overhead de formatação/transporte não é zero. Falha na primeira fronteira pode não produzir o resumo, mas o ponto temporal explícito é emitido quando alcançado.

Storage compilado:40bytes de escopo emprestado, ponteiro persistente8bytes, nenhum HANDLE adicional. Prologues do wrapper no objeto real reservam128bytes Debug e96Release e salvam8bytes RDI; esses números não são total de stack nem somados duas vezes aos40bytes já contidos no frame. Chamadas aninhadas/CRT/transporte e custos Windows continuam separados/UNKNOWN. [Disassembly](compiled-frame-inspection.json). Não se concede crédito nem economia de handles. Nativos128/admin1024/quatro papéis/60s/1s/headroom/stack/memória e NONE_GRANTED preservados. H/P3/N7/RAW1/T0–T14 não foram modificados. A correção é uma composição interna de observações estáveis, mantendo todos os pontos voláteis, sem nova exceção normativa accepted.

## Pins, preservação e próximo gate

Launcher `10fd6add1c4cd8cdb28845462a219c403b62271b4d5f894ee355279e293b150c`.
Qualificador Selected Release `724fd4c7a2554e362115c2fbb85744b8bcff27ca14882382201d024bffe8030d`.
DLL de transporte `c351ecb559834d74ebe36dd735b6174b4ca6226526ae928b7a2cc5f44c4d792c`; binding `513107355c46b8282ac17eb9763a1278bd201c0d4f9f9a925950676fba14d9f4`. Binário de SERVIÇO `44d8597469bdd4f40b317f467029ee96b3ad69fdc270bcdbf5e126788f623e5c` não mudou. [Pins completos](final-artifact-pins.json). Os quatro qualificadores/DLLs anteriores foram arquivados byte-exatamente antes do rebuild. Registros aprovados anteriores/pedidos/evidências/inputs protegidos não foram sobrescritos; source/WIP possuem preimagens. HEAD permanece igual.

Novo pedido específico ainda NÃO aprovado SHA256 `64f20a0fcbe94782b3178c9da27ce0c76a989e478802d7e2262d3cef04d0e18b`: [pedido integral](POSITIVE-OPERATION-REQUEST.DRAFT.json). A nova compilação e o launcher exigem outro endereço `EXACT-POSITIVE-TIMEOUT-CORRECTION-APPROVED.json`, que NÃO existe. Candidato tem status deliberadamente não executável; não foi registrado owner approval em nome do proprietário. Mesmo escopo normativo03/identidade ainda não consumida, sem GUID/rearm ou reutilização de claims. Nenhuma execução automática.

O owner deve revisar e aprovar especificamente a futura única execução manual com esses pins e efeitos condicionados. Só então registrar aprovação original e hash literal do comando. Freshness inclui espera humana, sem renovação artificial. Se falhar novamente, não repetir. Não exigir outro read-only equivalente: a observação discriminante é a própria única operação diagnóstica autorizada, com gates frescos antes do consumo, e trace temporal caso negue. [Procedimento](MANUAL-PROCEDURE.NOT-AUTHORIZED.md).

ED11 E13 incorpora correção fonte/offline e trace, sem PASS operacional. E15 consumo/colocação/rebind03 NOT_RUN; E16–E18 serviço/NTSTATUS/Stop/quiescência NOT_RUN. Demais componentes Phase6 continuam fora do alcance. Eliminar trabalho redundante não demonstra capacidade positiva nem fecha ED11.

CURRENT_TASK_COMPLETED=YES
REDUNDANT_CONTENT_HASHING=CORRECTED_OFFLINE_QUALIFIED
HISTORICAL_EXACT_TIMEOUT_BRANCH=UNPROVEN
VOLATILE_REVALIDATION=PER_ENTRY_PRESERVED
GLOBAL_SIXTY_SECOND_GATE=UNCHANGED
FINAL_ONE_SECOND_GATE=UNCHANGED
NEW_OPERATIONAL_APPROVAL=ABSENT
START_EXECUTED=NO
STOP_SERVICE_EXECUTED=NO
OPERATIONAL_INTENT_CONSUMED=NO
SCM_MUTATIONS=NONE
EXISTING_ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
FIRST_UNRESOLVED_DIVERGENCE=NEW_EXACT_OPERATIONAL_APPROVAL_ABSENT_THEN_FRESH_NATIVE_GATES
NEXT=OWNER_APPROVES_CORRECTED_EXACT_MANUAL_N0START03_OPERATION
