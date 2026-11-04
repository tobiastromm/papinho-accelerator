# Fronteira factual de aquisição

Não é nova ADR, aceitação ou autorização operacional. M3, custódia e PUBLISHED já estão aceitos nos seus escopos. A tarefa atual proíbe criar CA ou promover teste; essas restrições foram preservadas.

| Alternativa | Informação/autoridade necessária | Consequência |
|---|---|---|
| Instância original existente | Referência pública não secreta ao owner/ato/root/CA/cert/geração; aquisição pelos mecanismos originais, com identidade, custódia e continuidade verificáveis | Permite qualificar o binding real se os fatos forem demonstrados. Nome/hash/cópia de fixture não bastam. Nenhuma instância assim foi adquirida nesta tarefa. |
| Primeira instância pessoal/local ainda não materializada | Pedido próprio para a cerimônia existente M3, após bindings concretos de operador, conta operacional, alvo, recursos e efeitos; execução somente sob autorização correspondente | Pode produzir a fonte original futura. Não é recuperação, adoção de LAB, nova infraestrutura nem dispensa de gates. Criação de key/root/cert e publicação não estão autorizadas agora. |

Não existe pedido executável completo: faltam objetos/bindings factuais para fixar suas identidades e hashes. Não há aprovação a registrar por inferência. Não solicitar ownerAuth, chave privada ou segredo. Não iniciar outra auditoria de CAS/TPM ou outra família de ADRs para representar essa ausência.

O compositor preserva uma origem que já tenha sido verificada; **não é produtor de origem, de certificado original ou de exclusividade**. A factory canônica deixa proofs NORMAL ausentes enquanto esses fatos não forem fornecidos. A próxima decisão reservada é escolher a aquisição/materialização legítima; os detalhes operacionais somente podem ser finalizados com os bindings concretos.
