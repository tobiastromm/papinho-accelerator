# Plano exato SCQ — recomendação e decisões exclusivas

Executado NEXT-ONB-EXACT-SESSION-PLAN.md no branch main, HEAD
f341985924d29870d8ff627ed37c8a0874ad6469. Este é um resumo para decisão,
não ADR, aceite normativo, pedido operacional ou grant. Nada foi executado.

## 1. Artefato concreto recomendado

Recomendo Selected/Release para o planejamento PERSONAL_LOCAL Windows:
`build\phase6-functional-20261028\Selected\Release\papacc_server.exe`
SHA256 `90123fbadc3d7e4db1e5f48ea2b7fe85f6e78f61f6adee083b00d1288b951874`, 520192 bytes.
O hash foi conferido novamente contra os bytes atuais, sem rebuild. Não é
imagem instalada/protegida ou mapped-image attestation. Seleção operacional
continua dependente do proprietário; a recomendação não concede3MiB por inferência.

Alternativa Legacy/Release: `build\phase6-functional-20261028\Legacy\Release\papacc_server.exe`
SHA256 `caab74e4f320efbd361257f130982b9a9e0d3f475f26c0f76de167f311f73886`, 452096 bytes.
Selected usa limite gerenciado3145728 e censo conhecido1992700; Legacy usa2097152
e censo1824132. Restos1153028/273020 são aritmética estática desses componentes,
não headroom físico/commit nem envelope provider/kernel. Selected oferece maior
margem nesse ledger. Legacy conserva a opção menor; não significa NT4 qualificado.

Decisão disponível agora: selecionar um desses artefatos/perfis para compor o
pedido futuro. Não autoriza implantação, SCM, endpoint, Start/Stop ou execução.

## 2. Dependência de key: escolha de escopo, não PID faltante

OHI accepted incorpora a vinculação à chave MACHINE fixture exata. A regra3
cross-principal também a preserva. No código, scqp_static exige scqp_key mesmo
quando o construtor é fechado; a janela também é validada em scqp_prepare.
SSO-B proíbe KSP como consequência de startup, mas não dispensou essa vinculação.
Não há conflito normativo: existe uma dependência adicional no contrato atual.

Duas rotas legítimas, ainda não escolhidas:

- Manter o contrato atual e designar um seletor exato com origem legítima no plano.
  O seletor é intenção administrativa; não comprova existência/acesso à chave.
  Não usar fixture_name nem o nome da chave CurrentUser retida como MACHINE.
  Não criar CA ou chave apenas para preencher o campo desta preparação.
- Delimitar o primeiro ensaio a transporte/originais/close/readiness, sem KSP e
  independente de key. Recomendo esse escopo para separar a prova bilateral da
  prova CNG. Porém, tornar key não aplicável exige especialização focal explícita
  de OHI/perfil, antes de código; não decorre automaticamente de SSO-B.

Decisão disponível agora: manter a vinculação e indicar sua fonte/seletor, ou
orientar a preparação daquela especialização estritamente de transporte. Não
se está pedindo reaprovação SSO-B. Nenhuma nova ADR foi criada, nenhum contrato
foi modificado e key continua obrigatório enquanto essa decisão não existir.

## 3. Quantitativos: não há números fundamentados a aprovar ainda

As fontes D4/D5 de20261026/28 deixam idade, validade, duração global, piso físico
e piso de commit null/incompletos. Seus bindings são Browser/C2/C3/provider/
SiteTLS/destino. Não são uma política SCQ accepted e não devem impor ao ensaio
SCQ requisitos de Browser ou uma CA anterior por associação de nomes.

OHI e a regra7 cross-principal exigem parâmetros aplicáveis justificados à
sessão SCQ. Caps124/128/1024 e o censo compilado não demonstram duração ou
headroom. Timeouts locais, relógio de progresso, exemplos em testes e pisos LAB
não fundamentam valores para este plano. Portanto não proponho números nem
peço ao proprietário que aprove números arbitrários.

Depois de fixado o suporte exato do primeiro ensaio (artefato, efeitos, topologia
e rota key), os ingredientes quantitativos precisam ser fundamentados naquele
escopo, mantendo opacos/transitórios/retidos UNKNOWN e DENY onde obrigatório.
Apenas escolher um artefato e uma rota key não torna o pedido executável.

## 4. Originais e continuação: limitação preservada

OHI exige decisão operacional específica sobre um pedido byte-exato, identidade
nativa elegível e confirmação própria do plano. Os seletores nativos seguem NULL.
Não existe neste prompt aprovação dessa operação. Não foi criado arquivo de
aprovação, original de decisão, intent, registro de consumo ou setter de grant.
Os JSONs deste pacote não são lidos pelo runner como autoridade.

A aquisição e a continuação nativas precisam usar os originais retidos no mesmo
owner, e não confiar em hashes/paths/flags. O runner atual nega fonte vazia e
retém provas vivas sem continuação lifecycle governada. Uma aprovação isolada
não o transforma em executor autônomo. PID/criação/token são gates frescos e
não precisam ser inventados nem conhecidos previamente no pedido.

## Resultado concreto

O plano deixou de depender de uma revisão genérica: há duas escolhas exclusivas
concretas para o proprietário — artefato e rota key — e uma falta de evidência
quantitativa SCQ identificada, sem números transferíveis encontrados nas fontes.
Este pacote encerra a preparação de decisão disponível agora. Não foi produzido
outro prompt de auditoria, comando, pedido executável ou nova família de ADRs.

Código não alterado; builds/testes novos0. Evidências ONB/SPC permanecem históricas
e não foram contadas novamente. Nenhuma operação protegida ou coleta administrativa
adicional. Branch/HEAD/WIP, históricos, pins, chave fixture e PST0.6.4 preservados.
OPERATIONAL_REQUEST=NOT_READY; E21=PENDING; ED11_PHASE6=PARTIAL; AUTHORITY=DENY.

Fontes/hashes: ESP-SOURCE-RECONCILIATION.json, ESP-PLAN-READINESS.json e
ESP-ARTIFACT-HASHES.json. Aceites existentes permanecem byte-exatos.
