# ED11 — posse TPM, custódia, root e caminho paralelo

TASK=PACC-PHASE6-ED11-TPM-POSSESSION-CUSTODY-AND-ROOT-INTEGRATION

Avanço factual: revalidação funcional no owner canônico implementada e quatro builds qualificados; prova de posse com desafio fresco e concorrência qualificada em software; diagnóstico oficial de revogação concluído no alcance disponível; matriz E19/E20 corrigida quanto à aceitação D2. Posse NATIVA, custódia efetiva e publicação persistente integrada continuam NÃO DEMONSTRADAS. Não houve TPM/Q4/LAB/Start/Stop ou concessão de autoridade. ED11/Phase6 PARTIAL; E21 PENDING.

## Prova de posse

Reutilizado o núcleo criptográfico já qualificado728 verificações, sem repetir essa campanha. Novo PossessionOnceFixture implementa desafio30 bytes via RNG Windows, contexto/EK/AK Name imutáveis, idade aprovada como argumento (nenhum limite de política inventado), rejeição de clock regressivo, mismatch, Stop, fechamento, replay e resposta cruzada de outro desafio. Lock do fixture serializa16 verificadores: um único vencedor; não adiciona owner ao produto. Clear desativa a instância, não permite aceitar um desafio zerado. Defeito potencial de concorrência do fixture inicial corrigido antes do fechamento; logs anteriores preservados como BEFORE-CONCURRENCY.

Debug/Release:23 verificações por build,46 total, zero warnings. Chaves são RSA efêmeras SOFTWARE do fixture; EK/contexto de testes são sintéticos. Recuperação de credencial é simulada pela chave de software, não uma resposta TPM autenticada. Isso não prova posse da EK real, independência física, geração NV ou autorização de sessão. Não há novo sender TBS.

NATIVE-POSSESSION-REQUEST.CANDIDATE.json vincula os hashes reais EK/certificado/SPKI/root e propõe uma única sequência limitada de credential activation, sem NV, assinatura, CA remota ou chave persistente. Propõe até um EK primary e um AK primary transitórios, sujeitos a perfil exato/sessões/custódia/revisão de fechamento e aprovação própria. Template/AK Name/autorização reais não foram inventados. Candidato NÃO EXECUTÁVEL, sem aprovação ou nova identidade operacional; comandos TPM adicionais NÃO EXECUTADOS.

## Revogação

A consulta diagnóstica oficial confirmou HTTP403 AmazonS3 AccessDenied/CloudFront. A resposta não permite distinguir objeto ausente de configuração de acesso; isso continua UNKNOWN. Não houve contorno de bloqueio. A política oficial Intel citada no próprio certificado foi obtida: seção3.3.1.2 estabelece CRL e ausência de OCSP para essa CA;6.9.9/6.9.11 não estabelecem alternativa. Nenhum endpoint oficial equivalente foi demonstrado. APIs SGX/TDX PCK ou provisionamento AK de uma CA remota não substituem a CRL TPM CML.

Assinaturas da cadeia e correspondência EK/certificado mantêm o PASS limitado anterior. Revogação completa permanece UNKNOWN. [Revisão e fontes](REVOCATION-REVIEW.md), hashes/headers/body em REVOCATION-DIAGNOSTIC.json. O CPS é documento público, não permissão para relaxar W2/W4. Se a evidência não puder ser obtida, uma decisão explícita e estreita de risco de confiança/revogação seria necessária; nenhuma exceção foi inferida.

## Integração canônica realizada

Nova função papacc_configuration_q4_binding_current usa o owner e os guards existentes para recapturar e comparar o snapshot DATA inteiro no ponto de uso. Rejeita qualquer byte divergente, Stop, estado incerto, alteração de revision/ticket, owner/thread e aliases owner/pin; não permite transformar snapshot antigo em autorização. Não cria serviço, owner, mutex, registro ou quota.

132 verificações por configuração,528 total, incluindo88 posições de snapshot adulteradas e guards pós-captura. Quatro builds produto Selected/Legacy Debug/Release PASS, zero warnings. Estruturas owner preservadas: Selected263584/Legacy98088;88 bytes temporários locais não são custo total de stack/RAM ou crédito. Nenhum handle estável novo. Patch focal e preimagens preservados. Compilados candidatos atualizados e hashes calculados; pins operacionais históricos não ativados/alterados.

A nova função é um precondicionador DATA chamado pelos testes; NÃO está ligada a um ciclo TPM positivo ou a publicação persistente. Não é CAS nem atomicidade através da operação externa seguinte. O caller continua obrigado a sequenciar pelo mesmo owner/worker e a revalidar a cada fronteira.

A parte durável permanece no worker isolado já qualificado: prepare persistente -> testemunha simulada -> consumed persistente -> lease/read slot -> result/UNKNOWN, sem retry. Não repetimos14 testes/134 verificações de adapter inalterados. L1 exige revisão exata de schema/participante/worker antes de publicar nova extensão: ROOT8/issuer não possuem esse campo. Nenhum byte reservado foi reutilizado e nenhum arquivo externo tornou-se autoridade. A integração PERSISTENTE no produto continua NOT_DEMONSTRATED; os testes não equivalem a hardware monotônico ou crash de energia. [Revisão de custódia e root](CUSTODY-AND-ROOT-REVIEW.md).

## Custódia

Consolidado quem pode definir/incrementar/consultar, proteção e limites de DPAPI CurrentUser, binding root/device/index/custodian/purpose, reinício, perda, lockout, concorrência e UNKNOWN. Autorizações OWNER/AK/sessão e segredo real do índice ainda não estão disponíveis como evidência. Admin/TBS não é autorização; nenhum ownerAuth foi extraído/adivinhado nem segredo criado. DPAPI não protege contra software no mesmo usuário/admin e não é testemunha antirollback. Não se recupera disponibilidade recriando índice/TPM ou rebaseline. Nenhuma custódia foi concedida por observação do SID.

## E17–E20 sem esperar o TPM

- E17: R0–R9 accepted/offline permanece válido. A próxima prova é serviço real independente: TokenUser/grupos/restrições completos, acesso efetivo, módulos, readiness e recursos. Não repetir a operação consumida nem a observação do SID ausente.
- E18: o mesmo futuro ciclo elegível pode provar Stop solicitado/aceito/concluído, término e fechamento. Processo que terminou não prova quiescência global. Nova identidade/intent/efeitos só após autoridade própria; nenhum foi criado.
- E19: snapshots/validadores já implementados permanecem aproveitáveis. D2 foi ACEITA apenas para preparação; a matriz dizia incorretamente que essa escolha ainda faltava. Corrigido o estado documental sem promover D0–D9 inteira. D4/D5 continuam sem justificativa de idade/validade/duração/headroom alvo; não copiamos valores LAB.
- E20: uma operação Browser/C2/C3 NSS/x86 vs OpenSSL/x64 continua como escopo candidato. Reserva fixa Browser1,858,020 cabe sozinha em2,097,152; não prova envelope provider/kernel ou recursos compartilhados. Faltam política e bindings reais root/CURRENT/epoch/SiteTLS/destino/módulos/host, depois uma execução correlacionada autorizada.

Essas frentes não dependem criptograficamente do TPM para preparação/qualificação; admissão futura ainda precisa contabilizar as reservas históricas e passar os gates frescos, sem refund. Browser main/HEAD96f6cd2f9ec4f834999828fa13bb7d170db66a98 localizado e apenas consultado; nenhum checkout ou alteração. [Caminho crítico estruturado](ED11-PARALLEL-CRITICAL-PATH.json). Não há mais campanha equivalente de getter a repetir ou números normativos que possam ser escolhidos silenciosamente por código.

## Gates reais restantes

1. E21: confiança/revogação aplicável, autorização/custódia real, perfil e canal nativo de credential activation; depois aprovação exata antes de qualquer efeito transitório TPM. Prova de posse NÃO é provisionamento nem persistência.
2. L1: decisão/revisão do formato exato e participante persistente; ligação ao CAS original com objeto/RAW1 nativos. O helper novo prepara o guard real, não autoriza publicar schema proposto.
3. Somente com identidade/custódia/adapter e recursos demonstrados, finalizar provisionamento específico;0x01804143/0x00040014/8 bytes/max2 incrementos continuam candidatos não reservados. Sem pedido executável fictício.
4. E17/E18: operação de serviço independente exata, após condições/admissão próprias; nenhum retry de Q4. E19/E20: decisão quantitativa D4/D5 e bindings do alvo antes da execução única correlacionada.
5. Após testemunha real válida, leitura Q4 e disposição documental independentes; E22 depende dos demais gates aplicáveis.

## Evidência e hashes

[BUILD-TEST-RESULTS.json](BUILD-TEST-RESULTS.json), logs de testes, [patch focal](FOCUSED-SOURCE-PATCH.diff), [hashes de fontes e candidatos](FINAL-SOURCE-AND-CANDIDATE-HASHES.json), [verificação normativa](AUTHORITY-HASH-VERIFICATION.json). Builds produto comprovados pelos resultados das quatro execuções nesta sessão; os logs locais de testes e scripts reproduzíveis foram preservados. Nenhuma campanha anterior é contada como prova nova.

- server_configuration_q4_data_win32.inc: `b5408fc9025cb8ac28351d79fdded15d4c0dfc13b9fa99f805b6c2200ea6ecfa`
- server_configuration_commit_win32.h: `705509c9eaf57ff4a76cc846cf47ba3349463ecfaf84785d725f60d758c4d811`
- q4_root_binding_data_test.c: `a075c40d981cc22a546a2e2b2c478c59c13d415538cbb03692c8a3485a4bd93e`

HEAD/main preservado; WIP e registros históricos mantidos. Nenhuma mutação NV, Clear, Q4, Start/Stop, SCM/ACL, limpeza/refund, CURRENT, assinatura produtiva ou recuperação autoritativa. Nenhuma aprovação emitida pelo agente. PREPARATION=ADVANCED_PARTIAL; NATIVE_POSSESSION=NOT_DEMONSTRATED; CUSTODY_EFFECTIVE=NOT_DEMONSTRATED; ROOT_PERSISTENT_INTEGRATION=NOT_DEMONSTRATED; E21=PENDING; ED11/Phase6=PARTIAL; autoridades DENY.
