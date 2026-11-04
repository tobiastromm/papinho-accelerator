# QUERY + G3 accepted — integração offline e corte do deadline

Atualização posterior2026-11-01: o proprietário aprovou o delta SQB2 exato,
registrado e qualificado [neste pacote](../phase6-sqb2-deadline-20261101/CLOSEOUT.md).
O bloqueio de formato abaixo é histórico. O gate corrente é a ligação condicional
pré-Start; valores QUERY/G3 não são reabertos. Preservar este resultado anterior.

Task PACC-PHASE6-QUERY-G3-ACCEPTANCE-AND-COLD-CHAIN. Fechamento2026-11-01.
Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado.
**Resultado: integração parcial; cadeia fria canônica geral NÃO concluída.**

## 1. Decisão registrada

Conferido literalmente o SHA256 da proposta imutável:
5b77ddae089ba496c511a562575e22642a29555e536fd4563e5e5ebb8faefeca.
[Decisão accepted](../../adr/DECISION-personal-scq-initial-query-g3-20261031.md)
registrada antes do código dependente, por aprovação explícita do proprietário.
QUERY candidata não autoritativa não é empréstimo administrativo nem readiness.
OHI/B4/B5, SSO-B, TO e G0–G9 continuam obrigatórios. Nenhuma aprovação operacional.

## 2. Callers efetivamente integrados

ServiceMain fechado recebe SCQ1 e metadata transitório g3-until. O metadata não
está codificado no SCQ1 e não é autoridade. argc1/3 anterior permanece compatível;
sem deadline G3 a nova entrada nativa nega. O comando SCM persistido não mudou.

No owner canônico: carrier_capture → ingress_continue → política G3 selecionada
independentemente → identidade própria fechada → scqb_open_query → scqb_read →
scqb_candidates → captura nativa dos arquivos/proteção/conteúdo → correlação
runtime/SCM e imagem controladora → scqb_bind_query → prova de deadline →
first_prepare/promote → continuação/receipt/close existentes. Não há novo canal,
owner, serviço, ledger, grant ou callback permissivo instalado.

open_query não exige uma aprovação administrativa local pré-preenchida. A
permissão para aquisição é a política QUERY aceita, com contexto próprio e
seletores limitados; c.self impede segunda conexão. Após abertura, o peer é
conferido por referências originais de processo/token, PID/criação/estatísticas,
TokenUser/grupos/restrições e continuidade. Os três IDs candidatos exigem
direitos nativos exatos, tipo/identidade/descritor/conteúdo e imagem protegida.
Nome/hash/pipe/token isolados não aprovam. O controlador nativo já existente
continua a exigir três atos originais e consumo antes dos efeitos.

bind_query congela uma única scope receptora local a partir das verificações;
controller/source ficam NULL nessa scope, nunca ponteiros remotos ou outra
aprovação. Estado QUERY1 → tentativa3 → congelado2 impede nova captura/replay.
Só a admissão pelo offer original autenticado pode marcar o empréstimo restrito.
O getter de endpoint receptor só empresta essa scope após fase2/consumo original
e revalidação nativa. Fonte administrativa continua distinta de metadata QUERY.

Não se alterou a guarda inicial_receiver_source_unavailable=1 para uma constante
permissiva. A cadeia canônica pré-Start continua DENY antes de namespace/Start;
a ligação positiva só deve ocorrer após o binding geral qualificado abaixo.

## 3. G3 selecionado, sem alteração dos valores

| Parâmetro | Seleção implementada |
|---|---:|
| Handles completos | máximo128; guard124 antes de aquisição |
| Crescimento privado por participante |1994944 bytes sobre READY próprio|
| Crescimento working set por participante |1994944 bytes sobre READY próprio|
| Piso físico do host |7239172096 bytes|
| Piso commit do host |9823326208 bytes|
| Freshness até efeito |100ms|
| Janela original |240000ms, sem renovação|
| Duração global |60000ms após READY controlador, limitada pela expiração original|

Native cold acquisition rejeita parâmetros diferentes dos selecionados. A
identidade de policy continua SHA256 do preimage G3 exato já integrado na tarefa
anterior; o receptor compara com sua própria política compilada, não concede
recursos a partir de parâmetros recebidos. G3 permanece tolerância de risco.

O controlador congela global_until uma vez, min(READY+60000,expiração original).
O receptor congela READY próprio, recebe somente prazo restante e rejeita nova
captura/rebaseline/renovação. Amostras nativas completas são exigidas antes das
fases; scqt_resource_effect verifica QPC/idade novamente imediatamente antes de
novas referências e I/O. Query duration curta com gap que exceda100ms nega.
GetProcessHandleCount completo continua anterior às aquisições, sem desconto
de console/herdados/aliases. Stop, clock inválido, insuficiência e falha fecham
admissão; referências incertas permanecem no mesmo owner.

## 4. Primeiro bloqueio exato — autenticidade do prazo global

SQB1 carrega record1048 e três referências candidatas. Sua expiração original
é autenticada no conjunto original, mas não contém o deadline global independente
de READY. O argumento transitório SCM sozinho não o prova. O código novo nega
antes de promoção quando não consegue vinculá-lo à expiração original.

Há um caso-limite válido de coexpiração: READY ocorre suficientemente tarde e
min(READY+60000,original) é igual ao original. Foi qualificado como ramo de
componente, NÃO como estratégia operacional de esperar/atrasar confirmações.
O caso normal com prazo global anterior à expiração original permanece negado.

Uma extensão SQB2 privada (+8bytes de deadline e buffer+8), mantendo SCQ1/SQB1/
SQR1/SQC1 e formatos públicos/persistidos, foi submetida à revisão automática.
A revisão rejeitou por entender que novo formato/payload ultrapassa a autorização.
Confronto adicional com B3 accepted e preservação de SCQ1 foi apresentado; a
segunda submissão continuou rejeitada. Nada de SQB2/buffer ampliado foi aplicado.

A remoção direta da guarda pré-Start também foi rejeitada por possível progressão
sem prova independente. Permaneceu intacta. Não se contornou nenhuma rejeição
por outra ferramenta, repurpose de campos ou efeitos externos. O bloqueio de
formato é a primeira dependência técnica para o caso geral; não é nova seleção G3.

[Pedido técnico concreto](PRIVATE-DEADLINE-DELTA.REQUEST.md), SHA256
`62548c758282dd6b828b451a01a92bc4a369843ac55f7cc9c52f8c84a25ae11c`,
delimita exatamente o delta e os testes. Está CANDIDATE, aprovação específica
solicitada e pendente; sem inferência. Não há pedido operacional pronto.

## 5. Builds, testes e alcance

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings nos finais.
28 CTests afetados/configuração: **112 pares teste/configuração PASS**.
Logs de compilação iniciais com erros de shadowing/macros de modelos foram
preservados e corrigidos; não somar reruns ou os resultados históricos.

Novo QUERY_COLD_CHAIN:20 cenários,1017 checks por configuração. Receptor sem
seleção prévia, fonte controladora com três atos, identidade/imagem/direitos,
segunda conexão, replay de freeze/consumo, Stop, freshness no helper real,
ausência de receipt/close, close incerto e deadline sem origem. Três caminhos
de coexpiração passam o predicado de readiness antes de fault; dois fecham
normalmente e um injeta falha de fechamento/retém. Zero RUNNING real nesse teste.

G0 mantém33 cenários+15 casos de recursos QUERY e8 mutações de campos,555 checks.
Gates usam código real; endpoints/SCM/RNG/KSP/tokens/processos/clock/proteção e
conteúdo dos positivos são modelados. Helpers de SID/SD/thread não são prova
bilateral nativa. Nenhuma operação protegida executada.

SCQ1 continua308 bytes/622 caracteres, record1048, SQB1/SQC11076; novos frames
nativos não foram introduzidos. [EVIDENCE.json](EVIDENCE.json) contém fontes/logs
e hashes; [qualify.ps1](qualify.ps1) reproduz a suíte afetada.

## 6. Recursos, delta e caminho mínimo

Censo Selected atual: ROOT367928, FIXED_GRAPH1569536, KNOWN1994976 bytes,
PROCESS_LIMIT3145728. Incremento32 bytes contra a tarefa anterior. São tamanhos
controlados/embutidos, não somar novamente nem tratar como pico nativo. O orçamento
G3 selecionado1994944 permanece imutável: sua proveniência é o censo anterior,
não uma fórmula que aumente automaticamente quando o código cresce.

Máximos loader/RPC/provider/kernel/stacks/compartilhados continuam UNKNOWN sob
G1. Baseline/runtime/host da sessão real não foram medidos e serão gates frescos;
contagem futura não é exigida antes de o processo existir. Retenção não recebe
crédito de fechamento futuro; Stop/deadline não prova quiescência OS.

Selected/Release workspace:708608 bytes,
SHA256 `139a9d87955263dbedbbe61218e1b1c85b95a202e1d64fdd18e876431bde49c3`.
Delta+6656 sobre workspace anterior701952 e +137216 sobre staged histórico571392.
DLLs workspace sem delta: ssl1321472/3fb3cd78… e crypto7217664/09eec573…;
hashes integrais no manifesto. Bundle candidato9247744 bytes, não selecionado
para operação. Snapshot anterior preservado em build/private-bootstrap-preserved-20261031.
O staged/serviço não foi revalidado nem atualizado nesta tarefa.

Caminho mínimo: decisão técnica do delta privado → binding geral autenticado e
guarda pré-Start condicional qualificados → estabilizar imagem → uma atualização
governada → pedidos separados de endpoint/Start/eventual Stop → prova nativa.
Não preparar update agora: ainda há alteração funcional pendente. Nenhum comando
com placeholders, retry ou aprovação operacional foi emitido/reutilizado.

E21 PENDING; ED11/Phase6 PARTIAL; pedido operacional NOT_READY; autoridade DENY.
Preservados HEAD/WIP/históricos/pins, autorizações consumidas, chave fixture,
UNKNOWN/reservas, OS_QUIESCENCE UNKNOWN e PST0.6.4. Não executados update,
endpoint/Start/Stop/SCM/ACL/CA/CURRENT/rede/TPM/Q4/cleanup/refund/recuperação.

[Próximo prompt executável](NEXT-AUTHENTICATED-GLOBAL-DEADLINE.md).
