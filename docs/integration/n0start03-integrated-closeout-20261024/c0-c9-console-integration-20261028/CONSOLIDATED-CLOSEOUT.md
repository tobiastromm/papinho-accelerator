# C0–C9 — aceitação e integração definitiva do binding de console

TASK=PACC-PHASE6-ED11-C0C9-ACCEPTANCE-AND-CONSOLE-INTEGRATION
Data2026-10-28; projeto canônico/main/HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservados. Trabalho técnico concluído; nenhuma execução TPM autorizada ou realizada.

## Decisão normativa registrada antes do código

Proposta C0–C9 conferida integralmente, SHA2567dee709121cc9b56633efb6d968bd0935b81bc6c18b000226ed35cf0a179f985. [Registro ACCEPTED](../../../adr/DECISION-tpm-proof-explicit-console-host-binding-20261028.md) SHA256 `d54cfe4ed933e13c5b8ff952ec02e8d2dfe1af8c06066e64e41b3dfe51a879cd`. Fonte original PROPOSED byte-exata preservada e incorporada por referência; nenhum ADR accepted anterior editado. [Verificação](NORMATIVE-VERIFICATION.json).

A aceitação é apenas do binding/risco estreito para o experimento matemático standalone. Não transfere a N7 histórica para outra tentativa, não enfraquece R3 do LAB, não autentica geração física e não concede orçamento. B+D/N5/N7/RAW1/H/Q/R/T/W/L/L1 seguem aplicáveis em seus escopos. O risco de associação criptográfica completa e imagem mapeada UNPROVEN é aceito por C4 somente neste experimento; identidade/token/arquivo/proteção/lifetime/Stop/UNKNOWN não são dispensados.

## Implementação efetiva no mesmo host C

[Plano/revisão inicial](SOURCE-REVIEW-AND-PLAN.md), [patch relativo ao WIP anterior](SCOPED-WIP-RELATIVE.patch), [ownership](OWNERSHIP-REVIEW.csv), [cobertura](TEST-COVERAGE.csv).

hp_admin agora usa o pai REAL obtido do próprio processo, lista EXATA dos dois clientes do console herdado e class49 de self e pai. Exige status/width8/profile forward flag1/mascara alinhada não nula, mesma associação repetida antes/depois das métricas e auxiliares distintos de pai/self. HWND é apenas metadado; pode pertencer ao pai ou ao próprio cliente. Não se cria auxiliar artificial nem se substitui por nome conhost, PID indicado em argv ou janela.

O auxiliar é aberto separadamente com handle original. GetProcessId, criação não nula, Wait0 vivo e continuidade são conferidos antes e depois; criação do pai posterior ao filho nega PID reutilizado. Tokens primários de pai/auxiliar exigem SID autorizado, elevado, tipo/session corretos, AuthenticationId compatível com o nativo, estatísticas estáveis antes/depois e TokenId/AuthId/ModifiedId fixados por papel para revalidações futuras. O próprio hp_token continua verificando ausência de impersonation e token efetivo antes de cada boundary. Não afirmamos ausência de impersonation em todas as threads de outros processos.

Proveniência do arquivo não depende só de caminho/hash: QueryFullProcessImageName do handle, caminho System32 esperado, arquivo original não-reparse/disk, volume/FileID/tamanho, SHA256 completo e OWNER/GROUP/DACL exatos. PowerShell, conhost e ntdll ABI são verificados por originais; Windows update/substituição/descritor/ABI divergente nega, sem atualização automática de pins. [Pins lidos readonly](ROLE-FILE-PINS.json). Nenhum WinTrust/CRL provider foi acrescentado ao processo. Hash de arquivo não é imagem mapeada, conforme C4.

Pai e auxiliar mantêm dois handles temporários simultâneos. Um slot de arquivo original e o slot de token são usados serialmente, com reserva antes da aquisição e fechamento conhecido antes de reutilizar. O work_hash existente é reutilizado sem sobreposição; erro de close/hash retém o original e dependências. Owner-thread e BUSY abrangem todo o guard; outra thread não modifica estado, reentry congela. Nenhuma aquisição/novo comando depois de freeze; primeiro erro e etapa específica de token/arquivo são preservados. O fechamento incerto não é retried nem convertido em CLOSED; host real retém processo vivo.

A soma usa PAI+auxiliar REAL uma vez por objeto, e o nativo fica no orçamento próprio. O censo é repetido antes de sucesso; console externo, mudança de PID/criação/contexto, terceiro cliente ou auxiliar ausente negam. Não há seed antigo17204,20976 ou13944 compilado como participante futuro. Os três papéis da futura invocação são resolvidos live e apresentados para confirmação, que agora inclui request SHA + PID/criação do pai + PID/criação do auxiliar. Toda linha futura será específica, nunca copiada da execução anterior.

Contrato DATA agora exige console_binding_revision=C0-C9 e o digest exato da proposta aceita. Sem argumentos default DENY; opção CMake continua OFF por default. Nenhum launcher PowerShell, produto, serviço, owner, mutex ou IPC genérico adicionado. Host moderno MSVC/x64; nenhum fallback NT4/VC6/NSS/OpenSSL.

## Qualificação final

Debug/Release do host e cinco fixtures, /W4 /WX /GS /sdl, zero warnings. Nove CTests por configuração,18/18 PASS: artefatos/faults, novo grafo administrativo nativo, arquivos Windows readonly, metadata, console/full mock, protocolo, guards, selfcheck sem TPM e default DENY. [Debug](Debug-FINAL-ctest.log), [Release](Release-FINAL-ctest.log). Source/build afetados foram reconstruídos; não repetimos campanhas de produto/Browser inalteradas.

Por configuração:101assertions de plataforma/guards incluindo reentry e thread real,105 ao acrescentar quatro verificações de metadata,137 ao acrescentar arquivos Windows e seis fault modes. São extensões de um conjunto compartilhado, não343assertions distintas.155assertions de artefatos/persistência/once/novo policy digest,4375 core/vetores históricos reutilizados no processo integrado. Protocolo core e bytes dos vetores não alterados.

Os cenários cobrem HWND==pai/self como METADADO válido com auxiliar real distinto, auxiliar ausente/divergente, width/flags/status incorretos, mudança de clientes/associação, liveness/criação/PID, tokens de papel incorretos/não elevados/tipo/session/query/continuidade, SID, double-close, falhas parciais, reserva/BUSY/Stop, freeze/retenção. Role-file testes REALMENTE leem os três originais Windows: IDs/hash/descritor protegidos; falhas simuladas de security query/descriptor/ID/shortread/hash/close não alteram esses arquivos.

[Grafos nativos](NATIVE-ADMIN-GRAPH-RESULTS.json) executam o corpo de produção hp_admin em console clássico e CreatePseudoConsole reais, com pai e filho próprios no mesmo console, auxiliar Windows real e dois grafos simultâneos. Procesos finitos encerram normalmente e o hp_alive de produção rejeita um original encerrado; refs controladas são fechadas com retorno verificado. NÃO é processo artificial para obter PASS na futura operação; são fixtures isolados autorizados. Autoridade dos tokens e arquivo do pai (que é fixture unsigned, não PowerShell) é explicitamente modelada; ABI e arquivo real do auxiliar, kernel/PID/criação/class49/censo/contagens/fechamento não são substituídos. Esses resultados não são admissão operacional ou autenticação da associação.

O ensaio completo executa a entrada C original e o guard C0 integrado, console e confirmação reais e endpoint TPM de SOFTWARE. Pai-file usa surrogate ntdll com o mesmo ownership/hash/descriptor/overlap (arquivo maior que PowerShell), não prova o arquivo do pai-test. Tokens são abertos/sampled com LUIDs reais, mas a autoridade/elevated/SID dos atores de teste é modelada. Arquivo do auxiliar Windows e ABI são verificados pelo helper real. Por isso file mask6 do fixture é declarada parcial (não7); teste separado valida os três arquivos Windows reais. Não se declara prova TPM a partir desses resultados.

Caminho limpo, confirmação errada, Escape e máximo8candidatos/12Submits PASS. O máximo inclui boundary equivalente ao módulo e16observações administrativas, exercitando o limite existente sem ampliar arrays. [Saídas integrais](Release-maximum-FINAL.txt), [resultados](RESOURCE-RESULTS.json), [ledger](PHASE-LEDGER.csv). Falhas de fechamento mantêm referências originais de teste em storage persistente do fixture até perda do processo; perda não é fechamento operacional provado.

Na primeira adaptação, o controlador de teste ainda tinha buffers100 para a nova confirmação mais longa. Corrigido para160/192 e check de capacidade. Um filho sintético bloqueado foi cancelado com Escape/Enter após conferir clientes privados; terminou CONFIRMATION_DENY, mock open/submit/close0 e refs0/retainedfalse, sem kill. A falha inicial e saídas estão preservadas no log Release-initial-ctest e no diretório de fixture original; não houve TBS real. Um conflito de macro wmain do novo fixture foi corrigido com entry nomeada; warnings nunca foram desativados.

## Recursos, layout e alcance da prova

Preobserver124, próprio128, administração1024, sem crédito ou transferência. O par982 e o pico79 anteriores permanecem observações históricas, não base futura. Novo binding mede pai/auxiliar atuais antes de qualquer TBS; se faltar folga, DENY.

Nos ensaios completos SOFTWARE: pico amostrado92 em Debug e Release; caso limpo130samples/9observações administrativas/5Submits, caso máximo214samples/16observações/12Submits. O storage256samples e16admin samples permaneceu igual. Há capacidade para os checkpoints do caminho máximo testado; não é teto universal nem demonstra capacidade do TBS/RPC real. O módulo/contexto TBS, cache/kernel/transitivos e intervalos não amostrados seguem UNKNOWN. Control refs finais0/retainedfalse nos caminhos conhecidos, sem alegação de quiescência global.

Owner core TP_OWNER28688bytes inalterado. TP_PLATFORM166816bytes, acréscimo96 vs166720 anterior; RUNTIME166864. Inclui novo handle temporário, BUSY e continuidade/metadados; não é custo total de memória. [Frames compilados](COMPILED-STACK-FRAMES.json): hp_admin32, hp_admin_current216, hp_role_token240, hp_role_file8544 e wmain352bytes de subtração RSP Release; excluem pushes/callees/runtime/reserva e custos transitive, não somar como teto. Buffers são fixos, hash streaming sem copiar imagem inteira.

O budget observado dos grafos mede atores de teste, não PowerShell real durante a prova. O pareamento AuthenticationId dos dois processos reais20976/13944 também foi consultado readonly e compatível ([metadados](ROLE-AUTHID-READONLY.json)); não substitui a verificação do nativo futuro/token efetivo/freshness.

## Pins e pedido específico pronto

[Todos os fontes e binários](TECHNICAL-PINS.json), [candidato manual](MANUAL-CANDIDATE-PINS.json), [procedimento condicionado](MANUAL-CANDIDATE-HANDOFF.md).

Executável novo Selected Release SHA256 `59c67f6d81d88c49c1d676bde2553aa8a843a1925c76a33deb14acbab3424898`.
Pedido novo SHA256 `b575659fe446eb4b2ed3d53881dfbef77855a02a78d87fb76cccc96f3faf0c8b`.
Registro normativo SHA256 `d54cfe4ed933e13c5b8ff952ec02e8d2dfe1af8c06066e64e41b3dfe51a879cd`.

O pedido19651bytes/549nodes foi validado pelo parser e contrato C REAIS em modo DATA: [resultado](REQUEST-DATA-CHECK.log), sem executar TPM. Fonte host_admin.inc, fixtures/CMake/dependências/policy/role-file pins estão no conjunto derivado. PE x64 sem CLR e sem TBS estático; loader TBS real continua tardio, só depois da aprovação exata/confirm/receipt. [PE/imports](PE-IMPORTS.txt).

Não existe aprovação operacional nova, nem hash de aprovação inventado ou comando de prova ativado. A solicitação é UMA invocação manual futura, com gates frescos, até12Submits e zero NV/AK/hierarchy/Clear/Q4/Start/Stop/SCM/ACL. A aprovação4a5f foi usada; sua ausência de receipt não permite retry. SHA59c67f6d não reutiliza o executável/hash antigo. O marker local é suplemento administrativo, NÃO ledger Q4 resistente a rollback.

Próximo gate REAL: proprietário aprova especificamente o novo pedido `b575659fe446eb4b2ed3d53881dfbef77855a02a78d87fb76cccc96f3faf0c8b`. Depois de aprovado, registrar UMA decisão exata, calcular seu SHA literal e entregar comando definitivo ao proprietário. Sem execução automática. Não é necessário pedir mais uma coleta equivalente de topologia ou aceitar outra revisão normativa para esta integração qualificada.

## Continuação E17–E22 e preservação

[Históricos e sete fontes independentes reutilizadas](HISTORY-AND-PARALLEL-REUSE.json) conferem, sem regressão que exija rebuild de produto. E17 precisa futura identidade/acesso efetivo/readiness do serviço elegível; E18 precisa lifecycle/Stop/quiescência factuais, não encerramento histórico. Não criamos identidade/intent ou reutilizamos Q4.

E19/E20: D2 preparação-only aceita; D4/D5 age/validity/duration/headroom e bindings root/CURRENT/epoch/SiteTLS/backend/destino ainda pendentes, provider/kernel ativos UNKNOWN. Reserva Browser1858020/2097152 não vira margem para TLS/kernel. Essas frentes não dependem da próxima prova TPM e não foram reimplementadas/testadas sem hipótese nova. [Caminho crítico](CRITICAL-PATH.json), [dados reutilizados](PARALLEL-E19-E20-REUSE.json).

E21 avança em norma/binding offline e topologia diagnóstica; posse TPM, geração, revogação/custódia/NV/ledger factual/readback/disposição Q4 continuam ausentes. E22 derivado permanece pendente.4demonstrados/12limitados/6pendentes. ED11 e demais pendências Phase6 separados, PARTIAL.

HEAD/WIP/históricos/pins operacionais anteriores/UNKNOWN/reservas preservados; sem commits/worktrees/clone/limpeza/refund/CURRENT/assinatura/recuperação. Somente pins PROSPECTIVOS do novo pacote foram gerados. Nenhum TPM/TBS real, Q4, Start/Stop ou mutação SCM/ACL realizada.

CURRENT_TASK_COMPLETED=YES_TECHNICAL_SCOPE
C0_C9=ACCEPTED
CONSOLE_BINDING=INTEGRATED_QUALIFIED_OFFLINE_SCOPED_NATIVE_GRAPH_TESTS
DEFAULT_ENTRY=DENY
BUILDS=DEBUG_RELEASE_PASS_ZERO_WARNINGS
TESTS=18_CTEST_PASS
ACTUAL_TPM_PROOF=NOT_EXECUTED
NEXT_REQUEST=PREPARED_NOT_APPROVED
E21=PENDING
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
CURRENT_SIGNATURE_AUTHORITATIVE_RECOVERY=DENY
NEXT=OWNER_EXACT_C0C9_NATIVE_PROOF_APPROVAL
