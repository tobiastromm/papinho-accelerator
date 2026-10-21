# kind8 no dispatch canônico — prova DATA e próximo gate

TASK=PACC-PHASE6-KIND8-DISPATCH-AFTER-G7-1. Autoridades accepted F1–F5/G0–G7, PACROOT8/v3/PDG1/CP3, issuer/root/C8, E0–E8, Model E e ED08/ED11 preservadas. A cópia própria G7 já foi provada na tarefa anterior. Esta integração não aceita nova norma, layout ou fonte factual.

## Caminho implementado

papacc_root8_issuer_carrier_data continua o dispatch único existente. Somente no perfil explícito Windows3MiB e composição offline D3, kind8 chama o componente público existente papacc_root8_signer_data. Kinds5/6/7 mantêm os predicados anteriores; kind desconhecido e Legacy não selecionado não fazem fallback. O callback BODY histórico permanece separado e inalterado.

O contexto privado PAPACC_ROOT8_CARRIER_DATA recebe versão explícita1, cópia do ticket canônico já mantido e descritores de arenas/scratch do caller. Contexto inteiro deve ser zerado; o caller histórico conhecido foi atualizado para isso. Tamanho Selected x64 passa32→112; não é ABI pública, campo wire, layout persistente ou nova identidade de operação. Todos os callers e objetos precisam do mesmo perfil de compilação. Contexto não cria/adquire pin ou ownership; a cópia do ticket não cria segundo leitor.

O adapter verifica versão, capacidades, somas, alinhamento do CA decode, ausência de alias entre owner/contexto/descritores/CA arena/signer arena/certificate scratch e limite inferior contábil. Ausência de pin/backing/arena, Stop, corrupção ou revisão divergente nega. borrowed packet/CA são obtidos por papacc_configuration_v3_borrow_data, que exige ticket/index/serial/leitores reais do owner e verifica integridade/root/R/predecessor/I e issuer snapshot quando presente.

Header deve ser exatamente borrow.packet; carrier deve ser exatamente o intervalo16384 apontado pelo header protegido; row deve ser exatamente uma das D linhas384 do PDG retido. Um clone byte-idêntico externo não recebe o lifetime do pin. Nenhum pointer de caller é usado como header/row protegido por igualdade de digest. Os4 listeners e linhas são preservados, sem first-match/truncamento da coleção.

O DER público da CA pertence ao backing imutável. CryptoAPI existente decodifica signed certificate/TBS em CA arena do caller, com NOCOPY referindo somente DER retido ou esse arena. CERT_INFO é emprestado apenas durante a chamada; não vira trust anchor. Signer arena e certificate scratch são disjuntos da CA arena, inclusive das suas dependências profundas e de TODO o owner. Não há CertCreateCertificateContext/handle/heap explícito, geração/importação de chave ou operação privada nova no caminho.

papacc_root8_signer_data fixo realiza seus checks existentes: versão3/PDG, issuer íntegro/root/I, propósito PACKAGE, registro/certificado/SPKI/CA/geração/validade/perfil e verificação pública X509. APPROVED e REVOKED continuam representáveis como DATA histórica; OK não significa status corrente ou delegação. Falhas não chamam formatos antigos. Após resultado OK, outro borrow válido e ausência de Stop são exigidos antes do retorno DATA OK. Nenhum decode CA ou ponteiro novo é devolvido ao caller; arenas são scratch sem objeto/borrow exportado após retorno.

## Ownership e lifecycle

Caller conserva o MESMO pin, contexto e arenas exclusivos até retorno factual; não faz unpin concorrente e não compartilha com operação/Session ou recurso retido. A função é síncrona/não reentrante, fora do caminho de Stop e de sp_trust_acquire. Stop continua sinalização existente, não cancelamento presumido de CryptoAPI. Nenhum pin, buffer ou crédito é liberado por este adapter, inclusive após erro/UNKNOWN/Stop. O owner original mantém quiescência, retenção, fechamento factual e exactly-once. RELEASE_INVALID/E0–E8 permanecem sem tentativa/efeito.

Pin histórico válido preserva memória da sua revisão; não comprova CURRENT. Ticket/serial obsoleto é rejeitado, mas histórico realmente mantido não vira revisão corrente. O código não publica CURRENT/CP3, não fabrica R/A/P/I, não altera CAS/high-water/revogações/event-prefix e não promove DATA a RESOLVED/TRUST.

## Handoff exato para fonte factual

1. O novo dispatcher já pode consumir kind8 DATA das referências protegidas. Futuro caller canônico deve fornecer o pin real, revisão apropriada e arenas previamente contabilizados; não substituir o contexto por fixture ou boolean de autorização.
2. CURRENT factual/CP3/full predecessor/commit v3 e aprovação da policy permanecem gates próprios. Ambiente CURRENT ROOT08/v3 positivo está PENDING_EXTERNAL_ENVIRONMENT; bootstrap administrativo positivo BLOCKED_BY_ENVIRONMENT. Não criar store/publicação para fabricar prova. Worker Selected ainda possui28960 por imagem, máximo v330592; resize/integralização não foi aplicado por este dispatch.
3. E0 acquire futuro somente envolve referências JÁ adquiridas, protegidas, contabilizadas e revalidadas por fontes factuais. Este adapter faz hash/decode/validação nativa anterior e NÃO pertence a acquire. Não implementa fábrica/source/lease positiva. Sem CURRENT/policy/signer-purpose/status/UTC/delegação completos, E0 e TRUST continuam DENY antes KDF.
4. Public CA/certificado validado matematicamente não supre autoridade administrativa. Aprovação do assinante, revogação/validade vigentes, confirmação independente do Browser e autorização local são distintas. Browser não lê a root privada; nenhum Browser/SDK distribuído foi alterado.
5. Preservar STRUCTURE→SIGNATURE→TRUST→KDF→AEAD→CONTENT→RELEASE. Não reutilizar snapshot antigo como autoridade após CAS conflict/UNKNOWN ou fonte invalidada. Não executar efeitos dependentes enquanto ED11 total/fonte factual estiverem não qualificados.

Nenhum segundo dispatch/worker/root/journal/registry/pin system foi criado. O helper é exclusivamente a implementação de kind8 dentro do dispatch existente; não um catálogo de confiança. Extensões futuras exigem seus próprios hunks oficiais e provas, sem aprovação funcional por referência a este handoff.
