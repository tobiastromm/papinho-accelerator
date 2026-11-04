# Inventário focal no primeiro rename

Escopo: caminho UPDATE execute_mode, não preparação/SCQ. Valores derivam de
código e wrapper aprovados, não de enumeração do processo histórico. Objeto
original corresponde ao plano/IDs e é revalidado; não é identidade atual presumida.
R=FILE_SHARE_READ(1), W=WRITE(2), D=DELETE(4).

| Referência | Abertura / acesso / share | Aquisição, retenção e disposição |
|---|---|---|
| Anchor C:\ | native_open/NtCreateFile OPEN,READ_CONTROL/SYNCHRONIZE/READ_ATTRIBUTES/TRAVERSE=0x1200a0,R | antes dos filhos; bootstrap_parent no contexto; até close governado/retido |
| Program Files/Papinho slots0/1 | staged_update_open/NtCreateFile OPEN,0x1200a0,R | cadeia relativa, antes dos arquivos; IDs e SD originais retidos |
| Accelerator slot2 | mesma API,0x1200a2 incluindo FILE_ADD_FILE,R | parent original417614100:8257536:71209; retido até último readback/close; usado no RootDirectory antigo |
| Executor slot7 | NtCreateFile OPEN,READ_CONTROL/SYNCHRONIZE/READ_ATTRIBUTES/READ_DATA=0x120081,R | antes de DATA; ID417614100:1507328:540527/hash, imagem/PID/criação e SD estritos; não renomeado |
| Payload DATA slot8 | NtCreateFile OPEN,0x120081,R | após admissão executor; hash/SD/ID retidos, não executado |
| DLLs slots9/10 | NtCreateFile OPEN,0x120081,R | parent protegido, módulos correlacionados, não alteradas |
| Antigo slot13 | NtCreateFile OPEN,0x130081 incluindo DELETE,R | antes do ato; ID417614100:6225920:71237; mesma referência é o FileHandle do primeiro rename |
| Novo slot14 | NtCreateFile FILE_CREATE,0x130083 incluindo READ_DATA/WRITE_DATA/DELETE,R | após consumo; objeto417614100:1769472:537456 reportado; buffer/hash/flush usam o MESMO handle |
| Readback anterior slot11 | NtCreateFile OPEN,0x120081,RWD | só após primeiro rename confirmado; ainda AUSENTE no ponto que falhou; não pede escrita/delete, share compatível com original DELETE |
| Readback canônico slot15 | NtCreateFile OPEN,0x120081,RWD | só após segundo rename; não alcançado; originais continuam negando outros writers/deleters |
| Manager/service SCM | OpenSCManager CONNECT; OpenService QUERY_CONFIG\|QUERY_STATUS | antes da apresentação, revalidados até fim; SC_HANDLE não tem share-file; fechamento via CloseServiceHandle antes do cohort FS |
| Token original e check | OpenProcessToken TOKEN_QUERY; check fechado após comparação | bootstrap_token retido, check temporário; OpenThreadToken inesperado nega e retém se necessário; sem share-file |
| Process/imagem módulo | GetCurrentProcess pseudo; GetModuleHandle/GetModuleFileName | pseudo e HMODULE emprestados, não fechar; nenhuma duplicação de file handle para hash ou GetSecurityInfo |
| Console/Stop | GetStdHandle emprestado, console bridge existente | confirmação/Stop; não são handles ao antigo/candidato/parent; nenhum ControlService |
| Wrapper: root/ancestrais | CreateFileW OPEN_EXISTING,0x1200a0,R | retidos desde pré-chamada até retorno; parent Accelerator apenas RW, sem D |
| Wrapper: executor/DATA/DLLs | CreateFileW OPEN_EXISTING,0x120081,R | IDs/hash originais mantidos durante chamada; SafeFileHandle owns=false, stream emprestado não fecha original |
| Wrapper: antigo | mesmo OPEN/read/R | preflight de ID/hash; CloseHandle uma vez e remoção da lista ANTES de lançar executor; não coexistia deliberadamente no rename |

No ponto histórico: slots11/15 não alcançados, nenhum arquivo foi reaberto
para verificação de cópia; hashing usa handles já retidos. Buffers SD/nonce/hash
são memória, não duplicações de handles. Loader/OS/CRT/herança são opacos;
não os classificar como dispensáveis nem inventariados pela contagem176.

## Conflito próprio documentado

Microsoft descreve que RootDirectory não nulo é usado por
IopOpenLinkOrRenameTarget numa abertura relativa de FILE_WRITE_DATA|SYNCHRONIZE.
O parent slot2 mantido com share R recusa precisamente esse acesso de escrita;
isso não depende de um handle externo sem FILE_SHARE_DELETE. Wrapper parent RW
já é compatível; o conflito necessário está no parent do próprio owner.
Fonte primária: https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/ns-ntifs-_file_rename_information

A correção usa a forma documentada same-directory: RootDirectory NULL e leaf
simples. Os dois FileHandles provêm do MESMO parent original, retido e protegido;
barrier revalida parent/objetos/token/SCM/recursos antes da chamada. O rename
fica associado ao diretório do objeto, nunca ao cwd. Readbacks continuam relativos
ao parent original. Não se usa path absoluto, overwrite ou POSIX bypass.

Nenhuma referência é fechada/reaberta ou share mask ampliada. Reter arquivos
com share R permite o uso do próprio handle DELETE; impede novos opens
incompatíveis. Um reader externo sem share D impede aquisição DELETE, e um
conflito independente no rename continua DENY. O conflito próprio é demonstrado
por código/contrato API/modelo, não atestação do único fator do host histórico.

U2 exige retenção/continuidade, não RootDirectory não nulo. U3/U5 exigem dois
renames por referência sem overwrite; U6 mantém readback original. A correção
é técnica dentro desses contratos, sem nova decisão normativa. Lifetime e
tratamento de UNKNOWN não mudam. Em effects&&first, staged_update_close conserva
cohort em phases8/unresolved, inclusive após sharing violation síncrono.
