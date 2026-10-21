# PROPOSED — reconciliação focal ADR-0009

A decisão accepted permanece intacta. Esta proposta resolve a representação de destino; não corrige o código nem qualifica listeners. GOVERNANCE CONFLICT F01 permanece aberto na implementação: ADR-0009, Decisão, exige coleção; regras 1/7/8 exigem perfil explícito, opt-in Legacy e nenhuma conversão por ausência. `server_config.h.api3.inc` mantém control_port/bind_selection/listener_profile singulares e initializer Legacy; `papacc_server_cli_scan` não exige has_listener_profile no sucesso. API2 também deve ser preservada sem troca silenciosa de perfil. A admissão candidata negada não elimina a divergência nem prova exposição operacional.

## Representação normalizada de destino

SharedConfiguration contém listeners[], inclusive para uma instalação com um só listener. Cada elemento contém listener_id administrativo estável e único no escopo da root, bind_selection, port, transport_profile explícito, security_ref quando Secure, admission_mode, authentication_ref e administrative_policy_ref. Os três últimos compõem o contexto administrativo ortogonal; não são novos campos wire nem concedem autoridade. Listener_id é um rótulo de configuração, não Session ID, Principal, socket, epoch ou prova de identidade de processo. A associação durable ao contexto da root deve ser versionada no modelo/persistência antes de aplicação.

Bind ALL ou SELECTED é explícito. SELECTED resolve identidades persistentes factuais, não nomes/indexes transitórios. Uma interface pode produzir vários endereços/sockets IPv4/IPv6; todos continuam pertencendo ao MESMO listener lógico. Security e policy refs têm conteúdo/revisão/proveniência resolvidos e leases protegidos, nunca apenas environment mutável sob ref idêntica. Ausência de resolver/material necessário impede PREPARE/RUN.

`SECURE_PRINCIPAL` exige TLS1.3, mTLS e ALPN papacc/1 segundo os contratos atuais. `LEGACY_ENDPOINT` exige a linha explícita `transport LEGACY_ENDPOINT` em seu próprio bloco e uma operação administrativa autorizada que confirme esse diff. Não há default global ou herança de perfil; perfil ausente é erro, inclusive em migração. Não acrescentar um segundo booleano que possa contradizer o perfil. Nenhuma falha Secure seleciona Legacy. Legacy Network Egress v1 continua proibido.

Cada perfil é imutável no lifetime; mudança propõe recriação explícita com Stop/quiescência/retenção e admissão normal. Detectar conflitos entre endpoints efetivos, inclusive wildcard/seleção IPv4/IPv6 e portas sobrepostas; não verificar só igualdade textual de bind. Não destruir listener antigo ou slot pinned para acomodar o novo. Capacidade insuficiente rejeita o candidate antes de efeitos.

## Compatibilidade e gate mínimo

Um primeiro adapter pode suportar apenas um listener lógico, mas precisa anunciar essa capacidade e rejeitar explicitamente uma coleção maior, sem truncar. Não congelar cardinalidade 1 como arquitetura. Preservar comportamento/linkage produtivo 0.6.4 até migração autorizada; configuração antiga sem profile necessita resolução administrativa explícita, jamais conversão automática para Legacy ou Secure. Separar suporte de sintaxe, capacidade do target e permissão de executar.

Antes de código: aceitar contrato de source/representação e definir codec privado versionado para a coleção/associações na mesma root, preservando BODY históricos. Não reutilizar bytes livres nem promover esta proposta a ADR accepted. Testes futuros: perfil ausente/unknown, Legacy explícito, Secure sem security_ref, refs divergentes, endpoints conflitantes, vários sockets do mesmo listener, coleção acima do target, troca de perfil e retenção.
