# READ encerrado — investigação focal K5

2026-11-03. OFFLINE_ONLY / NO_PROVEN_CODE_DEFECT / ISOLATION_DENY.

## Conclusão

O impedimento de lifecycle foi resolvido e a sessão encerrou controladamente. O impedimento restante é distinto: K5 exige negação NTE_PERM+handleNULL, mas a sessão alcançou a classificação inconclusiva estrita BAD_KEYSET. Não há defeito demonstrado de seleção do nome, provider, flags ou token; nenhuma correção técnica especulativa, alteração de ACL, quota, fixture ou status foi aplicada. O motivo interno da resolução negativa do Software KSP sob o runtime permanece UNKNOWN.

A auditoria anterior READ-BADKEYSET-OFFLINE-AUDIT-REPORT.md já havia delimitado essa diferença. A evidência nova é o fechamento efetivo com o resultado tipado, não outra causa KSP demonstrada. Não há justificativa para repetir sessão/UPDATE apenas para reobter BAD_KEYSET.

## Comparação dos percursos existentes

Admin: scqkm_admin_control reconstrói os dois blocos de16bytes do nome original, liga o atoREAD ao owner, abre via papacc_ca_key_fixture_read_open/ca_begin, compara wcscmp com o nome do plano e revalida identidade/proteção/publicSHA. papacc_ca_key_fixture_control reabre a MESMA chave no mesmo provider com MACHINE|SILENT, compara integralmente ambos blobs públicos e libera só a referência temporária, retendo a original. Não é criação ou adoção de nova chave.

Runtime: pcp_probe_mode valida nome87caracteres e cópia WCHAR176bytes incluindo terminador, seleçãoREAD admitida e token próprio; rejeita token de thread antes do provider e novamente antes da abertura. Usa MS_KEY_STORAGE_PROVIDER, legacyKeySpec0, NCRYPT_MACHINE_KEY_FLAG|NCRYPT_SILENT_FLAG, iguais aos usados no reopen positivo administrativo. Não impersona nem passa credencial/token administrativo ao KSP. O contexto de segurança distinto é deliberado, não defeito de binding. Reconstrução offline do nome exato da fixture PASS; nenhum KSP foi chamado.

Retorno: pcp_probe_mode preserva SECURITY_STATUS, distingue handle inesperado de NTE_PERM e NTE_BAD_KEYSET. SKR1 encode/decode correlaciona tentativa/PublicSHA e campos do probe. scqkm_return_inconclusive e controller_inconclusive exigem papacc_personal_cng_inconclusive: provider_calls1/provider_status0/open_calls1/open_statusBAD_KEYSET/first_stage4/first_origin3/first_errorBAD_KEYSET, close conhecido e nenhum handle de chave retornado/retido, além de controlepositivo1. start_progress só gera o resultado interno tipado após esse getter original; serviço só reporta terminal após exit/SCM/finish válidos.

Assim, a mensagem SESSION_CLOSED_INCONCLUSIVE da tentativa2a2db773415e5b78cf4b165e5bbf91ae78f918ee8bb64acf030d0692aab5095b permite deduzir o predicado BAD_KEYSET estrito segundo o código qualificado. Não é uma linha bruta NCrypt separadamente exportada e não revela a causa interna do provider. O Win321168 é mapeamento terminal explícito, não o SECURITY_STATUS bruto original. Não promover códigos de tentativas anteriores a evidência bruta desta.

## Semântica e limites

Documentação Microsoft consultada em2026-11-03: [NCryptOpenKey](https://learn.microsoft.com/en-us/windows/win32/api/ncrypt/nf-ncrypt-ncryptopenkey). MACHINE seleciona chave do computador; SILENT impede UI. BAD_KEYSET é documentado como chave especificada não encontrada. A lista de retornos é não exaustiva; a documentação não garante que a mesma chave visível ao administrador produza NTE_PERM sob um token sem acesso. Logo, não se pode impor esse código ao Windows por correção cosmética ou deduzir ACL como causa somente do status.

O cache LSA documentado não prova o motivo da diferença e não autoriza liberar o original necessário ao controle. A advertência documentada sobre chamada durante startup de serviço também não é prova desta causa: o callback ps_main invoca o runtime, porém a tentativa presente completou e retornou resultado. Nenhum deslocamento para worker/thread ou alteração de lifecycle foi implementado com base nessa hipótese. Não confundir ausência de deadlock nesta tentativa com garantia universal de startup.

K5 é um critério normativo intencionalmente mais restrito do que simplesmente observar que a abertura não forneceu uma chave. O ensaio atual demonstra não obtenção de handle e fechamento inconclusivo correlacionado, não isolamento qualificado ou impossibilidade universal de acesso. K0–K9 e I0–I6 accepted permanecem intactos.

## Primeira lacuna e menor evidência útil

Falta evidência do motivo do provider para BAD_KEYSET sob o token original do runtime, apesar de controlepositivo da mesma identidade. Uma consulta administrativa fora do runtime, dump do mesmo status, enumeração de outra chave ou replay da sessão não resolve essa lacuna.

Se for indispensável continuar a investigação nativa, primeiro especificar um mecanismo Windows suportado de diagnóstico do Software KSP que revele a operação/predicado que falha sob aquele contexto, sem conteúdo privado e com custos/efeitos delimitados. A evidência teria de ser correlacionada a UMA nova tentativa autorizada, mesma chave/token/flags, sem fallback ou alteração de segurança. Provider/eventos/flags de tracing ainda não estão qualificados nesta investigação; não inventar comando ou pedir autorização de uma coleta indefinida. Não executar automaticamente. Alternativamente, decidir o requisito de produto que deve ser qualificado; qualquer mudança de K5 exige proposta focal e decisão normativa própria, não está autorizada por este relatório.

## Verificação e próximos passos

Somente auditoria das fontes e comparação determinística do nome foram executadas. Sem alteração de código/binário; quatro builds/88pares já qualificados reutilizados, sem campanha repetida. Imagem instalada882688/ID417614100:1966080:412413/SHA256e3a433c4a6edb565c783fee672bfc82a5794ba44988b123e7a38eb4ec5c9d6b7 permanece; nenhum UPDATE necessário por esta análise.

Primeiro gate para READ qualificado continua K5 sem provaNTE_PERM. Não há comando operacional pronto que remova legitimamente essa lacuna. Próximo trabalho offline: viabilidade documental de diagnóstico nativo específico, sem executar coleta ou criar subsistema. Não retornar a quotas ou repetir infraestrutura. Preservar sessão consumida, originais2555904:107563/3866624:108666, fixture, EXEs anteriores, HEAD/WIP/PST0.6.4 e UNKNOWN histórico/quiescência. Fechamento conhecido da sessão presente permanece conhecido, sem reclassificar histórias. ProdutivoDENY.
