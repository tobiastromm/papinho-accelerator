# Phase 6 — ligação nativa CAS/publicação, 2026-10-29

## Resultado e limites

**NATIVE_CAS_METADATA_BRIDGE=QUALIFIED_OFFLINE. NORMAL_FULL_COMPOSITION=NOT_COMPLETE.**

A ligação real do port nativo passou em fixtures de armazenamento exclusivos. Isso não demonstra inicialização positiva do issuer NORMAL: faltam fontes de continuidade CA/história/writer no owner live. O teste negativo confirma que NORMAL e RECOVERY não inicializam a partir das provas ausentes do fixture. Não se atribuiu autoridade de emissão aos dados de teste.

Autorização: PACC-PHASE6-NATIVE-CAS-PUBLICATION-OFFLINE-INTEGRATION e sua continuação explícita PACC-PHASE6-NATIVE-CAS-PUBLICATION-INTEGRATION-AUTHORIZED. A rejeição da ligação de código na tarefa anterior foi sucedida por essa autorização específica; nenhum patch foi aplicado por contorno antes dela. Nenhuma ADR accepted foi alterada.

## Código e callers atingidos

1. `papacc_server_launcher_configuration_issuer_port` registra o poll concreto do mesmo owner.
2. `issuer_root_port_cas` valida a view inteira e o registro anterior por campos semânticos, faz append no codec existente e chama `issuer_prepare_live`. Conserva a correlação no owner, inicia `issuer_publish_live` uma única vez e retorna PENDING.
3. `issuer_publish_live` usa os mecanismos existentes `issuer_begin06`, `issuer_poll06` e `issuer_publish06`. Não se criou worker, serviço, root ou autoridade paralelos.
4. `issuer_root_port_poll` só aceita a correlação original. Não refaz CAS/assinatura. Distingue thread pendente, commit, seleção/publicação e confirmação por releitura. Confere revisão, high-water, registro, packet nativo, snapshot selecionado, contexto administrativo A novo e P operacional preservado.
5. Stop do core retira a permissão de publicação por meio do Stop do componente. Isso não executa Stop SCM. Falha terminal ou releitura divergente conserva o contexto e marca perda de autoridade; novas consultas/admissões não recebem CURRENT positivo. Repetição do poll terminal falho retorna RETAINED.
6. `issuer_root_port_read` não expõe bytes de commit pendente como seleção publicada. Sob os guards live retorna o P factual e perfil NORMAL; não produz key/history/writer por inferência e mantém proofs ausentes. O core continua fail-closed.
7. `pending_begin06` agora também verifica `pending_blocks` antes de zerar/reusar o pacote: uma custódia/borrow/contexto anterior não pode ser apagada. O teste conserva um handle real em campo negativo, comprova a rejeição sem apagamento e o fecha pelo seu owner.

O DER é copiado para staging antes de begin; não há ponteiro para stack da chamada guardado no worker. O core preserva sua lease/saída durante PENDING. A correlação adicional é embedded no launcher existente. CAS e poll não mantêm simultaneamente dois buffers de imagem por chamada recursiva. Os limits existentes não foram aumentados.

## Qualificação final

- Selected/Legacy Debug/Release: builds PASS, zero warnings finais.
- Nove CTests não operacionais por configuração: **36 execuções PASS**. Incluem sete suítes issuer/enrollment e os testes isolados de autorização e estado/fechamento do secure owner. Não foi executado o teste real mTLS.
- **68 fixtures nativos finais PASS**: sete casos novos do port, seis regressões existentes e quatro casos do worker de armazenamento, por configuração. Ver NATIVE-FIXTURE-RESULTS-integrated.json e logs `*-integrated.log`.
- Casos: publicação de metadados; reserva/marker/DER sintéticos; caller que altera o buffer depois de staging; concorrência BUSY; poll com correlação divergente; replay; conflito lógico e conflito nativo de conteúdo; indisponibilidade antes do commit; Stop cedo e após commit; perda da prova operacional antes de publicação; releitura divergente; retenção/fechamento. O worker de componente exercita PENDING determinístico e UNKNOWN por injeção explicitamente sintética. Não é prova de todos os modos físicos de falha do Windows.
- As 207 verificações do core/modelo issuer por configuração continuam PASS: assinatura única e falha de assinatura são **modelo isolado**, não assinatura CNG real.

Uma tentativa de link do teste secure owner revelou dependências C8/launcher ausentes no target. O CMake passou a ligar esse teste aos mesmos owners reais quando C8 está habilitado; não foram criados stubs nem trocado o backend. O log da falha foi preservado. As campanhas preliminares foram repetidas apenas após alterações funcionais/de teste relevantes; os arquivos antigos permanecem.

O runner rejeita diretório de fixture existente e resolve todos os alvos sob `build/phase6-native-cas-20261029`. Os 68 caminhos/retornos/logs finais estão registrados. Nenhuma execução foi dirigida a ProgramData, CURRENT ou root operacional. Os testes chamam exclusivamente executáveis de teste e usam o attachment fechado existente; não houve listener operacional, autenticação de cliente real, emissão/assinatura real, rede operacional, serviço, SCM/ACL, TPM/NV/ownerAuth/Q4, bootstrap produtivo ou publicação PST.

## Recursos

O censo compilado por configuração está em `*-layout.log`. Após o avanço M7: launcher 344736 bytes; known managed 1971784; limite aplicável Windows 3145728, sem mudança. O fixture registra reserva combinada e créditos históricos próprios, sem transportá-los ao produto. Não confundir esses valores com o limite Browser de 2 MiB ou com headroom operacional. Provider/kernel continuam UNKNOWN. O root port embutido no core não deve ser contado novamente.

## Avanço M7

Nos adapters existentes API2/API3, após a extração PST e AuthN/AuthZ bem-sucedidas, o candidate conserva a identidade SHA256 completa da credencial. A adoção C2 exige esse vínculo e o copia ao pin original da Session antes de exposição. A verificação de Session compara esse hash e a identidade Principal separadamente. O fechamento confirmado limpa o candidate; retenção não apaga a identidade. O pin acompanha o lifetime e o reaper existentes.

Isso remove a perda anterior da identidade exata quando a evidência de handshake saía da stack. **Não é fanout M7 completo**: o port `revoke_dependents` nativo continua ausente, o status CURRENT ainda não está integrado à fonte de novas autenticações e não se demonstrou captura positiva com um cliente real. Não se revogou outra credencial do mesmo Principal. A alteração API2 foi espelhada em código; os builds finais deste pacote usam o SDK/API3 Selected/Legacy existente, sem afirmar nova qualificação runtime API2/R2.

## Primeiras lacunas concretas

| Propriedade | Fonte atual e limite | Consequência |
|---|---|---|
| CA/key/generation live | CNG `public_match/acquire` pode validar a chave já detida; o root não possui um binding CA live equivalente. `personal_ca_bind/current` pertence à cerimônia e ao seu pacote/borrow. | Não promover esse DATA ou o nome da key a KEY_BINDING NORMAL. |
| Original history/prefix/exclusive writer | ROOT06 valida conteúdo e o worker faz CAS por identidade/bytes originais naquela transação. Isso não é a fonte completa de continuidade original/exclusividade entre emissões exigida por M3. | proofs permanecem ausentes; init NORMAL e assinatura negadas. |
| Confirmação de certificado PUBLISHED | O core espera published_epoch após o port; o codec exige esse campo no registro durável, enquanto A nasce depois do commit. O adapter mantém publish=1 UNSUPPORTED e prepare06 mantém o gate PUBLISHED/REVOKED fora do fixture componente. | Não escolher P/revisão como A, não alterar bytes depois do commit ou retirar o guard para obter PASS. Resolver a representação/confirmação exata no modelo existente; demonstrar eventual lacuna normativa antes de propor alteração de formato. |
| Fanout M7 | Identidade de credencial agora preservada; falta fonte de status CURRENT e dispatch exato aos owners. | Não registrar callback permissivo nem alegar Stop/close/quiescência. |

A composição completa solicitada permanece, portanto, **não demonstrada**. Não é necessário reaprovar M3/M7 nem iniciar TPM; a próxima frente é a integração das fontes originais e da confirmação de credencial. A tabela distingue falta de implementação/fonte de decisão normativa: não foi criada ADR nova por inferência.

## Phase 6 e preservação

E8 continua SOURCE_ABSENT onde faltam aquisições reais. ENROLLMENT persistente continua condicionado à representação versionada L2. D4/D5 e custos opacos mantêm UNKNOWN. E17/E18, E19/E20, E21 e E22 não são promovidos por esta campanha. Nenhum gate previamente válido foi reaberto. ED11/Phase 6: PARTIAL; produção PST 0.6.4 preservada; 0.6.5 não publicada.

BASELINE/preimages preservam o WIP de entrada; SOURCE-CHANGES registra antes/depois e o delta; QUALIFICATION contém resultados finais e hashes. HEAD permanece o mesmo. O próximo prompt concreto está em NEXT-NATIVE-NORMAL-CONTINUATION.md.
