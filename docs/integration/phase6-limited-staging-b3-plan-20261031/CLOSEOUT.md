# Closeout — plano B3 quantitativo e validação offline

Task PACC-PHASE6-LIMITED-STAGING-B3-EXACT-PLAN. Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469. B0–B8/hashb4b705cc... lidos e conferidos; accepted preservado. O proprietário esclareceu nesta task que não selecionou valores e pediu recomendações concretas, sem inventar fatos ou importar outros domínios. Não se registra seleção por inferência.

## Entrega decisória concreta

B3-PLAN-FOR-SELECTION.md reúne pacote, efeitos e propostas numéricas com consequências. SHA256e4ecd81a00ed4d09a272ac9e3219b1d3ef08b725049a109b6436dee9ee4b724e. Não é ADR, reaprovação B0–B8 ou pedido operacional. Primeiro gate: seleção explícita desse conjunto de tolerâncias/pacote ou indicação do que precisa ser alterado. Não se devolvem campos vazios solicitando números de engenharia.

Derivados: cinco criações/uma tentativa; handles H_READY+5 (não132 fixo); payload P9.094.144bytes e projeçãoR por unidade real. Propostas de risco, não bounds medidos: crescimento private/WS de um payload cada sem somá-los; pisos físicos/commit da própria amostra histórica de staging6.296.461.312/8.479.985.664, sem chamar aquele ensaio falho de seguro; espaço inicial2R/pisoR, freshness500ms/duração120.000ms. Valores temporais e fator de slack são recomendações explícitas de tolerância operacional para decisão, não inferências de benchmark ou norma. B3 não exige um teto absoluto opaco que B1 dispensou; critérios observacionais continuam selecionáveis e fatos UNKNOWN continuam como tais.

## Implementação efetivamente integrada

PAPACC_STAGING_CRITERIA_DATA privado no packet existente,72bytes, sem wire/formato persistido/ledger/setter ou concessão. Campos de limites de handles/private/WS, pisos físico/commit/caller/global e duração/freshness. Valores zero significam não selecionados, jamais defaults de128 ou outros domínios.

staging_limited_criteria_check avalia DATA: completude, freshness<=duração, observação/volume conhecidos sem primeira falha, domínio/ordem de QPC, multiplicação ms*frequency sem overflow, elapsed/freshness, limites do processo, pisos do host, coerência com payload arredondado e espaço disponível. Retorna erros específicos (NOT_SUPPORTED, INVALID_DATA/STATE, ARITHMETIC_OVERFLOW, TIMEOUT, NOT_ENOUGH_QUOTA/MEMORY, DISK_FULL). Resultado0 é apenas DATA compatível, não autoridade.

staging_admit LIMITED chama o avaliador com QPC corrente, não reutiliza timestamp do sample como se fosse agora; relógio/erro invalida. Ainda preserva B3 original source/plan obrigatório no blocker128 e nega mesmo se teste preencher critérios DATA consistentes. STRICT não recebe esse perfil de risco e segue preservado. Avaliador não altera consumo/efeitos/thresholds nem restaura tentativa.

Continua faltando o producer original do plano operacional selecionado e congelamento de READY/ligação de todos os gates, em particular volume atualizado e reavaliação temporal segundo os critérios escolhidos, antes dos efeitos. Isso não é substituído por flags em fixture. Guard conservador124 não foi removido com política ainda não selecionada. Não há CLI executável positiva para LIMITED, logo não se publica um comando fictício.

## Qualificação

Quatro builds Selected/Legacy Debug/Release PASS, /W4 /WX, sem warnings MSVC;17CTests/configuração,68execuções PASS. Staging56cenários/169checks; recursos34casos/136checks por configuração. Novos negativos de critérios ausentes, overflow, clock regressivo, freshness/duração, handles/memória/disco insuficientes; DATA sintética válida continua sem capacidade (zero creates/writes/consumed no wrapper). Os valores sintéticos dos testes não são recomendações ou seleções operacionais. APIs mutantes/counters/known folders/RNG modelados; SHA/SD em memória reais.

Censo Selected1.988.776/Legacy1.820.208 geridos, +72 do DATA. Reserva nomeada inclui ainda8bytes de LARGE_INTEGER do novo gate no cálculo conservador automático; sem afirmar limite de stack transitiva. Quotas3.145.728/2.097.152 inalteradas; contagem/root continuam dentro de limites. Logs/preimages preservados. Nenhuma coleta/teste nativo operacional executado.

## Pacote selecionável

PACKAGE-SELECTABLE.json contém fontes absolutas, hashes/tamanhos reais e destinos candidatos, total9.094.144bytes. Exe555008bytes/hash01db19a5009e227d2910d1f7266cfd43e79e36ed3dfadd3e3100bda4b94ffe90. DLLs conferidas: libssl1321472/hash3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed; libcrypto7217664/hash09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044. Não selecionados para execução. Binário anterior1dad... preservado/conferido em build/staging-observation-preserved-20261031/pre-b3-plan-1dad7c09. Históricos/pins/aprovações consumidas preservados.

## Pedido e caminho mínimo

Pedido operacional de staging ainda NÃO completo. Agora há recomendação revisável única de pacote/valores com hash, e validação quantitativa integrada; a falta decisória não é mais uma investigação vaga de custo opaco. Seleção do plano não autoriza execução e não prova identidade atual: operador/host/boot/token/handles/ancestrais/SD/volume são gates originais frescos, não dados históricos a reaproveitar.

Após seleção explícita do plano: completar producer/binding original no mesmo presenter/owner e qualificar os gates antes de efeito, revisar apenas o guard conservador desse domínio sob a política completa, conferir manifesto final e preparar pedido byte-exato com procedimento manual real. Se o código ainda mudar, novo hash deve ser conferido e apresentado como artefato derivado, sem seleção silenciosa. Aprovação operacional específica separada; nada executar automaticamente. Depois de staging autorizado/verificado, pedido independente de registro SCM parado. Sem Start/SCQ/CNG/CA/CURRENT por consequência.

Sem nova ADR, infraestrutura, aprovação operacional ou autoridade produtiva. E21 PENDING; ED11/Phase6 PARTIAL; staging DENY/NOT_READY. HEAD/WIP, UNKNOWN, reservas, chave fixture e PST0.6.4 preservados.
