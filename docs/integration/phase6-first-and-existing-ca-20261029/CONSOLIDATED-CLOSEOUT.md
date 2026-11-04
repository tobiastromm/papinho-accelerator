# Primeira instância e CA existente — Phase 6

## Resultado e fronteira

Resolvida a distinção: **NEW_INSTALL não requer uma CA anterior; aquisição de instalação existente não é NEW_INSTALL nem recuperação**. Foram integrados o builder público inicial e a aquisição/ativação borrow-only distinta para key reaberta. Os dois caminhos passam em fixtures **MODEL_ONLY**, sem criar CA ou chave no equipamento.

Não se entrega aqui um NEW_INSTALL nativo habilitado. A tentativa de integrar um novo chamador produtivo de criação persistente foi rejeitada pela revisão automática; essa alteração não foi aplicada. A criação ficou exclusivamente no fixture de teste. Portanto ainda há essa fronteira técnica de integração/autorização, além das operações e fontes reais. **NORMAL produtivo DENY; ED11/Phase 6 PARTIAL.** Não se afirma que restam somente operações reais.

## Sequência NEW_INSTALL e fundamento accepted

| Fase | Regra e fonte | Evidência desta tarefa |
|---|---|---|
| 1. Autorizar primeira instalação | F0/F1: política pessoal aceita fora do candidato; identidade/contexto/direitos e declaração atribuída; nenhuma CA/CURRENT anterior exigida | Modelo usa as rotinas existentes de plano, apresentação, confirmação e consumo; nega sem CA_INITIALIZE explícito ou após Stop |
| 2. Nomear CA_INITIALIZE | F7/issuer cláusula 6: ato distinto, podendo estar na mesma interação somente com ambos os propósitos explícitos | Plano de efeito contém NEW_INSTALL e CA_INITIALIZE; não se alterou o CLI DATA-only para fazê-lo criar CA |
| 3. Identidades e alvo | F3–F5: IDs antes da apresentação, parent protegido, criação exclusiva, anexação do filho factual ao mesmo ato, consumo sem rearmar | Namespaces/operador do positivo são modelados; identidades produtivas não foram inventadas nem pesquisadas por segredo |
| 4. Key protegida e certificado inicial | Perfil CA 1.1/1.2/1.4, 2.1 e 3.1–3.4: geração explícita, KSP MACHINE, RSA2048/export0/signing-only, CA distinta de TLS; serial próprio 1 | Fixture começa sem key e instancia somente um objeto sintético após o ato consumido. Builder TBS usa CryptoAPI real e não exige R/A/P anteriores. Não há chamada nova de criação persistente no produto |
| 5. Persistência e publicação inicial | F7, D2/D2.3, issuer/C2 P3: efeitos separados, sem atomicidade presumida, sem predecessor/A/P fictícios; reserva/persistência precede assinatura quando exigida pelo perfil §2.2 | O fixture separa a criação da publicação modelada. O plano de efeito existente diz COMMIT=DENY/CURRENT=DENY e **não foi reinterpretado como aprovação de publicação**. A confirmação nativa de metadados v1/v2 foi revalidada separadamente |
| 6. Custódia e primeiro NORMAL | Handoff existente e M3: borrow contínua; origem NEW_INSTALL e generation correspondentes; ativação só com CURRENT/provas legítimos | Fonte modelada transferida antes de reutilizar o pacote; primeiro init e emissão modelados, uma assinatura sintética |
| 7. Uso e restart | Perfil §2.2–2.4/3.2 e M3: história original, reservas/high-water, key/cert, exclusividade, CURRENT; abrir a mesma key, nunca gerar substituta | Fixture existente reinicializa core/backend, adquire a key original modelada como `created=false` e não restaura attempt NEW_INSTALL. Restore/UNKNOWN/recovery permanecem negados |

F0 diz expressamente que NEW_INSTALL “Não exige CURRENT/predecessor anterior nem a CA que ainda será criada”. F7 exige CA_INITIALIZE distinto e não aceita implicitamente bootstrap v4 ou atalho de publicação. Não há motivo para pedir novamente M3/F0–F9/PUBLISHED. Não foi criada outra família de ADRs. O ciclo nativo completo ainda deve satisfazer a reserva/durabilidade do perfil §2.2/2.5 e todos os participantes, sem reinterpretar formatos.

## Código integrado

1. **`papacc_issuer_x509_ca_tbs`**: preparação unsigned do perfil CA aceito, sujeito/issuer da autoridade, RSA/SPKI, SKI pelo provider, BC CA=true/pathLen0, KU e validade exata. Requer high-water 1, não um issuer anterior. Não assina, gera key, grava, estabelece confiança ou concede autoridade.
2. **`source_kind`** no backend existente: NEW_INSTALL e EXISTING_CURRENT distintos. Metadado privado em memória, não formato persistido nem proof bit.
3. Handoff original conserva seu guard `key.created` e marca NEW_INSTALL; não foi relaxado para aceitar key reaberta ou LAB.
4. **`papacc_configuration_existing_ca_bind`**: exige key **já aberta**, `created=false`, token/propriedades/binding válidos pelo adapter, mesmo CURRENT original, R/I/A/P e proofs NORMAL completos. Usa uma borrow existente no mesmo owner; não abre, cria ou faz fallback. Tentativa NEW_INSTALL viva/recovery/UNKNOWN/pending nega. DATA já aposentado não vira autorização.
5. **`papacc_configuration_existing_ca_activate_normal`**: não recebe nem ressuscita nonce de criação; exige attempt vazio e modo existente, relê CURRENT e usa o mesmo core/CNG. Publicação incompleta ou epoch divergente conserva a borrow e nega ativação.
6. **`papacc_server_launcher_configuration_issuer_existing_ca_bind`**: wrapper restrito à factory e owner canônicos; a ativação seleciona explicitamente o modo. Sem D3 retorna UNSUPPORTED.
7. Finalização comum preserva re-read e retenção após falha de init, sem segunda assinatura, transferência ou liberação automática.

O signer, CAS, codecs PUBLISHED v2 e core portátil não foram reiniciados. CA privada não é exportada. O certificado público continua precisando de origem/backing legítimos; TBS ou perfil correspondente não provam self-signature ou publicação.

## Qualificação

| Campanha final | Resultado |
|---|---|
| Selected/Legacy Debug/Release | Quatro builds PASS, zero warnings finais |
| CTests afetados | 52 PASS, 13/configuração |
| Modelo CNG | **2382 verificações/configuração**; preserva casos anteriores e acrescenta genesis/existing/restart/mismatch |
| Fixtures nativos de metadados | Oito PASS, dois/configuração, novos caminhos exclusivos sob `build/phase6-first-and-existing-ca-20261029/final-qualified` |
| Jornada A | Sem CA anterior e sem R/A/P durante preparação; criação do **objeto sintético** condicionada ao plano consumido; replay/Stop/ausência de propósito negados; TBS público real, assinatura de CA sintética; handoff/init/uma emissão sintética |
| Jornada B | Snapshot original do mesmo fixture, outro core/backend, key aberta modelada sem criação; attempt de criação ausente; bind/init/emissão sintéticos; ausência não faz fallback para criar |
| Negativos | Key/cert/gen/root/epoch divergentes; `created=true` no caminho existente; CURRENT incompleto; recovery; tentativa antiga ativa; busy/Stop; re-read inconsistente; retenção após aquisição; proibição de misturar modos |
| Reuso | Campanha de 112 casos CAS/PUBLISHED anterior preservada, sem repetição inalterada; modelos M7/cohort e gates nativos negativos passaram nas regressões |

**Não se executaram `NCryptCreatePersistedKey`, abertura real de key CA ou assinatura privada nova nesta campanha.** A criação no fixture altera exclusivamente objetos/handles sintéticos do teste; sua assinatura não é válida como CA operacional. O TBS usa encoding/hash públicos reais. Native wrapper sem fonte nega; não há positiva nativa de autoridade CA/NORMAL.

As jornadas de emissão modeladas terminam no estado ISSUED_UNPUBLISHED; a confirmação PUBLISHED foi demonstrada separadamente nos fixtures nativos de metadados. Não se afirma composição nativa criptográfica ponta a ponta, token isolation ou continuidade física após reboot. O restart aqui é uma troca de instância do modelo, não restore qualificado do Windows/KSP.

[QUALIFICATION.json](QUALIFICATION.json), [NATIVE-RESULTS.json](NATIVE-RESULTS.json), [SOURCE-CHANGES.json](SOURCE-CHANGES.json), [FUNCTIONAL-DELTA.patch](FUNCTIONAL-DELTA.patch), [ARTIFACT-HASHES.json](ARTIFACT-HASHES.json) e logs registram os escopos e hashes. Backend 808 bytes (sem aumento medido), core 1072, key 480. Known managed canônico preservado: Selected 1972000/3145728, Legacy 1803432/2097152. Backings/stack da preparação pertencem ao caller; provider/kernel e headroom operacional continuam UNKNOWN. Não há crédito ou ceiling novo.

## Limitação da integração nativa e decisão concreta

A revisão automática rejeitou o patch que acrescentaria um chamador de `papacc_ca_key_create` ao código produtivo: risco de criação de chave CA persistente real além dos limites, com alternativa de implementação em fixture/teste isolado. Não se tentou contornar a rejeição. As alterações preparatórias de candidato/consumo associadas àquele patch foram revertidas para os preimages; o mecanismo aplicado não contém essa entrada.

Além disso, a autoridade nativa pessoal continua deliberadamente fechada: `personal_native_ceremony_ready` no caminho DATA retorna 0, e a factory issuer não produz proofs NORMAL completos. Confirmação de plano, criação simulada ou o novo modo existente não removem esses gates. Não seria factual dizer que fornecer um nome de CA torna o produto pronto.

O próximo gate de código é a integração especificamente autorizada da entrada nativa CA_INITIALIZE e de suas fontes de admissibilidade, sem execução automática. [Escopo concreto para decisão](NATIVE-ENTRY-DECISION.md). As normas de finalidade já existem; não se propõe alterar accepted ou aprovar quotas. Depois, a **primeira operação real** será uma cerimônia pessoal/local NEW_INSTALL + CA_INITIALIZE com bindings atuais, recursos e efeitos delimitados, não uma busca indefinida de CA inexistente. O pedido operacional ainda não tem artefatos/bindings completos; nenhum comando com placeholders foi entregue.

## Continuação funcional / Phase 6

- **M7:** aquisição existente agora tem entrada própria; wrapper/status e cohort vazio preservados. Faltam credenciais/dependentes realmente autenticados e receipts de fechamento/quiescência. Nenhum fechamento inferido por Stop.
- **E8 / MANUAL:** CA inicial ou existente não concede delegação PACKAGE_SIGNING/confiança. `mp_auth_pdg_candidate`/`sp_trust_acquire` continuam SOURCE_ABSENT/DENY; nenhum callback positivo instalado.
- **AUTOMATIC/enrollment:** ambos convergem no mesmo issuer, com key cliente no Browser. Trace/claim DATA preservados; fontes e persistência L2 ainda pendentes, sem alteração de formato não autorizado.
- **D4/D5 e alvo:** política quantitativa/provider/kernel continuam independentes e incompletos; não dependem de TPM para desenvolver fontes. Não foram inventados valores.
- **E17/E18/E19/E20/E21/E22:** estados históricos preservados. Não houve bootstrap, Q4, TPM ou admissão para suprir a lacuna. Demais jornadas Browser/GUI/Legacy/NT4 continuam na matriz recuperada.

HEAD/main permanece `f341985924d29870d8ff627ed37c8a0874ad6469`. WIP, históricos, pins, UNKNOWN, reservas e PST 0.6.4 preservados. Sem CURRENT produtivo, CA real, assinatura produtiva, rede operacional, Start/Stop, SCM/ACL, TPM/NV/Q4, cleanup/refund ou recuperação.

[Próximo prompt](NEXT-NATIVE-FIRST-INSTANCE-INTEGRATION.md).
