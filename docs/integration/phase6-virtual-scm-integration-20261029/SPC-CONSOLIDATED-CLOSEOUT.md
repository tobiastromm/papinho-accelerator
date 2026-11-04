# SSO-B — conclusão offline da cadeia pré-readiness

TASK: PACC-PHASE6-SSO-B-PRE-READINESS-COMPLETION.
Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469 preservado.
SSO-B/S0–S7 permanece ACCEPTED pela decisão de20261030, proposta original SHA256
60cdd50be68fa15c1379c31f287edd697140cfc2bf39ff8b319b86f0e5f08fa3 preservada.
Sem nova ADR, reaprovação, alteração accepted, root operacional ou autoridade.

## Resultado e sequência

A dependência circular foi removida no caminho privado fechado. A cadeia foi
qualificada em modelo fonte-linkado, incluindo ServiceMain; não é prova bilateral
nativa nem instalação pronta. A admissão produtiva continua DENY.

| Etapa | Integração | Evidência e limite |
|---|---|---|
| Entrada SCM | seleção interna exata `service host scq-closed`, nome/argc SCM conferidos, LAB rejeitado | Dispatcher/handler/status simulados; seleção não é autorização |
| Owner | mesma root estática e acquire/finish existentes, rede/config vazias, reservas e writer | teste de owner canônico real em memória termina fonte vazia sem endpoint/CNG/arquivos; nenhuma root publicada |
| Controller original | confirmação própria, take e scqa_startup_original antes de runtime | origem/arquivos/token/decisão modelados; PID futuro não preenchido |
| Identidade fechada | capture_closed: refs originais e todos os predicates existentes, SCM START_PENDING exato | mesmos TokenUser/grupos/restrições/PID/criação; normal mantém RUNNING estrito |
| Entrega | perfil fechado local, receptor OHI, scqr_acquire/open/admit_offer e transport apply | conteúdo/SD/refs e identidades nas duas pontas revalidados; IO/peers/RNG modelados |
| Readiness | consulta direta à root canônica, scqr_closed_ready_current e arm_closed_running antes de report | fonte local independente, consumo e native-delivery exigidos; nenhuma flag do caller substitui evidência |
| SCM RUNNING fechado | após arm/prova: report RUNNING e revalidação nativa do MESMO processo/token/serviço | modo fechado explícito, distinto de runtime_ready produtivo; erro/Stop conserva UNKNOWN |
| Operação posterior | runtime_ready, dispatch/probe CNG e adapter KSP negam contexto fechado | não há promoção para produção; exige outro ato/autorização própria, não esta seleção |

A representação fixa do record/SQR1 não mudou de tamanho, versão ou campos.
O novo construtor fechado exige Pending como estado observado no binding; os
codecs e construtores ordinários continuam rejeitando Pending. O campo privado
pre_readiness não é wire, armazenamento de grants ou permissão operacional.
Após evidência completa, o estado privado da identidade arma a expectativa de
SCM Running; current() então exige Running factual. O record original continua
como observação do binding Pending: ele não é reescrito como se Running já
existisse. Não há fallback entre estados nem reinterpretação de registros antigos.

## Callers e segurança

main.c.api3.inc desvia o callback fechado ANTES de configuração/provisionamento,
WinSock e runtime produtivo. ServiceMain permite somente preparação nesse modo;
ps_binding_ready continua exigido no caminho produtivo e nas mutações SCM.
A flag de seleção não permite reportar readiness. A única passagem fechada
consulta a evidência do owner canônico, arma o estado com evidência original e
só depois anuncia SCM RUNNING diagnóstico. Continuação sem lifecycle governado
retém o owner; nunca declara quiescência por retorno inteiro do callback.

Os wrappers nativos do controller, recipient e transporte agora aceitam a fase
Pending estritamente fechada; o bind do controller exige ato original já tomado.
Nenhum owner, serviço, IPC genérico, diretório, pool ou autoridade foi acrescentado.
A ligação estática serviço/runtime é privada e recíproca para consultar a MESMA
root; não há callback configurável ou setter de autorização. N0 não foi alterado.

Fonte ausente na entrada canônica termina o owner de metadados recém adquirido,
cujo conjunto de recursos continua vazio por construção. Um owner pré-existente,
originais/IO vivos, falha de report/fechamento ou perda de continuidade são retidos;
não são descartados para conseguir STOPPED. Stop concorrente após RUNNING fechado
nega prontidão e conserva UNKNOWN. Não rearma consumo, reconstrói F0 ou faz retry.

## Qualificação final

Selected e Legacy, Debug/Release: quatro builds PASS, zero warnings.
Doze CTests afetados/configuração:48 execuções PASS. Quatro observações puras de
layout PASS. A lista exata está em SPC-CTEST-PATTERN.txt e os resultados em
SPC-*-ctest.log/LastTest.log. Não executada a campanha de inputs com CNG real.

Novo teste completo:16 cenários,548 verificações/configuração;4 readiness fechadas
confirmadas antes dos fechamentos,12 retenções de host modeladas. Inclui fonte
ausente, token/identidade/escopo divergentes, receptor não RUNNING, entrega antes
da readiness, replay, peer perdido, Stop, falha de fechamento, report Running
falho e Stop concorrente. Nenhum Running prematuro/produtivo. A contagem não prova
quiescência de todos os participantes: as fontes modeladas retidas continuam em
backings estáticos distintos até o fim do fixture; não são liberadas por sucesso.

O teste adicional closed_empty_model usa o owner canônico compilado de verdade
para acquire/finish em memória, sob dispatcher/status simulados: sem fonte,
readiness e endpoint continuam negados. Não é uma root C8 publicada nem autoridade.

Quatro mutantes compilados e rejeitados: aceitar Running na captura Pending;
retirar separação de readiness produtiva; anunciar Running antes da prova;
permitir closed alcançar KSP. Somente executáveis de modelo foram executados;
originais foram restaurados e os artefatos finais recompilados. Logs SPC-mutant-*.
RNG, KSP, endpoint e APIs mutantes permanecem simulados; SID/SD helpers e threads
auxiliares das regressões são reais. Nenhuma chamada CNG/KSP real foi necessária
nesta tarefa; a divergência histórica SSB permanece registrada sem apagamento.

## Recursos

Layout C8 compilado: Selected1979776/3145728 bytes; Legacy1811208/2097152.
Crescimento do pacote/root16 bytes (identidade+8, perfil+8); host SCM104 bytes,
antes96 (+8), PS_NATIVE12816 e atomic4 separados. O layout C8 impresso não inclui
essa global SCM: o acquire já a cobra dinamicamente por storage_bytes(), sem
sobrepor owners. Soma estática desses componentes: Selected1992700 e
Legacy1824132; não é pico nem ceiling universal. Crescimento combinado24 bytes.
Nenhuma quota alterada;124/128/1024 preservados. Heap/kernel/provider, stacks
transitivas e headroom reais continuam UNKNOWN; não há crédito por modelos.

## Fronteira factual e pedido operacional

As seleções operacionais de decisão SCQ, endpoint e CNG permanecem vazias. A
entrada real não carrega provas de fixtures e não aceita aprovação normativa
como ato operacional. O caminho positivo exercitado recebe originais modelados
por factories de teste explicitamente identificadas, nunca por setters produtivos.

Primeira lacuna factual: par administrativo/runtime e referências originais de
uma sessão especificamente autorizada, observados em contexto SCM real Pending,
com alvo/imagem/proveniência e política quantitativa/freshness completos.
O registro operacional e a entrega nativa desses originais NÃO foram executados.
O runner produtivo não é um executor autônomo dessa prova: sem origem recebida,
nega; se houver provas vivas sem continuação lifecycle governada, retém. Não
prometer que aprovar endpoint isoladamente tornará executável essa sessão.

Pedido operacional exato: NOT_READY. Faltam bindings originais e política da
sessão real; não há comando ou placeholders. Eventual criação/conexão de endpoint,
Start/Stop/SCM e probe CNG são autorizações independentes; parent só quando
necessário, com sua autorização própria. A primeira prova bilateral pode excluir
KSP/CA/CURRENT e não qualifica isolamento CNG MACHINE por consequência.

Caminho até NEW_INSTALL: fechar esses bindings e um pedido nativo delimitado;
obter/qualificar originais e lifecycle SCM/token; qualificar isolamento CNG MACHINE
em autorização distinta; preparar parent/cerimônia NEW_INSTALL/CA_INITIALIZE e
publicação somente sob os respectivos gates. Sem reabrir TPM/Q4, criar nova
infraestrutura ou tratar fixtures como autoridade. E17/E18 recebem somente
cobertura offline nova; E19/E20, E21 e demais gates não são promovidos.

SSO_B_PRE_READINESS_CHAIN=PASS_OFFLINE_MODEL;
NATIVE_BILATERAL=NOT_DEMONSTRATED; OPERATIONAL_AUTHORIZATION=NONE;
OPERATIONAL_REQUEST=NOT_READY; E21=PENDING; ED11_PHASE6=PARTIAL; AUTHORITY=DENY.
Próximo prompt: NEXT-SPC-ONE-NATIVE-ORIGINAL-BINDING.md.
Evidências/hashes finais: SPC-QUALIFICATION.json, SPC-SOURCE-HASHES.json e
SPC-ARTIFACT-HASHES.json. HEAD/WIP/históricos/pins/chave fixture/UNKNOWN/reservas e
PST0.6.4 preservados. Nenhum endpoint, instalação, SCM/ACL, rede, CA, CURRENT,
TPM/NV/Q4, limpeza/refund ou recuperação operacional foi executado.
