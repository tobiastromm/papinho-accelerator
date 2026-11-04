# Phase 6 — fontes NORMAL, status CURRENT e fanout M7 (2026-10-29)

## Resultado

**NORMAL completo: NOT_DEMONSTRATED / DENY.** A primeira fonte original CA/key/cert/generation para o owner live não está disponível, e a representação da confirmação PUBLISHED A/P continua incompleta. Nenhuma prova foi fabricada. Não se declarou produção disponível nem se alterou o formato para obter PASS.

O trabalho independente autorizado avançou em código: status exato da credencial a partir do snapshot CURRENT e port M7 concreto para os owners existentes. A parte de efeitos foi exercitada em memória isolada; a rota nativa positiva completa permanece não demonstrada, pois exige uma revogação canônica PUBLISHED/REVOKED válida. Não é um callback que simplesmente retorna OK.

## Fontes e confirmação PUBLISHED

[SOURCE-BINDING-FRONTIER.md](SOURCE-BINDING-FRONTIER.md) identifica as fontes concretas existentes e a primeira aquisição faltante. A borrow CA disponível pertence ao pacote da cerimônia; sua função bind rejeita published. O backend CNG valida a key que já recebeu, mas não é produtor de original-history/prefix/writer. O handle exclusivo de uma transação não prova todo o lifetime de emissão. Os fixtures de arquivos não foram promovidos a autoridade C8.

[PUBLISHED-ACK-FRONTIER.md](PUBLISHED-ACK-FRONTIER.md) mostra a diferença exata de fases: PACIS601/v1 exige published_epoch não zero antes do append, enquanto A nasce depois do commit na root. O novo teste rejeita ack ausente sem modificar bytes. O core ainda possui a comparação de epoch que não distingue A/P para esse efeito; publish=1 e o guard nativo PUBLISHED/REVOKED continuam negados. ROOT07 não constitui sozinho um ato específico de publicação de credencial. Não houve revisão de ADR accepted nem nova família de ADRs.

## Implementação nova

### Consulta de identidade/status

- `papacc_issuer_image_credential`: consulta validada de DATA pela SHA256 **completa** do certificado. Retorna o último estado do request; não retorna um evento PUBLISHED antigo se o último é REVOKED.
- O mesmo digest em requests distintos retorna CONFLICT. Ausência, digest zero, imagem inválida e limites divergentes não produzem seleção permissiva. Nenhum match por Principal ou prefixo truncado foi criado.
- `papacc_issuer_credential_snapshot_data`: scan limitado, sem hash/native callback, de DATA. Não concede integridade/CURRENT por si. A rota nativa só o usa no snapshot imutável já validado antes do commit e selecionado pelo owner, sob lock e guard de R/A/P/projeção/source/ticket. Isso evita introduzir BCrypt/observador nativo no dispatch.
- `issuer_root_credential_status`: faz o vínculo com o CURRENT original, recusa prepared/pending/Stop/recovery/authority_lost e preserva os snapshots. Não produz flags NORMAL nem uma lease CA.

### Fronteiras de admissão

`papacc_server_launcher_credential_admit` aceita somente candidate original do secure owner canônico, com identidade preservada após AuthN/AuthZ, contexto válido e status PUBLISHED encontrado no CURRENT. Foi conectado antes de secure_publish nos adapters API2/API3, na adoção C2 e na verificação de Session corrente. Os gates de certificado, Principal, grants, policy, epoch e lifetime existentes permanecem. Ausência de registro/ack/status nega, inclusive quando o TLS e o resolver anterior haviam sido positivos. O caminho fora do owner C8 não recebe autoridade ou policy nova.

### Fanout M7

O factory nativo registra `revoke_dependents` para uma função concreta. Antes de efeito, ela exige REVOKED no registro **original do CURRENT**, comparação semântica exata de view/request/serial/digest e uma consulta independente por credencial. Depois resolve todas as Sessions inicializadas pelos pins e incarnations originais. Identidade ausente é UNKNOWN, não omissão do dependente.

O cohort exato passa a `credential_stop_cohort`, uma função do mesmo owner sem estado/serviço paralelo. Usa `papacc_c2_session_revoke` e `papacc_c2_operation_revoke`, invalida elegibilidade e tokens, conserva primeira causa, borrows, grants/snapshot como memória e estados RETAINED. Não seleciona outra credencial do mesmo Principal. Marca close_requested nos candidates exatos; solicita wake pelo mecanismo existente. Wake já confirmado para o mesmo registro não é repetido. Nenhum close nativo é executado diretamente pelo port.

A receipt é embedded no launcher: request/digest/R/A/P, invalidações, pedidos Stop de operações/controles, wake e primeira falha. `quiescence_proven` e `close_proven` permanecem falsos. Falha de propagação/re-read mantém receipt, referências e authority_lost. Sucesso só significa que as solicitações e a invalidação foram registradas/revalidadas, não que os recursos fecharam. O teardown continua nos owners Model E/ED08.

## Qualificação e alcance

- **Quatro builds Selected/Legacy Debug/Release: PASS, zero warnings finais.** Toolchain, backend e pins de produção preservados.
- **40 execuções CTest: PASS** (dez por configuração). Regressões issuer/CNG/enrollment e testes não operacionais de autorização/secure owner, mais o novo cohort.
- O novo cohort exercita as funções reais de revogação com Sessions/operações **sintéticas, sem autenticação/admissão**. Demonstra preservação de outra Session do mesmo Principal, borrowers/tokens retidos, primeira causa, idempotência do deadline, falha de token, owner incorreto e registro ausente. Não demonstra um cliente real revogado.
- **32 execuções focadas: PASS**: sete modos do port nativo e codec-only por configuração. Os 28 arquivos nativos são novos e exclusivos sob build/phase6-normal-status-fanout-20261029/final; os quatro codec-only não criam arquivo. O runner rejeita sobrescrita. CAS conflitante, retenção, Stop cedo/tardio, publicação não confirmada, replay, concorrência BUSY e releitura divergente continuam válidos no escopo do fixture de metadados.
- Codec: último estado da credencial, duplicidade/ambiguidade, digest ausente/zero, tamanho malformado, falta de ack e imagem inalterada na rejeição. Hash nativo SHA256; certificados/DER são DATA sintética.
- Não foi repetida toda a campanha anterior de 68 fixtures, nem executado mTLS real, CA/key persistente ou assinatura real. As 207 verificações do core e o backend CNG continuam testes/modelos nos alcances já declarados; não provam fontes NORMAL nativas.

| Caso pedido | Evidência / limite |
|---|---|
| Producer disponível | Status DATA e snapshot CURRENT implementados; produtor NORMAL completo não disponível. |
| Producer ausente | Init NORMAL/RECOVERY do port sem provas permanece negado no fixture. |
| CA divergente | Consumidores CNG/regressões em modelo; fonte CA live real ausente. |
| História/writer divergentes | Conflito e re-read nativos qualificados; isso não é prova de original-history/exclusividade de emissão. |
| PENDING/ack ausente | Worker e core pendentes preservados; codec rejeita PUBLISHED sem ack. |
| Stop/replay/concorrência/fechamento | Componentes e modelo nos escopos acima; nenhuma quiescência global inferida. |

## Recursos e preservação

O censo desta compilação está em `*-layout.log`: Selected known managed 1971888 / limite 3145728; Legacy 1803320 / limite 2097152. O acréscimo embedded da receipt é 104 bytes. Não se aumentou quota nem se concedeu crédito. Provider/kernel continuam UNKNOWN; esses valores não são capacidade/headroom operacional. Não se confundem com o limite Browser.

Nenhuma operação foi dirigida a root/CURRENT operacional, ProgramData, SCM/ACL, TPM/NV/Q4 ou ownerAuth. Não houve serviço, rede operacional, emissão produtiva, assinatura, limpeza/refund ou recuperação autoritativa. HEAD e WIP históricos preservados; PST 0.6.4 inalterado. Os novos fontes estão separados das preimagens no manifesto/delta desta tarefa.

## Estado e próximo gate real

O CAS não foi reimplementado. M7 ganhou fonte de status, negação e port de fanout reais, mas a integração nativa positiva não foi falsamente declarada PASS. A cadeia NORMAL completa ainda exige:

1. aquisição original CA/key/cert/generation e transferência de custódia/lifetime ao owner live, sem promover a cerimônia/fixture a autoridade por flags;
2. definição/revisão focal da representação do ato e ack PUBLISHED pós-commit, preservando A/P distintos e bytes históricos;
3. qualificação positiva com essas fontes, então receipts/finalização M7 reais.

Não é necessário repetir a aprovação M3/M7, perguntar por ownerAuth, criar uma root fictícia ou voltar ao TPM. Não há autorização inferida para materializar objetos protegidos que faltam. E8, L2, D4/D5, E17–E22 e Phase 6 permanecem nos estados anteriormente demonstrados, sem promoção global. ED11/Phase 6: PARTIAL; autoridades produtivas DENY.

[Próximo prompt concreto](NEXT-ORIGINAL-SOURCES-AND-ACK.md). Resultados/hashes: QUALIFICATION.json, SOURCE-CHANGES.json e ARTIFACT-HASHES.json.
