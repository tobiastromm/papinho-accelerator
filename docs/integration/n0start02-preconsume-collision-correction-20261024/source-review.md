# Revisão focal — ausência antes do consumo

HEAD/main 913c90cf2328f7939d8f5b927640601903ac6e91. Preimagens e pins anteriores em baseline.json. Autoridades: N0–N9, RAW1, B+D revisão2, ED11 revisão4, Model E e ED08, sem alteração normativa. A divergência comprovada e a aprovação condicional permanecem byte-exatas no pacote anterior; nenhuma aprovação foi promovida.

## Consulta e ordem

O mesmo ciclo/owner inclui ig_preconsume_absence, ig_absence_parent e ig_absence_name. Exige mutex canônico adquirido na thread atual, binding N0 bound, fase1, sem consumo, efeito, Stop ou retenção. O handle lab_parent original é conferido por lab_id (volume417614100, high1966080, low79426, diretório sem reparse) e lab_acl (descritor original capturado em lab_bound, protegido e permitido). Nenhum Test-Path prova ausência.

Para cada folha fixa, NtCreateFile recebe RootDirectory=lab_parent; DesiredAccess=FILE_READ_ATTRIBUTES|SYNCHRONIZE; share=READ|WRITE|DELETE; disposition=FILE_OPEN(1); options=FILE_OPEN_REPARSE_POINT|FILE_SYNCHRONOUS_IO_NONALERT(0x00200020); OBJECT_ATTRIBUTES=OBJ_CASE_INSENSITIVE|OBJ_DONT_REPARSE. Nenhuma criação, adoção ou escrita. Só STATUS_OBJECT_NAME_NOT_FOUND(0xc0000034) sem handle passa, após nova conferência do pai e Stop. PATH_NOT_FOUND, ACCESS_DENIED, SHARING_VIOLATION, reparse, sucesso sem handle e demais resultados não provam ausência. NTSTATUS original segue no primeiro diagnóstico existente.

Objeto encontrado implica colisão mesmo se diretório/reparse/outro tipo: referência adquirida deve fechar com sucesso antes de retornar FILE_EXISTS. Fechamento falho ou retorno inconsistente com handle preserva slot e retenção, sem segunda tentativa. STATUS_PENDING inesperado retém também o frame estático de OBJECT_ATTRIBUTES/UNICODE_STRING/IOSB; o buffer aponta para strings estáticas. Não há borrowed storage de stack abandonado. Limpeza recusa retenção e o processo permanece governadamente retido. Não liberar mutex/handle à força.

Sequência real: binding/exclusividade; censo inicial; confirmações exatas; consulta imagem e sink; validações protegidas completas/identidades/token/SCM/censo; coleta inicial de métricas; revalidação canônica final e arm; consumo; efeitos. ordering-verification.json registra posições do código. Após placement/rebind, permanece a segunda coleta final antes de Start. Não foi criada terceira geração, reamostragem em loop ou mudança dos limites. O contexto de60s e a idade máxima de1000ms continuam nos mesmos gates; amostra1001ms nega. As consultas potencialmente lentas precedem métricas e não renovam contexto.

Mask3 apenas comprova que os dois probes passaram nesta invocação, não concede autoridade nem reserva nomes. As criações finais existentes continuam FILE_CREATE/CREATE_NEW. Uma corrida externa posterior pode falhar após consumo; conserva tentativa consumida e efeitos parciais, sem overwrite/delete/rollback/retry. Essa limitação permanece explícita para aprovação.

## Recursos e provas

Frame fixo adicional medido:96 bytes no buildx64. O probe usa o slot cycle_file existente: no máximo uma referência transitória de colisão, imediatamente fechada ou retida em UNKNOWN. Nenhuma thread, processo, logger, owner ou mecanismo de exclusividade novo. Hash/ACL usam primitivas existentes; custos transitórios/kernel não observados continuam UNKNOWN. Sem aumento de quotas/créditos. 128 handles nativos,1024 administrativos compartilhados,quatro papéis,60s/1s preservados. Testes não demonstram que uma operação real passará ED11.

36 execuções de suites (nove em quatro configurações), launcher offline e16 CLI inválidas PASS. O fixture de diagnóstico contém111 casos, incluindo19 novos cenários do gate: ambos ausentes, imagem/sink existentes, pai/file-ID divergente, reparse, acesso/sharing/path ausente/erro desconhecido, pending, fechamento incerto, exclusividade ausente, handle inesperado, sucesso sem handle, ACL insegura, replay, Stop e colisões posteriores de imagem/sink. Falhas prévias afirmam consumed0/effect0/created0/starts0; corridas posteriores preservam consumed1/effect0. Sucesso sintético não é prova nativa de ausência.

O teste adicional de concorrência entre processos usa o fixture canônico existente e nomes Local isolados; foi bloqueado no primeiro CreateMutexExW por1307, antes de aquisição. Não houve segundo processo concorrente nem prova nativa de exclusividade nesta tarefa; não repetido/elevado. A guarda de ausência de posse canônica passou no modelo; o owner/mutex canônicos permanecem inalterados. Esse resultado e a origem contextual do token não foram convertidos em PASS. Nenhum processo do fixture permaneceu na consulta posterior.

Revisão de fonte e testes locais concluída; aprovação exata operacional permanece pendente. Sem preflight, Start/Stop, SCM/ACL, ETW ou rede. Nenhuma alegação de revisão por executor externo foi fabricada.
