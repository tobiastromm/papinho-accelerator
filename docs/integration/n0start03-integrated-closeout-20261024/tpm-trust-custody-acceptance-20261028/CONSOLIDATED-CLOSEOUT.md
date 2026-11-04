# TC0–TC9 — aceitação e integração técnica

TASK=PACC-PHASE6-ED11-TPM-TRUST-CUSTODY-ACCEPTANCE;2026-10-28. Canonical main/HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservado.

## Decisão registrada antes do código

Proposta integral133d0cdc74458259cfaed9d9a6e28504f090ee18ea41ba24fd0e50395b364845 conferida contra bytes originais, junto aos contratos W/L/L1/cross-boot. [Registro ACCEPTED](../../../adr/DECISION-q4-tpm-trust-custody-generation-20261028.md), SHA17636e994d53b449b55e94edf476cf29c0dc22d1737cc773cde3712854b2a23f, criado antes das alterações. Original proposta e decisões accepted anteriores intactos.

Aceitação do framework e obrigações fail-closed, não escolha implícita das designações condicionaisTC9. Âncora local ainda NOT_GRANTED; exceção diagnóstica UNKNOWN não foi escolhida/bindada e segue NOT_GRANTED; custodiante efetivo não designado; ownerAuth e geração física não demonstrados. O próprio task distingue aceitação de designação efetiva. Perfil estrito não contradiz W2/W4/L2/L1/cross-boot. Nenhum UNKNOWN virou PASS, M3 não herdado e nenhuma nova autoridade operacional criada.

## Integração no owner canônico

Adicionado papacc_configuration_q4_tc_policy_data ao arquivo existente server_configuration_q4_data_win32.inc e header privado, com teste registrado existente. Reutiliza writer/thread, critical-section, pin/ticket e snapshot root88bytes. Não cria owner, mutex, registry, serviço, IPC, arquivo, segredo ou handle. L1 carrier5/component512/worker14, formatos/CAS e estrutura persistente do owner não mudaram.

Entrada tipada472bytes e resultado16bytes são DATA do caller, não novos campos de autoridade persistente. API revalida root/pin atuais; rejeita Stop/unresolved/pending/stale/thread estrangeira e sobreposição de output com root/pin/entrada, sem alterar output na falha. Caller mantém entradas imutáveis e buffers válidos/disjuntos; não retém ponteiros/borrows.

Perfil1 estrito somente; perfil2/exceção UNKNOWN não selecionado é rejeitado. Máscara registra doze categorias independentes: designação âncora, cadeia/validade, revogação, custodiante, ownerAuth, backing protegido, geração não restaurável, índice original, contador, persistência ledger, invocação exclusiva/replay e recursos frescos. Valores fora de VERIFIED, inclusive UNKNOWN/rejected, continuam insuficientes.

Compara fingerprints exatos EK-SPKI/certificado/root Intel e pares não zero de manifestos aprovados/atuais de geração/custódia/predecessor original. Exige índice0x01804143,8bytes, perfil0x00040014 ou0x20040014 somente na faseWRITTEN correspondente; baseline+1 sem overflow e contador atual==baseline. EK divergente, troca de certificado, geração/custódia/predecessor restaurado, índice ausente ou contador divergente não são adotados ou rebaselined.

Essas comparações são DATA, não autenticação de manifestos/native IDs/RAW1/cadeia/custódia. O resultado mantém authority_denied1 e SOURCE_MISSING SEMPRE, inclusive quando todas as declarações sintéticas dizem VERIFIED. Retorno OK significa apenas diagnóstico calculado, nunca permissão. Não há setter que instale autoridade, produtor autenticado operacional ou caminho que converta esse resultado em sender TPM/reader Q4/worker autorizado. Esse produtor factual depende das designações e evidências ausentes; criá-lo com callbacks positivos fictícios violaria os contratos. Integração completa no alcance autorizado de política/diagnóstico fail-closed, não testemunha ativa.

## Builds e testes

Quatro builds Selected/Legacy Debug/Release: papacc_server e papacc_q4_root_binding_data_test PASS, zero warnings finais, mesmo CMake/Ninja/MSVC x64 e flags /W4 /WX.494checks/config,1976execuções do mesmo conjunto, não1976propriedades independentes. Incluem os132checks anteriores. Logs em QUALIFICATION-RESULTS.json e arquivos por perfil.

Testes da função real: todas doze categorias UNKNOWN/rejected/valor inválido; mutações EK/cert/anchor e geração; custódia/predecessor divergente; contador inesperado/overflow;WRITTEN/perfil incorretos; Stop/pin stale; output sobreposto; duas threads estrangeiras reais rejeitadas. Casos de falha de persistência/replay/custódia são declarações DATA negativas, não nova prova de storage/hardware. Mesmo vetor sintético consistente mantém DENY e SOURCE_MISSING. Sem store/TPM.

Warnings iniciais preservados/corrigidos: parâmetro sombreava global do fixture; sscanf passou a sscanf_s, sem supressão. Patch script falhou inicialmente em assertion de marcador antes de escrever fonte. Nenhum contrato relaxado. Patch relativo ao WIP/preimagens preservados.

Owner Selected266656/Legacy98088bytes, binding88 inalterados. Entrada472/result16 e fingerprints read-only96bytes mais código/temporários são custos, não custo total RAM/stack nem crédito ED11. Sem arrays/quotas/handles novos. Adapter C não mutante anterior1929checks/config reutilizado após conferir fontes/vetores/binários; não repetido. Codecs v4/v5 e CAS nativo sintético anteriores permanecem evidência de formatos/worker inalterados, não persistência real ledger+TPM.

## Estado factual e pacote de custódia

Prova matemática nativa e correspondência EK/certificado permanecem PASS no alcance observado; não repetidas. Revogação UNKNOWN; root Intel candidata ainda não designada localmente. Sem nova coleta EK/rede/trust-store. Vínculo à geração física não restaurável, custódia efetiva, ownerAuth legítima e persistência NV não demonstrados.

CUSTODY-DESIGNATION.REQUEST.NOT-APPROVAL.json especifica SID candidato1108, papéis definição/incremento/leitura, propósito rootQ4, CurrentUser DPAPI no MESMO participanteL1/RAW1/native IDs/entropy de propósito/root/device/index/custodian, continuidade/revogação/UNKNOWN. Nenhum segredo/backing operacional gerado, nenhum segredo solicitado em chat/log. Fonte legítima de ownerAuth, holder e entrega controlada em memória estão null. Admin/TBS/lockoutAuth não suprem ownerAuth. Se não houver origem legítima, STOP; não há correção técnica que fabrique autoridade dentro desse escopo.

CurrentUser/admin é fronteira de confiança explícita, não isolamento por processo. Revogação documental impede novas leases governadas, mas não apaga credencial já conhecida; troca/revogação criptográfica exigiria desenho/efeitos próprios. Reinício nunca restitui uma aprovação ou live lease consumida. Perda de perfil/índice, Clear/redefinição/substituição, ambiguidade/close incerto: DENY/UNKNOWN e reservas retidas, sem recriação/refund.

EK/Name/manifests locais não provam originalidade de geração; hardware restaurado junto ao disco não é testemunha independente. Ainda faltam evidência de inventário/origem/fronteira TPM/firmware/storage e posterior experimento nativo autorizado de índice/baseline/persistência. Não declaramos resistência real a rollback de modelos/aceitação.

## Próxima decisão específica e caminho mínimo

OWNER-FACTS-AND-DECISIONS.md evita pedir aprovação genérica outra vez:
1. Decidir designação local da âncora Intel exata e regra de revogação. Perfil estrito continua ativo; exceção diagnóstica opcional requer decisão própria, não inferida.
2. Designar/rejeitar custodiante SID1108 e fronteira same-user/admin. Informar SOMENTE identidade não secreta da fonte/holder legítimo de ownerAuth, existência/disponibilidade e mecanismo de entrega em memória. NÃO enviar valor de ownerAuth/segredo em chat/log/registro.
3. Fornecer evidência da fronteira não restaurável e critérios de geração/substituição; não basta aprovar que ela existe.

A primeira informação factual crítica é se existe fonte legítima de ownerAuth. Sem ela, manter DENY ou preparar alternativa arquitetural, não repetir provas de posse. Após fatos/designações, integrar produtor autenticado/sender NV com manifestos e recursos reais e qualificar seu lifecycle; então preparar pedido exato separado, aprovação/manual provision/persistência; depois readback cross-boot e disposição Q4 independentes. Provisionamento NOT_READY; nenhum pedido/comando executável com placeholders criado.1definição/2incrementos permanecem apenas limites candidatos, sem permissão.

## Frentes paralelas e preservação

Sete fontes/testes de serviço conferidos byte-exatos; históricos/pins consumidos intactos. E17 identidade/acesso/readiness e E18 lifecycle/Stop/quiescência precisam operação futura independente elegível. E19: D2 preparação-only aceito; D4/D5 max_age/validity/duration/headroom físico/commit ainda null por ausência de seleção aplicável justificada. E20: bindings root/CURRENT/epoch/SiteTLS/backend/destino e composição real Browser/C2/C3/provider pendentes; provider/kernel UNKNOWN. Reserva gerenciada Browser1858020/2097152 não é capacidade total. Não foi identificada regressão nessas fontes que justificasse campanhas equivalentes; TPM não resolve os fatos/política do target. PARALLEL-CRITICAL-PATH.json conserva as lacunas exatas e a independência das frentes.

E21 recebe só aceitação/política DATA e continua PENDING. E22 derivado pendente; ED114demonstrados/12limitados/6pendentes, Phase6 PARTIAL separado. Rebuilds produto recebem hashes derivados neste pacote, não ativação sob aprovações históricas. Futura operação de serviço deve revisar seus próprios qualifier/source-image/pins novamente; não reutilizar pacotes consumidos. Sem TPM/NV/Q4/Start/Stop/SCM/ACL/cleanup/refund/CURRENT/signature/recovery. HEAD/WIP/UNKNOWN/reservas preservados; sem commit/push/tag/clone/worktree. CMake bytes inalterados.

Hashes finais em SHA256SUMS.json, verificação em FINAL-VERIFICATION.json, decisão/aprovação operacional NÃO criada. Próximo gate é factual/designações, não autorização imediata de escrita NV.
