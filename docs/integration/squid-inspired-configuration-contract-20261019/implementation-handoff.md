# PROPOSED — handoff mínimo de implementação

Não há autorização de código por esta tarefa. Primeiro owner revisa a reconciliação ADR0009 e a sintaxe; decisões duráveis aceitas devem ser registradas normativamente pelo processo autorizado antes de implementar. Nenhuma ADR accepted foi criada/alterada. Não aceitar automaticamente a proposta completa para um target incapaz de enforcement.

1. Reconciliar Shared Model com listeners[]/identidade/contexto e perfil explícito; adapter inicial anuncia cardinalidade suportada. Remover dependência de initializer Legacy como autoridade só em futura alteração autorizada, com migração explícita e produção0.6.4 preservada. Codec versionado na mesma root sem reinterpretação de BODY02/03/04/05/ROOT06/trailer.
2. Implementar parser portável limitado com source spans, AST candidate imutável, presença explícita e tabela fechada de diretivas/target capabilities. Não consultar/alterar runtime durante parse. Definir limites/scratch por evidência ED11, não copiar quotas do exemplo.
3. Normalizar refs e coleções para os modelos existentes; RESOLVE exige conteúdo/revisão/integridade/proveniência e leases. Não mapear automaticamente para variáveis de ambiente lab, registros fictícios ou permissões de localhost. Falta de AuthN produtiva/ligação administrativa nega a aplicação correspondente.
4. Reutilizar PREPARE/participants/reservas/pending adapter/CAS/commit/publish da única root. Nenhum arquivo novo como shadow registry. Snapshot/pins antigos mantidos; Stop não aguarda worker. Troca de listener/perfil/trust precisa transação e lifecycle comprovados, não assumir que issuer-only hot update a implementa. I/R/A/P distintos, issuer-only mantém P só se projeção idêntica; recovery sem fencing DENY.
5. Enforcement por consumidor: provar todos os paths anunciados, especialmente callbacks básicos de ModeB; não confundir policy evaluator com integração integral. Audit sem segredos precisa ator/operação/contexto/tabela/revisão/regra/motivo efetivos. Atualizar live docs/future apenas após evidência da futura implementação.

## Interface CLI proposta, não existente

`papacc-admin config validate --file papacc.conf --target TARGET`: syntax/semantic/support report; sem commit, grants ou rede.

`papacc-admin config diff --file papacc.conf --against-current`: resolve autorizado e revisão factual, mostra alterações e necessidade de recriação/retenção; sem efeito funcional. Não divulgar segredos.

`papacc-admin config apply --file papacc.conf --expected-root REF --expected-current-revision N`: ator administrativo autorizado, CAS factual, mesma transação; reporta committed/current/pending/unknown separadamente. Não aceita epoch escolhido. Exige condições de suporte/ED11 existentes; não habilita admissão nesta fase. Legacy precisa confirmação explícita do diff pelo ator, sem fallback em modo unattended.

`papacc-admin config effective`: snapshot publicado+R/A/P/I pertinentes, proveniência/redações, dependências retidas e presença; não devolve raw private material.

`papacc-admin policy rules --listener ID`: tabela e ordem efetiva, snapshot/currency e limites do target.

`papacc-admin policy diagnose --listener ID --request-ref REF`: explica fatos disponíveis e gate/motivo, sem simular identidade como autenticada, conceder grants ou executar operação. Ausência é INCOMPLETE/DENY, não sucesso.

Não definir precedência automática CLI > arquivo > Registry. Todas são sources que convergem ao mesmo modelo; conflito de fontes precisa escolha administrativa explícita, não merge permissivo.

## Regressões exigidas antes de futura aplicação

Parser: comentários integrais/inline/# quoted, desativação por ; inicial (com espaços/tab), ; em string literal e ; em outra posição inválido, bloco parcialmente desativado rejeitado, versão ausente/unknown, UTF8 inválido/NUL, quotes/escapes, overflow, duplicatas, unknownblock/field, limites e refsforward/cycle/type mismatch, exemplos desativados sem nós ativos e preservação de localização/ordem física.

Semântica: listeners sem profile/Securesemref, Legacy explícito, bind conflitos wildcard/IPv4/6, targetcapacity, suportepredicate IP/grupo/tempo rejeitado, erro inteiro sem parcial; Principal16/32 separados; primeira ACCEPT/DENY/no-match/AND e gate independente; exact hostname+endpoint semsegundaresolução.

Transação: validate/diff sem efeito, CAS conflict/unknown, failure antes/depoiscommit, CURRENT/epoch corretos, issuer-only projeção idêntica/divergente, conteúdoresolvido alterado sob refigual, oldpins/slotsesgotados, Stop/quiescência/retenção sem liberaçãoantecipada. GeraçãoCA/credenciais/rede não incluídas.

Portabilidade: core sem CNG/Win32, target sembackend rejeita semfallback; box/ISA nãoqualified. ED11: parser/strings/AST/normalizado/scratch/resolvers/leases/slotscoexistentes/retidos e worker contados uma vez; overhead desconhecido permanece pendente. Último censo disponível 1801168 known / 1972291 combinado de perfil anterior, teto2097152; NÃO é censo desta proposta nem reserva disponível para parser. Provider/kernel UNKNOWN, freshness/headroom PENDING; nenhuma margem512MiB/1000ms.

Gates Phase6 continuam: ambos provisionamentos MANUAL/AUTOMATIC, emissão/admin e inputs reais, segurança, revogação ativa com política própria, ED08/ED11, ModeC/E10/qualificação nos escopos. Grupos/calendários/backupuniversal não viram obrigações novas. Próximo gate exclusivo: owner review deste contrato proposto; não iniciar implementação automaticamente.

Reconciliação2: plano executável e estados de ACL em ../config-contract-reconciliation-2-20261019/implementation-slice.md e acl-state-matrix.md. Aprovação normativa não aplica patches funcionais; submeter operação exata antes de código.
