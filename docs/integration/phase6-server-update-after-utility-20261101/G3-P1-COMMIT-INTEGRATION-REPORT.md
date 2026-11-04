# Integração offline — piso de commit G3-P1 8 GiB

2026-11-02. main/f341985924d29870d8ff627ed37c8a0874ad6469; HEAD/WIP preservados.
IMPLEMENTED_OFFLINE / QUALIFIED; sem efeitos operacionais.

## Decisão registrada antes de código dependente

DECISION-personal-scq-p1-commit-floor-20261102.md, em docs/adr, ACCEPTED:
B da proposta807ab4645cea56bad200e032e02bfa29c9de994f4eea91e2b13612948ec3a220
conferida integralmente. Precedência somente sobre commit9823326208 de QUERY/G3
e sua preservação na decisão de handles P1; novo8589934592bytes (8 GiB),
controlador e runtime P1 PERSONAL_LOCAL TRANSPORT_ONLY. Nenhum accepted anterior
ou candidato histórico modificado. Valor é risco administrativo, não suficiência
nativa/reserva/necessidade calculada. Sem conflito residual no escopo aprovado.

## Código e callers efetivos

- personal_scq_start_assurance_win32.h: constante privada P1_COMMIT_FLOOR8589934592.
- scqg_selected permanece9823326208; scqg_selected_p1 aplica8GiB e cap192.
- scqg_selected_exact_mode exige o novo valor exato somente se P1 explícito;
  sem P1 exige o conjunto anterior. A cópia local usada para comparar campos
  inalterados não modifica o plano original nem constitui fallback de perfil.
- scqg_sample verifica integridade do piso P1 e limites/initial; scqg_plan_data_mode
  rejeita commit divergente no preimage P1. Demais campos e formato84bytes intactos.
- Controlador: scqc_plan valida o conjunto antes de aquisição; seleção P1 original,
  caminho/token/SD/imagem/ancestrais/continuidade continuam gates independentes.
  personal_scq_start_plan_capture valida novamente por PAPACC_SCQ_P1_PATHS, e
  start_resource_current/ready/admit usam os mesmos limites congelados.
- Receptor: ingress_continue usa scqg_selected_p1 e compara policy com o carrier
  antes de QUERY; query_ready_mode/query_admit usam o novo piso. A política local
  QUERY é não autoritativa, não autentica o peer nem concede empréstimo. Originais
  e seleção da imagem/instalação continuam validados depois pelas mesmas rotinas
  P0–P7/OHI/bilateral/receipt/close; não se exige PID/token futuro antecipadamente.

READ/NEW_INSTALL/TO sem P1 e demais domínios não foram redirecionados para8GiB.
Factory/guard legacy permanecem; entrypoints READ/NEW_INSTALL não viram entradas
P1. O bool privado ou um argumento CLI não é autoridade: candidatos sem seleção
legítima continuam negando nos callers originais. Não há fallback por teto/peer.
Não foram alterados UPDATE/stager/SCM/transporte/CLI genérica/formatos ou owners.

## Bindings quantitativos preservados

P1: inicial128, guard188/cap192; físico7239172096; crescimento privado/WS1994944
bytes por participante; freshness100ms; janela240000ms/global60000ms; menor prazo
sem renovação; memória managed3MiB. READ/NEW_INSTALL/outros mantêm políticas e
piso9823326208 quando aplicável. Sem alteração em quotas de UPDATE ou SCM.
PreimageG3 continua84bytes, normaG0–G9 inalterada; apenas bytes do campo commit
P1 mudam. SHA256 anterior710b04a0990826b8a303f26b237d32f234fd152a93ba56ffb76519fec92c5bd5;
novo256fe6ab14dcc5554994b589017b1c8e7d7817bcdac89982d44d73048a31cafd.
Reprodução offline da serialização confirma o hash anterior observado nativamente
e o novo esperado. Comparação de preimages C e checks exatos rejeitam policy antiga;
o receptor calcula a mesma nova seleção antes de comparar. Hash não é autoridade.

## Builds e testes

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings finais, mesma
receita MSVC x64/W4/WX/PST0.6.4; logs g3-p1-commit-*. 68 pares (17/config) PASS.
Censo Selected2002152<3145728; Legacy1833584<2097152, inalterados. Nenhum handle,
objeto gerenciado ou reserva adicional.

Testes reais dos helpers com APIs protegidas simuladas: 8589934591 nega,
8589934592/8589934593 passam somente o gate pertinente P1, não readiness. O mesmo
sample fica abaixo do piso legacy; piso exato antigo passa legacy. Floor antigo
em P1, floor novo sem P1, teto divergente,0/MAX_U64/valor acima ou abaixo, janela
alterada, NULL/frequency0 e overflow de frequência negam. Parse CLI rejeita
uint64 overflow/zero/negativo. Quatro negativos no corpo scqc_plan negam ausência
ou divergência de seleção/piso antes de qualquer API nativa. Fronteiras de handles,
Stop/UNKNOWN/retenção/readiness prematura/replay/bilateral dos testes existentes
continuam PASS, assim como QUERY cold chain e modelos host product/lab/empty.

As primeiras execuções focais detectaram duas fixtures com premissas antigas:
P1 construído por factory legacy mais cap192 e sample legacy128 já bloqueado
pela guarda124. Foram corrigidas para usar factory P1 e sample legacy100,
sem alterar guardas ou enfraquecer testes de produção. Evidência final e hashes
em G3-P1-COMMIT-QUALIFICATION.json; campanhas alheias não repetidas.
Nenhuma consulta nativa de recursos/objeto instalado/SCM, RNG real, endpoint,
Start/Stop, UPDATE ou operação mutante foi executada.

## Artefato e primeiro gate

Novo Selected/Release786944bytes, SHA256
40b708daadae4342e5cd90090d99189234437ab84f47b2d00f76aeaab05eba76,
congelado papacc_server.g3-p1-commit-786944.exe. Mesmo tamanho, bytes diferentes.
Instalado historicamente786944/ID417614100:917504:303885/pina975ec0303c6c04d7d6e48ae1ebf803976b47a4f42c1996affc1eab21d8e0ce0
continua anterior; não presumir que source compilada esteja instalada. DLLs
idênticas e updater protegido histórico permanecem selecionados; gates reais
futuros revalidam identidades/proteção/continuidade e SCM STOPPED/PID0 fresco.

Pedido ONE-G3-P1-COMMIT-IMAGE-UPDATE-REQUEST.CANDIDATE.md pronto para decisão
operacional específica, sem autorização de execução nesta task. Mesmo UPDATE
U1–U7/U2, um candidato exclusivo/copy/hash/flush/dois renames sem overwrite,
anterior retido/readback/close. Retenção histórica e OS UNKNOWN; sem limpeza.
Não emitir comando executável de operação não autorizada. Após aprovação/UPDATE
qualificado, sessão E/Q/S independente com novos originais e binding ao ID real,
novo commit8589934592 no argv/policy e três confirmações. Não reutilizar f87d9281...
consumida. Gates/bilateral/receipt/close/RUNNING diagnóstico posteriores não são
PASS pela nova tolerância. Produtivo DENY, ED11/Phase6 PARTIAL.

Próximo prompt NEXT-G3-P1-COMMIT-UPDATE-AUTHORIZATION.md. Preservar todos os
históricos/pins/EXEs/originais/retidos/reservas/atos consumidos/fixture/PST0.6.4.
