# Phase 6 — integração funcional, 2026-10-28

TASK PACC-PHASE6-ISSUER-TRUST-ENROLLMENT-FUNCTIONAL-INTEGRATION.
Entrega funcional parcial, com código e qualificação novos. Não é fechamento da Phase6 nem qualificação positiva dos callers operacionais ainda sem fontes. Accelerator main f341985924d29870d8ff627ed37c8a0874ad6469 preservado; Browser canônico somente lido, HEAD96f6cd2f9ec4f834999828fa13bb7d170db66a98. Nenhum ADR accepted ou pin operacional alterado.

## Código implementado e callers atingidos

### Issuer NORMAL separado de RECOVERY

issuer_core.h/.c passam a representar explicitamente a continuidade privada do view/core. O init existente mantém o perfil estrito de recuperação0 e seus31 bits. papacc_issuer_init_normal seleciona somente o perfil pessoal NORMAL1: CURRENT, key binding, prefix/history, história original, exclusividade e serials duráveis precisam ser fornecidos pelo root port. Perfil, provas completas e ausência de bits de recuperação são revalidados em cada load. Perfil não pode mudar depois do init. Ausência de fato ou mistura de máscaras nega e preserva UNCERTAIN, sem preencher provas.

O caminho NORMAL executa os mesmos prepare/reserve/issue/transition/CAS/re-read do core. papacc_issuer_recover rejeita NORMAL antes de autorização ou efeitos. Os bits COMPLETE_CHAIN/FENCED_HIGH_WATER não foram reinterpretados. Modelos exercitam emissão/publicação/revogação, conflito de perfil, falta de cada prova, mistura de provas e falha pós-CAS; produtores dos modelos continuam explicitamente sintéticos, não fontes de autoridade do produto.

Integração efetiva de consumidor: server_issuer_cng_backend_win32.c usa o MESMO papacc_issuer_view_proofs_valid do core, deixando de exigir31 incondicionalmente. Seu view_equal também compara perfil/provas antes do efeito. A jornada modelo do backend verifica que NORMAL consegue passar pela composição existente sem falsas provas de recuperação, que mistura de provas bloqueia antes da assinatura e que recover permanece DENY. Assinatura, key borrow e root dos testes CNG são modelos; há parsing/encoding/hash públicos nativos, não assinatura privada nativa.

**Limite importante:** issuer_root_port_read/cas/issuer_publish_live não receberam um produtor positivo artificial. O port nativo ainda deixa epoch/proofs ausentes e publicação síncrona completa indisponível. Não foi implementada a aquisição nativa completa de história/ator/chave/custódia/exclusividade nem a continuação do core síncrono para commit/publicação assíncronos. Portanto a nova regra NORMAL está integrada no core e no consumidor CNG, mas emissão/publicação operacional continua DENY. Testar o hook de armazenamento não supre esses producers.

### Revogação M7 no core

O root port privado ganhou revoke_dependents, fornecido exclusivamente pelo mesmo owner administrativo. No perfil pessoal NORMAL, ausência desse port nega a transição ANTES do commit. Após REVOKED durável e re-read, o core chama o port e volta a conferir registro/revisão/epoch/high-water/certificado. Falha bloqueia novos efeitos; RETAINED mantém retenção; não há retry automático. Registro já revogado não retorna sucesso só por uma flag: exige revalidação do fanout latched pelo owner, sem repetir Stop/release.

Sucesso desse port significa invalidar admissões dependentes e solicitar Stop sob seus owners; não significa quiescência/fechamento. O callback modelo comprova ordenação e latch de falha, não fanout real. **A ligação concreta credencial→Sessions/operações do servidor continua não implementada e M7 não está qualificado ponta a ponta.** Nenhum callback permissivo foi instalado na rota nativa.

### Composição de enrollment

ep_trace_claim_pair integra os helpers existentes de trace e claim sob o mesmo owner e conexão/geração originais. Confere challenge/request/nonces, framing/ordem e código; o primeiro CONFIRM decodificado e bem pertencente consome a tentativa, mesmo se ordem/código/conexão falharem. Replay não reexecuta claim. Stop antes da confirmação suprime o efeito. Referências sobrepostas são rejeitadas; nenhuma alocação, socket ou owner novo foi criado.

Testes compõem HELLO→CHALLENGE→CONFIRM com os algoritmos reais, exercitam ordem, código, nonce, tamanho, conexão, expiry, Stop e replay. O resultado continua EP_DATA_ONLY: não é password validada, aprovação administrativa ou autoridade de emissão. Os layouts persistentes, listener mTLS operacional e perfil PST não foram alterados.

**Limite:** não houve registro de listener ENROLLMENT, ligação de CSR ao issuer ou entrega de credencial no Browser. A coleção atual ainda contém somente SECURE_PRINCIPAL/LEGACY_ENDPOINT. Ver a fronteira normativa abaixo; nenhum enum ou formato antigo foi reinterpretado.

## Builds, regressões e fixtures

Quatro composições novas e isoladas em build/phase6-functional-20261028: Selected/Legacy Debug/Release, MSVC x64, normal API3/SPI4 OpenSSL, receitas preservadas. papacc_server e targets focais compilam; builds finais sem warnings. As opções e SDK são os mesmos pré-requisitos governados dos builds root existentes; não se habilitaram pins N0 positivos ou operações LAB.

Sete CTests focais passaram em cada configuração: issuer offline, production gate, checkpoint, CNG X509 model, CNG production gate, enrollment DATA e perfil PST DATA. Asserções e outputs completos estão nos logs e LastTest; quantidade não equivale à qualificação nativa do produto. Exemplos discriminantes: NORMAL não chama recover; adapters test_only continuam rejeitados antes do root; core/backend concordam no perfil; falta de fanout nega antes de commit; fanout incerto bloqueia sem repetir; CONFIRM não pode ser reivindicado em outra conexão.

20 execuções adicionais do fixture nativo existente, cinco modos por configuração: native-history, native-snapshots, pending-conflict, pending-unknown e pending-stop. Usam arquivos NOVOS sob build, SHA256/TxF/handles/workers reais e metadados sintéticos. UNKNOWN é fault injection explícita. Nenhuma fixture é root operacional e nenhum estado retido/saída de processo virou crédito. Arquivos e outputs permanecem para evidência, sem cleanup. Native snapshots/persistência não demonstram assinatura, mTLS, readiness do serviço ou autoridade CURRENT produtiva.

Falhas iniciais preservadas: o fixture standalone compilava commit diretamente e não ligava helpers pessoais; a primeira correção expôs três referências CA no perfil Legacy. CMake agora inclui os mesmos personal_plan_data/server_ca_key e ncrypt/advapi32 nesse fixture. Não se usaram stubs permissivos. Também foram acrescentadas dependências explícitas issuer_core.h para os consumidores indiretos do layout no MSVC/Ninja.

Não foram repetidos E5/E11/Browser/NT4 ou campanhas sem mudança pertinente. Não houve edição Browser, então VC6/NSS não foi reconstruído; essa entrega não concede qualificação VC6/NT4.

## Recursos e formatos

Censo compilado contra preimagem e fonte atual x64: core216→224 bytes; root_port40→48; view176→176. O port está embutido no core: não somar ambos os deltas para a mesma instância. O backend mantém cópia própria do port e seu orçamento existente calcula sizeof(backend)+sizeof(core)+key/workspace; os testes Selected/Legacy preservam seus limites. Não se declara novo orçamento total/transitivo/stack/provider/kernel.

Enrollment não ganhou campos persistentes: reader64, trace4272, pair160, limiter1128 na composição x64 observada. Função nova não cria objetos nativos. Masks/perfil são privados em memória; issuer_codec continua zerando view e não serializa essas provas. Nenhum BODY/ROOT/certificado/wire/SDK público foi migrado.

## Frentes bloqueadas e fundamento exato

**E8 positivo:** DECISION-factual-evidence-e0-e8-lifecycle-20261021 incorpora E8; implementation-proof-and-source-gate identifica a obrigação de demonstrar funções reais de aquisição root/policy/issuer/TLS e confirmação independente ANTES de tipos/fábricas positivos. sp_trust_acquire continua binding negativo; mp_auth_pdg_candidate continua SOURCE_ABSENT; pb_m1_step continua parando antes de KDF. Não há fonte factual legitimamente adquirida para transformar em lease. Isso bloqueia a integração positiva, não autoriza passar modelos como trust real. M0–M8/F1–F5/G0–G7 não precisam ser reaprovados.

**Enrollment persistente:** DECISION-enrollment-listener-adr0009-specialization-20261020 L2 diz: “Ausência de representação versionada aceita deve negar persistência/aplicação até extensão específica”. A mesma cláusula não aloca enum/codec/wire e proíbe modificar BODY0.5/ROOT06/D3/LST1/ROOT08 por inferência. O header atual server_listener_configuration.h só representa os dois perfis originais. A autorização funcional não aceita bytes novos de formato por si só. A decisão focal restante é a representação exata do propósito no modelo/codec existente, não uma nova escolha de ALPN, código, listener separado ou família de ADRs.

**D4/D5:** cinco parâmetros ainda carecem de fundamento/decisão: idade, validade, duração global e pisos físico/commit. Os snapshots/getters existentes continuam sem policy; não se implementou setter fictício. Nenhum número LAB foi importado. Mantém-se o anexo quantitativo do pacote offline anterior, sem apresentá-lo como política completa.

**Publicação nativa/issuer e M7:** ainda exigem implementação e fontes reais no mesmo root. Não há evidência de root operacional/leases/CA originais ou confirmação Browser nesta tarefa. Fixtures não satisfazem esse binding. Há ainda uma diferença concreta de interface a resolver: o core CAS é síncrono, enquanto begin/poll/publish_live são fases do worker; o callback atual não pode devolver OK apenas por staging/commit. Este trabalho não disfarçou essa diferença de UNCERTAIN como sucesso.

## Caminho crítico atualizado

1. Concluir producer NORMAL nativo no root e a continuação de CAS/publicação assíncrona, com prova original de chave/história/serial/exclusividade e acknowledgements reais, mantendo o novo consumidor separado de RECOVERY. Incluir original-root native fixtures e concorrência; nenhuma root produtiva será publicada automaticamente.
2. Vincular o port M7 ao grafo real credencial→admissões/Sessions/operações, com invalidation/Stop e receipts separados de closure. A interface e o gate já existem; não fornecer callback que apenas retorna OK.
3. Depois das aquisições factuais necessárias, implementar E8 borrow-only com confirmação independente do consumidor e pipeline M1 existente; nenhuma permissão Browser de escrita foi solicitada nesta etapa porque ainda não existe binding positivo legítimo a integrar ali. Se um patch Browser se tornar concreto, usar a permissão de workspace apropriada, sem outro checkout.
4. Resolver a representação versionada ENROLLMENT delimitada e integrar runtime/coleção/owner aos helpers compostos; manter OPEN/CLOSED/approval/CSR reais e mTLS operacional isolado. Não reaproveitar enum antigo.
5. Em paralelo fechar D4/D5 fundamentados e preparar composição uma operação, certificados/GUI/Mode D/Legacy/NT4. Não há pedido operacional completo ou comando positivo nesta entrega.

E17/E18/E19/E20 e E21 permanecem nos seus escopos factuais pendentes; E22 é derivado. Não fechar gate por API nova ou fixture. ED11/Phase6 PARTIAL; autoridades DENY. Q4/TPM/ownerAuth não foram retomados, UNKNOWN/reservas preservados, nenhum Start/Stop/SCM/ACL/rede operacional/intent/CA produtiva/CURRENT produtivo/signature/recovery/release ocorreu.

[Próximo prompt técnico](NEXT-FUNCTIONAL-CONTINUATION.md). Hashes e patch relativos às preimagens desta tarefa estão em FINAL-VERIFICATION.json e FUNCTIONAL-DELTA.patch; não confundir o patch do WIP total com o delta desta implementação.
