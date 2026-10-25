# Atribuição de handles — closeout único e próxima observação discriminante

## Resultado

Atribuição PARTIAL: provamos objetos herdados no fixture e qualificamos a instrumentação do mesmo qualificador para obter a decomposição real. Os126 handles históricos continuam sem atribuição causal completa; nenhuma redução funcional foi presumida ou aplicada. Não há demonstração de impossibilidade universal nem de capacidade positiva. O trabalho técnico preparatório está concluído; falta somente aprovação específica da observação nova e seu resultado manual.

## Mapa factual e método

O launcher canônico usa ProcessStartInfo/UseShellExecute=false/CreateNoWindow=false,stdout redirecionado,stdin/stderr não redirecionados e Process.Start. Não fornece lista seletiva Win32. Oito eventos sintéticos marcados herdáveis apareceram no filho; não herdáveis não apareceram. Isso demonstra o mecanismo efetivo testado, não que todo handle do pai seja herdado.

Reutilizamos service_observer_stack_va_isolated_test: modo explícito handle-map, PSS_CAPTURE_HANDLES/BASIC/NAME/TYPE_SPECIFIC, PssWalkSnapshot(PSS_WALK_HANDLES), GetHandleInformation e identidade PID+criação. A enumeração inicial acontece APÓS CRT/loader, não no instante exato de nascimento. Type/Flags/Attributes/GrantedAccess e nome UTF16 vêm do snapshot; campos indisponíveis ficam UNKNOWN. Nomes não autenticam objetos. Consultas live podem ocorrer depois do snapshot; valor numérico, tipo, nome e flags iguais não provam identidade universal ou ausência de reutilização.

Para os eventos controlados há prova adicional: o pai conserva os originais não sinalizados, o filho sinaliza suas referências e o pai confirma WaitForSingleObject=signaled nos mesmos originais. Nenhum outro processo recebe esses eventos. A confirmação de16 objetos (8 por configuração) é mais forte que coincidência numérica. Nenhum objeto real do operador ou LAB foi sinalizado/modificado.

O fixture separou a fase CRT_ENTRY da criação de UM evento próprio controlado em OWNED_EVENT. Os logs incluem tabela, stdio, PID/criação e fechamento. O evento próprio foi identificado no snapshot como Event; seu lifetime e fechamento são explícitos. As16 capturas finais continham840 entradas no conjunto dos oito processos e duas fases, não840 recursos simultâneos. Release baseline42 e variante herdável50; Debug58 e66. Oito não herdáveis mantiveram42/58. Depois da captura: +1 handle amostrado; marker não acrescentou contagem no ponto observado; após free retornou ao baseline em todas essas capturas. A criação própria adicionou1. Picos entre pontos e custos internos/memória continuam UNKNOWN.

No baseline Release desta execução houve tipos File7,Event6,WaitCompletionPacket6,IoCompletion2,TpWorkerFactory2,IRTimer4,Directory2,ALPC Port1,Mutant1,Semaphore2,Key2 e7 entradas sem tipo conhecido. Isso é composição do fixture, NÃO atribuição de causa de cada alocação a CRT, loader ou SCM. Não houve SCM no fixture. Tabela do pai, filho e administrador permanecem separadas.

## Implementação focal pronta, ainda não executada no LAB

Nova opção PAPACC_ENABLE_READONLY_HANDLE_TABLE_TRACE, defaultOFF, exige pinsN0START03ReadOnly e proíbe positivo tanto no CMake quanto no C. scm-readonly-handle-table.inc usa os slots snapshot/marker do observador existente e sua rotina de fechamento; sem novo owner, worker, driver, registry, recorder ou mecanismo de autorização. A1 mantém exatamente dois slots auxiliares. O mapa de27 aliases é metadata temporária de borrow, não27 owners ou uma ampliação do retained store.

Três checkpoints no mesmo fluxo autorizado futuro:

1. Após CRT/console/aprovação e antes de lab_operator/boot.
2. Após arquivos comuns/dependências e readback SCM, antes de bindingN0.
3. Após bindingN0 e antes de lock/censo/métricas.

O mapa owner-slot-map.json identifica somente handles conhecidos ainda detidos pelo owner. owner0 significa origem desconhecida — não automaticamente Windows, interno, herdado ou dispensável. stdout/stdin/stderr são registrados como aliases. PID/criação acompanha cada tabela. Nomes de objetos são emitidos como SHA256, tipos em UTF16hex limitado16 caracteres; o original não é alterado. A tabela completa do fixture pode conter nomes públicos; o opt-in operacional emite digest para limitar conteúdo.

Cada checkpoint permite no máximo128 entradas, sem sobreposição com a captura de threads/stack. Se a contagem inicial já for128, recusa a aquisição opcional; se a contagem amostrada exceder128, falha e fecha no alcance comprovado. Não é aumento nem substituição do guard128. PSS pode ter custo interno não amostrado. Até128 linhas por checkpoint, buffers limitados, o limite131072 do launcher permanece; truncamento força resultado não qualificado e drena o stdout até recibo final. Não considerar o output um arquivo persistente.

Saída contraditória com referência nativa, falha de free/close, retenção prévia ou freeze impedem nova captura. Referências incertas permanecem nos slots originais; primeira falha preservada. Em testes, erros são injetados sem forçar falha real de CloseHandle em objeto operacional. Exit2 de contenção do fixture NÃO é fechamento nativo, quiescência ou refund. Os caminhos positivo e sem opt-in preservam suas linhas não vazias após remover os blocos novos; isso não é equivalência binária formal.

Layout medido: PSS_HANDLE_ENTRY136bytes; vetor temporário27 aliases x8=216bytes. Há também digest32bytes, FILETIMEs, escalares, alinhamento e frames de funções chamadas. Esses números não são memória total nem custo físico/kernel do PSS. Nenhum crédito foi concedido; o ledger/stack total permanece limitado ao alcance medido e sujeito às métricas futuras.

## Build e testes finais

Selected Debug/Release PASS, zero warnings (/W4 /WX).32 cenários do observador e do método real de tabela tiveram os resultados esperados;30 regressões existentes PASS. Oito casos finais de enumeração/herança e16 capturas passaram. Duas configurações CMake proibidas falharam como exigido. Parser do launcher PASS sem executar o script. Nenhum build Legacy/VC6 ou execução NT4 é reivindicado.

O novo include inicialmente não provocava rebuild incremental. Foram acrescentadas OBJECT_DEPENDS explícitas ao qualificador e ao fixture, incluindo seus dois novos includes. Final build5 recompilou os objetos; final5-qualification-results.json corresponde ao código final. Logs anteriores mostram os ensaios intermediários, não são o estado final. O teste Stop teve duas expectativas de fixture corrigidas (Stop inicial versus Stop durante walk); não houve mudança de política para obter PASS. Após a correção de dependências, hashes foram recalculados; nenhum artefato antigo foi usado como qualificação do fonte novo. Dry-run final: no work to do.

Evidências atuais: final5-qualification-results.json, map-final5/, experiment-checks/table-entries/table-summary (campanha inicial explicitamente histórica), guard-*.log, final5-trace-off-source-equivalence.json, FINAL-ARTIFACTS.json e FINAL-SOURCE-AND-BUILD-HASHES.json.

## Orçamento e alternativas

O resultado real anterior1002 administrativos/126 préPSS/128 amostrados continua histórico. Não somar as tabelas do coletor aos administradores. A projeção126−1+2+2=129 préStart e130 no primeiro monitoramento continua condicional; não mede pico positivo nem autoriza efeitos. H/P3, reservas reais e UNKNOWN preservados. Não fechar intent, imagem, originais RAW1, tokens-base ou thread de identidade antes do último uso. C1 após cycle_live não resolve o primeiro pico.

Alternativas concretas se a tabela real confirmar a causa:

- Herança seletiva no mesmo PowerShell via criação nativa com HANDLE_LIST: excluir apenas referências dispensáveis comprovadas, preservando stdio/console, PID/criação original, Stop e retenção. Não modificar flags globais do PowerShell. A API existe em Vista+, mas console/stdio no Windows7 e a integração PowerShell5.1 exigem testes próprios; não é port para VC6/NT4. Não aplicada sem causalidade demonstrada.
- Consolidar armazenamento de provas estáticas sob um único objeto protegido e owner existente: poderia reduzir referências a políticas, mas altera a garantia de originais independentes e exige decisão normativa focal, novo binding e análise de substituição/replay. Não é cache de hashes autorizado por esta tarefa. Não garante o pico posterior de cycle_live.
- Preservar DENY se custos necessários/internos não permitirem capacidade demonstrável. Nenhum aumento128/1024, troca de tabela ou crédito. Não afirmar impossibilidade universal a partir de projeção.

Nenhuma decisão arquitetural material é pedida agora: a menor próxima ação é a observação discriminante pronta, pois a causa real ainda não foi medida.

## Pedido exato e fronteira operacional

O pedido canônico READONLY-OPERATION-REQUEST.DRAFT.json foi atualizado, preservando sua preimagem. Observação N0HT-20261024-01 vinculada à mesma operaçãoN0START03/tentativa0b1de869e11493b7cb08fb329578f6ce; não cria nova tentativa, intent, imagem, sink ou dossiê. Solicita UMA execução manual read-only com os três checkpoints e métricas existentes, sem Start/Stop/rebind/SCM mutável/ACL. Guardas, RAW1, N7, H/P3, headroom5/9GiB,60s/1s, quatro papéis,128/1024 e NONE_GRANTED permanecem.

Qualificador SelectedRelease 95744bytes SHA256 2bf1746ae9de3f5d559b4fc989d9da3c3f9dacfe47539f8b0b067e2fe496ae15.
Launcher 11855bytes SHA256 3451d4d5e2b2dd6bfae8ac64fc97293b63a9d7a3658fd0d00cba805be35bf2ab.
Pedido SHA256 243a8a74ce5346998f4f75b656fa028064793f3f082dd621043f1d358e5cb03e.

Registro futuro exclusivo: handle-attribution/EXACT-HANDLE-READONLY-APPROVED.json, atualmente AUSENTE. O registro57610fea anterior e seu qualificador635eaa permanecem históricos íntegros; não autorizam os novos bytes/escopo. Candidate NOT_APPROVED mantém owner_decision=null e execution_authorized=false. Não existe hash executável a fornecer antes de nova decisão explícita. O procedimento manual candidato está pronto no pacote canônico; após aprovação específica registrar a decisão real e entregar o SHA literal. Codex não executa automaticamente.

## Compatibilidade e fontes oficiais

[PSS_HANDLE_ENTRY](https://learn.microsoft.com/en-us/windows/win32/api/processsnapshot/ns-processsnapshot-pss_handle_entry) documenta campos e lifetime dos nomes; PSS exigeWindows8.1+. Não generalizar esse experimento ao targetWindows7 do serviço ou NT4.
[GetHandleInformation](https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-gethandleinformation) documenta flags; inherit não é certificado de origem.
[Herança](https://learn.microsoft.com/en-us/windows/win32/sysinfo/handle-inheritance) exige configuração de herança, não herda indiscriminadamente todo handle do pai.
[UpdateProcThreadAttribute](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-updateprocthreadattribute) documenta HANDLE_LIST e os requisitos de handles herdáveis/bInheritHandles. A existência da API não prova compatibilidade da alternativa com o produto.

CURRENT_TASK_COMPLETED=YES
HANDLE_ATTRIBUTION=PARTIAL
INHERITED_REAL_HANDLES=UNKNOWN
CONTROLLED_HANDLES=LIVE_OWNER_SLOT_MAP_READY_NOT_FRESHLY_OBSERVED
INTERNAL_WINDOWS_SCM_HANDLES=UNKNOWN_REAL;TYPES_OBSERVED_IN_ISOLATED_FIXTURE
INSTRUMENTATION_OVERHEAD=PLUS1_HANDLE_SAMPLED_TABLE_CAPTURE;TOTAL_INTERNAL_COST_UNKNOWN
SAFE_REDUCTION=NOT_DEMONSTRATED_FOR_REAL_QUALIFIER
POSITIVE_PRESTART_CAPACITY=NOT_DEMONSTRATED
FIRST_MONITOR_CAPACITY=NOT_DEMONSTRATED
NATIVE_POSITIVE_EXECUTION=NOT_AUTHORIZED_NOT_RUN
NATIVE_READONLY_EXECUTION=NOT_RUN_THIS_TASK
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
ED11_TOTAL_STATUS=PARTIAL
PHASE6_TOTAL_STATUS=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
FIRST_UNRESOLVED_GATE=OWNER_APPROVAL_AND_ONE_REAL_DISCRIMINATING_HANDLE_TABLE_OBSERVATION
NEXT=OWNER_APPROVES_N0HT_20261024_01_EXACT_READONLY_REQUEST
