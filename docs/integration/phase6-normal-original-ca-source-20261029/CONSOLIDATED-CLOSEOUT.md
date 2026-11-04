# Composição da fonte original CA — Phase 6

## Resultado

Integração técnica offline concluída no escopo disponível: **origem verificada da cerimônia → borrow contínua no backend → validação pública CA/key/view → ativação NORMAL no mesmo core**, com wrapper do launcher canônico. Não foi criada CA, key ou autoridade. **A instância original operacional continua NOT_ACQUIRED; issuer NORMAL produtivo DENY; ED11/Phase 6 PARTIAL.**

HEAD permanece `f341985924d29870d8ff627ed37c8a0874ad6469`, branch main, WIP preservado. CAS/PUBLISHED v2 e seus formatos não foram redesenhados. Nenhum ADR accepted foi modificado ou novamente submetido.

## Fontes e composição implementada

| Informação | Owner/entrada existente | Composição e limite |
|---|---|---|
| CA/key original | `PAPACC_CA_KEY`, `personal_ca_bind/current/handoff` | A cerimônia verifica o ato consumido, contexto nativo, alvo original, operador, conta operacional, nome authority/generation e propriedades da key. Continua excluindo identidades LAB retidas. Não há busca/adoção automática de uma key por nome. |
| Custódia | Uma borrow do pacote original, transferida por `papacc_ca_key_transfer_borrow` | O mesmo backend agora conserva owner original e attempt/authority/generation verificados. Não é novo owner nem armazenamento persistente de autoridade. A borrow não retorna ao concluir cada assinatura. |
| Certificado público | `CERT_INFO` e backing imutável já fornecidos ao backend | Extraída a validação de perfil já usada no TBS para `papacc_issuer_x509_ca_matches`: RSA/SPKI, Subject/Issuer, serial da CA, validade, BC/KU/SKI e propósito. **Não comprova origem nem verifica self-signature de um DER original**. Seu owner/backing legítimo ainda deve ser adquirido; ponteiro e perfil corretos não o substituem. |
| Binding de chave/cert/root | `papacc_issuer_cng_backend_normal_source` | Consome a view da root existente, exige provas NORMAL já legítimas, confere key/authority/generation/SPKI/perfil público e relê a mesma view após as consultas de provider. Não produz proof bits ou cria material. Mudança/Stop nega. |
| Ativação | `papacc_configuration_personal_ca_activate_normal` | Consome origem anteriormente transferida, confere o mesmo root/attempt/generation, inicializa o core NORMAL e faz validação final. Falha após init retém borrow/contexto, bloqueia core e marca o root unresolved; não repete assinatura ou transferência. |
| Caller canônico | `papacc_server_launcher_configuration_issuer_normal_activate` | Exige owner real e factory exata de read/cas/poll/cert/ack/M7, custódia original e CURRENT. Backend test-only, outro root ou fonte ausente negam. Sem D3, a entrada retorna UNSUPPORTED. Não foi instalada em startup como efeito implícito. |
| História/serial/CURRENT/A/P | Root, snapshots, CAS e checkpoint existentes | Continuam fontes obrigatórias independentes. O compositor não deriva exclusividade/continuidade completa de um thread, hash ou descriptor de origem. A factory nativa continua sem as provas NORMAL completas. |

### Ordem de lifecycle corrigida

O pacote consumido da cerimônia não pode permanecer como dependência permanente da ativação: seus handles e estado bloqueiam a continuação do worker/bootstrap/CAS. A sequência técnica agora distingue:

1. Revalidar a origem efetiva e transferir a borrow pelo handoff já existente; capturar correlação no mesmo backend, sem intervalo de devolução/reaquisição.
2. Finalizar governadamente o pacote quando suas referências estiverem realmente encerradas; o mesmo backend continua responsável pela key. O código não executa essa finalização automaticamente.
3. Somente com a mesma root factualmente CURRENT, certificado original e provas legítimas, ativar NORMAL, revalidar a view e manter a borrow através de leases/PENDING.

O latch em memória **não é prova durável de restart, publicação, origem do certificado ou recuperação**. Após perda da instância, aquisição original/continuidade devem ser novamente demonstradas pelos contratos existentes. Nenhuma regra RECOVERY foi reutilizada.

## Jornada demonstrada e testes

- **Quatro builds Selected/Legacy Debug/Release PASS, zero warnings finais.** Mesma toolchain/SDK/backend e limites.
- **52 execuções CTest PASS**, 13 por configuração. Modelo CNG passou **1354 verificações/configuração**; as 614 anteriores foram preservadas e ampliadas.
- Jornada positiva **MODEL_ONLY**: construção do plano de efeito sintético pelas rotinas existentes, confirmação/consumo modelados, binding original modelado, handoff, reutilização do pacote de teste sem devolver a key, ativação NORMAL, emissão com **uma assinatura sintética**, lease encerrada conservando custódia e devolução após Stop/finish factual do modelo.
- Falhas: fonte/provas ausentes, key/cert/generation/root divergentes, origem incorreta, holder indevido, validade diferente, core ocupado, mudança de P/R/A/policy/high-water durante consultas, consulta incerta, Stop durante validação, falha após transferência/init e rejeição de repetição. Uma thread nativa de teste confirmou a rejeição fora do owner; seu handle foi encerrado. Não qualifica orçamento de threads do produto.
- **Oito fixtures nativos novos PASS**, dois por configuração: wrapper do root real nega ausência da fonte, e os caminhos históricos v1/v2 de metadados continuam válidos. O v2 conserva confirmação PUBLISHED e M7 com **cohort vazio**, sem criar autenticação.
- Fonte/cerimônia nativa positiva, CA/key privada, self-signature original, assinatura produtiva e jornada NORMAL nativa completa **não foram executadas**. As fontes positivas e assinaturas do modelo são exclusivamente de teste, nunca vinculadas ao produto.

Logs e hashes: [QUALIFICATION.json](QUALIFICATION.json), [NATIVE-RESULTS.json](NATIVE-RESULTS.json), [SOURCE-CHANGES.json](SOURCE-CHANGES.json), [ARTIFACT-HASHES.json](ARTIFACT-HASHES.json). O delta compara com preimages deste WIP, não com um checkout que descartaria mudanças anteriores. A campanha CAS/v2 de 112 execuções do pacote anterior foi reutilizada; não se repetiu sua fault matrix inalterada.

## Recursos

Backend CNG: **808 bytes**, antes 736; os 72 bytes adicionais conservam origem/correlação no mesmo objeto. Core 1072, key 480 e workspace de teste 4096 preservados. A reserva do backend usa sizeof e inclui o custo; o caller continua responsável por backing público e stack transitiva. Nenhuma dessas quantidades demonstra custos opacos ou capacidade operacional.

Censo gerenciado canônico permanece Selected **1972000/3145728**, Legacy **1803432/2097152**: o backend administrativo não é uma segunda instância adicionada ao runtime. Provider/kernel, picos não amostrados e headroom operacional permanecem UNKNOWN. Não se inventaram ceilings ou valores D4/D5.

## Primeira fonte factual ausente e próximos gates

**Falta uma instância original legitimamente adquirida CA/key/certificado/geração ligada à root administrativa real, com origem da cerimônia e custódia disponíveis.** Nenhuma root/CA de qualificação foi promovida. O código de consumidor/handoff não cria essa instância nem as provas ORIGINAL_HISTORY/EXCLUSIVE_WRITER/KEY_BINDING. A ausência das provas nativas não foi mascarada preenchendo flags.

Mesmo com um arquivo público ou nome de key informado, ainda seriam necessários validar objeto, cert/key, custódia, história original, CURRENT, reservas/high-water e autoridade de writer. O pacote não oferece um comando operacional com IDs ou hashes futuros inventados. [Decisão de aquisição](SOURCE-ACQUISITION-BOUNDARY.md) distingue os fatos faltantes de autorização normativa já resolvida.

| Frente | Estado corrente / próxima evidência |
|---|---|
| NORMAL | Compositor offline e handoff qualificados; fonte original e producer factual completo de provas ainda ausentes. DENY. |
| M7 | Port/status CURRENT e efeitos de cohort já integrados. Wrapper nativo com histórico e cohort vazio revalidado; faltam dependentes realmente autenticados e receipts de fechamento/quiescência. |
| E8 | `mp_auth_pdg_candidate`/`sp_trust_acquire` continuam SOURCE_ABSENT/DENY. Fonte CA modelada não concede delegação/confiança/package signer. Não foi adicionado callback positivo. |
| Enrollment | Trace/claim e perfil DATA preservados; listener/CSR-to-issuer com fontes reais e persistência L2 continuam pendentes. Nenhum formato não autorizado foi alterado. |
| D4/D5 | Anexo quantitativo ainda INCOMPLETE_DENY; custo conhecido do backend registrado separadamente. Nada permite inferir provider/kernel ou headroom real. |
| ED11 | E17/E18, E19/E20, histórico E21 e E22 derivado permanecem independentes. Nenhum gate foi fechado por contagem de testes. |

Future Directions de backup/recuperação e listener/configuração permanecem nos seus escopos; esta composição não os implementa nem cria nova obrigação de TPM. O próximo passo material é adquirir fonte original existente ou decidir separadamente a menor cerimônia pessoal/local necessária, sem bootstrap automático. [Próximo prompt](NEXT-ORIGINAL-INSTANCE-HANDOFF.md).

## Ausência de efeitos produtivos

Sem CA/key nova, exportação privada, certificado produtivo, CURRENT real, bootstrap produtivo, rede operacional, serviço/Start/Stop, SCM/ACL, TPM/NV/Q4, cleanup/refund, recuperação ou release. Fixtures novos exclusivamente em `build/phase6-normal-original-ca-source-20261029/final`; históricos e fixtures anteriores preservados. PST 0.6.4, HEAD, WIP, pins operacionais, UNKNOWN e reservas preservados.
