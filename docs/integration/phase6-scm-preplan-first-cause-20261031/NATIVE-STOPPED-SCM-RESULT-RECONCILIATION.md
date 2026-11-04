# Marco alcançado — registro SCM parado qualificado no ensaio R0–R9

Resultado do proprietário preservado byte-exato do anexo em OWNER-CORRECTED-SCM-RESULT.txt, SHA256 `2b7cd87e6931907b81a7b49df455eb45c6738c6867b72e5e0e6e9c518d3ab83d`. Origem: texto anexado pelo proprietário, sem assinatura ou atestação independente de imagem mapeada. Nenhuma nova consulta SCM, execução, Start/Stop ou coleta foi feita pelo agente para reconciliar.

## Resultado e consumo

O pedido fd08f3ae… foi utilizado pela única execução manual. A tentativa pública eb98ed9f…f11ede6 aparece na apresentação e na confirmação atribuída. Há sequência coerente de CONFIRMED, gate PRE_CREATE, effect1/result0, gate PRE_CHANGE, effect2/result0, fases de fechamento, STOPPED_REGISTRATION_QUALIFIED/consumed1/controlled_closeKNOWN e wrapperexit0/INVOKEDTrue. Não há primeira falha reportada nesta sequência positiva.

A autorização corrigida agora está UTILIZADA e não pode ser repetida. A autorização anterior7eb9e691… também continua utilizada. Seus registros de aprovação/pedido/script permanecem imutáveis; este resultado posterior estabelece disposição/consumo, sem rearmar grants. Fechamento controlado conhecido e ausência de retenção no caminho de sucesso não equivalem a quiescência OS universal, que continua UNKNOWN.

O registro parado é qualificado no alcance deste ensaio PERSONAL_LOCAL/R0–R9. A correção de BINARY_ONLY deixou de bloquear o percurso pré-plano nesta execução. Não se conclui retroativamente que o RNG passou na tentativa anterior; sua primeira condição exata permanece historicamente UNKNOWN.

## Serviço e originais

Serviço PapinhoAccelerator, conta configurada NT SERVICE\PapinhoAccelerator, SERVICE_WIN32_OWN_PROCESS, DEMAND_START3, ERROR_NORMAL1, SID type UNRESTRICTED1 e comando `"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed` correspondem ao plano aprovado.

O efeito1/result0 comprova a submissão de criação bem-sucedida no caminho qualificado; efeito2/result0 corresponde à configuração de SID type. O resultado terminal só é emitido depois de QueryServiceConfig/QueryServiceConfig2 concordantes, verificação de identidade SID e QueryServiceStatusEx completo com SERVICE_STOPPED, mais revalidação final e fechamento controlado. Essa é a evidência de readback original vinculada ao código/artefato qualificados; os valores de configuração/status não foram emitidos como um dump separado nem novamente consultados pelo agente. Não usar apenas exit0 como prova.

O estado foi SERVICE_STOPPED naquele readback. Não se afirma observação contínua depois dele nem impossibilidade de ação posterior de outro administrador. Efeito persistente demonstrado nesse alcance: registro SCM local com comando/conta/início/SID configurados; não há instalação produtiva, CA ou root administrativa criada por consequência.

O descritor de segurança do objeto serviço não foi emitido ou consultado nesta reconciliação; não inferir sua ACL a partir da proteção dos arquivos. Isso não amplia o resultado limitado de configuração/readback R0–R9 para uma qualificação geral de acesso ao serviço.

Originais reportados durante a tentativa (volume/high/low):

| Objeto | Identidade | SHA256 reportado |
|---|---|---|
| Diretório dos binários | 417614100:8257536:71209 | Não aplicável |
| papacc_server.exe staged | 417614100:6225920:71237 | c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf |
| libssl-3-x64.dll | 417614100:15990784:71249 | 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed |
| libcrypto-3-x64.dll | 417614100:22216704:9543 | 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044 |

Os três pins correspondem ao alvo aprovado; DLLs1321472/7217664 bytes. Essas identidades/proteções foram adquiridas/revalidadas pelo caminho original desta tentativa, no alcance declarado da evidência, não são autoridade permanente/cross-boot para operações futuras. O controlador63291745… é distinto do EXE staged c0f27f94…; não houve novo staging.

A imagem da interface de Serviços fornecida na mesma mensagem corrobora visualmente o nome e tipo Manual e mostra o trecho do comando diagnóstico. O campo de caminho aparece parcialmente deslocado/truncado; a imagem não revela token/runtime, conta na aba Logon ou SID type, nem comprova sozinha estado STOPPED/fechamento. Não foi tratada como prova independente ou autorização para clicar Iniciar.

## Recursos no alcance observado

Sete amostras de fase, handles136/149/152/159/159/165/160; pico amostrado165, privado4.009.984 bytes e working set9.297.920. Mínimos reportados: físico7.239.172.096 e commit9.823.326.208. Não são baseline, teto universal, prova de atribuição completa ou leak pelo incremento entre primeira/última amostra. Contextos SCM próprios não equivalem à contagem total de handles.

Reserva controlada apresentada2.008.964 <= Q3.145.728; isso não significa reserva física. Ambos os gates de efeito reportam físico/commit acima do piso Q. Não comparar privado total com Q como se fossem o mesmo domínio e não importar 124/128/171 como teto deste perfil R0–R9.

| Intervalo reportado | Efeito1 Create | Efeito2 SID type |
|---|---:|---:|
| Consulta de recursos | 3,0871 ms | 3,1375 ms |
| Fim da consulta até início do intervalo de efeito | 0,1683 ms | 0,1703 ms |
| Intervalo do efeito | 63,693 ms | 0,1722 ms |

Valores derivados exclusivamente dos QPCs/frequência10.000.000 reportados. O intervalo entre primeiro begin e último end é60,4958655 segundos, incluindo interação humana; não é tempo total medido do processo nem timeout ou ceiling. Freshness foi por evento/contexto conforme R3, sem importar500ms/120s. Custos opacos nativos/RPC, variações entre amostras e quiescência OS continuam UNKNOWN. Detalhes em CORRECTED-SCM-RESOURCE-RECONCILIATION.json.

## Escopo do PASS e primeiro gate restante

SCM_REGISTER_CLOSED_NATIVE = QUALIFIED_LIMITED_R0_R9 para esta tentativa. Isso não demonstra TokenUser/grupos/restrições do runtime, PID/criação de processo de serviço, autenticação SCQ bilateral, KSP MACHINE/isolamento CA, readiness produtiva ou Phase6 PASS. Não houve Start/Stop/endpoint/CA/CURRENT autorizado ou executado pelo agente. Nenhum código/build/teste inalterado foi executado nesta reconciliação.

O próximo gate concreto de Start está separado: na árvore atual, papacc_server_service_startup_source_status ainda retorna ERROR_ACCESS_DENIED e exige origem original/associação/escopo/lifetime/readiness; o caminho de registro não o preenche. A seleção operacional SCQ no backend também permanece NULL, como ausência deliberada de decisão original operacional, não como defeito a corrigir com um grant/flag. Registro SCM não restaura F0 nem autoriza startup por metadados.

Antes de solicitar Start, fechar a composição específica da admissão original diagnóstica no mesmo owner, distinguindo fonte já implementada em modelo de entrega operacional fresca ainda não demonstrada. Ver NEXT-FIRST-DIAGNOSTIC-START-READINESS.md. Não há comando/pedido operacional de Start pronto nesta entrega; emitir um que apenas encontre o gate sempre fechado seria prematuro. Endpoint, lifecycle e CNG mantêm autorizações distintas; R0–R9 não se estende automaticamente a eles.

E21 PENDING; ED11/Phase6 PARTIAL; OS_QUIESCENCE UNKNOWN; PST0.6.4, HEAD/WIP, históricos/pins, reservas e chave fixture retida preservados.
