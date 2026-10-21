# Prova de implementação e limite do worker

Autoridade: DECISION-package-signer-delegation-persistence-20261020.md, anexos frozen F1–F5/G0–G7 e matriz byte-exata. HEAD c50b8fb9a7e3293b5e6355fc08459bf85b56857a, main. Registro normativo/formatos históricos não foram alterados.

## Implementado

config_root8_v3.c/.h implementa encode/decode/validate, selagem de SNT1/PDG1/CP3 e relação DATA com predecessor completo v2/v3. Header512, PDG header64+linhas384+digest32, CP3512 e domínios C3/Hcp3/W3 seguem G0–G7. Versão/endian/contagens/presença/offsets/ZERO tails/status/purpose/ordem/digests/referências e intervalos são explícitos. Corrupção, truncamento, trailing bytes, aliasing e valores desconhecidos negam sem publicação parcial.

SNT1/descriptor e validação BODY/LST1/carrier foram extraídos de v2 para duas rotinas internas compartilhadas, preservando regras e leitores históricos. Não foi criado parser paralelo. V3 não converte bytes para v2 nem remove PDG para caber; leitores antigos rejeitam v3. CMake usa somente o opt-in offline ROOT8 existente; o teste v3 depende também do opt-in worker offline. Não há alteração de gramática, API wire, CLI produtiva ou selectors ED11.

PDG representa identidade da policy/listener, signer, certificado/SPKI, issuer/geração/CA, finalidade PACKAGE_SIGNING, revisão de disposição, intervalo e APPROVED/REVOKED. D0 é ausência estrutural, nunca trust. Status da delegação e do certificado são separados. Registro existente deve ter purpose PACKAGE e certificado correspondente; hashes/key binding/issuer/intervalo/high-water são verificados com o codec issuer existente. O policy ID histórico da emissão não é reinterpretado como aprovação atual.

Validação pública completa do certificado/perfil/key-role é dependência obrigatória do componente DATA kind8; não foi inventado certificado ou trust adapter para preenchê-la. Adapter nativo histórico retorna UNSUPPORTED para kind8. Fixture de teste contém três bytes explicitamente artificiais como certificado e callback DATA de modelo; portanto não qualifica X509, delegação factual, CURRENT, assinatura ou importação.

## Funções e lifetime

| Função | Ownership e precondições | Resultado e limites |
|---|---|---|
| validate/encode/decode | Input completo imutável do caller, outputs/metadata disjuntos, capacidades checadas | Somente DATA; output completo apenas após validação; sem ponteiros escapando/allocação/owner novo |
| snt/pdg/cp_encode | Prefixo completo do caller; hash síncrono e buffers disjuntos | Selagem de fragmento, não aprovação da tupla ou autoridade de epoch; CP contém valores fornecidos, não inventados |
| base_link | Ambas imagens completas imutáveis e disjuntas | Valida CP/base C/W/R/A/P/I, LST base, anchor e histórico issuer/event-prefix/high-water; mudanças SNT/PDG retornam INCOMPLETE até prova de aprovação |
| rotinas compartilhadas | Chamador interno validou antes o header/ranges da versão | Reuso DATA de SNT/BODY/LST/carrier; não resolve referências operacionais |
| callbacks hash/component | Owner existente retém contexto e fontes; síncronos, não reentrantes, sem retenção de pointers | Ausência/erro/unsupported nega; não permite callback assíncrono com stack emprestada |

Nenhum buffer da root/worker, slot, pin, Session, fila ou crédito foi substituído/liberado. As funções não gerenciam I/O ou Stop, devendo ser chamadas fora do caminho bloqueante de Stop. Owner continua responsável por inputs/contextos/retidos e créditos; falha não é quiescência ou destruição. Callback incapaz de cumprir o contrato síncrono não pode ser conectado. Model E/exactly-once/ED08 continuam nos owners existentes.

## Limite factual do worker

O worker não foi modificado para escrever v3. Entrada existente ainda exige CURRENT/pin/predecessor/fontes factuais e não suporta essa composição. A capacidade selecionada permanece28960 por imagem (legacy16896), menor que o máximo v330592; não houve crescimento sem censo/reserva. A inexistência do ambiente CURRENT qualificado já foi confirmada pelo owner; nenhuma instalação foi inicializada para suprir o gate.

No novo teste, chamada à entrada real sem CURRENT/pin, com bytes v3 de modelo, é negada antes de iniciar worker; Stop também nega. Os56 testes nativos locais exercitam somente os caminhos históricos ROOT06/v1/CAS/Stop/retenção existentes, com contextos administrativos sintéticos explicitamente rotulados. Não são commit v3 nem CURRENT operacional.

O próximo slice requer fonte factual de histórico de aprovação/policy/issuer/TLS, CURRENT/pin/lifetime e censo da composição efetiva antes de qualquer resize/dispatch. Changed rows INCOMPLETE não pode ser convertido em aprovação por boolean. Nenhum fallback, segundo carrier, registry, worker ou root foi criado.

## Evidência

Selected/Legacy Debug/Release, MSVC19.51 x64/Ninja e /W4 /WX: zero warnings,14/14 suítes por configuração.43.934 asserções v3 DATA_MODEL por configuração; min/max N/B/D, D0, revogação representável, intervalo, propósito/status desconhecidos, issuer/identity divergentes, todas as mutações de byte e truncamentos, negativos semânticos após selagem/recomposição, CP3 A/P zero/versão/correlação inválidos, round-trip, aliasing, fontes ausentes e predecessor/stale/approval-history. Compatibilidade v1/v2/LST/issuer e perfis/créditos ED11 passaram.

Após a primeira rodada passou um refinamento somente de testes para exercitar header/CP3 sem depender apenas do hash externo. A rodada final completa passou; os resultados iniciais e bytes initial-staged preservam a proveniência dos hunks aprovados. Os56 casos nativos históricos passaram antes desse refinamento de testes, sem mudança de implementação posterior.

M1 e seus adaptadores criptográficos não foram alterados. Não se executou rede, CA/keygen/certificado real, assinatura privada produtiva, pacote operacional, publicação CURRENT ou admissão. Código fonte portável não inclui Win32/CNG; o harness utiliza adapter hash público Windows e fixtures, não qualifica box/ISA/NT4.

## ED11

Ver ed11-census.json, stack-evidence.json e saídas compiladas Selected/Legacy. Nenhum novo buffer global/owner da root/worker e nenhuma ampliação de quotas. Caller paga suas capacidades efetivas e coexistência, mesmo em encode/decode offline. Code/readonly image e stack transitiva não foram operacionalmente atribuídos; não são crédito grátis.

Imagem máxima30592, prefixo30560 e PDG1632 são limites serializados, não reserva. Potencial crescimento de três imagens do worker:4896, não aplicado/aprovado como reserva. Frames diretos observados por disassembly: máximo608 Debug/592 Release; não são stack transitiva completa. Chamadas aninhadas BODY/LST/issuer/hash/provider/callback e quaisquer source leases continuam a censar.

Base conhecida conservadora Selected2103847 e Legacy2067655 permanece referência de composição, não pico medido. Cenário anterior de pacote não compartilhado3594733 inclui provider de fixture e não é pico operacional nem justificativa para remover custos; ativação continua negada. Provider/kernel UNKNOWN; ED11 total PARTIAL. Windows3145728 somente explícito, default2097152, quotas individuais intactas.
