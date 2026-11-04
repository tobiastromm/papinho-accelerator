# UPDATE direto — arquivos versus diretórios

2026-11-01. Revisão somente; nenhuma aceitação normativa ou execução.
A direção de preparar distribution-42d468faf367 foi interrompida pelo
proprietário. Não implementar nem solicitar criação dessa distribuição.
TRUSTED-SOURCE-CLOSEOUT.md fica como análise histórica, não plano ativo.

## Comparação objetiva

| Aspecto | EXE individual, DLLs idênticas preservadas | Pasta nova ao lado e troca de diretórios |
|---|---|---|
| Compatibilidade atual | U1/U3/U5/U6 já definem essa substituição | Não coberta pelo escopo atual U1/U3/U5/U6 |
| Criações planejadas | Um arquivo candidato | Um diretório e três arquivos, no mínimo |
| Renames planejados | Antigo EXE para retenção; candidato para canônico | Pasta antiga para retenção; pasta nova para Accelerator |
| Bytes novos do pacote atual | 737280 | 9276416 |
| DLLs canônicas | Objetos preservados, só validar | Novos objetos com bytes idênticos; anteriores retidos |
| Identidade do diretório canônico | Preservada | Substituída; parent de renames passa a ser Papinho |
| Espaço adicional | Novo EXE arredondado no volume | Três arquivos arredondados individualmente e metadados de diretório |
| Falha entre renames | EXE canônico pode ficar ausente | Toda a pasta canônica pode ficar ausente |
| Retenção | EXE antigo | Árvore antiga inteira, com conteúdo a inventariar |
| Rollback/exclusão | Não autorizados | Também não autorizados; reter não autoriza apagar depois |
| Reutilização de código | Composição UPDATE já integrada | Primitivos reutilizáveis; composição de diretórios ainda não qualificada |

Hashes do pacote de planejamento foram conferidos na tarefa anterior; não
selecionam artefato diferente nem demonstram proteção atual do host. Novo EXE
42d468faf367447ac8416194cadb5d2b0474c91aa3b7953838b310e65b0f29ca;
DLLs 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed e
09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.
R real depende do arredondamento fresco; bytes lógicos não são reserva de disco.

## Especialização estritamente necessária à opção de diretórios

- U1: permitir copiar DLLs, embora idênticas, e trocar o conjunto binário;
  a norma atual diz atualizar apenas EXE e nunca copiar DLLs idênticas.
- U2: conservar referências do parent Papinho e das duas árvores; vincular
  explicitamente a identidade nova ao mesmo caminho canônico, sem converter
  isso em mudança da identidade administrativa/CA/dados da instalação.
- U3: autorizar candidato de diretório irmão, três arquivos e renames de
  diretórios relativos ao parent original, exclusivos e sem overwrite.
- U4/U5: censar coexistência e novos objetos; ampliar somente essa multiplicidade
  e definir R do pacote inteiro. Não reutilizar K/R do EXE ou tolerâncias B3.
  Confirmação/consumo antes da primeira criação, não só antes dos renames.
- U6: readback do novo diretório e de cada arquivo, da árvore anterior retida,
  proteção/identidade e SCM ainda parado; renovar os bindings de imagem/parent
  exigidos pelo Start posterior a partir dos objetos efetivamente observados.
- U7: qualificar falhas por fase, Stop, conflito, readback e fechamento antes
  de novo pedido e autorização específicos. Nenhuma execução inferida.

Não é necessário mudar SCM, criar serviço/owner/root/canal, conceder Start,
afrouxar ACLs existentes ou autorizar exclusão. Tampouco é preciso modificar
os contratos SCQ/deadline ou reabrir o risco opaco já accepted para o efeito
EXE. Se for escolhida troca de pastas, a aplicação desse risco à composição
maior deve ser explícita, não presumida por reuso dos mesmos helpers.

Não presumir que toda a árvore atual contenha somente os três arquivos
conhecidos. Conteúdo adicional, dados ou referências não inventariadas impedem
troca cega da pasta. Não há inventário novo nesta tarefa. Compatibilidade de
rename de diretório com handles abertos/compartilhamento exige qualificação:
a implementação atual de rename de arquivo não prova essa compatibilidade.
Dois renames não formam uma transação; falha entre eles retém ambas as árvores
na disposição factual, sem completar, desfazer, deletar ou tentar outro leaf.
SERVICE_STOPPED/PID0 é gate fresco, não fencing contra administrador externo.

## Opção ainda menor e recomendação

Recomendo o UPDATE individual já integrado: criar o novo EXE protegido no
diretório original, copiar/verificar/hash/flush, revalidar serviço parado e
originais, renomear o anterior para retenção e o candidato para o nome canônico,
readback/fechamento. Preservar as duas DLLs porque os bytes são idênticos.
É a menor alteração funcional e a menor superfície de falha do pacote atual.

Sobrescrever o EXE diretamente é mais curto como comando, mas viola U3:
perderia a preservação do objeto antigo e poderia expor conteúdo parcial.
Copiar o antigo e depois sobrescrevê-lo tampouco equivale à preservação da
identidade original exigida. Não recomendar Copy-Item -Force, ReplaceFile,
remoção de Accelerator ou exclusão posterior como atalhos autorizados.

Se DLLs precisarem mudar em outro pacote, a opção de pasta pode ser útil
para verificar um conjunto antes da troca. Essa situação não foi demonstrada
para este pacote e não justifica ampliar o efeito agora.

## Origem do executor e caminho direto, sem distribuição adicional

A recomendação anterior de criar uma distribuição protegida não era exigência
literal de U2. Retiro-a como próximo passo obrigatório. U2 exige propriedades
de identidade/proteção, não uma nova pasta ou subsistema de distribuição.

Contudo, renomear arquivos ou pastas não soluciona a falha108: o pedido anterior
executava o mesmo EXE do workspace como controller e payload. O subpredicado
histórico permanece UNKNOWN; o snapshot atual não prova o descritor passado.
Não chamar novamente o mesmo controller esperando que a troca de alvo o legitime.

O caminho direto deve resolver, dentro do procedimento administrativo UPDATE,
duas responsabilidades distintas: (1) executor/owner legítimo e política
previamente definida; (2) entrada de bytes candidatos, capturados por referência,
retidos contra escrita/substituição, vinculados aos pins selecionados e copiados
para o candidato protegido. Hash ou token elevado não substituem (1).

Se a solução usar bytes de workspace como DATA sem exigir seu owner/DACL
estrito prévio, isso exige especialização focal de U2 somente para a entrada
DATA. Deve preservar a confiança independente no executor, captura/continuidade,
proteção contra substituição e validação do candidato ANTES do primeiro rename.
Não é autorização presente para relaxar slot8, nem torna o mesmo EXE recusado
confiável por redefini-lo como DATA. Não há executor alternativo qualificado
demonstrado no caminho atual. Um procedimento baseado em ferramenta Windows
também precisaria comprovar a integridade/proveniência do procedimento e sua
associação administrativa; confiar apenas no nome powershell.exe não basta.

Para o UPDATE individual, nenhuma especialização de troca de diretórios,
cópia de DLLs ou exclusão é necessária. A única questão normativa potencial
restante é essa separação executor/entrada DATA, se a origem existente não
puder satisfazer U2. Não criar outra ADR ou proposta nesta revisão. Não
declarar que uma mudança de path resolveu a confiança do controller.

Sequência mínima: resolver o executor e o papel da entrada no mesmo owner;
qualificar apenas esse delta se houver; congelar pacote/artefato; emitir novo
pedido de um EXE/duas renomeações com gates U1–U7; obter autorização específica;
após resultado qualificado, preparar o Start diagnóstico separado. Se o
proprietário preferir pastas, aceitar antes somente a especialização delimitada
acima e qualificar a composição maior, sem infraestrutura de distribuição.

Estado: pedido novo NOT_READY; autorização anterior utilizada, sem retry.
Nenhum código alterado, build/teste inalterado repetido ou efeito operacional.
DENY; OS_QUIESCENCE UNKNOWN; E21 PENDING; ED11/Phase6 PARTIAL; PST0.6.4,
HEAD/WIP, históricos/pins e EXE anterior preservados.
