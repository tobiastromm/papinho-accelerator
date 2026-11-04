# Alvo pessoal — preparação factual SCM/CNG

TASK: PACC-PHASE6-PERSONAL-TARGET-AND-SCM-CNG-READINESS.

Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469. Complemento independente ao closeout anterior, sem reescrever suas evidências. Nenhum diretório de instalação, serviço, conta, chave ou aprovação foi criado. Nenhuma consulta SCM real foi executada. Autoridade DENY; ED11/Phase 6 PARTIAL.

## Resultado e divergência delimitada

Uma única sessão futura pode qualificar parent, identidade SCM e isolamento de uma chave MACHINE fixture, sem CA produtiva, CURRENT ou root anterior. Não há ainda pedido operacional completo: faltam funções e bindings concretos abaixo. Não consumir autorização de Start para reencontrar esses bloqueios previsíveis.

**GOVERNANCE CONFLICT:** [SCM opcional accepted](../../adr/DECISION-personal-windows-optional-scm-20261022.md) seleciona `%ProgramFiles%\PapinhoAccelerator`. A preferência posterior propõe `%ProgramFiles%\Papinho\Accelerator`; a [decisão virtual](../../adr/DECISION-personal-scm-virtual-account-20261029.md) não alterou o destino. A revisão anterior não reconciliou essa seleção. Não se muda o accepted ou presume aprovação. Conservar o destino anterior dispensa nova decisão; o novo destino requer somente [reconciliação focal](TARGET-BINARY-DESTINATION.CANDIDATE.md), sem nova família normativa. A conta virtual já está resolvida. Hosting SCM continua opcional: sua prova não bloqueia implementação console, MANUAL/E8 ou enrollment independentes.

## Parent e layout

Resolver FOLDERID_ProgramData por SHGetKnownFolderPath sem flag de criação, liberar o buffer por CoTaskMemFree mesmo em falha e congelar o path absoluto antes da confirmação ([Microsoft](https://learn.microsoft.com/en-us/windows/win32/api/shlobj_core/nf-shlobj_core-shgetknownfolderpath)). Não usar variável de ambiente como identidade. O adapter atual exige path ASCII local abaixo de MAX_PATH; fora do domínio, DENY sem fallback.

Âncora original: a known folder existente. Abrir com proteção contra reparse, reter handle e registrar Volume/FileID, owner/descritor e direitos; proteger/revalidar a cadeia relevante contra substituição. Uma futura autorização de preparação pode permitir exatamente um create exclusivo do filho relativo `Papinho` sob essa âncora, com descritor revisado aplicado à criação. Não usar mkdir -Force, OPEN_ALWAYS, criação recursiva ou remediação de ACL existente. Retorno de create, não path reaberto, estabelece o novo parent original.

Se `Papinho` já existir, parar a rota create-only. Sua seleção como parent exige exame/ato explícito de identidade, direitos e proteção, sem confundi-lo com root publicada. P3 exige NOVO contêiner, não que todo parent seja novo. A criação de `Accelerator` fica para NEW_INSTALL: criar esse diretório antecipadamente tornaria o contêiner inelegível mesmo vazio. PERSONAL_LOCAL é lógico, sem pasta Personal. A confirmação F3 da root ocorrerá somente depois da aquisição factual do parent correspondente; a autorização de preparar o parent não é confirmação NEW_INSTALL/CA_INITIALIZE.

A proteção deve separar administrador legítimo, runtime, writer/root/history, logs e KSP. Não conceder escrita em namespace/estado de autoridade ao runtime, Users ou Everyone por conveniência. O descritor exato e sua herança ainda precisam de validação de direitos efetivos sob os tokens reais; não copiar RAW1 de N0 nem assumir que ACL presente prova isolamento. Trustees SYSTEM/Administrators de proteção não se tornam identidade operacional.

Fato de implementação: personal_native_open apenas abre/revalida o parent existente e verifica DACL presente/não nula naquele trecho; não cria ancestrais. bootstrap_create_child usa FILE_CREATE relativo ao parent e depende de herança, sem descritor explícito próprio. Portanto o preflight TEMP/elevado anterior não prova a proteção deste alvo. Falha após create, troca de objeto ou close incerto preserva efeitos/referências; não excluir, adotar, repetir ou conceder refund. A observação anterior ItemNotFoundException não é reserva nem identidade protegida; não foi repetida como microcoleta.

## Sessão factual mínima proposta

Esta tabela é desenho técnico, **não pedido operacional**. Máximos de efeitos propostos não concedem quotas. Não há SID, chave, intent ou hash futuro inventado.

| Fase | Efeitos máximos futuros | Critério factual |
|---|---|---|
| 0. Congelar plano | Nenhum efeito persistente | Operador elegível, destino reconciliado, âncora original, imagem/dependências, configuração e descritor exatos; aprovação específica e confirmação antes do efeito correspondente. |
| 1. Parent | Um create exclusivo de Papinho, se ausente e autorizado | Identidade e descritor do objeto retornado; herança/direitos qualificados. Colisão/reparse/UNKNOWN para. Não criar Accelerator. |
| 2. Chave fixture | Uma chave MACHINE nova sem overwrite, sem certificado CA | Owner/KSP existentes; RSA2048/e65537, signing-only, export0; identidade/SPKI/propriedades e uma assinatura de challenge administrativo verificada. Não usar chave retida ou N0. |
| 3. Hosting pessoal | Um registro do serviço, se ausente, e no máximo um Start | MESMO runtime/owner e imagem original; nome PapinhoAccelerator, virtual logon, OWN_PROCESS, tipo SID esperado. Sem serviço diagnóstico paralelo, rede ou admissão produtiva. Colisão não permite rebind. |
| 4. Token e acesso | Uma observação correlacionada e uma tentativa de abertura KSP sob runtime | Conta/tipo/PID SCM; TokenUser/grupos/censo/atributos/restrições; PID/criação/continuidade; originalidade da imagem e chave; provider fresco no processo runtime, sem handle CA herdado/emprestado. |
| 5. Stop | Até um Stop seguro do processo original | Separar pedido, término, fechamento e quiescência. Falha/UNKNOWN não permite retry, kill, timeout como cleanup ou nova operação. |
| 6. Reabertura admin | Novo provider e uma segunda assinatura fixture, se ainda elegível | Mesma chave/propriedades públicas; closes governados. Não é reboot, recuperação ou proof de CA operacional. |
| 7. Final | Somente closes governados | Primeira falha e efeitos individuais; chave, parent e registro de serviço persistentes retidos, sem delete/refund/rollback automáticos. |

ServiceSidType UNRESTRICTED corresponde à primitiva ps_install_virtual existente. NONE é aceito pelo binding somente quando explicitamente esperado; RESTRICTED permanece negado até prova adicional. Não trocar o tipo para obter PASS. Resolver SID legitimamente e observá-lo no TokenUser real; não usar SID LAB/derivado como prova. SERVICE_RUNNING diagnóstico não significa produto pronto. A fase diagnóstica tem de existir explicitamente no owner, sem executar o runtime produtivo pela falta de CURRENT/ED11.

A qualificação de CNG deve manter as referências originais da chave sob o administrador enquanto correlaciona o teste runtime, sem transferir handle privado. Depois, fechar e reabrir por provider novo sob o mesmo administrador; revalidar propriedades/identidade pública antes do segundo challenge. O fixture testa mecanismos MACHINE e isolamento no equipamento, não cria CA/certificado produtivo nem concede emissão.

O diagnóstico atual ca_key_readonly_diagnostic_win32.c chama papacc_ca_key_open; ca_identity rejeita TokenUser runtime ANTES de NCryptOpenKey. Esse resultado prova o guard local, não negação Windows/KSP. O probe futuro precisa alcançar a API no processo virtual observado, preservando o guard produtivo administrativo. NCryptOpenKey deve usar MACHINE|SILENT; NTE_BAD_KEYSET significa chave não encontrada, não PASS de isolamento ([Microsoft](https://learn.microsoft.com/en-us/windows/win32/api/ncrypt/nf-ncrypt-ncryptopenkey)). Falha de provider/query, API não alcançada e guard local são INCONCLUSIVE. Registrar origem/API/status bruto sem misturar DWORD e SECURITY_STATUS.

Se ocorrer negação nativa de permissão e não existir handle privado herdado/emprestado/prévio no teste, assinatura e exportação pelo caminho fresco ficam inacessíveis. Registrar NOT_REACHED_OPEN_DENIED, não inventar retornos de SignHash/ExportKey não chamados. Se abrir inesperadamente, FAIL e Stop governado; não assinar/exportar para investigar. Não retornar bytes privados nem testar exportação privada positiva. Export-policy zero, negação do caminho e impossibilidade universal são afirmações diferentes; não prometer isolamento contra administrador/OS comprometido. Não chamar CNG no caminho StartService, pelo risco de deadlock documentado ([Microsoft](https://learn.microsoft.com/en-us/windows/win32/api/ncrypt/nf-ncrypt-ncryptcreatepersistedkey)).

O pedido final precisa justificar duração, freshness, headroom e recursos do perfil: observador/admin, serviço, KSP/LSA, stack, CRT, sampler e retenção. Cinco handles do binding não são censo total. Não copiar limites do LAB/TPM, usar máximo amostrado como ceiling, excluir participante ou duplicar charge de papéis alias. UNKNOWN material mantém DENY.

## Readiness — primeira lacuna técnica

ps_binding_ready é `return 0`: instalação/start e ServiceMain pessoal negam. personal_native_ceremony_ready também é falso. Não há hoje entrada governada completa para a sessão acima. Faltam integração de autorização/parent/descritor/imagem/recursos no MESMO owner, fase diagnóstica sem admissão produtiva e probe KSP distinto do guard local. Não substituir esses guards por true, herdar LAB ou emitir comando que certamente falhará antes da observação.

Não existem ainda executável final dessa sessão, descritor revisado, bindings frescos, identidade de fixture e censo transitivo qualificados para pinagem. Por isso **nenhum pedido operacional, aprovação ou comando incompleto foi produzido**. Isso é uma fronteira técnica concreta, não falta de TPM, segredo ownerAuth, CA anterior ou root histórica C8.

## Frentes paralelas e qualificação

MANUAL/E8: shared_structure/trust.h mantém três objetos opacos (pacote, observação administrativa e confirmação independente). trust_unavailable.c não possui producer/lease factory. manual_package_pdg.c só correlaciona DATA; manual_package_validation_openssl.c retorna SOURCE_ABSENT em TRUST antes do KDF. O contrato D3 accepted admite entrega presencial e administrador reconhecido, mas não torna SPKI/CSR/checkbox uma fonte. Nenhum formato persistido, callback positivo ou canal extra foi inferido.

Enrollment/issuer/PUBLISHED/entrega e M7 exato já têm implementação/regressões offline válidas. Não foi encontrado defeito concreto novo que justificasse repetir campanhas ou alterar esses caminhos. O próximo trabalho independente é compor produtores originais E8 com os owners e o consumidor Browser, preservando os três lifetimes. GUI TLS pode apresentar estado/receipts e consumir a evidência; não cria confiança por mostrar uma confirmação. Browser não foi alterado nem sua qualificação Legacy/NT4/capacidade foi presumida.

Sem mudança funcional, não se repetiram builds nem os 84 CTests. TARGET-READINESS.json verifica hashes das fontes e dos doze executáveis do pacote anterior, proposta/decisão, guards e receipt da chave retida; isso é integridade/reutilização, não nova prova SCM/CNG. Os artefatos de evidência anteriores permanecem imutáveis.

Menor sequência: reconciliar destino ou manter accepted; integrar o caminho isolado da sessão e o probe nos owners; qualificar falhas/Stop/UNKNOWN e censo aplicável; congelar artefatos/bindings e emitir UMA solicitação exata para execução manual autorizada. Em paralelo: E8/Browser/enrollment/M7, sem esperar instalação. E17/E18 continuam pendentes de operação; E19/E20 de política/capacidade/composição; E21 histórico independente; E22 derivado. [Próximo prompt executável](NEXT-BOUNDED-SCM-CNG-QUALIFICATION-PATH.md).
