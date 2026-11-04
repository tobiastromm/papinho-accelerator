# Pedido — disponibilização da imagem diagnóstica de primeira causa

Status: READY_FOR_OWNER_SELECTION_AND_OPERATIONAL_AUTHORIZATION / NÃO AUTORIZADO.
Data: 2026-11-01. Solicita seleção conjunta dos artefatos abaixo e autorização
específica para UMA execução manual BINARY_UPDATE_CLOSED PERSONAL_LOCAL.
Não é retry, rearm ou continuação de uma autorização consumida. Não executar
este pedido ou seu procedimento antes da decisão explícita do proprietário.

## Norma e finalidade

U1–U7 accepted: docs/adr/DECISION-personal-binary-update-u1-u7-20261101.md,
incorporando UPDATE-CONTRACT-AND-ASSURANCE.CANDIDATE.md SHA256
9fb5318dd740430ebd7ffe9307b6348826417d73b8fcd35aaac71c768e5bb234.
U2 accepted: docs/adr/DECISION-personal-update-executor-data-u2-20261101.md,
incorporando U2-EXECUTOR-AND-CANDIDATE-DATA.CANDIDATE.md SHA256
58493c93f590282b0e47ee35d8c51c61169adde482562763298b78ac133875da.
Ambos os originais foram reconferidos byte-exatos, sem alteração normativa.
Não transportar B3/R3/G3 para a admissão UPDATE.

Atualizar somente o EXE principal, para disponibilizar a tradução corrigida de
falhas de consulta e o diagnóstico SCQ_COLD_FIRST_CAUSE já qualificados offline.
O erro 1816 histórico permanece UNKNOWN: a nova imagem não prova a causa passada
nem autoriza outra sessão diagnóstica. Nenhuma redução de quota ou de guarda.

## Operador, serviço e instalação originais

Operador selecionável: S-1-5-21-785699550-1440458426-1682014700-1108,
host TCORP-TOBIAS. Metadados não são autoridade: exigir contexto administrativo
nativo elegível, declaração e confirmação atribuída OWNER_BINARY_UPDATE.
Sem autoelevação ou resposta automática.

Known folder ProgramFiles deve resolver C:\Program Files. Parent existente:
C:\Program Files\Papinho\Accelerator, ID esperado 417614100:8257536:71209.
Exigir aquisição original, ausência de reparse, continuidade e proteção efetiva
dos ancestrais, aplicando a distinção anchor/child já accepted. Não editar ACL.

PapinhoAccelerator: WIN32_OWN_PROCESS (16), DEMAND_START (3), ERROR_NORMAL (1),
NT SERVICE\PapinhoAccelerator, SID UNRESTRICTED (1); comando exato:
`"C:\Program Files\Papinho\Accelerator\papacc_server.exe" service host scq-closed`.
Readback somente leitura nesta preparação: STOPPED, PID0, exit1077.
Não exigir exit0 pré-Start onde o contrato exige STOPPED/PID0. Snapshot não é
lease/fencing: repetir os gates nativos de estado/configuração antes dos efeitos.
Não chamar Start/Stop nem alterar SCM para conseguir atualizar.

## Seleção exata de arquivos

Executor independente, não alvo da substituição:
C:\Program Files\Papinho\Accelerator\papacc_update.exe,
104448 bytes, ID esperado 417614100:30670848:57859,
SHA256 084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a.
Proveniência: preparação e manutenção governadas já reconciliadas em
../phase6-utility-replacement-integration-20261101/UTILITY-UPDATE-RECONCILIATION.md;
capacidade UPDATE demonstrada no SERVER-UPDATE-RECONCILIATION.md desta pasta.
Correção qualificada de rename: RootDirectory=NULL, sem abrir novamente o parent
com sharing incompatível. Não reconstruir/substituir o utilitário nesta operação.

Anterior canônico:
C:\Program Files\Papinho\Accelerator\papacc_server.exe,
762880 bytes, ID esperado 417614100:1900544:539611,
SHA256 3d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b.

Novo EXE exclusivamente DATA, não executar/carregar:
C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release\papacc_server.exe,
763904 bytes, SHA256
7d29d50ca008dc7a92759270388bf62e962d2d343e39390db64bd96f971eca7d.
Proveniência: COLD-1816-FIRST-CAUSE-OFFLINE-CLOSEOUT.md e respectivo EVIDENCE.json,
quatro builds Selected/Legacy Debug/Release PASS sem warnings, 20 pares CTest PASS.
Delta instalado: +1024 bytes; metadados privados do diagnóstico +32 bytes.
Nenhuma identidade de novo objeto é inventada antes de sua aquisição/criação.
Capturar e revalidar identidade factual da DATA, SD completo, tamanho, atributos,
hash pelo handle e continuidade, sem atribuir autoridade ao workspace.

DLLs no parent protegido, somente leitura, sem cópia ou rename:

| Arquivo | Bytes | SHA256 |
| --- | ---: | --- |
| libssl-3-x64.dll | 1321472 | 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed |
| libcrypto-3-x64.dll | 7217664 | 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044 |

Hashes dos cinco arquivos reconferidos somente leitura nesta preparação.
Consulta adicional por handles confirmou parent8257536:71209, executor30670848:57859,
anterior1900544:539611 e DATA196608:2588986, todos no volume417614100,
sem reparse nos quatro objetos consultados, com continuidade dentro dessa
consulta e fechamento conhecido. ID DATA factual:417614100:196608:2588986;
o owner deve readquirir e vincular a referência original nesta futura invocação.
Parent, executor e canônico: owner Administrators, DACL protegida,
AU RX 0x1200a9, SY/BA full; não é atestação de páginas mapeadas ou continuidade
retroativa. IDs esperados são originais históricos; sua validação por handle,
associação imagem/processo/token, dependências e continuidade são gates frescos
obrigatórios do wrapper e owner. Preservar guarda estrita U2 do executor.

## Disposição retida: preservar sem adoção ou recuperação

Listagem/hash somente leitura atuais confirmaram estes objetos existentes:

- papacc_server.742d832576fb64525de6d3db57eedc5eb457c8fc31990f7f745dd19243d0f10b.candidate,
  758272 bytes, SHA256 7612d1114b641875ad4536a6e9a2a4899a9115674b8a098fe7cc8b7767b1cf7f;
  ID histórico 417614100:1769472:537456; cohort/close/créditos históricos UNKNOWN.
- papacc_server.638a92da338de9d8c3683be1b14e64d8f67cfdf89cd57fb9e0848d8dc3608ce6.previous,
  571392 bytes, SHA256 c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf.
- papacc_update.fcc29b1584d97bef8923a5fb7e7b5f4857316e70b157fcbbd50239525259230d.previous,
  104448 bytes, SHA256 6c0e77173f772b7748d08d9ef2ca29f844d629cc0090b3a73a3cafa87ea0bb87.

Nenhum desses objetos será tocado. Consulta não prova lifetime/fechamento nativo
histórico ou quiescência OS. Nenhum crédito é presumido liberado ou reaproveitado.
Se UNKNOWN for material à identidade, capacidade, exclusividade, retenção ou
reserva desta invocação, STOP; esta seleção não dispensa predicados do owner.
Resultado saudável recente não resolve uma coorte antiga. Não limpar/restaurar.

## Plano original, consumo e efeitos únicos

O mesmo owner captura os originais, gera tentativa CSPRNG de32bytes, congela
plano e apresenta os nomes exatos antes da confirmação. Dois leaves no parent:
papacc_server.<tentativa hex64>.candidate e papacc_server.<mesma tentativa>.previous.
Essa regra de binding tardio não é placeholder de comando. Não adotar nome
existente, mudar leaf para contornar conflito ou repetir após falha.

1. Captura/revalidação read-only de executor, DATA, anterior, DLLs, ancestrais,
   token/contexto, SCM, recursos e volume. Stop bridge existente do controlador.
2. Declaração e confirmação independente BINARY_UPDATE_CLOSED, consumo
   irrevogável antes de qualquer criação. Autorizações anteriores consumidas.
3. Uma criação exclusiva FILE_CREATE com proteção nova BA/SY full e AU RX;
   copiar somente763904 bytes da referência DATA retida, hash independente,
   revalidar identidade/tamanho/SD e flush. Não executar DATA.
4. Rename do anterior por handle para .previous exclusivo, sem overwrite;
   verificar disposição e original. API qualificada com RootDirectory=NULL.
5. Rename do candidato verificado para papacc_server.exe, sem overwrite;
   readback relativo ao parent original: identidade nova, hash/bytes/proteção,
   SCM ainda parado e configuração inalterada.
6. Fechamento factual uma vez, disposição e identidades finais. Preservar anterior.

Wrapper retém executor/DATA/DLLs/cadeias sem share DELETE. Apenas parent de
criação permite share WRITE. Fecha lock readonly de preflight do anterior ANTES
de chamar executor, que readquire por original com DELETE/guards necessários.
Não alegar continuidade no intervalo: readquisição/ID/hash nativos são obrigatórios.

## Recursos U4, sem importação G3

Executará o MESMO utilitário protegido084aa..., não o novo servidor.
Seu K compilado e observado no UPDATE qualificado =2027314 bytes; composição
conhecida do executor não mudou. Censo Selected dessa revisão2001408;
orçamento compartilhado3145728 bytes inalterado. Novo servidor tem censo2001440,
mas é DATA: seu censo não se soma à reserva nem altera o código do executor.
Buffer limitado existente faz cópia em partes; aumento1024 de payload muda R,
não K. Física/commit disponíveis >=K em amostra fresca antes de cada mutação.
Não é reserva física nem máximo nativo comprovado.

R=ceil(763904/U)*U, U da geometria real congelada/revalidada do volume,
aritmética verificada; com U4096, R765952 bytes. Exigir caller-free/actual-free
>=R antes da criação e payload restante arredondado antes das escritas.
Esse exemplo não substitui consulta fresca. Retidos já alocados não são novas
cópias; metadados/flush/rename não são cobertos por R.
Contar handles completos/herdados, preservar multiplicidades e reservas.
Não impor aqui124/128 de SCQ, nem alterar suas quotas em seu domínio.

Freshness por evento: gates imediatamente antes de cada efeito, depois da
confirmação humana e chamadas anteriores; sem TTL/duração arbitrários.
Custos loader/CRT/wrapper, alocador/hash/SD, RPC/SCM, filesystem/cache/metadados,
flush/rename/kernel e retenções opacas permanecem UNKNOWN sob U4. Aceitar risco
residual não prova ausência de exaustão, latência máxima ou cancelamento imediato.

## Stop, falhas e sucesso limitado

Esc cancela confirmação; Ctrl+C/Ctrl+Break sinaliza Stop do controller, nunca
ControlService. Antes de cada criação/escrita/flush/rename: Stop, contexto,
originais, SCM e recursos revalidados. Divergência, falha, insuficiência ou
UNKNOWN material impede o próximo efeito. Não matar processo ou elevar direitos.
Stop não cancela I/O já submetido. Não presumir atomicidade entre renames ou
durabilidade de metadados. Falha pode deixar candidato parcial, caminho canônico
ausente ou novo canônico sem readback. Reter objetos/cohort/créditos incertos;
sem rollback, delete, restauração, timeout cleanup ou retry.

Exigir saída integral: first0, consumed1, created1, renames3, readback canônico,
close_known1 e canonical_verified_closed1 com LIMITED_UPDATE_VERIFIED_CLOSED.
Exit0/wrapperclose isolados não bastam; OS_QUIESCENCE e retenção histórica UNKNOWN
permanecem explícitos mesmo após sucesso. Depois, nova decisão própria seria
necessária para endpoint/SCQ/Start, sem reusar decisão operacional consumida.

## Procedimento candidato, não executado

update-cold-diagnostic.MANUAL.CANDIDATE.ps1 nesta pasta, SHA256
e81f1e410cb7c6f87f9ae6a5a166a39994559f7ca622179f95a32cc432e503ae.
Windows PowerShell5.1 Desktop x64, sem perfil, contexto administrativo legítimo
do proprietário; nenhuma autoelevação. Verifica os hashes explícitos de si e
deste pedido, U1–U7/U2, contexto e cinco arquivos por referências retidas antes
da única chamada ao utilitário protegido (16 argumentos, sem resposta automática).
Mantém o mecanismo qualificado; somente pins/tamanhos/ID anterior/pedido mudam.
Parsing AST e comparação fechada do delta PASS, sem invocação do procedimento.
Não é comando para executar agora. Após aprovação byte-exata, preparar comando
manual literal com os dois hashes. ExecutionPolicy, se necessário, somente no
processo filho para esse script, nunca alterar políticas persistentes/GPO.
Invocar o procedimento utiliza a tentativa manual; consumed interno0 ou erro de
shell não concede retry. Não modificar artefatos após a seleção sem conferência.

Proibidos: atualização do utilitário/DLLs, SCM/ACL, Start/Stop, endpoint/SCQ,
CA/CURRENT/ProgramData, rede, TPM/NV/Q4, cleanup/refund, exclusão, rollback,
restauração e recuperação. Preservar HEAD/WIP/pins/históricos/EXEs anteriores,
originais e atos consumidos, reservas, PST0.6.4. DENY até autorização específica;
E21 PENDING, ED11/Phase6 PARTIAL, nenhuma prontidão produtiva concedida.
