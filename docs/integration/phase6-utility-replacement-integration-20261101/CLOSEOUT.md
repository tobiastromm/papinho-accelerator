# Manutenção do utilitário — integração offline concluída

2026-11-01. main/HEADf341985924d29870d8ff627ed37c8a0874ad6469. WIP preservado.
Nenhuma consulta real do alvo/SCM ou operação de produto executada nesta tarefa.

## Duas decisões independentes

1. Consulta read-only079f72d1…fb483 autorizada para UMA execução MANUAL, sem
   retry. Pedido/script originais byte-exatos preservados. Registro em
   ../phase6-update-disposition-utility-delta-20261101/OWNER-READONLY-AUTHORIZATION.md.
   Comando pronto em [MANUAL-COMMAND.md](../phase6-update-disposition-utility-delta-20261101/MANUAL-COMMAND.md).
   Consulta ainda PENDENTE, não executada pelo agente. Não altera autoridade UPDATE.
2. Proposta00d0cd8dca8ebea3d221a5e0d9b09281ab42a300b93d3dbd52a294ca804003b6
   conferida integralmente e aceita por decisão explícita, registrada ANTES do
   código em [DECISION-personal-utility-replacement-20261101](../../adr/DECISION-personal-utility-replacement-20261101.md).
   Somente implementação/qualificação offline; manutenção real NÃO AUTORIZADA.

Não há conflito identificado no alcance focal: U1–U7/U2 continuam no UPDATE
servidor, preparação exclusiva anterior continua limitada ao seu ato consumido.
A especialização de manutenção é alvo/purpose próprio e bootstrap/risco opaco
restritos, não redução da guarda do executor UPDATE servidor.

## Código efetivamente integrado

Entrada server --owner-manual-update-utility, negada no LAB e em configurações
sem capacidade. Mesmo owner, packet, slots e biblioteca; nenhum novo updater,
serviço, root, canal ou formato persistido. papacc_update continua UPDATE-only e
recusa essa entrada (21 checks), portanto não pode se atualizar por esse caminho.

O bootstrap compartilha as mecânicas originais de aquisição/continuity do
controller server e dependências, até seis diretórios fonte retidos, DATA e
parent protegido. Modo interno fechado2 selecionado somente pelo factory desse
ato; não é grant de caller. Domínio de plano/hash e confirmação próprios
UTILITY_UPDATE_CLOSED/OWNER_UTILITY_UPDATE. Contexto/token/PID/criação/imagem/
SD/IDs/hash/aliases originais revalidados. Getters/barriers negam sem esses
originais e confirmação, não há positivo derivado de fixtures/metadados.

Alvo antigo fixo papacc_update.exe no parent instalado, SD estrito e DELETE
somente para os renames autorizáveis. Server/DLLs/dados/candidato anterior não
são alvos de mutação. SCM é query-only e deve corresponder ao comando fechado,
conta virtual/DEMAND_START/UNRESTRICTED, STOPPED/PID0 antes e durante efeitos.
Snapshot não impede ação de outro admin. Não chama Start/Stop/configuração SCM.

Uma confirmação independente, consumo CAS antes de criação exclusiva, cópia/
hash/flush, rename same-directory NULL/leaf do utilitário antigo para previous,
readback, rename do novo para papacc_update.exe, readback e close governado.
Nomes derivam da tentativa original e são mostrados antes de confirmar. Hashes,
objetos, direitos e plano são revalidados nos barriers; Stop nega novos efeitos.
Sem overwrite, rollback, delete, limpeza, refund ou execução do utilitário DATA.

Reuso bounded do slot15: só uma referência temporária de readback por geração.
Depois do primeiro readback, fechar SOMENTE essa referência uma vez com sucesso;
original antigo slot13 continua retido. Só então limpar o slot conhecido/bit
da geração encerrada para novo readback canônico. Se CloseHandle falhar, reter
cohort/bit/handle UNKNOWN e não emitir segundo rename. Nenhuma quota/slot aumenta
ou original é fechado antecipadamente. Máxima cadeia de seis diretórios testada.

## Recursos próprios e qualificação

K_UTILITY_UPDATE Selected2026432bytes; Legacy1857864bytes. Censo explícito dos
frames bootstrap aplicáveis mais coexistência SCM/rename/readback e geração
temporária. Usa a mesma composição conhecida onde o lifetime é aplicável,
sem transportar o número antigo como capacidade. Quotas3145728/2097152 mantidas;
packet não cresceu, slots adicionais0. O label histórico K_PREP no relatório
compartilhado mostra o valor do K realmente reservado para o modo ativo; o censo
imprime separadamente UTILITY_UPDATE_K. Não confundir com K da primeira preparação.

Física/commit>=K próprio, payload arredondado por volume real, caller/actual
free e freshness por evento antes dos efeitos. Contagem completa de handles
observada, sem quota arbitrária. Custos opacos/latência/OS_QUIESCENCE UNKNOWN;
monitoramento não é reserva de capacidade ou garantia de cancelamento imediato.

Selected/Legacy Debug/Release: quatro builds PASS, zero warnings.17 CTests
afetados por configuração,68 pares distintos PASS. Resultado final:

- Manutenção:29 cenários/145 verificações por configuração.
- UPDATE servidor:106/634; preparação exclusiva:53/295; entrypoint utilitário:21.
- Casos novos: confirmação ausente, hash/owner/DACL/token/controlador/destino
  divergentes, controller=utilitário, DATA errada, serviço running/PID divergente,
  recurso UNKNOWN, colisão exclusiva, replay/concorrência, troca de propósito,
  Stop/flush, conflitos nos dois renames, readbacks divergentes e close incerto.
- APIs mutantes FS/SCM/RNG/processo simuladas. Nenhum produto/endpoint real.
  Falha e retenção não viram sucesso ou crédito. Sem chamada ao owner retido real.
- Wrapper draft: parsing/argc14/fechamento do preflight antigo verificados;
  helpers read-only17 checks reais só no workspace e loader apenas do executável
  de teste owner-stub/21 checks. Procedimento completo NÃO executado.

Falha inicial na fixture de manutenção: handlers antigos interceptavam readback
do utilitário como abertura de executor UPDATE. Corrigido o escopo dos handlers;
logs inicial/final preservados, sem relaxar produção. Não repetir campanhas
históricas sem alteração; novos reruns ficaram limitados ao teste alterado até
qualificação final da composição comum.

## Artefatos finais de qualificação, não selecionados operacionalmente

| Selected/Release | Bytes | SHA256 |
|---|---:|---|
| Controller papacc_server.exe |762880|3d59007bdc0714eb89eb646479e7a23d52fd82c953f1c72deec541a3643bce0b|
| DATA papacc_update.exe |104448|084aa081830263d0ed8d502297a411c83766ff7f7ea7bc46a0ae24c06068822a|

DLLs continuam com pins idênticos históricos, sem alteração real. FINAL-ARTIFACTS.json
SHA256b3903c289bd2fadd290c70478d273b67f9f51e441ffd1760a8773d668135c43a.
Mudança do controller é esperada pela nova entrada; não substituir silenciosamente
seleções de pedidos anteriores. Utilitário protegido histórico6c0e7717…0bb87 e
objetos da tentativa anterior NÃO foram alterados/reconsultados nesta tarefa.

update-utility.MANUAL.DRAFT.ps1 prepara os bindings atuais/13 argumentos mais
argv0, source utility DATA, cinco pins e seis componentes de IDs, manifesto e
norma. Não é pedido operacional nem comando autorizado: exige pedido futuro
exato que ainda não existe. Retém cadeias/controller/DLLs/DATA sem share WRITE/
DELETE; fecha somente preflight do utilitário antigo antes de native DELETE
capture, que o readquire por parent/ID/hash. Não transportar esse draft para
operação antes da reconciliação/seleção/autorização próprias.

## Primeiro gate restante e caminho mínimo

Proprietário executar UMA vez a consulta manual já autorizada e enviar saída
integral. Reconciliar canônico/candidato/previous/utilitário/parent e SCM.
query_close_known não resolve historical_owner_close/OS_QUIESCENCE. Até lá,
disposição atual continua UNKNOWN; nenhum pedido de manutenção real completo
ou retry de UPDATE é apresentado. Não exige nova decisão normativa para as
correções técnicas ordinárias já cobertas pela aceitação desta tarefa.

Após evidência suficiente: manter explicitamente incertezas residuais e não
reusar/liberar candidato/créditos históricos; finalizar UM pedido de manutenção
com artefatos selecionáveis/gates/riscos próprios. Não exigir prova universal
impossível nem presumir quiescência a partir do wrapper. Manutenção qualificada
factualmente→executor protegido corrigido→novo pedido UPDATE servidor somente
quando disposição/retention permitirem→primeiro Start diagnóstico sob autorização
independente. Nenhuma infraestrutura adicional necessária.

E21 PENDING; ED11/Phase6 PARTIAL; produção DENY; quiescência OS UNKNOWN.
Preservar HEAD/WIP/históricos/pins/EXE anterior/objetos/DLLs/SCM/ProgramData,
autorizações consumidas, reservas, chave fixture e PST0.6.4.
