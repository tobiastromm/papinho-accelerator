# Primeiro rename — conflito próprio corrigido offline

Task PACC-PHASE6-FIRST-RENAME-SHARING-SEMANTICS,2026-11-01.
main/HEADf341985924d29870d8ff627ed37c8a0874ad6469; WIP/históricos preservados.
Nenhuma operação de produto, coleta, rename, UPDATE ou leitura do alvo real
executada nesta tarefa. Estado persistente atual dos objetos não presumido.

## Causa e correção

O owner original mantém Accelerator com FILE_ADD_FILE(0x2)/share READ, passando
o mesmo handle como RootDirectory de FileRenameInformation. Microsoft descreve
que o I/O manager abre o diretório alvo para FILE_WRITE_DATA|SYNCHRONIZE nessa
forma. Essa abertura conflita com o share READ retido pelo próprio owner.
Isso demonstra um defeito de compatibilidade próprio, sem imputar a terceiros.
Referência primária e inventário completo em [HANDLE-OWNERSHIP.md](HANDLE-OWNERSHIP.md).

Correção mínima em server_launcher_staged_update_effects_win32.inc: usar leaf
simples com RootDirectory NULL, forma documentada de rename dentro do MESMO
diretório. Fonte do rename é o handle original do antigo/candidato, ambos no
parent retido; nomes restritos sem separadores/colon/dotdot. Bit e slot exatos
também validados. Parent/SD/identidades/Stop/SCM/recursos continuam revalidados
pelo barrier antes de cada chamada, readback relativo ao parent original depois.
Não usa cwd, novo path absoluto, overwrite, POSIX bypass ou rollback.

Nenhuma share mask foi ampliada e nenhum original fechado antecipadamente.
Não depende de liberar DELETE em arquivos/parent nem trocar lifetime de cohort.
U2/U3/U5/U6 accepted preservados: referências originais, consumo antes dos
efeitos, dois renames sem overwrite, continuidade/Stop/readback/UNKNOWN.
Não houve necessidade de nova proposta ou alteração de documento accepted.

É causa estrutural demonstrada por código/documentação e modelo; não é prova
nativa de que fosse o ÚNICO fator do host na execução histórica. Referências
externas/filtros continuam possíveis e devem permanecer fail-closed, não ser
ignoradas. A execução histórica permaneceu first0xC0000043/location701,
created1/consumed1/renames0/close_known0, sem sucesso de UPDATE.

## Qualificação

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings.
16 testes afetados por configuração,64 pares PASS. UPDATE106 cenários/634
verificações; preparação53/295 e entrada20 preservadas. APIs de FS/RNG/SCM e
operações de teste modeladas; nenhum endpoint, rename ou produto nativo executado.

Regressão nova modela a abertura interna de escrita e rejeita a forma histórica
RootDirectory=parent. Compara read-handle sem share DELETE versus compatível,
e parent RW compatível no modelo sem aplicá-lo à produção. Cenários novos
169/170 injetam sharing violation independente no primeiro/segundo rename,
preservando retenção e sem completar o fluxo;171 injeta Stop no flush antes
de rename. Sucesso existente preserva dois renames/readbacks originais.
Casos anteriores mantêm falhas intermediárias, pending, close UNKNOWN,
readback divergente e recusa de replay/consumo duplicado. Modelos demonstram
tratamento, não identificam handle externo nem capacidade operacional.

Preimages de ambos os arquivos alterados e logs de builds/testes preservados.
K_UPDATE Selected2027314/Legacy1858746 inalterado; sem crescimento de packet,
novos handles ou quota. DLLs idênticas. Não repetir campanhas inalteradas.

## Delta e limites operacionais

Artefatos recompilados de workspace, NÃO selecionados/instalados por esta tarefa:

| Selected/Release | Bytes | SHA256 |
|---|---:|---|
| papacc_server.exe |758272|841e9f8c2d6c877c9b415e51d3976e6022094bd21b8ace69e2377008a6fcaed4|
| papacc_update.exe |104448|be024e0bf0dbf5ee93fd4dde0bf0124233d0735d59f89488a57736e2a2817510|

Utilitário protegido histórico6c0e7717…0bb87 NÃO foi atualizado e continua
sem a correção demonstrada no workspace. Não selecioná-lo para retry por
reflexo. Disponibilizar utilitário corrigido exigirá caminho governado e
autoridade próprios; U1 UPDATE somente server/U2 proíbe autossubstituição do
executor vivo. Essa tarefa não instala nem concede efeito adicional para isso.
Pins de pedidos/autorização anteriores permanecem históricos e consumidos,
não substituídos silenciosamente por estes hashes novos.

## Primeiro gate restante

Reconciliação factual read-only da disposição deixada pela tentativa anterior:
canônico, candidato exato, previous planejado, executor e parent, com originais,
bytes/SD/hash/readback/close. Escopo mínimo em [READONLY-DISPOSITION.SPEC.md](READONLY-DISPOSITION.SPEC.md).
Não enumerar tabela de handles/tracing como pré-requisito de corrigir defeito
próprio. Observação não autoriza limpar/adotar candidato, liberar créditos,
rearmar ato ou provar quiescência universal. Retenção UNKNOWN permanece até
disposição governada suficiente; não há pedido de retry pronto.

Próximo prompt integra preparação dessa observação e explicita a disponibilidade
da versão corrigida do executor, sem criar infraestrutura. Só depois de resolver
disposição/autoridades específicas é possível retomar caminho UPDATE→Start
diagnóstico separado. Nunca repetir autorização utilizada.

E21 PENDING; ED11/Phase6 PARTIAL; produção DENY; OS_QUIESCENCE UNKNOWN.
Preservar objetos anteriores/candidato, DLLs, SCM/ProgramData, reservas,
chave fixture, históricos/pins, HEAD/WIP e PST0.6.4. Sem cleanup/refund/rollback.
