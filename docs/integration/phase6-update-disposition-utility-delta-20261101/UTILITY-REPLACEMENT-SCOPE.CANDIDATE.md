# Especialização focal necessária — manutenção de UM utilitário UPDATE

Status CANDIDATE / NÃO ACEITA,2026-11-01. Somente preparação documental/técnica.
Não autoriza implementação dependente, bootstrap, execução, retry ou recuperação.
Documento designado da mesma frente, sem outra família de ADR/infraestrutura.

## Lacuna exata e alternativas

U1 accepted: atualizar apenas papacc_server.exe; DLLs idênticas nunca copiadas.
U2 accepted: executor independente/protegido; proíbe self-update do executor vivo.
Preparação exclusiva accepted029a611b…07fb: UM FILE_CREATE papacc_update.exe;
leaf existente nega, nenhum rename/overwrite/adoção. Exceção bootstrap limitada
somente àquela preparação. Autorização utilizada não concede manutenção.
U4 reduz custos opacos somente para o ato UPDATE servidor, não qualquer efeito.

Portanto não há mecanismo accepted que atualmente substitua o utilitário
protegido. Reutilizar primitives não transfere escopo/autoridade. Nenhuma
incompatibilidade entre decisões accepted é alegada: há um efeito ainda não
coberto. Mantém-se DENY até decisão focal e autorização operacional separadas.

A — manter tudo estrito: executar por controller Papinho protegido independente,
capaz de manter utilitário. Nenhum demonstrado no inventário canônico. O server
instalado antigo não ganha capacidade por presença; o próprio utilitário não
pode se substituir. PowerShell protegido não é automaticamente esse owner.

B — recomendação mínima: permitir SOMENTE um ato administrativo independente
de substituição individual papacc_update.exe, pelo controller server de build
qualificado do MESMO projeto/owner, sob bootstrap explicitamente delimitado.
Reutilizar packet/primitivas e a correção same-directory já qualificada; não
criar outro executável/updater-do-updater, root, serviço ou distribuição.
É necessária decisão expressa de escopo e confiança bootstrap, não apenas
nova autorização operacional dos pedidos anteriores. Não se propõe inverter
nomes, trocar arquivos de lugar ou usar DATA como executor para contornar U2.

## Condições indivisíveis propostas

1. Escopo exclusivo PERSONAL_LOCAL, mesmo parent protegido existente e UM
   leaf papacc_update.exe. Propósito próprio UTILITY_UPDATE_CLOSED; não chamar
   UPDATE do servidor, self-update, preparação original ou limpeza por equivalência.
   Serviço/conta/comando/configuração permanecem inalterados e STOPPED/PID0
   revalidados. Server/DLLs/ProgramData e candidato da tentativa anterior intocados.
2. Controller é papacc_server do build qualificado, nunca utilitário alvo ou
   DATA. Aprovação operacional futura seleciona hashes/proveniência finais após
   implementação e qualificação. Aceitar SOMENTE nesse ato bootstrap de controller
   ainda no workspace: custódia original de cadeia/imagem/dependências, pins,
   reparse/aliases negados, exclusão de writers/deleters, contexto/token/PID/
   criação originais e confirmação atribuída. Não atesta páginas/intervalos
   anteriores/host comprometido e NÃO reduz U2 no UPDATE servidor posterior.
3. Utilitário fonte é DATA não executada; original retido, SD interpretado,
   tamanho/SHA selecionados e cópia independentemente verificada. Parent,
   utilitário antigo, candidato protegido e demais originais instalados mantêm
   owner/DACL efetiva/identidade/ancestrais/continuidade estritos. Nunca ajustar
   ACL real ou conceder capacidade por hash/nome/owner isolados.
4. Mesmo owner/caller existente, plano imutável com serviço/ator/propósito/
   referências antigas e novas/nomes/pins/custos/efeitos/tentativa/lifetime.
   Declaração/confirmação independentes, consumo único antes de qualquer efeito.
   Nenhum grant persistido, setter, ledger, canal ou autoridade administrativa
   reutilizável. Recusar replay/substituição/concorrência e alvo igual ao executor.
5. Uma criação relativa exclusiva de candidato do utilitário; cópia/hash/flush,
   DACL protegida e readback, dois renames sem overwrite usando handles originais
   e a correção same-directory. Antigo vai a leaf .previous exclusivo e permanece
   como objeto, novo vai ao leaf papacc_update.exe. Nomes exatos mostrados no plano
   fresco; nenhuma identidade futura inventada. Sem delete/rollback automático,
   diretórios novos ou alteração de objetos da tentativa anterior do servidor.
6. Censar composição controlada final, referências/frames/coexistência e retenção,
   manter quotas geridas existentes sem aumento. Margens frescas física/commit
   >=K próprio e espaço real arredondado para o payload antes de criar/escrever,
   conforme fundamentação final; não transportar K/B3/R3/G3 ou handles124/128.
   Propõe-se aceitar explicitamente SOMENTE aqui riscos opacos de bootstrap,
   CLR/CRT/loader/alocador/hash/SD/SCM-RPC/filesystem/cache/flush/rename/kernel/
   retenção, sob ED11/W4/F8. UNKNOWN, não zero/ceiling/reserva. Nenhuma capacidade
   produtiva ou garantia de pico/latência/cancelamento imediato é concedida.
7. Stop e gates originais/recursos antes de cada criação/escrita/flush/rename,
   readback de nome/ID/hash/SD e SCM ainda parado depois; fechamento governado.
   Falha parcial pode deixar utilitário canônico ausente. Reter cohort/objetos/
   créditos UNKNOWN sem timeout cleanup/refund/restart/rollback/retry. Ausência
   de resultado conclusivo não autoriza executar o utilitário possivelmente parcial.
8. Antes de operação, reconciliar disposição factual anterior e delimitar o que
   continua UNKNOWN sem apagar/assimilar crédito ou promover observação a
   quiescência. Esta decisão não sana a retenção anterior e não autoriza reuso
   do candidato/server/ato consumido. Implementação depende de aceitação exata;
   depois qualificar quatro builds/regressões afetadas em modelos, congelar
   controller/DATA e formular UM pedido operacional independente. Sem execução
   automática, retry ou comando com placeholders.

## Garantias e riscos

Reutilizados U3/U5/U6 como desenho: originais, criação exclusiva, cópia/hash/
flush, rename sem overwrite, preservação do antigo, Stop/readback/close/UNKNOWN.
Autoridade não deriva do utilitário ou de sua correção. Alterações normativas
estritamente necessárias: permitir alvo/purpose utilitário, bootstrap deste
controller para este ato e risco opaco próprio. Não alterar U1/U2 anteriores
nem transportar automaticamente exceções. Sem extensão a outros executáveis,
serviços, staging, Start, SCQ/READ/request/decisão, CA/CURRENT ou recuperação.

Risco residual para decisão do proprietário: controller de workspace sob
garantia reduzida, opacos/exaustão/I/O bloqueante, intervalo sem utilitário,
resultado parcial e retenção persistente sem rollback. Alternativa A preserva
garantia maior e mantém DENY na ausência de executor independente suficiente.
Escolher B não seleciona artefato futuro nem concede operação.

Não implementar dependente ou registrar ACCEPTED por inferência. Sem UPDATE,
preparação/renames/exclusão/restauração, SCM/ACL, Start/Stop, endpoint, rede,
CA/CURRENT, TPM/NV/Q4, limpeza/refund/recuperação nesta task. Preservar
HEAD/WIP/históricos/pins/atos consumidos/objetos/reservas/PST0.6.4.
