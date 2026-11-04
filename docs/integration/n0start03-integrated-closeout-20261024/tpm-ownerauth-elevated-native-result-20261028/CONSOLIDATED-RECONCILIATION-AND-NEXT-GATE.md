# STORAGE_20 administrativo — fonte indicada, custódia não concedida

Reconciliação da saída manual recebida em2026-10-28, continuação de PACC-PHASE6-ED11-TPM-OWNERAUTH-ELEVATED-METADATA-RECONCILIATION. Não foi executada consulta pelo agente nesta reconciliação.

## Resultado novo

A saída informa open_rc0/query_rc0/type13/query_attemptedtrue/output_bufferNULL/value_readfalse, stateMETADATA_INDICATES_SOURCE_PRESENT_NOT_VALUE_OR_CUSTODY_PROOF, close_rc0/context_closetrue. O código qualificado só emite esse state quando a consulta de metadado retorna tamanho positivo. Portanto o resultado relatado supera a consulta inconclusiva anterior NO ESCOPO da observação recebida: há indicação de fonte STORAGE_20 disponível pela API oficial. Não é prova de autorização vazia, valor exportado, posse do operador ou autorização NV.

TPM_Submits0 significa zero envios diretos do coletor; internos Windows/TBS continuam UNKNOWN. NV_mutationsNONE e authorityDENY. Amostras próprias48 antes e56 após load; não são pico total ou orçamento administrativo, nem headroom NV. Contexto fechado conforme retorno reportado; não se afirma fechamento de todos os internos ou quiescência global. Sem retenção incerta reportada no caminho observado. Nenhum segredo/hashde segredo foi solicitado/recebido.

Fonte antes UNKNOWN devido INTERNAL_ERROR, agora OWNER_REPORTED_METADATA_SOURCE_PRESENT_AT_OBSERVATION. Histórico preservado: erros anteriores não apagados nem convertidos retroativamente em PASS. PolicyOSManagedAuthLevel5 não é inventário/garantia de ausência de storage auth; a indicação positiva não autoriza adotar lockoutAuth nem explica por si só a causa do erro anterior. TPM2STORAGE_20/type13 e lockoutAuth continuam separados.

## Alcance e proveniência

O texto colado mostra & $coletor ANTES da linha Get-FileHash e da atribuição de caminho. Não podemos demonstrar pelo trecho que o hash foi validado ANTES da execução ou qual era o valor anterior da variável. O binário/source atuais foram reconferidos e correspondem ao pinb5d3dae5f17f890429639de538f4e0d49a8d52aa71f0c04e31688d090b34f64e; isso não reconstrói identidade nativa/PID/criação/arquivo no instante histórico. O coletor também não emite SID/elevation/PID/criação ou contexto administrativo autenticado. A execução é tratada como relato do proprietário com resultado compatível com o coletor, não atestação independente.

Essa limitação não requer repetir automaticamente a mesma consulta: já existe evidência discriminante de metadado e nenhuma autoridade operacional é concedida por ela. Qualquer futura cerimônia deve usar pins ANTES da execução e originais/token/processo/contexto frescos sob seus próprios gates. Não reexecutar prova matemática consumida. Transcrição copiada do chat com quebras de linha normalizadas, JSON derivado e hashes locais preservam a evidência textual; não são assinatura ou aprovação.

## Fonte e custódia

O Windows/TBS oferece a interface legítima candidata de entrega em memória via Tbsi_Get_OwnerAuth STORAGE_20. A observação indica disponibilidade de metadado nesse contexto, não identifica holder humano nem prova que os bytes sejam válidos/permitidos para a operação. Fonte gerenciada pelo Windows é distinta de custodiante do produto. SID1108 permanece candidato, não confirmado por este JSON e não efetivamente designado. A aceitação TC anterior não concede acesso ao valor.

WINDOWS-STORAGE20-CUSTODIAN-DESIGNATION.DRAFT.json torna a próxima decisão concreta: SID1108 como custodiante proposto do witnessQ4, fonte API oficial/type13 indicada, uso somente pelo owner/worker existente, entrega controlada em memória sob originais/token/purpose/manifests verificados, sem cópia de hierarchy secret em vault adicional. Per-index credential32bytes é outro segredo, só numa futura provisão especificamente aprovada, com backingL1/RAW1/DPAPICurrentUser e geração/índice/propósito. Nenhuma credencial foi gerada/instalada ou recuperada.

O draft NÃO aprova leitura do valor, definição/incremento de índice, root write, Q4, revogaçãoUNKNOWN ou geração física. Seu approval continua null. Designação humana/source em si ainda precisa decisão específica; provar entrega/validade em memória dependerá de autoridade própria e implementação/qualificação do procedimento. Não usar consulta administrativa como setter de autoridade. Documento de revogação/acesso não apaga segredo já conhecido; limites same-user/admin e requisitosL1/TC permanecem.

## Primeiro gate concreto

Preparar/obter decisão específica de designação do custodiante e uso da fonte WindowsSTORAGE_20 indicada, sem repetir aprovação genéricaTC. Depois qualificar a entrega controlada e o futuro sender autorizado, mantendo proibição de recuperar valor sem autoridade própria. Não é necessário trocar de testemunha por causa do antigo INTERNAL_ERROR: agora a linha de fonte local tem evidência positiva limitada. A alternativa externa permanece PROPOSED/reserva condicional, sem ativação. Se a fonte não puder fornecer autoridade legítima sob os gates aprovados, voltar à alternativa explícita, nunca senha vazia/Clear/adoption/retry.

Âncora Intel local ainda pendente de designação; revogaçãoUNKNOWN permanece strictDENY, exceção diagnóstica não concedida. Nonrestorable hardware/NV generation, original provisioning lifecycle e persistência não comprovados. Por isso NV provisionNOT_READY e pedido operacional executável ainda não existe. Após designações/fatos/adapter-sender/resources, uma futura operaçãoNV requer pedido exato e aprovação específica; readback/disposiçãoQ4 independentes.

## E21 e frentes independentes

E21 recebe evidência limitada de fonteSTORAGE20, não PASS de custódia/persistência/readback. Política DATA no owner continua authority_denied/SOURCE_MISSING; nenhum status foi alterado por booleans de relato. E17 identidade/acesso/readiness e E18 Stop/lifecycle factuais; E19 D4/D5 quantitativos e E20 bindings/composiçãoBrowser/C2/C3/provider permanecem abertos independentes. FonteTPM não fornece esses fatos/limites. E22 derivado, ED11/Phase6 PARTIAL; reservas/UNKNOWN retidos.

Nenhum build/teste repetido sem alteração funcional, nenhum TPM/NV/Q4/Start/Stop/SCM/ACL/cleanup/refund/CURRENT/signature/recovery executado pelo agente. HEAD/pins/históricos/autorizações consumidas preservados. SHA256SUMS.json e FINAL-VERIFICATION.json registram a conferência e os novos documentos, não nova aprovação.
