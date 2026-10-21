---
status: accepted
revision: 1
decision-date: 2026-10-20
scope-level: project
scope-target: PapinhoAccelerator
supersedes: []
decision-makers: [Tobias Tromm]
---

# Listener específico de enrollment — especialização delimitada ADR-0009

Registro normativo designado somente da decisão arquitetural explícita do owner na tarefa PACC-PHASE6-ENROLLMENT-LISTENER-ADR0009-SPECIALIZATION-1, OWNER_DECISION=SEPARATE_ENROLLMENT_LISTENER_WITH_IMMUTABLE_PREPUBLICATION_PROFILE. A escrita deste registro depende da revisão oficial do executor. Não aceita parâmetros novos por inferência.

## L0 — Endpoint separado na mesma administração

Adotar endpoint/listener específico para enrollment inicial, administrado pela única root e representado na coleção canônica de listeners. Não multiplexar autenticação server-only e mTLS operacional no mesmo listener nesta fase. Não criar root, cadastro, registry, journal, worker ou autoridade paralelos. Endpoints efetivos devem ser distintos e ter conflitos de bind rejeitados; porta/binding são escolhas administrativas explícitas, nunca default ou migração da produção.

## L1 — Perfil antes da publicação

Todo listener possui identidade administrativa e perfil explícitos, fixados antes da publicação e imutáveis durante seu lifetime, conforme ADR-0009. Mudança exige reconfiguração/recriação governada. Não selecionar política de autenticação por sniffing de payload, falha TLS, certificado ausente, SNI ou ALPN oferecido no listener operacional. Coleção não pode ser truncada ou convertida para o runtime singular; capacidade ainda insuficiente nega ativação.

## L2 — Perfis distintos

OPERATIONAL conserva o perfil SECURE_PRINCIPAL: TLS1.3, mTLS obrigatório, ALPN papacc/1 e admissão operacional governada. ENROLLMENT é o propósito restrito do listener específico: TLS1.3, autenticação obrigatória do servidor, sem exigir certificado de cliente antes do provisionamento, perfil de aplicação e ALPN exclusivos sujeitos a contrato exato. Não há fallback entre listeners/perfis. Falha em qualquer verificação encerra a tentativa e não redireciona para operação normal ou Legacy.

ENROLLMENT é identidade semântica administrativa, não valor de enum/codec/wire alocado por este registro. Não reinterpretar SECURE_PRINCIPAL ou LEGACY_ENDPOINT como enrollment. Ausência de representação versionada aceita deve negar persistência/aplicação até extensão específica; não modificar BODY0.5, ROOT06, D3/LST1 ou ROOT08/v2/v3 por inferência.

## L3 — Isolamento de autoridade

Enrollment não concede CONTROL Session, DATA association, grants, capabilities, Network Egress ou admissão operacional. Não reutilizar Site TLS como fonte de transporte/trust de bootstrap. Código, OPEN/CLOSED, aprovação automática/manual, CSR/PoP e autorização de emissão permanecem decisões independentes. Chave privada automática permanece exclusivamente no Browser; somente CSR/PoP público é transferido após pareamento verificado/autorizado. Issuer/serial/CURRENT/policy continuam regidos por seus contratos próprios.

## L4 — Recursos e lifecycle

Ambos os listeners compartilham o orçamento por processo/creditos e mecanismos administrativos existentes. Nenhuma quota individual ou de outro target é aumentada. Windows3MiB somente explícito, demais perfis inalterados. Recursos e callbacks retidos, conexão/owner/geração, snapshots/pins, trust/credential leases e workspaces permanecem até quiescência/fechamento factual sob Model E/ED08; Stop/timeout/saída de processo não autorizam destruição ou reutilização. Falta de reserva/identidade/autoridade nega novos efeitos.

## L5 — Precedência exata e limites

Este registro especializa somente a lista de perfis iniciais/regras correlatas do ADR-0009 e a aplicação do mTLS obrigatório do ADR-0006: a exceção de certificado cliente ausente aplica-se exclusivamente ao NOVO listener ENROLLMENT; o Secure Principal continua integralmente mTLS. Especializa também a preferência de listener único da decisão canônica de18/10, cláusula4, e a proibição anterior de segundo listener em T2 PROPOSED, unicamente quanto ao endpoint de enrollment explicitamente autorizado pelo owner nesta tarefa. Não substitui ADRs inteiros nem altera seus bytes.

P0–P9 e T0–T7 permanecem PROPOSED. ALPN, framing, mensagens e limites de T2/T6 nesta tarefa são propostas novas, não aceitas por este registro. A arquitetura aceita não implica que todos os contratos concretos estejam aceitos, implementáveis ou qualificados. Originais preservados; não aceitar patches funcionais por referência.

IMPLEMENTATION_AUTHORIZED=NO
AUTOMATIC_ENROLLMENT=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
