# Decisão específica sobre integração de código nativo — não operacional

Status: **PENDING_EXPLICIT_CODE_SCOPE**, não aprovação normativa nem pedido operacional executável.

O caminho modelado NEW_INSTALL/CA_INITIALIZE e o caminho borrow-only EXISTING_CURRENT estão qualificados offline. Não precisam de CA anterior ou outra aceitação de M3. A revisão automática rejeitou adicionar ao produto uma chamada a `papacc_ca_key_create`, pois ela pode criar uma key persistente real; recomendou fixture isolado, que foi utilizado.

A decisão reservada é autorizar especificamente a **integração de código** dessa entrada nativa, continuando proibida sua execução. O futuro patch deverá ser concreto e revisado antes de aplicação, nos owners atuais, sem serviço/autoridade/root paralelos.

Escopo mínimo da eventual integração:

- consumir somente o plano original explicitamente confirmado NEW_INSTALL + CA_INITIALIZE, sem CA prévia e sem rearmar consumo;
- preservar referências de operador/token/parent/filho/configuração e observações nativas, em vez de aceitar um bool de fixture;
- manter key owner existente, criação exclusiva sem overwrite, finalidade, geração e seleção de provider governadas;
- registrar tentativa e primeira falha antes do efeito, manter referências após resultado parcial/incerto e impedir retry;
- separar key, TBS, assinatura, reserva/persistência, publicação, CURRENT e custódia; não tratar COMMIT=DENY/CURRENT=DENY como concessão;
- manter admissão DENY enquanto fontes, recursos e contratos não estiverem demonstrados;
- qualificar com APIs substituídas apenas em teste e negativos nativos sem criação; nenhuma key/certificado real ou efeito produtivo;
- preservar EXISTING_CURRENT sem fallback para criar ou restaurar autorização antiga.

Não há autorização operacional incluída, hashes futuros inventados ou material secreto. A execução real precisará de pedido e decisão próprios, após artefatos e bindings completos. Não se solicita nova família de ADRs; eventual incompatibilidade de representação deverá citar a cláusula específica antes de alterar formato.
