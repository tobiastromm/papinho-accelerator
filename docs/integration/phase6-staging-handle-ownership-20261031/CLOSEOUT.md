# Ownership dos handles do staging — fechamento da análise estática

Task PACC-PHASE6-STAGING-HANDLE-OWNERSHIP-AND-REDUCTION.
Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP anterior preservado.
Nenhuma coleta, execução do produto, operação protegida ou alteração de código efetuada.

## Processo e métrica

server_personal_resource_sample_win32.inc usa GetProcessHandleCount(GetCurrentProcess()). A query pós-entrada e staging_open usam o mesmo processo. O pseudohandle GetCurrentProcess não é um OpenProcess nem uma referência controlada adicional. O script PowerShell chama um executável filho; não executa o sampler dentro de PowerShell. A contagem não soma o pai nem conhost. Handles herdados que efetivamente existam na tabela do filho contam normalmente; a saída não enumera quais são herdados. Recursos de um objeto compartilhado podem existir em múltiplos processos; isso não permite subtrair uma entrada real da tabela do processo.

A API fornece quantidade de handles do processo, não inventário de objetos, origem, stack, tipo, direitos ou flag de herança. Não é contagem só das referências geridas pelo stager. Duas entradas para o mesmo objeto continuam dois handles reais; não há deduplicação por objeto nessa quota. Nenhuma duplicação ou erro aritmético demonstrado.

## Intervalo causal limitado pelo código

| Ponto | Ação | Ownership / conclusão |
|---|---|---|
| Antes de main | Loader, imports, CRT e herança | Incluídos no processo; inventário histórico ausente |
| main | Dispatch explícito --qualification-staging-observe antes de service_cli | Não inicia caminho de serviço/rede por esse dispatch |
| Inicialização | Zero root; thread/invocation; reserva sizeof | Sem aquisição explícita de handle |
| Sample1 | QPF/QPC, GetProcessHandleCount, memória/global/performance, fprintf | 109 na query inicial; APIs internas não são inventário |
| Pós-frame | GetProcessHandleCount | 109; nenhum aumento líquido até aqui |
| STD | GetStdHandle(input/output) | Referências emprestadas, sem DuplicateHandle; não podem ser fechadas como se adquiridas |
| Layout | Gates de estado/thread/invocation; paths_binary | Sem aquisição explícita de handle nos gates |
| Resolver | SHGetKnownFolderPath(ProgramFiles,DONT_VERIFY,NULL), cópia/join, CoTaskMemFree | PWSTR alocado e liberado; recursos internos opacos, não handles transferidos ao chamador |
| Sample7 | QPF/QPC e GetProcessHandleCount antes das demais queries | 132; aumento líquido23 no intervalo anterior à query |
| Depois da query | Demais métricas, output e guard | Não explicam retroativamente o valor132 já amostrado |
| Falha | 1816/stage9/location2 | Guard124 excedido8; cap128 excedido4 |
| Close | Nenhum objeto de staging aberto; contexto não adquirido | Close controlado conhecido não prova encerramento de caches do OS |

Entre a query pós-frame109 e a query132 existem GetStdHandle, estado/local paths, SHGetKnownFolderPath, CoTaskMemFree, QPF/QPC e chamadas de thread/Stop/CRT. Não é possível distinguir criação líquida de23 de criação maior seguida de fechamentos. Não existe evidência do pico instantâneo entre samples. Atribuir todos23 a shell/COM seria uma inferência, não prova. A ausência de ProgramData nessa versão elimina apenas essa consulta como requisito do caminho binário, não identifica um componente interno culpado.

## Referências que NÃO explicam o excesso

As16 posições de objects do stager ainda estão vazias. Token, volume, ancestrais, fontes e destinos não foram abertos. Descritores, SID TrustedInstaller, scratch e proteção ainda não foram materializados. Não há referência original que possa ser liberada mais cedo nesse ponto. O bloco4096 e os buffers de path são memória, não handles. stdin/stdout são emprestados; fechar esses handles quebraria output/Stop e não seria redução legítima demonstrada. GetModuleHandle de ntdll é usado mais tarde e é emprestado; FreeLibrary não é fechamento correspondente autorizado.

## Dependências estáticas verificadas

DUMPBIN /DEPENDENTS do executável aprovado foi executado como análise de arquivo, não como execução do produto. Imports: IPHLPAPI, libssl-3-x64, libcrypto-3-x64, WS2_32, bcrypt, ktmw32, CRYPT32, ADVAPI32, SHELL32, ole32, KERNEL32, VCRUNTIME140 e famílias UCRT heap/stdio/string/convert/environment/runtime/math/locale. Imports demonstram dependência de carregamento, não chamadas executadas nem quantidade de handles criada por cada DLL. Não há fundamento para remover OpenSSL, shell ou COM da imagem canônica apenas a partir dessa lista. Não se altera backend/toolchain nem cria executável/helper para deslocar custos.

## Aplicabilidade e correção

124 é o guard próprio usado pelo sampler;128 é o cap mantido no pedido BINARY_ONLY aprovado, em TO0–TO9 e nas especializações de recursos. 1024 administrativo não compensa violação do nativo. A decisão LAB B+D distingue explicitamente processos e proíbe transferência de custo, mas não é autoridade operacional geral para esta instalação. O pedido BINARY_ONLY determina STOP se o cold path obrigatório ultrapassar o envelope. A decisão accepted de destino exige known folders congelados e qualificados, não autoriza paths inventados, ambiente como prova original ou alteração de quotas.

Nenhuma redução segura foi demonstrada: não existem handles controlados supérfluos abertos nesse intervalo. Reordenar resolução para antes do primeiro sample esconderia a atribuição, mas manteria132 no mesmo processo e não satisfaria o cap. Liberar PWSTR já ocorre. Descontar os109 de entrada, ignorar herança, fechar handles por enumeração, encerrar cache/COM ou liberar módulo emprestado viola ownership ou contabilização. Repetir a mesma coleta não identifica a origem. Não foi implementada mudança cosmética ou permissiva.

Não há pico esperado corrigido fundamentado: UNKNOWN. O único pico amostrado informado é132; não é teto. Builds/testes não repetidos: código e binários inalterados, sem correção a qualificar. Evidências anteriores permanecem válidas no seu alcance offline, sem reivindicação de melhoria nativa.

## Lacuna factual EXATA e próximo gate

Faltam a identidade, origem e lifetime das entradas da tabela de handles criadas/fechadas entre a query pós-frame109 e a query do sample7=132, correlacionadas ao mesmo processo/criação e às chamadas do intervalo. Os dois snapshots agregados não contêm essa informação; não existe transformação estática que a recupere. Falta também inventário da composição dos109 iniciais; não se presume herança dispensável. Mesmo um inventário final sem histórico de criação não prova ownership transferível ao stager.

Para fundamentar uma correção, é necessária evidência de atribuição de criação/fechamento nesse intervalo, ou prova estática de uma aquisição controlada desnecessária. A segunda não foi encontrada. Outra medição somente de contagem, nova remoção de ProgramData ou amostragem de memória não resolve a lacuna. Uma eventual prova nativa de atribuição tem de registrar origem/lifetime sem fechar handles, mudar quota, mover a medição ou conceder autoridade; precisa de escopo e autorização próprios e não está aprovada. Este relatório não emite retry nem comando. Não se afirma que esse instrumento já exista ou que seu overhead caiba no envelope.

Staging permanece DENY. Primeiro gate é identificar um recurso efetivamente removível com ownership comprovado; depois corrigir, qualificar offline e somente então solicitar nova coleta específica. Não há pedido de staging protegido pronto. E21 PENDING; ED11/Phase6 PARTIAL; PST0.6.4 e ambas autorizações consumidas preservadas.
