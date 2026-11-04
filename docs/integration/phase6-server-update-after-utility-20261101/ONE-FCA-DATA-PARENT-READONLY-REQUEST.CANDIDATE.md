# Pedido operacional — leitura focal do parent FCA

CANDIDATE / MANUAL_ONLY / NOT_AUTHORIZED_NOT_EXECUTED. 2026-11-03.

Solicita UMA observação manual somente leitura, no host TCORP-TOBIAS, Windows PowerShell 5.1 AMD64 e console original do administrador aprovado. Não é nova sessão FCA ou E/Q/S/P e não reutiliza aprovação consumida. FCA0–FCA7 autorizou integração offline; a execução desta leitura exige decisão específica do proprietário.

## Necessidade factual

O UPDATE FCA já terminou LIMITED_UPDATE_VERIFIED_CLOSED. A primeira lacuna é a aquisição legítima do parent que o caller deriva de ProgramData KnownFolder acrescido de Papinho. Não há identidade atual comprovada desse parent. A evidência histórica readonly-constituents-result.json de 24/10 contém um SD de ProgramData que a política estrita atual rejeita: ACE4 BUILTIN Users mask0x116. Essa projeção foi reproduzida offline e não prova ACL atual, causa histórica de outra tentativa ou ausência de Papinho. Preparar uma criação agora presumindo esses gates seria incorreto.

## Artefatos congelados

- `fca-parent-readonly.cs`: SHA256 `daa881d7b9ac370371d4d1bf714ab6ca1ab3bace389ef685c78cdb1ec5fbcfa2`.
- `observe-fca-data-parent.MANUAL.CANDIDATE.ps1`: SHA256 `5c27d2fdacc319d4c0b6c6d5a0b73b54ee9f4f76fdf12ec6645ac772bdd951be`.
- Modelo: `fca-parent-readonly.model.cs`; qualificação CodeDom .NET Framework existente, 63 checks PASS, native IO substituído, nenhuma chamada do adapter nativo.
- Histórico de entrada: `../n0start03-protected-dossier-qualification-20261024/readonly-constituents-result.json`, SHA256 `92db5967c7dadbb867f03d53850229bccc72b68ae4aecc0f77520b2c15543022`.

## Escopo da única chamada

Verificar pins de procedimento/pedido/código, host, arquitetura, console, não impersonação e SID administrativo esperado S-1-5-21-785699550-1440458426-1682014700-1108. Compilar o adapter em memória pelo mecanismo já existente, sem carregar o EXE instalado. Exigir confirmação manual exata `OBSERVE_FCA_DATA_PARENT_ONCE` antes de resolver/abrir a cadeia. Uma execução consumida não pode ser repetida; cancelamento não concede retry automático.

Resolver uma vez FOLDERID_ProgramData, KF_FLAG_DONT_VERIFY=0x4000, token NULL. Acrescentar somente a folha fixa Papinho. Sem ambiente/registro/fallback/hardcode como fonte de autoridade. Aceitar apenas caminho local ASCII drive absoluto, componentes não vazios, sem ponto/dotdot/ADS/reparse, máximo oito diretórios incluindo volume. QueryDosDeviceW da unidade fornece localização candidata direta Device; o prefixo não concede autoridade.

Abrir volume nativo e componentes sucessivos, relativos ao original imediatamente anterior, por NtCreateFile: access0x001200a0 (READ_ATTRIBUTES|TRAVERSE|READ_CONTROL|SYNCHRONIZE); attrs0x80; share1; disposition1 FILE_OPEN; options0x00200021; OBJECT_ATTRIBUTES0x1040 (CASE_INSENSITIVE|DONT_REPARSE). Nunca FILE_CREATE, ADD_SUBDIRECTORY, escrita/DELETE/WRITE_DAC/WRITE_OWNER. Manter as referências até o final. Validar tipo disk/directory, ausência reparse e flags do handle. Capturar identidade e OWNER|DACL pelo mesmo padrão NT de sizing/fill uma vez, máscara5, máximo65536 bytes, sem fallback/resize. Revalidar uma vez os mesmos objetos por identidade/atributos e hash byte-exato do SD antes de fechar.

Projetar somente a política existente de owner confiável, DACL protegida e bloqueio conservador de escrita por trustee não confiável; exceção AU self-only0x4 restrita ao volume. Nenhuma exceção ProgramData. Emitir por componente ID, hashSD, match/rejeição e índice/máscara/papel da ACE; sem exportar dados de arquivo, chaves, SD bruto ou trustees desconhecidos. Uma rejeição de política é DATA da observação: registrar e continuar apenas a leitura fixa até Papinho, sem admissão ou efeito mutante. Falha de abertura/consulta, Stop, troca de identidade/SD ou formato inválido encerra a leitura sem retry. Exportar primeira falha e NTSTATUS bruto da abertura, não erro5 inventado.

Não abrir nem criar Accelerator, source de configuração, EXE, DLL, chave ou objeto SCM. Não enumerar diretórios ou outros alvos. Parent ausente deve aparecer como falha nativa, não ser criado. Sem alteração de ACL, privilégio, guard ou contrato. Sem UPDATE/endpoint/SCQ/StartStop/CA/CURRENT/rede/TPM/limpeza/refund/kill.

## Limites de equivalência, recursos e lifetime

Processo/token/CRT/console/Shell são diferentes do owner FCA. A leitura não reproduz toda a aquisição FCA: não chama ProgramFiles KnownFolder, não solicita ADD_SUBDIRECTORY0x4 no parent completo, não valida source nem mede capacidade/envelope FCA, não testemunha ausência do futuro filho ou autorização de criação. A projeção gerenciada foi confrontada com a política existente e com o SD histórico, mas não substitui a função nativa na futura sessão. Até um resultado todo match não autoriza FCA nem prova continuidade após o fechamento.

Até oito handles próprios de diretório simultâneos; OA/UNICODE/IOSB/nome/slot de handle mantidos durante a chamada e retidos se retorno pending/inconclusivo. Uma consulta sizing e uma fill por SD por passagem, duas passagens no máximo. Buffers SD até64KiB/call; até16 capturas gerenciadas (coleta GC não é teto de memória nativa). Custos CLR/console/Shell/hash/kernel/latência e handles internos UNKNOWN. CoTaskMemFree tem retorno void, sem prova de allocator-close mais forte. Não impor nem inferir PASS dos limites FCA128/188/192 a partir deste processo diferente; nenhum limite é alterado.

CloseHandle próprio uma vez por referência em ordem reversa. Pending, handle em retorno inconclusivo ou close incerto retém o adapter/backings e a janela pelo padrão de contenção existente; não mata nem libera backing por timeout. Não fechar handles internos Windows. Não garantir duração máxima nativa; Esc é Stop cooperativo entre chamadas, não cancela uma chamada pendente. Retenção histórica e quiescência OS UNKNOWN.

## Reconciliação e próximo efeito

Ler a saída integral, registrar consumo, primeiro erro, cada componente e revalidação, policy projection, close/retention. Se houver rejeição atual de ancestral, não materializar source nem criar parent/filho: apresentar somente a decisão focal necessária. Se tudo estiver elegível, preparar o pedido próprio de materialização do original mínimo e FCA, com reaquisição/proteção/continuidade frescas no próprio owner; não reutilizar estes handles fechados como originais futuros.

Readiness produtiva DENY. P4/K5 não qualificados. Históricos, pins, EXEs anteriores, autorizações consumidas e PST0.6.4 preservados.