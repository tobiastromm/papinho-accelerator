# Caller P1 — ligação específica ao controlador frio

2026-11-01. CODE_INTEGRATED_OFFLINE / OPERATIONAL_DENY.
main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469, WIP preservado.

## Autoridade e compatibilidade

Reutiliza P0–P7 accepted em ../../adr/DECISION-personal-scq-existing-target-p0-p7-20261101.md.
Original aprovado conferido e inalterado: EXISTING-PROGRAMFILES-ORIGINAL-BINDING.CANDIDATE.md,
SHA256 f2dd0651f1a546b47f5edb756dbfa815533a8c883ceda121a20a956af8f0bb28.
Não há nova decisão normativa/ADR nem alteração de accepted.

O patch genérico rejeitado NÃO foi reaplicado. A alternativa específica não
acrescenta argumentos de IDs/proveniência/owner/permissões. Há somente um nome
exato de ingresso, --owner-manual-first-scq-start-to-existing, com os mesmos15
argumentos totais do ingresso anterior: paths/pins dos originais e G3. Não há
parâmetro para escolher READ, NEW_INSTALL ou propósito arbitrário. Arguments
extras, nomes de perfil alternativos e cap>128 negam antes de adquirir o owner.
O ingresso anterior permanece separado, sem habilitar implicitamente P0.

P0/P1/P2 permitem seleção DATA pré-confirmação e exigem origem administrativa
independente. P3 exige congelamento e binding, não autoridade por locator/ID.
P4/P5/P6 mantêm continuidade, recursos e retenção. O adaptador aplica essas
condições sem dispensá-las: nenhuma string/ID/história/argumento habilita getters
ou autoriza efeito por si só. A revisão automática anterior não demonstrou
conflito normativo em P0–P7; apontou a amplitude de uma implementação específica.
Esta implementação delimitada foi aplicada sem contornar a rejeição por script
ou mecanismo indireto e sem solicitar reaprovação de decisão já accepted.

## Percurso efetivamente ligado

1. O entrypoint existente seleciona SOMENTE o ingresso TO-existing. Não recebe
   a tupla por argv, não fornece resposta automática e não fabrica decisão.
2. O mesmo owner instala seu console/Stop e adquire o token administrativo pelo
   mecanismo original. Antes disso não lê a seleção P1. Exige console nativo
   de entrada e saída; stream redirecionado não substitui essa leitura.
3. O console existente recebe uma linha NÃO SECRETA de DATA, limitada a256bytes,
   com identificador P1, perfil literal TRANSPORT_ONLY/AMD64, três IDs nativos
   completos e o digest de proveniência. Seleção deve ser copiada do NOVO pedido
   aprovado, nunca de uma decisão consumida ou relatório tratado como grant.
   Ausência, truncamento, overflow, arquitetura/perfil divergente, cancelamento
   ou falta de console nega, sem fallback para Shell ou outros perfis.
4. O parser privado produz apenas PAPACC_SCQ_EXISTING_SELECTION. A seleção é
   copiada para o packet existente e passa pelo constructor/chain P0–P7:
   provenance esperada, readquisição relativa, IDs esperados versus observados,
   SD/owner/DACL, ancestrais, reparse, imagem e continuidade. Não transfere handles
   por argv, herança, arquivos ou canal adicional.
5. Os caminhos/pins do pedido e decisão continuam candidatos: aquisição original,
   proteção estrita, identidade, conteúdo/SHA256 e token são gates independentes
   antes de autoridade. As três declarações/confirmações atribuídas E/Q/S seguem
   indispensáveis e vinculam o plano PXT1 à mesma seleção/operador/tentativa/
   originais/G3. A leitura DATA não é quarta autorização, não confirma nem
   consome qualquer ato. Ingresso válido sem fonte legítima continua DENY.
6. Receptor recompõe PXT1 independentemente; QUERY/carrier/ACK/target continuam
   candidatos. Autenticação bilateral, empréstimo, receipt/close e RUNNING
   somente diagnóstico permanecem separados. Sem autoridade produtiva.

Não foi emitida nesta tarefa uma seleção operacional original real, pedido ou
decisão E/Q/S. O caller está funcionalmente ligado para recebê-los; sua emissão
e aprovação continuam futuras. Não presumir que o conteúdo de uma seleção DATA
isolada seja prova da emissão administrativa. O plano deve ser conferido contra
o novo pedido e a decisão efetivamente emitida nas três confirmações nativas.

## Código e formatos

- server_launcher_scq_cold_start_win32.inc: ingresso exato, sem tupla em argv.
- personal_scq_cold_start_win32.h: discriminador privado de leitura DATA.
- personal_scq_existing_data_win32.inc: parser limitado, sem efeito ou grant;
  rejeita READ/NEW_INSTALL/x86, campos faltantes/overflow e resultado parcial.
- personal_scq_cold_acquisition_win32.inc: token original -> DATA no console ->
  seleção copiada -> cadeia nativa/target/inputs/confirmadores já existentes.
- personal_confirmation_console_win32.inc: mesma leitura de eventos/Stop/Esc/
  eco não secreto extraída em helper; confirmação continua comparação exata de
  buffer128bytes, sem preenchimento ou resposta automática.

A linha DATA é transitória no console/processo; não cria arquivo de seleção,
manifesto, formato público/persistido, protocolo ou canal. SCQ1/SQB1/SQB2,
receipts/close e PXT1 mantidos. Não há novo owner, serviço, root ou distribuição.

## Qualificação final

Quatro builds Selected/Legacy Debug/Release PASS, /W4 /WX, zero warnings finais.
Script offline build-p1-caller-offline.cmd; logs p1-<perfil>-<config>-build.log.
15 CTests/configuração, 60 pares PASS: p1-<perfil>-<config>-tests.log.

- Novo teste do corpo REAL do entrypoint:7cenários/14checks, todas as fronteiras
  owner/native/SCM modeladas. Ingresso específico e anterior permanecem distintos;
  READ/NEW_INSTALL, argumentos extras/ausentes e cap129 rejeitados. Fonte fria
  negada não pode chamar a CLI SCM.
- COLD_RAW_OPEN:47cenários/149checks. Parser e ligação scqc_plan reais; leitura
  de console/token/paths/NT/hash modeladas. DATA válida chega ao validador nativo;
  perfil divergente, seleção ausente, cancelamento, console indisponível e flag
  inválida não chegam a abrir arquivo/consumir ato. Nenhum RNG executado.
- Target P0–P7:17cenários/95checks, incluindo substituição/SD/reparse, recomposição
  do receptor e coexistência que nega antes do reader em124.
- Regressões de caminhos/READ/NEW_INSTALL, originais/seleção, replay/consumo,
  G3, Stop, retenção, receipt/close e readiness; presenters de instalação e registro
  e sete casos do helper de confirmação preservados. DATA lida não altera atos.

Isso qualifica composição offline, não prova autenticação bilateral nativa,
capacidade da sessão ou estado atual do host/SCM.

## Recursos e delta

Guard pré-open124, cap128 e G3 intactos: privado/WS1994944 por participante,
físico7239172096, commit9823326208, freshness100ms, original240000ms e global60000ms
sem renovação. Nenhum handle excluído, fechamento antecipado ou quota nova.
Nenhuma nova observação executada. +23 histórico continua sem atribuição interna.

Censo sizeof permanece Selected2002000/Legacy1833432. Seleção DATA usa72bytes e
linha256bytes automáticos; parser possui cópia72bytes, além dos escalares/frame.
Usa a reserva de stack existente65536bytes; não apresenta essa soma como pico
físico/commit ou ceiling de CRT/provider/kernel. Leitura não abre novos handles
explicitamente; recursos internos do console/CRT continuam contados e opacos.
Preimage/target e provas retidas permanecem no packet anterior.

Pico parcial conhecido de referências continua B+14 enquanto raw3, cadeia5,
duplicatas3 e readers3 coexistem. B é observado no processo, não selecionado
como113. Para apenas essas aquisições, sem custos adicionais, exige-se B<=110
pela guarda do último pre-open. B113 levaria a DENY antes do primeiro reader.
Não é prova de insuficiência/suficiência de uma execução futura nem ceiling.
SCM/peers/transitórios continuam a exigir seus próprios gates e contagem total.

Workspace Selected/Release final:772608bytes,
SHA256 54d374b2fddac81e30b4f8bb574fbaf3446b5a52cbdb002769fe5cdcfe1b4a50.
Comparado ao último offline771072bytes: +1536bytes. Não selecionado para operação.
Imagem instalada histórica:763904bytes, ID417614100:1835008:134613,
SHA2567d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d.
Delta de tamanho+8704bytes; instalada NÃO atualizada, sem nova consulta factual.
DLLs workspace idênticas aos pins: ssl3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed;
crypto09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
Nenhuma DLL, serviço, utilitário protegido ou EXE retido foi alterado.

## Primeiro gate e caminho mínimo

Bloqueio técnico do caller P1 fechado offline. Primeiro gate operacional:
preparar UPDATE U1–U7/U2 da imagem qualificada usando papacc_update.exe existente,
com disposição/retidos, fonte DATA, hashes e gates frescos; autorização própria,
sem executar. Não reutilizar nenhuma autorização consumida.

Depois de UPDATE qualificado, sua identidade real permite preparar novo pedido/
decisão P1 e sessão E/Q/S independentes, com a linha DATA exata no procedimento
manual e três confirmações não automatizadas. Estado fresco SCM/arquivos/recursos
continua obrigatório. PID/token real/bilateral/receipt/close são provas de sessão,
nunca PASS antecipado. Se quota negar, preservar a primeira causa e não repetir.

Próximo: NEXT-P1-QUALIFIED-IMAGE-UPDATE.md. Start/endpoint/SCQ NOT_AUTHORIZED;
readiness produtiva DENY. Retenção histórica, causa1816 histórica e OS UNKNOWN.
ED11/Phase6 PARTIAL, E21 PENDING; históricos/pins/EXEs/atos consumidos/PST0.6.4 intactos.
