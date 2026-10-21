# PROPOSED: organizacao semantica completa antes da sintaxe de arquivo

Status PROPOSED, nao ADR accepted, nao parser/schema executavel, nao wire. Esta proposta e posterior aos achados/matrizes desta auditoria. Nao selecionar INI/JSON/YAML/Registry, codificacao de operadores, IDs ou calendarios por inferencia. Nao propor como produto um arquivo que so reproduza o pequeno BODY0.5.

## Contrato de source e autoridade

SOURCE -> leitura candidate -> normalizacao/validacao -> resolvers/participants/reservas -> PREPARE -> COMMIT factual na mesma root -> CURRENT -> A e associacaoP vigentes -> snapshots/pins -> enforcement atual.

Arquivo e intencao, nao segunda autoridade nem dump editavel de CURRENT/epoch/serial. Valores propostos de revision/predecessor sao precondicoes de CAS; source nunca escolhe A/P nem concede grants. O estado aplicado e consultado na root, nao inferido de arquivo gravado. Reload geral e troca de perfil/credencial exigem prova de todos participants; issuer-only ja integrado nao basta.

## Estrutura conceitual de familias (nao gramatica)

| Area | Conteudo necessario | Nivel atual / tratamento |
|---|---|---|
| Documento/candidate | identidade/versao de source, compatibilidade, expected-root/revision, integridade/proveniencia nao secreta, declaracao de capacidades exigidas | mecanismo exato de source/integridade ainda PROPOSED; nao criar autoridade de assinatura paralela |
| Interfaces e bindings | refs persistentes com presence, entrada/saida distintas, enderecos/familias/scope, disponibilidade, intencao selecionada/ALL | modelos existentes; physical/logical detalhado e eventos hot nao inventados |
| Listeners[] | bind intent, porta, transport profile EXPLICITO, securityref; admission/refAuthN e associationpolicy | colecao acceptedA09 precisa reconciliacao; um port/bindset atual nao toda colecao |
| Admission/AuthN | OPEN/CLOSED por contexto, requisito/mecanismo de autenticacao separado do transport, ref a provedor autorizado | nenhuma combinacao desabilita mTLS do Secure; server-auth-only future nao e terceiro perfil criado aqui |
| Identidade e sujeitos | Principal, device/client identity, mapping de credenciais, lifecycle, referencias administrativas | users/groups/nesting/RBAC sem contrato concreto: manter no catalogo de requisitos/draft, nao gerar registros operacionais |
| Source/context policy | listener/arrivinginterface/origem/IP/subnet/Principal/grupo/tempo | somente predicates aprovados/implementados podem aplicar; IP/groups/schedule nao tratados como suportados |
| Capability/AuthZ tables | contexto explicito, keys/majors habilitadas, regras ordenadas, condicoes conjuntas, ACCEPT/DENY terminal | primeira terminal e default deny; nao criar universal OR/NOT/priority/heranca; bounds atuais nao limites globais do modelo |
| Destination policies | separadas por identidade/versao: exacthostname+port e exactendpoint+port, sensitive denial | DENY_ALL/publicinternet/CIDR/suffix/ranges/explicitdeny preservados como requisitos futuros quando nao aprovados; nao retrofitar v1 |
| Egress profiles | approvedinterface/source/link/default-route/refResolver; disponibilidade | nao derivar do bindinbound; sem fallback silencioso ou migracao; source atual suporta subset |
| TLS do canal | localidentity/peerauth/trust/expectedname/ALPN/refsresolvidas | public cert/status/proveniencia refs, secrets fora source/logs; sem CA ephemera ou bypass |
| Outbound TLS ModeB | outbound_tls_profile_ref + catalogo/materializado pelo resolver | trustSYSTEM/custom explicito conforme A27; ref nao vem da Destination Policy |
| SiteTLS ModeC | tls_profile_ref, catalog/profile IDs e revisoes, origemadmin, trust/hostname/SNI/TLS/ALPN/UTCvalidade/limites | BODY05 suporta um perfil SYSTEM; conjunto maior e CUSTOM so apos suporte/aprovacao; nao dependeNETWORK_EGRESS do Accelerator |
| Tempo | distinguir civil policy, UTC de cert/profile, monotonic deadline, metricvalidity | civiltimezone/DST/feriados/grupos de dias ainda semcontrato; nao escolher UTC/local silently; parametrizacao deve preceder apply |
| Client provisioning | MANUAL/AUTOMATIC, enrollment OPEN/CLOSED e AUTO/MANUAL approvals, refsIssuer/pacote/CSR/codigo/status/rotacao | obrigacoes accepted; formatos/protocolos concretos pendentes nao escolhidos; mesmos registry/root e Principal/grants separados |
| Issuer | authority/CAgeneration/serial/reservation/state/revoke/renew/correlations | estado gerado e consultado pela root; NAO editavel em source; key/protectedstore fora modelo comum |
| Recursos | limites suportados por perfil, reservasCONTROL/CANCEL/FINAL, credito/retencao, evidence refs | nao congelar budgets candidatos, criar budgetglobalBrowser+Accel, thresholdsheadroom/freshness ou 512MiBmargin |
| Observabilidade/admin | actor/request/correlacao, rule/table/versionefetiva e motivo, audit naosecreta, validar/diff/aplicar/consultar/exportar | CLI concreto/authz/audit faltam; nomes de comando de handoffs continuam propostas |
| Atualizacao/estado | rootCURRENT,R/I/A/P associados, leases/snapshots/pins, participacao, rollbackseguro, Stop/retencao | estado factual retornado, nao authority fabricada pelo arquivo; tres slots e quota limitam coexistencia |
| Recovery/checkpoint | refs a historia/checkpoints/proofs existentes, disposicao read-only/denied, fontebackup/fencing aprovada | nenhum .papacc.bkp completo/migracao universal/newgeneration/fencing externo implementado aqui |
| Platform capabilities | CORE_PORTABLE e requisitos de adapters por target | target que nao suporte um requisito nega, nunca ignora ou faz fallbackprovider |

## Como preservar todos os requisitos sem aceitar regras ficticias

O catalogo de requisitos167linhas faz parte da proposta semantica e marca cada condicao/acao pela autoridade e suporte real. Uma configuracao draft pode ser documentada para revisao, mas nao e committed/current nem operacional. Apply de candidate com regra/acao/ref/versao nao suportada deve falhar explicitamente, sem "ignorar campo", degradar para regra ampla, tratar ausencia como zero ou desabilitar predicate ate resultar ACCEPT. Nao salvar grupos/calendarios nao executaveis como se estivessem aplicados.

Source completo so pode ser congelado depois de resolver os gaps de modelo accepted e os contratos indispensaveis do perfil a implementar. Nao e necessario implementar todas as futuras direcoes para Phase6: um perfil de source pode declarar escopo suportado e rejeitar extensoes, mantendo o modelo maior documentado e futuro versionamento explicito. Isso nao transforma o subset4rules/1profile/1listener em arquitetura completa nem aceita implementacao silenciosamente parcial.

## Decisoes ainda necessarias antes de formato executavel

- Reconciliacao do modelo accepted de listeners/perfil explicito e composicao normalizada por contexto.
- Escopo da primeira source/operacoesadmin efetivamente suportadas; parser/version/bounds/presence/duplicate/unknown handling, autorizacao e audit, sem nova autoridade.
- Refs/digests/version e leases do material resolvido, para reproduzir configuracao sem environment mutavel sob mesma identidade.
- Onde o perfil exigir richer subjects/time/destination, contratos normativos especificos antes dos predicates: nao escolher nesting/heranca/DST/OR/NOT/suffix/CIDR agora.
- Tratamento exato de revogacao ativa de credencial, bootstrap/enrollment/package e backendCNG/backup/fencing nos gates aceitos; nao contornar para "funcionar" com fixture.

Nao houve escolha de formato final nem congelamento de sintaxe/algoritmo nesta auditoria. A existencia desta estrutura proposta nao e aprovacao de nova engine, politicas ou source administrativa.
