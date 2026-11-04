# Carrier inicial privado — implementação e limites factuais

Task: PACC-PHASE6-PRIVATE-BOOTSTRAP-CARRIER-IMPLEMENTATION. Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469. WIP anterior preservado. Nenhuma aceitação normativa, seleção operacional ou autorização operacional foi criada nesta execução.

## Código e callers integrados

O carrier SCQ1 tem 308 bytes de campos tipados, representados por 621 caracteres ASCII canônicos e NUL (622 posições). Contém tentativa, propósito TRANSPORT_ONLY, controlador PID/criação, identidade pretendida do runtime, digests de escopo e prazo original. Não contém handles, ponteiros, chave, segredo, token ou grant. Posse e decodificação não concedem autorização. scqa_carrier_scope e scqb_scope confrontam todos os campos com uma seleção original independente, inclusive a janela original, sem aceitar prazo escolhido pelo carrier.

server_service_data implementa o codec e a seleção DATA service start --scq-closed. server_service_win32/ps_main recebe apenas argumentos transitórios tipados [nome SCM, scq-to, carrier]; o comando SCM persistido permanece intocado. O callback fechado captura DATA no packet existente e continua START_PENDING. Nenhuma referência remota é reconstruída a partir de um ponteiro.

scqt_reserve fixa o namespace privado antes de existir PID do runtime, usando a origem SCQ consumida e o ato Start confirmado. Reserva não significa conexão, autenticação ou prontidão. A autorização independente de endpoint continua necessária. O owner SCM revalida serviço parado, configuração e imagem originais e toma o ato Start imediatamente antes da única chamada StartServiceW. O perfil LAB rejeita esse caminho.

personal_scq_bootstrap_win32.inc acrescenta captura inicial no MESMO adapter/pipe/packet. SQB1 leva a projeção SCQ existente e três identificadores da tabela de handles destino obtidos pelo DuplicateHandle do controlador. São candidatos, sem autoridade. Antes da promoção, o receptor confronta seleção independente, identidade kernel do peer, processo/criação, token e grupos/restrições, direitos exatos e não herança. Depois exige aquisição original, conteúdo/descritor e correlação da imagem. Só então scqr_first_prepare e scqb_promote entregam os objetos ao receptor comum. Os formatos ordinários SQR1/SQC1 e N0 não foram alterados.

server_configuration_personal_native liga captura, validação, promoção e progressão aos callers reais do launcher/ServiceMain. O controlador liga o processo SCM observado tardiamente e progride I/O parcial/pendente sem nova entrega ou consumo. Receipt, close original e retorno continuam necessários antes de RUNNING diagnóstico. Esse ramo finito fecha o conjunto controlado e termina; não chama ControlService para Stop e não anuncia readiness produtiva. Falha, perda de peer ou fechamento inconclusivo retêm UNKNOWN.

O bridge de console acrescenta empréstimo temporário do evento original para despertar por Stop. Não cria evento nomeado ou owner. Callback ainda em uso ou erro de sinalização impede declarar fechamento; o censo inclui os 24 bytes estáticos do bridge Win64.

## Evidência offline e alcance

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings de compilação. Cada configuração passou 24 CTests afetados: 96 pares teste/configuração PASS, sem repetir campanhas históricas inalteradas. Quatro testes focais também foram executados com saída detalhada em Selected/Debug (não somados novamente aos 96). Logs desta pasta; EVIDENCE.json contém pins dos artefatos de qualificação. São APIs de efeito simuladas, sem endpoint, Start/Stop, SCM/RPC mutante, RNG ou KSP reais. Não foi executado papacc_server para provar um efeito.

scq_bootstrap_carrier: 73 verificações, codec e entrada real ps_main com APIs simuladas, namespace pré-runtime e binding tardio. scq_bootstrap_capture: 20 casos, 776 verificações, seleções e vistas de runtime locais distintas, receptor inicialmente sem inputs/profile/originais. Três trajetórias chegam ao fechamento conhecido; outra chega ao ponto anterior à falha de fechamento injetada. Testa fonte ausente, carrier/endpoint/SID divergentes, identidade/token, candidatos inválidos ou excessivos, parcial/pendente, digest divergente, replay, Stop, peer perdido, receipt adulterado e close UNKNOWN. A fronteira CNG/conteúdo/SD/imagem nesse teste é explicitamente MODELADA; não prova autenticação bilateral nativa. console_original_wait_model cobre sete casos sem wait/event real. A regressão histórica de fronteira mantém 28 verificações negativas.

A circularidade de disponibilidade foi resolvida EM MODELO: seletor antes do runtime → captura de candidatos sem leitores previamente preenchidos → validação independente → promoção → receipt/close → readiness fechada. Não há teste que autorize dados recebidos por compartilhamento de ponteiros entre os dois principals.

## Primeira lacuna concreta e DENY

A composição nativa ainda NÃO está operacionalmente completa. personal_scq_registered_owner_decision e personal_scq_registered_endpoint_decision continuam retornando NULL. O código de captura requer seleção original independente no runtime antes de abrir o endpoint. O modelo fornece essa seleção local distinta, mas esta execução não implementou seu producer nativo legítimo pré-captura. Esse é trabalho funcional remanescente, e não algo que uma aprovação no chat preencheria automaticamente. Não foi demonstrada impossibilidade normativa nem necessidade de nova ADR. Não se pode substituir esse producer por metadata do carrier, setter/grant ou callback positivo.

Além disso, papacc_server_service_diagnostic_start_status mantém PS_DIAGNOSTIC_START_NEEDS_RESOURCE_PLAN e retorna DENY. R0–R9 é registro; B0–B8/B3 é staging. Nenhuma dessas garantias foi importada para Start/SCQ. Fonte original e capacidade são lacunas separadas. O corpo SCM positivo foi compilado, mas sua execução positiva completa pelo CLI continua bloqueada e não foi demonstrada por esta campanha.

Os direitos Windows efetivos do runtime virtual sobre o processo/token administrativo, a autenticação bilateral real, o escalonamento entre close e saída e a quiescência OS permanecem não demonstrados. Guardas conservadoras mantêm UNKNOWN em vez de afrouxar validação para PASS.

## Imagem e próximo gate

O EXE de qualificação mudou; o EXE staged histórico continua c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf / 571392 bytes. Não foi consultado ou substituído nesta execução. O delta e o procedimento derivado estão em STAGED-IMAGE-UPDATE-PROCEDURE.CANDIDATE.md. Nenhuma imagem final foi selecionada operacionalmente: resolver o producer e a admissão pode alterar novamente o binário.

Pedido Start: NOT_READY. Pedido de update executável: NOT_READY; o stager qualificado cria objetos ausentes, não autoriza sobrescrever imagem existente. Próxima ação técnica em NEXT-NATIVE-ORIGINAL-SELECTION-AND-START-ADMISSION.md. Depois: qualificação final → seleção/pedido separado de update → reconciliação do update → pedido específico de endpoint e Start diagnóstico. Sem comandos operacionais ou placeholders.

E21 PENDING; ED11/Phase 6 PARTIAL. Autoridade produtiva DENY; OS_QUIESCENCE UNKNOWN. Serviço registrado/parado somente na evidência histórica; não houve nova consulta SCM. Autorizações consumidas, pins, reservas, chave fixture e produção PST 0.6.4 preservados.
