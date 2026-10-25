# A1 — revisão focal de reserva antecipada de ownership

PROPOSED — NÃO IMPLEMENTADO. O owner escolheu explicitamente manter A1 em STOP e concluir somente B1. Este documento é proposta de implementação no owner existente, não decisão accepted nem autorização para execução. Nenhuma mudança do helper, caller funcional, array, quota ou pin foi aplicada para A1.

## Propriedade que falta no helper atual

`lab_close_aux(HANDLE h)` recebe uma cópia de um valor já adquirido; depois de duas falhas de close não tem onde guardar um terceiro valor. Bloquear aquisições futuras é necessário, mas não constitui sozinho prova de capacidade para referências já abertas. As confirmações `lab_confirm`, `cycle_confirm` e `ig_confirm` possuem IN e OUT antes de fechar qualquer um. O fixture caracteriza três handles falsos distintos, três chamadas falhas, apenas dois valores guardados e `native.retained` não marcado pelo helper. Não comprova ocorrência real, leak físico ou três referências nativas vivas; não é PASS de segurança. Não usar a mudança B1 como correção dessa falha.

## Menor alternativa candidata

Manter EXATAMENTE `lab_aux_retained[2]`, no mesmo owner e thread de controle. Transformar esses dois slots em ownership desde ANTES da aquisição, em vez de depósito posterior ao erro. Não aumentar storage de handles ou conceder crédito. Estados candidatos por slot: FREE, RESERVED, OWNED, UNKNOWN. Um campo escalar limitado pode representar os estados; um DWORD preserva a primeira falha de fechamento, sem substituir o primeiro erro global. A dimensão exata e o custo desses escalares precisam de revisão de storage/ED11 antes de aplicar; não se presume custo zero. Nenhum novo owner, registro de admissão ou mutex.

Contrato interno candidato, ainda sem código aplicado:

1. Reservar um slot FREE antes de chamar a API de aquisição; verificar Stop, estado do owner e ausência de close incerto. Se não existir slot, retornar STOP/erro antes da API. RESERVED não significa handle adquirido.
2. A API escreve diretamente no slot persistente reservado (ou um wrapper transfere sincronicamente o resultado antes de retornar, com prova explícita). Nenhum local pode ser o único owner após retorno. Saída contraditória/handle não nulo com falha fica UNKNOWN; não descartar nem tentar fechar para fabricar resultado limpo. Validar contrato de saída de cada API usada.
3. Sucesso: RESERVED → OWNED. Falha inequívoca sem referência: RESERVED → FREE. Não liberar reserva em retorno inconclusivo. Nenhuma chamada assíncrona é permitida nesse helper sem análise própria de storage/lifetime.
4. Callers recebem índice/borrow limitado ao slot; não recebem ownership independente nem fecham o alias. O close recebe identidade de slot/estado, não valor arbitrário. OWNED → FREE somente depois de CloseHandle bem-sucedido; limpar valor somente então.
5. Primeira falha de close: OWNED → UNKNOWN, guardar o valor original e o primeiro código, marcar retenção no owner e sinalizar Stop pelo mecanismo existente. Preservar qualquer primeiro erro anterior do fluxo; o erro de close é evidência subordinada quando já há primeira falha. Não fazer nova chamada CloseHandle nesse slot.
6. Congelar novas aquisições e novos usos/efeitos. Outros slots já OWNED permanecem rastreáveis; nesta alternativa conservadora ficam retidos, sem cleanup automático após a incerteza. Logo uma falha em IN não perde OUT nem provoca retry de IN. Não afirmar que os outros slots estão UNKNOWN fisicamente: diferenciar OWNED retido de UNKNOWN, ambos impedem término limpo.
7. Nenhum reset/compactação/reuso de slot congelado no caminho operacional. `cleanup`/`finish` retornam retenção, preservando mutex/dependências conforme contratos; não liberam autoridade. O fixture pode zerar SOMENTE handles falsos, nunca um owner real.

Com dois slots ocupados, a terceira reserva falha antes de qualquer API. Se há dois OWNED, ambos já têm storage; se um é UNKNOWN, qualquer nova reserva falha mesmo que o outro seja FREE. Uma chamada de close com índice inválido, estado RESERVED/UNKNOWN/FREE ou alias que não pertence ao slot nega SEM chamar CloseHandle. Não aceitar um terceiro handle como argumento de transferência depois de adquirido: isso recriaria o defeito.

## Mapa dos callers a adaptar, sem alteração aplicada

| Caller / aquisição transitória | Reserva e lifetime candidato | Propagação obrigatória |
|---|---|---|
| lab_operator / OpenThreadToken | Reservar antes de consultar token inesperado; referência base lab_token continua em seu slot original | Token inesperado nega; close incerto congela e retém; não continuar OpenProcessToken base |
| lab_operator_current / OpenThreadToken e OpenProcessToken | Reserva prévia para cada chamada; soltar reserva apenas na ausência inequívoca de token; token primário temporário no slot até close confirmado | Não abrir token primário depois de token inesperado/close incerto; retorno preserva primeira falha |
| lab_confirm, cycle_confirm, ig_confirm / CONIN e CONOUT | Reservar ambas capacidades antes do par; adquirir em slots; qualquer falha mantém referências já adquiridas | Saturação nega antes de abrir; erro no segundo abre nenhuma referência extra; depois de close incerto reter outro OWNED sem novo close |
| lab_intent, cycle_intent / arquivo temporariamente local legado | Reserva antes de CreateFileW e ownership persistente até close factual | Não ativar rotas legadas; preservar CREATE_NEW e efeito parcial; sem retry/overwrite |
| cycle_live / OpenThreadToken de thread observada | Reservar antes de cada consulta e liberar apenas após ausência/close inequívocos | Close incerto impede próximo thread/token/snapshot; demais globals continuam pertencendo ao cycle owner |
| ig_actor / OpenProcessToken | Reserva antes da API; borrowed token válido apenas durante validação | Falha fecha somente se permitido/inequívoco; incerteza nega censo/métricas e congela |
| ig_extract / CreateFileW do diagnóstico | Reserva antes de abrir; nenhum extract em owner congelado | Não abrir arquivo adicional depois de close incerto; diagnóstico pode permanecer indisponível/UNKNOWN |

Esse inventário cobre sites atuais de lab_close_aux, mas a implementação futura deve provar cobertura de TODAS as entradas/acquisicões alcançáveis após falha, inclusive `ig_project_census`, `ig_observe`, callbacks N0, bind de serviço, ciclo de monitoramento e extração final. Gate tardio depois do callback não basta. Aquisições em slots globais existentes não migram automaticamente para estes dois slots; devem parar antes de adquirir, conservando as referências já armazenadas. Não remover suas verificações de identidade/RAW1/freshness.

## Stop, retenção e contratos

O loop pós-efeito não pode continuar census/observer/cycle_live e adquirir novos tokens/snapshots após congelamento. Deve preservar o primeiro erro e ir à condução existente de Stop/retention; Stop seguro já autorizado pode usar o SC_HANDLE original quando o estado permitir. Não inventar aceitação de Stop em START_PENDING, matar processo, liberar mutex ou criar novo canal. Se não houver como comprovar término, owner/dependências ficam retidos. Fechamento local, Stop, quiescência e disposição persistente continuam distintos.

Model E: UNKNOWN não equivale CLOSED/DESTROYED/refund; borrows e seus owners não podem desaparecer ao sair do frame. N4/N5 e H1/H7 continuam proibindo referências controladas incertas tratadas como resíduo opaco. N6/B4 e ED11 preservam quatro papéis, 128/1024, freshness60s/1s, headroom e NONE_GRANTED. Esta alternativa não relaxa norma accepted nem afirma resolver headroom do ciclo positivo. Se a adaptação exigir garantia diferente, apresentar proposta normativa antes de implementar.

## Provas e testes exigidos antes de aprovação de implementação

- Enumerar todos os pares reserva/API/resultado/uso/close e demonstrar máximo simultâneo <=2, inclusive reentrância/callbacks. Não assumir ausência de reentrância só por usar C síncrono; callback que tentar reservar sem capacidade deve negar antes da API.
- Registrar ponteiros/aliases e provar que nenhum caller mantém ownership local exclusivo. Nenhuma saída do frame descarta slot não-FREE.
- Injetar falha de reserva 1/2/3, primeira e segunda aquisição, primeira falha de close, dois OWNED antes dessa falha, saída contraditória de API, saturação, alias inválido e tentativa de reclose. Contar chamadas: ZERO aquisições depois de congelamento e ZERO retries de close.
- Preservar valor de ambos os slots, primeiro erro, Stop/UNKNOWN/retention, globals/borrows e mutex; negar finish limpo e replay. Testar falha anterior ao close para provar prioridade do primeiro erro.
- Qualificar loop START_PENDING/Stop não aceito usando SOMENTE modelos, sem SCM real; nenhuma observação nova após freeze. Testar Stop antes de aquisição, fechamento incerto e dependências retidas.
- Recontar storage e compilar Selected/Legacy Debug/Release /W4 /WX. Fixtures nativos factuais de lifetime são gate separado, sem execução LAB por esta proposta.

Gate: A1=STOP até revisão da cobertura/invariantes e autorização funcional específica. Não há patch executável A1, novo comando positivo ou autorização operacional neste pacote.
