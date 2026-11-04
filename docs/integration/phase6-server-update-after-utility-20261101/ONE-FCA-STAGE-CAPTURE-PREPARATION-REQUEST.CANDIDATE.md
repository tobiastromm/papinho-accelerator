# Pedido operacional — preparação FCA independente com captura de etapa
Status: CANDIDATE / AGUARDANDO AUTORIZAÇÃO ESPECÍFICA. Não executado pelo agente.

## Escopo solicitado
Uma execução manual PERSONAL_LOCAL Selected AMD64 da preparação limitada NEW_INSTALL_CA_PREPARE, conforme FCA0–FCA7 e PDP0–PDP7 accepted. Reutilizar configuração e parent originais D1/PDP verificados com fechamento conhecido. Não recriar documentos, Papinho, fixture ou infraestrutura.

Autorizar somente: UMA criação exclusiva do novo diretório C:\ProgramData\Papinho\Accelerator, com proteção final no ato, e UMA criação/finalização de chave MACHINE CA pelo owner existente, com identidade pública/readback/proteção e fechamento. O nome da chave, IDs a/g e nonce serão produzidos pelo owner nativo; não são valores preautorizados inventados. Colisão, falha ou resultado inconclusivo: STOP, sem adoção, retry, limpeza, rollback ou refund.

## Originais e proveniência
Host TCORP-TOBIAS; operador S-1-5-21-785699550-1440458426-1682014700-1108, contexto administrativo original não impersonado.
Configuração C:\Program Files\Papinho\Accelerator\FIRST-INSTANCE-PREPARATION.CONFIG.ORIGINAL.conf: 43 bytes; identidade 417614100:2490368:315690; SHA256 16c4847a27d32689ad7eb2c0bd29f6f0f698df74092e9df107cc8203e07e0ae7.
Conteúdo UTF8 sem BOM: format_version 1 seguido de LF, client_admission DISABLED seguido de LF.
Parent C:\ProgramData\Papinho: identidade 417614100:1310720:452619; owner operador; DACL protegida operador/SYSTEM/Administrators FA OI|CI, verificada no ato PDP.
ProgramData ancestral original: 417614100:131072:227641; exceção restrita accepted PDP, sem dispensa de outros gates.
Âncora ProgramFiles 417614100:131072:226316; binary parent 417614100:8257536:71209.
A reconciliação de D1/PDP provém de OWNER-PDP-PARENT-PREPARATION-OUTPUT.txt, SHA256 b8bd090e16a4ac76b311ea85ff9f40c22bfd09081b835dc668052ac96b50b780. Autorizações D1/PDP e UPDATE consumidas, não reutilizáveis.

## Executáveis
Imagem canônica C:\Program Files\Papinho\Accelerator\papacc_server.exe: 911360 bytes; identidade 417614100:8716288:318363; SHA256 747ba426d55af50947169a178c8366def46cb57ad4a8c29308ff44664aeaee96.
libssl-3-x64.dll: 1321472 bytes; SHA256 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed.
libcrypto-3-x64.dll: 7217664 bytes; SHA256 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
Imagem com captura de etapa já instalada e reconciliada. Quatro builds sem warnings e120 pares teste/configuração PASS em FCA-PREFLIGHT-STAGE-OFFLINE-QUALIFICATION.json; nenhum rebuild ou novo UPDATE necessário para este pedido.

## Procedimento exato
Wrapper run-fca-stage-capture-preparation.MANUAL.CANDIDATE.ps1, SHA256 f664df8123bb7098d9df6c83829f7fa708f242ba27f1cad0673ee106576e91ec.
A sequência nativa é exatamente os dez argumentos:
config first-install --declare NEW_INSTALL_CA_PREPARE --source "C:\Program Files\Papinho\Accelerator\FIRST-INSTANCE-PREPARATION.CONFIG.ORIGINAL.conf" --parent "C:\ProgramData\Papinho" --container Accelerator
O wrapper exige hashes reais do próprio script, deste pedido e da decisão específica futura OWNER-FCA-STAGE-CAPTURE-PREPARATION-APPROVAL.txt. Esse arquivo de decisão ainda não foi emitido. Depois da autorização será registrado seu hash real e fornecida a linha PowerShell completa, sem placeholders.
O wrapper abre somente referências existentes, confere IDs/tamanho/hash em handles retidos, verifica componentes sem reparse e mantém referências durante a chamada única. Direitos metadata-only de diretórios não incluem TRAVERSE; apenas o parent de criação permite share WRITE, mantendo share DELETE negado. Arquivos negam share WRITE/DELETE. Não altera ACL.
Esses prechecks são dados, não autoridade ou atestação da imagem mapeada. O owner nativo readquire e valida originais, identidade, owner/DACL, ancestrais, ausência de reparse, proteção e continuidade; apresenta o plano original. Operador compara source/parent/leaf/imagem/escopo e só então responde à confirmação atribuída OWNER_NEW_INSTALL_CA_INITIALIZE com o nonce original exibido, sem resposta automática. Cancelar em divergência. Não pedir PID/token de runtime futuro: não há Start neste ato.

## Gates frescos e limites
Seleção explícita FCA, fonte original, contexto administrativo, revalidação e consumo único antes da submissão; Stop e guards nativos intactos. Entrada máxima128, pré-aquisição DENY >=188, máximo192, private growth1994944, working growth3145728, physical floor7239172096, commit floor3221225472 bytes, freshness100ms, janela240000ms, global60000ms sem renovação/rebase.
Custos opacos UNKNOWN; valores não provam suficiência nativa. Sem fallback de perfil ou quotas.
Diretório novo protegido no create e readback original obrigatório. Chave CA mantém política vigente: deny serviço, allow operador/SYSTEM, validação nativa de owner confiável e DACL protegida. Não aplicar template de fixture S0–S5. Incompatibilidade, colisão, Stop, deadline, falta de recursos, query indisponível, proteção/continuidade divergente ou fechamento desconhecido mantém DENY/UNKNOWN.
Não inferir consumo apenas do exit. Resultado parcial persiste; close desconhecido retém recursos/backings no owner. Não encerrar processo retido automaticamente.

## Efeitos proibidos e riscos
Sem assinatura, certificado, CSR, exportação privada, emissão, CA/CURRENT produtivo, A/P/I, NORMAL, readiness, endpoint, SCQ, Start/Stop, SCM/ACL, rede, TPM/NV/Q4, fixture adicional, observação independente, UPDATE, limpeza/restauração/refund/kill.
Risco aceito solicitado: criação parcial, chave persistente mesmo em falha posterior, custos opacos e retenção, sem rollback automático. P4/K5 e isolamento do runtime continuam sem PASS presumido. Retenção histórica e quiescência OS UNKNOWN, PST0.6.4 preservado.

## Reconciliação futura
Ler a saída integral: primeira falha e primeira etapa, confirmação/consumo, criação exclusiva e ID do diretório, nome/identidade pública da chave realmente retornados, proteção/readback, fechamento e retenção. Não repetir em qualquer falha. Exit0 não basta para readiness. Nenhum resultado futuro é declarado PASS por este pedido.
## Independência desta tentativa e limitação diagnóstica
Pedido novo, decisão e confirmação nativa futuras independentes. Não rearmar nem executar o wrapper da tentativa antiga2d7d03574ffcacce1bffd64e3510e7082d4e7c5841c1d74cdf157334957ec8a3; tentativa manual usada, consumo nativo de efeito não observado. Configuração/Papinho permanecem os originais já criados com fechamento conhecido. Não materializar novos documentos/configuração/parent.
UPDATE877f8a5c4a5d4991703a5bd87e431f6415f8b5d93d7e9817f350e6635c4e4896 consumido e reconciliado LIMITED_UPDATE_VERIFIED_CLOSED. Fonte OWNER-FCA-PREFLIGHT-STAGE-IMAGE-UPDATE-OUTPUT.txt SHA2560170cfbbe94b34a73201ea09c8dc4c2f61ef728ead5e94d86a773a05e64ca07b; novo canônico417614100:8716288:318363, anterior preservado no leafpapacc_server.5dee322236e8a49cd9dfa22a5e6cafbbc12e113191e6919970e65012026ece08.previous.
A tentativa antiga retornou sem apresentação/confirmacão/worker demonstrados pelo percurso, com flagszero e close conhecido; flagszero não são consulta independente de ausência. Nesta tentativa, colisão/objeto existente nega criação exclusiva; não adotar nem limpar.
Se houver nova rejeição anterior à apresentação, FCA_PREFLIGHT_REJECT identifica somente a região1–12. operation_result é o último retorno capturado nas chamadas4/11, não errno/NTSTATUS do predicado corrente nas outras regiões. Não presumir causa histórica resolvida.
Este pedido solicita o mesmo escopo limitado de um diretório/uma chave caso TODOS os gates e a confirmação atribuída passem; não é coleta pura ou autorização automática para efeitos. O operador pode cancelar a apresentação. Nova falha exige STOP, sem repetir.