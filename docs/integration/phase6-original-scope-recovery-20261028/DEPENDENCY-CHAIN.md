# Dependências e limites da expansão ED11

Documento derivado, sem nova norma. As fontes abaixo estão em docs/adr salvo indicação.

| Aresta | Fonte / causa factual | Perfil e necessidade | Circularidade / ação |
|---|---|---|---|
| ED11 → observação N0 | ADR-0034 §ED11 exige recursos; DECISION-ed11-single-lab-diagnostic-native-boundary-20261023 B0–B6 selecionou laboratório | N0 foi uma estratégia aprovada para medir serviço/listener desabilitado; não é a única prova possível de todos os alvos nem qualificação Browser | Não exigir N0 em cada build. Preservar evidências e limites; capacidade produto ainda deve ser provada. |
| N0 → Q0–Q9/Q4 | DECISION-n0start03-poststart-disposition-and-one-token-observation-20261025 Q1–Q9; tentativa03 realizou Start; P3 pré-Start não serve | Disposição03 e uma observação independente de token. Obrigatória a governança dos efeitos já produzidos; não obrigatória repetição do diagnóstico para instalar produto | Q2 documental03 aplicado não libera recursos físicos; Q4/Q7 consumida trouxe evidência de SID, não readiness. Não repetir diagnóstico que já respondeu ausência. |
| Q4 → readback/disposição R6–R8 | DECISION-service-virtual-user-identity-and-q4-documentary-disposition-20261025; readback requisito R7 | Efeitos10584, originais/RAW1/closure, reservas/UNKNOWN. R0–R9 identidade prospectiva só LAB fixo; não regra universal do token produto | Não renomear encerramento de processo como Stop/global quiescence. Q4 pode ser analisada independentemente, mas reservas não financiam sucessor. |
| Readback → cross-boot | Guard histórico HOST_BOOT_DENY; DECISION-q4-cross-boot-historical-readback-20261026 | Somente rota documental histórica após troca de boot. Não altera guard original, não é recuperação produtiva nem startup normal | Não atualizar pins para passar. Rota nova precisa autoridade própria; aprovação antiga consumida não serve. |
| Cross-boot → ledger durável | Mesma decisão incorpora d78c4... e exige ledger governado existente/consumo único durável | Anti-replay da autorização histórica, inclusive reinício | Arquivo marcador/ACL/hash não prova resistência a rollback. Necessidade não deriva de Mode B ou GUI. |
| Ledger → testemunha independente | DECISION-q4-l0-l7-20261026 incorpora L0–L7 | Root ledger + testemunha fora do domínio restaurável para esta rota | Root/histórico/checkpoint restaurados juntos não provam recência. Não aplicar M3 por inferência a Q4. |
| Testemunha → TPM | DECISION-q4-w0-w7-20261026 incorpora W0–W7 | Escolha normativa específica de testemunha, sem criar autoridade genérica | TPM não é exigência original universal Phase6; continua exigência da implementação histórica escolhida, sem alteração unilateral. |
| TPM → NV | W0–W7; contador persistente independente, índice candidato0x01804143 | Necessário para demonstrar continuidade durável dessa testemunha. Prova EK transitória não é contador persistente | Modelo PASS não demonstra rollback nativo. Não provisionar para fechar papel. |
| NV → ownerAuth | Perfil candidato attrs0x00040014 e autoridade da hierarquia; TC framework accepted | Autorização legítima de definição/incremento conforme perfil, não segredo requerido pelo Browser para rede/TLS | Não tentar auth vazio ou extração. Sem custódia efetiva, DENY operações NV. |
| ownerAuth → STORAGE_20 | DECISION-q4-storage20-source-designation-20261028 + custodian-designation | Fonte Windows designada para possível entrega controlada em memória ao owner Q4 | Metadado type13 retorno0 não é valor/validade/custódia. Designações não concedem acesso nem provisionamento. |
| STORAGE_20 → binding root C8 | L0–L7/L1 e integração privada st20_root_guard / designation DATA | Vincula propósito/destinatário/consumo à root ledger da rota histórica | Root deve ser factual, não fixture. Não inverter em obrigação de criar root para Q4 antes de permitir primeira root. |
| Produto → root/CURRENT/epoch/SiteTLS | Consolidação canônica; SiteTLS authority18/10; produção provisioning18/10; MVP M0–M11 | Requisito próprio de configuração/issuer/grants/catálogo e consistência, não herdado da TPM | Separar existência/publicação da prova de recuperação. Auditoria locator não demonstrou inexistência e não autoriza bootstrap. |

## Classificação de bloqueios

**Obrigação de entrega**: recursos/identidade/ownership/Stop/ED08, C2/C3 real, ambos provisionamentos, perfis e alvos aplicáveis. **Obrigação histórica**: efeitos N0/Q4 e reservas retidas. **Escolha de mecanismo histórica**: cross-boot+ledger+TPM/NV. **Ainda não demonstrado**: disponibilidade de root operacional, custódia efetiva, geração física e atribuição de recursos ativos.

E21 da matriz de 22 itens é um gate composto de budgets independentes/efeitos históricos/readback. Não é a especificação inteira ED11 da ADR-0034. Separar sua aplicabilidade não o fecha ou torna opcional. Se a compatibilidade de uma admissão futura depender de recursos históricos indisponíveis, ela continua DENY até prova/decisão própria. Não afirmar independência financeira/operacional só porque tarefas offline são paralelizáveis.

## Correção da recomendação anterior

Os relatórios C8-ROOT-METADATA-RECONCILIATION e OWNERAUTH-ROOT-DISCOVERY preservam fatos válidos de descoberta. Sua sugestão de primeira instalação é condicional, não o próximo passo obrigatório de todo ED11. O trabalho imediato passa ao pacote offline de caller/contrato/quantitativos e jornadas do produto. Não se pede criar uma root agora e não se inicia nova série de ADRs. Decisões accepted permanecem intactas.
