# Preparação exclusiva do utilitário — fechamento offline

Task PACC-PHASE6-ONE-UTILITY-PREPARATION-ACCEPTANCE, 2026-11-01.
Branch main, HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado.

## Decisão e implementação

[Decisão accepted](../../adr/DECISION-personal-one-utility-preparation-20261101.md)
incorpora o original byte-exato 029a611bb19e41827719c4269f6e029895a8f57f2d125141258274da8b2707fb,
preservado. Aprovação exclusivamente normativa/offline; operação NÃO autorizada.
Não foram identificadas divergências no alcance delimitado da proposta.

O owner existente recebeu UTILITY_PREPARE_EXCLUSIVE e entrada restrita no server.
Mesmo packet, sem novo root/serviço/canal/ledger. Controller de build qualificado
é bootstrap somente para esse ato; papacc_update permanece UPDATE-only e não
pode preparar a si próprio. U2 continua exigindo executor protegido independente.

Originais do controller/DLLs/DATA/canônico, token/processo/criação, até seis
diretórios de origem e cadeia protegida são retidos/revalidados. Plano próprio,
nonce/declaração/confirmação atribuída, CAS de consumo antes do efeito. Uma
criação relativa exclusiva de papacc_update.exe, cópia/hash/flush compartilhados
com primitivas existentes, readback por parent original e fechamento ordenado.
Sem DELETE, overwrite, rename, diretórios novos ou SCM. Colisão nega; efeito
parcial/close incerto retém disposição UNKNOWN, sem limpeza ou retry.

Separação explícita no dispatcher, no plano e nos barriers impede usar esse
modo em UPDATE/renames ou inferir autoridade de DATA/owner/hash. Guarda estrita
do parent/canônico/novo arquivo preservada; exceção de bootstrap somente nos
originais de origem designados pela norma. SD de DATA deve ser interpretável,
não nulo. Aliases/reparse/substituição e falhas materiais negam.

## Qualificação e limites de evidência

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings nos logs.
64 pares distintos teste/configuração afetados PASS, 16 por configuração.
Após ampliar somente a fixture de profundidade máxima, os três testes afetados
foram reconstruídos/reexecutados nas quatro configurações; sem contar esses
reruns como nova campanha ou novos pares.

- Preparação final: 53 cenários / 295 verificações por configuração.
- UPDATE: 102 cenários / 602 verificações; entrada do utilitário: 20 verificações.
- APIs mutantes, RNG, SCM, endpoints e efeitos do produto simulados. Inclui
  fonte/parent/proteção/hash/token/PID divergentes, colisão, I/O parcial,
  recursos insuficientes/UNKNOWN, Stop, replay, consumo e close/retention.
- Helpers read-only do wrapper: 17 verificações reais Windows PowerShell5.1
  x64 sobre arquivos/diretórios do workspace. Loader exercitado somente com
  executável de teste owner-stub (20 verificações), nunca controller/utilitário
  de produto. Não é preparação nativa nem prova no alvo protegido.

Logs base *-tests.log/*-details.log e overlays finais *-final-tests.log/
*-final-details.log preservam ambos os estados da qualificação. A fixture
anterior 52/285 é histórica; o resultado final é 53/295.

K_PREP Selected=2024636 bytes, Legacy=1856068 bytes; composição de sizeof/censo,
não memória de processo medida ou teto nativo. Quotas existentes 3145728 e
2097152 bytes respectivamente, sem aumento. Packet privado cresceu 32 bytes;
formatos públicos/persistidos e perfis existentes preservados. Gates físicos/
commit >=K_PREP, volume real e espaço arredondado, recursos e originais frescos
por evento antes dos efeitos. Não importa handles124/128 ou B/R/G/U4.

Custos opacos CRT/loader/kernel/filesystem/flush/retention permanecem UNKNOWN,
sob especialização aceita; monitoração não reserva recursos nem garante pico,
latência ou cancelamento. OS_QUIESCENCE UNKNOWN. Nenhum diagnóstico operacional,
consulta do alvo protegido, SCM ou preparação de produto executado nesta tarefa.

## Artefatos e próximo gate

Selected/Release congelados para PROPOSTA de seleção operacional:

| Arquivo | Bytes | SHA256 |
|---|---:|---|
| papacc_server.exe controller |758272|7612d1114b641875ad4536a6e9a2a4899a9115674b8a098fe7cc8b7767b1cf7f|
| papacc_update.exe DATA |104448|6c0e77173f772b7748d08d9ef2ca29f844d629cc0090b3a73a3cafa87ea0bb87|

DLLs idênticas, apenas lidas; pins completos em FINAL-ARTIFACTS.json. EXE
instalado antigo 571392 bytes/c0f27f94…806ecf continua sem substituição demonstrada;
serviço, DLLs instaladas e ProgramData não foram modificados.

[Pedido operacional](ONE-UTILITY-PREPARATION-REQUEST.CANDIDATE.md), SHA256
`a8dd42aad016b30ef574181a148bf5b7631053cdb1225a5167ed185b606d9489`,
READY_FOR_OWNER_SELECTION_AND_SPECIFIC_MANUAL_AUTHORIZATION, NÃO AUTORIZADO.
Procedimento candidato SHA256
`3c16e6b50a369b011ee9df9dee8b1276bb5bb781b86d98eaa11a427eebb63df5`.
Somente o parent protegido de criação admite share WRITE; arquivos e cadeia
fonte negam escrita/delete concorrentes. Procedimento completo NÃO executado.

Primeiro gate: seleção dos artefatos e autorização operacional específica do
pedido byte-exato. Depois, registrar ato e preparar comando manual conferido,
sem execução automática. Nenhuma autorização anterior é reutilizada.

Caminho mínimo: preparação manual autorizada de UM utilitário → reconciliação
de ID/hash/proteção/readback/close → executor independente estrito U2 → pedido
UPDATE próprio → UPDATE/reconciliação → endpoint/primeiro Start diagnóstico
sob autorizações separadas. E21 PENDING; ED11/Phase6 PARTIAL; produção e demais
autoridades DENY. PST0.6.4 preservado. Presença do utilitário não autoriza uso.
