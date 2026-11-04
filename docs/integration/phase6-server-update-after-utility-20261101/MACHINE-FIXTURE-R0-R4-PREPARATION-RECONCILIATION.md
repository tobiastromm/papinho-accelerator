# Reconciliação — criação marcada, validação administrativa negada

2026-11-03. Fonte integral byte-exata OWNER-MACHINE-FIXTURE-R0-R4-PREPARATION-OUTPUT.txt, copiada do attachment3b45b867-3470-4849-8eda-5622261262f8 sem edição, SHA256311bb9c5a978963aa6a6f4a50ce8687de6197eb183a2f800c477d53ffae4cbce. Resultado MACHINE-FIXTURE-R0-R4-PREPARATION-RESULT.json/SHA256f73d6b818629abba81fbe28e944cc785114dc80ccae938690dee663df00b3516. Não houve retry, query nativa adicional ou mudança de recurso/ACL/SCM pelo agente.

D1firstNONE/consumedTrue/createdmask1/verifiedmask1/closeknownTrue; nova decisão5034bytes/ID417614100:1572864:759151/SHA2567c3ee69b26eb89088b5f07e6fbd8d151a77d07f65af0295c962fe47ca87bf2ab. Preservar nova cópia em MACHINE-TEST-ONLY-FIXTURE-R0-R4-DECISION.ORIGINAL.txt e todas as anteriores. Não criar outra, sobrescrever ou excluir.

F recebeu confirmação do plano bf019713bc60dc31f1246579a4ab166f1ada77a9967ab2bb03f29a2a8ecbaedd, consumiu antes de ca_begin e marcoucreated1. Retorno2148073488=0x80090010=NTE_PERM emsource_stage12, positive_control0/identity_known0, nativecloseknown1/persistent_key_created_reported1, exit2. A marca created é definida após NCryptCreatePersistedKey ter sucesso; não prova sozinha proteção/finalização/disposição persistente final qualificada. Preservar efeito parcial; não tratar como fixture admitida ou como inexistente. Nome reportado PapinhoAccelerator.CA.dea8648758bebcbbcabde016bf911e77.b4f8ff6a41ba3b4d3e5ff403d0d017a7 é locator do efeito criado, não identidade pública autenticada. PublicSHA desconhecido, controle positivo não alcançado. Não repetir criação, adotar objeto ou iniciar READ/E/Q/S/P/Start.

## Recursos passaram; erro mudou de fronteira

18consultas/18amostras/queryerror0/budgetfirst0, entrada116/último152, físico9867087872/commit10515353600. Privado2904064→3899392, crescimento995328; working6754304→8343552, crescimento1589248. Nenhum gate de recursos negou nesta tentativa. A amostra de outra execução não revela qual métrica/magnitude falhou na tentativa anterior; não reabrir quotas, reduzir riscos aceitos ou afirmar resolução retroativa do1816 antigo.

source_stage12 corresponde ao retorno de papacc_ca_key_fixture_create, não identifica diretamente uma API CNG. O owner já retém first_failure.function/stage/origin/native_api/raw, mas esses campos NÃO foram exportados na saída. Ausência desses campos não é firstcause0. NTE_PERM pode ser nativo ou rejeição de validação local estrita; não atribuir a setter, owner, serviço, máscara ou Windows sem prova.

## Delimitação offline com o código existente

Source faz seis amostras até a submissão F. No caminho CA positivo, as doze chamadas guardadas seguintes chegam à leitura NCRYPT_SECURITY_DESCR_PROPERTY em ca_acl. Modelo do MESMO owner/budget, reproduzindo essas seis amostras iniciais, demonstra cinco caminhos com total18: normal; NCryptGetProperty(Security Descr) devolve NTE_PERM; ou leitura sucede e a validação local nega por controle protegido ausente, flags de ACE ou máscara diferente. Nos quatro negativos o retorno externo é NTE_PERM/created1/finalization2 em modelo, com origins/stages internos distintos. Isso delimita leitura/validação do descritor como fronteira compatível, mas NÃO transforma inferência por contador em trace nativo histórico ou prova de qual ramo ocorreu.

Testes guardam os predicados, sem relaxar máscara/ordem/owner/SE_DACL_PROTECTED ou alterar bytes produtivos. Exemplos de máscaras diferentes são contraexemplos de rejeição, não forma efetivamente observada nem prova de equivalência. Não há defeito de proteção demonstrado a corrigir por conveniência.

Somente testes foram alterados: scq_machine_fixture_test.c e modelos da consulta. Quatro configurações Selected/Legacy Debug/Release de dois testes afetados=8pares PASS/zero warnings; fixture244checks/source37cenários/146checks. Warning inicial de comparação signed/unsigned no teste corrigido antes dos quatro builds; log inicial preservado. Sem repetir a campanha produtiva52pares ou recompilar/instalar EXE para instrumentação. Código produtivo continua nos pins do manifesto R0–R4; imagem instalada871936/ID417614100:10223616:73344/SHAa6b8ee1ac5ef3e7b6908686f3690ce2a6ca6c4171812a201b7d3024ea6f2ab8a inalterada.

## Menor evidência adicional preparada, não executada

Consulta prospectiva de owner/DACL do único nome MACHINE reportado, sob administrador legítimo em outro processo/handle, usando NCryptOpenStorageProvider uma vez, NCryptOpenKey MACHINE|SILENT uma vez e NCryptGetProperty(Security Descr,1024bytes,flagsOWNER|DACL=5) uma vez, sem sizing/retry/fallback. Apenas closes do próprio observador. Nenhuma API Create/Set/Finalize/Delete/Sign/Export, impersonação, rede/SCM/Start ou produto. Mesmo mecanismo diagnóstico pontual PowerShell/C#, sem subsistema/serviço/root/canal permanente.

Não é reprodução fiel da abertura histórica: referência original da criação fechou, token/processo/handle são novos e não há publicSHA prévio para provar continuidade. Observação pode mostrar status nativo/forma ATUAIS, não autorizar adoção ou qualificação da fixture original. Match/exit0 não é controle administrativo positivo da mesma chave retida, probe de runtime, receipt/close ou readiness. Não executar consulta sem autorização específica.

fixture-security-descriptor-observer.cs qualificado com29checks C#/IO injetado/reflexão de apenas4imports/Stop/replay/size/parse/nativefail/closeUNKNOWN; construtor Native não chamado. AST PASS do procedimento, sem execução. FIXTURE-SECURITY-DESCRIPTOR-OBSERVATION-OFFLINE-QUALIFICATION.json/SHA256aa35e420871e7344f9d97de5a2f3c4c40381d8ac64dfb69f90908f72d5c0cf10 fixa pins/testes. Métodos e flags confrontados com SDK local e documentação primária [GetProperty](https://learn.microsoft.com/en-us/windows/win32/api/ncrypt/nf-ncrypt-ncryptgetproperty), [OpenKey](https://learn.microsoft.com/en-us/windows/desktop/api/ncrypt/nf-ncrypt-ncryptopenkey) e [propriedades](https://learn.microsoft.com/en-us/windows/win32/seccng/key-storage-property-identifiers); não se alega que a documentação explique esta falha.

Primeiro gate: autorização específica de ONE-FIXTURE-SECURITY-DESCRIPTOR-OBSERVATION-REQUEST.CANDIDATE.md/SHA25670bc5bae6e9fc95dddfb9cebd2d0491910b5a0f557acb4dfa137738d685ccbf6. O pedido delimita custos/caches/latência/close incertos, não usa orçamento de outro perfil ou concede autoridade produtiva. Nenhum comando com placeholders ou aprovação fictícia emitido. Próximo NEXT-FIXTURE-SECURITY-DESCRIPTOR-OBSERVATION-AUTHORIZATION.md.

## Consumo e preservação

D1 e F consumidos; sessão não reutilizável, mesmo com validação falha. Snapshot inicial pendente preservado e uso separado em MACHINE-FIXTURE-R0-R4-PREPARATION-AUTHORIZATION-USED.json. Não limpar/excluir/recuperar/refund/rollback/kill ou alterar ACL/proteção da chave parcial. Fechamento conhecido atual não resolve application-close/retenção/reservas/quiescênciaOS históricos UNKNOWN. Nenhuma redução de guard ou proposta de quota nova é necessária para a classe de falha atual.

Somente após evidência suficiente e compatibilidade accepted definir correção técnica ou decisão normativa focal indispensável; sem transição para READ/E/Q/S/P/Start enquanto fixture não qualificada. Preservar todos os originais/decisões/EXEs/DLLs/SCM/HEAD/WIP/históricos/pins/consumos/PST0.6.4 e DENY produtivo.
