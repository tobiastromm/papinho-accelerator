# Matriz factual e menor caminho de implementação

| Obrigação | Fonte existente | Limite factual | Próxima dependência |
|---|---|---|---|
| Identidade/policy | server_policy_model.h: PAPACC_ADMIN_POLICY | Identity/profile/admission/capability rules; sem delegação signer | F1 PROPOSED, depois layout exato na mesma root |
| Signer/issuer/histórico | issuer_core.h / issuer_codec.h | IDs, purpose PACKAGE, policy identity, estado/serial/DER; não aprova pacote | Consumir registro/certificado sob F2, sem repurposing |
| CURRENT/pin | configuration_pin_read / issuer_snapshot06 | APIs existem; cópia issuer é data-only e pin histórico não concede CURRENT | Adapter existente com autoridade/lifetime completos; ambiente factual ainda pendente |
| Issuer root port | issuer_root_port_read | Deixa epoch/proofs ausentes por contrato; não pode autenticar trust | Não elevar consulta a autoridade |
| Confirmação independente | CLI/config frontend | validate candidato; sem produtor da cerimônia D3.4 | F3, captura sob owner existente após revisão |
| Autorização de instalação | BrowserProfile/generation/leases | Fronteira Browser preservada, não implementada aqui | Handoff próprio; não inferir de autorização servidor |
| Pipeline | manual_package_validation_openssl.c | TRUST retorna SOURCE_ABSENT; zero KDF; CONTENT também sem contexto completo | Ligar somente após fontes factuais e orçamento |

Localizações e SHA-256 exatos: source-inventory.json. Propostas antigas de delegação não foram promovidas. O owner confirmou nesta tarefa que não há fonte implementada comprovada para as duas evidências. Trata-se de lacuna de representação/produtor além da ausência de ambiente CURRENT; não é conflito entre decisões accepted.

## Escopo implementável após gates

1. Revisar F1–F5 semanticamente, sem reabrir D3/M0–M8. Apenas após aceitação, preparar contrato byte-exato do menor acréscimo à policy/root para F1; não alterar o issuer como segunda autoridade de delegação.
2. Delimitar captura transitória F3 no owner administrativo existente e fronteira de destinatário, comprovando origem independente; CLI sozinha não é essa origem. Não adicionar callback permissivo à API pública de validação.
3. Submeter um patch exato de aquisição/revalidação/release com pins das fontes existentes, preservando ausência=>DENY. Separar fixtures de source/certificate/CURRENT nativos. Completar CONTENT contra expectativas administrativas, sem confiar em metadados criptografados como origem.
4. Censo de composição real e reserva antes da ativação; integração Browser/CLI e qualificação positiva dependem dos respectivos ambientes/autorização. Não criar store/CURRENT para satisfazer testes.

## Matriz de falhas e regressões futuras

| Caso | Resultado obrigatório |
|---|---|
| delegação ausente, ambígua, expirada, revogada ou purpose divergente | TRUST DENY, KDF0 |
| certificado revogado/incompatível ou issuer/generation/SPKI divergente | TRUST DENY, KDF0 |
| CURRENT ausente, pin inválido, snapshot obsoleto, revisão substituída | DENY; pin protege memória, não nova autoridade |
| confirmação ausente, canal não comprovado, pacote/destino/operação divergente | TRUST DENY, KDF0 |
| alteração/revogação após TRUST | DENY antes de RELEASE, limpar só após quiescência |
| Stop / falha ou incerteza de release | nenhum output; retenção factual sem refund antecipado |
| validação completa sem autorização local Browser | nenhuma instalação |
| provider/custo requerido desconhecido ou crédito insuficiente | nenhum efeito dependente |

Esses são critérios de testes futuros do binding, não novos PASS. As regressões atuais são as implementadas anteriormente e reexecutadas sem alteração; não qualificam fonte ausente.
