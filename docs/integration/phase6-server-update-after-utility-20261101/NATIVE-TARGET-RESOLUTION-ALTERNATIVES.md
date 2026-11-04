# Resolução ProgramFiles — alternativas e conclusão focal

Task: avaliação offline do destino nativo, incluindo complemento do proprietário.
Branch main/HEAD f341985924d29870d8ff627ed37c8a0874ad6469/WIP preservados.
Conclusão: não foi demonstrada troca de API que simultaneamente preserve a
semântica de ProgramFiles atual e evite os recursos opacos observados. Hipótese
de um substituto Win32 simples SEM Shell encerrada no conjunto documentado abaixo.
Não é prova de impossibilidade universal ou de que qualquer API use23 handles.

Nenhuma coleta/SCM/UPDATE/endpoint/Start, mudança de código/guard/quota ou nova
proposta normativa nesta tarefa. Sem builds/testes repetidos: não houve delta
funcional. Evidências anteriores e autorizações consumidas preservadas.

## 1. Reutilização de identidade/referência original: prioridade examinada

A instalação existente tem proveniência já qualificada: ProgramFiles x64
resolvido como C:\Program Files na sessão documental anterior, objeto
417614100:131072:226316, SD digest
3e56550c9b305c98d4ca07f98ba18e2be96c86546539cd09b024ad506c838f40.
Fonte: phase6-root-chain-procedure-20261031/NATIVE-RESULT-RECONCILIATION.md.
Parent instalado417614100:8257536:71209 e imagem canônica posterior têm originais
próprios. Esses objetos não são intercambiáveis: ProgramFiles, Papinho e
Accelerator têm papéis distintos na cadeia.

Disponibilidade factual: a consulta histórica fechou referências; o novo
owner_acquire zera o root desta invocação. Antes da primeira guarda fria, há
token próprio, mas não diretório ProgramFiles original vivo no packet. Arquivos
de relatório, volume/file ID, digest e nome NÃO são aquele HANDLE nem provam
continuidade no intervalo. Não foi encontrada fonte existente que entregue esse
diretório ao controlador frio com provenance/lifetime já qualificados.
Coortes históricas UNKNOWN não são fonte de handles para tentar recuperar.

Portanto distinguir:

- **Revalidar referência viva já adquirida:** tecnicamente compatível com a
  proteção original, se mesma invocação/objeto/direitos/lifetime/proveniência
  forem demonstrados. Pode evitar rediscovery; não há referência assim disponível
  na entrada fria inventariada. Adquiri-la antes via Shell nesta mesma invocação
  não demonstra economia: o custo já teria ocorrido.
- **Reaquisição por identidade previamente selecionada:** OpenFileById é
  suportada e pode integrar essa solução. Exige handle de volume hint, seleção
  legítima do objeto, validação atual e cadeia. Não restaura o handle histórico
  nem, isoladamente, certifica que o objeto é a known folder ProgramFiles x64.
- **Handoff de outro processo:** DuplicateHandle exige referência viva e contexto
  de processos/direitos próprios. Não existe esse ingresso inicial de diretório
  qualificado no carrier frio. Um número de HANDLE em argv, cópia de string ou
  posse de endpoint não o implementa. Não criar novo canal/helper nem deslocar
  custos para outro processo como desconto de quota.

Esta é uma lacuna de disponibilidade/binding, não uma proibição normativa geral
de reutilizar objetos. A rejeição de EXISTING no código SCQ não constitui, por
si, uma norma proibindo essa reutilização. Porém simplesmente remover essa
rejeição permitiria paths DATA sem a prova que falta e não está autorizado.

## 2. APIs comparadas

| Alternativa | O que realmente obtém/dependência | Equivalência e conclusão |
| --- | --- | --- |
| SHGetKnownFolderPath/FOLDERID_ProgramFiles | Caminho atual da known folder; Shell32, PWSTR do task allocator | Fonte atual preservada. Não entrega HANDLE de diretório nem ceiling de internos |
| SHGetFolderPathW/CSIDL_PROGRAM_FILES | Buffer do caller; Shell32 | Em Windows Vista+ encaminha para SHGetKnownFolderPath. Compatibilidade semântica pode ser mantida com flags/arquitetura corretos, mas NÃO remove a descoberta Shell ou demonstra redução de handles |
| SHGetSpecialFolderPathW | API CSIDL antiga; Shell32, BOOL/fCreate | Microsoft declara não suportada e aponta SHGetFolderPath. Não adotar como solução suportada ou como garantia de menores recursos |
| IKnownFolder::GetPath | Known folder via interface COM/Shell32, PWSTR | Documentada equivalente a SHGetKnownFolderPath. Acrescentaria aquisição/lifetime de interfaces/apartment; não é rota mais simples ou livre dos mesmos internos demonstrada |
| SHGetKnownFolderIDList + SHGetPathFromIDListEx | PIDL e conversão de namespace Shell para caminho | Mais objetos/passes Shell, não identidade de kernel do diretório. PIDL não substitui handle protegido; sem garantia de redução |
| KF_FLAG_DEFAULT_PATH | Caminho padrão em vez de atual/possivelmente redirecionado | Muda semântica; não equivalente automaticamente à fonte selecionada |
| KF_FLAG_NO_ALIAS | Opção de alias para PIDL | Documentação diz que não afeta SHGetKnownFolderPath/GetPath; não fundamenta redução |
| GetSystemDirectoryW/WindowsDirectory + concatenação | Diretório do Windows/sistema, Kernel32 | Não identifica ProgramFiles. Inferir unidade/nome/relocação é caminho inventado, não equivalência |
| GetModuleFileNameW/QueryFullProcessImageNameW | Caminho de um módulo/processo | Pode fornecer candidato para correlação com arquivo próprio; não identifica independentemente a known folder nem autoriza adoção do seu parent |
| QueryServiceConfigW | Configuração de um serviço, via SC_HANDLE/SCM RPC | Útil para confrontar comando/conta/estado originais, não resolver known folder. SCM/origem da imagem não são sozinhos autoridade da seleção de diretório |
| GetFinalPathNameByHandleW | Caminho de objeto JÁ aberto, Kernel32 | Boa projeção/revalidação de uma referência legítima, não sua fonte inicial ou prova independente do papel ProgramFiles |
| OpenFileById + GetFileInformationByHandle | Referência de objeto por ID e fatos nativos atuais | Candidato técnico mais próximo do reuso pretendido, mas precisa binding original do alvo e proteção/cadeia; não substituto direto do getter atual |
| getenv/GetEnvironmentVariable/registro/path fixo | Valor de configuração/string | Não substitui a fonte de destino nem a prova original; nenhuma equivalência aceita por esta tarefa |

Não se conclui que COM ou Shell causou23, que haja vazamento ou que os internos
persistam por um lifetime específico. As duas coletas só localizam um grupo
contendo getter/allocator/drive/query e variação assíncrona. Não testar APIs
aleatoriamente para procurar uma contagem que passe.

## 3. Resolver caminho não é provar identidade/proteção

O resolver devolve texto e define qual ProgramFiles é pretendido. A prova de
objeto vem depois, em personal_scq_image_chain_win32.inc: aquisição relativa
NtCreateFile com OBJ_DONT_REPARSE e direitos mínimos, volume/file ID, tipo/
atributos/continuidade, GetSecurityInfo e validação owner/DACL/ancestrais. A
mesma guarda124 precede aquisições e os originais permanecem retidos até close.
GetFileInformationByHandle permite correlacionar identidade; texto de caminho
ou SD estruturalmente válido sozinho não prova papel/proveniência do alvo.

Handles diretamente controlados: os de arquivo/diretório abertos e retidos
pela cadeia, token e demais referências explicitamente adquiridas com owner.
No grupo getter atual, recursos devolvidos explicitamente são PWSTR, liberados
com CoTaskMemFree. GetDriveTypeW devolve enum, não HANDLE. Módulos obtidos por
GetModuleHandle são empréstimos, não justificam FreeLibrary. Internos do Windows
sem contrato de ownership não podem ser fechados pelo app.

Na alternativa COM, somente interfaces/apartment cuja aquisição própria seja
demonstrada teriam release/balanceamento correspondente; não chamar
CoUninitialize ou liberar cache para tentar reduzir contagem sem esse ownership.
Não inferir o estado COM do processo a partir da ausência de chamada explícita
no código do produto ou da contagem136. Não atribuir esse aumento a COM.

## 4. x86/x64 e fronteira normativa

Builds trabalhados são MSVC x64; a escolha atual é FOLDERID_ProgramFiles no
contexto desse target. CSIDL_PROGRAM_FILES equivalente deve respeitar a arquitetura;
não substituir por ProgramFilesX86, variávelProgramW6432 ou caminho de um helper
32bits. O ID histórico só identifica o objeto desse volume/host, não todas as
instalações Windows ou associação x86/x64 por si. O papel x64 precisa da seleção
original e sua proveniência, não de grafia do nome do diretório.

DECISION-personal-binary-destination-new-install-20261029 incorpora SHA256
e6d5ab6d57486dbe15d548e68e35dea4fd9abb124852ae36f9143378e9335d4d,
reconferido. Cláusulas exatas relevantes: “Instalações existentes preservam
caminho, identidade e configuração originais” e “Resolver ProgramFiles pela
known folder do target; congelar path absoluto e adquirir a identidade
original/proteção no procedimento autorizado”. A decisão exige known folders e
caminhos congelados/qualificados antes dos efeitos; não nomeia SHGetKnownFolderPath
como única implementação possível. Não inventar proibição ou dispensa adicional.

OHI/TO4–TO5/SSO-B/G2 mantêm originais, proveniência, identidade e lifetime;
G3/G4 e QUERY mantêm contagem completa124/128. Reuso de referência original
legítima é conceitualmente compatível com esses requisitos, mas equivalência
da fonte alternativa precisa ser demonstrada, não presumida de ID/string/SCM.
Não foi demonstrado GOVERNANCE CONFLICT entre normas vigentes.

O helper paths_existing preserva DATA de paths, e current retorna sem nova
descoberta porque objetos devem ser validados separadamente. Na entrada fria
não há producer de referência de diretório para cumprir esse segundo requisito;
scqi_chain_capture/current rejeitam EXISTING e validam NEW/BINARY_ONLY. Portanto
ligar esse helper diretamente seria alteração incompleta, não solução já coberta.

Um binding equivalente precisaria, conjuntamente, origem legítima que selecionou
o objeto ProgramFiles x64 existente; referência viva ou reaquisição original
autenticada; volume/ID/proteção/cadeia atual; ausência de reparse e substituição;
direitos/lifetime/Stop/retention governados; associação ao mesmo alvo/imagem/
serviço e ato atual. Sem autoridade por relatório/hash ou novo canal genérico.
Não precisa automaticamente nova ADR: primeiro demonstrar esse binding técnico;
se se pretender dispensar knownfolder/proveniência, isso sim exigiria decisão
focal explícita. Nenhuma proposta ou aceitação foi criada agora.

## 5. Resultado e primeiro gate

Conclusão negativa limitada: nenhuma API suportada examinada oferece uma
substituição simples, normativamente equivalente E com menor custo de handles
demonstrado. Alternativas equivalentes continuam Shell; alternativas Kernel32
resolvem outro objeto ou exigem referência original ainda ausente. Não afirmar
que uma rota mais barata é impossível em todo Windows, nem medir por inferência.

Não implementar uma troca especulativa, repetir coletas ou fazer UPDATE para
testar essa hipótese. Hipótese “trocar getter Win32 basta” encerrada sem nova
campanha. Nenhum código alterado, nenhum build/teste afetado/repetido.

Primeiro gate técnico restante, se o trabalho continuar: demonstrar a origem/
disponibilidade e binding de um alvo existente por referência original no
controller frio, não comparar mais APIs de string. Enquanto inexistente, DENY.
Não pedir aumento de quota, fechar internos ou deslocar Shell a outro processo
como desconto. Um eventual ingresso de diretório entre processos seria alteração
real do contrato de aquisição, não detalhe silencioso do carrier existente.

Preservados canônico/EXEs anteriores/retidos/pins/HEAD/WIP/atos consumidos,
guard124/cap128/G3/PST0.6.4. Retenção histórica/OS e causa1816 anterior UNKNOWN.
Sem coleta, UPDATE, endpoint/SCQ/Start/Stop, SCM/ACL, CA/CURRENT, rede, TPM/NV/Q4,
limpeza/refund ou autoridade produtiva. ED11/Phase6 PARTIAL, E21 PENDING.

## Fontes primárias da comparação

- [SHGetFolderPathW](https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetfolderpathw)
- [SHGetSpecialFolderPathW](https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetspecialfolderpathw)
- [IKnownFolder::GetPath](https://learn.microsoft.com/en-us/windows/win32/api/shobjidl_core/nf-shobjidl_core-iknownfolder-getpath)
- [SHGetKnownFolderIDList](https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderidlist)
- [SHGetPathFromIDListEx](https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetpathfromidlistex)
- [KNOWN_FOLDER_FLAG](https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/ne-shlobj_core-known_folder_flag)
- [GetModuleFileNameW](https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamew)
- [GetSystemDirectoryW](https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsystemdirectoryw)
- [QueryServiceConfigW](https://learn.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-queryserviceconfigw)
- [GetFinalPathNameByHandleW](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfinalpathnamebyhandlew)
- [OpenFileById](https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-openfilebyid)
- [GetFileInformationByHandle](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfileinformationbyhandle)
- [DuplicateHandle](https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-duplicatehandle)
