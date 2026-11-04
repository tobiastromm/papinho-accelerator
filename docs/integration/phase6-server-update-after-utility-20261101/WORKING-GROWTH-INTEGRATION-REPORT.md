# G3-P1 — working set3MiB integrado e qualificado

## Resultado

O proprietário autorizou escolher valor adequado se o guard estivesse correto. O guard aplicou fielmente a seleção1994944; não foi demonstrado vazamento ou fechamento seguro. Seleção técnica delegada3145728bytes/3MiB registrada ANTES do patch em docs/adr/DECISION-personal-scq-p1-working-set-growth-20261102.md. O registro cita literalmente a autorização e distingue delegação da falsa alegação de que o proprietário escreveu esse número/hash. Accepted anteriores preservados.

3MiB é o próximo MiB inteiro acima da observação2129920, margem1015808bytes; não é pico nativo comprovado ou promessa de sucesso. Private continua1994944, memória física7239172096, commit3221225472, handles128/188/192, freshness100ms, janela240000/global60000, demais quotas e perfis intactos. Não se transporta teto gerenciado Selected por coincidência numérica.

## Código e prova

personal_scq_start_assurance_win32.h/.inc: constante única P1 working3145728; seletor P1 e comparação exata local, sem fallback a políticas antigas; guard incremental verifica o valor selecionado. Seletor legacy conserva1994944. READY não foi deslocado/reiniciado, nenhum trim/fechamento conveniente foi implementado. personal_scq_start_plan_data_win32.inc: preimage84bytes inalterada, discriminação do valor P1 antes do digest; receptor usa o mesmo seletor fixo. Policy nova0b260c85dfceec5e6e861bb998e4504723785b927d875d113d41a405e1ce1b60; antiga3961396a607d265f73c730e9599fa8208313a59e65832a24b170bf8206bd15a1 não pode ser reaproveitada nesta seleção.

Quatro builds MSVC/Ninja Selected/Legacy Debug/Release PASS sem warnings,60pares de testes afetados PASS. Fronteiras3145727/3145728/3145729, amostra histórica modelada (não repetida nativamente), preservação privada/legacy, política antiga inválida, overflow, query desconhecida e Stop qualificados. Teste de entrypoint verifica propagação do working correto. Sem aumentar layouts/estado persistente, handles ou consultas; formatos públicos/persistidos e PST0.6.4 preservados. Logs e fontes: WORKING-GROWTH-OFFLINE-QUALIFICATION.json; patch focal contra preimagens: WORKING-GROWTH-FOCAL-DELTA.diff.

Candidato Selected/Release congelado papacc_server.working-growth-798720.exe,798720bytes,SHA2566ad6e378ea5ec074ad6af44327b6fd1a22880a8cf18a8f02a3ecc28baf0edb9e, delta+512 contra o instalado798208/ID417614100:327680:333488/pin39a9da2f53ad6208ed875a916debd9c2fec760826f7a17d702d65afa28095b15. Instalado/updater/DLLs/SCM não tocados. Não houve coleta, endpoint ou Start.

## Menor caminho operacional preparado

Controlador21484/criação134381297590479169 retido na última saída. Não assumir término, fechamento ou readback SCM fresco. O proprietário autorizou aumentar o limite e preparar atualização/teste; isso não equivale à decisão específica de término forçado G5.

Pedido de disposição fixa ONE-WORKING-GROWTH-CONTROLLER-DISPOSITION.REQUEST.CANDIDATE.md,SHA25614fd1afb7d6a594d54e0f3a1c1b042b251d29576f82eb7977a83375fa756e491. Mesmo adaptador existente de uma única TerminateProcess ao objeto fixo, com processo/token/imagem e SCM STOPPED/PID0 revalidados; sem perseguir outroPID, retry, closeSCQ presumido ou limpeza.23cenários/15APIs modeladas, fonte real só compilada/AST PASS. Autoridade específica ainda necessária; nenhum arquivo de decisão antecipado. Não fornece efeito por DATA/hash.

UPDATE preparado condicionalmente: ONE-WORKING-GROWTH-IMAGE-UPDATE-REQUEST.CANDIDATE.md,SHA256a8206bfcb55be565a0fc640718da490a7fe4933e8a73b27a663075aad0c4bacd. Mesmo papacc_update.exe protegido/U1–U7/U2/16argv;22bindings/AST/helpers byte-idênticos PASS, sem invocação. O comando só pode ser disponibilizado após disposição suficiente e binding operacional aos bytes atuais, com gates frescos; não executar enquanto21484 estiver retido. Criação exclusiva, DATA retida, hash/flush, dois renames, anterior preservado, readback, Stop e retenção continuam. Nenhum UPDATE foi executado.

Depois da saída manual do UPDATE com nova identidade REAL, preparar sessão independente E/Q/S com working3145728/private1994944/policy nova e imagem atualizada. Não usar comando antigo/tokenfix ou decisões consumidas. Não inventar IDs/PID/token futuros. Provas nativas do runtime/bilateral/receipt-close/término ainda pendentes; prontidão produtiva, KSP/CA/CURRENT/rede/TPM/NV/Q4 DENY. Retenção histórica e quiescência OS UNKNOWN. Sem limpeza/refund/rollback/recuperação.
