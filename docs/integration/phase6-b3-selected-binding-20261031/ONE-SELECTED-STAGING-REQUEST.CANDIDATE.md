# Pedido operacional — UM staging LIMITED_QUALIFICATION selecionado

CANDIDATO PARA APROVAÇÃO EXPLÍCITA. NÃO APROVADO/NÃO EXECUTADO. Aprovar este pedido integral seleciona o pacote derivado abaixo e autoriza somente uma execução manual do procedimento aqui delimitado. Não autoriza execução pelo agente. Não é nova decisão normativa B0–B8/D0–D7 nem nova seleção dos critérios já escolhidos.

## 1. Autoridade e escopo

Base: DECISION-personal-limited-staging-assurance-b0-b8-20261031, proposta incorporada SHA256b4b705cc20899a58295c67e95105a6ad3792df4505fe51c0cd68a10d9e8acfbb; D0–D7 accepted. Seleção administrativa explícita OWNER-B3-SELECTION.json SHA256c013a98b6b3e0e2a154a1b4cc43dd804c055ad53bcfb163c08daa983489713f3, vinculada ao plano original SHA256e4ecd81a00ed4d09a272ac9e3219b1d3ef08b725049a109b6436dee9ee4b724e. Nada disso é autorização operacional por si.

Executor: o proprietário legítimo no console Windows PowerShell5.1 Desktop x64 já elevado, no equipamento local que contém o projeto canônico C:\Projetos\PapinhoAccelerator. Não autoriza outro serviço/usuário remoto ou impersonação. O script registra SID/host e evento Kernel-General12 do boot correntes; são metadata, não concessão de autoridade. O backend nativo adquire/revalida token efetivo/contexto original e direitos/objetos, apresenta SID, fonte/destino/conteúdo/limites e exige confirmação atribuída ao mesmo contexto com desafio fresco CSPRNG32bytes. Não preencher SID/boot/PID/criação ou handles futuros usando história. O plano é restrito ao equipamento/contexto original da invocação; reboot ou perda de continuidade não restaura a tentativa.

A confirmação nativa é o ato local específico sobre o plano mostrado, não uma assinatura do OS, arquivo de grant ou autoridade derivada do nome/SID. Aprovação deste pedido e confirmação nativa são ambas necessárias ao procedimento. Se houver divergência do plano/contexto, STOP sem confirmar. Autorização desta invocação é usada quando o proprietário inicia o procedimento, mesmo se terminar antes de criar; consumed nativo mede tomada antes dos efeitos, não rearma aprovação administrativa. Nenhum retry automático ou manual sob o mesmo pedido.

## 2. Artefatos byte-exatos finais

Fonte comum: C:\Projetos\PapinhoAccelerator\build\phase6-functional-20261028\Selected\Release.

| Arquivo | Bytes | SHA256 |
|---|---:|---|
| papacc_server.exe |563712|59e8808e1cdacde27cab30a76265b59def2f28362858f9524e06a36980a9941d|
| libssl-3-x64.dll |1321472|3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed|
| libcrypto-3-x64.dll |7217664|09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044|

Total9.102.848bytes. Manifesto FINAL-PACKAGE.json SHA2568fb360e36490af2051bdfa167950718f8f44cc1285286af3b52581604d7af161. O executável cresceu8.704bytes em relação ao candidato01db19a5... por integração solicitada. Este pedido apresenta explicitamente o pacote novo para aprovação, sem promovê-lo por seleção anterior. Orçamentos privados/working set continuam EXATAMENTE9.094.144bytes de crescimento; não foram reajustados para o tamanho novo.

Destino: known folder ProgramFiles resolvido/congelado por SHGetKnownFolderPath, sufixo Papinho\Accelerator; caminho de planejamento C:\Program Files\Papinho\Accelerator. Objetos: diretórios Papinho e Accelerator, arquivos nesses diretórios com os três nomes acima. A identidade nativa não existe antes da criação e não é fabricada. Colisão/preexistência de qualquer diretório/arquivo pretendido implica negação, sem adoção/migração/overwrite. Não toca C:\ProgramData\PapinhoAccelerator/N0, rootC8, CURRENT ou storageCA.

Script manual fixo neste diretório: run-selected-staging.MANUAL.ps1 SHA25688dfb7e8f0de05ca745aecc7fe39d6a9082532260fad2d917aa794c222e4502e. Parsing estático PASS; não executado. Usa exatamente uma chamada --owner-manual-selected-staging com os três hashes esperados, derivando fonte do módulo nativo. Segura três FileStreams read-only (FileShare.Read, não transferência de referência administrativa) após hash e durante a chamada; sem novos arquivos de staging/sink/ledger. Queries/hash/loader e parent PowerShell têm custos próprios opacos, não crédito nem franquia1024. Métricas nativas são do processo do stager; pisos do host incluem coexistência real. Hash em disco e path não são atestação de imagem mapeada; não se afirma proteção absoluta contra troca de ancestrais/falha de loader pelo wrapper.

## 3. Critérios selecionados e congelamento

READY é a única amostra após fontes originais/hashes/volume e geração do desafio, antes da apresentação/confirmar/consumir. Congelamento uma vez no packet original:

- maxHandles = H_READY+5, soma checada; sem descontar herança, handles temporários ou internos. Antes de nova abertura, exigir espaço para a referência adicional. Não usa128/124 para este perfil selecionado; strict/LAB/TPM/SCQ permanecem intactos.
- maxPrivate = Private_READY+9.094.144bytes e maxWorking = WS_READY+9.094.144bytes, independentemente; não somar métricas sobrepostas. Crescimento opaco acima da tolerância observada implica falha, sem ampliar limite.
- Piso de físico disponível6.296.461.312bytes; piso de commit disponível8.479.985.664bytes. Exatos da seleção e da referência histórica phase7 da segunda coleta de staging que FALHOU132handles. Não se reclassifica essa coleta como segura ou PASS; referência não é disponibilidade atual/reserva.
- R = soma dos três tamanhos finais arredondados pela unidade real do volume, com overflow checado. Exigir caller/global >=2R antes do primeiro efeito, >=R depois; unidade/R não podem mudar após congelamento. Espaço livre não é reserva.
- Freshness500ms do início MAIS ANTIGO entre revalidação de identidade/proteção, query de volume e sample de recursos até o gate da submissão. Query falha/tempo regressivo/frequência divergente/overflow nega. Nova amostra não refaz READY, critérios ou consumo.
- Duração120.000ms da primeira amostra nativa até fechamento observado. Inclui leitura/confirmar. Antes de efeito, expiração nega. Depois de efeito, suprime novas submissões e mantém retenção onde incerto. Close deve ser tentado somente segundo lifetime/governança; expiração não é kill/cleanup/cancelamento físico. Se fechamento conhecido terminar fora do prazo, resultado é falha não limpa, sem inventar referência retida já fechada.

Quotas geridas mantidas Selected3.145.728/Legacy2.097.152 e todas as menores quotas aplicáveis. Código reservado/censo e automáticos registrados no closeout. O pedido é Selected/Release, não ativa perfil de memória em outro target. Não existe ceiling demonstrado para provider/kernel; são tolerâncias de risco.

## 4. Efeitos persistentes e protocolo

Uma tentativa local/invocação, máximo cinco criações exclusivas: dois diretórios, três arquivos. Proteção nova O:BAG:BAD:P(A;;FA;;;SY)(A;;FA;;;BA)(A;;GRGX;;;AU), aplicada na criação, sem ACL em objetos existentes. Fontes lidas e copiadas por blocos4096, hash1024/contexto serializado; bytes/hash/tamanho/identidade/descritores e readback verificados. Os bytes finais copiados ficam para qualificação; não há limpeza automática.

Antes de admitir: token/contexto/parent/fontes/volume legítimos, pacote exato, READY/nonce originais, confirmação específica, recursos e relógios válidos. Consumo único imediatamente antes da primeira submissão mutante. Antes de cada criação/write/flush, revalidar originais/proteção e volume/recursos/tempo/Stop. As revalidações de metadata não movem o cursor de cópia; hashes adicionais ocorrem nos pontos de captura/current/readback. Fontes mantidas sem compartilhamento de escrita/delete pelo native open; sem substituir referências por strings.

Verificar resultados e progresso de cada efeito. Query inesperadamente pending, disposição contraditória, readback divergente, erro de recurso/IO, Stop ou close incerto preservam primeira falha e cohort conforme governança; não rearmam consumo. Fechamento único por handle adquirido, sem fechar emprestados/caches. Falha parcial pode deixar diretórios/arquivos e impedir nova tentativa. Sem promessa de atomicidade de cinco objetos ou rollback.

## 5. Riscos residuais reconhecidos

Custos internos do OS/CRT/alocador/cache/metadata/IO continuam UNKNOWN quanto ao máximo/atribuição. Exaustão ou degradação de host e falha de IO ainda podem ocorrer entre samples/dentro de uma chamada. Monitoramento não impõe teto físico; Stop não cancela IO síncrono em andamento; prazo não é hard deadline de kernel. Nem sucesso de um ensaio prova bound universal, quiescência OS, persistência após reboot, ED11 total PASS ou prontidão produtiva.

O wrapper abre arquivos/hash/provider e consulta metadata de boot/identidade; o runtime/loader e metadados de acesso têm efeitos/custos internos opacos. Nenhum deles é declarado zero. O plano não autoriza obter segredos/chaves/ownerAuth. Retenção de dados/backings em UNKNOWN não concede retry, devolução de crédito ou autorização de limpeza.

## 6. STOP

STOP sem novos efeitos em qualquer divergência de pin/bytes/pedido/aprovação/contexto/token/identidade/destino/proteção; reparse/colisão/substituição; recurso abaixo dos pisos/acima dos limites; stale/timeout/overflow/query falha; consumo repetido/owner concorrente; confirmação ausente; erro/pending/resultado contraditório; perda de observação ou close incerto. Nunca baixar piso, mover READY, aumentar tolerância, renomear destino, rearmar ato ou executar novamente para obter PASS. Cancelamento antes da criação deixa efeitos0; após efeitos segue retenção/UNKNOWN.

## 7. Procedimento manual sujeito à aprovação

Não executar agora. Após aprovação integral deste pedido/hash, registrar separadamente OWNER-STAGING-EXECUTION-AUTHORIZATION.json neste diretório, com decision=AUTHORIZE_ONE_OWNER_MANUAL_SELECTED_STAGING, maximum_manual_invocations=1, automatic_execution=false, request_sha256 igual aos bytes deste pedido e package_manifest_sha256 igual ao manifesto acima. Calcular hash literal desse registro; é registro documental de autorização, não ledger operacional/grant persistido. Originalidade/autorização local continuam no contexto/presenter/nonce nativos.

Preparar handoff final com hash do script conferido ANTES da chamada e hash literal desse registro. Script rejeita ausência/divergência do registro; ele ainda não existe. Não se imprime comando com hash futuro inventado. Console já elevado; sem UAC automático/Start-Process. Caso política exija, esta aprovação incluirá EXCLUSIVAMENTE salvar ExecutionPolicy Process atual, aplicar Bypass ao processo PowerShell desta invocação, conferir política efetiva e restaurar no finally. Sem CurrentUser/LocalMachine/GPO/UnblockFile/assinatura. Essa exceção não é executada nesta preparação.

Na única execução autorizada, conferir metadata/pins/arquivos/fontes e a apresentação nativa. Se todos os campos corresponderem, proprietário digita exatamente a confirmação com nonce que o native mostrar. O agente não a digita nem executa. Coletar toda saída do console e exit/result; sem redirecionar a confirmação ou criar log operacional extra. Não repetir se houver falha/UNKNOWN, mesmo effects=0. O registro de aprovação é reconciliado após a saída; não se alega enforcement persistente cross-process de uso único por arquivo/novo ledger.

## 8. Resultado e separações

Resultado saudável limitado exige cinco efeitos verificados, hashes/readback originais, close conhecido/sem retenção e prazo observado; receipt deve indicar qualificação, nunca produção. Ausência de evidência/receipt/continuidade é UNKNOWN, não rollback presumido. Falha mantém primeira causa e disposition exata; aprovação utilizada não equivale a consumed nativo ou sucesso.

Sem SCM/Start/Stop, instalação/NEW_INSTALL real, contas, rede/endpoint, CNG/CA/CURRENT/credenciais, TPM/NV/Q4, limpeza/refund/recuperação ou PST0.6.5. Depois do staging verificado, registro SCM PARADO exige pedido/autoridade próprios e aquisição/readback original dos arquivos staged. PST0.6.4, históricos/pins e demais decisões accepted preservados. E21 PENDING; ED11/Phase6 PARTIAL até evidências próprias.

