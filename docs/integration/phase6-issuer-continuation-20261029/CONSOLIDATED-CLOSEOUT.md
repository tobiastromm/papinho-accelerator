# Phase 6 — continuação do issuer (2026-10-29)

## Implementação efetiva

O core privado agora admite CAS pendente por turnos serializados do mesmo owner. Conserva cópias do contexto esperado e dos registros anterior/próximo, além dos empréstimos de CSR, DER e saída. O port poll consulta somente a transação já iniciada: não reinicia CAS nem assinatura. Não há novo worker, thread, serviço ou autoridade.

O marcador SIGN_REQUESTED precisa ser confirmado por releitura antes da assinatura única. O CSR é revalidado na continuação. A lease permanece retida enquanto o port utiliza DER. Conclusão exige releitura exata do registro e dos contadores; incerteza conserva retenção e bloqueio. Stop não autoriza publicação. Revogação NORMAL só chama o fanout depois do commit confirmado; isso não prova fechamento nativo. Recuperação que registra RECONCILIATION_REQUIRED continua UNCERTAIN mesmo após conclusão assíncrona.

O chamador deve conservar CSR/saída e contexto vivos, sem modificação, enquanto houver empréstimo. O core não fornece sincronização entre threads: permanece sob o owner serializado existente. Os tamanhos reais são impressos pelo teste; o crescimento da reserva não equivale a capacidade produtiva concedida.

## Limite da integração nativa

A revisão automática rejeitou a alteração de issuer_root_port_cas para a rota live e seu polling por considerar que poderia habilitar publicação CURRENT fora da autorização. A alteração não foi aplicada nem contornada. A identidade por hash do arquivo nativo foi preservada contra a preimagem desta continuação.

Consequentemente, o produtor NORMAL nativo, a ligação begin/poll/publicação e o fanout nativo continuam incompletos. O core qualificado não constitui prova de integração positiva do root. Os testes usam modelo explícito; assinatura e publicação do modelo não são operações autoritativas.

## Evidências e preservação

BASELINE.json e preimages preservam o WIP de entrada. SOURCE-CHANGES.json registra os hashes antes/depois; QUALIFICATION.json registra os resultados finais e logs. Builds Selected/Legacy Debug/Release e sete CTests afetados por configuração são a campanha pertinente. Logs preliminares permanecem, inclusive a invocação de cmd com caminho incorreto; o resultado final usa caminhos absolutos.

Nenhuma operação TPM, ownerAuth, Q4, serviço real, rede operacional, root/CURRENT produtivos, SCM/ACL, assinatura autoritativa ou publicação PST foi executada.

## Gates independentes

E8 continua SOURCE_ABSENT onde faltam aquisições factuais; não foi criada fábrica de confiança por booleano/hash. Persistência ENROLLMENT depende da representação versionada exigida por L2. D4/D5 continuam UNKNOWN nos valores sem fundamento. Browser não foi alterado. E17/E18, E19/E20, E21 e E22 não ganham PASS a partir desta campanha de modelo. ED11 e Phase 6 permanecem PARTIAL, com autoridades produtivas DENY.

## Próxima decisão concreta

É necessária aprovação específica para implementar e qualificar offline a ligação nativa de código CAS → prepare_live/begin/poll/publish_live, mantendo proibida qualquer execução produtiva. A revisão automática bloqueou essa ligação nesta sessão. Antes de implementação dependente, confirmar escopo de código e revalidar a semântica dos epochs A/P, sem confundir revisão, publicação e autoridade. Não há justificativa para retomar TPM ou repetir as qualificações anteriores.
