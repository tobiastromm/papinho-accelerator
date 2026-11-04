# Pedido operacional — preparação FCA independente com captura da aquisição
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
Imagem canônica C:\Program Files\Papinho\Accelerator\papacc_server.exe: 912384 bytes; identidade 417614100:2555904:1094956; SHA256 f4d91013a34da09053dcb800dcd909f82ba544384f1430d9c22584d1d24ea82b.
libssl-3-x64.dll: 1321472 bytes; SHA256 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed.
libcrypto-3-x64.dll: 7217664 bytes; SHA256 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
Imagem com captura de etapa já instalada e reconciliada. Quatro builds sem warnings e120 pares teste/configuração PASS em FCA-ACQUISITION-BRANCH-OFFLINE-QUALIFICATION.json; nenhum rebuild ou novo UPDATE necessário para este pedido.

## Procedimento exato
Wrapper run-fca-acquisition-capture-preparation.MANUAL.CANDIDATE.ps1, SHA256 f6e18558d056aef98d596a0dbf131c2308447796c8567ce9f1e7ab43ece69b3e.
A sequência nativa é exatamente os dez argumentos:
config first-install --declare NEW_INSTALL_CA_PREPARE --source "C:\Program Files\Papinho\Accelerator\FIRST-INSTANCE-PREPARATION.CONFIG.ORIGINAL.conf" --parent "C:\ProgramData\Papinho" --container Accelerator
O wrapper exige hashes reais do próprio script, deste pedido e da decisão específica futura OWNER-FCA-ACQUISITION-CAPTURE-PREPARATION-APPROVAL.txt. Esse arquivo de decisão ainda não foi emitido. Depois da autorização será registrado seu hash real e fornecida a linha PowerShell completa, sem placeholders.
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

## Independência e estado anterior
Novo pedido e decisão/confirmacão/nonce/consumo futuros independentes. Não repetir wrappers/aprovações das tentativas2d7d e ae3b já usadas. A última rejeitou aquisição original com stage4/INVALID_STATE antes de apresentação/consumo de efeito observado, com close conhecido; flagszero não são consulta de ausência. Nesta tentativa, criação exclusiva deve negar qualquer colisão, sem adoção/limpeza.
UPDATE5fb51f54b9832364c19cd79ea4f1189c24a54a43258aecb2720b9982bc575355 consumido, reconciliado LIMITED_UPDATE_VERIFIED_CLOSED. Saída OWNER-FCA-ACQUISITION-BRANCH-IMAGE-UPDATE-OUTPUT.txt SHA256b2c4d9c98e334c3763960334b83dea48d0b20f85847863f021e2cead300b5108. Novo canônico912384bytes/417614100:2555904:1094956/SHA256f4d91013a34da09053dcb800dcd909f82ba544384f1430d9c22584d1d24ea82b. Anterior preservado no leafpapacc_server.1985a8d920557df3e54412244722ef12f2376bbebb9c7fa7901d5ea52668f66a.previous. Não repetir UPDATE nem importar sua autoridade.
Configuração43bytes/417614100:2490368:315690 e Papinho417614100:1310720:452619 já concluídos: somente reutilizar, sem novas cópias/materialização/parent.

## Limite da captura e efeito solicitado
FCA_ORIGINAL_ACQUISITION_REJECT indica um dos26 ramos originais numerados, mantendo retorno/predicados/ordem. Ramos compostos ainda não são o subpredicado/NTSTATUS individual. FCA_PREFLIGHT_REJECT permanece captura de região; operation_result é retorno PAPACC_RESULT, não erro do Windows. Não se presume causa histórica resolvida.
Essa é uma preparação com efeitos possíveis (um diretório/uma chave) se a aquisição, o plano e a confirmação atribuída passarem, NÃO coleta pura. Não executar sob aprovação do UPDATE. Qualquer nova falha encerra a tentativa sem retry; se não alcançar apresentação não fornecer resposta nem reiniciar.