# Pedido candidato — UMA preparação exclusiva de papacc_update.exe

Status: READY_FOR_OWNER_SELECTION_AND_SPECIFIC_MANUAL_AUTHORIZATION / NÃO AUTORIZADO.
Data:2026-11-01. Não executar. Solicita-se uma única invocação manual, sem
retry, exclusivamente sob as condições abaixo. A aprovação normativa existente
autoriza código/qualificação, não esta operação nem a seleção desses artefatos.

## Norma e ato independente

DECISION-personal-one-utility-preparation-20261101 incorpora a proposta original
SHA256029a611bb19e41827719c4269f6e029895a8f57f2d125141258274da8b2707fb.
Ato exclusivo UTILITY_PREPARE_EXCLUSIVE no owner já existente. Não reutiliza
NEW_INSTALL, staging, registro SCM, UPDATE, SCQ ou Start. Todas as autorizações
anteriores utilizadas continuam não reutilizáveis, mesmo consumed nativo0.

O pedido aceita SOMENTE os riscos limitados de bootstrap/controlador de build
e custos opacos dessa proposta, sem estendê-los ao UPDATE U2 posterior. Não
constitui novo serviço, root, canal, autoridade ou distribuição. A presença
do arquivo resultante jamais é aprovação para executá-lo ou iniciar serviço.

## Operador e alvo

Operador selecionável: S-1-5-21-785699550-1440458426-1682014700-1108.
Host selecionável: TCORP-TOBIAS. São metadados históricos para comparação,
não prova de identidade atual, propriedade, boot ou autoridade. Procedimento
deve partir do próprio proprietário em console Windows PowerShell5.1 x64 já
elevado; sem elevação automática. O backend exige token/contexto originais
elegíveis, direitos sobre parent, declaração atribuída e confirmação local
específica. Sem eles, DENY, inclusive se os hashes conferirem.

Known folder ProgramFiles deve resolver C:\Program Files e ser congelada.
Parent EXISTENTE: C:\Program Files\Papinho\Accelerator.
Identidade histórica esperada: volume417614100/high8257536/low71209.
Capturar e revalidar o objeto original e ancestrais com owner/DACL/proteção
estritos, ausência de reparse e regra anchor/child já qualificada. Não adotar
ou recriar qualquer diretório. Divergência do known folder/identidade nega.

Único leaf novo: papacc_update.exe. Destino completo:
C:\Program Files\Papinho\Accelerator\papacc_update.exe.
Se existir, inclusive objeto desconhecido/reparse/colisão, STOP; não abrir
para substituir/adotar, mudar nome, apagar ou tentar novamente. A autoridade
deste pedido não cobre completar uma criação parcial anterior.

Canônico somente leitura, nunca escrito/renomeado por este ato:
C:\Program Files\Papinho\Accelerator\papacc_server.exe,
571392bytes, SHA256c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf,
identidade histórica417614100/high6225920/low71237. Revalidar fresca, não presumir
que a leitura estática anterior substitui custódia original.

## Artefatos finais para seleção conjunta

Fonte: C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release.
Quatro builds qualificados; para esta operação propõe-se SOMENTE Selected/Release.

| Papel/arquivo | Bytes | SHA256 |
|---|---:|---|
| Controller papacc_server.exe |758272|7612d1114b641875ad4536a6e9a2a4899a9115674b8a098fe7cc8b7767b1cf7f|
| DATA papacc_update.exe |104448|6c0e77173f772b7748d08d9ef2ca29f844d629cc0090b3a73a3cafa87ea0bb87|
| Dependência do controller libssl-3-x64.dll |1321472|3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed|
| Dependência do controller libcrypto-3-x64.dll |7217664|09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044|

Manifesto FINAL-ARTIFACTS.json SHA256
103790944d90fbdcf18de1f5cb9c8a1fac340c71f1a6e1caf927b6411ac29cdc.
Proveniência: fontes e evidências da branch main/HEADf341985924d29870d8ff627ed37c8a0874ad6469
com WIP preservado, CLOSED_OWNER_TYPED_ACT, builds/testes registrados nesta
pasta. Artefatos de planejamento anteriores f449ca4d…390a6/100352 e
021c7e5c…26dd/740352 NÃO são os selecionados por este pedido novo. A integração
autorizada mudou os bytes; não há substituição silenciosa de seleção anterior.
Não usar build posterior sem conferência e nova seleção do pedido.

O controller é o server de build, NÃO o utilitário DATA e NÃO o EXE instalado
antigo. Ele não será copiado, instalado ou usado para UPDATE por esta aprovação.
O utilitário de origem jamais será executado. DLLs serão apenas retidas/lidas;
as duas DLLs instaladas e configuração SCM/ProgramData não serão modificadas.

## Efeitos e referências, nesta ordem

1. Conferências somente leitura de pedido/script/norma/manifesto/pins/contexto.
   Manter arquivos e cadeias originais sem compartilhamento de delete; arquivos
   fonte e sua cadeia sem share WRITE. SOMENTE o parent protegido de criação
   permite share WRITE para o child adquirir FILE_ADD_FILE; isso NÃO concede direitos a atores.
   Proteção/identidade continuam verificadas pelo backend, não inferidas do lock.
2. UMA chamada do controller qualificado. Capturar originais de operador/token,
   target, cadeia de origem, controller/DLLs/DATA e canônico, com comparação de
   processo/imagem/PID/criação. Fontes/bootstrap têm a redução específica aceita;
   parent/canônico e arquivo novo permanecem estritos. Nenhum handle/pointer ou
   token administrativo é transmitido em argumentos. Aliases e substituição negam.
3. CSPRNG32bytes existente para tentativa; apresentar plano imutável com IDs,
   hashes e fingerprints de SD, leaf, proteção, recursos e riscos. Proprietário
   declara administração legítima e confirma SOMENTE esse plano digitando
   OWNER_UTILITY_PREPARE seguido do nonce nativo mostrado. Nonce futuro/ID do
   novo filho não são inventados aqui. Nenhuma resposta é fornecida pelo script.
4. Revalidar plano/originais/contexto/recursos/Stop; consumir irrevogavelmente
   antes de UMA NtCreateFile FILE_CREATE exclusiva de papacc_update.exe.
   Descritor novo O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;GRGX;;;AU).
   Sem WRITE_DAC/WRITE_OWNER/DELETE em originais ou arquivo novo; não editar ACL.
5. Copiar exatamente104448bytes a partir do handle DATA retido, usando buffer
   limitado e I/O parcial com progresso. Revalidar antes de cada escrita/flush,
   conferir hash independente/tamanho/proteção, flush e readback relativo ao
   mesmo parent, com identidade do novo objeto observado. Não há rename.
6. Fechar referências governadas uma vez: arquivos-filhos antes de cadeias,
   preservando o cohort em erro parcial/close incerto. Emitir resultado e ID
   observado, sem autoridade para UPDATE. Wrapper revalida suas referências
   originais e fecha as próprias, sem confundir close com quiescência universal.

Somente o arquivo acima pode ser criado/escrito pelo fluxo. Nenhum diretório,
outro binário, sink/log operacional ou arquivo de grant/ledger por conveniência.
Interop somente leitura do wrapper é emitido em memória, sem compilação de
helper/DLL em disco. Metadados/caches/loader/registro OS implícitos não são
prometidos como zero, mas nenhum efeito adicional é solicitado pelo procedimento.

## Recursos e alcance da garantia

K_PREP Selected=2024636bytes de composição conhecida final; quota gerida
compartilhada3145728bytes, sem aumento. Censo não é processo real medido nem
teto nativo/CRT/kernel. Não importar K do UPDATE/B3/R3/G3 ou handles124/128.
Inventário nativo: objeto-slots existentes, até3 ancestrais de destino neste
layout e6 de origem, controller/DATA/DLLs/canônico/novo/readback e contexto
nativo. Sem descartar referências para caber. Contagem total de handles,
privado/working set e margem física/commit são observadas, inclusive herança.

Antes dos efeitos, observação fresca de física e commit disponíveis >=K_PREP;
insuficiência, erro ou incoerência nega. R_PREP=ceil(104448/U)*U, com U obtido
da geometria REAL/coerente do volume. Caller-free/actual-free >=R_PREP antes
da criação e >=restante arredondado antes das escritas. Sem inferir U do ensaio
antigo, reserva de disco ou teto de metadados. Mudança de geometria/overflow nega.

Freshness é por evento/contexto: depois de confirmação e de chamadas anteriores,
revalidar originais/recursos imediatamente antes das submissões dependentes.
Não se escolhem TTL/duração numéricos sem fundamento. A mesma invocação viva,
plano, tentativa e contexto devem continuar; Stop/suspensão/perda de continuidade
invalidam novos efeitos. Sem hard deadline de kernel, cancelamento imediato ou
promessa de tempo entre samples. A duração/pressão pode variar inclusive na
digitação da confirmação; nova amostra não renova ato ou modifica o plano.

Wrapper PowerShell é outro processo e não uma franquia do orçamento PACC.
Seus originais/leitura/hash/interop de memória e CRT/framework/loader têm custos
próprios/transitórios opacos; a margem física/commit do host é compartilhada e
observada pelo controller, não reservada. Custos opacos enumerados no ato aceito
permanecem UNKNOWN, não zero, máximos provados ou ED11 PASS. Pode haver exaustão,
degradação/IO bloqueante e falha parcial apesar dos gates. Exige-se reconhecer
expressamente esse risco residual; monitoração não o elimina.

## Bootstrap, Stop e UNKNOWN

O controller qualificado ainda vem do workspace sob a garantia limitada
explicitamente aceita SOMENTE para esta preparação. Os locks/hash/identidades
e a correlação não atestam todas as páginas mapeadas nem intervalos anteriores,
DLL/mapeamento já adulterados ou host/admin comprometido. Aprovação/declaração
elegível continuam distintas dos fatos técnicos. Não dizer que a ACL do
workspace passou na política estrita U2. O UPDATE posterior continua exigindo
o utilitário protegido independente e todos os seus gates.

STOP em qualquer pin/pedido/norma/contexto/identidade/SD/parent/volume divergente,
reparse/alias/colisão, falta de recurso, replay/concorrência, mudança de plano,
confirmação ausente, operação pending/contraditória ou close incerto. Não ampliar
permissões, tolerâncias, nomes, quotas ou número de tentativas para obter PASS.
Stop suprime novas submissões, não cancela I/O já emitido ou apaga arquivo.

Falha depois da criação pode deixar arquivo parcial/indisponível que impede
outra criação. Reter disposição/referências/créditos UNKNOWN pelo owner conforme
lifetime, sem delete, rollback, refund ou retry. Autoridade deste pedido é usada
quando o proprietário inicia a invocação manual, mesmo se wrapper/native negar
antes de consumir ou criar. consumed nativo0 não rearma autorização administrativa.
Não há alegação de enforcement global cross-process por arquivo/marker.

Resultado de preparação limitado exige UTILITY_EXCLUSIVE_VERIFIED_CLOSED,
created1/consumed1, first0, readback/hash/SD e fechamento conhecidos, além de
WRAPPER_CLOSE_UNCERTAIN=False. Ausência/contradição de evidência mantém UNKNOWN;
exit0 isolado não concede autoridade produtiva. OS_QUIESCENCE permanece UNKNOWN.

## Procedimento manual candidato e aprovações separadas

prepare-utility.MANUAL.CANDIDATE.ps1 SHA256
3c16e6b50a369b011ee9df9dee8b1276bb5bb781b86d98eaa11a427eebb63df5.
Parsing estático e helpers somente leitura foram qualificados; o procedimento
completo NÃO foi executado. Não executar agora. Após aprovação exata deste
pedido: registrar o ato do proprietário como auditoria, preparar/conferir o
comando com hashes literais antes da chamada e aguardar a única ação manual.
O script exige os hashes do próprio script, pedido, norma e manifesto antes
de invocar o controller. Sem placeholders, retry, resposta automática ou UAC.

Se a política local impedir script não assinado, essa aprovação operacional
incluirá EXCLUSIVAMENTE o processo filho Windows PowerShell5.1 x64 desta
invocação com -NoProfile -ExecutionPolicy Bypass, sem alterar política persistente,
do console pai, GPO, ACL ou assinatura; política superior que bloqueie exige STOP.
Não autoriza Set-ExecutionPolicy, Unblock-File ou alterações de permissões.

Evidências:64 pares teste/configuração afetados PASS, quatro builds sem warnings,
53 cenários/295 verificações de preparação por configuração; helpers Windows
PowerShell5.1 x64 reais somente sobre workspace e executável de teste modelado,
sem executar controller/utilitário/target/SCM/preparação. Essas provas não são
execução nativa positiva no alvo. Reconciliação da saída futura é obrigatória.

Não concede UPDATE, uso do utilitário, SCM/Start/Stop, endpoint, CA/CURRENT,
rede, TPM/NV/Q4, provisionamento adicional, exclusão/limpeza/refund/recuperação
ou promoção de PST. Preservar EXE anterior/canônico, DLLs/ProgramData/configuração,
HEAD/WIP, históricos/pins, autorizações utilizadas e reservas. E21 PENDING;
ED11/Phase6 PARTIAL; autoridade produtiva DENY; produção PST0.6.4 preservada.
