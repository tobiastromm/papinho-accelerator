# TPM device binding / custódia / NV — closeout integrado

TASK=PACC-PHASE6-ED11-TPM-DEVICE-BINDING-AND-NV-READINESS; 2026-10-28.

## Identidade reconciliada

A prova matemática nativa bem-sucedida não foi repetida. Reconciliamos o pedido byte-exato, certificado, EK pública e core qualificado. Reextração pública offline com OpenSSL pinado confirmou SPKI idêntico, sem consulta TPM/rede privada/trust-store. PKCS1 SHA406874753bd60db238226c888765b49940cc6f0229b8a0296f4ad1547e92aba2; SPKI1c643066b17bdf9e81c09870661aeb7c9cfff1f2ffbef71b43532c14a25c474e; certificado965f56aa0d4c5b5772b423d65d8a06a623109f5828c0eddcb8ec4ae8fa3de761. Reutilizamos a verificação anterior das assinaturas Intel; não a confundimos com designação da âncora. CHAIN-ANALYSIS anterior declara DOWNLOADED_CANDIDATE_NOT_DESIGNATED.

A sessão salted unbound HMAC usou a chave correspondente, challenge/nonce frescos e revalidação pública antes/depois segundo o core byte-exato. A saída registra Name000b46b6236cd5e80474e064d24deea3db9c71d4cdb0d3952e419789389f5f89aa80, prooftrue, término autenticado e contexto fechado. Não foram exportados salt/nonces/challenge privado; não existe revalidação criptográfica independente da sessão a partir de transcript público. O julgamento decorre do executável/source pinados e recibo factual, não de um novo replay.

Identidade pública+posse endpoint estão demonstradas no alcance observado. Não demonstram geração física original, ausência de relay/compromisso do host/firmware, continuidade NV após Clear/redefinição ou armazenamento independente de snapshot. Mesmo EK/Name não prova mesma geração de provisionamento. Metadados INTC/firmware/boot/computer não identificam autenticamente hardware. W2/W4 exigem a designação e prova da fronteira não restaurável; o sucesso matemático não as dispensa. Revogação da EK continua UNKNOWN, CRL do dispositivo403 histórico; não houve nova consulta redundante.

## Custódia concreta, ainda não designada

Definir: cerimônia específica com autoridade legítima OWNER comprovadamente disponível; administrador Windows/TBS não é ownerAuth. Sem extração de registry, tentativa vazia, adivinhação, mudança de GPO/hierarquia ou retry. A documentação Microsoft informa que versões modernas podem descartar ownerAuth após provisionamento; isso é risco documentado, não uma observação de que ESTE equipamento descartou o segredo. Disponibilidade local permanece NOT_DEMONSTRATED. Se não houver origem legítima, STOP: não existe código que fabrique essa autoridade.

Incrementar/ler8bytes: somente owner/worker canônico sob lease original, manifestos congelados root/predecessor/device-generation/index/custódia, token atual e sessão HMAC autenticada; AUTHWRITE/AUTHREAD vinculam conhecimento do segredo, não isolamento TPM por processo. Secret32bytes só numa futura cerimônia aprovada, nunca gerado nesta tarefa. DPAPI CurrentUser em backing ciphertext do MESMO participante L1, criação exclusiva/RAW1/native IDs protegidos; entropy versionada propósito/root/device/index/public-profile/custodian. Nenhum segredo em argv/log/evidência, nenhum owner extra. Leitura pública não concede leitura autenticada do contador.

Observado operador SID...1108 não vira custodiante por inferência. Designação deve nomear o SID e aceitar explicitamente a fronteira same-user/admin: DPAPI não impede software autorizado como o mesmo usuário/administrador de obter o segredo, nem rollback do ciphertext. Se esse risco não for aceitável, é necessária proposta de política de autorização mais forte, não um claim fictício de isolamento. Nenhum segredo real, ciphertext operacional ou objeto protegido foi criado.

Reinício: verificar identidade/generation/manifests e recuperar ciphertext CurrentUser apenas sob autoridade válida; não retomar aprovação/lease consumida. Perda de perfil/credencial, substituição/Clear, índice ausente/redefinido, fechamento incerto: DENY/UNKNOWN, originais/dependências/reservas retidos, sem adoção/recriação/cleanup/refund. Auth errors reais podem causar DA-lockout: sem fault injection no hardware. Custódia efetiva e recuperação real permanecem não demonstradas.

## L1 e ledger

W/L/L1 accepted e respectivas propostas exatas conferidas. As oito fontes relevantes de carrier5/worker14/testes conferem com FINAL-ARTIFACT-HASHES anterior; formatos/CAS nativos sobre arquivos sintéticos continuam qualificados. Codec512, same-root DATA/callbacks não são autoridade factual. Não repetimos quatro builds,69241 checks/config v5,97663 v4,20 worker-v5,10 worker-v4 e528 binding sem mudança. Dados e scopes estão no closeout L1, sem somá-los como prova de hardware.

Ordem obrigatória: lease/manifestos/original predecessor -> CAS PREPARED durável -> autenticar witness/baseline -> UM incremento -> verificar expected -> CAS CONSUMED durável -> guard fresco -> leitura única na MESMA invocação -> RESULT/UNKNOWN. NV incremento não é CAS; todo writer autorizado deve estar sob exclusão governada. Crash nos gaps ou resposta perdida consome/nega, nunca resend/rebaseline/refund. Reboot com CONSUMED não autoriza continuação. Rollback só detectável contra hardware independente da MESMA geração; simular essa independência não é demonstrá-la. Não há persistência real ledger+NV/reboot/rollback demonstrada.

## Adapter e trabalho novo

Perfil candidato mantido: índice0x01804143, não reservado; SHA256, contador8bytes, attrs0x00040014=AUTHWRITE|COUNTER|AUTHREAD, authPolicy vazio, ORDERLY/NO_DA claros. Uma definição e até2incrementos candidatos (inicialização observada, depois consumo sintético baseline+1). Nenhum baseline0/1 presumido. Ausência histórica do índice não é reserva nem precondição fresca.

Adicionado nv_profile_data.py, isolado DATA, sem imports TBS/native/secret/root. Valida bytes públicos exatos e Name SHA256 por fase. WRITTEN altera o perfil para0x20040014 e portanto o Name após inicialização. Não mascarar flags genericamente nem usar Name pré-incremento no próximo cpHash. Isso é requisito de integração futura, não defeito operacional observado: o codec anterior recebe Name como parâmetro e ainda não há sender NV ativo.

Implementada também serialização isolada NV_Read 8bytes/offset0 com autenticação HMAC e validação estrita de resposta. Só sessões/segredos SINTÉTICOS fornecidos pelo teste, sem criação/recuperação de segredo real. Pacote99bytes, resposta93bytes; nenhum transporte. Define/encrypted-auth/Increment/TPM_RC e faults anteriores reutilizados como evidência mock, não qualificação de envio nativo. Sessão GetRandom comprovada não qualifica automaticamente autorização OWNER/index, parâmetro cifrado de DefineSpace ou canal NV.

12 métodos PASS, log DATA-TESTS:224 mutações de bits dos perfis com Name recalculado,34 alterações de Name, truncamentos, fase WRITTEN incorreta, overflow/delta/rebase,744mutações de bits da resposta e93truncamentos, nonce/secret/key incorretos, handle de sessão inválido, divergência Name e dados de prontidão UNKNOWN. Nenhum desses testes afirma TPM_RC real, reboot nativo ou persistência. Python apenas; não houve alteração C/C#/produto nem build desnecessário. O teste positivo com todos os flags sintéticos continua authorityDENY: validador DATA não é admissão.

## Pedido e decisões reais

NV-PROVISIONING-READINESS.NOT-EXECUTABLE.json atualiza candidato histórico sem sobrescrevê-lo. Não é pedido pronto para aprovação de execução; commandnull/approvalfalse. Falta:
1. Designação explícita da âncora Intel pinada e regra admissível de revogação; UNKNOWN não significa não revogado. Não existe exceção W2 implícita.
2. Binding autenticado ao dispositivo e fronteira não restaurável/generation com critérios para Clear/replace/redefine.
3. Custodiante designado, origem legítima ownerAuth e aceitação da fronteira CurrentUser/admin; prova de backing protegido e recuperação.
4. Integração/qualificação do sender NV autenticado sob mesmo owner, manifestos factuais, recursos frescos e lifecycle; novos efeitos requererão pedido próprio.

Decisão de confiança/custódia não é autorização NV. Se as propriedades não puderem ser satisfeitas, conservar DENY ou apresentar alternativa arquitetural explícita; não mudar W/L por interpretação. Nenhum comando de provisionamento foi fabricado. Não é suficiente pedir apenas autorização para escrever no TPM.

## Caminho ED11 paralelo

E17: R0–R9 aceito/offline, faltam identidade/acesso efetivo/readiness reais numa futura operação independente elegível. E18: Stop solicitado/aceito/concluído, lifecycle e quiescência factuais, sem inferência de processo encerrado. E19: D2 preparação-only; D4/D5 quantitativos e headroom ainda sem decisão justificada. E20: uma operação Browser/C2/C3/provider com bindings root/CURRENT/epoch/SiteTLS/backend/destino; provider/kernel ativos UNKNOWN. As fontes/reports dessas frentes são reutilizadas, não substituídas por TPM ou reserva Browser. Não surgiu divergência técnica nova nessas frentes que autorize valores inventados ou repetir campanhas.

E21 continua PENDING no alcance acima; E22 derivado. ED11/Phase6 PARTIAL separados; UNKNOWN/reservas preservados. HEAD f341985924d29870d8ff627ed37c8a0874ad6469; sem commit/clone/worktree. Somente novos dados/testes/documentação isolados; históricos, aprovação consumida, binários e pins operacionais intactos. TPM calls0, NV/Q4/Start/Stop/SCM/ACL/CURRENT/assinatura/recuperação NONE.

## Fontes primárias consultadas

- Microsoft TPM reference TPMA_NV/TPMS_NV_PUBLIC: https://raw.githubusercontent.com/microsoft/ms-tpm-20-ref/main/TPMCmd/tpm/include/public/TpmTypes.h
- Microsoft TPM reference NV_ReadPublic/NV_Read: https://raw.githubusercontent.com/microsoft/ms-tpm-20-ref/main/TPMCmd/tpm/src/command/NVStorage/NV_ReadPublic.c e https://raw.githubusercontent.com/microsoft/ms-tpm-20-ref/main/TPMCmd/tpm/src/command/NVStorage/NV_Read.c
- Windows ownerAuth: https://learn.microsoft.com/en-us/windows/security/hardware-security/tpm/change-the-tpm-owner-password
- DPAPI CurrentUser: https://learn.microsoft.com/en-us/windows/win32/api/dpapi/nf-dpapi-cryptprotectdata

Fontes explicam semântica; não comprovam configuração local ou designam confiança. Hashes finais e arquivos novos em SHA256SUMS.json.
