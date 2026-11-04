# TO0–TO9 — aceitação e integração offline do transporte SCQ

TASK PACC-PHASE6-TO0-TO9-ACCEPTANCE-AND-OFFLINE-INTEGRATION, 2026-10-30.
Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservados.

## Decisão normativa registrada antes do código

[Registro accepted](../../adr/DECISION-personal-scq-transport-only-to0-to9-20261030.md)
incorpora exclusivamente TO0–TO9 do candidato SHA256
0702a2e733024118bdad7c0993b4993e20cf44e825a872ad50245e8fd2b45d30.
Proposta original e accepted anteriores preservados byte-exatamente.
A autorização explícita desta task é normativa/técnica, não operacional.
A exceção focal às passagens de chave OHI e B3 não reinterpreta SSO-B,
F0–F9, NEW_INSTALL, CA_INITIALIZE ou outros perfis. Nenhum conflito novo
identificado. N0/T0–T14 não foram alterados. Direções futuras de listeners e
backup/recuperação foram revistas e permanecem independentes, sem mudança.

## Código integrado e callers

- personal_scq_profile: propósito2 TRANSPORT_ONLY e construtor tipado
  papacc_scq_profile_prepare_transport_data. Campo key integralmente zero,
  inclusive após terminador. READ1 mantém scqp_key. Construtores ordinário e
  fechado READ e codecs ordinários rejeitam propósito2; a rota de transporte
  exige preparo fechado Pending. Record1048/SQR11052/SQC11076 permanecem iguais.
- Dispatch mantém discriminador privado próprio, sem reutilizar erro/autoridade.
  papacc_personal_scq_prepare_transport exige runtime virtual em preparo fechado,
  associação não vazia e WCHAR[112] inteiramente zero. personal_scq_dispatch_body
  nega qualquer propósito diferente de READ antes do provider, inclusive com
  nome KSP válido e metadados closed substituídos. Não executa probe de transporte
  nem relata uma negação KSP como fato nativo.
- Admin/handoff/receptor/driver/adapter correlacionam o mesmo propósito original,
  registro, plano, destinatário e key. scqa_current revalida propósito e todos os
  WCHAR do seletor contra o plano antes de novos efeitos; a captura associa a
  seleção do plano já validado, sem criar aprovação. scqr_acquire deriva o
  propósito do plano, não hardcode READ. Revalidação independente nas duas pontas,
  consumo, token/grupos/restrições, PID/criação, imagem, direitos e lifetime
  permanecem; nenhum deles é autorizado por metadata/fixture.
- Owner canônico expõe papacc_configuration_personal_scq_profile_prepare_transport_data
  e conecta offer_data e recipient_query_prepare aos construtores tipados.
  Public/runtime CNG probe e dispatch negam TRANSPORT_ONLY. Os três getters de
  decisão operacional continuam literalmente NULL. Nenhum loader/setter/grant
  permissivo foi instalado. Caminho READ e perfil SAM não foram dispensados.
- A cadeia original preserva offer → aquisição independente → receipt SQR1 DATA →
  ack controlador → produtor SQC1 fecha referências entregues → entrega/validação
  original de close → readiness fechada. Readiness produtiva continua negada;
  receipt DATA não virou autenticação nem quiescência. Stop/peer perdido/UNKNOWN
  conservam consumo e retenção, sem retry, liberação por timeout ou cleanup.

## Qualificação e limites da prova

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings finais.
12 CTests afetados/configuração, 48 execuções finais PASS. Logs TO9-* e
TO9-QUALIFICATION.json registram a evidência, sem repetir campanhas históricas
independentes. Houve um warning de variável não usada detectado por /WX durante
edição e corrigido; resultados stale após aquele build falho não são evidência
final. O helper de mutantes foi corrigido para preservar finais de linha antes
dos três resultados válidos; nenhum mutant ficou no código final.

Cadeia integral:47 cenários,26 TRANSPORT_ONLY,1980 verificações/configuração,
8 readiness fechadas e39 retenções modeladas. Inclui fonte/identidade/alvo
substituídos, propósito original/dispatch divergentes, key após terminador,
replay, Stop, perda de peer, close incompleto/IDs substituídos e UNKNOWN.
Perfil privado:3470 verificações, incluindo cada um dos112 bytes de key,
READ sem key, propósito/N0, direitos excessivos e thread concorrente.
Dispatch:1284 verificações, incluindo cada WCHAR do seletor e entradas diretas
que não alcançam KSP mesmo com nome válido e contexto closed alterado.

Três mutantes compilados e rejeitados: aceitação de key não canônico; remoção
da barreira KSP do dispatch; remoção de revalidação do propósito administrativo.
Fontes restaurados e recompilados antes dos resultados finais.

Token/processos/SCM/originais/decisões/IO/RNG/KSP são modelos explícitos.
Helpers SID/SD e testes de thread/compilação são nativos; não provam o par
administrador/runtime operacional. Zero endpoints reais criados/conectados;
nenhuma sessão SCQ, Start/Stop, KSP/CNG real, CA/CURRENT ou operação protegida.
Zero chamadas de provider nos percursos TRANSPORT_ONLY modelados, com negativos
que detectam sua remoção. Os testes READ exercitam seu probe substituído por
modelos, não um provider real. Autenticação bilateral nativa NOT_DEMONSTRATED.

## Censo, política e hashes

Discriminador privado: dispatch352 bytes, crescimento compilado8 bytes incluindo
alinhamento. Censo managed Selected1979784/3145728 e Legacy1811216/2097152;
SCM separado12924, somas estáticas1992708 e1824140. Não há crédito por key vazio.
scqs_current usa snapshot local de plano1048 bytes: isso não mede frame/stack
transitivo, provider/kernel ou pico de processo. Não declarar envelope completo,
headroom físico/commit ou limite universal com esses números.

TO9-QUANTITATIVE-ANNEX.json mantém valores não fundamentados como null:
janela do ato, validade/idade de coleta, freshness, duração total, encerramento,
headroom físico/commit e custos ativos. Aponta gates/fontes e metodologia por
fase, relógio, ownership, compartilhamento/deduplicação e retenção. Ausência de
KSP não zera custos públicos hash/RNG/SCM/pipe/kernel. Não se importou Browser/
D4–D5/LAB nem converteu amostra/modelo em teto nativo. Nenhuma medição nativa
protegida foi executada para preencher números por inferência.

Seleção antiga90123fbadc3d7e4db1e5f48ea2b7fe85f6e78f61f6adee083b00d1288b951874
preservada em TO9-Selected-Release-PLANNING-BASELINE.exe; não executada.
Novo Selected/Release candidato: `e013dd67bcec10b8d5a5231a249ea56bf7464c33f3cf79eba50e85806027f582`, 520704 bytes.
Não selecionado pelo proprietário, instalado ou atestado. Todos os quatro
hashes derivados: TO9-CANDIDATE-ARTIFACTS.json. Registro, fontes, logs e documentos:
TO9-SOURCE-HASHES.json e TO9-ARTIFACT-HASHES.json. Não substituem identidade original.

## Primeira fronteira real e caminho até NEW_INSTALL

Pedido operacional NOT_READY; sem comando ou autorização criada. Antes de um
pedido completo falta selecionar explicitamente o artefato derivado e fundamentar
a política SCQ, obter pedido/decisão originais e sua confirmação atribuída no
owner, e comprovar a continuação nativa do runner. A aceitação TO0–TO9 não
preenche os seletores NULL nem comprova essas fontes. A matriz
TO9-ORIGINAL-BINDINGS.csv separa seleções prévias de identidades/gates frescos:
PID/token/SCM/imagem/referências/nonce são adquiridos na sessão, não inventados
antecipadamente. A ausência de key deixou de bloquear exclusivamente propósito2.

Depois: pedido byte-exato, autorização própria de endpoint, lifecycle SCM e
Start/Stop, implantação/imagem e parent somente quando necessários; não agregar
instalação ou KSP. Só então prova bilateral nativa original/close/readiness.
Isolamento CNG MACHINE e cerimônia NEW_INSTALL/CA_INITIALIZE/publicação continuam
operações separadas, sem necessidade de CA prévia para o transporte. Diretório
existente não é root publicada; N0 não é C8, fixtures não são autoridades.
Não reabrir TPM/Q4. Nenhuma capacidade produtiva ou PST0.6.5 foi promovida.

Próxima continuação técnica concreta:
[NEXT-TO9-EXACT-TRANSPORT-SESSION-PLAN.md](NEXT-TO9-EXACT-TRANSPORT-SESSION-PLAN.md).
TO0–TO9 integradas e qualificadas OFFLINE; E21 PENDING, ED11/Phase6 PARTIAL,
autoridade DENY. HEAD/WIP/históricos/pins/chave fixture/UNKNOWN/reservas/PST0.6.4
preservados. Não é declaração de conclusão operacional por builds.
