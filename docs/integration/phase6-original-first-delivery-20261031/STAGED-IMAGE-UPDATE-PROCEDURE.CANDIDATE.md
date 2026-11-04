# Atualização da imagem staged — procedimento candidato, NÃO EXECUTÁVEL

Não há autorização nesta task para sobrescrever/copiar, alterar SCM/ACL, Stop, limpar ou atualizar. O EXE novo ainda não é seleção operacional. Este documento delimita o delta e os gates; não é um comando ou aprovação.

Destino existente: C:\Program Files\Papinho\Accelerator\papacc_server.exe.
Identidade histórica: volume417614100/high6225920/low71237, hash c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf, 571392 bytes.
Fonte candidata: C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe; hash890dd9276993f8cf80b089f7dd33d5feb7ac71d0e8af45d2ef4740e067fb22db, 582144 bytes.
Delta lógico: ato Start independente no source existente; primeiro preparo do receptor no packet; validação e retenção de close. Tamanho +10752 bytes não mede custo de execução. A necessidade de atualização decorre desses novos símbolos e caller, não apenas de diferença de hash.

Ordem governada proposta:

1. Terminar o ingresso nativo antes de congelar o artefato final. Não solicitar seleção repetida de binários ainda mutáveis.
2. Identificar no owner UPDATE existente uma operação aplicável à substituição de imagem protegida. O stager LIMITED_QUALIFICATION atual cria exclusivamente objetos ausentes e NÃO permite overwrite/adopção; sua aprovação utilizada não é autorização de update. Não deduzir autorização de F0, R0–R9 ou do layout.
3. Preparar pedido separado com identidade original de serviço/target/EXE/DLLs, novos bytes/pins e tratamento de material antigo, falha parcial, Stop e retenção. A conta, configuração e caminho SCM atuais não devem ser alterados por consequência automática.
4. Gates frescos sob futura autorização: originals/owner/DACL/reparse/ancestrais/continuidade, estado parado e inexistência de conflito/lifetime ativo, hashes de fonte e destino, recursos aplicáveis e consumo antes da primeira mutação. Descritor inadequado não autoriza ajustar ACL. Não assumir quiescência OS pelo readback histórico.
5. Uma eventual substituição deve usar primitiva governada já qualificada, preservar originals durante o efeito, verificar conteúdo/descritores/readback e manter UNKNOWN/retidos quando inconclusivo. Não improvisar Copy-Item -Force, DeleteService, rename/cleanup ou rollback.
6. Reconciliar o resultado antes de preparar Start. Autorização de update não autoriza Start/Stop nem endpoint.

Primeira lacuna para tornar este procedimento executável: identificar e qualificar a primitiva governada aplicável a esse destino já existente. Não demonstrada nesta task. Se não existir, implementar offline sob contratos UPDATE aplicáveis ou demonstrar a cláusula normativa faltante; não criar nova ADR por conveniência. Não executar comandos com placeholders.
