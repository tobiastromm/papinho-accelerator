# Coleta nativa de staging — falha fechada e diagnóstico corrigido

Fonte: saída manual enviada pelo proprietário, transcrita em OWNER-OUTPUT.txt.
Proveniência owner-reported, não recibo assinado/atestação independente.
Branch main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469; WIP preservado.

## Resultado factual

Uma invocação nativa foi relatada, exit2, first1816, primeira localização4
(aquisição dos ancestrais de ProgramFiles). A autorização da coleta9ffe2080...
e a exceção Process38fc35af... foram utilizadas para essa invocação: não repetir.
O `consumed=0` do output é o ato de STAGING, não um crédito para repetir a coleta.
Nenhuma aprovação ou nova execução foi criada/inferida neste fechamento.

| Evidência | Valor e alcance |
|---|---|
| Frame completo alcançado | Apenas phase1, após loader/root zeroing |
| Handles nesse frame |115; não é a contagem da negação posterior ou pico global |
| PrivateUsage |2.867.200 bytes, amostra; não é censo de memória gerida |
| WorkingSet |7.172.096 bytes, amostra; não comparar com o cap de memória gerida |
| Físico disponível |6.662.586.368 bytes nessa amostra; não margem/reserva admissível |
| Commit disponível |9.096.044.544 bytes nessa amostra |
| Janela QPC |38.449 ticks/10.000.000Hz =3,8449ms do frame; não duração total |
| Máscara remanescente |95: observação inicial existe, volume e demais custos continuam UNKNOWN |
| Consulta de volume |Não alcançada; zeros não significam disco sem espaço |
| Descritores reportados |parent168, nova proteção124 bytes, scratch não observado |
| Fechamento controlado |known1, retained0, refs de staging0 ao final, phase6 |
| Efeitos/ato staging |effects0, consumed0, RNG não chamado, confirmação não pedida |
| Contexto nativo |first_predicate0/native_error0; SID não emitido, não inventar identidade |

Não foram demonstrados espaço de disco, fontes nativas do pacote, headroom de
preparação, pico nativo, cohort administrativo, custos ativos do FS ou admissão.
Fechamento interno do OS permanece UNKNOWN. O bloco tinha finally de restauração
de Process e não mostrou erro, mas o valor final da política não foi observado
independentemente. Não consultar a política do processo sandbox como substituto.

## O que1816 prova e o que o log não distingue

O código aprovado de staging_open pode retornar1816 quando uma consulta de
handles falha com esse erro OU quando a consulta tem sucesso e own_count>=124.
Ambos podem produzir a mesma primeira localização4 e fechamento conhecido.
O NtCreateFile ainda não seria chamado nesses dois ramos. O log antigo não
registrava sucesso/erro/contagem da consulta; logo não é válido concluir que
115 era o valor do guard, que houve leak ou que a RAM/disco se esgotou.

O crescimento entre inicialização e aquisição também não foi atribuído. Known
folders, contexto/token e materialização de descritores estão entre os pontos
da execução, mas não são declarados culpados por inferência. Não baixar guard,
elevar128/1024, omitir referências ou trocar de host para obter PASS.
O handles115 é consultado antes das demais queries e da impressão do próprio
frame: sua inicialização pode consumir recursos depois dessa amostra. Não
atribuir esse intervalo ao layout que ainda não tinha sido executado.

## Correção técnica offline

No mesmo packet/owner, staging_open agora conserva e reporta o resultado da
MESMA consulta anterior à abertura: attempted, succeeded/count_known, own_count,
query_error, guard_denied e object_index. Uma falha de API não vira contagem0;
nenhuma nova consulta é usada para substituir o instante original.

No modo somente observação foram acrescentados três frames: após known-folder
resolution(7), após contexto original(8), após SID/proteção(9). Os cinco frames
anteriores permanecem, total8 quando todos forem alcançados. A localização da
primeira falha e a fase de cada sample são domínios distintos, preservados.
Uma query simples adicional após o primeiro frame e antes do layout separa o
autocusto inicial do observador. Ela não é outro frame completo e não mede teto
interno do OS; conserva sucesso/contagem/erro próprios, sem fallback ou retry.
Nenhum guard/ACL/direito/backend ou fonte de admissão foi relaxado. Sem otimização
especulativa, fonte positiva, novo owner/root/ledger ou operação protegida.

Dois casos reproduzem o mesmo frame inicial115/erro1816/location4: contagem
posterior124 com consulta válida, e query falhando1816 com contagem UNKNOWN.
Ambos negam, não abrem ancestral, não consultam volume/RNG e fecham o cohort.
Dois casos adicionais isolam a negação antes do layout por count125 ou query1816,
sem culpar etapas que não foram chamadas. Resultado:30 casos/99 checks de
recursos por configuração;48 cenários/141 checks
de staging preservados. Quatro builds Selected/Legacy Debug/Release PASS, zero
warnings;64 CTests afetados PASS. Sem nova coleta nativa e sem repetir mutantes
de semântica inalterada. APIs mutantes/contadores continuam simulados nos testes.

Cohort+40 bytes de diagnóstico:packet7.360; Selected1.988.704 e Legacy1.820.136
geridos, sem crédito de capacidade nativa. Três frames a mais também têm custo
de observação: seus custos opacos/temporais não são declarados zero ou ceilings.

## Preservação e próximo gate

Antes de rebuild, os três artefatos aprovados foram preservados por bytes/hashes
em build/staging-observation-preserved-20261031/approved-9ffe2080. O manifesto
APPROVED-ARTIFACT-ARCHIVE.json distingue cópia de evidência de identidade original
ou staging. Pedido, script e registros de autorização antigos ficaram intactos;
seus estados pendentes eram snapshots históricos, reconciliados separadamente
em OWNER-USE-RECONCILIATION.json. O novo build NÃO herda a aprovação antiga.

Novo candidato: exe553.472 bytes, SHA256
`1d2b92c2c397e1415db83d57cb8b3ac22127107855123f0340c573e7940dbe85`;
três arquivos/9.092.608 bytes em DIAGNOSTIC-BUNDLE.json. Não é seleção operacional,
staging, imagem instalada ou release. O script antigo permanece pinado ao exe
anterior e não deve ser executado novamente.

DIAGNOSTIC-REQUEST.CANDIDATE.md delimita uma nova coleta, não aprovada. Ela só
busca resolver a ambiguidade de handles e a primeira fase de crescimento. Não
promete alcançar volume ou provar capacidade. Eventual exceção Process/BYPASS
para essa nova coleta também está proposta explicitamente, não herdada.

Primeiro gate: aprovação específica do novo pedido diagnóstico byte-exato.
Nenhuma execução/retry pelo agente. Staging continua DENY; E21 PENDING;
ED11/Phase6 PARTIAL. Sem SCM/ACL, Start/Stop, endpoint, conta/CA/CURRENT, rede,
TPM/Q4, KSP/ownerAuth, cleanup/refund ou recuperação. Produção PST0.6.4 preservada.
