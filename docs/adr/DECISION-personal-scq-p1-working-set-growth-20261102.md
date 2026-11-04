---
status: accepted
date: 2026-11-02
scope: first PERSONAL_LOCAL P1 TRANSPORT_ONLY closed diagnostic attempt
supersedes: []
---
# Decisão — tolerância de crescimento working set G3-P1

## Autoridade e seleção delegada

O proprietário instruiu “prossiga, pode considerar autorizado atualizar e executar só me mandar os comandos” e, após a explicação de que faltava selecionar um novo limite G3, autorizou expressamente: “se o limite g3 é baixo e tá tudo certo no código, pode aumentar para valor adequado”. O código aplicou corretamente o limite anterior; não há vazamento ou fechamento seguro demonstrado. Esta é autorização explícita para selecionar uma tolerância adequada, não aprovação histórica de um número ou hash que o proprietário não escreveu.

Exercendo essa delegação técnica, seleciona-se EXATAMENTE3145728bytes (3MiB) de crescimento de working set POR participante sobre seu próprio READY, somente P1 PERSONAL_LOCAL TRANSPORT_ONLY no primeiro diagnóstico fechado. Registra-se antes do código dependente. Não se converte essa escolha em teto nativo demonstrado, garantia de sucesso ou novo direito de ação.

## Fundamentação e precedência exata

TOKENFIX-SESSION-RESULT.json/saída integral associada: controlador cresceu2129920bytes de working set, de7487488 a9617408, ultrapassando1994944 por134976. Private cresceu1204224, handles167<188, pisos físico/commit satisfeitos. A falha ocorreu antes de Start. Não se atribui crescimento a vazamento, console ou provider sem prova.

A alternativa de manter1994944 preserva a negação observada.2MiB(2097152) também é inferior ao crescimento observado; mero ajuste até2129920 não deixa margem.3MiB é o próximo MiB inteiro acima da observação, com1015808bytes de margem (aproximadamente47,7% sobre essa observação) e aumento1150784 sobre a tolerância anterior. Isso fundamenta uma ESCOLHA limitada de risco, não prevê picos futuros. A igualdade numérica com o teto gerenciado Selected não importa esse teto: working set, private, arquivo PE, memória gerenciada e memória física são domínios distintos. Runtime ainda não tem envelope nativo máximo demonstrado; seu risco permanece explicitamente opaco.

Especializa somente working_growth1994944 da tabela G3 e DECISION-personal-scq-initial-query-g3-20261031, e as passagens que o preservam como “demais parâmetros inalterados” nas decisões P1 de handles/commit, exclusivamente neste perfil. Nenhum documento accepted anterior é editado nem supersedido integralmente. READ, NEW_INSTALL, TRANSPORT_ONLY sem P1, SAM, LAB, UPDATE, SCM e demais domínios mantêm seus contratos e limites anteriores.

Private_growth continua1994944; físico7239172096; commit3221225472; entrada128/guard188/cap192; freshness100ms; janela original240000ms/global60000ms compartilhada e menor deadline sem renovação; memória gerenciada e demais quotas permanecem. Contagem total e baseline READY original íntegros: não trim/EmptyWorkingSet, exclusão de custos, troca de baseline ou deslocamento da guarda para produzir PASS.

## Implementação e garantias preservadas

Atualizar seletor/validador P1, produtor/controlador e política receptora fixa no mecanismo existente, com digest quantitativo novo nas duas pontas, sem mudar formatos públicos/persistidos ou aumentar campo recebido por fallback. Seleção P1 ausente/divergente, policy antiga ou valor arbitrário mantêm DENY. Queries incompletas, Stop, expiração, UNKNOWN, overflow e recursos insuficientes negam.

Preservar P0–P7, OHI, QUERY/SQB2, SSO-B, TO0–TO9, G0–G9: origem/identidade/proteção/continuidade, três atos independentes/consumo, bilateral, receipt/close, Stop, reservas e retenção UNKNOWN. Não conferir autoridade por hash/DATA/token/carrier/imagem ou orçamento. Produtivo/KSP/CA/CURRENT continuam DENY.

Autoriza implementação/qualificação offline desta seleção e preparação governada da atualização e da sessão solicitadas pelo proprietário. Comandos manuais devem vincular artefatos/efeitos atuais e gates frescos; não se fabrica confirmação nem se executa automaticamente. Não concede término forçado do controlador21484 retido, limpeza/refund/rollback/recuperação, novos serviços/owners/canais/roots, ACL/SCM ou autoridade produtiva. Disposição excepcional exige decisão posterior específica G5; autorizações anteriores consumidas não são reutilizadas.

Riscos nativos opacos, retenção histórica e quiescência OS UNKNOWN. PST0.6.4 preservada. Modelos/builds não qualificam operação real; atualizar documentação viva e entregar hashes reais antes de qualquer operação manual futura.
