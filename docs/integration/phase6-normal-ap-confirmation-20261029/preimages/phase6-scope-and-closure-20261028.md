# Phase 6 — escopo recuperado e plano de conclusão

Auditoria técnica/documental de 2026-10-28. Não é ADR, autorização operacional ou dispensa de gates. Substitui apenas a sequência de trabalho derivada que tratava a descoberta/bootstrap C8 como próximo passo automático de todo o ED11. Não modifica decisões accepted nem apaga históricos.

Accelerator: main / f341985924d29870d8ff627ed37c8a0874ad6469. Browser canônico existente: C:\Projetos\PapinhoBrowser, HEAD 96f6cd2f9ec4f834999828fa13bb7d170db66a98. WIP de ambos preservado; Browser somente lido. PST produção 0.6.4/API2, candidato normal 0.6.5/API3/SPI4 e R2 TEST_ONLY permanecem separados.

## Conclusão que orienta a execução

O fechamento integral tem três conjuntos: núcleo Network/TLS histórico; integração Browser/Mode C e gates conjuntos; instalação/provisão pessoal adicionadas por decisões expressas. A recuperação documental Q4 é obrigação real de uma tentativa LAB consumida e ocupa E21 da matriz LAB atual. A rota escolhida para essa recuperação passou a exigir ledger/testemunha TPM. Isso não transforma TPM, NV, ownerAuth ou STORAGE_20 em pré-requisitos universais da primeira instalação pessoal ou de toda qualificação offline C2/C3.

Não há fundamento para fechar E21, liberar reservas ou ignorar incompatibilidades. Tampouco há fundamento para manter todo trabalho do produto esperando E21. A integração C8/issuer/configuração é necessária no produto por autoridades próprias; não deve ser criada para satisfazer artificialmente o diagnóstico Q4. Ausência de manifesto no locator não prova inexistência de root no equipamento e não autoriza bootstrap.

## Roadmap original e ampliações rastreáveis

| Camada | Origem | Conteúdo obrigatório / limite |
|---|---|---|
| Núcleo original Network & TLS Offload | phase6-network-tls-offload.md; ADR-0015 a ADR-0024 | Capability negotiation, Jobs/Streams bounded, Destination Policy/Egress Profile, DNS único, TCP_PLAIN, TLS_REQUIRED/Mode B e lifecycle. READY histórico preservado somente nesse escopo. |
| A–D e integração conjunta | ADR-0022/0028/0029; Browser ADR-0007; mode-c-c2-joint-contract-gate.md | A local/local; B remoto/remoto; C rede Browser/TLS Accelerator; D rede Accelerator/TLS Browser. Não há Mode E. C0–C12 são o roadmap conjunto, não nomes dos componentes C8/ED11. |
| C2 encoding/Model E/ED08/ED11 | ADR-0034 e especificação C2 revision 1.0; consolidação canônica 18/10 | Contrato/encoding aceitos separados de implementação, qualificação e habilitação. ED11 exige baseline/incremento/total/picos/retidos/shared/host; não exige TPM genericamente. |
| Provisão de produção | DECISION-production-client-provisioning-20261018, itens 1–8 | MANUAL e AUTOMATIC permanentes, OPEN/CLOSED enrollment, auto/manual approval, hot update e renovação/revogação no mesmo issuer/root/cadastro. Importação granular R1 não basta. |
| MVP pessoal Windows | DECISION-phase6-personal-mvp-20261022 M0–M11 | Instalação/configuração/SCM/CA protegida/ambos provisionamentos/Browser/listagem/revogação/histórico/uninstall. M3 adia recuperação antiga no perfil pessoal não recuperável, mantendo integridade original/CAS/serial/custódia/ED11. M7 exige invalidar admissões e Stop factual por credencial. |
| Listeners/live config | DECISION-personal-listener-selection-direction-20261023 | Direção accepted; gramática/defaultLan e cláusulas técnicas pendentes não se tornam implementação. Reusar IDs/coleção/owner; não importar exposição genérica ou rede distribuída. |
| Diagnóstico LAB N0/Q4 | decisões N7, B/H/P/Q/R, cross-boot, W/L/L1/TC | Aplicabilidade exata à tentativa/perfil/recuperação especificados; não constituem uma nova definição global da Phase 6. Reservas e UNKNOWN históricos continuam obrigações. |

## C0–C12: estado recuperado sem reiniciar implementação

A matriz CSV é a única tabela de rastreabilidade desta auditoria; contém cada C0–C12, cada E01–E22, A–D e entregas de provisão/GUI/Legacy/NT4. As linhas se sobrepõem e não formam denominador somável de qualificação.

A recuperação de 11/10 é fonte de requisitos originais, não inventário atual de implementação. ADR-0034 resolveu o contrato/encoding antes bloqueado em C2. Os closeouts Browser api3-real-composition e c2-s6-canonical-implementation de 18/10 registram composição real de código e testes offline posteriores: Session durável, owner/grupo/leases, job C2, papinho_c2_s6.inc, RECORD/APPLICATION e teardown. Isso corrige a interpretação de C1/C2/C3 como totalmente ausentes. Não fecha HTTPS real conjunto, E1–E12, GUI inteira ou NT4. API3_MODEL_E_INTEGRATION=NOT_IMPLEMENTED no checkpoint inicial de 18/10 não prevalece sobre o closeout posterior api3-real-composition; nenhum deles é apagado.

E5/E11 históricos de lifecycle e roadmap C7/C11 não são E05/E11 da matriz LAB nem os identificadores internos ADR-0031/0032. Preservar suas provas no escopo original. Reabrir somente por fonte alterada, pin divergente ou regressão concreta. Não usar o número de testes como prova do alvo.

## Diretórios, identidades e autoridades

| Objeto | Fato / autoridade | O que não demonstra |
|---|---|---|
| C:\ProgramData\PapinhoAccelerator | Locator elevado fornecido pelo owner: Protected/N0 e seis arquivos, first NONE | Não é publicação administrativa C8 por existir. Não houve conteúdo, File ID original, RAW1 ou lease adquirido. |
| Protected\N0 | Namespace dos inputs protegidos das tentativas 75330..., 0b1de...,10584... | Não é CURRENT/epoch C8 ou fonte de credenciais de produção. Sem nova leitura protegida nesta auditoria. |
| SYSTEM/Administrators | Descriptor N0 previsto por n0-protected-root-descriptor.h: owner/group BA, ACEs SYSTEM/BA | Não é conta operacional padrão do serviço, custodiante atual autenticado, Principal ou autoridade de publicação. Locator não leu owner ACL desses nomes: null não é ausência de owner. |
| Root administrativa C8 | PAPACC_CONFIGURATION_COMMIT embutida no launcher owner; commit/publish e pins factuais | Código e stores de teste não provam instância operacional publicada. Inicializador exige alvo ausente, não é leitor/adotador. |
| Roots build/* | Testes canonical_root_transaction e issuer/session; commits locais offline | Nomes live/root/publication não promovem fixtures. Não foram abertos para procurar segredos. |
| Root PST/Session Browser | Lifetime de runtime/leases/owner nos receipts Browser | Não equivale à root administrativa do Accelerator. |
| CURRENT/epoch e SiteTLS | Commit canônico e catálogo administrativo BODY, DECISION-mode-c-site-tls-profile-authority | Diretório, config LAB, grant isolado, default ref zero ou ordinal não os fabrica. SiteTLS não herda trust mTLS nem Egress. |

## Cadeia causal ED11 → root C8

Ver [análise de dependências](../integration/phase6-original-scope-recovery-20261028/DEPENDENCY-CHAIN.md). Cada aresta possui escopo. A cadeia não é uma ordem universal de instalação. Em particular, exigir root publicada para persistir a autorização Q4 e depois exigir Q4/TPM para criar a primeira root seria circular se usado como procedimento de NEW_INSTALL. Não foi demonstrado contrato accepted que imponha essa circularidade. O plano deixa a recuperação histórica separada e impede reutilizar suas reservas, sem conceder um caminho positivo novo.

## Caminho crítico mínimo e trabalho paralelo

1. **Conservar baseline**: pins e qualificações já válidas, snapshots/WIP, distinção Selected/Legacy, NSS/x86 versus OpenSSL/x64. Executar o verificador documental do pacote; não repetir campanhas inalteradas.
2. **Trilha alvo C2/C3**: fechar D4/D5 com política quantitativa justificada de idade/validade/duração/headroom e bindings reais de backend/host/destino/root/CURRENT/epoch/SiteTLS. Reusar snapshots passivos/receipts já integrados. D2 accepted permite preparação sem envelope opaco completo; não libera execução. Primeira entrega técnica: mapa caller→campo→origem→invalidação, distinguindo parâmetro normativo e observação, com teste offline apenas onde há divergência demonstrada.
3. **Trilha provisão/administrativa**: reconciliar implementações atuais manual/automatic/shared core, M3 NORMAL versus RECOVERY e M7 revogação ativa. Auditar a entrada autorizada do ato de instalação/commit e os consumidores; não pressupor nova root. Implementar depois apenas correções cobertas por contratos existentes; lacuna normativa concreta fica decisão focal, não nova família de ADRs. Emitir manifesto C8 somente após publicação real separadamente autorizada.
4. **Trilha Browser**: preservar composição API3/S6; concluir lacunas de certificados/GUI/rotas D/Legacy e preparar matriz de execução NT4 real. Leitura e checklist agora, mudanças no Browser em sua autorização/raiz de escrita aplicável. CSS/renderização geral não entra.
5. **Trilha histórica**: conservar Q4 efeitos/UNKNOWN e preparar somente o que já está autorizado sem TPM/NV/readback. Para qualquer futura admissão, provar compatibilidade e ausência de reutilização de reservas. W/L/TC continuam obrigatórias se for executada a rota cross-boot escolhida. Esta auditoria não substitui nem dispensa E21.
6. **Integração e observações futuras**: após políticas/inputs/autoridades, submeter o pacote exato mínimo para serviço/readiness/acesso/Stop E17/E18 e o pacote correlacionado Mode C para E19/E20. Agrupar efeitos só quando contratos e autorizações permitirem; não reutilizar tentativas. Podem ser operações distintas; não prometer um único Start ou um único teste para toda Phase6. TLS, buffers/prefixos/EOF/FINAL, owner teardown e quiescência são evidências distintas.
7. **Fechamento**: E22 somente depois de gates aplicáveis reconciliados e escopos limitados justificados. Demonstrar jornadas de provisão MANUAL/AUTOMATIC, revogação, restart normal, GUI/certificados, perfis Legacy autorizados e NT4. Phase6 integral não fecha só com ED11. PST0.6.5/release requerem fechamento integral e autorização própria.

Trilhas 2–4 podem avançar sem TPM; trilha 5 não dá crédito às demais. Nenhuma ordenação permite efeitos sem seus gates frescos.

## Decisões realmente reservadas ao proprietário

- Política quantitativa Mode C D4/D5 e aceitação residual exata se for usado diagnóstico empírico; D0–D9 inteiro não está accepted. Sem valores fundamentados, preparar alternativas, não pedir aceitação vazia.
- Legacy produtivo D9B, se continuar pendente após confrontar suas fontes correntes: não converter laboratório em produção nem excluir C9–C11. Mudança de escopo exige decisão expressa.
- Eventual cláusula concreta ainda ausente de autorização/consumo NEW_INSTALL ou reload/CURRENT: primeiro demonstrar com callers atuais; não pedir novamente M0–M11, SiteTLS, TC/W/L/custodiante/fonte já aceitos.
- Operações exatas futuras de serviço/rede/provisão/publicação e disposição Q4 são autoridades distintas da norma. Ações TPM/NV somente se a rota histórica for retomada com suas condições e autoridade próprias.

Não é indispensável aceitar nova norma só para executar o primeiro pacote offline. Não se propõe novo bootstrap, ledger, confiança distribuída, MDM, CSS ou recuperação ampliada. M3 não é waiver de Q4 e W/L não é requisito TPM global do MVP.

## Validação e limites da auditoria

Fonte factual: documentos normativos designados, closeouts anteriores/posteriores, call sites canônicos de root e Browser C2, matriz LAB e transcript locator já reconciliado. O pacote gera hashes das referências e verifica integridade estrutural da matriz; não requalifica binários, autenticidade dos relatos ou conformidade integral de cada linha de código. Não há prova nova operacional nem inventário universal do disco. Ausência de evidência permanece UNKNOWN.

Nenhum ADR accepted, código produto, binário, quota, pin operacional, objeto protegido ou arquivo Browser foi alterado. Nenhum TPM/NV/Q4/Start/Stop/SCM/ACL/cleanup/refund/CURRENT/assinatura/recuperação foi executado.

## Artefatos para continuar

### Status CURRENT e port M7, 2026-10-29

O [novo pacote funcional](../integration/phase6-normal-status-fanout-20261029/CONSOLIDATED-CLOSEOUT.md) implementa lookup da credencial completa no snapshot CURRENT, negação antes de publicação/admissão e o port nativo de revogação para Sessions/operações/candidates exatos. O efeito do cohort foi qualificado isoladamente; a rota nativa positiva ainda depende do status REVOKED original. Quatro builds, 40 CTests e 32 execuções focadas: PASS nos escopos declarados. NORMAL completo continua DENY: falta binding original CA/key/cert/generation e o ack PUBLISHED A/P não cabe na sequência atual sem resolver a representação. Não foi atribuído published_epoch a partir de P/revisão/valor futuro. Ver SOURCE-BINDING-FRONTIER.md e PUBLISHED-ACK-FRONTIER.md; não refazer CAS/M7 nem retomar TPM. ED11/Phase 6 PARTIAL, sem operação produtiva.

### CAS/publicação nativa e identidade M7, 2026-10-29

A autorização específica posterior permitiu integrar o código antes rejeitado. O [port nativo CAS/poll](../integration/phase6-native-cas-publication-20261029/CONSOLIDATED-CLOSEOUT.md) agora usa prepare_live/begin/poll/publish_live e confirmação exata sob o mesmo owner. Quatro builds: PASS; nove CTests por configuração e 68 fixtures nativos finais: PASS, sem publicação produtiva. O SHA256 exato da credencial é preservado após AuthN e no pin C2, sem confundi-lo com Principal. Fanout M7 completo ainda ausente. **NORMAL completo não demonstrado**: faltam fontes CA/history/writer live e a confirmação durável PUBLISHED A/P; provas continuam ausentes e init/assinatura negam. Essa qualificação é de metadados e publicação do fixture, não emissão real. Ver NEXT-NATIVE-NORMAL-CONTINUATION.md desse pacote; não repetir CAS nem retomar TPM por default. ED11/Phase 6 permanecem PARTIAL.

### Continuação issuer, 2026-10-29

O [core pendente qualificado](../integration/phase6-issuer-continuation-20261029/CONSOLIDATED-CLOSEOUT.md) conserva CAS/CSR/DER/lease entre turnos do mesmo owner, sem repetir assinatura. Selected/Legacy Debug/Release: PASS, zero warnings, sete CTests por configuração e 207 verificações do modelo issuer por configuração. Core x64: 1056 bytes; root port: 56 bytes (embutido, não somar novamente ao core). Não há concessão de capacidade. A revisão automática rejeitou a ligação nativa CAS/live/publicação por potencial CURRENT fora da autorização; o arquivo nativo permaneceu idêntico à preimagem. Produtor NORMAL, fanout nativo, E8 e representação L2 continuam pendentes. Nenhum status global ED11/Phase 6 foi promovido. Ver QUALIFICATION.json e NEXT-CONTINUATION.md no pacote.

### Integração funcional posterior, 2026-10-28

[Código/qualificação atuais](../integration/phase6-functional-integration-20261028/CONSOLIDATED-CLOSEOUT.md) implementam continuidade NORMAL separada no core e consumidor CNG, gate de fanout pós-commit M7 e composição trace/claim de CONFIRM. O achado anterior de ROOT_PROOFS31 incondicional foi corrigido nesses consumidores; o port nativo de root ainda não produz as provas e a publicação completa, portanto não há habilitação operacional. Quatro builds, sete suítes por configuração e vinte fixtures de armazenamento passaram nos escopos declarados. E8 positivo e representação persistente ENROLLMENT continuam gates expressos; não há fonte permissiva ou alteração de formato accepted. Ver o próximo prompt funcional desse pacote em vez de repetir o inventário offline. Nenhum gate global foi fechado por esses testes.

O [pacote técnico foi executado](../integration/phase6-product-closure-offline-20261028/CONSOLIDATED-CLOSEOUT.md): 36 fronteiras atuais mapeadas e 30 ingredientes D4/D5 reconciliados. M0–M8, F1–F5/G0–G7 e P0–P9/T0–T7 já são accepted, não decisões a repetir. O bloqueio atual da provisão é fonte/owner/integração positiva: E8 SOURCE_ABSENT, listener ainda só perfil offline e issuer root port sem epoch/proofs/publicação completa. M3 NORMAL ainda necessita implementação por propósito, preservando RECOVERY. Veja JOURNEYS.csv e CALLER-MAP.csv desse pacote antes de retomar o prompt abaixo; ele não deve ser reaplicado como auditoria genérica.

- [Matriz única CSV](../integration/phase6-original-scope-recovery-20261028/REQUIREMENTS.csv).
- [Pacote técnico offline e verificador](../integration/phase6-original-scope-recovery-20261028/README.md).
- [Próximo prompt executável](../integration/phase6-original-scope-recovery-20261028/NEXT-TECHNICAL-EXECUTION.md).
