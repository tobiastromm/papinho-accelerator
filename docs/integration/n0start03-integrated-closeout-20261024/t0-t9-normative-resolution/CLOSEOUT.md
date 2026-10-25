# T0–T9 — revisão normativa focal e integração pronta para implementação

2026-10-25. TASK=PACC-PHASE6-ED11-T0-T9-NORMATIVE-RESOLUTION.
main / HEAD913c90cf2328f7939d8f5b927640601903ac6e91 preservados. Revisão1 conferida integralmente e byte-exata:045b3edd03cb5841036eb047a62f54adee9763055429cfac6fc062d131257395. Original preservado em preimage, junto com N5/B+D, launcher e código do protótipo anteriores a esta tarefa.

## Decisão preparada

Recomendar a emenda EXATA contida em T0 da proposta T0–T9 revisão2. Ela permite somente o transporte stdout privado, efêmero e local entre o PowerShell elevado original e seu qualificador nativo original para N0START-20261024-03/tentativa0b1de869e11493b7cb08fb329578f6ce, no tuple host/boot/serviço normativo vigente. Não permite IPC genérico, comandos de controle, daemon, novo owner, comunicação remota, outros consumidores ou quinto papel.

**A revisão é técnica/normativa por este agente; não é aprovação do owner nem segunda revisão externa inventada.** Não há autoridade normativa válida para ACCEPTED nesta mensagem do owner, que expressamente reservou essa decisão. Proposta permanece PROPOSED. Primeiro gate: aceite específico dos bytes da revisão2, depois registro normativo designado preservando N5 original. Não falta autorização para editar/rebuild/testes; falta a decisão durável reservada.

Proposta revisão2 SHA256: 5336728e06b1daeb87fe691e637e3ed2a64262967c27ad3c48fb518436b5d995.

## Conflito exato e intenção de N5

[N5 accepted, primeira frase](../../../adr/DECISION-first-native-lab-diagnostic-boundary-20261024.md) exige console interativo utilizável e veda, entre outras coisas, “new IPC”. O named pipe proposto é IPC novo: novo endpoint endereçável, novo bootstrap de conexão e autenticação e novos lifetimes/evento; não deixa de sê-lo porque preserva os bytes stdout e não concede autoridade. Não reinterpretar silenciosamente a proibição como se significasse apenas controle remoto ou um segundo owner.

N5 protege: operador/console existentes antes de efeitos, autoridade canônica, consumo antes de efeito governado, Stop factual, distinção Stop solicitado/aceito/concluído/quiescência/close, supressão de efeitos ao invalidar contexto, retenção UNKNOWN sem kill/retry/rearm/refund. Tudo isso é preservado.

A emenda substitui SOMENTE a primeira frase de N5 no escopo03; a segunda frase e os demais parágrafos ficam integralmente controladores. Ela mantém a vedação a topologia detached/unattended, replacement actor e background launcher; veda todo IPC fora do único transporte delimitado. B+D R0–R7, N0–N4/N6–N9, RAW1, N7, L/H/P3, Model E, ED08/ED11 permanecem. Não altera fontes accepted.

A aquisição preparatória de endpoint/evento é explicitamente delimitada, para não criar uma circularidade: ocorre após verificação do launcher e aprovação exata futura da observação, antes de criar/bindar o owner nativo do filho, como o antigo pipe stdout. Isso não consome intent nem autoriza efeitos positivos; todos os efeitos LAB governados continuam posteriores ao consumo canônico. Bootstrap hello/ack apenas estabelece transporte; após isso o canal só conduz stdout qualificador→supervisor. Confirmação e Stop continuam no console.

## Alternativas compatíveis ou rejeitadas

| Alternativa | Avaliação sob contratos atuais |
|---|---|
| Manter launcher/anonymous pipe atual e investigar outra economia | Não exige exceção IPC, mas não resolve a herança global demonstrada nem prova128 positivos; permanece DENY. |
| HANDLE_LIST de transfers privados | Exposição inversa já comprovada; lock local não coordena outros criadores. Não repetir observação equivalente. |
| Stdout direto no console, sem pipe | Perde captura atribuída ao processo/linhas/recibos e mistura outros escritores; não preserva o contrato existente. |
| DuplicateHandle com bInherit=FALSE | Ainda exige autenticar destino e informar o valor remoto por um bootstrap; não elimina a necessidade de transporte e não resolve stdio/console sozinho. |
| Helper-processo | Muda topologia/quatro papéis e lifetimes; não é solução dentro do escopo atual. |
| Named pipe local autenticado, exceção restrita | Resolve a janela testada sem global serialization/novo ator; introduz custos e obrigações explícitos. Recomendação técnica condicionada à emenda exata. |

Não foi demonstrada alternativa que elimine esta falha, preserve todos os contratos e dispense tanto nova prova/decisão como a exceção estreita. Isso não é afirmação universal de impossibilidade; é o resultado das alternativas concretas auditadas.

## Trabalho técnico novo e correções

Somente tests/noninherited_transport_test.c foi alterado funcionalmente. Nenhum launcher, owner canônico, recorder, dossiê ou pin operacional foi modificado.

1. **Namespace e IDs antes de abertura.** Prefixo local exato, sufixo de32 hex minúsculos; rejeitar UNC remoto/aliases/sufixos/IDs malformados, sinais, whitespace, lixo e overflow. Validação roda antes de OpenProcess/CreateFile do cliente. Não se tentou conexão remota: strings remotas são casos puros da validação; casos nativos inválidos locais/IDs mostram endpoint_opens=0.
2. **Replay de bytes por outro processo.** Concorrente envia hello e bytes de sessão anterior antes do ack. O server compara o PID conectado ao processo ORIGINAL esperado e nega; accepted=0/bytes=0. O teste não pretende provar resistência contra código comprometido dentro do próprio processo autorizado.
3. **Peer morto no ponto de uso.** Filho sintético termina após hello; o teste espera sinalização do handle original antes da verificação de uso. Canal negado sem payload, sem reabrir PID ou reaproveitar identidade. Isso acrescenta prova de lifetime ao caso anterior de criação divergente; não força reutilização real de PID.
4. **Falha de ack corrigida.** O caminho anterior saía diretamente com código35. Agora write/ack falho preserva o erro e segue cleanup verificado; aceitação só é marcada após ack bem-sucedido. Fault injection fecha endpoint original antes da escrita: Win326, accepted=0, bytes=0, pending=0, parent_owned_slots=0. Não existe retry/novo endpoint.
5. **Ausência de impersonation do cliente.** Igualdade de tokens primários não prova que o thread não impersona. Guarda explícita agora antes da abertura e depois do ack. Teste usa ImpersonateSelf real no próprio processo, sem credenciais externas: thread token detectado, endpoint_opens=0, predicate DENY. O fixture reverte somente a impersonation que ELE criou e termina84; não é remediação de contexto inesperado no produto. Não é teste cross-account e não dispensa TokenId/AuthId/ModifiedId/elevation do owner na integração.

6. **Criação falha pelo caminho comum.** O caso de executável inexistente antes usava tratamento separado, enquanto a criação comum podia retornar diretamente. Ambos agora usam failed_create_cleanup: preservação do Win32 original, saídas PROCESS_INFORMATION contraditórias causam retenção e connect pendente é concluído/cancelado factualmente antes de fechar. O teste real retorna2, child_created=0/pending=0 e SHARED_FAILURE_PATH, sem uma implementação de teste que evite o código comum.

O código continua isolado e sem dispatcher Prepare/SCM/intent/Start/Stop. Nenhuma propriedade foi relaxada para fazer PASS. A mudança de cleanup corrige um caminho de falha real no protótipo; não altera o código funcional de admissão.

## Builds e provas

Selected Debug/Release PASS, /W4 /WX, zero warnings. Nova árvore build/ed11-t0t9-20261025 preserva os binários históricos anteriores. Apenas o alvo do protótipo alterado foi reconstruído; nenhuma necessidade de rebuild/reexecução operacional.

36 cenários nativos de processo,5 iterações por cenário=180 iterações, resultados esperados PASS.24 casos base foram reexecutados como regressão porque a guarda comum de todos os clientes mudou;12 casos novos (6 por configuração) cobrem ID textual, namespace local inválido, replay, peer-exit, ack-write-fail e impersonation real.40 verificações puras adicionais em Debug/Release validam namespace e números, com zero aberturas de endpoint. Não são40 cenários de processo.

Os62 testes de owner/observer anteriormente qualificados NÃO foram repetidos sem hipótese: suas fontes e callers funcionais não mudaram. São evidência histórica preservada, não execuções novas desta tarefa. Concorrentes não recebem objetos privados nos casos aplicáveis; guardas de namespace não tentam rede; nenhum teste lança o serviço ou qualificador operacional.

JSON/logs: native-results.json, guard-results.json. Ledger: new-resource-ledger.csv. Nos casos novos rejeitados, accepted=0, bytes=0, pending=0 e slots próprios do supervisor0. accepted aqui é somente canal, não admissão operacional; nenhuma amostra é orçamento fresco ou demonstração de capacidade positiva.

## Recursos, ownership e riscos preservados

Estado NT_OWNER144 bytes; nova contagem escalar de aberturas tem4 bytes. Não é memória total: código/rdata, buffers/frames, FILE/CRT, segurança, BCrypt, PSS e kernel têm custos separados e entre-amostras UNKNOWN. Não se concedeu crédito por esses números. O novo teste de impersonation também adquire referência própria temporária do token de thread e a fecha/reverte no seu escopo.

Amostras clean mantêm o padrão anterior: filho52→54→53 na entrada/autenticação/canal estável; supervisor warm-up52→80→79, depois79→82→80 e80→82→80. Não somar tabelas de processos diferentes, nem declarar pico universal ou leak por aquecimento. Recurso/evento novo precisa caber no1024 administrativo; qualificador continua128. O histórico126/128 e economia12 do outro fixture não são capacidade atual.60s/1s, quatro papéis, headroom, HOST_CREDITS=NONE_GRANTED, UNKNOWN e reservas reais retidas permanecem.

Riscos/obrigações adicionais: cliente errado pode causar DoS fail-closed sem reconnect; nome não é segredo/autoridade; Windows/operador confiáveis permanecem dependências; arquivo executável/host/boot/TokenId/ModifiedId continuam com provas canônicas; DACL owner tem direitos implícitos dentro do trust model, sem promessa contra administrador equivalente. Pending I/O/FILE/borrow devem ter lifetime demonstrado, e perda de console/pipe não prova quiescência. Não ampliar N7 para outra propriedade.

## Integração pronta para execução técnica após a decisão

[Plano concreto](conditional-integration-plan.md) fecha arquivos, funções, API e sequência:

- Launcher único existente mantém Pin/locks/identidade/Read-Host/F10 e patch Console.Out; troca só o recurso de criação/supervisão/leitura pelo handle original.
- Native helper DLL no MESMO processo e binding managed pré-compilado/pinado, sem compilador/segundo PowerShell/helper-processo durante operação. Primitivas e ownership estão especificados; nenhum artefato foi ativado.
- Adapter explícito stdout no alvo LAB moderno;104 ocorrências inventariadas para classificação por defines/stream, não substituição cega. Sem _dup2; preservar fflush/erros/truncamento/recibos e todas as rotas não opt-in.
- Core N0/ig_approval/PROTECTED_BINDING/censo/Stop/retention não é substituído pelo transporte. Default EMPTY_DENY e positiva DENY.
- Exigir callers reais e console PowerShell de teste, quatro configurações Selected/Legacy Debug/Release sem warnings, contabilidade de frames/storage/DLL/CRT e regressões pertinentes. Não criar harness equivalente aos já existentes.
- Depois, hashes reais de todos os artefatos novos e novo pedido exato de observação manual, incluindo o endpoint efêmero como aquisição autorizada. Não atualizar pins enquanto integração não estiver qualificada.
- Reversão técnica antes de ativação é por hunks/preimagens e hash atual conferido, nunca reset genérico sobre WIP. Não há rollback automático de objetos/effects/runtime.

O plano não exige outra microautorização técnica: após aceite normativo e registro autorizado, a autonomia técnica desta tarefa permanece suficiente para implementar/reconstruir/testar/atualizar pins derivados. A operação read-only ainda precisará de aprovação específica posterior, distinta da norma. Nenhum comando operacional com placeholder é entregue agora.

## Estado e próximo passo único

N5 e B+D accepted: byte-exatos e inalterados. T0–T9: PROPOSED revisão2, com emenda textual exata, revisão integral T0–T9 registrada como avaliação do agente em integral-review.csv e riscos explícitos. Nenhuma aprovação/ACCEPTED foi criada em nome do owner.

Próxima ação concreta: owner decidir especificamente os bytes de T0–T9 revisão2, SHA2565336728e06b1daeb87fe691e637e3ed2a64262967c27ad3c48fb518436b5d995. Se aprovado, verificar integralmente e registrar a decisão focal em documento normativo designado, preservando N5 original; continuar imediatamente a integração técnica conforme o plano. Não é aprovação de Prepare/Start/Stop ou concessão de orçamento. Se a emenda não for aceita, N5 continua impondo DENY a esse transporte.

TECHNICAL_REVIEW_AND_PREPARATION=COMPLETE
NORMATIVE_DECISION=PROPOSED_REVISION2_PENDING_OWNER
N5_ACCEPTED=PRESERVED
PROTOTYPE_CORRECTIONS=OFFLINE_QUALIFIED
DEBUG_RELEASE=PASS_ZERO_WARNINGS
NATIVE_CASES=36_EXPECTED_OUTCOMES_180_ITERATIONS
PURE_GUARD_CHECKS=40_PASS
CANONICAL_INTEGRATION=NOT_STARTED_NORMATIVE_GATE
OPERATIONAL_PINS=UNCHANGED
POSITIVE_CAPACITY=NOT_DEMONSTRATED
ED11_TOTAL_STATUS=PARTIAL
PHASE6_TOTAL_STATUS=PARTIAL
PREPARE_EXECUTED=NO
START_EXECUTED=NO
STOP_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE_EXISTING
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES
NEXT=OWNER_DECIDES_EXACT_T0_T9_REVISION2_THEN_TECHNICAL_INTEGRATION
