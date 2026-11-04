# Pedido operacional — uma consulta manual de metadados da cadeia

Status: READY_FOR_OWNER_DECISION / NOT_AUTHORIZED / NOT_EXECUTED.

## Autoridade e escopo incorporado

Requer autorização operacional explícita do proprietário deste documento byte-exato. Autoriza-se, somente se aprovado, UMA submissão manual da consulta read-only, sem retry automático ou manual sob o mesmo ato. Não autoriza staging nem rearma qualquer aprovação consumida. Consulta atual não reconstrói o descritor não salvo da tentativa de staging.

Incorpora integralmente os limites do pedido preservado `../phase6-root-metadata-readiness-20261031/READONLY-CHAIN-REQUEST.CANDIDATE.md`, SHA256 `a9ffdc0d26ba8583b8e162cf127d5563893e1766f94551ce7ce994258ca6167b`. A presente vinculação completa procedimento, direitos e pins; não aceita implicitamente alteração normativa ou amplia escopo de instalação. F2/F3/F5/F6, D5 e B3.1/B4 continuam protegendo originais e destino; B0–B8 não dispensa proteção.

## Operador, alvo e objetos

Host exato `TCORP-TOBIAS`; SID do proprietário/controlador `S-1-5-21-785699550-1440458426-1682014700-1108`, adquirido de token nativo próprio. Console Windows PowerShell 5.1 x64 já elevado, Administrators habilitado no teste do wrapper. Elevação/SID são contexto, não aprovação do pedido. Native rejeita impersonação e revalida identidade do token original e do token primário fresco. Nenhuma conta é criada.

Known folder ProgramFiles resolvido sem criação e congelado como `C:\Program Files`; destino lógico `C:\Program Files\Papinho\Accelerator`. Consultar somente raiz C:\, Program Files, Papinho se existir e Accelerator se existir. Parar no primeiro ausente; acesso negado/erro não é ausência. Componentes relativos a handles originais; nenhum reparse seguido, irmão listado ou arquivo do destino aberto. Nenhum diretório existente é adotado como instalação. Ausência de raiz/ProgramFiles é falha, não alvo novo.

## Procedimento congelado

Entrada nativa no owner existente: `papacc_server.exe --owner-manual-root-chain-metadata`. O dispatch ocorre antes do fluxo de serviço/servidor. Perfil LAB rejeita essa entrada. Não invoca o preflight de staging ou a abertura que solicita criação.

Direitos exatos dos diretórios: `0x00120080` = READ_CONTROL | FILE_READ_ATTRIBUTES | SYNCHRONIZE; FILE_OPEN, FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT | FILE_OPEN_REPARSE_POINT, OBJ_DONT_REPARSE. Compartilhamento FILE_SHARE_READ, sem SHARE_WRITE/DELETE. Não pede FILE_TRAVERSE explícito, listagem, criação, escrita, DELETE, WRITE_DAC/WRITE_OWNER ou SACL; não ajusta privilégios. Travessia pode depender da elegibilidade corrente do token; negação implica STOP, não fallback de direitos.

Até quatro handles originais de diretórios e dois tokens próprios de consulta no fechamento normal; uma consulta de thread token inesperada pode adquirir referência adicional antes de negar. SID/descritor temporários ficam no packet existente. IO_STATUS_BLOCK e backing de nome permanecem nesse packet se o open retornar PENDING inesperado. A contagem de referências controladas não é teto total do processo/kernel.

GetSecurityInfo solicita somente OWNER|DACL. Saída: identidade volume/file ID, atributos, owner SID, controle, tamanho/hash dos bytes retornados e ACEs ALLOW/DENY básicas em ordem original (tipo/flags/máscara/SID), motivo/índice/máscara do MESMO classificador estrutural de staging. FOREIGN_WRITE/owner divergente podem ser reportados como diagnóstico sem interromper a observação válida. Não equivalem a AccessCheck ou autoridade; ACE não compreendida ou descritor inválido mantém UNKNOWN e encerra. Sem impressão de SACL, blobs condicionais ou conteúdo de arquivos.

Uma passagem e uma revalidação final nos mesmos handles, token/ModifiedId, mapeamento de volume e resolução do known folder. Owner/DACL são comparados por hash dos bytes retornados, sem normalização. Igualdade entre amostras não prova imutabilidade entre elas, ausência de handles anteriores de terceiros ou proteção futura universal. Boot identity fica UNKNOWN; uptime é somente metadado, não substituto de identidade de boot. PID/token/file IDs são adquiridos frescos, não pré-preenchidos.

## Pins e conferências antes da chamada

Diretório do pacote já existente: `C:\Projetos\PapinhoAccelerator\build\windows-virtual-scm-integration-20261022\Selected\Release`.

| Artefato | Bytes | SHA256 |
|---|---:|---|
| papacc_server.exe | 570880 | c58d1eb24fa2754d230ad4d27b61458443557daa8f4bcf4e1baad5dd8d1e5b6b |
| libssl-3-x64.dll | 1321472 | 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed |
| libcrypto-3-x64.dll | 7217664 | 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044 |

Receita revisada: `collect-chain-metadata.MANUAL.ps1`, SHA256 `34b4fa42bc30d4af53866b9a690c3b2125a8f87d6091cc8b87df7fc26b80ea38`. Antes da chamada, conferir hashes do pedido atual, escopo original, receita e três artefatos, além dos tamanhos do pacote. São leituras apenas desses arquivos públicos conhecidos, adicionais às consultas de diretórios; não enumeram ou leem arquivos do destino. Pins em disco não são atestação de imagem mapeada e não garantem ausência de corrida após hashing. Nenhum artefato posterior pode substituir esses bytes sem reconciliação e nova vinculação operacional.

O comando de console derivado contém literalmente o corpo da receita, com parâmetros ligados aos hashes finais e PSCommandPath substituído pelo caminho exato da receita. É colado no console já aberto: não carrega arquivo .ps1 nem altera execution policy, e não usa ScriptBlock.Create/Invoke-Expression, dot sourcing ou mecanismo de evasão de política. O arquivo da receita é verificado como referência de bytes. O comando final é preservado em MANUAL-COMMAND.txt e integra a entrega para decisão; não foi executado. A mera existência da entrada/receita/comando não registra aprovação.

## Efeitos, riscos e STOP

Sem efeitos persistentes de aplicação pretendidos. Há abertura/retenção temporária de handles, alocações e registro/remoção do handler de Stop existente; share READ pode impedir temporariamente novas aberturas incompatíveis. Loader/CRT/OS podem produzir atividade interna opaca; não prometer zero efeitos universais de OS ou limite físico de alocação. Não há nova quota: custos opacos continuam UNKNOWN, sem importação de 124/128/1024 ou capacidade de staging. Testes offline qualificam desenho, não custo observado no host nem sua ACL atual.

STOP na primeira divergência de pin/identidade/host/caminho, reparse, query inválida, substituição, insuficiência de recursos, tipo não suportado, Stop ou fechamento incerto. Ausência legítima de Papinho/Accelerator encerra a cadeia sem criação. Não ampliar direitos para superar falha. Nenhum retry de aquisição/free/close incerto. Primeira falha é preservada; PENDING ou fechamento incerto retêm referências/backings no owner e mantêm DENY. Retorno do processo não prova quiescência OS. Saída somente no console; não criar sink de resultado.

Uma submissão ao nativo usa a autorização manual, mesmo quando retorna falha antes da aquisição completa. NÃO_RETRY. Falha anterior à submissão deve ser reconciliada; não presumir reuso automático. Não registrar execução/consumo/sucesso antes da evidência futura.

Proibidos staging, criação de diretórios/contas/CA, alteração ACL/SCM/registry, Start/Stop de serviço, endpoint, rede operacional, KSP/CNG, RNG de cerimônia, CURRENT, TPM/NV/Q4, cleanup/refund, recuperação ou promoção produtiva. PST0.6.4, históricos/pins e autorizações consumidas preservados.

## Reconciliação posterior e limites de conclusão

Após saída manual: verificar submissão única, primeira falha, objetos existentes/primeiro ausente, originalidade e revalidação, owner/ACEs/flags, razão estrutural e fechamento/retidos. Separar ADD_SUBDIRECTORY de DELETE_CHILD, DELETE no filho e alterações de ACL/owner por relação parent/filho. Não declarar proteção efetiva universal ou substituir análise de risco por classificador positivo. Nenhuma correção da guarda ou nova tentativa de staging decorre automaticamente da coleta.

Consulta ainda NOT_EXECUTED; staging DENY; E21 PENDING; ED11/Phase 6 PARTIAL. Próximo gate é aprovação específica deste pedido e a execução manual única pelo proprietário, seguida de reconciliação; nenhuma aprovação operacional foi inferida nesta preparação.
