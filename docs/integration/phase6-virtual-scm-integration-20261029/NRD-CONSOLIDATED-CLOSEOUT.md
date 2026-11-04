# SCQ — ligação nativa de observação e fronteira dos originais

Task: PACC-PHASE6-BQI-NATIVE-EVIDENCE-READINESS. 2026-10-30.
Continuação de NEXT-BQI-NATIVE-EVIDENCE-READINESS.md, sem nova decisão normativa.

## Resultado implementado

O owner canônico agora expõe `papacc_configuration_personal_scq_observe_runtime_data`,
que chama `papacc_personal_runtime_observe_data` sobre o estado de identidade
já capturado e retido no packet existente. Não abre processo por PID/nome,
não cria owner nem reconstrói referências a partir do DTO.

O adapter revalida processo/token/SCM antes e depois da projeção. Usa PID/criação,
processo não sinalizado, TokenUser, TokenId/AuthenticationId/ModifiedId, enumeração
completa de grupos, atributos, restrições, elevação, conta SCM, service SID type,
PID do serviço e estado RUNNING. Consulta TokenSessionId e verifica sessão zero
e grupo de logon de serviço único, ENABLED e não DENY_ONLY. Esse último campo
representa a classe de logon comprovada pelo grupo do token; não se afirma uma
consulta LSA ou atestação criptográfica. O caminho SAM console permanece intacto.

A saída possui máscara explícita `known=15` para processo/token/SCM/sessão-grupo.
Imagem original permanece UNKNOWN: bit IMAGE ausente, identidade e digest zero.
`COMPLETE=31` não é atingido. O teste injeta essa projeção no perfil existente e
comprova rejeição, sem completar os campos com dados históricos ou fixtures.
A função não cria aprovação, não consome autorização e não entrega handles.
O resultado é uma observação local momentânea, não prova transferível de origem;
a referência original continua no owner e deve ser revalidada no ponto de uso.

A saída é apagada nas falhas, sem publicar uma observação parcial como sucesso.
Sobreposição com o estado retido ou com Stop é rejeitada antes de escrever.
Stop no início ou durante a observação e mudança de token no segundo readback
negam. Falha de fechamento continua usando a retenção UNKNOWN do adapter existente.
A entrada do root também rejeita acesso fora do writer, Stop/worker pendente,
CA emprestada e saída sobreposta ao root. Não houve relaxamento do source OHI.

## Fontes concretas e lacunas

| Elemento | Fonte/caller existente | Alcance e lacuna |
|---|---|---|
| Runtime original | runtime_capture → referências process/token/SCM no packet → current → observe_runtime_data | Helpers ligados e qualificados com APIs de processo/token/SCM modeladas. Serviço pessoal original não foi observado nesta tarefa. |
| Imagem do runtime | terceira referência de scq_inputs; perfil exige identidade nativa e digest próprios | Falta aquisição/proveniência original e correlação ao processo SCM. Nome de imagem ou PID não satisfazem o requisito. |
| Administrador | personal_token e scq_capture_inputs | Token elegível não é aprovação. A integração não cria nem recebe uma decisão operacional original por esses predicados. |
| Pedido/decisão | três referências readonly mantidas por scq_inputs e verificadas por inputs_current | Custódia estrutural existe; conteúdo tipado, confirmação administrativa atribuída e proveniência original ainda não alimentam o source. Nenhum arquivo bruto foi aberto/lido nesta tarefa. |
| Alvo | install_paths NEW/READY, known folders congeladas/revalidadas | DATA, sem adquirir/criar parent ou instalar root. |
| Consumo | profile_take_data e personal_scq_receive_consume | Interlocked/borrow modelados, não comprovação de consumo durável do controlador. A validação de restart do modelo não prova persistência nativa. |
| Entrega | perfil privado SCQ codec/state + receptor OHI existente | Nenhum backend de endpoint criado/conectado/testado; autenticação bilateral nativa não demonstrada. |

A primeira lacuna de autoridade está no caller concreto
`papacc_configuration_personal_scq_receive_original`: continua chamando
`personal_scq_receive_original(..., NULL, ...)`. O native_source_status retorna
SOURCE_UNAVAILABLE/ORIGINAL_PROVENANCE_UNAVAILABLE. Somente um producer do
controlador que adquira os originais protegidos, confirme o plano SCQ atribuído
e conserve a continuidade/consumo pode substituir essa ausência. Não pode ser
um setter de approved, digest, receipt ou confirmação NEW_INSTALL.

Não existe pedido SCQ operacional original aprovado para esta nova sessão.
A aceitação B/OHI não o fornece. Além disso, não há destinatário SCM original,
imagem adquirida ou continuidade/consumo durável qualificados. Portanto não
foi gerado pedido operacional executável nem comando com valores fictícios.
Não se propõe root C8, TPM ou recuperação histórica como dependência desta ligação.

## Qualificação e recursos

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings.
Allowlist focal: oito CTests/configuração, 32 execuções PASS. A campanha BQI
inalterada permanece histórica, sem nova execução dos demais 19 testes/configuração.
Identidade: 163 verificações/configuração; perfil privado existente: 3.338.
Regressões: root nativo/modelo, runtime, probe, dispatch e separação N0.

Cobertura nova: snapshot válido de referências modeladas, source incompleto
rejeitado, sessão não zero, grupo service ausente/desabilitado/deny-only,
TokenUser/token/peer divergentes, consulta falha, processo terminado,
PID/criação divergentes, perda de continuidade tardia, Stop durante readback,
SAM não promovido ao perfil privado, foreign thread real, buffer sobreposto,
saída apagada, root bloqueado e preservação de DENY/KSP zero.

SID helpers e foreign thread usam Windows real. Processo/token/SCM/serviço e
originais positivos são modelados; nenhuma identidade SCM nativa operacional
foi autenticada. Não se declara autenticação bilateral, persistência, rollback
nativo ou negação KSP real demonstrados.

Estado runtime permanece 216 bytes; observação é scratch de 224 bytes do caller.
Zero crescimento persistente. Known managed compilado permanece Selected
1.976.080/3.145.728 e Legacy 1.807.512/2.097.152. As consultas adicionais têm
scratch/stack e tokens temporários sob o fechamento existente, sem novo crédito.
Não houve medição de pico operacional. Provider/kernel UNKNOWN, caps
124/128/1024 preservados e admissão nativa DENY.

[Qualificação e hashes](NRD-QUALIFICATION.json), [fontes](NRD-SOURCE-HASHES.json),
logs NRD-*-build/ctest/LastTest/layout. Propostas/decisões accepted e N0
inalterados; HEAD/WIP/históricos e receipt da chave persistente preservados.
A chave não foi aberta, usada ou excluída.

## Próxima ação concreta e fronteira operacional

Antes de solicitar execução: ligar o producer do controlador à aquisição e
confirmação dos objetos originais, demonstrar consumo/continuidade governados
e vincular a imagem original à observação. Isso é implementação offline coberta
por B/OHI; não exige reaprová-los. Sem esses mecanismos não basta pedir ao
proprietário mais um PID, hash ou elevação administrativa.

A primeira prova nativa futura deverá envolver os dois principals reais.
Criação/conexão de endpoint requer autorização independente; instalação SCM,
Start/Stop, chave MACHINE e parent, quando indispensáveis, exigem efeitos e gates
próprios no pedido exato. Não foi solicitado nem executado esse pacote incompleto.
Não há comando manual nesta entrega e não há autorização consumida ou criada.

NEW_INSTALL segue dependente da preparação legítima do parent, identidade SCM e
isolamento KSP MACHINE, além da cerimônia própria. Esta observação não é
instalação nem CA_INITIALIZE. E17/E21 sem promoção; E18/E19/E20 independentes;
E22 não concluído; ED11/Phase6 PARTIAL; autoridades DENY; PST 0.6.4 preservada.
