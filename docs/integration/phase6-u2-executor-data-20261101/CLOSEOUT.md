# U2 aceita — separação executor/DATA integrada offline

2026-11-01. Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469.
Estado técnico: INTEGRATED_QUALIFIED_OFFLINE. UPDATE operacional NOT_READY/DENY.
Não foi criado controller, serviço, distribuição ou outro owner/root/canal.

## Decisão e compatibilidade

Proposta original SHA25658493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da
conferida integralmente e preservada. Registro normativo precedeu código:
[decisão accepted](../../adr/DECISION-personal-update-executor-data-u2-20261101.md).
Só a origem DATA deixa de exigir a allowlist do executor; todas as outras
guardas U1–U7/SD/SCM/Stop/readback/retidos permanecem. Não é o patch genérico
IsValidSecurityDescriptor rejeitado anteriormente: este exige primeiro um
executor original independente, além de interpretar owner, DACL e cada ACE.
Revisão dos future docs pessoais de listener/backup não identifica direção de
UPDATE conflitante ou implementação correspondente a atualizar nesta tarefa.

## Integração efetiva no mesmo owner

`personal_staged_update_win32.h`: referências/seleção do executor distintas de
source_path/new_hash, com ID, hash, PID/criação e path capturado no packet.
Nenhum formato persistido/público foi criado. Slots existentes de objetos são
reutilizados; slot7 é reservado ao executor apenas quando não ocupa um ancestral.
Se não houver slot seguro (ancestor_count>7), nega; não aumenta quota nem descarta
ancestral obrigatório. O caminho conhecido atual usa três ancestrais.

`server_launcher_staged_update_win32.inc`: antes da aquisição DATA no caminho
mutante, obtém a imagem do próprio executor e a abre relativamente ao parent
protegido já retido, com guarda estrita de SD/owner/DACL, sem escrita/delete.
Esta integração deliberadamente só admite controller independente diretamente
no mesmo parent instalado, reaproveitando sua cadeia original; não abre outro
root/path de distribuição para aceitar o workspace. Verifica ID/hash selecionados,
PID/criação, contexto/token nativos e correlação nome nativo do processo versus
handle original da imagem. Verifica módulos SSL/crypto contra as referências
originais protegidas das DLLs no destino; nenhum carregamento a partir de DATA.

Essa associação é equivalente em alcance à correlação de processo/imagem dos
adapters existentes: soma arquivo original/proteção/hashes/contexto à comparação
de nomes, não é somente QueryFullProcessImageName. NÃO prova todas as páginas
mapeadas nem continuidade anterior à aquisição. Não afirmar que corrigiu a
confiança do loader ou qualificação/proveniência prévia de um artefato desconhecido.
O futuro pedido deve selecionar um executor com evidência legítima de qualificação
e proteção independente; hoje isso não existe demonstrado.

O arquivo executor não pode ser origem DATA, EXE anterior, DLL, candidato ou
outro alias do cohort. Isso inclui self-update do executor vivo, rejeitado
expressamente pela proposta aceita. A referência estrita do executor é
revalidada antes dos efeitos e quando a fonte DATA é validada. Não há callback
de aprovação, setter positivo ou metadado externo capaz de preencher a prova.

Slot8 DATA: FILE_OPEN read-only/share READ/OBJ_DONT_REPARSE; tipo regular,
volume, identidade, tamanho/mtime, owner/SID, DACL presente/não nula, ACL e
ACEs/SIDs interpretáveis. ACEs não suportadas negam. Owner estrangeiro/ALLOW
de escrita pode ser observado SOMENTE nesse slot e depois do executor admitido.
Descritor completo é digerido e revalidado pelo mesmo handle; sem reabrir path
para copiar. Fonte é hasheada antes do plano; candidato tem hash independente
verificado após cópia/flush e antes do primeiro rename. Escritor incompatível,
troca de identidade/SD ou hash divergente nega; share não é defesa contra kernel
ou administrador comprometido/mapeamento anterior.

`server_launcher_staged_update_effects_win32.inc`: ato/plano agora congela
política U2, executor e suas referências/seleção/PID/criação, além do DATA e
originais anteriores. Presenter mostra esses papéis separados e seu alcance.
Consumo antes de criação; cópia/flush, dois renames exclusivos, readback,
Stop/concorrência e retenção sem rollback continuam no caminho original.
CLI privado exige a seleção independente de ID/hash do executor (17 argumentos
no modelo atual); os 13 argumentos do handoff utilizado não admitem a operação.
Nenhum comando operacional novo é fornecido.

O caminho read-only `staged_update_prepare` preserva a política histórica estrita,
sem consumo ou redução DATA automática. Nenhuma alteração em request/decisão,
READ, SCQ, staging exclusivo ou registro SCM foi feita.

## Controller instalado: avaliação e primeiro bloqueio

Foi feita somente leitura estática do EXE instalado, com FileStream read-only e
share READ, sem executar o binário, consultar SCM ou alterar filesystem. Captura:
[INSTALLED-STATIC-CAPACITY.json](INSTALLED-STATIC-CAPACITY.json).

Resultado:571392bytes/SHA256c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf,
o mesmo artefato histórico. Não contém os literais `--owner-manual-binary-update`,
`BINARY_UPDATE_CLOSED` ou a política U2. A ausência de strings isolada não é
prova formal de impossibilidade de qualquer programa; conjuntamente ao histórico
de build/caller e pedido anterior que afirma a ausência do UPDATE nesse EXE,
não há capacidade desse caminho demonstrada. Não o executar para experimentar.

Além disso, reutilizar esse mesmo objeto como executor do UPDATE do próprio
EXE contraria a restrição explícita de independência/self-update aceita. Mesmo
uma revisão antiga capaz não satisfaria essa relação de papéis. Por isso não
se repetiram consultas de ACL/token/SCM para tentar promover o mesmo objeto.
Sua proteção/identidade histórica não constitui nova prova original nesta tarefa;
a leitura estática não adquiriu file ID nativo nem demonstrou continuidade de
processo. Nenhum outro executor protegido e capaz foi identificado/qualificado.

Primeiro bloqueio concreto: falta um executor independente protegido capaz,
legitimamente obtido, com proveniência e referências originais. No caminho
integrado o workspace é recusado antes da admissão DATA; não há fallback. A
aceitação U2 separa os papéis, mas não provisiona o executor inexistente nem
autoriza transformar o próprio candidato em controller. Pedido corrigido:
NOT_READY. Não há solicitação operacional completa ou comando legítimo agora.

## Builds, testes e limites

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings;14 CTests
afetados por configuração, total56 pares distintos PASS. Evidências em
BUILD-TEST-SUMMARY.json e *-build/tests/details.log. Teste UPDATE:102 cenários,
602 verificações/configuração (33 preparação,69 efeito/roles), contra77/382
anteriores. Execuções de desenvolvimento não são contadas como requisitos extras.

Cobertura inclui DATA com owner estrangeiro/ALLOW de escrita somente depois
de executor protegido, ausência de seleção/confirmacão, executor não protegido,
workspace, imagem/processo/dependências divergentes, hash errado, alias/hardlink,
self-update, DACL nula/ACE não suportada, escritor incompatível, reparse,
mudança de SD/identidade/processo/token, cópia/flush/renames/readbacks parciais,
replay/consumo duplicado, Stop e fechamento UNKNOWN. Controllers positivos dos
fixtures são MODELOS, não controllers instalados ou fontes operacionais reais.
As APIs novas de processo/imagem e as mutantes/SCM/RNG são substituídas no teste;
nenhum endpoint, Start/Stop ou UPDATE real é executado. SHA256 e parsing de SD
são reais sobre dados dos fixtures. A leitura estática acima é separada dos testes.

| Perfil | Known | K UPDATE | Cap gerido |
|---|---:|---:|---:|
| Selected |2001376|2027282|3145728|
| Legacy |1832808|1858714|2097152|

K cresceu1896bytes:576 do estado privado e1320 de charge adicional conservadora
nomeada para frames/buffers/processo/SD do executor. Nenhuma quota mudou.
Contagem de referências inclui o executor; ele reutiliza um slot já reservado,
não uma franquia de handles. Censos são composição/sizeof, não picos nativos,
capacidade do host ou prova de custos opacos; U4/UNKNOWN continuam aplicáveis.

## Delta e continuação

Selected/Release qualificado offline:740352bytes,
SHA256021c7e5cbc697d3bf5c2e4676cb19e61aadcdcfbff36117219490d9ff2c426dd.
É candidato de build, não controller protegido/seleção operacional/EXE instalado.
Delta3072bytes para o build737280/42d468…29ca; delta168960 para o instalado.
DLLs permanecem com os pins3fb3cd78…/09eec573…; não foram copiadas ou modificadas.
Não selecionar o novo build para operar enquanto faltar o executor.

Próximo prompt: [NEXT-INDEPENDENT-EXECUTOR-AVAILABILITY.md](NEXT-INDEPENDENT-EXECUTOR-AVAILABILITY.md).
Não criar distribuição nem controller auxiliar por inferência. Primeiro resolver
essa dependência factual/autoridade específica, reutilizando alternativas reais
existentes se houver; depois seleção/pedido UPDATE novo, e só após resultado
qualificado tratar endpoint e Start diagnóstico separadamente. Não repetir
staging ou registro SCM, não reabrir a especialização DATA já accepted.

HEAD/WIP, históricos/pins, EXE anterior, DLLs, ProgramData, configuração SCM,
fixture/reservas e PST0.6.4 preservados. Autorização baa501…4726 continua utilizada;
consumed0 histórico não a rearma. OS_QUIESCENCE UNKNOWN; E21 PENDING;
ED11/Phase6 PARTIAL; autoridades produtivas DENY.
