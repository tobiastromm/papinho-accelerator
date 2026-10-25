# ED11 — qualificação isolada de herança explícita

Data de fechamento: 2026-10-25. Branch main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91 preservados. Escopo técnico do documento enviado pelo owner; nenhuma autorização operacional foi inferida.

## Conclusão factual

Herança limitada reduziu a tabela do filho no experimento serializado: Debug68→56 e Release52→40, duas repetições por variante/configuração. Oito eventos sintéticos retidos no pai foram identificados por CompareObjectHandles: oito correspondências na herança global e zero na limitada. A diferença total12 não prova a origem das outras quatro entradas e não demonstra exclusão nem dispensabilidade das nove candidatas reais do LAB.

A alternativa permanece experimental e a ADR permanece PROPOSED, revisão2. Capacidade positiva não demonstrada. Nenhum novo comando operacional ou aprovação foi criado.

## Implementação e ownership

CMake adiciona DLL apenas na área de testes Windows. O novo scm-explicit-inheritance-fixture.c inicia somente o executável de teste isolado. Mesma instância PowerShell de teste, sem helper-processo de autoridade; controlador verifica bytes dos artefatos sob referências de leitura antes da criação. Guardas de basename/argumentos são limitações de segurança do fixture e não prova de identidade executável.

STARTUPINFOEX/HANDLE_LIST contém duplicatas privadas de stdin/stderr e escritor privado de stdout; nenhum handle original do operador tem flags modificadas. Oito eventos novos pertencem exclusivamente ao fixture. O leitor do pipe e o processo original pertencem ao contexto C persistente. Borrow do leitor usa ticket/geração; SafeFileHandle não é owner do handle. PID/criação vêm do objeto original de CreateProcess, Wait/exit-code usam a mesma referência; não se reabre PID para supervisão. A comparação dos eventos abre o pai e confere criação/lifetime para importar somente referências sintéticas de identidade.

Pico de referências controladas do helper6; estado estável2 (processo e pipe). Contexto compilado4408 bytes em ambas as configurações, atributo lógico48 bytes, pipe solicitado65536 bytes. Isso não inclui total de heap, loader/DLL, CLR/JIT, buffers gerenciados, kernel/cache/stack ou picos não amostrados. Aquisição privada4, mais processo/thread2, depois fechamento comprovado de quatro; close final comprovado das duas restantes no caminho limpo. Nada é transferido como crédito ED11 entre tabelas.

Falhas congelam quando ownership é incerto: falha simulada de close retém6 referências, saída contraditória de pipe retém2, lease inválido/erro de saída retêm2. Novas aquisições são rejeitadas após freeze. Não há retry de close, kill ou apagamento. Subprocessos de teste encerram naturalmente com exit2 e contenção pelo fim do processo; isso não constitui procedimento de recuperação do produto nem refund factual de custos transitivos. Close=0 nunca é quiescência global.

## Alternativas

1. Sem herança global: adequada para processo sem capacidades herdadas. Não demonstrada como equivalente ao protocolo atual que precisa de stdout redirecionado e stdin/stderr. Não escolhida para substituir o launcher.
2. HANDLE_LIST com referências privadas: menor hipótese implementada e testada isoladamente, API Vista+. Não entra em componente NT4. APIs ausentes causam ERROR_NOT_SUPPORTED sem fallback irrestrito.
3. Processo adicional de transferência: não adotado; mudaria topologia/autoridade e acrescentaria custos.

## Testes e medição

Debug/Release PASS /W4 /WX, zero warnings. 30 casos de comparação, protocolo e falhas +6 de lease/duplicação/erro de escrita PASS; 32 cenários do observador/tabela e30 regressões do owner/recursos PASS. Resultados JSON e logs individuais no mesmo diretório. Nenhum qualificador LAB foi executado: houve build em árvore nova, sem substituir os artefatos qualificados anteriores.

Protocolo ASCII atual, linha vazia, texto UTF16 hexadecimal, stdin de confirmação, comando Stop sintético e stderr passaram. Stop sintético não é StopService. Não qualificado: console interativo real/F10/Escape, host personalizado, transcript, Unicode arbitrário e a política de truncamento do launcher real. O controlador de teste drena saída, mas seu ReadToEnd não é proposta de substituição do loop limitado do produto.

PSS foi medido separadamente: dump inicial captura +1 handle na tabela do filho e retorna à contagem inicial após free; marcador não acrescentou handle da tabela no ponto observado. Comparação de identidade acontece depois do primeiro dump e adquire referência do pai, snapshot e uma referência de evento por vez; esse custo não é deduzido do baseline. Não mede pico universal de PSS, nem reproduz SCM/RAW1/WinTrust/runtime do LAB.

As primeiras execuções das duas variantes falharam com0xC0000005 e exibiram caixas ao owner. Logs trial1/trial2 e diagnostic preservam o ocorrido. Instrumentação de fase localizou a falha dentro da segunda PssCaptureSnapshot: a combinação reduzida HANDLES/BASIC usada pelo teste falhou; combinação completa já utilizada pelo enumerador existente passou. Não se afirma causa interna do Windows. O teste final usa a combinação completa. SetErrorMode elimina apenas a caixa modal de falha no modo handle-map do fixture; o status fatal continua sendo falha e reprova o teste. Não foi alterada política do sistema ou do produto. Outra correção no fixture rejeita explicitamente pseudo-handles antes de duplicação, pois GetHandleInformation isolado não demonstrou rejeitá-los neste ambiente.

## Histórico e projeções

Preservados: observação71/105/126, controlados1/9/27, administração984/1024, pico amostrado128, pico não amostrado UNKNOWN, P3/H/Model E e todos os objetos históricos. Mapear12 entradas herdáveis não prova origem: historical-inheritance-dependencies.csv mantém as nove não stdio UNKNOWN.

Projeções condicionais existentes: pré-Start129−R+U; primeiro monitor130−R+U; cycle_live131−R+U estável,132−R+U no thread observado,133−R+U com captura posterior. R é redução real futura comprovada e U o novo custo interno do filho. Não inserir8,12 ou9 por extrapolação. +26 sem mapa após SCM não foram atribuídos nem eliminados. Todos os limites128/1024, quatro papéis, freshness60s/1s, headroom e créditos NONE_GRANTED preservados.

## Primeiro gate não resolvido e próximo passo concreto

A lista restringe ESTA criação; não impede que outra criação global concorrente no mesmo pai herde transfers privados durante a janela em que são herdáveis. O fixture é serializado e não provou ausência dessa concorrência em todos os componentes/runspaces de um PowerShell real. O [contrato HANDLE_LIST](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-updateprocthreadattribute) exige referências herdáveis. [Análise oficial da janela concorrente](https://devblogs.microsoft.com/oldnewthing/20260511-00/?p=112313%2F) distingue a restrição do filho da exposição para outras criações. A [comparação oficial de objetos](https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-compareobjecthandles) limita a prova aos objetos efetivamente comparados.

Não há evidência suficiente para ativar a hipótese no launcher protegido. Próximo gate é revisão arquitetural focal de I1/I3: definir como demonstrar exclusão de criação global concorrente no supervisor existente e qualificar o console real/Stop/retention sem operação LAB. Se isso não puder ser garantido mantendo a topologia, STOP nessa alternativa; não substituir por outro owner/processo nem inferir segurança a partir de uma saída visível. Somente depois dessa fronteira podem ser revistos integração e eventual novo pedido operacional. Não repetir N0HT01 ou criar outra observação genérica.

Prompt pronto: “Revisar a revisão2 I0–I6 e CLOSEOUT de explicit-inheritance; resolver exclusivamente o isolamento das referências privadas contra CreateProcess global concorrente e os contratos de console/Stop do supervisor existente. Autorizar somente fixture discriminante novo para essas duas lacunas; nenhum LAB/Prepare/Start/Stop, mudança de autoridade ou ativação de pins. Se não existir solução na topologia atual, apresentar a menor decisão arquitetural necessária.”

## Estado

CURRENT_TASK_COMPLETED=YES
INHERITANCE_DEPENDENCY_MAP=12_HISTORICAL_ENTRIES_9_UNKNOWN
SELECTED_IMPLEMENTATION=ISOLATED_OPTIN_STARTUPINFOEX_PRIVATE_TRANSFERS
NT4_BOUNDARY=PRESERVED_MODERN_TEST_ONLY
STDIO_AND_CONFIRMATION=PIPE_FIXTURE_PASS_INTERACTIVE_NOT_QUALIFIED
CONTROLLED_CHILD_HANDLE_REDUCTION=8_SYNTHETIC_OBJECTS_PROVEN_TOTAL_DELTA12_NOT_FULLY_ATTRIBUTED
QUALIFIED_OFFLINE_INHERITANCE=PASS_ISOLATED_SERIALIZED_SCOPE_ONLY
OVERHEAD_AND_BUDGET=PEAK6_STABLE2_STATE4408_ATTRIBUTE48_TRANSITIVE_UNKNOWN
DEBUG_RELEASE=PASS_ZERO_WARNINGS
REGRESSIONS=62_PASS_NEW_CASES36_PASS
ADR_STATUS=PROPOSED_REVISION2
POSITIVE_CAPACITY=NOT_DEMONSTRATED
ED11_TOTAL_STATUS=PARTIAL
PHASE6_TOTAL_STATUS=PARTIAL
LAB_EXECUTED=NO
START_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
OPERATIONAL_PINS=UNCHANGED
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
WIP_PRESERVED=YES_WITH_TASK_LOCAL_EDITS_AND_PREIMAGES
FIRST_UNRESOLVED_GATE=PRIVATE_TRANSFER_CONCURRENCY_AND_INTERACTIVE_SUPERVISION
NEXT=FOCUSED_ARCHITECTURAL_BOUNDARY_REVIEW_NOT_OPERATIONAL_REPLAY
