# Conformidade e limites da prova

| Contrato | Implementação/prova | Estado |
|---|---|---|
| E0/E1 tipos C89/revisão/assinaturas | trust.h, opacos sem factories | INTERFACE_IMPLEMENTED |
| E2 pacote/operação/SPKI/servidor | sem produtor factual; sem leitura de path ou dados como autoridade | DEPENDENT_DENY |
| E3 origem/CURRENT/R/A/P/I/pins | não fabricados; PDG DATA continua insuficiente | DEPENDENT_DENY |
| E4 identidade/issuer/propósito/validade/status/revogação | modelo de regressão existente apenas CONTROL_ONLY | POSITIVE_SOURCE_PENDING |
| E5 confirmação independente/destinatário | sem checkbox/token/senha ou root remota como confirmação | DEPENDENT_DENY |
| E6 ligação síncrona | negação sem output, ownership, I/O ou pendência | NEGATIVE_IMPLEMENTED |
| E6 acquire válido/evidências completas | sem construção de lease de modelo | NOT_IMPLEMENTED_NOT_TESTED |
| E7 INVALID | null precondition sem tentativa/efeito; nenhum reconhecimento de pointer | NEGATIVE_IMPLEMENTED |
| E7 borrow válido/release retido/query/liberação factual | depende do owner concreto; sem afirmar prova a partir de null | NOT_IMPLEMENTED_NOT_TESTED |
| M1 | shared core existente; adapter fixo chama a interface e nega TRUST | INTEGRATED_NEGATIVE |
| Stop/retention/KDF/plaintext | 28 checks novos ×4; regressão146 model+1189 crypto ×4 | PASS_OFFLINE |
| Compatibilidade | VC6 x86 /Za /W3 /WX Debug/Release; MSVC x64 /W4 /WX C90 Debug/Release | PASS_COMPILATION_NOT_NT4 |
| ED11 | sizeof op792 intacto; ponteiro transitório contabilizado, frames/backing desconhecidos | PARTIAL_ACTIVATION_DENY |

Regressões de propósito/status/revogação/CURRENT/pin/pacote/confirmação permanecem testes de controle sintético existentes, não exercício de um produtor factual. Crypto usa somente fixtures públicas existentes e assinatura de teste em memória previamente autorizada. Não geradas fixtures rejeitadas, pacotes operacionais, chaves, CA ou certificados. Não executada rede. Nenhuma prova NT4/box/ISA ou trust operacional.
