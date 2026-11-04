# Pedido — nova sessão E/Q/S para a cadeia P1 corrigida

Status: CANDIDATE / FOR_OWNER_DECISION / NOT_AUTHORIZED / NOT_EXECUTED.
Uma única sessão MANUAL PERSONAL_LOCAL TRANSPORT_ONLY AMD64. Não é retry de
sessão anterior. Não registra decisão emitida nem presume aprovação do proprietário.

## 1. Aprovação operacional específica solicitada

Solicita-se aprovação expressa deste pacote byte-exato para:
- E — endpoint privado, independente de Q/S.
- Q — entrega SCQ TRANSPORT_ONLY, independente de E/S.
- S — um primeiro Start diagnóstico fechado, independente de E/Q.
- Efeito documental auxiliar: preservar literalmente a decisão REAL futura do
  proprietário e criar exclusivamente DUAS cópias originais protegidas NOVAS,
  do pedido e dessa decisão, no parent documental existente.

Uma única execução MANUAL da sequência. O agente só registra aprovação real e
prepara comandos após a decisão, não executa. Os três confirmadores/consumos
nativos continuam separados; confirmação documental não substitui nenhum deles.
Não inferir aprovação de arquivo/hash, token elegível, carrier ou presença do EXE.
Autorização parcial/preparatória não habilita o driver completo. Uso, falha ou
cancelamento da sequência não dão retry mesmo que consumed interno seja zero.

Não reutilizar pedido86303...2a34/decisãoa7d354...8607, sessões25ea...dff4/af85...7bc9,
originais312999...6798d/7f645...641a ou qualquer aprovação/coorte de UPDATE,
SCM/staging/observação consumida. Não recriar/sobrescrever/adotar originais antigos.
Este pedido e sua futura decisão são fontes novas; seu escopo é um ensaio limitado,
não restart produtivo ou autorização genérica persistida.

## 2. Contratos accepted e fronteiras

Aplicam-se P0–P7 (DECISION-personal-scq-existing-target-p0-p7-20261101.md),
QUERY/G3, G0–G9, SQB2, continuação condicional pré-Start, OHI, SSO-B/S0–S7,
TO0–TO9, SCQ, F1–F6 e owner SCM existentes. Propostas incorporadas:
- P0f2dd0651f1a546b47f5edb756dbfa815533a8c883ceda121a20a956af8f0bb28.
- QUERY/G35b77ddae089ba496c511a562575e22642a29555e536fd4563e5e5ebb8faefeca.
- G0–G9d65da3287ac5c1da7f59e6a467498ae2198f0896db801584b4b6c5fedad0ad1b.
- SQB262548c758282dd6b828b451a01a92bc4a369843ac55f7cc9c52f8c84a25ae11c.
- Continuação9637246ad9ad6de34ff9eb5cb9440e628a14909ea7f95cb6a2ba2b7265287f68.

Nenhuma norma/guarda/quota/formato muda. Sem nova ADR/owner/root/serviço/canal.
Não transportar UPDATE U4, B3 ou R3 ao Start. READ/NEW_INSTALL/CA/CURRENT e
demais perfis permanecem estritos. P0–P7 seleciona o OBJETO instalado de origem
ProgramFiles x64, não recertifica mapping Windows atual. Exige proveniência,
readquisição/IDs/SD/ancestrais/reparse/continuidade e binding PXT1 nas duas pontas.
Mapping/string/hash/identidade histórica não são autoridade ou referência viva.

## 3. Operador, serviço e imagem selecionáveis

Host TCORP-TOBIAS; operador SID
S-1-5-21-785699550-1440458426-1682014700-1108, proprietário/administrador legítimo
declarado em confirmações atribuídas, token nativo original elegível e sem
impersonação/autoelevação. Controller é o processo original do EXE canônico
instalado; nunca workspace, papacc_update.exe, fixture/LAB ou controlador antigo.

Serviço PapinhoAccelerator, conta NT SERVICE\PapinhoAccelerator,
WIN32_OWN_PROCESS16/DEMAND_START3/ERROR_NORMAL1/SID UNRESTRICTED1,
comando exato:
`"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`.
SCM SERVICE_STOPPED/PID0/configuração/imagem exigem gates frescos antes de Start.
Não criar/reconfigurar serviço, alterar ACL, chamar Start diretamente fora do
owner ou presumir que o readback do UPDATE seja lease permanente/fencing.

Imagem canônica:
`C:\Program Files\Papinho\Accelerator\papacc_server.exe`,
**774656bytes**, **ID417614100:1638400:813992**,
SHA256 **9816585b3d240ce59da70769245204378f8e08e67c67f5d5a0b16b729b8d0220**.
Proveniência P1-CLOSED-CHAIN-UPDATE-RECONCILIATION.md / resultado JSON e
OWNER-MANUAL-P1-CLOSED-CHAIN-UPDATE-OUTPUT.txt,
SHA25690da0d0f476add7fec48521daf36bf4be41f894d09dcd16151f90181ec71e5d5.
UPDATE consumido/fechado, anterior preservado no .previous da tentativa
135037c74363d380224bc39f449cfbaec2235f93bf478d92ad15b75d383b4b8a.
Quatro builds sem warnings e60pares CTest da imagem qualificados offline;
isso não prova aquisição nativa P1/recursos/autenticação bilateral atuais.

DLLs somente lidas/validadas, nunca alteradas:
- libssl-3-x64.dll1321472bytes,
  SHA2563fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed.
- libcrypto-3-x64.dll7217664bytes,
  SHA25609eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.

Proteção/identidade/owner/DACL efetivos, ancestrais/reparse e continuidade são
gates nativos obrigatórios. BA para imagem TO não dá autoridade aos documentos
ou ao Start. Pins on-disk não atestam páginas mapeadas. Não houve consulta atual
à instalação/SCM/ACL nesta preparação; os dados acima são expectativas com
proveniência reconciliada, não observações frescas.

## 4. Nova seleção P1 atribuída

Papel ProgramFiles x64/AMD64, volume417614100, objeto esperado
417614100:131072:226316; parent binário417614100:8257536:71209;
imagem417614100:1638400:813992. Cadeia relativa esperada:
ProgramFiles/Papinho/Accelerator/papacc_server.exe, sem reparse/substituição.
Proveniência inicial phase6-root-chain-procedure-20261031/OWNER-CHAIN-RESULT.txt,
SHA25643fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87.
Raiz histórica C:\417614100:327680:5, não referência atual ou autoridade.
IDs são expectativas selecionadas SOMENTE após aprovação deste pedido.

Linha DATA exata para digitação MANUAL exclusivamente no prompt nativo P1:
```
P1 TRANSPORT_ONLY AMD64 417614100:131072:226316 417614100:8257536:71209 417614100:1638400:813992 43fb6712be30ce5b7afca8111512bd284672efa4bb4b8678b21a04eb7b36dd87
```
Não é comando/confirmador/grant; o driver só exibe, não transmite nem responde.
Entrypoint específico --owner-manual-first-scq-start-to-existing, sem CLI genérica.
Token/console originais precedem DATA; constructor/cadeia/PXT1 revalidam IDs/SD/
proveniência/relação/imagem/contexto antes de autoridade e consumo dos efeitos.

O novo resolver compartilhado consulta localização nativa candidata direta
`\Device\...` para originais e âncora; mantém DONT_REPARSE, acesso/share e
validação independente dos objetos. Filhos relativos/handles retidos preservam
continuidade. Prefixo/mapping não prova confiança; alias/reparse/ID/SD divergentes
negam, sem fallback Shell/READ/NEW_INSTALL ou novo mecanismo de namespace.
Não afirmar que essa instalação prova o componente histórico C000050B ou sucesso
das aberturas futuras. A causa histórica permanece UNKNOWN.

## 5. Dois originais protegidos NOVOS

Parent documental existente:
`C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101`.

Após aprovação REAL do pacote, preservar literalmente os bytes da mensagem do
proprietário em OWNER-P1-CHAIN-THREE-ACTS-APPROVAL.txt, sem overwrite, com hash
REAL e escopo registrado. Esse arquivo não foi emitido/criado nesta preparação.
Não fabricar decisão/modelo ou hash de aprovação ainda inexistente.

| Fonte DATA retida | Novo original protegido exclusivo |
|---|---|
| Este ONE-P1-CHAIN-THREE-ACTS-REQUEST.CANDIDATE.md | FIRST-P1-CHAIN-SESSION-REQUEST.ORIGINAL.md |
| OWNER-P1-CHAIN-THREE-ACTS-APPROVAL.txt real | FIRST-P1-CHAIN-OPERATIONAL-DECISION.ORIGINAL.txt |

Somente DOIS FILE_CREATE no parent existente, ownerSID operador e DACL protegida
operador/SY/BA full desde criação. Nenhuma criação de pasta, mudança de ACL
existente, overwrite/adoção/rename/delete/recuperação. Se nome já existir ou
falha parcial, negar/preservar, não escolher outro leaf nem repetir a sequência.
O helper retém fontes/ancestrais, verifica identidade/SD/reparse/single-link,
Check/Stop, consumo antes de criação, escrita/hash/flush/readback, fechamento
e retençãoUNKNOWN. Não copiar originais históricos para fabricar nova decisão.

Só após created_mask3/verified_mask3/close_knownTrue, conteúdo/pins/SD e IDs
realmente criados, o controlador readquire os NOVOS originais e mantém as
referências. IDs/hash da decisão são bindings tardios de fatos legítimos,
não números inventados ou placeholders operacionais. Comparar IDs dos planos
com a saída documental. Cópia não é quarto ato E/Q/S, store de grants ou prova
isolada de autoridade; nenhuma confirmação documental substitui as três nativas.

## 6. Três atos independentes incorporados byte-exatos

| Ato | Documento | SHA256 |
|---|---|---|
| E | P1-CHAIN-ACT-ENDPOINT.REQUEST.CANDIDATE.md | 4733cede33b2ddb24eece1a71bbc1b84da4278f95896a644cff94c7d70b538b6 |
| Q | P1-CHAIN-ACT-SCQ.REQUEST.CANDIDATE.md | 517739874ed5f833bae7ad1d052f50c1a894f2e9f7b0ed4e65a827eac4754f75 |
| S | P1-CHAIN-ACT-START.REQUEST.CANDIDATE.md | 0d1c54d1b51a1f729eb41f22a223d263299f9d3ced5d34054230814fc51ea8f4 |

Confirmadores distintos: OWNER_SCQ, OWNER_SCQ_PRIVATE_ENDPOINT e
OWNER_SCQ_DIAGNOSTIC_START. Nonce/tentativa e plano comum imutável gerados no
owner; não fornecer respostas automaticamente. Consumo independente ANTES
do efeito respectivo. Ausência de um ato não é suprida por outro/copiar decisão.
Continuação pré-Start só após revalidar controlador original, três atos, G3 e
namespace original; não antecipar autenticação do runtime ou readiness.

No máximo um namespace privado exclusivo, uma conexão local receptora, uma
entrega governada e uma StartServiceW pelo owner SCM existente. Poll da mesma
operação pendente não é retry; reconexão/nova tentativa não é autorizada.
Carrier/QUERY/ack DATA/nome de endpoint não conferem autoridade operacional.

## 7. G3 e riscos de recursos — valores intactos

| Parâmetro | Valor selecionado accepted |
|---|---|
| Handles totais próprios por participante | guard124 antes das aquisições; máximo128 |
| Crescimento privado sobre READY próprio | 1994944bytes |
| Crescimento working set sobre READY próprio | 1994944bytes |
| Piso físico disponível host | 7239172096bytes |
| Piso commit disponível host | 9823326208bytes |
| Freshness até efeito | 100ms |
| Janela original sem renovação | 240000ms |
| Duração global após READY controlador, compartilhada | 60000ms |

São escolhas de risco, não máximos nativos demonstrados. Nenhum número/unidade/
domínio foi alterado. Contar todos os recursos, inclusive herdados/console/CRT/
loader/SCM/peers/temporários/aliases/originais. Sem fechar refs ou excluir handles
para obter PASS. Preservar reservas, Stop e retenção. Falha de consulta, margem
insuficiente, teto/freshness/prazo vencido ou UNKNOWN material mantém DENY.
Deadline receptor é o MENOR prazo original/global/SQB2/local aplicável; recebimento,
ACK/arquivo/carrier não renovam duração ou janela. Sem cancelamento imediato
garantido para API síncrona ou I/O já submetido.

A correção de namespace NÃO demonstra redução de handles/capacidade. Count113
do controlador anterior e68 do observador são observações de processos distintos,
não baselines futuras. Submodelo de coexistência B+14 (raw3+cadeia5+duplicatas3+
readers3 antes de fechar raw) ainda exige, nesse trecho isolado, B<=110 sem
outros custos; B113 negaria antes do primeiro reader. Não garantir confirmações.
Pico171 do updater não é quota/ceiling do SCQ. No último UPDATE, mínimos físico
6667689984/commit5230235648bytes foram inferiores aos pisos G3; não são amostras
atuais da nova sessão, mas também não são prova de capacidade. Se os gates
frescos não passarem, parar sem nova coleta/retry/bypass ou alteração de valores.

## 8. Pré-Start versus provas posteriores

Antes de Start: fonte/operador/originais/imagem/P1/PXT1/ancestrais/proteção/
configuração SCM/namespace/recursos/plano/consumos devem ser demonstrados.
Não exigir PID/token de runtime inexistente. Depois de Start autorizado:
START_PENDING, PID/criação/TokenUser/grupos/restrições/SID virtual, imagem/serviço/
peer originais, binding e continuidade reais. Autenticação bilateral independente
precede empréstimo admitido. ReceiptSQR1 e closeSQC1 originais antes de RUNNING
somente diagnóstico; ACK DATA não autentica. Não declarar readiness prematura.

Exigir término natural finito, exit0, SCMSTOPPED/PID0 e readback/close próprios.
Não autoriza ControlService Stop, kill/restart ou cleanup para produzir sucesso.
Retenção incerta pode manter owner/processo estacionados; não terminar para
fabricar PASS ou liberar créditos por timeout. Nenhum KSP/chave/CA/CURRENT,
rede ou readiness produtiva são concedidos.

## 9. Procedimentos e qualificação offline

| Artefato | SHA256 |
|---|---|
| p1-chain-session-originals.cs | cfd70b055b747611fdaf0a873d1cb4a9aaaea4687bc09782651fb9ddf8b46c9a |
| prepare-p1-chain-originals.MANUAL.CANDIDATE.ps1 | ba7a902d4e0e895c7a9f79bc7efd8fa484136c85b7974285cb6b487e4985fab5 |
| run-p1-chain-diagnostic.MANUAL.CANDIDATE.ps1 | cdf8d918a7e6e114f69b4fda5be3a3983687f108e65dfb8b11a2ac0d4249a13e |

Mesmo adaptador C# (somente4nomes fixos diferentes), mesma sequência documental
e driver administrativo. Guard de pins amplia rejeição para request/decisão
consumidos da sessão P1 anterior. Wrappers não invocados nesta preparação.
Windows PowerShell5.1 Desktop x64, console original e contexto administrativo
legítimo; sem autoelevação. Bypass eventual só no filho, nunca configuração
persistente/GPO. Request pin exato e decisão REAL serão argumentos fechados
após aprovação; não emitir comando com decisão fictícia/placeholder.

Qualificação limitada aos novos bindings: comparação de deltas exatos, compilação
C# em memória W4/zero warnings e modelo com I/O mutante simulado; replay/Stop/
falhas parciais/SD/closeUNKNOWN, AST e escopo dos wrappers, pins antigos negados,
G3/14argumentos e DUAS invocações futuras controladas (helper e controller).
Não instanciar constructor/Prepare nativos ou chamar produto/SCM/endpoints.
Quatro builds/60pares da imagem reutilizados, sem repetir campanhas inalteradas.
P1-CHAIN-SESSION-OFFLINE-EVIDENCE.json registrará evidência específica; o manifest
de bindings técnico não é aprovação nem grant.

## 10. Falhas, limites e decisão futura

G0–G9 aceita risco residual delimitado de processo/loader/CRT/RPC/pipe/hash/
provider/kernel e transitórios, não ausência de exaustão/latência ou bypass de
capacidade. Processo real existe antes da primeira amostra própria. Nenhum pico
observado vira ceiling. UNKNOWN de custos/fechamento/retidos/OS permanece.

Stop/Esc/Ctrl+C, original/ID/SD/SCM/imagem divergente, nome/namespace existente,
substituição/replay/forwarding, ato ausente, quotas/margens/freshness/deadlines,
peer perdido ou receipt/close incompleto negam próximo efeito. Preservar primeira
falha, consumo e retenção. Sem retry/reconexão/restart/adoção/limpeza/refund/
recuperação. Não tocar candidato742d..., .previous, originais anteriores ou
reservas de coortes UNKNOWN. Se conflitarem com obrigação desta sessão, STOP.
O UPDATE recente não resolve retenção histórica ou quiescência OS.

Somente após decisão REAL futura: registrar literalmente bytes/hash/escopo,
conferir pacote e preparar comando exato para a única execução pelo proprietário.
Sem aprovação expressa E/Q/S e dos dois originais auxiliares, não completar por
inferência. Este pedido NÃO é decisão operacional emitida. O agente não executa.

Reconciliar saída integral: par created_mask3/verified_mask3/close conhecido,
IDs/hashes/SD, primeira falha nativa, planos/três confirmações e consumos,
endpoint/SCQ/Start, PID/token/peer/imagem, bilateral/receipt/close, término e
retidos. SCQ_START=DIAGNOSTIC_CLOSED_QUALIFIED exige suas provas; exit0 ou
SERVICE=REQUEST_SUBMITTED isolados não bastam. Resultado inconclusivo é UNKNOWN.

Proibidos UPDATE/staging/registro/reconfiguração SCM, ACL de objetos existentes,
novo diretório/conta/serviço/root/canal, Start adicional/ControlService Stop,
endpoint genérico/rede operacional, KSP/CA/CURRENT/TPM/NV/Q4, exclusão/restauração/
rollback/limpeza/refund/recuperação ou autoridade produtiva. HEAD/WIP/históricos/
pins/EXEs/fixture/originais/reservas/atos consumidos/PST0.6.4 preservados.
E21 PENDING; Phase6 PARTIAL; readiness produtiva DENY.
