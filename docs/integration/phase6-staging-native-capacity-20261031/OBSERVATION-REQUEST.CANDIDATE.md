# Pedido de uma coleta manual de recursos — staging sem efeitos

Estado: READY_FOR_OWNER_DECISION, NÃO APROVADO, NÃO EXECUTADO.
Este pedido não autoriza staging nem constitui admissão de capacidade.

## Artefatos byte-exatos

Script: `collect-staging-resources.MANUAL.ps1` neste diretório.
SHA256: `1935b7dd349193f288f30b3e9ee99f0f88389d8b3fa1f0e0473e4df3cb498f67`.
Validado somente por parsing estático; não executado, inclusive sem elevação.

Fonte fixa dos três arquivos públicos:
`C:\Projetos\PapinhoAccelerator\build\phase6-functional-20261028\Selected\Release`.

| Arquivo | Bytes | SHA256 |
|---|---:|---|
| papacc_server.exe | 552960 | 74f430e0bd86529396bd267c8ca40548218dc83611ef126c24e72da70388e8ad |
| libssl-3-x64.dll | 1321472 | 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed |
| libcrypto-3-x64.dll | 7217664 | 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044 |

Total9.092.096 bytes. O servidor mudou por implementação/dispatch de observação;
não reutilizar o pin anterior. SDK normal/API3 qualificador0.6.5; não release,
imagem instalada ou promoção de PST0.6.4. Hash de disco não é atestação de imagem
mapeada nem autoridade de instalação. Este pedido fixa esses bytes exclusivamente
para observação; não pede seleção intermediária para execução de staging.

## Operação proposta

Uma invocação manual pelo proprietário legitimamente administrador local, em
Windows PowerShell5.1 Desktop x64 já elevado. Sem UAC automático ou mudança de
ExecutionPolicy/ACL. O script confere seu hash e os três tamanhos/hashes ANTES
da única chamada ao exe com seletor privado read-only
`--qualification-staging-observe`. Nenhum comando de invocação é entregue antes
de aprovação explícita deste pedido; os valores do script já estão preenchidos.
Não há aprovação persistida, senha, segredo ou valor operacional futuro inventado.

O exe reutiliza o mesmo owner/contexto administrativo e captura token/SID e
originais durante a invocação. SID de sandbox, operador histórico ou conta SCM
futura não preenchem esse binding. Elevação isolada não autoriza staging.

Escopo solicitado: carregar esse exe/DLLs e observar o próprio controlador;
resolver known folders; abrir o volume e ancestrais de ProgramFiles já existentes;
ler seus metadados de identidade/proteção; abrir/ler/hash os três arquivos
públicos; consultar recursos próprios/sistema e espaço do volume por handle;
fechar referências controladas e emitir evidência não secreta no console.
Não abre/cria/enumera `Papinho\Accelerator`, não exige ProgramData ou serviço.

As aberturas são FILE_OPEN, não FILE_CREATE. Reutiliza o access profile do
preflight: token QUERY; fontes READ_DATA/READ_ATTRIBUTES/READ_CONTROL/SYNCHRONIZE;
diretórios query/traverse/list/read-control/synchronize. O helper original ainda
solicita ADD_SUBDIRECTORY no volume/parent final para verificar elegibilidade
do contexto, mas o modo de observação não usa esse direito para criar nada.
Nenhum WRITE_DAC/WRITE_OWNER, takeover ou acesso a chaves/credenciais.

## Evidência e limites

Cinco frames saudáveis: entrada após loader/zeroing, ancestrais originais,
fontes/hash/volume, antes do close, depois do close conhecido. Handles próprios,
PrivateUsage/WorkingSet, físico e commit disponíveis, QPC, quota/espaço do volume,
unidade de alocação, projeção de payload arredondada por arquivo, comprimentos
de descritor observados, operador, hashes e estados/primeira falha. A origem
continua SNAPSHOT, não grant, reserva ou garantia futura. Não ler material privado.

Primeira localização:1 orçamento/primeiro frame;2 layout/console;3 contexto
nativo/descritor inicial;4 aquisição de ancestrais;5 proteção dos ancestrais;
6 fontes/hash;7 revalidação;8 volume;9 frame após fontes;10 close incerto.
Predicado/erro nativo do contexto é reportado separadamente. Uma negação mantém
sua primeira causa; não deve ser contornada para alcançar os frames posteriores.

CallerAvailableAllocationUnits é relativo à quota do caller e pode diferir do
espaço global. Usa-se NtQueryVolumeInformationFile/FileFsFullSizeInformation
sobre o handle original; nenhum path de filho inexistente precisa ser criado.
Referências oficiais: [consulta](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntifs/nf-ntifs-ntqueryvolumeinformationfile)
e [estrutura](https://learn.microsoft.com/en-us/windows-hardware/drivers/ddi/ntddk/ns-ntddk-_file_fs_full_size_information).

Custos ativos de criação/escrita/flush, metadados do FS, allocator/CRT/kernel,
cohort administrativo e margem/reserva admissível não são provados por esta
coleta. O wrapper PowerShell e seus contextos internos de hashing não são o
processo nativo medido. Picos amostrados não são ceilings; ausência de observação
não é zero. Não usar a saída para afirmar fechamento interno do OS.

## Efeitos, Stop e condições de interrupção

Um processo nativo no contexto já elevado; leituras e consultas. Nenhuma mutação
de instalação solicitada, RNG, confirmação/take de staging, arquivo de evidência
gravado pelo script, serviço, endpoint, conta, CA, CURRENT, KSP, TPM/Q4 ou rede.
Loader/cache/antivírus/metadados de acesso do OS podem ter efeitos internos e
custos opacos: não são declarados inexistentes ou limitados por esta aprovação.

STOP em pin/tamanho/contexto/proteção divergente; fonte/consulta indisponível;
guard124 excedido; memória esgotada; projeção de disco não satisfeita; dado
inconsistente/overflow; NT pending; Stop/perda de continuidade; close incerto.
Preservar128/1024 nas respectivas aplicabilidades, sem omitir participantes.
Não relaxar guarda, tentar outro caminho, elevar de novo ou repetir a coleta.

UNKNOWN retém os objetos/backings no owner enquanto a invocação existe; não se
afirma persistência dessas referências após término do processo. Interrupção ou
ausência de receipt completo é UNKNOWN, não close/rollback provado. Não há retry,
cleanup/refund ou recuperação autorizados. O Stop do OS pode terminar o processo;
não confundir término solicitado com quiescência controlada demonstrada.

Exit0 significa COLLECTED_NOT_ADMITTED; autoridade e APPLY continuam DENY.
Reconciliar a saída integral, primeira falha, frames alcançados, máscaras UNKNOWN
e fechamento antes de qualquer próximo pedido. Nem este pedido nem sua eventual
aprovação permitem staging, registro SCM, Start/Stop ou instalação.
