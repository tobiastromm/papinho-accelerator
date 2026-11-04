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
809984bytes/ID417614100:1638400:439388,
SHA25678886f53119af1875dd7f326221b8f154e9528ef02f2f61f6a7bd9d23fab3de5.
UPDATE content-detail7291bf5b5f77ca3ca08ef857af572e5e31126abfdc7d5662f309235447502c12 consumido; tentativa984306131079e5b4e267734fbab4340f106a38dbd5d9f4f9acc25e216604eb1c, first0/created1/renames_mask3/readback-close conhecidos. Evidência OWNER-SQF1-DETAIL-CONTENT-DETAIL-UPDATE-OUTPUT.txt SHA256d458886552879a4e206f237a9a7d628aded2e208e5d895a609d6e18deeb460bc. Anterior809472/ID417614100:2883584:329728/pin81a849dc51bf2e7fd0455c09f30c926329ef4c30196b36747f1429e5fdecee86 preservado no previous dessa tentativa. Não é lease fresco nem atestado de páginas mapeadas.
Sessão SQF1 anterior c66738655b58f16697e40e914cf83f217e73d667f5e08070be75c12406aaa177 consumida: receptor reportou etapa10/erro5 sem raw status, causa interna UNKNOWN. Disposição acfe92abd4913b7dd5159be89aaa6fcfba7a4aecfe6604ab763e76a059b03918 consumida, runtime15332/criação134381442827103477 e controlador20104/criação134381442604810139 com saída observada, SCM STOPPED1067. Não comprova SCQ/application close, bilateral ou quiescência. Preservar todos os históricos/originais/retidos/reservas. Nova imagem refina catálogo20–24: own-count/guard, ReOpenFile pedido/decisão/imagem e revalidação/deadline; raw somente quando demonstrado na chamada original. Não corrige nem prova retroativamente causa histórica.

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

Quatro builds sem warnings/96pares afetados PASS; corpo REAL ingress/wait com
fronteiras de aquisição/validação mockadas individualmente,18falhas/ordem positiva,
codec/envio16/recepção14/fragmentação/replay/Stop/deadline/resources/UNKNOWN e guard
unresolved permitindo só espera do negativo já submetido. Não prova pipeline nativo
inteiro; nenhum modelo é autoridade. Censo Selectedroot375256/known2002304 sob3145728,
Legacyroot206688/known1833736 sob2097152. Custos internos opacos UNKNOWN.
Sem repetir builds/testes inalterados. Ver SQF1-CONTENT-DETAIL-OFFLINE-QUALIFICATION.json.

## Contexto, seleção e gates prévios
Host TCORP-TOBIAS/PowerShell5.1Desktop AMD64/console original não redirecionado,
operador administrativo legítimo SID S-1-5-21-785699550-1440458426-1682014700-1108,
sem impersonação/autoelevação. Nova instância do EXE protegido, não workspace/updater.
P1 seleção DATA manual vinculada à proveniência original, nunca autoridade isolada:

P1 TRANSPORT_ONLY AMD64 417614100:131072:226316 417614100:8257536:71209 417614100:1638400:439388 43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87

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
Fontes ONE-SQF1-DETAIL-THREE-ACTS-REQUEST.CANDIDATE.md e
OWNER-SQF1-DETAIL-THREE-ACTS-APPROVAL.txt (AINDA NÃO EMITIDA).
Dois destinos exclusivos FIRST-SQF1-DETAIL-SESSION-REQUEST.ORIGINAL.md e
FIRST-SQF1-DETAIL-OPERATIONAL-DECISION.ORIGINAL.txt. IDs somente após criação,
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
- sqf1-detail-session-originals.cs: efb3fcb6c014b0acef43cf37a93e67fe9d1699b5066a6a71ca0d10b5f4ee5a46
- SQF1-DETAIL-ACT-ENDPOINT.REQUEST.CANDIDATE.md: f65d0817bf8669ba6cbafbc487a8c7aed5c76b84be38072288986643d67e4828
- SQF1-DETAIL-ACT-SCQ.REQUEST.CANDIDATE.md: 4cbfa28ee32324ef68e1b68b5b6ab358415ee6eea63ab253fcf669c59ad37abb
- SQF1-DETAIL-ACT-START.REQUEST.CANDIDATE.md: 83cf7af95bfe31cb141033801985ef663dd1f9a4a7aca1a96078f6b874b6cc92
- prepare-sqf1-detail-originals.MANUAL.CANDIDATE.ps1: 0b4260ecfa4636f494b8464c00f125ac66896d048ce61be079c9bc834b94e59e
- run-sqf1-detail-diagnostic.MANUAL.CANDIDATE.ps1: cc0bded057b1f10125f091b24e71f92a12241a37ba740c041ccb309d4dee1950

Mesmo fluxo documental e caller específico, seis artefatos com deltas exatos em
SQF1-DETAIL-SESSION-ARTIFACT-BINDINGS.json. C# somente quatro nomes; preparador nega também
request/decisão SQF1 anterior consumido. Driver14argv/duas invocações futuras,
sem resposta automática ou comando extra. Qualificação dos novos bindings/46casos
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
