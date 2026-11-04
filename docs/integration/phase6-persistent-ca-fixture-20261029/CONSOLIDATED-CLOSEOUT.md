# Primeira CA — persistência CNG isolada e reaquisição

**PERSISTENT_FIXTURE_REOPEN: PASS_AT_OBSERVATION.** A cerimônia operacional e a custódia MACHINE produtiva não estão qualificadas. NORMAL operacional DENY; Phase 6/ED11 PARTIAL.

## Implementação e resultado factual

Foi adicionada ao executável de teste uma rota explícita `--native-persistent`. Ela não é CTest automático e não está no produto. Reutiliza o corpo de CA_INITIALIZE incluído na tradução de teste, o perfil da CA, CSR, backend NORMAL, CAS/PUBLISHED v2 e o cohort M7 do ensaio anterior.

A criação usa Microsoft Software KSP real, CurrentUser, RSA2048 e nome CSPRNG exclusivo `Papacc.Qualification.FirstCA.*`, sem MACHINE_KEY, overwrite, abertura de material operacional ou exportação privada. A tentativa é registrada e flushed em receipt CREATE_NEW antes da criação. O nome não é o namespace operacional da CA. Esse receipt é evidência de teste, não ledger/autoridade nem prova anti-rollback.

Uma única execução persistente foi realizada, com o binário SHA256 `ab7b0c5dfbc9198a211bd7cab5df57d0518103f744bb97db5249d4992a72c33b`. Resultado: **85 verificações PASS**. Todas as referências iniciais da chave/provider foram fechadas após retorno dos borrows. Um novo provider abriu a mesma chave pelo nome; nome CNG, política de exportação zero e SPKI foram conferidos, incluindo correspondência com o certificado CA original em memória. Nome ausente foi rejeitado, sem fallback para criação.

A reaquisição entrou por `papacc_configuration_existing_ca_bind` e `papacc_configuration_existing_ca_activate_normal`, sem restaurar o nonce/ato NEW_INSTALL. O borrow duplicado foi negado. O backend emitiu um certificado com CNG real após a reabertura. Assinatura e PUBLISHED v2 foram verificados, e a revogação preservou a credencial não relacionada. Stop/receipts continuam distintos de fechamento/quiescência.

A chave persistente do fixture foi **retida**, com referências nativas fechadas. Não foi chamada exclusão. Seu nome e receipt estão em `PERSISTENT-OBSERVATION.json` e `ONE-ATTEMPT.receipt.txt`. Nenhuma limpeza futura fica autorizada por este relatório.

## Real versus modelado

| Propriedade | Alcance demonstrado |
|---|---|
| Criação/persistência/reabertura | CNG real, CurrentUser; mesmo processo, novo provider |
| Export policy | Valor zero consultado por NCryptGetProperty; nenhum teste de exportação privada executado |
| Nome do objeto | NCRYPT_NAME_PROPERTY real igual ao nome exclusivo retido |
| Chave/certificado | SPKI original comparado após reabertura; assinaturas reais verificadas |
| Borrow/transfer/return | APIs existentes reais e exclusividade do owner; não exclusividade global do KSP |
| Identidade/geração do perfil | Metadata do fixture; nome de qualificação CNG é explicitamente distinto do nome lógico esperado pelo perfil |
| Autoridade NEW_INSTALL/parent/declaração | Fixture; não prova de admissibilidade operacional |
| Custódia | Acesso legítimo no contexto CurrentUser observado; SID do operador não capturado no receipt original |
| Publicação/CURRENT/A/P | Codec/SHA256/ack/re-read reais em memória; publicação e autoridade simuladas |
| M7 | Digest do certificado emitido e efeitos C2 reais sobre Sessions modeladas; sem close/quiescence proof |

Não foram demonstrados reboot, restart de processo, perda/restauração do Windows, ACL MACHINE produtiva, conta operacional distinta, negativa de acesso por outro usuário, custódia durável administrativa ou resistência a rollback. Não se afirma ausência de efeitos internos opacos do KSP. Provider/kernel permanecem UNKNOWN; quotas inalteradas.

## Qualificação

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings. Três CTests diretamente afetados/configuração: 12 execuções PASS (modelo CNG, gate produtivo, composição efêmera). Os 2.718 checks do modelo/configuração foram preservados. O experimento persistente foi executado uma vez em Selected Debug, não quatro vezes nem inscrito em testes automáticos. A campanha criptográfica anterior e os fault injections inalterados são reutilizados.

Falhas de custódia, identidade/geração, concorrência, Stop, falha parcial e UNKNOWN permanecem cobertas no modelo anterior. Os negativos nativos desta composição incluem assinatura adulterada, certificado/chave incompatíveis, thread incorreto, nome indisponível, ack substituído/replay e histórico corrompido. Isso não equivale a fault injection real no armazenamento KSP persistente.

## Cerimônia e fronteira real

O fluxo NEW_INSTALL→CA_INITIALIZE→CA/custódia→NORMAL→A/P é composto no fixture. **Não foi habilitada a cerimônia produtiva.** O guard `personal_native_ceremony_ready` continua fechado e não pode ser substituído por uma flag derivada deste experimento.

A primeira lacuna técnica é o presenter nativo atribuído, CSPRNG/attempt e confirmação exata vinculados ao token/parent/source originais. Os helpers atuais de identidade/source/worker não fornecem sozinhos a apresentação e confirmação F0–F9. Sem isso não existe pedido pronto para executar a primeira instalação real. A próxima operação produtiva futura seria NEW_INSTALL/CA_INITIALIZE sob autorização específica, depois dos gates de cerimônia, custódia/identidade operacional e recursos; nenhum comando operacional é emitido aqui.

O ensaio não remove essas verificações nem exige nova decisão M0–M11. Ver `NEXT-NATIVE-FIRST-INSTALL-PRESENTER.md` para a continuação técnica concreta, sem reabrir a criptografia ou criar nova root.

## Frentes restantes

M7: composição positiva isolada preservada; autenticação/admissões e quiescência reais pendentes. E8: fonte positiva legítima/trust ainda ausente. Enrollment L2 e suas fontes permanecem pendentes. Nenhuma dessas frentes é satisfeita pela persistência da chave de teste. D4/D5/Browser seguem independentes. Não houve TPM/Q4, rede, serviço, CURRENT operacional, SCM/ACL, refund, recuperação ou mudança de PST 0.6.4.

`QUALIFICATION.json`, logs, preimages, `SOURCE-CHANGES.json`, delta e `ARTIFACT-HASHES.json` registram evidência e hashes. HEAD, WIP prévio, históricos, pins, reservas e ADRs accepted foram preservados.
