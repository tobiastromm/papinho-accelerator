# Destino accepted e despacho SCM/CNG — integração offline

TASK: PACC-PHASE6-BINARY-DESTINATION-ACCEPTANCE-AND-SCQ-INTEGRATION. Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469. WIP/históricos preservados, sem commit/checkout ou efeito operacional. Não se criou diretório de instalação, serviço, conta, chave persistente, CA produtiva ou CURRENT. PST 0.6.4, UNKNOWN e reservas preservados.

## Decisão registrada

A proposta aprovada foi lida integralmente e seu SHA256 literal conferido: `e6d5ab6d57486dbe15d548e68e35dea4fd9abb124852ae36f9143378e9335d4d`. [Registro ACCEPTED](../../adr/DECISION-personal-binary-destination-new-install-20261029.md), escrito antes da implementação dependente. Registro SHA256 `4f8485983a6ad6cd2d8b0527fea1df37a7c522686333b0c45e8eb01446b443f3`.

Especializa somente o destino binário em novas PERSONAL_LOCAL: `%ProgramFiles%\Papinho\Accelerator`. Instalações existentes mantêm path/configuração/identidade originais. Decisão de SCM opcional anterior, proposta original e decisão da conta virtual permanecem byte-exatas; não há migração, adoção, rebind, novo formato ou reaprovação da conta virtual. A divergência focal de destino está resolvida para NEW_INSTALL, sem ampliar outros perfis.

## Código e callers

`personal_install_paths_win32.h/.inc`: resolução de ProgramFiles/ProgramData com SHGetKnownFolderPath, KF_FLAG_DONT_VERIFY, sem flag CREATE, sem filesystem mutation. Buffers da API são liberados por CoTaskMemFree inclusive em falha parcial; o contrato void não é apresentado como prova observada de internals do allocator. Path local absoluto, limites, componentes inválidos e resultado parcial são tratados fail-closed.

O subestado faz parte do packet canônico. NEW congela binários `Papinho\Accelerator`, parent de dados `Papinho`, leaf `Accelerator` e data resultante. EXISTING copia os paths originais supplied pelo owner, não consulta known folders nem aplica os novos defaults. O root recusa layout NEW se já houver inicialização, publicação, preparação/commit ou recuperação; ausência de arquivo não seleciona NEW. As entradas negam alteração após apresentação ou preparação do despacho. Paths são DATA: não demonstram Volume/FileID, proteção, existência de parent ou publicação.

Callers: novas entradas `papacc_configuration_personal_layout_new/existing/current`; presenter NEW_INSTALL existente; revalidação de `personal_packet_current` no mesmo handoff. O presenter resolve/congela e exibe o destino binário antes da confirmação, revalida depois dela e antes do handoff. Parent/container explicitamente informados continuam vinculados aos seus parâmetros e ao handle original sob F3, sem substituição silenciosa pelos defaults; o target pessoal solicitado usa os defaults resolvidos. A helper de EXISTING não move paths, objetos ou dados e não qualifica sua proveniência por strings apenas.

`personal_scq_dispatch_win32.h/.inc`: preparação de DATA de associação e nome bounded, cópia privada, snapshot PID/criação/TokenId/AuthenticationId/ModifiedId, despacho síncrono serializado no MESMO writer. Não há worker/owner/serviço novo ou transferência insegura de handles entre threads. Association é DATA, não intent, aprovação, grant ou prova anti-rollback durável.

Callers: `papacc_configuration_personal_scq_prepare_data/dispatch` e a entrada CNG existente, que só pode usar o nome congelado. O dispatcher rejeita associação trocada, contexto alterado, replay, thread divergente, Stop e retenção. O corpo encadeia o binding original e `personal_cng_probe`; o guard de admissão nativa é verificado antes desse corpo pela entrada do root. Em produção a fonte de autoridade continua indisponível: NOT_SUPPORTED, fase terminal LOCAL_DENY e zero chamadas KSP. Não foi trocado guard por true ou instalado callback permissivo.

O fixture source-linked executa o corpo do despacho com APIs modeladas, sem alegar que a aplicação passou sua admissão. O teste do root prepara DATA coerente e confirma que a entrada real continua negada antes de KSP. Metadados válidos e diretórios resolvidos não concedem execução.

O coletor CNG anterior é reutilizado, não refeito. Distingue identidade SCM configurada e TokenUser real, grupos completos/restrições, PID/criação/continuidade e tipo SID; processo próprio versus observador; thread token/impersonação; negação local; falha de consulta/provider; chave ausente; permissão KSP observada; abertura inesperada; Stop e close UNKNOWN. Não cria/assina/exporta/deleta chave. Guard CA administrativo original permanece intacto. Não há herança da exceção LAB.

Pending/busy/retire e close incluem o despacho. Cancelar uma preparação sem efeito produz receipt STOPPED antes de fechar referências; um despacho em curso ou retido impede teardown que omitiria recursos. Falha de close do coletor/binding preserva UNKNOWN e bloqueia retry/reinicialização. Estado terminal de observação não é admissão nem quiescência global.

## Builds e testes

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings nos logs finais. **24 CTests/configuração, 96 execuções PASS.**

| Evidência focal/configuração | Resultado e alcance |
|---|---|
| Paths | 39 verificações; known folder APIs modeladas, flags sem CREATE, paths não-C preservados, falha parcial/null/UNC/overflow/Stop, free calls, alteração da resolução e EXISTING sem recalcular |
| Dispatch | 59 verificações; MESMO código de dispatch/probe, APIs modeladas, freeze de nome, associação/PID/criação/token trocados, replay/Stop/UNKNOWN; thread concorrente Windows real rejeitada |
| Coletor KSP | 95 verificações existentes preservadas; sem CNG real nesse teste |
| Binding runtime | 91 verificações existentes; SID helpers reais, token/processo/SCM modelados |
| Presenter | 38 verificações/13 cenários; console/preflight modelados, source read/CSPRNG reais e resolução readonly das known folders; sem alvo/chave/CURRENT |
| Root/CA | Regressões ampliadas: owner inicializado não aceita NEW; EXISTING não migra; preparação/dispatch negado/Stop/cancelamento e replay sem chamadas KSP |

As regressões de issuer NORMAL/PUBLISHED, enrollment, M7 e ownership passaram pela mudança funcional dos headers/packet. O fixture criptográfico efêmero mantém 89 verificações, três assinaturas e sete verificações CNG reais; não é MACHINE persistente nem serviço real. Não houve nova abertura/criação de chave MACHINE, uso da chave CurrentUser retida ou execução SCM. Receipt da chave retida conferido por hash somente.

Storage novo de paths: 1.708 bytes; despacho: 344 bytes; alinhamento do root resulta em **+2.056 bytes fixos** versus SCQ. Censo conhecido Selected **1.974.640/3.145.728**, Legacy **1.806.072/2.097.152**. Presenter contabiliza mais 1.040 bytes UTF8 transitórios em sua reserva; resolver/current têm stack e alocações Shell a considerar separadamente. Nenhuma quota ou crédito aumentou. Esses censos não provam pico total, kernel/provider/LSA/CRT ou headroom operacional; NATIVE_ADMISSION=DENY e PROVIDER_KERNEL=UNKNOWN permanecem.

## Destino, parent e prova restante

| Item | Estado atual |
|---|---|
| Destino NEW_INSTALL | ACCEPTED e resolvido/congelado por código |
| Instalações existentes | Paths recebidos do original preservados, sem defaults novos |
| Dados pessoais | Defaults ProgramData/Papinho + Accelerator, perfil lógico; sem pasta Personal |
| Parent | Não adquirido/reservado/criado; resolução de known folder não é identidade de objeto |
| Serviço pessoal/token real | Não instalado/iniciado/observado nesta tarefa |
| KSP MACHINE sob runtime | Não executado; fixtures não são prova factual |
| Despacho | Integrado e qualificado offline; fonte de autorização nativa continua fechada |
| CA/SCM produtivos | DENY; nenhum efeito |

A primeira prova factual continua sendo **uma sessão específica SCM/CNG**, com originalidade do parent/imagem, conta e token reais e a MESMA chave MACHINE fixture original observada pelo administrador e pelo runtime. Ela não precisa de CA operacional/CURRENT anterior, mas também não pode iniciar runtime produtivo sem seus gates. Negação anterior ao KSP, NTE_BAD_KEYSET e ausência de chave não são isolamento PASS. Assinatura/exportação não alcançadas não recebem retornos fictícios.

O primeiro pedido necessário é a sessão descrita em [BDI-FIRST-OPERATION-SCOPE.md](BDI-FIRST-OPERATION-SCOPE.md). **Não é ainda um pedido executável ou uma aprovação:** falta fonte original da autorização diagnóstica vinculada ao despacho, além de imagem/objetos/descritor/recursos frescos. Isso não é uma pendência de layout ou uma necessidade de refazer o probe. Não se emitiu comando com placeholders ou destinado a reencontrar um gate sempre fechado. A aprovação normativa desta tarefa não preenche essa autorização.

## MANUAL/E8 e caminho crítico

E8 continua SOURCE_ABSENT antes de KDF por ausência dos producers originais pacote/observação administrativa/confirmação independente. Nenhum constructor de trust lease, SPKI/CSR positivo ou callback foi fabricado. Enrollment (77 verificações de composição) e M7 exato permanecem qualificados offline, sem presumir emissão, quiescência ou GUI TLS produtivas. Nenhum novo defeito independente demonstrado justificou alterar esses consumidores; suas regressões afetadas passaram. Browser/GUI e os três producers podem avançar sem instalar SCM; não se modificou outro checkout.

Caminho mínimo: (1) ligar a fonte original e de uso único da autorização diagnóstica aos inputs já congelados e ao despacho existente, com guards/recurso/Stop preservados; (2) fechar bindings e limites para UMA solicitação exata, sem repetir paths/binding/probe; (3) após aprovação específica, execução manual e reconciliação do parent/SCM/token/MACHINE/Stop/UNKNOWN; (4) continuar E8/provisão e composição quantitativa em paralelo. Sem nova família normativa, recuperação histórica, TPM/Q4, CURRENT ou CA produtiva.

E17/E18 ainda precisam de observação real, E19/E20 de política/capacidade/composição, E21 histórico permanece independente, E22 é derivado. ED11/Phase6 PARTIAL e autoridade produtiva DENY. [Próximo prompt executável](NEXT-BDI-ONE-SCQ-REQUEST.md).

Evidências e hashes atuais: BDI-QUALIFICATION.json, BDI-SOURCE-HASHES.json e BDI-ARTIFACT-HASHES.json; arquivos anteriores permanecem históricos, sem repinagem.
