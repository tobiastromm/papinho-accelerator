# Sysinternals — atribuição isolada de handles administrativos

## Resultado

Handle5.0 oficial foi baixado e executou76 consultas nativas bem-sucedidas neste Windows10Pro22H2 AMD64 build19045.6456. Quatro execuções sintéticas: Write-Host,Console.Out.WriteLine e uma réplica de cada;sete fases por execução. Não se repetiu LAB,qualificador operacional,SCM do LAB,Start/StopService,intent ou arquivos protegidos. Nenhum novo recorder ou parser de kernel foi criado. Python lê somente CSV gerado pela ferramenta Microsoft; o filho nativo é fixture de chamadas PSS.

O resultado identifica um custo evitável do caminho de exibição no fixture. Não autentica retrospectivamente os oito handles do PowerShell e dois do auxiliar da tentativa consumida. Eles continuam UNKNOWN por objeto,sem leak comprovado.

## Ferramentas e autorização

Inventário em tool-inventory.json. Não havia candidato Sysinternals localizado nos caminhos legíveis pesquisados;há limites de observabilidade fora deles. A página oficial [Handle](https://learn.microsoft.com/en-us/sysinternals/downloads/handle) identifica5.0,consulta por processo/tipo e requisito administrativo. O ZIP veio exclusivamente de https://download.sysinternals.com/files/Handle.zip,sem execução automática ou mirrors. Entradas previstas verificadas e extraídas com criação exclusiva. handle64.exe tem PEAMD64,assinatura Microsoft válida,SHA25624bafcc570cc9bbb6b6e6652a57a519e0464e3996891aaba6f55299cce20b04f. ZIP SHA256279aaf8eccb6f79147f4dcc6ba091fb895c4cb8b199a0dd186a4c76bc519d2cd.

A página não explicita mínimo de Windows suficiente;portanto compatibilidade foi TESTADA localmente,não presumida. O teste inicial sem elevação iniciou e recusou EULA,exit1;isso prova apenas loader. As76 consultas elevadas posteriores passaram e constituem a prova funcional local. A saída redirecionada é UTF16;listagens binárias originais e conversõesUTF8 preservadas.

O binário contém driverPROCEXP152.SYS/NtLoadDriver. Foi solicitada e recebida autorização explícita do owner para essa exceção Sysinternals,sem tocar o LAB. Consultas usaram -accepteula -a -g -v -p PID;nenhum -c/-y. Estado EulaAccepted da ferramenta pode persistir no usuário. O serviçoPROCEXP152 e o caminhoSystem32/drivers/PROCEXP152.SYS não foram observados antes/depois (1060/False);não há trace integral de criação/carga/remoção. Não transformar ausência posterior em alegação de zero efeitos temporários de driver. Nenhuma remoção manual foi realizada.

[Process Explorer](https://learn.microsoft.com/en-us/sysinternals/downloads/process-explorer)17.2 e [Process Monitor](https://learn.microsoft.com/en-us/sysinternals/downloads/procmon)4.12 atualmente declaram clienteWindows11+. Não foram necessários nem baixados/testados:Handle respondeu às tabelas. Isso não prova impossibilidade de executar essas versões emWindows10. WPR/WPA/Exporter10.0.19041 instalados com assinaturasMicrosoft válidas foram inventariados como fallback;nenhuma sessão ou nova capturaETW foi iniciada.

## Fixture e fronteiras

Fontes somente em build/sysinternals-admin-handle-attribution-20261024;nenhum código do produto foi alterado. PowerShell5.1x64 cria console novo oculto para não abrir janelas interativas ao usuário. O controlador externo usa Toolhelp e PID+criação para o cohort contemporâneo;conhost por parent é candidato de experimento,não autoridade/autenticação de console. Cada tabela temPID/criação antes/depois conferidos. Não reutilizarPIDs históricos.

Um filho sintético,sem SCM/rede,intent ou arquivo operacional,replica stdout,métricasAPI e PssCaptureSnapshot(THREADS|THREAD_CONTEXT)/walk/free do PowerShell e depois do auxiliar. A fontePSS segue as APIs já usadas no observadorED11;sem acesso a memória de stack ou novo formato de recorder. Cada snapshot/marker fecha antes da fasePOST_PSS. Saída contém PSS_RESULT0 e comandoStop do filho termina com CLOSED/exit0. A enumeração externa tem seu próprio processo/driver,cujos recursos não são somados ao administrador nem considerados gratuitos.

FasesA baseline;B processo/pipe/supervisão preparados;C_OUTPUT64linhas lidas e exibidas;C_METRICS consultas semPSS;C_PSS_POWERSHELL;C_PSS_AUXILIARY;D processo-filho/streams/Process descartados;E término normal do PowerShell e enumeração externa sem cohort residual. D mantémPowerShell/console para medição;E é outro fato. Não extrapolar a quiescência de fixture para a do serviçoLAB.

## Resultados nativos

| Caso | PS A | PS B | PS após stdout | PS D | Token A | Token após stdout | Token D |
|---|---:|---:|---:|---:|---:|---:|---:|
| Write-Host1 |575|623|644|645|1|27|37|
| Console1 |576|581|579|570|1|0|0|
| Write-Host2 |577|616|644|650|1|29|39|
| Console2 |577|582|580|571|1|0|0|

As contagens totais são API antes de cada consulta;os tipos vêm do CSV nativo da mesma janela,não de amostra atômica. Listagens por PID,tipo,nome,handle e acesso estão preservadas. A comparação alterou somente as chamadas de exibição dos dados sintéticos;controles/PSS/filho/Stop permaneceram. O primeiroWrite-Host também pode carregar móduloUtility e inicializar estado de hosting;isso explica por que não se deve transformar a diferença total em número exato de handles inevitáveis de supervisão.

Em Write-Host1,B→C_OUTPUT adicionou28 entradas de tabela e removeu7 (saldo21);26novosTokens líquidos. Exemplos de handles adicionados:0xA7C,0xA98,0xAC0,tipoToken,nomeTROMM/tobias:17ae46. A réplica também acrescentouTokens na mesma etapa. Ao finalD as tabelas ainda mostram37/39Tokens;os lifetimes posteriores entreD eE não foram traçados. As execuçõesConsole não apresentaram esse acúmulo. Isso demonstra redução do fixture ao evitar o caminhoWrite-Host;não identifica wrapper.NET alocador,stack ou leak universal.

Auxiliar:130 emA,131 emB/C,130 emD nos quatro casos. No primeiro caso,a entrada adicionada é0x218,tipoProcess,nomepss-fixture.exe(13968),removida emD. Nas réplicas,PID/handle variam e os CSVs mostram as identidades correspondentes. É um custo de lifetime do filho observado,sem atribuir ao auxiliar os handles que o observador mantém na sua própria tabela. O crescimento histórico+2 não foi reproduzido:nenhum quinto slot ou crédito é concedido.

Observador sintético permaneceu54handles nas capturas B/C e terminou normalmente apósStop. PSS foi capturado/liberado com sucesso;nos casosConsole,não houve crescimento persistente positivo apósPSS nas fronteiras medidas. Isso não prova custo transitório zero nem substitui o orçamento real do qualificador124/128. Nenhum valor54 foi usado como crédito ou limite normativo.

A contagem do alvo entreantes/depois de uma consultaHandle variou de-6a0. São mudanças NA JANELA do observador,não custo causal negativo da ferramenta. GC,threads e estado assíncrono podem variar durante a enumeração. Recursos internos do driver,físico/kernel e picos transitórios continuamUNKNOWN. Não somar76processos de ferramenta ao orçamentoLAB nem esconder seus custos.

## Limites de atribuição e lifetimes

Handle fornece snapshots de tabelas,não eventos completos de criação/fechamento. CSVs mostram owner,tipo,valor,nome e acesso. Novas entradas só são delimitadas entreduas capturas;remoção indica ausência na captura seguinte,não instante exato deCloseHandle. Mesmo handle/tipo/nome/acesso entreduas amostras não autentica mesmo kernelobject:reuso intermediário e objetos anônimos permanecemUNKNOWN. Erros <Unknown type>/<Error Opening Thread> foram preservados,não convertidos em zero.

TabelasPowerShell,auxiliar e filho são separadas. Um handleProcess do filho na tabela do auxiliar não é handle do próprio filho. Objects.NET e stacks de alocação não são fornecidos pela ferramenta;as entradasToken não provam qual classe gerenciada retém cada objeto. Não há novaevidência histórica suficiente para particionar exatamente8+2.

## Proposta focal,não aplicada

launcher-console-forwarding-PROPOSED.patch altera uma linha do loop existente:Write-Host $line → [Console]::Out.WriteLine($line),somente para encaminhamento da saída nativa. Não altera confirmação,Pin,locks,stdin,Process,reader,recibos,Stop,censo,owner,exclusividade,consumo,quiescência ou retenção. As demais mensagens e prompts permanecem. FontequalificadaSHA573bf1ca9a95dc052c992a0893b47f06f9dd96a92f53d3aa41cdf5bcc25951c2 permanece intacta;arquivoPROPOSED não deve ser executado como launcher.

O teste offline usa o loop real da proposta contraStringReader:StringWriter;drena>150000caracteres,preserva truncamento e recibos finais,e rejeita starts1/quiescent0 como fechamento seguro. Primeira versão do teste teve conflito deTextWriter do próprioharness;log preservado. Corrigido somenteo harness,mockandoWrite-Host de avisos;teste finalexit0. Isso não é falha do produto nem prova do caminho positivo. NenhumModePrepare/Cycle nativo foi invocado.

Segurança e efeito de saída devem ser revisados antes de aplicar:Console.Out bypassa o informationstream do PowerShell,podendo mudar transcript/customhost. A execução operacional exige console nativo interativo;isso deve ser confrontado com os contratos e recibos,sem dispensar logging obrigatório. Não fechar referências de identidade/pipe/processo/pins para tentarPASS. UNKNOWN/Stop doLAB permanecem código inalterado;somenteStopnormaldo fixture e falsofinalUNKNOWN do loop foram exercitados aqui,sem injeção de retençãoPSSnativa real.

Mínima redução demonstrada:redução do custo de exibição no fixture,sem dispensar referências de segurança. Redução segura no produto ainda é CANDIDATA,dependente de autorização funcional separada,revisão e qualificação offline do slice. Não prometer que resolverá o limite1024,numa futuraoperação ainda não autorizada. Tentativa02 continua consumida e não pode ser repetida. Não alterar quotas/ED11/N7,serviçoLAB,imagem,sink,intent,produção0.6.4 ou autoridadesDENY.


## Diferenças explícitas em relação ao LAB

O controlador externo redireciona stdin/stdout do PowerShell do fixture para gates e evidências,enquanto o launcher operacional exige console interativo sem esse redirecionamento. O fixture continua criando console nativo novo oculto e filho sintético com stdout redirecionado. A comparação é internamente controlada,mas não é qualificação factual da saída do operador noLAB. Não há pinosRAW1,autorização,cópia,rebind ou serviço no experimento. Diferenças de warmup,GC,threads e timing explicam por que números absolutos não são metasED11. O próximo slice precisa qualificar a proposta offline e sua semântica de console/transcript;nenhuma execução positiva deriva deste teste.
