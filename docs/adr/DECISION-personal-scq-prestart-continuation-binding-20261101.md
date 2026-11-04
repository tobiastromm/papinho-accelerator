---
status: accepted
date: 2026-11-01
scope: PERSONAL_LOCAL QUERY/G3/SQB2 conditional pre-Start continuation, offline only
supersedes: []
---
# Decisão — ligação condicional da continuação pré-Start

O proprietário aprovou a ligação específica por resposta atribuída, texto
explícito e confirmação final da identidade byte-exata do pedido. A divergência
de transcrição do hash no texto intermediário foi explicitamente resolvida pelo
proprietário em favor do SHA256 real, terminado em f68. Conferência integral:
`9637246ad9ad6de34ff9eb5cb9440e628a14909ea7f95cb6a2ba2b7265287f68` de
[PRESTART-CONTINUATION-BINDING.REQUEST.md](../integration/phase6-sqb2-deadline-20261101/PRESTART-CONTINUATION-BINDING.REQUEST.md).
Aceita-se exclusivamente seu corpo por incorporação, antes de código dependente.
O original CANDIDATE, as rejeições e a divergência histórica ficam preservados.

Manter initial_receiver_source_unavailable=1 como inicialização. Somente após
source controladora original e resource_current originais terem sucesso, com
três atos atribuídos, identidade/imagem protegidas, G3 selecionado e deadline
congelado, reconhecer a implementação da continuação QUERY/SQB2. Limpar
DELIVERY_CONTINUATION somente com namespace original revalidado. Sem essas
provas, DENY. Não instalar flag/callback permissivo ou criar autoridade por DATA.

Não é autenticação antecipada de runtime futuro, readiness diagnóstica ou
produtiva nem prova de provisão integral. Preservar resource_pre_missing e
later_obligations, consumo único, Stop, prazos sem renovação, replay, receipt/
close originais, retenção UNKNOWN e separação N0/READ/TRANSPORT_ONLY.
Compatível com QUERY/G3, SQB2, OHI/B4/B5 e SSO-B: não dispensa nenhuma prova
posterior à criação nem inventa PID/token. Nenhum valor G3 ou formato muda.

Autoriza somente essa ligação e qualificação offline, APIs protegidas modeladas.
Não autoriza update staged, Start/Stop, endpoint real, SCM/ACL, CA/CURRENT,
rede operacional, TPM/Q4, limpeza/refund ou recuperação. Preservar HEAD/WIP,
históricos/pins, chave fixture, UNKNOWN/reservas, OS_QUIESCENCE UNKNOWN e PST0.6.4.
E21 PENDING; ED11/Phase6 PARTIAL até provas próprias.
