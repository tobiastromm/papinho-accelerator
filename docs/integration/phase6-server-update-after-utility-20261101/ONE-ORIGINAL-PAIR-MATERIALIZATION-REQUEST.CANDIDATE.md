# Pedido operacional candidato — duas cópias documentais protegidas

Status: CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. Uma execução MANUAL, sem retry.
Escopo exclusivo de materialização documental PERSONAL_LOCAL. NÃO aprova nem
executa endpoint, SCQ, Start/Stop ou qualquer chamada ao executável do produto.

## Operador, fontes e destinos exatos

Operador esperado S-1-5-21-785699550-1440458426-1682014700-1108,
host TCORP-TOBIAS. Elegibilidade/contexto administrativo e ausência de
impersonação serão verificados nativamente na execução; metadados não autorizam.

Parent existente, sem criação de diretórios:
C:\Projetos\PapinhoAccelerator\docs\integration\phase6-server-update-after-utility-20261101.

Fonte1 FIRST-START-SESSION-REQUEST.DRAFT.md:
SHA256312999dd9ba4f701991560e65b4a25e3a13c7914bc75603a6efb47a11536798d.
Destino exclusivo FIRST-START-SESSION-REQUEST.ORIGINAL.md, mesmo parent e bytes.

Fonte2 OWNER-FIRST-START-APPROVAL.txt:
SHA2567f6451d59fc0b7015e0ac612ee471782950880fcfc2013137dc9643276af641a.
Destino exclusivo FIRST-START-SESSION-DECISION.ORIGINAL.txt, mesmo parent e bytes.
O conteúdo dessa decisão aprova SOMENTE PREPARAÇÃO: sua cópia não aprova Start.
Não substituir seu conteúdo pelo futuro aceite deste pedido operacional.
Registrar esse eventual aceite separadamente como evidência documental.

## Artefatos fixados

Procedimento prepare-first-start-originals.PREPARATION-ONLY.MANUAL.ps1:
SHA2569e4c8cecc4d1e5bf58808100a6085ce717270936a9314856051af7530e126516.
Adaptador first-start-originals.cs:
SHA25618719507fafe6ea6fb81f73dc688410ce7531dfb6074c331de123e0441878f5f.
Nenhuma mudança no script original pinado pelo rascunho ou no EXE instalado.

## Efeitos, gates e sequência

Efeitos permitidos solicitados: até DUAS criações exclusivas, escrita dos bytes
exatos, flush, hash/readback/SD/identidade e fechamento. Sem overwrite/adoção,
rename, exclusão, rollback, ACL de objetos existentes, diretórios ou execução
do produto. Ambos os objetos permanecem se houver resultado parcial/UNKNOWN.

Antes de criar: hashes de script/adapter/fontes conferidos; compilação do
adaptador dos mesmos bytes pinados; console próprio; host/contexto elegíveis;
aquisição original de cada ancestral sem seguir reparse, retenção e leitura
de identidade/SD. O parent final exige ADD_FILE/TRAVERSE, não WRITE_DAC.
Não se presume que o workspace possua a proteção do diretório instalado.
O adaptador observa/retém/revalida descritores dos ancestrais, não é um AccessCheck
universal nem corrige ACL inadequada. Reparse/substituição/divergência/negação de
acesso interrompem. A proteção estrita do arquivo novo é imposta desde criação.

Fontes são DATA não secreta, abertas sem compartilhar escrita/deleção, mantidas
por referência e rehasheadas; conteúdo congelado em memória é usado na cópia.
Novo SD: owner do operador, DACL protegida, ACEs ALLOW sem herança: operador,
SYSTEM e Administrators full. A exceção BA da imagem não é usada como owner.
NT FILE_CREATE relativo ao parent retido impede adoção de objeto preexistente.
Identidades/SD dos novos objetos são capturadas e revalidadas; não são preditas.
Não conferir autoridade aos bytes, hashes, DACL ou presença do arquivo.

Confirmação atribuída exibirá exclusivamente o par e seus pins. Consumo da
tentativa local precede a primeira criação. O ator não reentra; FILE_CREATE
exclusivo impede overwrite em outra execução. Isso NÃO demonstra unicidade
global/ledger durável/fencing. A autorização manual será usada uma vez, mesmo
se falhar; não há retry por ausência de máscara de efeito confirmado.

Lifetime: uma invocação do procedimento e seu contexto administrativo, sem
restauração por arquivo/restart. Nenhum prazo numérico novo foi inventado para
a engenharia documental. G3 do futuro controller/runtime permanece inalterado;
não se aplica sua seleção como waiver ou capacidade do host PowerShell.
Esc/Stop é verificado antes de create/write/flush e verificações. Fechamento é
tentado uma vez, preservando primeira falha; fechamento incerto mantém UNKNOWN.

## Recursos e fronteira da qualificação

Cada documento é limitado por desenho a65536 bytes, com cópia/readback/hash
sequenciais. Referências próprias: cadeia de ancestrais existentes, duas fontes
e até dois destinos, além de alocações transitórias de descritor/hash/compilação.
Esse censo não é teto nativo/commit/latência; overhead CLR/kernel permanece
não delimitado. Não se transportam garantias B0–B8/R0–R9/G0–G9 para a ferramenta
documental nem se declara capacidade produtiva. Falha de aquisição/alocação/I/O
deve interromper e preservar o estado parcial, sem compensação automática.

Qualificação focal: compilação em memória W4/warnings-as-errors;44 verificações
novas de fluxo com I/O injetado e3 casos da guarda de escopo, parser PASS.
APIs Win32/NT reais NÃO foram exercidas. Coleta de hashes e inspeção documental
não equivalem a aquisição/identidade/SD frescos dos destinos ainda inexistentes.

## Reconciliação e proibições

Após saída manual: conferir primeira falha, consumo, máscaras de efeitos
confirmados, os dois IDs, bytes/pins/SD/readback e close_known. Zero em máscara
depois de falha não prova ausência. Não prosseguir ao caller frio a partir desta
decisão de preparação. Ausência de erro por si só não prova quiescência OS.

Não tocar em canônico, DLLs, .previous, candidato retido, SCM ou ProgramData.
Retenção histórica e quiescência OS continuam UNKNOWN. Sem limpeza/refund,
recuperação, rede, KSP, CA/CURRENT, TPM/NV/Q4 ou autoridade produtiva.
O próximo marco é decisão operacional INDEPENDENTE dos atos endpoint/SCQ/Start,
com originais do propósito correto e gates frescos; sem PID/token futuro antecipado.

## Procedimento manual

O comando exato está preparado em MATERIALIZE-ORIGINALS-MANUAL-COMMAND.md.
Ele não deve ser executado antes de autorização explícita deste pedido e
conferência de seu hash externo. Não será invocado pelo agente.
