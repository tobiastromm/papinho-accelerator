# raw0 — investigação do namespace e correção offline restrita a P1

main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469. Sem coleta nova.
A observação ce5b3f33...bee166 permanece consumida, sem retry.

## Caminho efetivamente analisado

Win32:
`C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\FIRST-P1-SESSION-REQUEST.ORIGINAL.md`

NtCreateFile recebeu, por concatenação literal:
`\??\C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101\FIRST-P1-SESSION-REQUEST.ORIGINAL.md`

OBJECT_ATTRIBUTES zero-inicializado, RootDirectoryNULL, nameUTF16 sem NUL na
Length/MaximumLength, flags1040(CASE_INSENSITIVE|DONT_REPARSE), SD/QoSNULL.
Access00120089,shareREAD,FILE_OPEN1,options00200060,EA/allocationNULL.
Observação: handles68, NTSTATUSC000050B, IOSB snapshots0, handle0. Não houve
readback de identidade/hash/descritor nessa abertura. Exit0 não significa openPASS.

O caminho da PRIMEIRA abertura não contém Program Files. A seleção P1 dos
objetos instalados não altera o path do pedido, que está na árvore do projeto.
Não associar C000050B a Program Files ou a DACL inadequada.

## Componente demonstrável e limite factual

Há um indirecionamento de namespace DOS explícito ANTES dos componentes do
filesystem: `C:`. A documentação Microsoft define nomes DOS, incluindo C:,
como symbolic links no Object Manager; QueryDosDevice retorna seu mapping
atual, armazenado como junction do namespace de objetos. Isso é diferente
de demonstrar junction/reparse NTFS em C:\Projetos ou no arquivo final.

- [MS-DOS device names](https://learn.microsoft.com/en-us/windows-hardware/drivers/kernel/introduction-to-ms-dos-device-names)
- [QueryDosDeviceW](https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-querydosdevicew)
- [OBJECT_ATTRIBUTES](https://learn.microsoft.com/en-us/windows/win32/api/ntdef/ns-ntdef-_object_attributes)

DONT_REPARSE rejeita reparses durante resolução do nome. O caminho construído
obriga o parser a lidar com a seleção DOS; a rejeição observada é compatível
com esse ponto. A classe de indirecionamento existe por desenho documentado.
O status NÃO traz o componente que o originou: não afirmar prova factual de
que foi especificamente C: naquela chamada, nem declarar ancestrais NTFS livres
de reparse agora. Nenhum volume/device target atual foi observado nesta tarefa.
A causa histórica da sessão Win325 continua UNKNOWN, por falta de NTSTATUS
histórico e equivalência integral entre os processos da sessão e da observação.

Ancestrais possíveis: C:\, Projetos, PapinhoAccelerator, docs, integration,
phase6-server-update-after-utility-20261101 e o leaf do pedido. Não há na saída
raw0 evidência de junction em nenhum deles. A materialização p1-session-originals.cs
retinha cada ancestral por CreateFileW e criava o leaf relativo ao parent com
NtCreateFile/DONT_REPARSE. Seu resultado conhecido prova a coorte documental
daquela operação, não referência parent viva disponível hoje nem continuidade
ininterrupta dos mappings/ancestrais desde então.

## Correção técnica integrada

personal_scq_cold_acquisition_win32.inc: scqc_selected_name é usado pelo MESMO
scqc_open_raw, exclusivamente quando install_paths.mode=SELECTED_OBJECT(P1),
phaseREADY, selected_role1/controlador, thread própria e AMD64. O construtor
scqc_plan continua fixando propósito TRANSPORT_ONLY. Não há nova CLI/setter/owner.

Reutiliza o padrão já existente em papacc_configuration_personal_native_open:
consulta o mapping atual por QueryDosDeviceW, extrai SOMENTE a primeira string
(mapping atual, não mappings antigos), exige nome direto `\Device\<componente>`
e concatena o sufixo do caminho original a partir de `\` após a unidade.
Rejeita UNC, aliases `\??\...`/SUBST, target com subdiretórios, nome vazio,
overflow e path com componentes vazios/dot/dotdot/ADS/separador alternativo.
Não hardcode de número de volume, variável de ambiente, Shell ou OpenFileById.
Exemplo de modelo, NÃO observação do host:
`\Device\HarddiskVolume3\request.md`.

A NtCreateFile continua com os MESMOS direitos/share/options/RootDirectoryNULL,
DONT_REPARSE e OPEN_REPARSE_POINT. Não há fallback após C000050B. Se um ancestral
ou leaf do filesystem exigir reparse, a abertura continua negada. Não seguir
junction legitimamente existente por inferência de que pertence ao Windows.
Mapping não é grant/identidade: a validação de tipo/ID/owner/DACL estritos,
conteúdo/pins, proveniência/atos, referências retidas/continuidade permanece
integral nos consumers OHI. Não alterar a guarda de request/decisão/READ.

Outros modos preservam exatamente o caminho anterior `\??\...`; a correção não
é um conversor universal ou especialização de READ/NEW_INSTALL/UPDATE.
Falhas da resolução têm diagnóstico de primeira causa stage7; status de open
continua capturado stage6. Ausência/falha de consulta nega, sem busca alternativa.

A consulta do mapping ocorre ANTES da contagem original imediatamente anterior
à abertura, preservando a contabilização completa e guard124/cap128. Sem novos
handles pertencentes à aplicação, ou fechamento de internals Windows. Buffer
automático de WCHAR[MAX_PATH+4] (528bytes no target), não quota de memória nem
teto de allocator/provider/kernel. Custos nativos opacos continuam UNKNOWN.
Não alegar margem real/freshness/capacidade G3 por modelo ou por count68/113.

## Parent original versus nome absoluto e ancestrais

Se parent legitimamente adquirido/validado/retido estivesse disponível na MESMA
invocação, leaf relativo com DONT_REPARSE eliminaria nova resolução dos ancestrais.
Ele não existe como original reutilizável vindo do processo de materialização
encerrado. Não fabricar HANDLE, duplicar referência de coorte UNKNOWN ou promover
ID/string histórica a parent confiável. Abrir parent pelo mesmo nome DOS não
resolve a questão do namespace na primeira aquisição.

Validação separada é possível com a primitiva já existente scqi_chain_open:
adquirir âncora, filhos relativos unitários, conferir tipo/ID/SD/reparse e reter
até fechamento. Exige sua fonte/proteção/lifetime/recursos próprios. Não generalizar
a exceção AU0x4 do volume à árvore do workspace ou assumir que P0–P7 autoriza
seleção de qualquer parent documental.

OHI exige originais protegidos e continuidade, não a representação literal DOS
no argumento NtCreateFile. P0–P7 especializa a seleção da instalação existente,
não transforma mapping em autoridade nem certifica árvore do pedido. A mudança
é adaptação técnica de localização candidata, por mecanismo já usado no owner;
não reduz garantia normativa, não exige nova proposta/ADR. Se uma solução futura
precisar seguir reparse filesystem ou dispensar origem/ancestrais, será outra
decisão e NÃO é coberta aqui.

## Qualificação

Selected/Legacy Debug/Release:4builds PASS /W4 /WX, sem warnings.
15CTests/configuração,60pares PASS. Logs p1-native-name-<perfil>-<config>-build/tests.log.
Raw-open64cenários/240checks: mapping direto, consulta falha, alias recursivo,
target com sufixo inválido, selected_role/arquitetura divergentes, pathdotdot,
negação de reparse ainda efetiva, 123/124/128/queryUNKNOWN, Stop, firstcause,
retention/pending; perfis nãoP1 preservados. QueryDosDevice e NtCreateFile são
simulados; nenhuma coleta nativa foi executada por estes testes.

Artefato workspace Selected/Release:774144bytes,
SHA256 e1ff3510d45f546c21d486ef60fbd7a20d8a17f78ccc4338fd9310ca2084756a.
NÃO selecionado/instalado operacionalmente. Sem UPDATE, novo readback SCM ou
mudança nos originais/pins/EXEs anteriores. Imagem instalada preservada.

## Primeira lacuna e próximo passo

Falta localizar factual e individualmente o componente da observação passada;
o NTSTATUS isolado não permite isso. A correção evita depender da indireção DOS
na abertura P1, mas seu sucesso nativo continua não demonstrado.

Não preparar UPDATE/sessão imediatamente: há o mesmo padrão `\??\C:\` em
scqi_chain_capture para adquirir a âncora de OUTRA cadeia (imagem), ainda não
alcançada no resultado raw0. Não dizer que corrigi esse segundo caminho ou que
a cadeia fria completa agora passará. Antes de congelar imagem operacional,
auditar sua representação nativa dentro do mecanismo de cadeia já existente,
preservando parent original/IDs/SD/reparse/G3; não iniciar nova frente ou coleta.

Próximo prompt NEXT-P1-NATIVE-NAMESPACE-CHAIN-CLOSURE.md delimita esse fechamento
offline. Sem nova prova real, retry, E/Q/S/Start/Stop/UPDATE/SCM/ACL/CA/CURRENT,
rede/TPM/NV/Q4/cleanup/refund/recuperação. DENY, UNKNOWN histórico/OS, reservas,
autorização consumida, HEAD/WIP/históricos/pins/originais/EXEs/PST0.6.4 intactos.
