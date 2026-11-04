# Próxima decisão real — escopo diagnóstico, sem execução automática

Projeto: PapinhoAccelerator + PapinhoBrowser, checkouts canônicos existentes.
Uma operação candidata; não reutilizar Q4/Q7.

Revise a proposta D0–D9:
`docs/adr/ADR-CANDIDATE-mode-c-one-operation-diagnostic-authority-20261026.md`
SHA256: `84e2c9a7e3922247ee5ca8901594d29a2e1418eef68f91e58cd69ff53cc2a2bf`

Decida entre:
A. Manter DENY até obter envelopes rígidos completos.
B. Prosseguir com uma emenda explícita e limitada de política diagnóstica para uma operação, sujeita a anexo quantitativo justificado, à configuração administrativa/CURRENT/SiteTLS existente e a aprovação operacional posterior.
C. Manter o alvo desabilitado.

A decisão técnica de uma operação já está registrada no desenho; não precisa ser repetida. A proposta acima NÃO está pronta para autorização incondicional de execução: faltam parâmetros quantitativos justificados, binding administrativo/destino/módulos e integração de snapshots ativos nos owners. Não basta responder “aprovo” para transformar esses campos ausentes em valores válidos. Uma decisão sobre B deve nomear exatamente a garantia opaca relaxada e os requisitos que continuam obrigatórios; não autoriza setters fictícios, bypass, novo owner/serviço ou alteração de quotas.

Após uma decisão normativa válida e completa, continuar autonomamente a integração técnica já autorizada, suas builds/regressões e os pins derivados; preparar um ÚNICO pedido manual exato com hashes finais. Não solicitar aprovação por edição. Só esse pedido operacional posterior poderá receber aprovação de efeitos. Não executar Prepare, Start/Stop, conexões ou consumo automaticamente.

## Texto de futuro pedido operacional — INERTE / NÃO APROVÁVEL AINDA

TASK=MODEC-ONE-OP-CORRELATED-MANUAL-DIAGNOSTIC
REQUEST_SHA256=NOT_CREATED
OPERATION_IDENTITY=NOT_CREATED
NORMATIVE_DIAGNOSTIC_POLICY=NOT_ACCEPTED
ROOT_CURRENT_EPOCH=UNBOUND
SITE_TLS_AND_DESTINATION=UNBOUND
MAPPED_PROVIDER_OBJECTS=UNBOUND
FRESHNESS_HEADROOM_AND_DURATION=UNBOUND
ARTIFACTS=STATIC_PREINTEGRATION_ONLY
APPROVAL=ABSENT
EXECUTE=DENY

Escopo futuro: uma operação pela rota canônica, uma navegação text/plain sem redirect/subrecursos/retry; observar provider inicializado, handshake, atividade e Stop/fechamento ou retenção. Canais e owners existentes; orçamento individual sem transferências. Erro/UNKNOWN retém efeitos e primeira falha; sem kill, rollback, delete ou refund. A topologia inclui as conexões CONTROL/APP/RECORD e loopback C3 existentes, não apenas “um socket”. Aprovação futura deverá delimitar esses efeitos e o Site autorizado.

Q4: readback informado pelo proprietário como autorizado em decisão anterior, independente e não executado aqui. Reconciliar o binding exato no procedimento próprio antes da eventual leitura manual; nenhum efeito Q4, disposição, reserva liberada ou nova tentativa decorre deste prompt.
