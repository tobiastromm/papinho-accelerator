# Receptor original e G3 — resultado focal

Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado.
Continuação de NEXT-RECEIVER-ORIGINAL-ADMISSION.md. Nenhuma norma foi aceita
por inferência. G0–G9 continua accepted; nenhuma atualização staged executada.

## Fonte receptora: primeira lacuna exata

A fonte administrativa original existe no controlador frio, após aquisição
nativa e três atos atribuídos. O runtime não possui esse empréstimo antes da
conexão. Sua configuração SCM, imagem, token e carrier são objetos/metadados
de identidade, não uma decisão administrativa independente. A ausência é
concreta, não uma consulta SCM faltante ou um parâmetro a preencher com PID futuro.

No código, carrier_capture só armazena DATA; registered_owner_decision/
registered_endpoint_decision não encontram fonte receptora confirmada;
ingress_continue rejeita antes de scqb_open. A aquisição SQB1 e as verificações
originais só ocorrem depois dessa abertura. O getter não pode ser preenchido
usando a própria mensagem cuja obtenção depende dele.

Confronto integral: OHI exige proveniência e decisão/ato atribuídos; B4/B5 admite
empréstimo autenticado, não novo approval nem restauração por arquivo; S3 permite
somente metadados/referências permitidos e S4 conserva autorização de endpoint.
G2 diz expressamente que a garantia G1 não resolve/dispensa producers. Nenhuma
dessas cláusulas designa positivamente a permissão inicial QUERY sem empréstimo.
Não há GOVERNANCE CONFLICT entre elas: falta a designação da fronteira inicial.
Não se afirma impossibilidade de qualquer implementação; a pré-condição atual
não pode ser removida por simples interpretação de nomes/bytes do peer.

Foi preparada uma única [decisão focal concreta](INITIAL-QUERY-AND-G3.CANDIDATE.md):
conservar DENY ou permitir aquisição QUERY mínima sob política local explícita,
independente de admissão administrativa, no mesmo owner/canal. Ela distingue
permissão QUERY de confiança no empréstimo posterior. PROPOSED, sem caminho
positivo implementado, sem nova família de ADRs, sem pedir reaprovação G0.
SHA256: `5b77ddae089ba496c511a562575e22642a29555e536fd4563e5e5ebb8faefeca`.

## Correção técnica autorizada G3

Defeito encontrado: scqc_plan usava o hash fixo de G0 como plan.policy;
quantitativos diferentes recebiam a mesma identidade de política. Agora esse
campo recebe SHA256 de preimage local canônico: domínio G3P1, norma G0 exata,
quatro u64 (crescimento privado/working set, pisos físico/commit) e quatro u32
(handles, freshness, duração e janela original). 84 bytes, little endian, sem
padding/ponteiro. Não é registro persistido, novo wire ou fonte de autoridade.

Caller efetivo inalterado: main → launcher frio → owner existente → scqc_plan
→ papacc_store_hash → plan.policy → seleção/source/SCQ1 existentes. Os limites
continham sua view congelada; agora também distinguem a identidade da política
transportada. A norma é um componente do digest, não sua substituição por flags.
SCQ1 permanece308 bytes, record1048, nenhum formato persistido foi reinterpretado.
Request/decisão/READ, guarda TO e caminhos SAM/N0 não foram afrouxados.

O novo helper está em personal_scq_start_plan_data_win32.inc; alteração de
producer em personal_scq_cold_acquisition_win32.inc. O teste existente de G0
verifica os oito campos individualmente, high bits u64, endian e inválidos.
Não foi criada API permissiva, grant, owner, root, ledger ou novo canal.

## G3 para decisão, não campo vazio

A proposta acima traz um conjunto concreto: handles128/guard124 obrigatórios;
crescimento privado/WS1994944 por participante (orçamento escolhido de um KNOWN
Selected, não teto físico); pisos7239172096/9823326208 das sete amostras Windows
históricas do registro concluído; freshness100ms; janela240000ms; duração60000ms
após READY controlador. Os quatro últimos valores de relógio/margem/crescimento
continuam escolhas administrativas propostas; não foram selecionados por G0.
O documento explicita consequências e proveniência de cada campo.

As sete queries históricas duraram3.0871–3.8784ms, não ceilings. Pico165 de
handles do registro não é quota SCQ e não justifica elevar128. Pisos históricos
não são margem fresca; devem ser verificados antes de efeitos e podem negar a
sessão. Não se importa o piso R0=3145728 nem a seleção B3. Nenhuma nova coleta.

KNOWN=1994944 e PROCESS_LIMIT=3145728 Selected continuam storage/quota controlados,
não previsão do footprint nativo. Loader/RPC/kernel/provider/stacks e retenção
opaca UNKNOWN sob G1; não exigir máximos integrais que G1 especializou. Retenção
consome seus recursos até fechamento factual, sem refund por timeout/Stop.

Estado G3: proposta concreta pronta para seleção; **não operacionalmente fechado**.
Além da escolha explícita, faltam binding do deadline global e gates quantitativos
da entrada receptora: não conceder ao receptor um novo prazo ao receber metadata.
Esses callers dependem da decisão de ordem QUERY, e não de novo processo medido
antecipadamente. Não foi emitido comando para um plano incompleto.

## Qualificação e imagem

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings. Dez CTests
afetados por configuração: **40 pares teste/configuração PASS**. Não somar os
108 anteriores ou reexecutar a campanha histórica inteira. G0 mantém33 cenários,
agora oito mutações de campos e520 checks. Source orchestration e máquinas reais;
APIs protegidas/peers/RNG/SCM/endpoint/CNG dos positivos são modelados. Não se
demonstrou autenticação bilateral nativa nem capacidade operacional.

Selected/Release workspace:701952 bytes,
`216458697180f6d60766ac14d39745bfec7d4db65d092837c65445233984f870`.
Delta+512 contra workspace anterior701440; +130560 contra staged histórico571392.
DLLs sem delta contra pins históricos. Snapshot anterior preservado em
build/private-bootstrap-preserved-20261031. Não houve leitura/revalidação de
objetos protegidos staged ou SCM; sua última qualificação continua histórica.
Imagem ainda sujeita à integração receptora; não selecionar ou atualizar agora.

## Primeiro pedido e caminho mínimo

Pedido operacional: NOT_READY. Primeiro ato autorizável é a decisão técnica
focal B/QUERY e seleção G3 propostas, somente para implementação offline;
nenhum pedido de endpoint/Start pode ser emitido como se já estivesse pronto.
Depois: integrar ambas as validações/gates do receptor, qualificar, estabilizar
imagem, preparar uma atualização governada e autorizações separadas de endpoint,
Start e eventual Stop. Não repetir staging/registro ou autoridades consumidas.

Nenhum update, endpoint real, Start/Stop, SCM/ACL, CA/CURRENT, rede operacional,
TPM/Q4, cleanup/refund ou recuperação executado. UNKNOWN/reservas/históricos/pins,
PST0.6.4, chave fixture e quiescência OS UNKNOWN preservados. E21 PENDING;
ED11/Phase6 PARTIAL; cadeia fria NOT_COMPLETE; autoridade DENY.

[Próximo prompt](NEXT-OWNER-QUERY-AND-G3-DECISION.md), [evidências](EVIDENCE.json).
