# Revisão consolidada pessoal/local

Task PACC-PHASE6-PERSONAL-FIRST-INSTALL-AUTHORIZATION-CLOSURE-1, 2026-10-22.
main / HEAD 913c90cf2328f7939d8f5b927640601903ac6e91.

O owner decidiu expressamente que PERSONAL_LOCAL é o escopo atual e INSTITUTIONAL é futuro fora de escopo. Essa decisão resolve a escolha de origem que não estava congelada nas propostas anteriores. Não tratar os STOPs antigos nem suas exigências propostas de fonte externa como norma accepted concorrente.

| Obrigação | Fechamento na revisão consolidada |
|---|---|
| P1: autoridade fora do candidato, administração da mesma root | F1: política previamente aceita, contexto/direitos nativos + declaração atribuída de legitimidade + confirmação do plano + gates; privilégio ou flag isolados não bastam |
| Limite de prova de legitimidade | F1/F2: OS não prova propriedade jurídica/presença humana; modelo confia na administração legítima do host e na ação atribuída, sem alegar fencing ou prova global |
| Origem sem instituição/CA prévia | F0/F1: ato local sob política anterior, independente da CA/root candidatas; sem AD/nuvem/Configuration Manager ou serviço obrigatório |
| Fonte versus adapter versus decisão | F2/F3: OS atesta contexto/identidade/direitos; humano declara/confirma; adapter associa plano/fatos protegidos; Accelerator decide admissibilidade cumulativa |
| Configuração exata e substituição | F3: plano imutável, referências/revisões protegidas, apresentação e confirmação específica, parent retido/nome antes do filho, revalidação imediatamente antes do efeito |
| Identificador sem aprovação externa fictícia | F2/F4: ID local aleatório32bytes/CSPRNG existente, não licença/serial CA/root; aprovação externa não aplicável; nonce isolado não autoriza |
| Revogação e consumo | F5/F6: transição indivisível antes de submissão, uma tentativa, invalidação local por evento, UNKNOWN nega/retem e não rearma |
| Stop/restart/Model E/ED08 | F6: autorização inválida não libera borrows; quiescência/fechamento factuais, sem timeout destrutivo nem restauração por arquivo/log/restart |
| CA inicial separada | F7: CA_INITIALIZE explícito e confirmado como ato distinto; mesma interação pode nomear ambos, mas NEW_INSTALL não concede CA implicitamente; issuer cláusula6 intacta |
| Transação e CURRENT | F7: efeitos nativos separados, sem promessa de rollback global; não publicar estado parcial, formatos/histórico/CAS/ack e gates próprios preservados |
| Portabilidade e ED11 | F8/F9: backend Windows candidato, núcleo sem SID/CNG/Win32, box/ISA não qualificados; orçamento completo pré-patch, UNKNOWN não é zero |

P1 comporta o ato local composto: seu próprio texto distingue identidade/contexto OS de ação atribuída e exige administração legítima da mesma root e direitos sobre parent. A revisão especializa explicitamente o mecanismo pessoal, não altera seus bytes e não reclassifica declaração como observação nativa. A premissa de confiança no administrador legítimo do host é declarada, não provada por legalidade/posse física/elevação. Uma organização futura poderá exigir provas adicionais; não foi aceita sua fonte por referência.

As propostas F2/F5 e F2/F3 anteriores permanecem históricas, integralmente PROPOSED. O novo texto consolida as separações úteis e remove a obrigação não aceita de prova institucional para o perfil pessoal. Não aplicar patch textual a P1 ou aos formatos. O registro accepted deve incorporar somente F0–F9 consolidados byte-exatos, com hash do corpo incorporado, depois da aprovação oficial.

A norma pode estar completa sem o produto estar qualificado: atores/targets reais, UI/CLI de confirmação, fonte/material, atomização dos estados, CA, persistência e ED11 ainda exigem implementação/provas específicas. Não há fonte positiva ou cerimônia executada nesta tarefa. Nenhuma necessidade de reabrir contratos v3/v4/CA/wire ou criar nova frente normativa foi demonstrada.

Nenhum build, teste, bootstrap, rede, chave, certificado ou CURRENT executado. Quotas, produção0.6.4, PST/Browser/PACC wire e LABs preservados. Censos históricos não foram repetidos nem transformados em PASS operacional. ED11 PARTIAL.
