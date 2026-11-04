# Preparação protegida exclusiva de papacc_update.exe

Status: CANDIDATE / NÃO ACEITA. Data:2026-11-01.
Documento focal designado desta frente, não outra ADR/família normativa.
Autoriza-se atualmente somente sua preparação, não implementação positiva
dependente, seleção operacional ou execução. Nenhuma autorização consumida
é restaurada. A presença do utilitário não concede autoridade de UPDATE.

## Necessidade e precedência exatas

U1–U7, incorporadas na DECISION-personal-binary-update-u1-u7-20261101, U1:
"Atualizar apenas papacc_server.exe; DLLs idênticas são adquiridas/validadas,
nunca copiadas." U3/U5 permite um candidato desse EXE e dois renames, não
provisionamento de um executor adicional.

A especialização accepted de U2, SHA25658493c93…75da, declara:
"Esta proposta não instala um controller, não autoriza provisionamento e não
inventa uma fonte de confiança." A decisão de aceitação também exclui
provisionamento de controller. Separação DATA/executor não é autorização de cópia.

B0/B3 de B0–B8 accepted delimitam outro ensaio: três arquivos nomeados
papacc_server.exe, libssl-3-x64.dll e libcrypto-3-x64.dll, sem outro arquivo.
O ato anterior está utilizado. R0–R9 é registro SCM; G0–G9 é Start diagnóstico.
Nenhum fornece este novo efeito, autoridade ou redução de garantia.

Propõe-se permitir SOMENTE o ato de preparação descrito abaixo. Não substituir
U1–U7 ou reabrir U2. A precedência nova é (a) permitir um arquivo adicional
com propósito independente e (b) aceitar explicitamente a garantia limitada
de preparação pessoal/local e de seus custos opacos. A proteção estrita do
executor de UPDATE permanece intacta. Não é conflito entre normas accepted:
é efeito ainda sem cobertura, que permanece DENY até decisão específica.

## Alternativas e recomendação

A — exigir preparador já protegido e provisão integral de custos opacos.
É a alternativa estrita, sem redução de garantia. Não há preparador protegido
capaz demonstrado para esse novo ato no owner atual; ele não pode ser obtido
por aprovar o próprio artefato a preparar. Continuar DENY nessa ausência.

B — recomendação para decisão: UM ato pessoal/local de primeira preparação
do utilitário, pelo administrador legítimo, no owner já existente. Reutilizar
as mecânicas originais F1–F6 de declaração, identidade/token, plano, tentativa,
confirmação e consumo, e as primitivas de staging, sem fingir que a antiga
autorização de staging vale novamente. Fonte técnica da cerimônia: controller
papacc_server do projeto, depois de integrar/qualificar a entrada restrita
de preparação; NUNCA executar papacc_update para proteger a si próprio.
Não se exige criar outro preparador/helper/serviço/root/distribuição.

Escolher B exige aceitar conjuntamente as condições e riscos abaixo. Não
tratar essa recomendação como algo já permitido por F0 ou pela task técnica.

## Ator e limite de confiança da preparação

O ato é distinto de NEW_INSTALL, BINARY_UPDATE_CLOSED, SCM_REGISTER_CLOSED,
Start e SCQ. Somente administrador/proprietário legítimo do mesmo host,
com contexto nativo elegível, direitos sobre o parent original, declaração
atribuída e confirmação do plano exato. Token/elevação/hash/path sozinhos não
autorizam. O owner fixa política/plano/referências; nenhum grant persistido,
ledger ou setter de autoridade, nem delegação a um novo processo/serviço.

O controller de preparação pode vir do build qualificado do projeto, ainda
no workspace, APENAS neste ato de bootstrap delimitado. Isso é uma redução
explícita da garantia de controller previamente protegido, não uma prova de
que a ACL do workspace é segura ou equivalente à instalação. A política
confia no controle administrativo legítimo do host e na escolha atribuída
do controller revisado; não comprova por token que o ambiente não foi adulterado.
Não reutilizar essa redução na execução posterior do UPDATE U2.

Antes da invocação manual futura, o pedido deve selecionar controller e suas
dependências, com proveniência de fonte/build/qualificação e hashes finais.
O procedimento deve adquirir e manter referências originais de arquivos e
cadeia, recusando reparse, substituição, compartilhamento de escrita/delete
concorrente e divergências; conferir bytes/pins antes da chamada e correlacionar
processo/imagem/contexto na cerimônia, conforme os adapters qualificados.
Não inventar file IDs ou tratar nome de processo como identidade.

Essa associação e os locks não atestam universalmente imagem mapeada, intervalo
anterior à aquisição, DLLs já adulteradas/mapeamentos preexistentes ou kernel/
administrador comprometido. Aceita-se expressamente o risco residual do
bootstrap administrativo local nesse alcance. Ausência das referências ou
das verificações descritas continua DENY, não pode ser dispensada por risco.
Uma solução que não consiga implementar/qualificar essas condições não pode
operar sob esta proposta. Não basta adicionar bypass de SD ou uma flag positiva.

## Efeito único e arquivos preservados

Destino: known folder ProgramFiles resolvido/congelado, parent EXISTENTE
Papinho\Accelerator, leaf exato papacc_update.exe. Planejamento:
C:\Program Files\Papinho\Accelerator\papacc_update.exe. O parent e ancestrais
devem passar aquisição relativa original, proteção efetiva/owner/DACL estritos,
ausência de reparse e continuidade, preservando a regra anchor/child já aceita.
Nenhum diretório novo, adoção de diretório ou edição de ACL preexistente.

Somente UM arquivo FILE_CREATE exclusivo. Se o leaf existir ou houver dúvida,
STOP sem abrir para substituir/adotar, sem tentar nome alternativo ou apagar.
Aplicar descritor protegido na criação: BA/SY full e AU read/execute, com
owner admitido pela guarda estrita existente. Presença/pin desse arquivo não
será uma autorização administrativa nem permitirá executar por reflexo.

Nenhuma cópia/rename/write no EXE canônico ou nas DLLs; preservar seus objetos,
ProgramData, configuração SCM, CA/CURRENT e identidade da instalação. Não
consultar/alterar SCM ou executar Start/Stop como parte da preparação. Criar
um arquivo independente não depende de iniciar serviço futuro. UPDATE posterior
continua exigindo SERVICE_STOPPED/PID0 em seus próprios gates frescos.

## DATA, plano e sequência governada

O utilitário fonte é DATA somente, arquivo regular adquirido read-only por
referência original com identidade/tamanho/atributos e owner/DACL interpretados.
Recusar descritor nulo/inválido/semântica não suportada. Não lhe atribuir a
autoridade do preparador por owner/hash. Inspecionar/capturar descritor e digest,
manter custódia e negar escrita/delete concorrentes; não executar/carregar o
utilitário de origem. Fonte/controller/EXE canônico/DLLs/destino não podem ser
aliases/hardlinks utilizados para confundir papéis.

Congelar no mesmo packet: operador/contexto, propósito, controller/fontes e
proveniência, parent original, leaf, tamanho/pins, proteção nova, custos/gates,
tentativa e consequências. Mostrar todos os campos ativos antes de confirmar.
Tentativa CSPRNG existente, confirmação atribuída própria e consumo irrevogável
ANTES da primeira criação. Nenhuma resposta automática; replay/concorrência
ou mudança de plano/contexto/referência invalida a confirmação.

Reutilizar criação relativa exclusiva, buffer limitado existente de cópia,
progresso/I/O parcial sem repetir bytes aceitos, SHA256 independente, flush,
readback por parent original e identidade do objeto novo, tamanho/conteúdo/SD.
O file ID do filho é adquirido após criação, nunca predito. Gate original/
recursos/Stop antes de criação, de cada escrita, de flush e demais efeitos.
Hash de origem e hash do arquivo copiado devem coincidir com o pin selecionado.
Não reabrir fonte por strings nem substituir os originais durante a cópia.

## Recursos próprios e redução limitada de garantia

Não importar B3/R3/G3, K do UPDATE, limites124/128/1024 ou picos históricos como
admissão desse ato. Censar packet/caller/buffers/temporários, referências próprias,
originais/controller/fontes/parent/filho, concorrência e retenção na composição
final. Reutilização somente com lifetime comprovado. Preservar quotas geridas e
individuais existentes; nada nesta proposta aumenta ou escolhe quotas novas.

Propõe-se especializar, SOMENTE para este ato, a exigência de provisão prévia
integral para custos opacos de loader/CRT/alocador/SD/kernel/cache/metadados/
filesystem/flush e retenção opaca, sob ED11 incremental20261012, W4 e F8.
Não se transporta a exceção B2 ou U4 automaticamente: esta seria uma aceitação
nova, independente e delimitada dos mesmos tipos de risco para outro efeito.
Custos permanecem UNKNOWN quanto ao máximo, não PASS/zero/crédito.

Critérios propostos, a implementar/qualificar antes de qualquer pedido:

- K_PREP é a charge conhecida completa demonstrada pelo censo final, não o
  tamanho do EXE nem um número preenchido por inferência. Exigir observação
  corrente de física e commit disponíveis >= K_PREP antes dos efeitos.
- Contar handles totais do processo original e inventariar referências próprias,
  sem descontar herança, temporários ou kernel opaco; falha de query nega. Não
  inventar teto universal de handles ou chamá-lo limite de exaustão demonstrado.
- R_PREP é o tamanho fonte congelado arredondado pela geometria real do volume,
  com overflow e coerência verificados. Caller-free/actual-free >= R_PREP antes
  da criação e >= restante arredondado antes das escritas. Metadados opacos
  não estão cobertos por R_PREP; espaço livre não é reserva.
- Freshness por evento/contexto: depois da confirmação e de chamadas anteriores,
  revalidar originais/recursos imediatamente antes da submissão dependente.
  Sem renovar plano, consumo ou lifetime por amostra. Não propõe TTL/duração
  numéricos sem fundamento nem garantia de tempo de chamada síncrona.

Esses gates são margens observacionais fracas, não reserva nem garantia contra
exaustão. Host pode mudar entre amostras; alocação/IO pode falhar ou bloquear
apesar dos gates. A aceitação deve reconhecer expressamente esse risco, a
ausência de pico/latência/cancelamento imediato demonstrados e resultado parcial.
Se o censo/controlados não couberem nas quotas ou faltar predicado acima, DENY.

## Stop, fechamento e UNKNOWN

Stop, erro, fonte/substituição divergente, colisão, recurso insuficiente,
consumo duplicado, suspensão/perda de continuidade ou consulta incerta nega
novos efeitos. Stop não cancela I/O já submetido nem elimina o arquivo criado.
Fechar cada referência uma vez, por owner/lifetime, sem fechar emprestadas.
Retenção de packet/originais/créditos em I/O/close incerto conforme mecanismos
existentes; sem timeout cleanup, reset, retry ou liberação presumida.

Após efeito parcial pode restar papacc_update.exe incompleto ou não admitido.
Conservar disposição factual, sem delete, rollback, rename ou correção automática.
Resultado saudável exige criação exclusiva, bytes/hash/SD/readback e fechamento
controlado conhecido. Quiescência OS e durabilidade universal continuam UNKNOWN.
Mesmo sucesso não inicia utilitário/serviço, não autoriza UPDATE ou promoção.

## Artefato de planejamento e sequência até operação

Artefato atualmente qualificado offline: Selected/Release papacc_update.exe,
100352bytes, SHA256f449ca4d96853897d009e5b317ec71bdd9fd70dde65240266504e6d07d7390a6,
na pasta de build canônica. É referência de planejamento, NÃO seleção operacional.
A integração de preparação no owner pode alterar o controller e artefatos
relinkados; verificar delta, qualificar e congelar hashes finais antes de
formular o pedido. Não substituir silenciosamente um pin selecionado ou
pedir seleção de binário ainda sujeito a alterações.

Se aprovada integralmente, registrar a precedência focal em documento normativo
designado antes do código dependente, preservando os contratos/originais. Então
integrar a entrada restrita de preparação no owner do server existente, sem
alterar o update_main do utilitário para autopreparar ou aceitar novos propósitos.
Qualificar APIs mutantes modeladas: contexto/confirmacão ausentes, source/tamanho/
hash/SD inválidos, writer concorrente, parent/anchor/reparse/substituição, leaf
existente, quotas/volume desconhecidos ou insuficientes, falha de create/write/
flush/readback/close, replay, Stop/concorrência e retenção, sem executar nativo.

Só depois: pedido byte-exato de UM ensaio manual, artefatos finais, operador,
parent/leaf, efeitos, riscos, gates, STOP/UNKNOWN e autorização operacional
própria. Não gerar comando agora. A aprovação desta proposta não autoriza tal
execução, preparação de arquivo, UPDATE ou uso do resultado.

Não reusar a autorização consumida de staging/UPDATE. Após futura preparação
qualificada, o utilitário deverá satisfazer U2 estrita na execução de UPDATE,
com seu ID/hash original, ato independente e serviço parado. Este ato de
preparação não se restaura após restart nem se transforma em autoridade U2.

Nenhum Start/Stop, endpoint, SCM/ACL mutante, CA/CURRENT, rede, TPM/NV/Q4,
limpeza/refund/recuperação. Sem alteração de produção PST0.6.4. Preservar
HEAD/WIP, históricos/pins, EXE canônico/anterior, DLLs, reservas e UNKNOWN.
