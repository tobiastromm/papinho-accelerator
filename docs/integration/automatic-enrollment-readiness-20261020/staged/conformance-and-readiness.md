# Reconciliação de contratos e prontidão — automática

HEAD main/c50b8fb9a7e3293b5e6355fc08459bf85b56857a. PROJECT-CONTEXT.md ausente. AGENTS.md e decisões accepted vigentes regem a tarefa; owner confirmou que não existe contrato concreto adicional para código/bootstrap e autorizou apenas uma proposta focal. Não há conflito entre decisões accepted: há ausência expressa de decisão técnica. Nenhuma alteração de fontes é necessária/autorizada por inferência para preencher essa lacuna.

## Matriz factual

| Exigência | Autoridade vigente | Implementação reutilizável | Resultado desta tarefa |
|---|---|---|---|
| Address/servidor verificável/código | ADR-0006 Pairing; decisão provisioning item3 | listener secure exige TLS1.3/mTLS/ALPN; não há endpoint pré-credencial | DENY; P1–P4 propostos, transporte P7 aberto |
| Código contextual curto/single-use/rate-limited | ADR-0006; provisioning itens3/5 | C2 tokens são pós-Session/offload, não enrollment | não reutilizar PAIR_OPEN/C2-token como enrollment; sem código executável |
| OPEN | provisioning item4; ADR-0028 | não há caminho de solicitação sem primeira credencial | sem username/password inicial por contrato, não significa ausência de mTLS/trust |
| CLOSED | provisioning item4 | authentication_authorization resolve certificado/Principal; provider username/password de enrollment não encontrado | DENY até fonte factual; não inventar catálogo |
| AUTO/MANUAL | provisioning item5 | issuer authorize é fronteira administrativa, não workflow de confirmação de pairing | fontes e binding ainda faltam; não usar boolean sintético |
| Chave local/CSR/PoP | CA/custódia item5; técnico1.1/1.3 | verify_request nativo Crypt32 verifica CSR DER/RSA2048/e65537/SHA256/digests/PoP; atributos unsupported | componente preservado; nada no servidor gera/exporta chave automática |
| CSR posterior ao pareamento autorizado | custódia item5 | não há ingress automático | P2 separa contexto original por request/nonce e vínculo CSR derivado posterior; não presume CSR pré-pairing |
| Reserva/serial/high-water | técnico2.1–2.3; issuer core/codec | prepare/reserve/issue/query/transition; mesma root via ports, CAS/re-read e burn | regressões existentes executadas, emissão operacional negada |
| Renewal/revocation | provisioning6/7; custódia8; técnico2.3 | core predecessor/new key/new serial e REVOKED, dados persistentes issuer | preservados; ausência de API/remoting real não resolvida; Sessions ativas POLICY_PENDING |
| Registro/Principal/grants | ADR-0006; provisioning8; root/C8 | issuer device separado de Principal; authz/gates existentes | não criar identidade/cadastro paralelo nem conceder grants |
| Lifecycle | Model E, ADR-0003/0030/0031/0032, root/C8 | owner Stop/retention/release e créditos existentes | regressões sintéticas preservadas, sem alegar cancelamento nativo |
| ED11 | decisão Windows3MiB W1–W6; ED11 incremental | explicit selected3145728/default2097152 e crédito compartilhado | sem buffers novos/reservas; automático completo UNKNOWN, DENY |

## Compatibilidade da proposta P0–P9

| Cláusula candidata | Decisões que a restringem | Classificação |
|---|---|---|
| P0 | ADR-0006/0028 e custódia5 | preserva separação e chave local; nenhuma ativação |
| P1 | ADR-0006 CSPRNG/short-lived/context-bound | 10base32/50bits e nonce são NOVAS escolhas PROPOSED, não herdadas da norma |
| P2 | provisioning3/5/8; custódia5; root/C8 | contexto imutável factual; CSR somente posterior; fontes faltantes negam |
| P3 | ADR-0006 monotônico/UTC; Model E | 300000ms propostos, não altera validade X509 accepted |
| P4 | ED11 W2/Model E | 5 tentativas, 2 contextos, janelas2/10 por60000ms propostos; sem nova quota efetiva ou crédito |
| P5 | provisioning4/5; ADR-0028 | OPEN/CLOSED e AUTO/MANUAL distintos, provider ausente DENY |
| P6 | técnico2.2/2.3; root/C8/CAS | consumo irrevogável não é commit/assinatura; UNKNOWN retém; binding ainda precisa implementação aprovada |
| P7 | ADR-0001/0006/0009; provisioning Gates/Escolhas não aprovadas | não existe transporte inicial aprovado; nenhuma extensão/wire implementável por esta proposta |
| P8 | issuer/C2; Model E/ED08 | preserva serial, snapshots, Stop, retenção e política ativa pendente |
| P9 | ED11 W1–W6 | nenhuma capacidade/reserva presumida; censo de ativação falta |

Não há base para registrar ACCEPTED. Revisão oficial desta tarefa cobre documentos PROPOSED e relato factual, não aceita P0–P9 ou patches funcionais. Próximo gate executável: revisão/decisão dos parâmetros exatos P1/P3/P4 e binding P2/P5/P6; o transporte P7 continua DENY e não pode ser suprido por novo protocolo inferido. Após aceitação do código, o primeiro slice possível é lógica local pura de código/estado, sem rede/issuer/authority positiva, somente com aprovação de patch exato. Não adicionar máquina de estados dormente agora para criar aparência de avanço funcional.

## Evidência nativa versus modelos

12/12 suites por Selected/Legacy Debug/Release. Issuer117checks usa root/signer sintéticos; production gate4checks rejeita adapters de teste; checkpoint41checks combina metadata sintética e SHA256 nativo, recuperação DENY. CSR Windows33checks usa Crypt32 real em entradas inválidas, inclusive assinatura zero; não é CSR positivo nativo de Browser. CNG X509504checks usa assinatura/root sintéticos, nenhuma private operation; output de testes não é certificado operacional. Gates CNG testam isolamento e borrow guards sem chamada nativa de chave. ED11 créditos/Stop/retenção são sintéticos, não medição operacional. Código válido/inválido/expirado/replay, OPEN/CLOSED e aprovação automática/manual de enrollment NÃO EXECUTADOS: implementação ausente, fixtures não seriam prova funcional.

Builds repetem CMake/MSVC19.51 /W4 /WX existentes nas árvores canônicas opt-in. Build incremental final sem work adicional/sem warnings; nenhum source patch ou compilação de novo componente foi aplicado. SDK existente somente consumido; nenhum acquire, rede ou teste socket foi executado. Logs detalhados preservam assertions e censo por configuração.
