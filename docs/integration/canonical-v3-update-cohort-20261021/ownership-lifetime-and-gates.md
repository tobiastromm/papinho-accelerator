# Prova do cohort no produtor/owner existentes

## Gate pré-patch e compatibilidade

Esquema nativo temporário compilado antes de alterar fontes: sizeof(PAPACC_V3_UPDATE_COHORT)==80, MSVC/W4/WX. SHA da prova pré-patch a27b3a220c6c5150721c6f9e41192162a618435d6eec52592031973d68fc5f70. O descritor mantém predecessor_pin, serial do candidato, source_ticket, invocação, lower-bound reserved, fase, índice e pending anterior. Não contém autoridade booleana, novo epoch, ponteiro externo retido ou storage de credenciais. Não é serializado. Layout comum a todos os consumidores Windows3MiB; funções novas apenas no opt-in offline. Default/Legacy/box/ISA não recebem alteração de orçamento ou layout pelo profile Windows.

D2.2/G0/G7 permitem mesma root/owner com base CURRENT, snapshot/pin e coexistência; a implementação não revoga guards de bootstrap/restore, não aceita baseROOT06/v1 nem inicialização v3. Não altera PACROOT8/PDG1/CP3/SNT1/LST1/BODY ou hash domains. E0–E8, M1 e ownership compartilhado não recebem fontes/leases/fábricas positivas. Não foi aceita decisão normativa nova ou API wire.

## Inventário e ownership

| Objeto | Owner / tamanho | Início e término factual | Coexistência |
|---|---|---|---|
| Descritor80 | configuration_commit dentro da root canônica | Fase EMPTY→HELD; limpo após abort factual | Reservado em todos layouts Windows Selected; sem pool paralelo |
| Predecessor pin | Ticket canônico existente32 dentro dos80 | current_pin real no produtor; hold move e zera o pin local; abort unpin factual | Protege geração antiga e CA/SNT/PDG/carrier G7; não publica candidato |
| Predecessor completo | committed + G7 CURRENT + pending.previous30592, todos já reservados | Copiado inteiro para previous no hold; committed/G7 anterior nunca sobrepostos | Mantém CP3/R/A/P/I/C/W e histórico issuer durante hold/Stop/retido |
| Candidato | Slot G7 livre packet30592/CA8192 já entre3 slots reservados | Cópia por valor; slot serial próprio; backing serial0 até validação futura | Sem pointer caller, sem pin, sem borrow candidato exportado; slots reader/retained excluídos |
| Source antiga | Objeto publicado t.source permanece owner; ticket8 nos80 | borrow existente após hold; unborrow próprio depois de quiescência | Material e configuração não copiados ou usados como scratch; borrower extra protege lifetime, não resolve refs candidatas |
| Input caller | Até30592 bytes | Imutável e exclusivo até retorno; depois reutilizável | Lower-bound do call contabiliza input e locais; não vira crédito factual |
| Frames/hash | Caller/owner; medidos em direct-frames.json no evidence.zip | Somente chamada síncrona, sem filho pendente | Stack transitivo/provider/kernel UNKNOWN; nenhum hash/close é iniciado por Stop |

Hold DATA recusa owner/pin inválidos, limite/reserva insuficiente, store não8, operações pendentes/incertas e comprometimento anterior não disposto. Confere pin atual, backing íntegro, imagem committed inteira, R/predecessor/A/P/I/C/W; candidato exige versão3, bounds, hash completo, checkpoint ausente e carrier inteiro idêntico ao predecessor. São checks de staging/correlação, NÃO validador completo de PDG/SNT, perfil X509, aprovação ou status. Um candidato internamente inválido mas re-selado jamais alcança RESOLVED: validação positiva não existe neste slice.

Seleção de slot, cópia e transferência do ticket ocorrem sob lock após retorno do trabalho hash; rechecagem da tabela canônica confirma ticket/serial/readers. Campo serial0 do backing impede que validação futura confunda bytes staged com snapshot exportável. O slot recebe somente domínio/R/predecessor candidatos; A/P/I e flag issuer válida permanecem ausentes. Não se inventa publicação.

## Integração com o produtor canônico

configuration_v3_update_hold exige singleton/thread canônicos, runtime dormente, ausência de pins de Sessions/listener/attachment, CURRENT factual e reserve compatível com combined_reserved. Adquire pin por current_pin existente e o transfere ao descritor, preservando alcançabilidade até num erro de release. Usa source_borrow existente sem mudar t.model, policy, source material ou estado de publicação. Retorno NOT_SUPPORTED pode significar cohort HELD; o caller deve consultar o descritor no mesmo owner e usar update_abort, não transaction_abort/close histórico. Esse padrão não é acquire E0 nem aquisição assíncrona; não há lease SDK.

configuration_v3_update_validate lê a mesma referência canônica, A/P/I e invocação existentes, revalida CURRENT/source e ticket source. Não emite novo pin/epoch. Termina NOT_SUPPORTED por falta do produtor factual de aprovação/referências candidatas. update_commit também nega e não chama packet11. Sem CURRENT/source reais, os guards anteriores negam sem fabricar estado. A primeira implementação não suporta base v2, fonte alterada, validação positiva ou operação online.

## Dispositions e liberação

| Estado/fato | Ação segura |
|---|---|
| Hold rejeitado antes de move | Pin caller permanece; nenhum slot/cópia ou owner novo |
| HELD não resolvido | Mesmo owner conserva pin, bytes e source_ticket; sem worker/commit/CURRENT |
| Stop | Sinaliza pelos caminhos existentes; nega validação/novos efeitos; não libera recursos |
| Abort antes de efeito | Verificar thread/transaction/file/close/result/slot/serial/readers e source; unborrow source, depois unpin; somente então limpar slot/cópia e restaurar pending anterior |
| Falha de quiescência/release | RETAINED; mesmos recursos alcançáveis, nenhuma liberação por timeout |
| Necessidade de COMMITTED/CONFLICT/UNKNOWN | Cohort não resolvido não inicia efeito. Worker11 existente distingue resultados e retém incerteza. Integração futura deve preservar ambos grafos e revalidar autoridade/reserva imediatamente antes do efeito |

Os mutadores históricos prepare/commit/publish/abort/finish e a captura G7 não podem consumir/sobrescrever o cohort. current/current_pin continuam a reconhecer a geração anterior no modelo enquanto hold não causa efeito; Stop invalida aplicabilidade mas mantém memória protegida. Abort próprio não usa cleanup da preparação publicada. Se houver resultado não-IDLE/UNKNOWN, handles ou commit pendente, abort conserva ambas gerações e source; não supõe rollback/cancelamento.

## Provas e próximos gates

108 checks por configuração no novo teste: ticket obsoleto não emitido, slots ocupados, predecessor/issuer/CP3/PDG corrompidos, reserve insuficiente, pin transferido, CURRENT-modelo e committed inalterados, bytes candidatos isolados do caller, mutadores históricos negados, slot não quiescente retido, abort, Stop, borrow anterior preservado e finish só após unpin. Contexto CURRENT é explicitamente MODEL; SHA/CA pública e operações de ticket/unpin são nativas. Singleton real é somente negativo: sem CURRENT/source/store, hold/validate/commit/abort negam e t.model/source/preparação ficam intactos. Não há prova nativa de autoridade positiva.

Permanecem: fonte CURRENT/predecessor factual e histórico completo; resolução/proveniência/status/revogação de referências candidatas; validação completa kind8 pré-publicação sem pin fictício; base v2 e alterações autorizadas de source/issuer/policy; reservas factuais e integração ao packet11; resultado e eventual publicação separadamente autorizada. O cohort80 resolve ownership/staging, não substitui essas dependências. Não criar store/CURRENT para suprir a ausência já declarada pelo owner.
