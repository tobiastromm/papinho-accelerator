# kind8 — revisão do dispatch e gate de lifetime

TASK=PACC-PHASE6-KIND8-CANONICAL-DISPATCH-V3-1. Revisão factual, sem nova decisão normativa.

O dispatch canônico `apps/server/server_config_root8_data_win32.c:38` aceita kinds5/6/7; kind8 permanece UNSUPPORTED. Seu contexto `server_config_root8_data_win32.h:13` fornece owner, workspace de configuração e floor, não CA pública protegida, arena X509 ou scratch DER. A busca integral de PAPACC_ROOT8_SIGNER_DATA encontra somente definição/implementação e teste; nenhum produtor canônico.

O componente público existe em `server_config_root8_signer_win32.c:9`. Seu header exige caller manter CA completa e ALL deep CA backing, input imutável e scratch/arena disjuntos até retorno, serializando Stop fora desse caminho. Copiar um CERT_INFO raso ou acrescentar um ponteiro sem owner não satisfaz isso. O retorno OK é DATA, inclusive para histórico revogado; não delegação/trust.

A cláusula exata G7 accepted, incorporada em `docs/integration/package-delegation-persistence-acceptance-20261020/G0-G7-frozen-clause-bytes.txt`, exige: “Pin de root não protege objeto externo sem cópia própria ou lease do seu owner.” O registro accepted `docs/adr/DECISION-package-signer-delegation-persistence-20261020.md`, seção Transaction, lifecycle and historical readiness, exige manter snapshots/pins/source leases/immutable input/crypto buffers e créditos até quiescência e fechamento factuais. Não há conflito entre autoridades; há uma precondição de implementação não satisfeita.

O handoff anterior, current-v3-and-e0-handoff.md item3, já condiciona ligar o callback ao dispatch a owner fornecer CA pública exata/generation, buffers, proteção transitiva de lifetime e créditos. Nesta revisão, nenhum produtor foi localizado. Não substituir essa obrigação por um campo booleano, callback de trust, digest válido ou ponteiro de teste.

STOP da integração dependente conforme seção8 da tarefa: lifetime inseguro se o dispatch fosse ligado sem esse grafo. Nenhum patch funcional aplicado ou submetido como se seguro. Não houve negativa oficial; o bloqueio é factual de ownership. kind8/codec/testes independentes existentes preservados.

## Composição e capacidades

`config_root8_v3.c:78` já encaminha kind8 através do port PC_ROOT8_COMPONENT depois da correlação issuer/PDG. `pc_root8_v3_base_link` exige predecessor integral; mudanças SNT/PDG sem histórico de aprovação continuam INCOMPLETE. Não existe permissão para eliminar issuer/PDG e chamar v2 como fallback.

`server_configuration_commit_win32.h:26` armazena slots de configuração histórica; :67 mantém snapshots issuer. Não há backing SNT/PDG/CA por revisão nesses slots. `server_configuration_root_transaction_win32.inc:148` distingue pin held de CURRENT; :181 verifica live separadamente. Pin histórico não protege automaticamente o grafo v3 externo. `issuer_root_port_read` deixa deliberadamente epoch/proofs ausentes: read OK não prova CURRENT/freshness/histórico completo.

Selected possui buffers de transação28960 bytes; v3 máximo30592. Diferença1632 por imagem. Três imagens independentes hipotéticas exigiriam4896 adicionais, além de novos backings. Não foi feito resize: isso isoladamente não resolveria o lifetime. Os formatos e os guards históricos permanecem byte-exatos.

## Menor sequência implementável posterior

1. No mesmo owner/slots existentes, definir e contabilizar backing imutável de SNT/PDG/CA/certificados por revisão; pins devem cobrir todas as dependências transitivas. Provar coexistência com committed/predecessor/candidato e slots retidos; não sobrescrever committed ou slots pinned.
2. Fornecer CA exata/geração pelo owner existente e scratch/arena já reservados, disjuntos e exclusivos até retorno síncrono. Definir Stop serializado, quiescência e liberação factual, sem bloquear Stop ou pressupor cancelamento de CryptoAPI.
3. Só então submeter hunks exatos do dispatch existente e de seus callers, verificando inicialização completa dos contextos históricos. Kind/version desconhecido nega; ausência de fonte continua INCOMPLETE/UNSUPPORTED, sem fallback.
4. Dimensionar v3 integral, integrar predecessor/CP3/R/A/P/I e CAS no mesmo worker. CURRENT positivo permanece qualificação externa, sem criar store ou publicar para fabricar prova. E0 continua borrow-only negativo até fonte completa.
5. Qualificar separadamente DATA sintético, verificações públicas nativas e autoridade operacional. Browser confirma primeiro contato/local install; root não produz essa confirmação.

Nenhuma nova autoridade, API normativa, layout, registry, journal, worker ou decisão accepted é criada por este handoff. Não se pede novamente ao owner um CURRENT que já declarou indisponível.
