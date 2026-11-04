# Alternativa B SCQ — integração e qualificação offline

Task: PACC-PHASE6-ONA-B-ACCEPTANCE-AND-OFFLINE-INTEGRATION. Data: 2026-10-30.

## Decisão e limites

A alternativa B e suas regras indivisíveis 1–8 foram registradas em
[decisão accepted](../../adr/DECISION-personal-scq-cross-principal-offline-20261030.md)
antes da implementação dependente. A proposta original permanece inalterada:
SHA256 `e31b4e5feb78a96baca47494df8d26ffb019d49518c72efd84b08ffc97b559d1`.
N0, T0–T14 e seu predicado same_token permanecem intactos. OHI continua fonte
obrigatória da autoridade original; confirmação NEW_INSTALL não autoriza SCQ.

Nenhum endpoint foi criado, conectado ou testado, inclusive em fixture.
Nenhuma sessão SCM/CNG, Start/Stop, instalação, parent, conta, CA operacional,
CURRENT, SCM/ACL, rede operacional, TPM/NV/Q4, limpeza/refund ou recuperação foi
executada. HEAD preservado; WIP e evidências anteriores mantidos.

## Código integrado

`personal_scq_profile_win32.h/.inc` implementa o perfil privado SCQ DATA no
packet/owner existente: projeção fechada, codec little-endian de tamanho exato,
validação em ambas as pontas com observações e inventários independentes,
exclusividade, sequência take/offer/receive/receipt/ack/close, Stop e UNKNOWN.
O dispatcher rejeita N0. Não existe novo serviço, root ou owner.

A projeção vincula pedido, decisão, política, tentativa, alvo, chave MACHINE
fixture identificada, propósito, serviço, imagem e pares. Exige PID/criação,
identidades de token/autenticação/modificação, SID primário, enumeração completa
de grupos, atributos, restrições, sessão, logon, estado de processo e imagem.
Administrador e runtime virtual são distintos. Nome ou SID resolvido isolados
não concedem acesso. Recebimento exige correspondência com as observações
independentes fornecidas pela própria ponta; modelos não autenticam sua origem.

Somente três tipos de referência são representáveis: pedido, decisão e imagem.
Direitos exatos de leitura `0x00120089`, identidade nativa, tamanho, atributos,
link único, digest e descritor resumido são verificados. Não há tokens
impersonáveis, direitos de ação administrativa, chaves privadas ou conteúdo bruto.
Ponteiros locais self/owner nunca entram no frame. Campos desconhecidos,
comprimento diferente, identificadores ambíguos e direitos excessivos são negados.
A projeção não classifica nem desclassifica conteúdo arbitrário: a prova de
conteúdo/proveniência e os originais retidos continuam obrigatórios no source.

Antes do primeiro Start modelado, o controlador consome a tentativa DATA.
O destinatário pode inicialmente conter apenas o SID virtual esperado; nenhuma
identidade futura de PID/token é inventada. Após observação modelada do runtime,
o binding exige SID, serviço, imagem e demais predicados, mantendo congelados
pedido/decisão/política/alvo/referências. Registro incompleto não é entregável.

O frame tem 1.048 bytes, receipt DATA 1.052 e contexto embutido 1.096.
O receipt é confirmação de modelo, não ack autenticado nativo. Interlocked e
thread affinity demonstram exclusividade local, não consumo durável após reboot.
Perda do peer, ausência de ack e fechamento incerto preservam UNKNOWN/retenção.

A entrada canônica de recepção original continua com source ausente e DENY;
preparar DATA não libera KSP. O teste no root confirma zero chamadas de provider.
Não foi implementado um backend de endpoint operacional por consequência desta
aceitação. A aquisição nativa bilateral e o handoff de originais continuam pendentes.

## Correções técnicas necessárias

O guard de paths exigia fase transitória 1 embora o resolver publique READY 2.
Foi corrigido para a constante READY, preservando revalidação e autoridade.
O caminho de identidade agora verifica também WaitForSingleObject sobre a
referência original, com QUERY_LIMITED_INFORMATION e SYNCHRONIZE, rejeitando
processo sinalizado mesmo com exit code STILL_ACTIVE e falha de consulta.
Nenhum direito administrativo é transferido por essa alteração local.

## Qualificação

Selected/Legacy, Debug/Release: quatro builds PASS, sem warnings. Allowlist
restrita de 27 CTests por configuração: 108 execuções PASS. Não foi executada
campanha genérica que pudesse abrir endpoints ou reabrir a chave persistente.

O teste privado executa 3.338 verificações/configuração: identidade, destinatário,
perfil/propósito, serviço/imagem, token/grupos/restrições, referência/direitos,
comprimentos, substituição de cada byte, replay/forwarding, consumo duplicado,
Stop, clock inválido, concorrência, entrega e fechamento incerto. SID helpers e
thread estrangeira usam Windows real; peers, inventários, relógio e consumo são
modelados. Identidade runtime: 97 verificações/configuração. Não se declara
autenticação bilateral nativa com base nesses resultados.

Known managed compilado: Selected 1.976.080/3.145.728 bytes; Legacy
1.807.512/2.097.152. Crescimento fixo: 1.096 bytes. Frames/receipts do caller,
stack, snapshots dinâmicos de descritores e provider/kernel exigem contabilização
própria; não constituem crédito ou capacidade concedida. Limites 124/128/1024
inalterados; custos ativos provider/kernel UNKNOWN, admissão nativa DENY.
A janela de 10 segundos do fixture não é política operacional de freshness.

Evidência: [qualificação](BQI-QUALIFICATION.json),
[fontes e hashes](BQI-SOURCE-HASHES.json), logs BQI-*-build/ctest/LastTest/layout.
Resultados antigos ONA/OHI permanecem históricos; não foram recalculados como
se qualificassem os novos fontes.

## Primeiro gate factual e caminho até NEW_INSTALL

A qualificação offline do perfil está concluída no alcance descrito. Ainda não
há prova de aquisição bilateral nativa dos originais, destinatário SCM original,
consumo durável governado ou entrega real de referências. Serviço pessoal não
instalado, parent não adquirido e isolamento KSP MACHINE não demonstrado.
Logo, não existe pedido operacional byte-exato completo nem comando manual.

O próximo pacote deve mapear e qualificar os adapters nativos de origem e
continuidade, sem endpoint; identificar os bindings que só poderão ser colhidos
na sessão aprovada; e preparar pedido somente quando executável. A menor prova
bilateral futura exige administrador original e runtime virtual original.
Um endpoint entre processos same-user não provaria essa fronteira.
Criação/conexão de endpoint exige autorização independente; Start/Stop e sessão
SCM/CNG exigem escopo próprio explícito; criação de parent, se necessária, também.
Não há autorização implícita nem nova aceitação de B/OHI a solicitar.

E21 PENDING; E17/E18 e E19/E20 mantêm suas dependências independentes. E22 não
concluído. ED11/Phase 6 PARTIAL, autoridade produtiva DENY, PST 0.6.4 preservada.

Continuação: [próximo prompt](NEXT-BQI-NATIVE-EVIDENCE-READINESS.md).
