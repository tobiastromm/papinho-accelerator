# Conformidade e gate de implementação

Não é conflito entre ADRs accepted. É escopo de autorização nativa ainda não definido para o produtor UPDATE. Seção3 da tarefa autoriza proposta focal quando necessária; AGENTS.md exige decisão durável registrada antes de código dependente. Nenhuma decisão nova foi aceita e nenhum hunk funcional preparado/aplicado por inferência.

| Obrigação accepted | Evidência de código auditada | Correção focal proposta |
|---|---|---|
| D2.2/D4, G4: candidata não é CURRENT, ato/fonte antes de RESOLVED | update_hold preserva pin/bytes; update_validate termina NOT_SUPPORTED | U0/U2/U6: ato para candidata exata, sem promover snapshot ou codec |
| S1-S5, F1/G3: autorização explícita da tupla e disposição | PDG representa disposição; CLI config valida texto e nega apply; não produz aprovação | U1/U2: autorizar captura explícita local sob token factual; não usar arquivo/flag como autoridade |
| Origem administrativa e revisão | bootstrap_auth_native guarda SID/TokenStatistics, root/candidate/container, mas só no modo inicial sem CURRENT | U1/U3: escopo UPDATE separado, sem chamar bootstrap_authority_valid ou remover guards |
| C8/root e segurança | c8_policy_adapter_authorized consulta Session/Principal/epoch/role; não é produtor do ato administrativo candidato | U0/U1: não converter autorização de operação C8 em aprovação de SNT/PDG |
| F2/G3: certificado e delegação têm status separados | issuer_core tem QUERY, estados PUBLISHED/REVOKED e backend.utc; codecs retornam DATA sem epoch/proofs | U4: derivar observação da fonte factual protegida; não inventar status, UTC ou prova de recovery |
| G7/E0: lifetime sem pin fictício | G7 protege snapshots publicados; candidato serial0 pertence ao cohort; kind8 exige endereços exatos sob pin publicado | U3/U5: contexto no mesmo owner e acesso por ownership candidato, sem usar pin antigo nos bytes novos |
| Model E/Stop | Abort do cohort conserva grafo quando falta quiescência; bootstrap pending.previous contém prova exclusiva de seu modo | U3/U5: não sobrescrever previous/predecessor/source; Stop invalida aplicabilidade e mantém memória/crédito |
| F3/F4/E0: Browser independente; SDK sem autoridade | Nenhuma fonte local Browser pode ser produzida pela root | U6: sem confirmação Browser, E0/TRUST/KDF seguem negados |
| ED11 | Censo anterior Selected1948656/Legacy1790872; composição completa3747601 | U7: delta atual0; dimensionamento da futura prova/caller/token/arenas antes de hunks, sem reserva presumida |

## Menor implementação após eventual aceitação

Reutilizar o ponto de entrada administrativo/owner/cohort existente para um ato explicitamente distinto de validate. Capturar identidade e correlação antes da exposição; consumir issuer/policy/UTC por fontes factuais já protegidas e revalidar no mesmo owner. Não criar segundo resolvedor, port de trust booleano, arquivo de aprovações ou fornecedor de usuários. A CLI atual roda antes da construção da root: um novo verbo não pode declarar sucesso enquanto não houver operação administrativa canônica ligada ao owner e CURRENT factual. Assinatura/sintaxe de comando, tipos privados e hunks exigirão revisão funcional exata; não são congelados por este handoff.

Se não for aceito que o administrador local elevado possa praticar o ato UPDATE definido em U1/U2, o produtor deve continuar negado; não há mecanismo alternativo factual de papel administrativo identificado. Esse é o primeiro gate normativo delimitado. Independentes disso permanecem os gates factuais CURRENT/predecessor externo, contexto nativo elevado, UTC/status qualificados e ED11 de ativação. Não classificá-los todos como ambiente.

## ED11 e testes

Nenhuma alteração executável ou nova reserva. Mantidos Selected1948656 e Legacy1790872 como censo fixo anterior, não medição nova/pico. Backings3x38872 e imagens3x30592 já incluídos; não adicionar PDG duas vezes. A futura prova não pode usar committed/previous/candidato como scratch enquanto retidos. Custos do token, handles, prova/observação, borrows/DER/caller/stack e coexistência precisam tamanho compilado e lifetime antes do patch. Provider/kernel UNKNOWN; totalPARTIAL. Windows3MiB apenas explícito, quotas intactas; composição completa excedente permanece DENY.

Builds/testes não executados nesta tarefa documental sem patch funcional. As74 suítes e4 builds do closeout anterior são evidência histórica, não reexecução nem teste positivo deste produtor. Casos de aprovação/status/revogação solicitados ficam pendentes de implementação; fixtures não suprem fonte factual. Não foram geradas chaves, certificados, pacotes, CURRENT ou rede. Browser/PST/wire/produção0.6.4 e quatro LAB não modificados.
