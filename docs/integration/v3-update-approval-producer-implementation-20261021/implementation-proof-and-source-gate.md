# Produtor UPDATE — captura nativa implementada; status factual pendente

U0-U7 accepted permitem a captura explícita no mesmo owner. Nenhuma decisão nova foi aceita ou formato persistente alterado. A entrada privada configuration_v3_update_approve estende o produtor canônico existente; não cria comando CLI de sucesso, segundo produtor/resolver ou fonte de confiança. A CLI atual continua validate offline/apply negado; não constrói CURRENT para esta operação.

## Fluxo e proveniência

O guard canônico existente comprova singleton/thread, operação/cohort HELD, invocação, source PUBLISHED e seu borrower real, ticket/serial/view R/A/P/I e CURRENT antes da captura. A função exige a imagem completa selecionada pelo caller, disjunta do owner, com tamanho e comparação byte-exata contra o candidato G7. A chamada específica é o ato solicitado U2, não uma flag em config, digest isolado ou startup; a validação posterior de todas as tuplas ainda é obrigatória.

A captura nativa recusa impersonation, token não primário, sem elevação, limitado/filtrado, Administrators desabilitado, sessão não interativa/local-console, contexto service ou erros de consulta. O subconjunto inicial é conservador: sessão do processo deve coincidir com a active-console session, grupo Interactive habilitado e Service ausente; acesso remoto/session0 fica negado. Não altera token/grupo/ACL/privilégio, não eleva nem usa token de outro processo. Usa consultas nativas de token, não prova/custódia bootstrap.

SID e TokenId/AuthenticationId/ModifiedId são capturados e comparados novamente; elevação/grupos/contexto também são revalidados. A mesma prova pertence ao cohort304 (80 anteriores +224), sem pointer caller, certificado externo, pin novo ou epoch. Hashes de transferência da imagem completa candidata e predecessor são somente correlação: mantêm vínculo com todos os bytes SNT/PDG/CA/policy/CP zero/baseCP já fixados, mas não aprovam os dados por si. Committed/previous e slot corrente continuam byte-exatos. O candidato continua serial0 para borrow, sem pin fictício.

A prova pode ficar CAPTURED enquanto status=SOURCE_ABSENT. Esse estado NÃO é RESOLVED/VALIDATED/TRUST/COMMITTED. O wrapper só retorna NOT_SUPPORTED após captura bem-sucedida e nova validação CURRENT/source. Sem CURRENT factual nem sequer captura. validate exige revalidação do ato e termina NOT_SUPPORTED enquanto faltar status/UTC. commit reutiliza o mesmo gate e não alcança packet11. Falha CURRENT/source invalida ato capturado sem limpar recursos.

## Lifecycle e restart

Cada handle aberto pela captura síncrona tem seu fechamento tentado uma vez. Falha mantém exatamente a referência incerta no mesmo owner: dois campos independentes cobrem falhas simultâneas do token de thread e do processo. Não há retry nativo de close, timeout que libere ou ponteiro retido no stack. Abort recusa ambos handles retidos antes de unborrow/unpin; generic finish/mutators continuam bloqueados pelo cohort. A contenção permanece alcançável até disposição factual futura; não foi criado mecanismo de recuperação de handle incerto.

Stop não chama captura, hash ou consulta nativa; invalida aplicabilidade no próximo uso e mantém bytes/pins/prova. Abort só limpa após quiescência e fechamento já demonstrados. Abort limpa prova uma vez; inicialização nova em memória não ressuscita ato. Nenhuma prova é serializada em PDG/CP, previous, t.model ou source; restart não restaura aprovação/CURRENT. Resultado incerto e CAS UNKNOWN mantêm retenção.

## Gate U4 factual e handoff mínimo

Não há backend issuer/UTC aprovado e qualificado ligado ao fluxo do cohort nesta root. A porta UTC do CNG delega ao backend administrativo; não cria essa autoridade. O relógio local usado em outro contexto Site TLS não foi reaproveitado como UTC issuer. Não foram adicionados callbacks booleanos, data/hora caller ou status decodificado para preencher a lacuna.

Para concluir: vincular o backend administrativo issuer existente e sua UTC qualificada ao owner por referências protegidas e reserva factual; consultar registros/certificados/histórico aplicáveis sob CURRENT/pin e conferir tuplas SNT/PDG, perfil, validade e status separados de certificado/delegação. Comprovar propriedade de DER/arenas e acesso pré-publicação por ownership candidato, sem adaptar pin predecessor para bytes novos. Em seguida integrar a validação completa ao mesmo produtor e revalidar antes do efeito. Sem isso manter SOURCE_ABSENT/DENY; nenhum status positivo é fabricado pelo ato. CURRENT factual continua dependência externa conhecida, não inicializada nesta tarefa.

Ato sintético CAPTURED no teste é somente correlação DATA, nunca identidade/autorização nativa. Positivo administrativo operacional, operador alterado sob token elevado válido e decisões factuais UTC/status/revogação continuam não qualificados; não são mascarados pelos testes de codecs ou por esta captura. E0/M1/Browser não foram modificados; não se fabrica lease ou confirmação local.

## ED11 e validação

Probe pré-patch224 compilado; produto após integração: root324392, commit247336, cohort304, fixo Windows1948880 (+224); Legacy1790872 intacto. Três backings116616 e três imagens91776 já incluídos, sem cópia nova de SNT/PDG/CA. Chamada exige margem para input completo e allowance4096, que NÃO é limite qualificado de stack transitivo. Frames diretos medidos no censo não incluem pushes/provider/CRT/nested calls. Stack do worker65536 e scratch8192 anteriores não são duplicados. Provider/kernel/stack transitivo UNKNOWN; ED11 totalPARTIAL.

Cenário completo conservador3747825 excede3MiB em602097 e não é pico/reserva; integração dependente/ativação continuam negadas. Captação síncrona isolada não autoriza coexistência de runtime/Sessions ativas nem composição completa. Windows3MiB somente opt-in, default/Legacy2MiB e quotas individuais inalterados.

Quatro builds finais sem warnings;74 suítes finais (21+21/16+16), cohort127 checks por configuração após ampliação,24 casos nativos locais históricos/worker. Query token atual negada; positivo DATA e falha de close MODEL separados da I/O TxF nativa com contexto administrativo sintético. Nenhuma instalação/current/assinatura, chave/certificado novo ou rede. Testes de status/UTC positivos pendentes porque a fonte não está integrada.
