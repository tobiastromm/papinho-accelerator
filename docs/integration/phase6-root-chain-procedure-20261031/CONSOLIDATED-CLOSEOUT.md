# Consulta read-only da cadeia — implementação e qualificação offline

Task PACC-PHASE6-ROOT-CHAIN-METADATA-PROCEDURE. Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP/históricos preservados. Consulta real NOT_EXECUTED; aprovação operacional NOT_GRANTED; staging DENY.

## Implementação entregue

`apps/server/server_launcher_chain_metadata_win32.inc` implementa a entrada `--owner-manual-root-chain-metadata` no launcher/owner canônico, usando o packet e slots de referências existentes. `main.c.api3.inc` despacha antes de serviço/rede; LAB nega. Demais builds sem o perfil necessário têm stub DENY. Nenhum owner, serviço, root, ledger, IPC ou formato de autoridade novo.

Não utiliza `personal_native_open` nem o preflight completo de staging: ambos pedem mais direitos. Abre somente diretórios existentes, relativos ao parent original, com READ_CONTROL | FILE_READ_ATTRIBUTES | SYNCHRONIZE (`0x00120080`), sem FILE_TRAVERSE explícito ou ajuste de privilégios, compartilhamento READ, FILE_OPEN, opções de diretório síncrono e reparse não seguido. Consulta raiz, ProgramFiles e Papinho/Accelerator se existentes; para no primeiro ausente. Acesso negado não vira ausência; raiz/ProgramFiles ausentes são falha. Nenhuma criação ou adoção.

O controlador é observado via token próprio e comparado com o SID esperado; impersonação nega. Retém token original, reconsulta TokenUser/TokenId/ModifiedId e compara com token primário adquirido fresco antes do fechamento. Known folder/mapeamento do volume, identidade e atributos dos objetos e hashes OWNER+DACL são revalidados. Boot identity permanece UNKNOWN; uptime não é prova de boot.

OWNER e DACL são lidos via GetSecurityInfo no handle original, sem SACL. Saída preserva ordem/tipo/flags/máscara/SID de ACEs básicas, controle/proteção, owner, bytes/hash e motivo do classificador original. Classificação FOREIGN_WRITE/owner não confiável é dado diagnóstico, não bloqueio da própria leitura válida e nunca autoridade para staging. Descritor inválido/tipo não suportado encerra com UNKNOWN; não expõe blobs condicionais ou segredos. Não implementa AccessCheck geral.

O corpo do classificador foi extraído, byte-exatamente e sem mudança de política, para `server_staging_descriptor_win32.inc`, compartilhado com staging e o teste. Comparação contra a preimagem confirmou igualdade integral. A guarda de staging continua rejeitando AU/0x4; nenhum predicado foi dispensado.

Close/free é único; falha incerta não é repetida. IOSB, handle e nome de open inesperadamente PENDING permanecem no packet existente, retendo owner. Stop impede novas aquisições e revalidações dependentes, sem promessa de cancelar chamada síncrona já em andamento. Perda de continuidade e fechamento incerto preservam primeira falha/UNKNOWN. A consulta não toma o ato de staging ou restaura autoridade F0.

## Recursos e garantias

Sem novo storage estático: reutiliza `PAPACC_QUALIFICATION_STAGING` e contexto pessoal existentes, incluindo o IOSB retido. A reserva gerida soma o censo C8 existente, storage do serviço, LONG e o censo conservador de automáticos nomeados `CHAIN_KNOWN_AUTOMATIC_BYTES`, cobrindo frames síncronos e hash. Não altera caps Selected/Legacy ou 124/128/1024 dos demais domínios. O censo de automáticos não é ceiling de stack ABI/CRT, heap, GetSecurityInfo, loader/kernel ou consumo total. Esses custos permanecem opacos/UNKNOWN.

Até quatro diretórios e dois tokens de consulta próprios ficam simultaneamente vivos no caminho normal; thread token inesperado pode acrescentar referência antes de negar. Falha/close incerto pode reter todo o cohort. FILE_SHARE_READ pode bloquear temporariamente novas aberturas incompatíveis; erro de compartilhamento é STOP, não motivo para ampliar direitos. Não há garantia de imutabilidade entre amostras, ausência de handles anteriores de terceiros ou resistência a OS/admin comprometido. Nenhum limite B3 foi transplantado para a consulta.

## Qualificação realizada

| Configuração | Build MSVC /W4 /WX | CTests afetados |
|---|---|---|
| Selected Debug | PASS, zero warnings | 4 PASS |
| Selected Release | PASS, zero warnings | 4 PASS |
| Legacy Debug | PASS, zero warnings | 4 PASS |
| Legacy Release | PASS, zero warnings | 4 PASS |

Builds serializados, `--parallel 1`, no toolchain/target/backend substitution. Targets: servidor, teste de cadeia, staging e close guard. Quatro CTests por configuração: root_chain_metadata, qualification_staging, staging_resources e staging_close_guard; total 16 execuções PASS. Não se repetiu campanha histórica geral inalterada.

`tests/root_chain_metadata_test.c`: 26 cenários e 126 verificações por configuração. Positivos modelados de cadeia completa, Papinho ausente e Accelerator ausente; AU/0x4; herança/INHERIT_ONLY/INHERITED; owner divergente como diagnóstico; reparse; identidade substituída; aquisição negada; pending; Stop; token modificado e token primário trocado; host/contexto/layout divergentes; falha de uninstall; close/free incertos; DACL nula; falha de query; descritor alterado; falha de identidade; raiz ausente; ACE não suportada e rejeição de nova entrada no owner retido.

As aquisições de diretórios, consultas de objetos/token/layout, transporte de API e console são simulados. Codecs Windows de SID/SD e SHA256 em memória são reais. O sink temporário do teste é um artefato isolado de teste, não leitura do destino ou coleta operacional. Não executamos o novo comando do servidor, nem consultamos a ACL atual da raiz. Os testes não provam a configuração real do host ou proteção efetiva atual.

Receita e bloco de console receberam parse AST, whitelist de comandos, verificação de uma única chamada nativa, ordem dos gates de hash e equivalência literal do corpo. Essa validação do wrapper é estática, não execução end-to-end no PowerShell do proprietário. Falhas intermediárias de compilação do teste (shadowing/signedness) foram corrigidas antes dos resultados finais; nenhuma redução de /WX.

Logs e resultados: BUILD-VERIFICATION.json, quatro pares build/tests.log e MANUAL-STATIC-VERIFICATION.json. Sem operação protegida, coleta real, staging, ACL/SCM, serviço, endpoint ou chave operacional.

## Vinculação e pedido pronto para decisão

Pedido original preservado e reconferido: SHA256 `a9ffdc0d26ba8583b8e162cf127d5563893e1766f94551ce7ce994258ca6167b`.

[Pedido operacional completo](ONE-CHAIN-METADATA-REQUEST.CANDIDATE.md): SHA256 `df0e9ee77bf61aabf7535e2037fa5788dff6c1a052b0748949cf5cd76d0afa94`.

Receita SHA256 `34b4fa42bc30d4af53866b9a690c3b2125a8f87d6091cc8b87df7fc26b80ea38`; [comando de console](MANUAL-COMMAND.txt) SHA256 `b185035c0394c5f82d08565fbbc9a16efe13b70300f26d386e01a76bdc16b693`. O bloco é colado em Windows PowerShell 5.1 x64 já elevado; verifica hashes antes da chamada e não carrega o .ps1, altera execution policy ou usa eval/dot-source. Não foi executado. Os pins e estados approved=false/executed=false estão em PROCEDURE-PINS.json.

Binário Selected/Release 570880 bytes, SHA256 `c58d1eb24fa2754d230ad4d27b61458443557daa8f4bcf4e1baad5dd8d1e5b6b`; DLLs e tamanhos/pins exatos no pedido. Hash de disco não é atestação de imagem mapeada; não se promete eliminar corrida entre hash e loader. Nenhuma imagem instalada ou selecionada para staging foi substituída por este pin.

## Norma, limites e próximo gate

OrganizationEngineering ADR0001/0003 e PapinhoEngineering ADR0006/0007 delimitam aplicabilidade, documentação, continuidade e backend; F2/F3/F5/F6, D5, B3.1/B4 e decisão accepted de destino mantêm originais/proteção/ausência de adoção. Não há alteração accepted nem decisão normativa nova; future direction de listeners lida permanece fora deste comportamento e não foi promovida a implementação.

O primeiro gate agora é exclusivamente autorização operacional específica do pedido completo e execução manual única pelo proprietário. Não há lacuna técnica declarada para preparar essa consulta; há fatos que somente sua execução autorizada poderá produzir. Não executar automaticamente nem reusar a autorização a928... consumida. Não declarar a observação histórica/current como equivalente.

Após resultado: reconciliar objeto/descritor/ACEs e fechamento; analisar cada relação parent/filho, separando criação de irmão e substituição do filho protegido (DELETE_CHILD, DELETE, WRITE_DAC/WRITE_OWNER e condições reais). Somente então aplicar correção mínima compatível, se demonstrada, ou manter DENY apontando risco factual; eventual decisão normativa deve ser concreta, não inferida. Novo staging e registro SCM parado continuam autoridades distintas. E21 PENDING; ED11/Phase6 PARTIAL; produção PST0.6.4, chave fixture, históricos/pins/UNKNOWN/reservas preservados.
