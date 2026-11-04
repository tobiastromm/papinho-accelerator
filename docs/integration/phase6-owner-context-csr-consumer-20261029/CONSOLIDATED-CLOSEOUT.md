# Phase 6 — contexto elevado e consumidor CSR

**Contexto elevado: PASS_AT_OBSERVATION reconciliado. Consumidor enrollment/issuer: QUALIFIED_OFFLINE.** A coleta não autorizou instalação. Phase 6/ED11 permanecem PARTIAL; autoridades produtivas DENY.

## Reconciliação da coleta do proprietário

A saída manual fornecida foi reconciliada com os bytes do script SHA256 `5bd9056cf954a611c4e98fea432b2f0e9bee1991b85c774eeb532245275387d2`. O script audita versão Windows PowerShell 5.1/x64, elevação e hash do coletor antes de executá-lo. A conclusão sobre esses gates decorre da saída bem-sucedida do script auditado, não de atestação independente de imagem mapeada. A transcrição foi estruturada em `OWNER-MANUAL-RESULT.json`; não foi reexecutada coleta elevada equivalente.

- TokenUser observado: `S-1-5-21-785699550-1440458426-1682014700-1108`; PID19844.
- Administrators presente, atributos15: inclui ENABLED; token elevado, ElevationTypeFull=2.
- Token/process/active-console session=1.
- `preflight_result=0`; predicate_id/failure_kind/native_error=0. O rótulo histórico `NONE_OR_UNINSTRUMENTED` não significa falha nesta saída: o retorno e a linha terminal registram elegibilidade, revalidação e fechamento bem-sucedidos. Reconciliação: primeira falha NONE no escopo do probe.
- Parent TEMP aberto e revalidado, native_close_result=0; observador fechado, erro0. Não é o parent da instalação escolhida.
- Host de metadata TCORP-TOBIAS; boot RecordId237900, StartTime `2026-10-26T21:35:16.5000000Z`, TimeCreated separado `2026-10-26T21:35:17.6469098Z`.

Isso resolve a distinção factual entre o processo de agente não administrativo da coleta anterior e o contexto elevado do proprietário. Não fornece autorização NEW_INSTALL/CA_INITIALIZE, identidade jurídica, root publicada, alvo definitivo, custódia MACHINE ou identidade de runtime. Não foram consumidos intent nem criados target, chave, CA, serviço ou CURRENT.

O coletor histórico continua preso ao hash anterior `0cf471896a5f40cc4319054ee35f6b980201ae77c29843470906bcbb35ef31dc`. Builds posteriores podem alterar o executável na pasta de build; o script/pin histórico não foi atualizado para reutilizá-lo. Não repetir esse comando para obter um novo resultado por conveniência.

## Consumidor CSR integrado

`src/provisioning/enrollment_issuer.c/.h` integra o trace/pair existentes ao **mesmo** issuer NORMAL. É um componente privado C90, sem novo owner, serviço, registro, listener ou callbacks de autorização selecionáveis pelo chamador.

1. HELLO/CHALLENGE/CONFIRM e `ep_trace_claim_pair` conservam a tentativa e a correlação originais.
2. `ep_trace_csr_candidate` fornece borrow da CSR copiada no trace, validando conexão, geração de transporte, request/nonces, fase e deadline.
3. `ep_issuer_submit` exige um registro CLIENT **previamente autorizado e reservado** no issuer NORMAL. Não cria aprovação/prepare/reserve a partir do APPROVED da rede ou da elevação administrativa. O core/backend verifica DER, digests e PoP e conserva as mesmas guards de assinatura.
4. `ep_issuer_poll` continua somente o pending do mesmo request/buffers. Não ressubmete CAS nem assinatura. Troca de conexão/geração, expiração ou Stop não liberam nem rearmam trabalho: retorna retenção sem parar globalmente outros contextos por inferência. Após Stop explícito do issuer pelo owner, permite apenas o poll factual do pending original, com referências/correlação preservadas e sem rearmar efeitos.
5. `ep_issuer_publish` pede a transição existente; autorização PUBLISH, commit/ack PUBLISHED v2 e confirmação continuam no root/core. Não confunde SIGN_REQUESTED/ISSUED_UNPUBLISHED com publicação.
6. `ep_issuer_deliver` exige PUBLISHED, consulta o registro/root corrente, revalida CSR/PoP, recupera e verifica o DER em quarentena, reconsulta policy/epoch/identidade e só então copia para o buffer destinatário. Falhas deixam esse buffer e `used` intactos. A cópia é única por trace (`credential_released`); replay não repete emissão ou entrega.

A geração numérica da conexão não é a geração16 da CA: a primeira é validada contra o pair, a segunda contra a identidade original do core/root. Nenhum campo remote/candidato substitui esses vínculos. A sinalização APPROVED no trace é DATA do protocolo, não aprovação administrativa.

## Validação pública separada de permissão de assinatura

O verificador Windows existente só aceitava SERIAL_RESERVED/SIGN_REQUESTED. Esse guard foi preservado na função original. O corpo matemático comum foi extraído para permitir `verify_request_data`: verificação pública de digest/SPKI/DER/PoP, sem autoridade e sem fabricar um estado SIGN_REQUESTED para um registro publicado.

O backend CNG fornece a porta privada opcional de prova pública; outros backends sem essa porta negam entrega. O core não usa essa porta para dispensar verify_request/authorize ou suas guards de fase. Não houve novo formato persistido, wire, algoritmo ou grant.

## Qualificação

- Selected/Legacy Debug/Release: **quatro builds PASS, zero warnings**.
- 20 CTests/configuração: **80 execuções PASS**.
- Novo consumidor: **77 verificações de modelo/configuração**, incluindo autorização negada, propósito errado, epoch/provas divergentes, conexão/geração, pré-publicação, replay, saída alias, porta pública ausente, PENDING/poll sem re-sign, expiração/Stop, CSR alterada e falha de release/retention.
- Jornada CNG: **89 verificações/configuração**. CSR cliente, CA autoassinada e leaf usam criptografia real efêmera; três assinaturas, sete verificações instrumentadas. A jornada passa pelo novo consumidor e codifica/decodifica o CREDENTIALS já aceito: leaf seguida da CA pública original, terminal e sem replay. Nenhuma rede ou instalação Browser foi executada.
- O guard original de fase nega verificar request publicado como etapa de assinatura; a verificação pública separada confirma PoP. Falha matemática preserva saída destinatária.
- Os 2.718 checks CNG e 1.405 checks enrollment DATA/configuração foram preservados. Oito fixtures v1/v2 anteriores são reutilizados; root/codec/CAS não foram reimplementados. Não foi repetida criação persistente nem aberta/excluída a chave retida.

O backend CNG passou de808 para816bytes pela porta privada. Sua validação de reserva usa sizeof e continua negando falta de espaço; limites inalterados. O layout canônico permaneceu Selected1.972.064/3.145.728 e Legacy1.803.496/2.097.152. Os fixtures não qualificam capacity provider/kernel ou picos do produto; esses custos continuam UNKNOWN. Trace/pair/quarentena são borrows do owner existente, sem alocação ou créditos concedidos pelo consumidor.

A publicação, aprovação administrativa, contexto TLS e Sessions dos testes continuam modelados. A matemática CNG é real, mas não constitui fonte produtiva de autoridade. O teste não prova continuidade MACHINE/reboot/isolamento por conta de serviço.

## MANUAL/E8 e Browser

Os bindings reais continuam sem produtores factuais dos três objetos E8 (pacote, observação administrativa e confirmação independente). `mp_auth_pdg_candidate` e Browser M1 não podem converter digest/CSR/receipt em trust lease. O build mantém SOURCE_ABSENT em TRUST antes do KDF, mesmo com matemática válida. Não foi instalado callback positivo nem criado owner paralelo para contornar isso.

O consumidor CSR é independente de NEW_INSTALL e foi implementado sem esperar instalação. Para uso operacional faltam o binding administrativo original, a fonte da CA DER/credencial serverAuth e a representação/integração do listener ENROLLMENT L2. CREDENTIALS recebidos ainda precisam da validação e instalação transacional do Browser; o buffer entregue não concede CONTROL/DATA ou capabilities. Esses pontos são pendências técnicas/factuais próprias, não justificativa para repetir o preflight ou abrir TPM/Q4.

M7 mantém o caminho já qualificado por identidade exata de credencial e preservação da credencial não relacionada. Stop solicitado não é close/quiescence; nenhuma conclusão adicional foi inferida.

## Recomendação de alvo e identidade

O proprietário informou que os campos **não foram escolhidos**. Após examinar F0–F9, bootstrap P3, preinitialization P0/P3/P4, a CLI e os namespaces existentes, a recomendação é:

| Campo | Candidato recomendado |
|---|---|
| Parent | `C:\ProgramData\PapinhoAccelerator` |
| Contêiner novo | `Personal` |
| Conta local padrão dedicada | `.\PapaccRuntime` |

Root candidata resultante: `C:\ProgramData\PapinhoAccelerator\Personal`. Não adota o parent existente como root publicada e não reutiliza `Protected\N0`. Nenhum desses valores foi designado ou criado. O nome da conta não é SID/token nem prova de existência ou custódia. A CA continua MACHINE KSP administrativa; o runtime deve ter negativa factual de uso privado.

A fundamentação, fontes Microsoft e alternativas realmente compatíveis estão em `RECOMMENDED-INSTALLATION-FIELDS.md`. A regra R0–R9 de virtual account é exclusiva do LAB fixo e não autoriza migrar SYSTEM/Administrators/conta interativa/NT SERVICE para este perfil pessoal por inferência.

## Primeiro gate e próxima operação

Primeira decisão pendente: seleção explícita do par parent/contêiner e da conta dedicada entre as alternativas permitidas. Depois, a menor observação necessária é metadado read-only do parent **selecionado** por referência original e da identidade/conta selecionada, no contexto administrativo legítimo; é diferente do probe TEMP já concluído. Nenhuma coleta equivalente precisa ser repetida.

Se a conta não existir, sua criação/provisionamento Windows exige autorização separada; não foi preparado comando que solicite senha ou altere ACL/SCM. NEW_INSTALL/CA_INITIALIZE real, CA MACHINE e CURRENT só podem ser pedidos após os gates completos. A confirmação destes nomes não autoriza tais efeitos.

A continuidade offline de E8/enrollment está detalhada em `NEXT-E8-AND-SELECTED-TARGET-READINESS.md`. Estado final: Phase6/ED11 PARTIAL; instalação NOT_EXECUTED; autoridades DENY. HEAD/WIP, históricos, pins, UNKNOWN, reservas, chave retida e PST0.6.4 preservados.

## Evidências

`OWNER-MANUAL-RESULT.json`, `PATH-CANDIDATE-METADATA.json`, `QUALIFICATION.json`, logs, preimages, `SOURCE-CHANGES.json`, delta e `ARTIFACT-HASHES.json` registram observações, alcance e hashes finais.
