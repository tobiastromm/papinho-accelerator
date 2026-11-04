# Phase 6 — presenter nativo de primeira instalação

**PRESENTER/BINDINGS: QUALIFIED_OFFLINE.** A instalação operacional não foi executada. Admissão produtiva continua DENY; Phase 6/ED11 PARTIAL.

## Implementação

A revisão encontrou `server_launcher_personal_win32.inc` já integrado ao launcher, com token/parent/source retidos, CSPRNG, apresentação e confirmação NEW_INSTALL somente DATA. Foi estendido esse mesmo presenter, sem nova root, serviço, owner ou CLI automática.

A nova entrada `papacc_server_launcher_personal_ca_present` pede explicitamente o propósito conjunto NEW_INSTALL/CA_INITIALIZE. Valida a representação do SID candidato, copia os IDs e SID solicitados antes de inicializar a root, recusa identidade igual ao operador e usa o plano conjunto existente. Os valores pedidos são apresentados como candidatos, não como fatos de custódia ou identidade do runtime.

O presenter mostra operador observado, parent e source por identidade nativa, nome relativo congelado, configuração exata com tokens escapados, efeitos persistentes e plano conjunto byte-exato escapado. O parent/leaf apresentados vêm do pacote retido, não de uma releitura de argumentos mutáveis. A confirmação é `OWNER_NEW_INSTALL_CA_INITIALIZE <tentativa CSPRNG>`; a rota anterior continua exigindo somente NEW_INSTALL. Escape cancela, Stop invalida, redirecionamento é negado e o CAS do estado do launcher impede instâncias concorrentes. Não se alega presença humana ou autorização OS do plano por bytes do console.

Foi acrescentado um receipt interno ao contexto nativo existente: tentativa, propósito e disposição (apresentado, confirmado, negado/cancelado, UNKNOWN). O resumo atribuído é emitido no mesmo sink do launcher, sem novo formato persistente ou arquivo de aprovação. Confirmação e fechamento são registrados separadamente. Falha de fechamento/worker pendente retém o owner e registra UNKNOWN.

O `personal_handoff_begin` existente agora aceita o plano conjunto **somente em sua submissão read-only**: consome o plano exato antes da única submissão e usa o mesmo worker que revalida token/parent/source. Nenhuma chamada ao target worker, CA key create, commit, assinatura ou publicação foi adicionada ao presenter. O guard `personal_native_ceremony_ready` permanece fechado; a existência/receipt do presenter não o concede.

## Bindings e efeitos

| Owner/contrato | Ligação demonstrada |
|---|---|
| Launcher/root | Estado e exclusividade originais; novo propósito na mesma função de apresentação |
| Configuração | Source original retida, parser/validator existentes, bytes congelados e revalidação |
| NEW_INSTALL/CA_INITIALIZE | Plano conjunto já aceito, confirmação explícita e consumo read-only; execução continua separada |
| CNG/custódia | SID/IDs solicitados no plano; nenhuma aquisição de chave durante a apresentação |
| Issuer NORMAL | Caminho já integrado; a apresentação não cria fonte factual NORMAL |
| CAS/A/P/CURRENT/epoch | Contratos/worker v2 reutilizados em fixtures; sem nova publicação ou epoch fabricado |
| Lifecycle | Console/Stop e fechamento do mesmo owner; nenhum SCM/Start/Stop de serviço |

## Qualificação

- Selected/Legacy Debug/Release: PASS, zero warnings.
- 18 CTests/configuração: **72 execuções PASS**.
- Presenter: **13 cenários, 37 verificações/configuração**. CSPRNG, leitura do arquivo de configuração e parser reais; console, contexto nativo e submissão/fechamento do worker substituídos exclusivamente na tradução de teste. Nenhum callback positivo foi instalado em produção.
- Cenários: confirmação conjunta, cancelamento, divergência de identidade/parent, source alterada, contexto com CA/target indisponível, fechamento incerto, Stop, stdin redirecionado, root ocupada, falha de saída, confirmação incorreta e worker pendente. Rota anterior NEW_INSTALL preservada.
- Preflight nativo read-only real: **DENIED_NO_ELIGIBLE_CONTEXT_OR_PARENT; POSITIVE_NOT_QUALIFIED**. Não foi convertido em PASS de elegibilidade.
- Oito fixtures nativos v1/v2 do root/worker: PASS após a mudança de layout. Confirmação A/P e guard NORMAL preservados.
- Os 2.718 checks do modelo CNG/configuração e a jornada efêmera passaram na regressão. A criação persistente de teste **não foi repetida**, e a chave retida não foi aberta, alterada ou excluída nesta tarefa.

O layout gerenciado aumentou 40 bytes: Selected 1.972.056/3.145.728; Legacy 1.803.488/2.097.152. O presenter contabiliza também suas cópias locais do pedido na reserva. Isso não qualifica picos ou provider/kernel; custos opacos seguem UNKNOWN, sem quotas/headroom ampliados.

## Custódia CurrentUser versus MACHINE

Não há lacuna normativa nova: `DECISION-personal-ca-preinitialization-20261022.md` P0 exige Microsoft Software KSP MACHINE para a CA pessoal/local; P3 exige seleção legítima de conta local padrão distinta para o runtime e posterior observação de seu TokenUser. P4 exige negativa efetiva de uso privado pelo runtime e custódia administrativa qualificada. O nome de conta, privilégio administrativo ou SID candidato não comprova isso.

A demonstração CurrentUser de teste permanece válida exclusivamente no seu escopo. Não comprova MACHINE, ACL, isolamento da identidade operacional ou reboot. O presenter não cria conta, modifica ACL ou escolhe SYSTEM/LocalService/Administrators implicitamente. Hosting SCM e perfil de token devem ser qualificados separadamente, sem confundir conta SAM, virtual account e per-service SID. Não foi proposta nova ADR para substituir esses contratos.

## Primeiro gate e continuação

O próximo gate factual é qualificar o **contexto nativo elegível e as identidades administrativa/operacional reais**, com parent/source originais e política aplicável. O preflight real atual negou antes disso. Também faltam custódia MACHINE e isolamento do token operacional; não há pedido pronto para executar a primeira instalação.

A confirmação read-only do plano não equivale à autorização operacional, à criação do target, à CA ou a CURRENT. A futura execução NEW_INSTALL/CA_INITIALIZE precisa autorização específica e todos os gates; nenhum comando executável com placeholders foi criado.

## MANUAL/AUTOMATIC e Browser

M7: preservados digest exato, isolamento da credencial não relacionada e Stop versus fechamento. MANUAL/E8 ainda depende de fonte original/delegação legítima: o Browser M1 para em TRUST antes de KDF e não pode consumir um receipt do presenter como grant. AUTOMATIC/enrollment mantém o claim/trace já qualificado, mas L2/fontes e integração operacional permanecem pendentes. Regressões de enrollment passaram; nenhum formato ou callback de trust foi alterado.

A preparação concreta dessas frentes e da qualificação do serviço está em `SERVICE-AND-PROVISIONING-HANDOFF.md`. Não se continuou refinando criptografia do issuer nem se retomou TPM/Q4. Nenhuma alteração no Browser/PST ou publicação 0.6.5 ocorreu.

## Rastreabilidade

`QUALIFICATION.json`, preimages, `FUNCTIONAL-DELTA.patch`, `SOURCE-CHANGES.json`, logs e `ARTIFACT-HASHES.json` registram o WIP e hashes. HEAD, ADRs accepted, históricos, pins, UNKNOWN, reservas e chave de teste retida foram preservados. Próxima continuação técnica: `NEXT-NATIVE-CONTEXT-AND-PROVISIONING.md`.
