# Prova corretiva:249 handles observados antes do TPM

Reconciliação da única invocação manual do pedido4aa1d74f, aprovaçãodf36112d. PID15108/criação134376204260676276/boot237900. [Saída original estruturada](OWNER-MANUAL-OUTPUT.json), [reconciliação](RECONCILIATION.json) e [pins históricos preservados](HISTORICAL-PINS-PRESERVED.json).

## Resultado e fechamento

GetProcessHandleCount retornou sucesso, erro0, contagem249. O guard124 negou corretamente antes de abrir contexto TBS:125 acima do guard e121 acima de128. É uma amostra factual da tabela do processo, não pico universal nem atribuição dos handles a owners individuais. Não há margem para a prova nesse ponto. Não descontamos runtime/CLR/kernel handles para fabricar PASS.

Comandos TPM0; contexto NOT_OPENED; prova NÃO ALCANÇADA; observadores próprios ainda não adquiridos; retained=false e observer_refs_retained=false. context_close=false não representa uma falha de close: não houve contexto a fechar. Sem sessão/Name observado, sem amostras administrativas, sem concessão de orçamento. Não foi falha de HMAC/identidade EK ou consulta TPM. Fechamento de todos os recursos internos Windows/CLR e quiescência histórica não são comprovados por esses campos.

A autorização corretiva foi usada por uma invocação. Falha anterior à confirmação/recibo não permite retry nem reaproveitar o registro. Não modificamos a aprovação/executável/pedido anteriores e não executamos o host de prova. Histórico Q4, UNKNOWN e reservas permanecem.

## Investigação não operacional nova

Hipótese específica: a composição CLR+metadata já pode exceder128, independentemente de herança. Não repetimos campanha criptográfica ou outra prova TPM. Novo ResourcePhases.cs não possui APIs TBS/NV nem autoridade, e mede somente recursos sintéticos de inicialização. Métodos noinline separam amostras de entrada, WindowsIdentity, EventLogReader/Process, SHA256 e GC diagnóstico; os próprios mecanismos de observação influenciam a composição, e a JIT pode carregar dependências antes das amostras.

Seis processos CLR foram medidos em Debug/Release: execução direta e filhos de CreateProcess com bInheritHandles=false/true. Pai usa CREATE_NO_WINDOW, aguarda resultado e confere fechamento de suas duas referências. Nenhuma flag global de handles do pai foi alterada. Não introduzimos canários/IPC ou inferimos identidade de objetos por valores numéricos.

| Caminho isolado | Entry Debug/Release | Após EventLog/Process/SHA Debug/Release |
|---|---|---|
| CLR direto |172/173|232/232|
| CLR filho sem herança |188/188|232/232|
| CLR filho com herança |178/193|237/237|

[Logs e resultados](BUILD-TEST-RESULTS.json). O teste sem herança ainda excede128; portanto eliminar herança SOZINHA é insuficiente nesse fixture. Isso não atribui todos os249 handles reais a CLR ou Event Log, nem mede a economia de um launcher operacional. Ambiente/token/console do fixture não equivalem à execução elevada do proprietário. GC.Collect/WaitForPendingFinalizers não reduziu essas amostras; foi diagnóstico isolado e não será aplicado ao host como correção. Não fechamos referências desconhecidas/obrigatórias.

## Alternativa nativa investigada

NativeResourcePhases.c usa Win32/CNG sem CLR para token statistics, consulta EventLog e SHA256, sem TBS/TPM/prova. Debug/Release /W4 /WX PASS, zero warnings. Ambos observaram entry62, token held63, após identidade62, evento held71/após fechamento próprio71, SHA held74/após fechamento próprio74. Fechamento das referências controladas foi conferido; caches/recursos transitivos podem permanecer, por isso contagem não precisa voltar à entrada. Não alegamos liberação global.

São amostras de duas execuções isoladas com CreateProcess sem janela e close_fds, não um teto de processo ou prova de viabilidade completa. Não subtraímos249-74 como economia operacional. Faltam RSA pública, HMAC/AES, TBS, console completo, múltiplas fases/retidos e identidade exata do caminho final.

Quatro builds novos no total (CLR e C x Debug/Release), zero warnings; seis filhos CLR e dois processos C com as fases PASS. [Fonte nativa](NativeResourcePhases.c), [resultados nativos](NATIVE-BUILD-TEST-RESULTS.json). Nenhum produto/launcher/pin operacional mudou; regressões anteriores inalteradas não foram repetidas. Nenhum comando TPM, rede, Q4, Start/Stop, SCM/ACL, cleanup/refund/CURRENT/signature/recovery.

## Decisão técnica e próximo gate

O host gerenciado atual é inviável nas observações deste caminho sob128; trocar apenas recibo ou pedir nova execução equivalente não resolve. Sem waiver de quota ou transferência de custos, a alternativa fundamentada é um host C transitório preservando integralmente o protocolo e os guards. [Plano de implementação e limites exatos](NATIVE-HOST-IMPLEMENTATION-BOUNDARY.md). O fixture nativo não é essa implementação e não foi promovido a qualificador/admissão.

Não há novo pedido operacional neste pacote. O primeiro gate é qualificar tecnicamente o host completo sem CLR e demonstrar composição, lifetime/Stop/falhas antes de pedir uma nova prova manual. A correção de recursos completa ainda NÃO ESTÁ DEMONSTRADA; este resultado fornece a evidência que faltava para rejeitar outra tentativa do mesmo desenho.

E21=PENDING: posse, geração/custódia/NV durável/readback Q4 continuam ausentes. Revogação UNKNOWN. E17/E18 e D4/D5/E19/E20 independentes, sem nova hipótese para repetir suas campanhas; E22 derivado. ED11/Phase6 PARTIAL; reservas/históricos/WIP/autoridades DENY preservados. Nenhuma aprovação nova criada. NEXT=NATIVE_NON_CLR_FULL_PROTOCOL_TECHNICAL_QUALIFICATION; NO_RETRY_MANAGED_PROOF.
