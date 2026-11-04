# Encoding privado revisão 2

Implementação da decisão focal accepted de 2026-10-29. Inteiros big-endian,
sem structs serializadas. Tamanhos e quotas existentes preservados.

PACIS602, versão 2, ocupa 8192 bytes. Header 256 e footer SHA256 dos
primeiros 8160 bytes conservam offsets v1; nenhum leitor v1 aceita magic v2.
Eventos mantêm prefixo de 48 e registro de 276 bytes. A extensão de 112
bytes começa em evento+324: kind u32 (0 ordinário, 1 intenção, 2 ack),
reserved u32 zero, P do ato u64, R/I/A originais u64 cada, ordinal da
intenção u64, digest SHA256 integral da intenção (seu trailer) 32 bytes,
correlação do ato 32 bytes. DER começa em +436; SHA256 do evento sem
trailer encerra cada evento. Overhead fixo 468 bytes. Kind 0 exige extensão
inteiramente zero. Intenção usa estado físico 9, sem projeção PUBLISHED,
A/R/I e digest de referência zero, ordinal próprio e correlação não nula.
Ack usa estado físico 5, A original em published_epoch, referência à última
intenção do mesmo request e mesmos bytes da credencial; não contém DER.
I original é baseI+ordinal da intenção; R original deve corresponder a I
neste perfil ROOT06. Ambos são conferidos também pela seleção nativa.
P do ato deve coincidir com P vigente/approved_epoch neste perfil ISSUER_ONLY.
Nenhum digest ou correlação concede autoridade por si.

PACICP02, versão 2, permanece 512 bytes no mesmo arquivo de 16896 bytes.
Campos 0..239 conservam significado ROOT07; purpose u32 em 240 vale 2,
244..479 zero, SHA256 em 480. Seu R/I/A/P é o CURRENT que contém toda a
história de acks, independente do A original de cada ack. Pareamento de
versões é obrigatório. Não há migração implícita de imagens v1 existentes.

Sequência finita: CAS intenção/publicação original; CAS ack/publicação da
história; checkpoint pelo mesmo worker, sem nova seleção nem epoch; releitura
original. CAS ordinário v2 também confirma checkpoint antes de terminar.
PENDING retém contexto; falha/Stop conserva primeira causa e UNKNOWN.
Leitura DATA não reconstrói proofs ou autoridade NORMAL. Ausência de fontes
CA/key/custódia originais continua DENY. Nenhuma operação produtiva autorizada.
