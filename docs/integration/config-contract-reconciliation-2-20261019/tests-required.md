# Testes necessários — planejados, não executados

| ID | Caso | Resultado exigido |
|---|---|---|
| C01 | # integral/inline e # em string | Comentário fora, literal dentro, spans preservados |
| C02 | ; primeiro não branco, espaços e tabs | Linha inteira sem tokens/AST/refs/rules/listeners |
| C03 | ; dentro de string | Valor literal passa à validação do campo |
| C04 | ; inline depois de diretiva, end ou predicate | Erro lexical; nenhuma aplicação parcial |
| C05 | # comentário contendo ; | Texto já comentado não tokenizado |
| C06 | Sintaxe inválida/aspas abertas após ; inicial | Linha desativada ignorada inteira, sem diagnóstico de diretiva |
| C07 | Remover ; de linha válida | Somente candidate; sem commit/CURRENT/grants/rede |
| C08 | Opener ativo, end desativado | Bloco incompleto rejeitado |
| C09 | Opener desativado, campos/end ativos | Contexto inválido rejeitado |
| C10 | Campo obrigatório desativado | Rejeição, sem initializer permissivo |
| C11 | Regra inteira desativada | Regra não existe; ordem física restante preservada |
| C12 | Predicate parcialmente desativado inline | Erro lexical; não ampliar match |
| C13 | Exemplo distribuído | Cinco linhas ativas, zero listener/regra/ref operacional |
| A01 | Condições IP/group/calendar não implementadas ativas | Candidate integral rejeitado |
| A02 | Principal16 versus usuário/local Session ID | Sem conversão/invenção de identidade |
| A03 | ACCEPT depois DENY e inverso, AND/no-match | Primeira terminal, conjunção, DEFAULT_DENY |
| L01 | Profile ausente/unknown, Secure sem ref | Rejeitar, sem fallback Legacy |
| L02 | Coleção acima do target um | Erro explícito, sem truncar |
| L03 | Um listener/múltiplos sockets versus dois listeners | Cardinalidade lógica correta |
| P01 | Versão/UTF8/NUL/overflow/duplicatas/ref type | Erro localizado, candidate inválido |
| K01 | Codec truncated/corrupt/unknown/historical | Rejeição estrita, sem reinterpretar BODY |
| E01 | AST/source/normalizado/scratch/coexistência | Censo/delta/reserva, sem dupla contagem |
| T01 | Core sem CNG/Win32 e alvo sem recurso | Compila core; unsupported nega, sem fallback |

Debug/Release zero warnings somente após patch funcional oficialmente aprovado. Falhas/Stop/retidos/CAS/slots e hooks de testes não podem publicar autoridade ou afetar runtime. Somente inspeções documentais nesta revisão. Certificados/CA/rede não fazem parte dos testes autorizados aqui.
