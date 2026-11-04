# Phase 6 — intenção/ack PUBLISHED versionados

## Resultado

Decisão focal registrada como [ACCEPTED](../../adr/DECISION-published-intent-and-ack-20261029.md), após conferência literal da proposta SHA256 `3fcce389e58299aaa161779b9c7976f00f8e79d76dc5622d40d60b091e34f684`. [Registro da decisão](OWNER-ACCEPTANCE.json). A proposta histórica permanece byte-exata. Nenhuma autoridade operacional foi concedida.

PACIS602/PACICP02 integrados e qualificados offline no mesmo root/arquivo/worker. PACIS601 e PACICP01 preservam seus encodings e regras; não existe migração implícita. **Issuer NORMAL completo: DENY; ED11/Phase 6: PARTIAL.**

## Código e fluxo atingido

- `issuer_codec.c` + `issuer_publication_v2.inc`: dispatch explícito, intenção física 9 sem A, ack vinculado ao ordinal/digest integral/correlação/P da intenção, referências originais R/I/A, certificado/serial/high-water preservados. Reserva espaço para os dois eventos antes da intenção. Sem descarte de história para ganhar capacidade.
- `issuer_root_port_cas → issuer_prepare_live → begin/poll → publish_live`: primeiro commit/seleção da intenção; captura factual de A original; segundo CAS do ack no mesmo histórico; seleção que contém o ack; checkpoint pelo mesmo worker, sem outra seleção/epoch; re-read original antes do resultado terminal.
- `issuer_root_port_poll`: contexto e correlação originais retidos entre turnos. Read/cert/admissão negados enquanto pendente. Stop/falha conserva contexto e UNKNOWN, sem assinatura/CAS repetidos automaticamente. Não há loop de ack do ack.
- `papacc_configuration_issuer_prepare06`: além do codec, compara o ack com R/I/A/P da seleção original. Bytes/digest fornecidos pelo chamador não fabricam A. Prefixo histórico e versão anterior devem coincidir; não se converte v1 em v2.
- `issuer_core.verify_store`: confirmação tipada pelo owner canônico distingue A original de A atual. O callback não é setter de grants, tuple ou bool; confere história/packet/checkpoint/seleção originais. Adapters anteriores sem esse callback mantêm sua verificação anterior. Nenhum producer de proofs NORMAL foi fabricado.
- `issuer_root_credential_status`: snapshot v2 só é consultado sob CURRENT original e checkpoint v2 correspondente. DATA reaberto após perda do processo não se torna CURRENT.

O [encoding](VERSION2-ENCODING.md) foi registrado antes do código dependente. Imagem 8192 bytes, evento v2 fixo 468 + DER, checkpoint 512 e arquivo máximo 16896: limites anteriores preservados. A significa contexto administrativo pós-publicação; P significa contexto operacional de policy. Nenhum deles é sinônimo de commit ou revisão.

## Qualificação final

| Evidência | Resultado e alcance |
|---|---|
| Selected/Legacy Debug/Release | Quatro builds PASS, zero warnings finais |
| CTests afetados | 52 PASS, 13/configuração |
| Execuções focadas | 112 PASS, 28/configuração; arquivos NOVOS exclusivos em `build/phase6-published-ack-v2-20261029/final-v3` |
| Codec | 142 checks/configuração, SHA256 nativo sobre DATA sintético; referência/P/R/I/A/ordinal/digest/correlação divergentes, replay, corrupção, certificado e ack histórico após revogação |
| Pipeline nativo v2 | Publicação apenas de metadados do fixture; A original e atual distintos, P preservado, re-read, checkpoint v2 e rejeição de confirmação divergente |
| Stop | Antes/depois dos commits de intenção, ack e checkpoint; contexto retido, sem retry ou sucesso prematuro |
| Perda do processo/reabertura | Saída deliberada sem cleanup nas fronteiras antes da intenção, após commit da intenção, após ack e após checkpoint; outro processo relê o arquivo original e permanece DATA/RECOVERY_DENIED. Não é ensaio de power loss ou rollback global |
| Compatibilidade v1 | Pipeline anterior, conflito, compartilhamento negado, Stop, proof/readback divergentes e codec preservados |
| Core/CNG/custódia | 279 checks de core e 614 de modelo CNG/configuração; handoff/lifetime e fonte nativa negativa preservados |
| Gates produtivos | Init NORMAL/RECOVERY sem fontes originais nega antes de assinatura; nenhuma key/cert/autoridade produtiva foi criada |

[QUALIFICATION.json](QUALIFICATION.json), [FOCUSED-NATIVE-RESULTS.json](FOCUSED-NATIVE-RESULTS.json), logs por configuração e [hashes finais](ARTIFACT-HASHES.json) discriminam cada escopo. Testes de processo usam commits locais reais, mas não demonstram persistência física após falha elétrica. UNKNOWN não foi convertido em PASS.

## Recursos e ownership

Core 1072 bytes; root port 64; backend CNG 736, incluindo o core; key 480. O owner mantém 104 bytes de fatos originais; as reservas calculadas por sizeof incluem o custo. Known managed Selected **1972000/3145728**, Legacy **1803432/2097152**. Reservas combinadas observadas do fixture: Selected 2151943, Legacy 1983375. Esses números não concedem capacidade: provider/kernel/picos não amostrados continuam UNKNOWN. Não se alteraram quotas, owners ou toolchains.

## CA/key/custódia e M7

Handoff original `personal_ca_current → transfer_borrow → backend CNG` e lifetime entre leases permanecem integrados e passaram nas regressões. A fonte factual CA/key/cert/generation ainda não foi adquirida. Não há root operacional demonstrada nem instância original apta a preencher KEY_BINDING/ORIGINAL_HISTORY/EXCLUSIVE_WRITER. Um fixture, hash ou ponteiro não os substitui.

O wrapper M7 foi exercitado positivamente com histórico PUBLISHED→REVOKED confirmado nativamente e **cohort vazio**: releitura, identidade exata e revalidação sem Stop nativo. Os efeitos sobre cohort sintético continuam cobertos separadamente pelo teste de Sessions/operações; o fanout completo sobre Sessions realmente autenticadas não foi demonstrado. Invalidação/Stop/wake não são receipts de close ou quiescência.

## Caminho restante

1. Adquirir fonte original legítima CA/key/cert/geração com custódia/história/writer no owner existente. Se não houver uma instância existente, a eventual cerimônia isolada que a materialize exige delimitação/autorização própria; não se iniciou bootstrap nem se promoveram fixtures.
2. Com essa fonte, qualificar composição NORMAL e M7 com dependentes autenticados e fechamento factual, sob autoridade separada para os efeitos necessários. Não refazer o ack/CAS por falta de fonte.
3. Seguem independentes E8/source de provisão, representação persistida ENROLLMENT L2, jornadas Browser/certificados/GUI, composição/recursos E17–E20, histórico Q4/E21 e alvo Legacy/NT4 conforme a matriz. E22 permanece derivado. Não se reabrem qualificações históricas válidas.

As Future Directions de listener/configuração e backup foram revisadas quanto à aplicabilidade: este ack ISSUER_ONLY não implementa live reload de settings, backup ou recuperação; suas direções não foram promovidas. A [matriz única](../phase6-original-scope-recovery-20261028/REQUIREMENTS.csv) e capability corrente foram atualizadas, preservando os gates ED11 históricos.

HEAD/WIP/históricos/pins operacionais e PST 0.6.4 preservados. Nenhuma publicação CURRENT real, assinatura/emissão produtiva, CA/key operacional, bootstrap, rede operacional, serviço/Start/Stop, SCM/ACL, TPM/NV/Q4, cleanup/refund ou recuperação autoritativa. Fixtures e estados retidos não foram apagados.

[Próximo prompt executável](NEXT-ORIGINAL-SOURCE-COMPOSITION.md).
