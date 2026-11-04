# TPM trust/custody/generation — integração técnica e decisões reais

TASK=PACC-PHASE6-ED11-TPM-TRUST-CUSTODY-RESOLUTION.2026-10-28; canonical main/HEAD f341985924d29870d8ff627ed37c8a0874ad6469. Nenhuma execução TPM/Q4/serviço iniciada. Autoridades produtivas DENY.

## Resultado

Concluída implementação C isolada do adapter não mutante sobre o contrato de owner/transport nativo existente, com Debug/Release zero warnings e1929verificações por configuração. Nova proposta normativa focal torna independentes confiança/revogação, custódia, geração/índice e posterior operação. Nenhuma decisão foi registrada como accepted. Provisionamento ainda NOT_READY: decisão do proprietário sozinha não fornece ownerAuth nem prova fronteira física.

## Contrato de confiança e primeira decisão

Revisados W/L/L1 accepted e seus originais, C0 accepted e os relatórios EK/nativo anteriores. W2 exige identidade/custódia/limites/autorização reais antes de mutação; W4 exige MESMA geração independente e nega ambiguidade/reset/redefinição. Não há designação local de trust anchor já registrada para este witness nem autorização accepted para converter CRL403/UNKNOWN em confiança NV/Q4. A exceção explícita para prova MATEMÁTICA com revogação UNKNOWN no pedido consumido não se transfere ao witness.

Âncora candidata Intel DER2e1b3ba79af56d758be51697621bc4b9e8cee0983db3e749c55eb9b37c6d2ae0, chave/certificado e cadeia pinados. A propostaTC1 descreve uma designação LOCAL por propósito, preservando bytes/histórico e sem instalação no trust store. TC2 recomenda perfil estrito para Q4; alternativa de risco é somente diagnóstico delimitado com UNKNOWN preservado e decisão explícita, não uma conclusão de não revogação. Nenhum limite quantitativo/prazo operacional foi inventado.

Invalidação: mudança de EK/certificado/chain/policy/algoritmo, invalidade temporal, revogação positiva, identidade/generation/firmware/host fora do perfil, perda do índice/credencial/linhagem, consulta/fechamento incertos => DENY e retenção. Histórico nunca sobrescrito para parecer confiança contínua.

## Vínculo à geração e rollback

Identidade EK e salted session demonstram posse endpoint no alcance observado; não identificam sozinhas geração de provisionamento. Name vincula perfil público, não criação original. TBS interface3/manufacturer/firmware/computer/boot são metadados, não prova de TPM físico independente de snapshot.

O binding proposto inclui EK/chain designadas, origem/autorização do equipamento, fronteira TPM/firmware não restaurada com o disco explicitamente evidenciada, root/predecessor/custódia/purpose/approval, criação original do índice e primeira inicialização observada, public area/Name por fase e baseline real. Não há identificação fictícia de geração ou Name futuro. Public profile é0x00040014 antes e0x20040014 após WRITTEN; digest refeito é exigido, não mascarado genericamente.

Restauração do ledger antigo deve divergir da testemunha avançada da MESMA geração. Mudança de EK, índice ausente/redefinido, credential drift, baseline inesperado, reset/Clear ou continuidade ambígua nega; não rebaseline/adopt/recreate. Se toda a testemunha for restaurada junto com o disco, não existe detecção garantida por esse mecanismo: os modelos anteriores demonstram essa limitação. TC6 torna explícita a confiança no hardware/firmware/host e nos custodiantes legítimos; não a transforma em fato nem aplica M3. Ainda faltam prova da fronteira e ensaio NV/persistência/reinício, que exigem autorização própria. Sem resistência real a rollback declarada.

## Custódia concreta proposta

Custodiante candidato SID...1108 só depois de designação; operador observado não vira owner por inferência. Definição depende da fonte legítima de TPM_RH_OWNER auth, com entrega controlada em memória, sem valor em chat/log/argv/ENV. TBS/admin/lockoutAuth não substituem ownerAuth. Microsoft documenta que Windows moderno pode não guardar ownerAuth após provisionamento; não consultamos segredos, registry ou GPO e não inferimos a configuração local. Se não houver origem legítima, STOP; não há correção de software que crie essa autoridade dentro do escopo.

Incremento/leitura AUTHWRITE/AUTHREAD pertencem ao owner/worker canônico, com lease e manifestos originais/autenticação de sessão/token/identidade. Per-index secret32bytes gerado somente depois de futura autorização específica, protegido por backing CurrentUser DPAPI do MESMO participante L1/RAW1/IDs/descriptor e entropy versionada de root/device/index/purpose/custodian. Nenhum segredo operacional criado. DPAPI e AUTHWRITE não fornecem isolamento contra outro software same-user/admin; TC4 exige aceitação dessa fronteira ou proposta mais forte, sem novo vault/owner/serviço por inferência.

Reinício recupera disponibilidade de segredo sob perfil/identidade legítimos, não recupera aprovação consumida ou live lease. Perda de perfil/master/TPM/index ou close incerto: retain/UNKNOWN e DENY, sem recriação/refund. Revogação documental bloqueia novas leases, mas não apaga credencial já conhecida; revogação criptográfica/rotação exigiria outro desenho e efeitos aprovados. Auditoria contém hashes/identidades/purpose/first-stage/TPM_RC/close/resultado, nunca secret. DA-lockout continua risco: nenhum teste de auth errada real ou retry.

## Adapter C implementado e alcance

Arquivos nv_adapter.c/.h recebem TP_OWNER original por borrow; não criam owner/contexto/serviço, não abrem/fecham TBS e não adquirem handles persistentes. Reutilizam as funções CNG/core pinadas existentes; módulo permanece no pacote isolado, NÃO foi ligado ao executável consumido ou à rota operacional. Não amplia aquele pedido ou seus pins.

q4_tbs_public_only tem a assinatura Windows TBS e aceita apenas14bytes exatos de NV_ReadPublic0x169 para0x01804143. Define/Increment/Undefine e demais comandos são negados antes de tocar no callback, inclusive bytes alterados. Não carrega tbs.dll; depende do ponteiro original que um futuro owner qualificado fornecerá. ABI compilada/dispatch mock validada, TBS real não invocado.

q4_nv_public exige owner thread/BUSY, contexto/transport/boundary originais e guards antes/depois, Stop/freeze/limite de chamadas, parsing de status/TBS/TPM_RC/tamanho e perfil/Name exatos com SHA256 CNG real. Public-area read é não autenticada em si: não confundir Name/checksum com dispositivo autenticado. Ausência0x8b/collision/auth/rate/result incerto => primeira falha, sem retry/adoption.

Codec nativo NV_Read99bytes e resposta93bytes usa SHA256/HMAC CNG reais, key=saltsession+auth trimmed, cpHash/rpHash, Name/nonce e8bytes offset0; equivalente byte-exato ao vetor Python independente já qualificado. Nenhum NV_Read autenticado foi enviado. Valores/segredos de teste são padrões públicos sintéticos; não há operacional secret/session establishment. Builders não concedem autorização e recebem buffers válidos/disjuntos do caller; aplicação factual e manifestos continuam responsabilidade do owner. O adapter não contém sender de escrita habilitado. DefineSpace/Increment de preparação anteriores permanecem evidência de codec/mock, não permissão de escrita ou prova de canal real.

## Testes e builds

MSVC x64 atual /std:c11 /W4 /WX /GS /sdl, Debug e Release PASS, zero warnings finais.1929checks/config,3858execuções de checks sobre o mesmo conjunto, NÃO3858propriedades distintas. Evidência NATIVE-TEST-RESULTS.json e logs por configuração.

Cobertura: perfis antes/depoisWRITTEN e fase incorreta,62mutações do envelope público,744mutações de bits da resposta HMAC e93truncamentos, match de pacote com vetor independente, TPM_RC0x98e/0x922 e TBS erro preservados, Stop antes/depois, reentry freeze, thread estrangeira real rejeitada sem mutação, dispatch-only allowlist, classe de comando mutante negada, fechamento CNG real limpo e close UNKNOWN simulado. Owner fica BUSY ao longo do uso; nenhuma nova consulta após freeze, primeiro erro preservado.

No caminho UNKNOWN, um contexto SINTÉTICO e seus providers CNG são conservados até saída do fixture. Isso demonstra decisão de retenção no processo observado, não fechamento global, custódia operacional ou retenção pós-morte. Não abrimos contexto TPM. Tests não autenticam hardware, validam TBS ABI por callback mock e não medem recursos NV reais.

Falhas iniciais de build preservadas: quoting de /Fo no script; nome de constante SDK; cast explícito de HRESULT para TBS_RESULT. Corrigidas sem desativar warnings. Adição relevante do testeWRITTEN e do reciboTPM_RC motivou rebuilds afetados; nenhuma campanha produto/Browser/core crypto inalterada foi repetida. Fontes/pins históricos consumidos permanecem iguais. Não foram criados launcher ou comando de provisionamento.

## Decisões independentes e pedido

OWNER-DECISION-PACKAGE.NOT-APPROVAL.json referencia o SHA da proposta. A: designação de âncora e escolha explícita da regra de revogação. B: designação de custodiante e fronteira de proteção, mais comprovação de origem legítima de ownerAuth sem revelar valor. C: evidência/designação da fronteira não restaurável e lifecycle de geração/índice. D: pedido operacional separado somente após requisitos completos. Todos owner_decisionnull; não registrar ACCEPTED por esta tarefa.

Nenhum pedido de provisionamento pronto: source de ownerAuth, custódia protegida original, manifesto de generation/boundary/authority e recursos frescos não existem factualmente. Preparar comando neste estado fabricaria pré-condições. Adapter não mutante está qualificado no alcance acima; sender NV autenticado/fechamento/recursos reais ainda terá de ser integrado e qualificado sob o perfil explicitamente escolhido antes do pedido. Se ownerAuth não puder ser legitimamente disponibilizado, a menor decisão real é manter DENY ou revisar a arquitetura de witness, não repetir provas de posse.

## Ledger e ED11 paralelo

L1 carrier5/component512/worker14 e CAS/persistência sobre arquivos sintéticos permanecem qualificados. PREPAREDdurável -> UM incremento -> expected -> CONSUMEDdurável -> guard fresco -> mesma live invocation -> RESULT/UNKNOWN. Crash/TPM_RCambíguo/Stop/replay não geram retomada/retry. Fonte de autoridade factual/manifestos não existe por chamar callback positivo; produção root/publicação não executadas.

E17: R0-R9 norma/offline sem identidade/acesso/readiness reais; E18 exige Stop/lifecycle/quiescência factual. E19 ainda depende D4/D5 quantitativos fundamentados; E20 requer bindings root/CURRENT/epoch/SiteTLS/backend/destino e composição de uma operação com provider/kernel ativos UNKNOWN. Reaproveitados planos/fontes existentes, sem atribuir ceilings/valores fictícios ou reexecutar campanhas equivalentes. Esses gates não são fechados por TPM nem devem esperar seu provisionamento para decisões de target policy. E21 PENDING; E22 derivado;4demonstrados/12limitados/6pendentes; ED11/Phase6 PARTIAL.

## Fontes primárias

- Windows ownerAuth: https://learn.microsoft.com/en-us/windows/security/hardware-security/tpm/change-the-tpm-owner-password
- ABI TBS e códigos/TPM_RC: https://learn.microsoft.com/en-us/windows/win32/api/tbs/nf-tbs-tbsip_submit_command
- Semântica inicialização/WRITTEN/incremento: https://raw.githubusercontent.com/microsoft/ms-tpm-20-ref/main/TPMCmd/tpm/src/command/NVStorage/NV_Increment.c

Fontes não são observações do estado local. Nenhum Clear/hierarchy/registry/GPO/SCM/ACL/NV/Q4/Start/Stop/cleanup/refund/CURRENT/signature/recovery. UNKNOWN/reservas/HEAD/WIP preservados; alterações somente novos candidatos/fixtures/evidências/live docs. Hashes finais em SHA256SUMS.json.
