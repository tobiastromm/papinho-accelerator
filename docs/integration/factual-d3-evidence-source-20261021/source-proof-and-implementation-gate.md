# Prova de fontes e limite de implementação

A tarefa é parcial: não há conjunto factual completo de fontes existentes que possa ser conectado positivamente a E0–E8. Nenhum hunk funcional foi aplicado; repetir o shim SOURCE_ABSENT ou acrescentar booleans/structs de caller não produziria essa fonte. O binding negativo implementado permanece byte-exato e o SDK/Browser já o consomem. Sem fonte factual, DENY conforme o pedido.

## Funções existentes e o que provam

- server_configuration_root_transaction_win32.inc: root_snapshot_held valida ticket, revisão/domain/predecessor e epochs do slot; root_snapshot_live exige CURRENT separado. papacc_server_launcher_configuration_current_pin só trabalha na única root ativa com publicação normativa factual. Não serve para criar um CURRENT candidato.
- issuer_root_port_read, no mesmo arquivo, é uma porta canônica EXISTENTE. Porém lê no perfil dormente e deixa explicitamente ausentes proofs de key/freshness/prefix-history/CURRENT; retornar OK da leitura significa dados, não issuer qualificado. O core issuer verifica o conjunto de proofs antes de operar. Não preencher bits31 para fazê-lo passar.
- server_configuration_commit_win32.c pin/unpin e server_configuration_issuer06_win32.inc issuer_snapshot06/pin_read protegem tickets/serial/readers e cópia imutável do issuer. Pin preserva memória histórica; não comprova status corrente. Essas funções não produzem observação de delegação PDG1 nem leases dos backings externos.
- config_root8_v3.c/.h valida/seleciona PDG1 como DATA owned, com R/A/P/I históricos. manual_package_pdg.c correlaciona pacote/SPKI e rejeita propósito/status inválidos, mas retorna SOURCE_ABSENT mesmo no match completo. Isso é comportamento obrigatório, não implementação incompleta a corrigir com STEP_OK.
- server_config_root8_data_win32.c aceita somente kinds6/7 na composição issuer; kind8 requerido para signer certificado/perfil retorna UNSUPPORTED. A fixture v3 usa callback de modelo e certificado artificial, sem provar perfil/status/chain do signer real.
- O worker/snapshots existentes não expõem uma composição v3 administrativamente publicada e pinada para PDG1. A capacidade selecionada do worker é PC_ROOT8_V2_MAX28960; o máximo v3 é30592. Não foi ampliada nem declarada incapacidade universal: só não há prova de suporte integral v3/retidos. O callback DATA/floor histórico e o predecessor verificado não substituem CURRENT/publicação.

source-evidence.json fixa linhas, excertos e SHA dos arquivos reais. source-inventory.csv separa fatos disponíveis de sua autoridade. Nenhuma inferência de ausência foi feita apenas pela busca textual: foram lidas as implementações root/current/pin/issuer/component/codec e seus contratos. A instalação CURRENT qualificada já foi declarada indisponível pelo owner em tarefa anterior; não foi solicitado novamente nem criado store produtivo para suprir essa lacuna.

## Cláusulas que impedem o patch dependente

F2 accepted exige "snapshot imutável aplicável da policy, registros/histórico issuer e certificado correspondente, vínculo de ExpectedServerName/listener/TLS já accepted e CURRENT factual" e estabelece que consulta somente DATA não satisfaz aquisição. E8 accepted exige demonstrar funções reais dos owners, captura, lifetime/revalidação/release antes dos patches dependentes. E1 proíbe shim que aceite dados do caller sem funções factuais revisadas. E6 exige recursos já adquiridos/protegidos antes de acquire; chamar current_pin, alocar ou iniciar aquisição dentro de sp_trust_acquire contrariaria o perfil borrow-only.

Portanto não há conflito normativo novo a resolver: há dependências factuais/implementação não atendidas. Não alterar accepted, criar API/registro/autoridade paralela ou preencher a ausência com modelo. Esta autorização não dispensa F2/E8. A ausência de confirmação Browser (fora do escopo) também mantém M1 TRUST negado, mesmo que uma futura observação administrativa parcial se torne disponível. Não implementar confirmação no servidor.

## Provas independentes preservadas

14 suítes codec/issuer/root/ED11 por MSVC Debug/Release e a suíte compartilhada negativa passam; 8 testes nativos locais de história/snapshots/pending UNKNOWN/Stop passam sob autoridade de componente sintética. Estes provam código, filesystem e lifecycle do componente; não fonte factual positiva, CURRENT operacional, publicação, perfil real de signer ou E0 lease conhecido. Nenhuma fonte presente positiva foi fabricada.
