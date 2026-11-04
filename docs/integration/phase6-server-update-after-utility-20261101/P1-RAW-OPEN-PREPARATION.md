# Prova mínima preparada — abertura raw0, sem atualizar a imagem

Estado: READY_FOR_OWNER_DECISION / NOT_AUTHORIZED / NOT_EXECUTED.
main / HEAD f341985924d29870d8ff627ed37c8a0874ad6469, WIP preservado.

Pedido operacional único: ONE-P1-RAW-OPEN-OBSERVATION-REQUEST.CANDIDATE.md
(8437 bytes), SHA256
**ce5b3f33ee311eae9f5771e0117a112e4a81d6e0197e388c049617da13bee166**.

O pedido explicita ANTES da autorização: argumentos são reproduzíveis;
equivalência histórica de processo/token/namespace/concorrência NÃO demonstrada.
Não afirmar que uma consulta independente recupera NTSTATUS descartado. Ela
identifica o status de sua própria abertura atual e testa a hipótese focal sem
UPDATE ou repetição da sessão. Falha histórica continua UNKNOWN por si só.

Fonte mínima p1-raw-open-observer.c e driver manual já preparados; nenhuma
alteração no produto/owner/runtime, formatos públicos/persistidos, guards ou
normas. É observador read-only pontual, como os cortes anteriores; não executor
administrativo, novo serviço/root/protocolo/canal ou store de autoridade.

O alvo é o pedido materializado, não a decisão:
FIRST-P1-SESSION-REQUEST.ORIGINAL.md no mesmo diretório desta nota;
ID417614100:8978432:356716,15529 bytes,pin86303aac1bf3b040a5949e8947a2bee2cb157fdbde472f31d5f2830b469a2a34.
Uma NtCreateFile: access00120089/share1/disposition1/options00200060,
attributes1040,rootNULL,mesmo nome absoluto prefixado por `\??\`.
Após sucesso apenas: identidade/atributos/tamanho/hash e revalidação pelo mesmo
handle. Não há pré-abertura do alvo pelo wrapper nem fallback. Uma divergência
não autoriza leitura de outro arquivo ou correção de ACL.

Preflight atual: host/SID/elevado/sem impersonação, drive fixo, count completo
imediato<124 e Stop não solicitado; tokenQUERY fechado antes da abertura.
No máximo uma tentativa, sem retry. A confirmação textual é somente da prova.
Não renovam G3, não admitem E/Q/S, não solucionam retenção histórica.

## Qualificação realizada

Toolchain existente MSVC18/vcvars64. build-p1-raw-observer-offline.cmd compila
observador /O2 e modelo /Od, ambos x64 /W4 /WX. PASS, zero warnings.
Modelo executado somente com OBSERVER_MODEL, que exclui context/verify/wmain
nativos. NtCreateFile injetada valida nome/Unicode/root/SD/QoS/acesso/share/
disposition/options/EA/allocation. 23checks PASS:113/123 admitidos no modelo,
124/128/queryUNKNOWN/Stop negados; sucesso/ausência de handle/NT access-denied,
sharing e reparse diferenciados; IOSB inconsistente/pending/unexpectedhandle.
O teste não consultou alvo/token/SCM/endpoint reais. Win32 erro5 não é utilizado
como substituto de status nativo. Não é prova de equivalência de contexto real.

Saída do build/modelo:
```
p1-raw-open-observer.c
p1-raw-open-observer.c
RAW_OPEN_MODEL checks=23; NO_NATIVE_OPEN_NO_TOKEN_NO_SCM
```
PowerShell AST: MANUAL_AST_PASS_NOT_EXECUTED. Driver NÃO executado.
Não repetidas campanhas Selected/Legacy do produto inalterado.

Pins adicionais:
- exe145408bytes:7f1f8bddad246144803406a97f6b849c84d4a556d334fbee4448ee680638cf1d
- fonte8553bytes:224e017b0fbc6ab0bd2b4e2a2afa7ea11edec3cdc8a001630a6ca476a0f805c3
- manual1964bytes:087f5a164f770cd3d239e50765e18fda3577e9bef2c55004ea4e9b77de650c3c
- build715bytes:53b63c34d8984d5a7b0f193cc4378870a459afee225382f9d8be2aaf53d6395b

## Compatibilidade e caminho mínimo

OHI/SSO-B/TO/QUERY-G3/G0–G9/P0–P7 mantêm origem/proteção/consumo/receipt/close
e separação DATA/autoridade. Não há admissão administrativa no observador.
Assim não se exige nova especialização de confiança ou alteração de accepted:
exige autorização operacional read-only própria. Pins/SID/posse não a substituem.
124/128/G3 intactos; não importar riscos opacos G0–G9 como dispensa de recursos.
Documentação da API e CNG consta no pedido; APIs de hash não acessam KSP/chaves.

Primeiro gate restante é a aprovação dessa única prova, com sua limitação
expressa. Depois, preparar comando literal com os pins; somente o proprietário
executa manualmente. A saída permitirá delimitar NTSTATUS atual ou falha de
preflight; não iniciar outra campanha ou presumir correção. Nenhum UPDATE
instrumental necessário para este caminho. Só uma causa concreta justifica
correção posterior e eventual nova sessão E/Q/S com autorização independente.

Próximo prompt: NEXT-P1-RAW-OPEN-OBSERVATION-AUTHORIZATION.md.
Imagem instalada772608bytes e IDs/históricos preservados; não reconsultados.
Sem coleta, UPDATE, endpoint/SCQ/Start/Stop, SCM/ACL, cleanup/refund/recuperação,
CA/CURRENT/rede/TPM/NV/Q4. Retenção histórica e OS_QUIESCENCE UNKNOWN, DENY,
autorizações consumidas, HEAD/WIP/pins/originais/EXEs/reservas/PST0.6.4 intactos.
