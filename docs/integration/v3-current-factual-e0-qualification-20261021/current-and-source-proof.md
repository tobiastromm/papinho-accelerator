# Correlação DATA e limite da fonte factual

## Matriz de conformidade

| Obrigação | Evidência implementada | Limite |
|---|---|---|
| Identidade/revisão/CP3 | Mesmo owner, pin, serial; imagem inteira committed igual ao backing; CP3 magic/versão/domínio/R/predecessor/A/P/I iguais ao slot | Correlação DATA, não prova de origem nativa |
| CURRENT | Preflight exige singleton canônico, preparação válida, Stop ausente e configuration_current existente | Fonte positiva indisponível; nunca retorna OK |
| Predecessor completo | Divergência do predecessor CP3 rejeitada | Digest/revisão não provam composição anterior completa protegida |
| G7 | Borrow canônico revalida integridade e lifetime existentes | Não cria novo owner/pin ou autoridade |
| E0–E8 | SOURCE_ABSENT permanece; acquire DENIED sem lease; RELEASE_INVALID sem efeito | Nenhum produtor/opaque concretizado |
| Stop/retenção | Novos borrows negados; borrow já protegido continua até unpin/quiescência | Nenhuma liberação por timeout |
| Compatibilidade | Leitores históricos/formatos/guards CURRENT preservados; regressões existentes | Não implementa commit v3 ou migração |
| M1 | Testes isolados de sequência e ausência da fonte | TRUST negado; KDF não autorizado |

## Código delimitado

`papacc_configuration_v3_checkpoint_match_data` somente compara imagem retida com committed e CP3 com metadados da revisão sob lock e pin. Recusa tamanhos maiores que o armazenamento existente; não trunca nem amplia buffers. Só aceita estado publicado, sem pending, prepared, committed_pending ou recovery. Mesmo OK não comprova commit nativo, CURRENT, aprovação ou frescor.

`papacc_server_launcher_configuration_v3_evidence_preflight` opera somente na root canônica e exige o CURRENT já existente. Propaga falhas DATA e termina NOT_SUPPORTED mesmo após correlação completa. Nenhum borrow ou lease sai dessa função. Nenhuma operação nativa nova ocorre dentro de acquire E0–E8.

## Handoff necessário antes de qualquer fonte positiva

1. Integrar commit v3 e publicação CP3 no worker/transação existentes, com resultado factual e CAS; não inferir sucesso de UNKNOWN. A capacidade atual de imagem é 28.960 e máximo v3 30.592; eventual ajuste de 4.896 para três imagens ainda não foi aplicado.
2. Proteger o predecessor completo v2/v3/checkpoint e seu histórico no owner existente durante PREPARE→RESULT. pending.previous mutável não é prova de lifetime por si só. Separar captura de candidato de consumo de snapshot publicado sem inventar CURRENT candidato.
3. Obter proveniência administrativa de delegação, disposição/revogação/validade e revisão policy/issuer sob pins coerentes. PDG decodificado, RSA e domínio hash são DATA, não autorização.
4. Executar qualificação integrada posteriormente em instalação Windows autorizada, pela cerimônia accepted e mesma root/worker. CURRENT ausente/obsoleto, Stop, UNKNOWN e ausência de evidência devem negar. Não inicializar store para suprir esta tarefa.
5. Só após essas provas preparar referências factuais já protegidas para acquire síncrono E0–E8; owner anterior conserva ownership, borrow é explícito e RELEASE_INVALID não tenta liberação. Confirmação independente e autorização local continuam no Browser, sem acesso à root privada.

Não houve nova decisão normativa, mudança de formatos ou API compartilhada. As autoridades F1–F5/G0–G7, E0–E8, M1, root/C8 e Model E permanecem limitadoras.
