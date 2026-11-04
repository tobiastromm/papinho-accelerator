# Pedido operacional — observação read-only do descritor da fixture

CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. 2026-11-03.

Solicita UMA execução MANUAL de observação prospectiva somente leitura, estritamente do descritor owner/DACL do nome MACHINE abaixo, sob o administrador original legítimo. Não repete criação, materialização D1/F ou sua autorização consumida. Não qualifica/adota a fixture nem autoriza READ/E/Q/S/P/Start. Não requer UPDATE, serviço, diretório, protocolo ou infraestrutura nova.

## Evidência e limite do objetivo

Na preparação c504edee8142647c3a5e2661feb8d46985baf4dbe28acb1b45d3cbe3c6bc026e, D1 fechou a decisão417614100:1572864:759151/5034bytes/SHA7c3ee69b26eb89088b5f07e6fbd8d151a77d07f65af0295c962fe47ca87bf2ab. F consumiu, marcou created1 e retornou2148073488/0x80090010/NTE_PERM em source_stage12, sem controle positivo ou identidade pública conhecida. Nativecloseknown1; o contador persistent_key_created1 não prova sozinho disposição final, proteção ou identidade persistente qualificada. Fonte integral OWNER-MACHINE-FIXTURE-R0-R4-PREPARATION-OUTPUT.txt/SHA256311bb9c5a978963aa6a6f4a50ce8687de6197eb183a2f800c477d53ffae4cbce; resultado MACHINE-FIXTURE-R0-R4-PREPARATION-RESULT.json/SHA256f73d6b818629abba81fbe28e944cc785114dc80ccae938690dee663df00b3516.

Recursos não negaram:18consultas/18amostras, query_error0/budget_first0, entrada116/último152, físico9867087872/commit10515353600, crescimento privado995328/working1589248bytes. Não subir quotas ou atribuir a nova falha aos recursos. A causa1816 antiga não foi resolvida por esses dados.

Modelos do owner CA existente demonstram que18amostras, com seis no source, são compatíveis tanto com NTE_PERM nativo de NCryptGetProperty(Security Descr) quanto com NTE_PERM de validação local estrita do owner/controle/ACEs/máscaras após uma leitura bem-sucedida. A primeira causa CA já é armazenada no owner, mas esses campos não foram exportados pela saída e o owner fechou. Não afirmar que o setter/DACL ou o Windows são culpados apenas pelo código. Casos de descriptor normal/controle protegido ausente/flags de ACE/máscara diferente são contraexemplos modelados, não observações do host ou prova de equivalência de direitos.

Objetivo desta observação: obter STATUS nativo da abertura e da leitura owner/DACL ATUAIS e, se a leitura suceder, a forma estrutural atual para confrontar o predicado exato, sem modificar o objeto. Não reproduz fielmente a tentativa histórica: novo processo/token/handle, objeto reaberto por nome depois do fechamento; sem referência original retida da criação ou publicSHA prévio para provar continuidade. Essas limitações são explícitas ANTES de solicitar autorização. Nenhum resultado atual resolve automaticamente a causa passada.

## Objeto único e APIs exatas

Provider fixo: Microsoft Software Key Storage Provider.

Nome MACHINE, obtido da saída original, somente LOCATOR/DATA:
PapinhoAccelerator.CA.dea8648758bebcbbcabde016bf911e77.b4f8ff6a41ba3b4d3e5ff403d0d017a7

Nenhum nome alternativo, enumeração ou argumento de chave livre. Não consultar candidatos das tentativas anteriores nem CurrentUser. Se o objeto não puder ser aberto, registrar o código real e encerrar, sem criar/adotar/recuperar a chave. Nome sozinho não é identidade ou autoridade original.

Uma NCryptOpenStorageProvider(provider,0); uma NCryptOpenKey(nome,legacy0,flags0x60=MACHINE0x20|SILENT0x40); uma NCryptGetProperty("Security Descr",buffer1024bytes,outlength,flags5=OWNER1|DACL4). A leitura usa MESMO nome de propriedade, tamanho e flags do predicado CA; a aquisição por OpenKey e seu lifetime são diferentes da aquisição CreatePersistedKey histórica e não alegam equivalência. Não realizar sizing call, aumentar buffer, tentar outros flags ou fallback. Código nativo não zero, tamanho inválido ou falha de parsing encerra; campos inicializados não são prova de escrita nativa na falha.

Closes de handles do próprio observador via NCryptFreeObject uma vez por handle não nulo, no máximo dois. Falha de close retém o restante/owner, sem retry ou limpeza automática. Nenhum NCryptCreatePersistedKey/SetProperty/FinalizeKey/DeleteKey/SignHash/ExportKey, impersonação/duplicação de token/elevação de privilégio, endpoint, SCM ou Start. As quatro únicas importações nativas foram verificadas em modelo; SECURITY_STATUS preservado como hexadecimal de32bits, não convertido em Win32 genérico5. Não há buffer privado ou exportação pública/privada da chave.

Referências primárias: [NCryptGetProperty](https://learn.microsoft.com/en-us/windows/win32/api/ncrypt/nf-ncrypt-ncryptgetproperty) documenta a leitura de propriedade e seleção owner/DACL; [NCryptOpenKey](https://learn.microsoft.com/en-us/windows/desktop/api/ncrypt/nf-ncrypt-ncryptopenkey) documenta abertura MACHINE/SILENT e possíveis caches internos; [identificadores](https://learn.microsoft.com/en-us/windows/win32/seccng/key-storage-property-identifiers) define Security Descr. Flags/nome também conferidos em Windows SDK10.0.26100.0/um/ncrypt.h local. Documentação confirma API suportada, não sua causa histórica ou forma retornada neste host.

## Autoridade, confirmação e efeitos

Operador esperado TCORP-TOBIAS/SID S-1-5-21-785699550-1440458426-1682014700-1108, contexto administrativo legítimo, sem impersonação, console WindowsPowerShell5.1 Desktop AMD64, entrada/saída não redirecionadas. Essas verificações são elegibilidade; arquivo/hash/CLI/owner/keyname não são autoridade. Aprovação específica do proprietário deste pedido e declaração manual OBSERVE_FIXTURE_SD_DEA86487_ONCE são obrigatórias. Consumo único antes de abrir o provider, Stop entre chamadas; sem respostas automáticas, retry ou continuação após erro.

Máximo três chamadas de aquisição/leitura e dois closes. Nenhuma mutação explícita persistente de chave/arquivo protegido/ACL/SCM. Reutiliza o mecanismo diagnóstico pontual PowerShell/C# já empregado, sem instalar componente ou criar canal permanente. Compilador/CLR/CNG/Windows podem alocar memória/handles/cache e arquivos temporários de framework; custos opacos e latência síncrona permanecem UNKNOWN, não são zero ou limites nativos provados. Software KSP pode manter caches enquanto o handle estiver aberto. Não se importam quotas K1/R0/K7 do Accelerator ao processo observador nem se alega capacidade da aplicação pelo observador. A aprovação reconhece esses riscos residuais próprios.

Se close falhar, o script preserva owner/referências e aguarda, sem kill/refund/retry. Stop não garante cancelamento de chamada síncrona já submetida; nenhum prazo de SO é garantido. Não executar nova coleta automática para resolver um resultado inconclusivo.

Saída permitida: statuses de provider/OpenKey/GetProperty, comprimento/capacidade capturados, disponibilidade de sample, owner_operator booleano, flag de DACL protegida, contagem de ACEs, tipo/flags/máscara e papel SID(OPERATOR/RUNTIME/SYSTEM/OTHER), projeção estrita atual MATCH/REJECT e motivo. Não emitir SDDL completo/SIDs desconhecidos/ponteiros/handles/bytes privados ou credenciais. Projeção não é autoridade ou alteração de guard. MATCH/exit0 não qualifica fixture, não substitui controle positivo público da mesma chave original, não atesta isolamento do runtime ou readiness. NTE_PERM do administrador não é o probe restrito do runtime.

## Artefatos e qualificação offline

fixture-security-descriptor-observer.cs SHA25679dc77c154229e51188ad5c04b4a3122ecfb1409749338f737c6a2b192119fa7.
observe-fixture-security-descriptor.MANUAL.CANDIDATE.ps1 SHA25626d058ee2de0da7cb467d0c105e322e139b6834a330418d5c0ebd49de01dc12c.
FIXTURE-SECURITY-DESCRIPTOR-OBSERVATION-OFFLINE-QUALIFICATION.json SHA256aa35e420871e7344f9d97de5a2f3c4c40381d8ac64dfb69f90908f72d5c0cf10.

Modelos C#29checks/compilação WarningLevel4/warnings como erros/AST PASS, sem instanciar Native ou executar KSP; IO injetado para sucesso/códigos nativos/falhas parciais/Stop/replay/tamanho inválido/parse/closeUNKNOWN. Reflexão confirmou somente quatro APIs de leitura/close. Modelos CA cinco contraexemplos/18amostras,244checks fixture, source37cenários/146checks; quatro builds de testes sem warnings/8pares afetados PASS. Código produtivo/EXE não mudaram; não requer UPDATE. Log inicial de warning de signedness no teste foi preservado e corrigido antes dos quatro builds finais. Não repetir campanhas inalteradas.

Após aprovação específica, registrar somente a decisão realmente recebida, verificar os hashes e preparar comando manual exato com SHA256 real do script e deste pedido. Não executar automaticamente e não fornecer comando com placeholders. Nenhuma cópia documental protegida é necessária para esta consulta pontual; aprovação não é outra materialização D1.

## Reconciliação e limites absolutos

Após única saída manual: conferir primeira falha/consumo/calls/statuses/comprimentos/forma atual/close; distinguir erro nativo atual de rejeição da projeção local atual. Preservar UNKNOWN histórico, ausência de publicSHA/continuidade original e falta de controle positivo. Se houver evidência de incompatibilidade legítima de representação, confrontar contratos accepted antes de qualquer correção; não aceitar máscara/owner/flags divergentes por conveniência ou promover objeto atual a fixture original.

Não autoriza repetir criação D1/F, abrir chave como runtime/probe P, materialização, UPDATE, StartStop/endpoint/SCQ/READ operacional, assinatura/exportação privada, alterações SCM/ACL existentes, CA/CURRENT/rootstore, rede, TPM/NV/Q4, limpeza/refund/rollback/delete/kill ou produção. Preservar chave/efeitos parciais, todos os originais/decisões/EXEs/DLLs/SCM/HEAD/WIP/históricos/pins/consumos, UNKNOWN histórico/quiescênciaOS/retention e PST0.6.4. Somente após nova evidência suficiente definir próximo gate; fixture/READ/Start permanecem não qualificados.
