# Persistência v3 no mesmo worker

## Limites da implementação

As decisões accepted PACROOT8/v3/F1–F5/G0–G7, D2/D3/D4, root/C8, E0–E8 e Model E permanecem intactas. O novo packet_format11 é um discriminador privado do packet do worker, não bytes wire/persistidos. Sua entrada produtiva não foi criada. O teste liga o mesmo código de commit/pending_worker em target isolado como os testes históricos; não introduz segundo worker no processo do produto.

A capacidade das três imagens committed/previous/next do owner existente aumenta de28.960 para30.592 apenas no perfil Windows3MiB explícito. O default/Legacy não muda. A imagem anterior não é sobreposta: worker recebe previous e next por valor; committed e snapshots ficam no owner. O worker não recebe root, pin, Session, callback de autoridade ou ponteiro caller.

Os guards do novo packet verificam magic/versão3, limites e offsets calculados da coleção/SNT/PDG/LST/carrier/checkpoint, SHA256 da imagem inteira, hash CP3 e conteúdo-base, R/predecessor/base/A/P/I e C/W referindo à mesma imagem. Nova CP3 deve estar AUSENTE (512zero): persistência não inventa publicação/epochs. O primeiro slice exige rows SNT/PDG e carrier16384 inteiros idênticos; nenhuma revogação/high-water/reserva/história é removida. Esses checks são integridade/correlação de packet previamente validado, NÃO substituem o codec completo, kind8, policy/status ou fontes factuais.

Begin08/v1 continua usando os guards históricos sem issuer. Packet11 somente é iniciado pelo helper sob PAPACC_ISSUER_STORAGE_COMPONENT_TEST, Windows3MiB/offline, recusando até o bit published diagnóstico. Sem novo símbolo público em header produtivo, CLI ou fallback. O helper não é autoridade: serve exclusivamente à qualificação nativa de IO de fixture, não bootstrap/CURRENT/predecessor administrativo.

## Transação e lifetime

Mesma CreateTransaction/CreateFileTransacted, OPEN_EXISTING e identidade nativa volume/fileindex; comparação inteira em chunks512 com previous imutável; escrita completa, SetEndOfFile, FlushFileBuffers, commit e closes do worker existente. A comparação nativa detecta conflito até no último byte. Não se usa digest no lugar de CAS integral.

Após sucesso factual, poll não sobrescreve committed nem publica pending; marca committed_pending/authority_lost. CONFLICT/UNKNOWN bloqueiam autoridade anterior. UNKNOWN conserva handles/transaction e unresolved; não é rollback. Stop pede parada sem espera, e rollback só é considerado factual pelo resultado nativo do worker. Enquanto thread não sinalizada e close confirmado não há liberação das imagens. Finish nega em pending/retained/unknown e nos resultados que perderam autoridade. G7 mantém suas regras de slot/serial/ticket/borrow; nenhuma segunda tabela de pins foi criada.

O prefixo anterior completo continua em previous e committed durante a operação e após sucesso até resolução governada; isso protege BYTES, não prova aprovação de história nem CURRENT. O novo caminho não emite receipt público de autoridade, checkpoint publicado, lease E0 ou fonte de confiança.

## Provas e limitações

O builder existente valida a relação v3 completa com callback MODEL antes do helper; reutiliza certificado público sintético histórico, sem nova fixture rejeitada ou assinatura privada. Native success/conflict/UNKNOWN/Stop são resultados reais TxF nas imagens máximas30.592. O sucesso compara o arquivo inteiro gravado; Stop/rollback confirmado compara o arquivo anterior inteiro. Todos conservam previous/committed/carrier e não publicam. CP3 corrompido, digest alterado, predecessor divergente e published indevido são rejeitados antes da thread. Esses negativos de corrupção não equivalem a provar status administrativo ou novo certificado nativo positivo.

As72 suítes adicionais cobrem codecs v2/v3, PDG revogado representável como DATA, divergências, kind8 nativo negativo, CURRENT ausente, pins, G7/Stop/retenção, limites e formatos históricos. Os16 casos nativos históricos exercitam ROOT06/high-water/história/snapshots, v1/CAS/Stop. Nenhum resultado é prova de CURRENT real.

## Gate exato antes de completar transação canônica

1. Definir no owner EXISTENTE o caminho de preparação do candidato ainda não publicado, com CA/source/policy/issuer factuais e lifetime; chamar o validador público kind8 sem fingir que candidato possui CURRENT pin. O dispatch atual serve consumo de backing protegido, não faz essa preparação. Se o contrato precisar extensão, propor somente essa fronteira antes de código.
2. Exigir CURRENT v2/v3 factual, pin/revisão/native identity e predecessor integral com CP/história de aprovação. Captura DATA/digest/pending.previous isolados não autorizam isso. O owner já informou indisponibilidade de instalação factual; não criar uma para suprir o teste.
3. Integrar prepare/validate e freeze dos inputs ao mesmo packet/worker, antes de abrir a entrada produtiva, incluindo base v2 e transições permitidas. Mudança de issuer/SNT/PDG depende de proveniência administrativa, não de relaxar memcmp. Preservar os registros anteriores.
4. Reconciliar reserva/retidos/stack/criptografia/coexistência e custos transitivos. Valores numéricos nos testes são guard lower-bound, não ledger factual ou crédito automaticamente disponível. Cenários acima do limite impedem ativação dependente.
5. Só depois de fonte/entrada/autorizações e ambiente próprios qualificar commit canônico, publicação separadamente autorizada e E0. Enquanto isso preflight termina NOT_SUPPORTED e trust_unavailable permanece SOURCE_ABSENT; M1/KDF continuam negados.

Este handoff delimita implementação ainda ausente; não aceita novo contrato, não autoriza executar cerimônia ou publicar CURRENT. [Campanha integrada existente](../canonical-bootstrap-v3-qualification-20261021/campaign.md) segue aplicável, com a capacidade/persistência restrita aqui atualizada.
