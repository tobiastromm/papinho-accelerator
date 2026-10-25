# N0START-03 — reconciliação da observação nativa do transporte privado

A execução manual aprovada passou no alcance read-only observado. Não houve consumo de intent, criação diagnóstica, Start, Stop ou efeito operacional. O recibo informa first=0, close=0 e exit=0; o transporte confirmou fechamento local, retained=0. Isso não comprova quiescência global ou histórica.

## Integridade e alcance

Saída fornecida pelo owner preservada byte a byte em owner-manual-output.txt, SHA256 `8502efdfd876c01f753457eebcb95da8d07b7cdb2ee4dc7aa158fc67f2212601`. Não foi produzida por nova execução pelo Codex. Os cinco artefatos derivados, pedido e aprovação foram recalculados e coincidem integralmente com os pins; ver artifact-revalidation.json. HEAD permanece `913c90cf2328f7939d8f5b927640601903ac6e91`, main. Nenhuma fonte, launcher, pin, ADR accepted ou evidência histórica foi alterada. O registro aprovado é agora histórico de uma observação realizada e não pode ser reutilizado. Seu arquivo original não foi modificado.

## Medições factuais

| Local | Antes | Durante captura | Após fechamento |
|---|---:|---:|---:|
| Checkpoint p0 |63|64|63|
| Checkpoint p1 |97|98|97|
| Checkpoint p2 |118|119|118|
| PSS/stack walk dos três papéis |118|120, pico amostrado|118|

PowerShell:734 handles; auxiliar:271; orçamento administrativo compartilhado:1005/1024, margem observada19. Qualificador:118/128 antes do PSS, margem10; pico amostrado120/128, margem8. As janelas pertencem à tabela do coletor6812; não são três picos somáveis nem medições do serviço. Picos não amostrados permanecem UNKNOWN. Memória física disponível7116959744 bytes e commit disponível13933473792 bytes, acima dos mínimos5/9GiB no instante observado. Não há crédito ou reserva concedida.

Os incrementos entre checkpoints são34 e21. Comparados aos checkpoints históricos71/105/126, os novos63/97/118 são oito menores em cada ponto; o pico PSS histórico128 passa a120 nesta observação. A diferença entre execuções não prova causalidade por identidade de todos os oito objetos, nem reproduz automaticamente os12 do fixture. Inicialização Windows/RPC/CRT e processos diferem no tempo. Matching por valor numérico de handle não elimina ABA. owner=0 no mapa antigo não significa recurso dispensável: o stdout privado252 é um exemplo de recurso novo fora daquele mapa. Não há demonstração de leak ou refund.

## Capacidade positiva e próximo gate

A observação resolveu a lacuna da composição real read-only com o transporte integrado. Não é necessário repetir uma observação equivalente. A pressão conhecida diminuiu no resultado observado; capacidade positiva completa permanece NÃO DEMONSTRADA.

O ledger positivo anterior identifica +2 referências estáveis antes de Start, +3 no primeiro monitor, +6 após cycle_live e +7 dentro dele, em relação ao binding. O fechamento self-token pertence apenas ao ramo positivo e deve ser contabilizado uma vez. Somar118+7+2=127 é apenas uma projeção conservadora das referências identificadas e do overhead PSS amostrado, sem atribuir zero aos custos omitidos; não é pico medido, teto transiente ou admissão. SCM/rebind, validações específicas, runtime real e orçamento administrativo durante os efeitos continuam sem prova completa. O serviço tem seu próprio limite128, separado da tabela do qualificador. A margem administrativa19 não garante os custos posteriores.

Primeiro gate técnico: revisar e qualificar o despacho stdout das funções positivas e sua composição com o transporte. A feature atual exige positive OFF; o inventário stdout-dispatch-review.csv qualifica somente emissores read-only. Não basta inverter uma flag ou transferir a aprovação usada. O próximo pacote deve incluir o mesmo owner e transporte, cobertura dos emissores positivos, lifetime/retention e gates frescos antes do consumo, com default EMPTY_DENY e aprovação positiva específica ausente. Nenhum comando positivo executável é entregue aqui. Se a cobertura exigir nova decisão normativa, apresentar a cláusula exata antes de alterar a autoridade.

A autorização desta observação não autoriza efeitos. A continuidade técnica pode preparar a composição positiva separadamente, mas qualquer execução positiva depende de pedido exato, aprovação específica e admissão fresca60s/1s; não se deve consumir para descobrir se os limites comportam a execução. Preservar intent/efeitos02, P3, UNKNOWN, reservas reais indisponíveis, RAW1, N7 específico e H0–H9.

## Verificação realizada nesta reconciliação

Leitura e extração integral do recibo, conferência dos sete hashes, confronto com ledgers/relatórios de cobertura existentes. Sem alteração funcional, não houve rebuild ou repetição de testes: os quatro builds sem warnings,60 regressões,52 resultados nativos de transporte e28 casos PowerShell anteriores continuam evidência offline histórica, não foram executados novamente. Nenhuma consulta a objetos protegidos/SCM ou operação nativa foi iniciada pelo Codex nesta reconciliação.

ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
READONLY_OBSERVATION=PASS_AT_OBSERVATION
POSITIVE_CAPACITY=NOT_DEMONSTRATED
POSITIVE_AUTHORITY=DENY
APPROVAL_DISPOSITION=USED_SINGLE_MANUAL_OBSERVATION_NO_REUSE
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
