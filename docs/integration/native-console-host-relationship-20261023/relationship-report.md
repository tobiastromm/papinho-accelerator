# Relação nativa do console host — diagnóstico focal

TASK=PACC-PHASE6-NATIVE-CONSOLE-HOST-RELATIONSHIP-1

Conclusão: o parent histórico do conhost rejeitado era o PowerShell ELEVADO. Isso foi recuperado dos eventos existentes do Windows; não foi inferido de um processo atual com o mesmo PID. A associação autenticada do console host e sua compatibilidade B4 continuam não comprovadas. Nenhum patch funcional, nova execução ou mudança normativa.

## Identidades recuperadas

| Papel | Evidência histórica | Resultado |
|---|---|---|
| Shell interativo externo | Não é ator administrativo identificado no recibo | Não atribuir PID nem confiança |
| PowerShell launcher original | Operational4100/record203381, Execution.ProcessID21220, HostApplication modo Prepare, HostIdab03990d-96fb-40f6-a615-b490374468c8 | PID21220, modo original confirmado |
| PowerShell elevado | Operational4100/record203380, Execution.ProcessID13056, HostApplication modo ElevatedCheck, HostIda2c555de-cb2f-478c-8130-4b2090914258 | PID13056, papel elevado confirmado |
| Qualificador | Recibo pinado: PID22060, creation31286091:2265030170 | Instância histórica identificada |
| Filho rejeitado | Toolhelp/censo: nome conhost.exe, PID19516, parent13056 | Parent é o ator elevado; geração/imagem/token/console não capturados |
| Serviço futuro | PapinhoAcceleratorLab20261023, quarto papel previsto | Nenhum novo Start, token ou PID de serviço coletado nesta tarefa |

O comando ElevatedCheck registrado inclui LaunchProcessId21220 e LaunchCreation134372739879380107. A criação do ator elevado usada no sufixo do recibo é134372739910985976. O código pinado deriva esse sufixo de Creation($PID), e ig_actor valida PID+creation+imagem+SID dos dois atores antes do censo. A relação registrada pelo snapshot é uma relação de parentage; não é prova independente de autoridade ou ownership do console.

## Correlação e proveniência

[Recibo](../canonical-preflight-busy-diagnosis-20261023/manual-prepare-134372739910985976-final.json), SHA256ba08088de05ad163e28a75cc3ce4a3962301cb5980fd2450845af124db9fe47f; log SHA256dc9095ff6f0165574810553bd476665dd85c5db4f155c31c88e6b78baccb743d. Ambos referenciam launcher3983d3fc... e helperd8913071..., conferidos nesta revisão.

[Eventos históricos selecionados](historical-event-evidence.json) foram consultados com Get-WinEvent, janela UTC de10s antes a25s depois da criação elevada; máximo64 registros por canal. Exportados somente12 registros pertinentes e campos selecionados (canal,recordID,eventID,UTC,ExecutionPID,HostApplication/HostId/RunspaceId e SHA256 do XML original em UTF8). Não se exportaram logs completos, scriptblocks de outras aplicações, variáveis/segredos ou dados de Security protegido. Os XML hashes permitem cotejar o evento original enquanto retido pelo Windows; não são recibos de criação de processo nem substitutos de assinatura de imagem.

Windows PowerShell clássico400/403: records224421/224430 para Prepare e224428/224429 para ElevatedCheck. Seus ExecutionPID são0 e NÃO foram usados para atribuir PID. O HostId/HostApplication ligam o contexto aos records Operational4100, que contêm ExecutionPID real. Operational40961/40962/53504 corroboram a inicialização dessas mesmas instâncias. Eventos de engine start/stop não são prova de liberação de todos os recursos.

Tempos UTC (a data local continua2026-10-23, America/Sao_Paulo):

- Launcher creation:2026-10-24T00:06:27.9380107Z, conforme argumento capturado.
- Elevated creation:2026-10-24T00:06:31.0985976Z, conforme sufixo e código pinado.
- Elevated Operational40961 record203377:00:06:31.4415782Z, PID13056.
- Elevated clássico400 record224428:00:06:32.2582844Z, HostIda2c555de... .
- Qualifier creation:2026-10-24T00:06:32.9710106Z, conforme FILETIME do recibo.
- Operational4100 record203380:00:06:48.7901481Z, PID13056/HostApplication ElevatedCheck.

Isso resolve a lacuna anterior de papel do parent. Não recupera retroativamente handle, creation ou consoleID do conhost19516. Nenhum OpenProcess/Get-Process ou consulta WMI a PID histórico foi feita. Não se presume instância atual equivalente, término factual ou cleanup histórico.

## Relação observada e classificação

O snapshot registrou o conhost19516 como filho do PowerShell elevado13056, fora dos participantes. O predicado continuou !participant && child; project_prefix=0. O resultado permanece170/STOP. Ainda não há evidência suficiente para classificar o filho como infraestrutura B autenticada; a classificação segura é E (identidade/associação incerta), com parent de papel conhecido. Não se conclui C (filho arbitrário malicioso) nem D (outra admissão concorrente) a partir do erro.

PRIMEIRA PROVA AUSENTE: uma observação contemporânea console associado → processo host19516, vinculada ao handle e creation time desse host. Também faltam sessão, token, imagem nativa, hash/proveniência/assinatura e métricas atribuíveis. Nome conhost.exe, parent conhecido e mesma sessão isoladamente não preenchem essas lacunas.

## APIs suportadas e limites

- [GetProcessTimes](https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getprocesstimes): creation/exit por handle, direitos de consulta; combinar com boot/PID e retenção do handle para não aceitar reutilização de PID.
- QueryFullProcessImageNameW, GetTokenInformation e ProcessIdToSessionId: nome de imagem, contexto de token/sessão; não provam associação a console. Caminho retornado não autentica sozinho o arquivo efetivamente mapeado. Abrir arquivo por caminho e obter FileID/hash/SD exige controlar substituição; não promover isso automaticamente a identidade da imagem histórica.
- [GetConsoleProcessList](https://learn.microsoft.com/en-us/windows/console/getconsoleprocesslist): membros anexados ao console ATUAL. Pode provar compartilhamento entre clientes com identidades revalidadas; não documenta que o servidor conhost constará da lista, nem retorna sua identidade de imagem. Buffer insuficiente/erro permanece UNKNOWN, não lista parcial aceita.
- [GetConsoleWindow](https://learn.microsoft.com/en-us/windows/console/getconsolewindow) e [GetWindowThreadProcessId](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowthreadprocessid): correlação candidata entre console do chamador e processo criador da janela, quando disponível. Em pseudoconsole há semântica distinta; janela nula não deve ser usada sozinha para decidir ausência de associação, como demonstraram as fixtures anteriores. A correlação não autentica imagem/token e deve ser revalidada contra troca/reutilização.
- [WinVerifyTrust](https://learn.microsoft.com/en-us/windows/win32/api/wintrust/nf-wintrust-winverifytrust) e [WINTRUST_DATA](https://learn.microsoft.com/en-us/windows/win32/api/wintrust/ns-wintrust-wintrust_data): verificação de confiança do objeto indicado, não ownership do console. Se utilizada futuramente sob restrição offline, deve evitar rede explicitamente com WTD_CACHE_ONLY_URL_RETRIEVAL, sem UI, tratar catálogo/estado e fechamento, e preservar UNKNOWN em falha. Nenhuma chamada WinVerifyTrust ou consulta de assinatura foi executada nesta tarefa. Assinar outro arquivo atual não provaria a imagem histórica.

Não foi encontrado nos recibos um consoleID nativo capturado. Nenhuma API foi apresentada como capaz de reconstruir, depois do término, uma relação não registrada.

## Menor correção viável, ainda não aplicada

1. Instrumentação focal no owner existente: emitir papel/PID/creation dos atores já validados e self, em vez de depender novamente da reconstrução de eventos. São fatos já armazenados, sem nova autoridade ou outro censo.
2. Na rejeição atual, coletar relação de console e identidade/creation do host, com direitos mínimos, limites explícitos, resultados de API e closes factuais. Registrar ausência/indisponibilidade sem mudar a decisão170. Não usar AttachConsole/FreeConsole como recuperação automática: mudariam o contexto de associação/handler. Não criar receptor/worker/serviço.
3. Congelar teste/patch diagnóstico, custos adicionais e pins e submetê-los à revisão antes de qualquer futura execução. Esta tarefa NÃO aplica o patch nem repete preflight para suprir o dado.

A topologia mais simples continua candidata (console preexistente herdado), mas o resultado real já demonstrou que a mudança isolada não fechou o gate. Reduzir intermediários exigiria rever o binding de dois atores e sua retenção; não é um rename de papel. Se o host for comprovadamente auxiliar novo da operação e sua presença inevitável, B4.B.4 precisará de revisão EXPLÍCITA para esse auxiliar: limite de multiplicidade, identidade, métricas, agregado e lifetime, sem crédito ou quota nova. A [alternativa D já proposta](../lab-launcher-topology-simplification-20261023/B4-B4-topology-PROPOSED.md) é apenas candidata condicional; não foi aceita nem selecionada por esta investigação. Não é necessário criar outra ADR concorrente. Associação comprovada, sozinha, ainda não concede quinto slot.

## ED11 e governance

Memória/handles/threads/stack/tempo de coexistência do host histórico continuam UNKNOWN, não zero; nenhum crédito concedido. close=0 do qualificador não prova término do conhost, e eventos403 não provam release de handles. B4.B.1/B4.B.4/B4.B.5/B4.B.6, Model E e ED08/ED11 mantêm identidade, retenção e admissão antes de efeitos. Contratos accepted preservados. Somente consulta histórica read-only e documentação foram executadas.

HEAD main913c90cf2328f7939d8f5b927640601903ac6e91. Baseline de8542 arquivos versionados/untracked não ignorados coletado antes da escrita documental; ver verification.json para mudanças exatas. LABs e binários pinados preservados. Sem mudança PST/Browser/wire/produção0.6.4/quotas, clone/worktree/commit/push/tag/release.
