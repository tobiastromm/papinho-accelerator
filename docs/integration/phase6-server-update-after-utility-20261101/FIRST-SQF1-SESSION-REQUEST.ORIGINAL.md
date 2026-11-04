# Pedido — sessão independente de primeira falha SQF1

CANDIDATE / FOR_OWNER_DECISION / NOT_AUTHORIZED / NOT_EXECUTED.
UMA sessão MANUAL PERSONAL_LOCAL P1 TRANSPORT_ONLY AMD64: E endpoint privado,
Q SCQ, S Start diagnóstico fechado, com três confirmações/consumos independentes.
Solicita também DUAS cópias auxiliares protegidas dos novos pedido/decisão reais.
Sem retry, resposta automática, nova tentativa por falha ou autoridade por DATA.

## Finalidade e imagem instalada
Validar nativamente a cadeia P1/SQB3/SQC3/Q6 com retorno NEGATIVO SQF1 aceito.
Se o receptor falhar e os gates permitirem, SQF1 reporta componente/Win32 da primeira
falha. Não é receipt/close/autoridade e não garante revelar causa nativa completa.
Com Stop/prazo/recursos/completion/provas insuficientes o envio é suprimido e pode
continuar havendo timeout sem diagnóstico remoto. Não presumir correção ou sucesso.
A causa histórica das sessões anteriores permanece UNKNOWN.

Canônico C:\Program Files\Papinho\Accelerator\papacc_server.exe:
809472bytes/ID417614100:2883584:329728,
SHA25681a849dc51bf2e7fd0455c09f30c926329ef4c30196b36747f1429e5fdecee86.
UPDATE f154e909453fc7072ced802f298cf0fc5c9868708093f6448279474fd7d1af09/
decisão389821759097e861581de03e20a736eeadc382cf91ad2cbbdd3c7f35a171c3f1,
consumido, first0/created1/renames_mask3 (DOIS renames)/readback-close conhecidos.
submitted202 é contador de efeitos, não202tentativas;missing2 preserva máximos
opacos UNKNOWN, não representa bypass de gate. Evidência exata
OWNER-SQF1-UPDATE-OUTPUT.txt/SHA256 faaae4719253a5a6dc985db105bd0360e26b13509afbb87a8048e31b72182c8e.
Anterior805376/ID417614100:2097152:411753/pin174db00a… preservado em
papacc_server.81145817d44f5936bb2e5b1b79a22b904af42c0a925b4c91040c38db6ebcada4.previous.
Readback/close anteriores não são lease nem atestado de páginas mapeadas.

Última sessão db62ad04… consumida: receipt1052/operation2/pending1/offset0,
timeout1460/212/retained1. Consulta3e32bbc6… e disposição e8cb88b0… consumidas,
saídas de runtime21316/criação134381413935377225 e controlador7400/criação
134381413716812022 observadas e SCM STOPPED1067/specific0; não SCQ close/bilateral
ou recuperação. Não reutilizar originais/decisões/autorizações dessas sessões.
Todos os históricos/retidos/reservas/previous/EXEs continuam preservados;
application close/bilateral/receipt históricos e quiescência OS UNKNOWN.

## Contratos accepted

P0–P7 f2dd0651f1a546b47f5edb756dbfa815533a8c883ceda121a20a956af8f0bb28;
QUERY/G3 5b77ddae089ba496c511a562575e22642a29555e536fd4563e5e5ebb8faefeca;
G0–G9 d65da3287ac5c1da7f59e6a467498ae2198f0896db801584b4b6c5fedad0ad1b;
SSO-B/S0–S7 60cdd50be68fa15c1379c31f287edd697140cfc2bf39ff8b319b86f0e5f08fa3;
OHI 34c68d97250f89634e2fff78ed48b1dbc4a02b4cc6c87bb91bb00a487fb2e561;
TO0–TO9 0702a2e733024118bdad7c0993b4993e20cf44e825a872ad50245e8fd2b45d30;
SQB2 legado preservado62548c758282dd6b828b451a01a92bc4a369843ac55f7cc9c52f8c84a25ae11c;
SQB3/SQC3 com errata Q6: DECISION-personal-scq-p1-query-references-sqb3-20261102.md,
base726e3f786b6ca187a0058734b6c9fd9e67d2d2b9d465f59bc9bd5313cbbe2c4c/
errata b8d73c7541df2363e6f02489d5143a2becec78fdd315003806010a0303bbe810;
continuação pré-Start9637246ad9ad6de34ff9eb5cb9440e628a14909ea7f95cb6a2ba2b7265287f68.
Aplicam-se precedências P1 de handles e commit3GiB accepted, sem ampliar outros
perfis ou importar envelopes UPDATE/SCM/B0. Nenhuma norma ou formato alterado.

SQF1 F0–F7 accepted: DECISION-personal-scq-p1-receiver-first-failure-sqf1-20261102.md,
base96b2d26f83cecc7688eeb9e3543a6b19d6660978b31b62ae175fe604f41a3643.
Exceção focal à proibição Q2/Q6 de outro frame, sem alterar SQR1/SQC3/legados/
formatos públicos ou persistidos. Mesmo owner/pipe/tentativa e escopo P1 apenas.
SQF1/56bytes: magic4/tamanho4/stage4/win324/native_kind4/raw4/attempt32, LE.
Metadado remoto rotulado RECEIVER_REPORTED_FIRST_FAILURE, não erro nativo próprio.
Componente composto usa native_kind0/raw0; raw não é adivinhado por GetLastError.
Uma escrita, sem enviar restante/retry em short write; pending/completion incerta
preserva buffer/OV/refs. +88bytes/zerohandles por pacote, controlados44 intactos.
Negativo válido sempre DENY local, inclusive se código remoto for997/234; não
concede ack/receipt/close/bilateral/readiness/liberação ou nova sessão.

Quatro builds sem warnings/80pares afetados PASS; corpo REAL ingress/wait com
fronteiras de aquisição/validação mockadas individualmente,18falhas/ordem positiva,
codec/envio16/recepção14/fragmentação/replay/Stop/deadline/resources/UNKNOWN e guard
unresolved permitindo só espera do negativo já submetido. Não prova pipeline nativo
inteiro; nenhum modelo é autoridade. Censo Selectedroot375256/known2002304 sob3145728,
Legacyroot206688/known1833736 sob2097152. Custos internos opacos UNKNOWN.
Sem repetir builds/testes inalterados. Ver SQF1-OFFLINE-QUALIFICATION.json.

## Contexto, seleção e gates prévios
Host TCORP-TOBIAS/PowerShell5.1Desktop AMD64/console original não redirecionado,
operador administrativo legítimo SID S-1-5-21-785699550-1440458426-1682014700-1108,
sem impersonação/autoelevação. Nova instância do EXE protegido, não workspace/updater.
P1 seleção DATA manual vinculada à proveniência original, nunca autoridade isolada:

P1 TRANSPORT_ONLY AMD64 417614100:131072:226316 417614100:8257536:71209 417614100:2883584:329728 43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87

Proveniência OWNER-CHAIN-RESULT.txt/pin43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87.
P0–P7 seleciona instalação existente, não recertifica mappingWindows. Reaquisição
original/owner-DACL/ancestrais/ausência reparse/continuidade/target obrigatórios
nas duas pontas. Nome nativo direto DATA/OBJ_DONT_REPARSE não é autoridade.
Owner BA somente para imagem TO sob guarda aceita, não relaxar originais/outros perfis.
Entrypoint específico --owner-manual-first-scq-start-to-existing; sem CLI universal.
DLLs intactas: ssl1321472/pin3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed;
crypto7217664/pin09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
SCM original PapinhoAccelerator/NT SERVICE\PapinhoAccelerator, OWN_PROCESS16/
DEMAND_START3/ERROR_NORMAL1/SID_UNRESTRICTED1, comando exato
"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed.
Exigir SCM STOPPED/PID0/configuração e imagem frescos nos gates, sem modificar SCM/ACL.
Não limpar1067 para obter PASS. PID/token runtime só após processo existir.

## Dois originais auxiliares novos, somente após decisão efetivamente emitida
Pasta documental existente C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101.
Fontes ONE-SQF1-THREE-ACTS-REQUEST.CANDIDATE.md e
OWNER-SQF1-THREE-ACTS-APPROVAL.txt (AINDA NÃO EMITIDA).
Dois destinos exclusivos FIRST-SQF1-SESSION-REQUEST.ORIGINAL.md e
FIRST-SQF1-OPERATIONAL-DECISION.ORIGINAL.txt. IDs somente após criação,
sem fabricar original/decisão/confirmação. Hash/conteúdo idênticos às fontes reais.
Owner SID operador, DACL protegida3ACEs operador/SY/BA FULL sem herança,
ancestrais/originais retidos/ausência reparse/cap65536bytes cada. Sem criar diretório,
adoção ou alterar ACL existente. Consumo antes do primeiro create, write/flush/
readback e close conhecido antes da fase nativa. Stop/parcial/close incerto conserva
objetos/UNKNOWN e impede E/Q/S, sem delete/rollback/retry. Cópias DATA não confirmam atos.

## Três atos e provas posteriores

E: uma criação exclusiva de namespace privado SCQ e uma conexão local prevista,
sem adoção/reconexão/forwarding. Q: entrega original TO, identidade/proveniência
independentes e autenticação bilateral antes de empréstimo admitido. S: no máximo
UMA StartServiceW do serviço existente com carrier/SQB3/deadline dessa tentativa.
Cada confirmação e consumo atribuídos independentes antes dos respectivos efeitos.
Efeitos anteriores podem persistir se ato posterior falhar; preservar UNKNOWN.

Antes de Start não exigir PID/token futuro. Após existir o processo:
START_PENDING, PID/criação/TokenUser/grupos/restrições/SID virtual e imagem/
serviço/processo/token originais, binding tardio e continuidade reais.
QUERY candidata e ACK DATA não são admissão. Bilateral/empréstimo legítimo/
receiptSQR1/closeSQC3 originais precedem RUNNING somente diagnóstico fechado.
Readiness produtiva DENY e nenhum KSP/chave/CA/rede produtiva permitido.

SQB3 entrega as duas referências originais do controlador, processo0x00101000 e
tokenQUERY8, pelo mesmo owner/pipe, depois da autenticação do runtime no remetente.
No receptor, QUERY inicial pode ler frame limitado e esperar evento/Stop/deadline
antes do binding do peer; não declara esse peer vivo nem admite empréstimo. Validar
tipos/direitos/identidades/proveniência/continuidade e ambos os lados antes de admitir.
Sem reabertura administrativa, SeDebug, impersonação, alteração de ACL ou autoridade
por token/frame. SQC3 fecha cinco handles entregues, sem declarar fechadas as cópias
originais de custódia do recipient; estas e os readers ficam até resultado terminal
e liberação do empréstimo. Não usar SQC1 como fechamento completo de SQB3.
SCQ1/SQB1/SQB2/SQR1/SQC1 legados preservados; nenhuma ampliação READ/NEW_INSTALL.

Exigir término NATURAL finito, exit0, SCM STOPPED/PID0/readback e close próprios.
Não conceder ControlService Stop, kill, restart, fechamento de console ou
recuperação por timeout. Se processo ficar retido, manter evidência/UNKNOWN,
sem repetir ou presumir autorização de terminar pela disposição anterior.

## G3-P1 concreto e riscos aceitos para decisão

Contagem inicial completa máxima128; guarda deny>=188 antes de aquisição;
máximo192 por participante. Orçamento controlado44, opaco16 e folga4 são escolhas
conservadoras/risco, não máximos Windows demonstrados. Contar todos os handles,
sem subtrair baseline/CRT/console/loader ou fechar referências por conveniência.
Private growth1994944bytes; working set growth3145728bytes (3MiB), cada métrica sobre READY próprio;
físico disponível>=7239172096bytes; commit>=3221225472bytes (3GiB exatos).
Freshness100ms até efeito, janela original240000ms sem renovar, duração global
compartilhada60000ms. Receptor usa menor prazo original/global/SQB3/local.
Policy0b260c85dfceec5e6e861bb998e4504723785b927d875d113d41a405e1ce1b60.
Policies antigas ou seleção P1 ausente/divergente=>DENY, sem fallback a8GiB ou
ampliação para READ/NEW_INSTALL/outros perfis. Managed3MiB e outras quotas intactas.
Retenção conta no mesmo orçamento; sem coortes concorrentes/rearme/refund.
Gates antes de efeito e de duplicação por processo de destino já retido;
UNKNOWN material/Stop/expiração/insuficiência/query falha nega próximo efeito.

Custos de processo/loader/CRT/RPC/pipe/provider/kernel e transitórios são opacos;
LIMITED_RISK_ELIGIBLE não é capacidade nativa universal. Picos/margens do UPDATE
não pertencem a G3. Coleta amostral não garante evitar pico, deadline não garante
retorno/cancelamento/quiescência. Pode falhar antes ou depois de um efeito.

## Artefatos imutáveis
- sqf1-session-originals.cs: 916c6cb13817ad76d745d1731a1e99949cbb6bc5a1ba45193ad93a430cc9c1a1
- SQF1-ACT-ENDPOINT.REQUEST.CANDIDATE.md: d20b43105b8fb9e1c58c71102613c8a75a23a775e7220d6843ab3f02b5f934cc
- SQF1-ACT-SCQ.REQUEST.CANDIDATE.md: 4b69ffad8712a26af38161fafb217b4ad0631057ce1924c072f282e46d787b3a
- SQF1-ACT-START.REQUEST.CANDIDATE.md: fdb50be31b28ff4ba1e081712522cbf7ae83063efd762124fcb630ba7ce92c67
- prepare-sqf1-originals.MANUAL.CANDIDATE.ps1: 96c478be1bec74bfa5795922a5d2c3f1250d1c1f1d8c353ab2fa30903414bd47
- run-sqf1-first-failure-diagnostic.MANUAL.CANDIDATE.ps1: 42eac739ad9eb600ab3887d2ef127b75db5a8f7b79a9adcf7b00328d1649a44a

Mesmo fluxo documental e caller específico, seis deltas exatos em
SQF1-SESSION-ARTIFACT-BINDINGS.json. C# somente quatro nomes; preparador nega também
último request/decisão RECEIVER_PATH consumido. Driver14argv/duas invocações futuras,
sem resposta automática ou comando extra. Qualificação documental78checks/44casos
de escopo offline, sem constructor/Prepare/Create ou operação real.

## Decisão solicitada
Aprovar estes bytes para UMA sessão MANUAL E/Q/S e DUAS cópias auxiliares novas,
reconhecendo riscos G0–G9/G3-P1/SQF1. Sem aprovação específica DENY. Registrar
somente a decisão efetivamente emitida/hash real, preparar comando com hashes
reais sem placeholders ou respostas automáticas. Não executar automaticamente.
Nova tentativa/lifetime produzidos e revalidados pelo owner, sem PID/token futuros
fabricados. Três confirmações/consumos independentes permanecem obrigatórios.

Depois: reconciliar primeira falha/cópias/IDs/pins/SD/close/atos/G3/endpoint/SCQ/
Start/SCM/runtime-token/bilateral/receipt-close/RUNNING diagnóstico/término natural
finito/exit0/SCM STOPPED-PID0/readback/close se alcançados. Exit0 isolado não basta.
SQF1 é somente relato negativo, não prova de cleanup/close/quiescência. Em retenção
não repetir, não fechar console ou inferir Stop/kill por decisões antigas.
Não concede UPDATE/Start produtivo/Stop/kill/ACL-SCM edit/CA/CURRENT/rede/TPM/NV/Q4/
KSP/chave/limpeza/refund/rollback/restauração/produtivo. Preservar HEAD/WIP/pins/
EXEs/originais/DLLs/reservas/históricos/UNKNOWN/DENY/PST0.6.4.
