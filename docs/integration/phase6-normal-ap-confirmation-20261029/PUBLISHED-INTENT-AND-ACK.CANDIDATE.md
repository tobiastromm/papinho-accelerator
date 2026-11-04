# Confirmação PUBLISHED — extensão focal da decisão issuer/C2

**Status: PROPOSED. Não aceita, não implementa novo formato, não autoriza operação.**

Esta é uma proposta focal de representação no mesmo codec/root. Não é outra família de ADRs nem revisão automática de normas accepted. Normas: DECISION-issuer-c2-epoch-authority-relation-20261019 P1.1/P1.2/P3; M3/M7 accepted; issuer/custódia e Model E/ED08.

## Distinção obrigatória

R é a revisão CURRENT, I é revisão de conteúdo issuer, A é o contexto de publicação administrativa confirmado pela root após commit e P é o contexto operacional de policy. Commit não é “A”, e P não significa publicação. Nenhum deles pode ser derivado de outro. A view privada foi corrigida para carregá-los separadamente; isso não cria uma representação persistida de ack.

## Problema demonstrado

PACIS601/v1 exige published_epoch não zero no registro PUBLISHED antes de append/commit. A só nasce depois do commit na função nativa de publicação. O teste do codec rejeita o valor ausente, preservando a imagem. P, revisão, PID, hash ou um número futuro não suprem A. O link ROOT07 atual confirma uma publicação de conteúdo, mas não vincula sozinho um ato específico de publicar a credencial. Não é suficiente promover automaticamente todo ISSUED_UNPUBLISHED com checkpoint.

## Decisão focal recomendada

Autorizar a preparação de uma representação explicitamente versionada de **intenção de publicar + ack original pós-commit**, na mesma root e no mesmo worker. Antes de aplicá-la ao codec canônico, o patch deve apresentar os bytes exatos, discriminadores de versão e regras de compatibilidade exigidos por P1.2. Esta proposta não atribui novos números de wire, offsets ou tags de BODY/ROOT por inferência.

Semântica requerida:

1. PREPARE captura o ato administrativo autorizado, request/correlação, identidade de credencial e geração CA, CSR/serial/high-water, predecessor R/I, policy e P efetivos, objeto original e recursos retidos. A intenção não é PUBLISHED nem concede admissão.
2. VALIDATE/CAS confirmam os participantes e o conteúdo integral. O commit pode persistir a intenção sem publicar a credencial. Não reservar outro serial nem repetir assinatura para confirmar o resultado.
3. Após o commit, a mesma root seleciona CURRENT e emite A. A/P permanecem distintos. Nenhum A é concedido antes do commit.
4. O ack específico referencia a intenção exata e os bytes originais, R/I/predecessor, A/P, request, certificado/serial/high-water e geração. Ele é produzido e persistido pelo mecanismo governado da mesma root, com identidade dos objetos e CAS; hash isolado não é autoridade. Não criar um segundo ledger.
5. O consumer só projeta PUBLISHED após verificar o ack original, sua vinculação à seleção atual e uma releitura consistente. Registro de intenção, staging, commit e assinatura retornada continuam insuficientes. A record/view lógica expõe o A confirmado; nunca copia P para published_epoch.
6. A persistência do ack pode usar a rota de checkpoint do mesmo worker, desde que a revisão exata demonstre que não exige nova seleção/epoch para confirmar a própria confirmação. Não criar uma recursão infinita de “ack do ack”. O formato atual ROOT07 não recebe essa semântica por reinterpretação.
7. Versão desconhecida, ack ausente/substituído/duplicado, P/R/I divergentes, conteúdo não correspondente, Stop e fechamento incerto negam o efeito e preservam intenção, serial, pins, leases e UNKNOWN. Recuperação continua independente e negada sem suas próprias provas.
8. PACIS601/ROOT06/ROOT07 históricos não são reescritos, migrados ou “corrigidos” automaticamente. Leitura de DATA histórica não restaura autoridade. Perfil novo só é aceito por dispatch versionado explícito, após revisão e decisão válida.

## Alternativa e consequência

Manter a representação atual mantém segurança e DENY, mas não permite concluir PUBLISHED nativo completo. Alterar v1 para aceitar zero sem uma intenção/ack discriminados, preencher valor futuro ou tratar ROOT07 como ato de credencial são alternativas rejeitadas por não demonstrar a autoridade requerida.

## Critérios de qualificação da revisão

Intenção sem commit, commit sem seleção, seleção sem ack, ack atrasado, ack de outra credencial/geração/root/objeto, replay, conflito entre instâncias, perda do writer, Stop, crash antes/depois de cada efeito, persistência interrompida e close incerto. Confirmar preservação de P em ISSUER_ONLY, snapshots anteriores, assinatura única e ausência de admissão positiva antes do resultado final. Separar modelos, arquivos nativos de fixture e operações reais.

## Decisão ainda necessária

O proprietário pode aprovar esta direção focal e exigir o patch versionado exato, ou manter PUBLISHED/DENY. Isso não aprova automaticamente os bytes futuros nem operações protegidas. A proposta não resolve a aquisição original CA/key/custódia e não autoriza sua fabricação. Não se pede novamente M3/M7 ou o CAS já aprovado.
