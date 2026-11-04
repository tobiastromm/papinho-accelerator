# Entrega original de Start — integração offline

Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservados. WIP anterior preservado; alterações desta task são descritas por símbolos, não pelo diff global acumulado. Nenhum Start/Stop, endpoint real, SCM/ACL, staging, CA/CURRENT ou coleta operacional foi executado.

## Resultado e alcance

Integração positiva OFFLINE acrescentada, não apenas relatório: ato local independente de Start no mesmo PAPACC_SCQ_ADMIN_SOURCE; composição de aquisição inicial no receptor do mesmo packet; integração dessa composição ao caminho nativo papacc_configuration_personal_scq_recipient_prepare; cadeia real de helpers exercitada com fronteiras Windows simuladas. Não foi concluída uma entrada operacional executável de ponta a ponta para o ServiceMain. Pedido de Start NOT_READY.

## Código e callers

- personal_scq_admin_win32.h/.inc: start_phase/start_first/start_consumed pertencem à fonte administrativa existente. scqa_start_present/current/confirm/take revalidam originais, controlador, alvo, TRANSPORT_ONLY, janela e Stop. O pedido local de Start toma o plano original imutável e acrescenta a operação específica apresentada; confirmação OWNER_SCQ_DIAGNOSTIC_START é independente de OWNER_SCQ e OWNER_SCM_REGISTER. Não há novo formato persistido ou transporte de grant lifecycle. O nonce já vinculado ao plano é separado por domínio de confirmação, não uma nova identidade futura.
- server_configuration_personal_native_win32.inc/.h: produtor console específico start_confirm_console e consumo start_take, com elegibilidade administrativa, console original e revalidação nativa existentes. Não chamam StartService e não dispensam seleção operacional original, readback SCM, plano de recursos ou autorização de endpoint.
- personal_scq_session_win32.inc: um ato de Start explicitamente apresentado precisa estar consumido antes de offer. Depois do consumo, close do controlador exige close_accepted e a perna original de retorno; quiescent DATA não basta. Falha conserva retenção/UNKNOWN.
- personal_scq_recipient_win32.inc: scqr_first_prepare compõe dispatch, profile, scqr_acquire e session driver em um receptor inicialmente vazio. Requer runtime original já observado em START_PENDING, inputs QUERY/READ já adquiridos e verificados independentemente e decisão original imutável; verifica scope/runtime/FileIDs/content/SD antes de usar. Não recebe ponteiro remoto nem aceita números de HANDLE como autoridade. Falhas parciais não zeram o packet para retry.
- papacc_configuration_personal_scq_recipient_prepare agora encaminha TRANSPORT_ONLY/START_PENDING por scqr_first_prepare após capture_closed, aquisição/conteúdo dos originais e correlação da imagem/observação nativa. READ e demais caminhos continuam no percurso anterior.
- tests/scq_session_driver_test.c: observação do runtime separada da criação do receptor para testar a primeira aquisição sem receptor/profile/session prepopulados.
- tests/scq_pre_readiness_chain_test.c: fonte administrativa modelada independente; confirmação de Start, ambos os consumos, único efeito Start modelado, observação tardia de PID/token, primeira aquisição do receptor, adapter, receipt, close, retorno e só então RUNNING diagnóstico. Readiness produtiva continua negada.

A confirmação local de Start não substitui uma aprovação operacional específica do proprietário. O propósito SCQ transportado continua exclusivamente diagnóstico; a autoridade lifecycle é consumida e fica no controlador, sem transferência reutilizável ao runtime. Nenhuma aprovação de registro ou staging utilizada entra nesse percurso.

## Qualificação

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings/errors. Onze CTests afetados por configuração, 44 pares distintos PASS: sete regressões SCQ, três modelos de host e censo de memória. A última revisão de assertions foi recompilada e a cadeia repetida nas quatro configurações; não é uma campanha operacional.

Cadeia final: 57 cenários, 36 TRANSPORT_ONLY, 2.359 verificações/configuração; dez negativas específicas do ato independente, oito cenários de readiness fechada entre READ/TRANSPORT_ONLY, 39 retenções. Cobre fonte ausente, confirmação de outro domínio/tentativa, cópia da fonte, controlador divergente, replay/consumo duplicado, restart, decisão alterada, expiração, Stop, peer divergente/perdido, receipt incompleto, close divergente/incerto e ausência de RUNNING prematuro. Reentrada first_prepare é negada sem segunda aquisição/consumo.

RNG, processo/token/SCM, aquisição/conteúdo de originais, endpoint/IO e KSP das jornadas são MODELOS. Helpers SID/SD existentes e teste de thread não comprovam autenticação bilateral real. Nenhum endpoint real é criado ou conectado. qopens/qproviders permanecem zero na cadeia. A contagem de testes não substitui qualificação nativa.

Censo algébrico final: KNOWN Selected=1.989.320 de PROCESS_LIMIT=3.145.728; Legacy=1.820.752 de 2.097.152. Não são pico de processo, margem física/commit, custo RPC/provider ou capacidade operacional concedida. Nenhuma quota alterada; 124/128/1024 e planos próprios SCQ permanecem. R0–R9/B3 não são política de Start.

## Primeira lacuna ainda real

A nova composição começa com originais legitimamente adquiridos. Ela não torna disponível a primeira origem remota por simples metadata. personal_scq_registered_owner_decision e personal_scq_registered_endpoint_decision continuam NULL, sem aprovação operacional original. Na entrada atual do launcher, captured_empty ainda representa ausência de fonte e encerra vazio sem readiness; não foi convertido em fonte positiva.

Portanto há uma ligação nativa ainda não fechada: disponibilizar no packet recém-criado do ServiceMain a aquisição inicial governada que alimenta recipient_prepare, sob escopos originais independentes de Start e endpoint. O helper e seu caller agora existem e são testados; a ativação/acquisition desse primeiro ingresso, a correlação dos originais administrativos e o vínculo ao SCM owner ainda não foram demonstrados. Não declarar que apenas autorizar Start resolveria tudo. Não foi demonstrada impossibilidade normativa; SSO-B cobre integração offline, mas não permite inventar aprovação nem usar arquivo/flag/ack DATA como fonte.

O gate produtivo/CLI continua DENY. Não há retorno positivo novo em papacc_server_service_startup_source_status, nem fallback para abrir serviço ou executar Start pela GUI. A ausência deliberada de decisão operacional não deve ser resolvida por alterar NULL para constante permissiva. Recursos de Start, aquisição inicial e lifetime entre os dois owners de processo precisam de caminho original completo antes do pedido operacional.

## Artefatos e atualização

Runtime staged/registrado permanece c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf, 571392 bytes; referência histórica original, não nova atestação de imagem mapeada. A cópia preservada no workspace foi novamente conferida, sem abrir o serviço.

Novo Selected/Release: 890dd9276993f8cf80b089f7dd33d5feb7ac71d0e8af45d2ef4740e067fb22db, 582144 bytes, apenas candidato de build. Não selecionado operacionalmente. Contém os novos helpers/estado e caller de aquisição; o staged c0 não os recebe ao recompilar o workspace. A qualificação dessa nova composição no runtime exigirá atualização governada, se esse for o artefato finalmente selecionado após fechar o ingresso. Ver STAGED-IMAGE-UPDATE-PROCEDURE.CANDIDATE.md. Não substituir antes de estabilizar a implementação.

## Caminho mínimo e estado

1. Concluir a ligação original do primeiro ingresso no packet vazio e do ato Start ao SCM owner, usando os helpers agora integrados e modelos existentes; não reimplementar originais, adapter ou registration.
2. Qualificar essa ligação e estabilizar um único candidato; verificar mecanismo existente UPDATE para imagem protegida, sem reutilizar criação exclusiva do staging anterior.
3. Preparar atualização protegida, endpoint e lifecycle como efeitos separados, cada qual com pedido/identidade/consumo próprios e gates frescos. Não pedir PID/token futuro antecipadamente.
4. Somente com código/bindings/plano próprios completos, apresentar pedido byte-exato de Start para decisão específica. Sem comando prematuro.

E21 PENDING; ED11/Phase6 PARTIAL; OS_QUIESCENCE UNKNOWN. Registro parado mantém STOPPED_REGISTRATION_QUALIFIED no alcance histórico R0–R9. Não inferida instalação produtiva, continuidade SCM atual ou fechamento OS por status/exit. Preservados históricos, pins, autorizações utilizadas, reservas, chave fixture e produção PST0.6.4. Nenhuma ADR criada ou accepted alterada; direções futuras de listener/recovery não foram incorporadas como bloqueios.
