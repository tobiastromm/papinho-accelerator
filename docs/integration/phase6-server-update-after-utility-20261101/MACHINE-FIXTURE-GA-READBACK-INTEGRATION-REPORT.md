# Integração offline B/S0–S5 — 2026-11-03

Proposta aprovada e conferida byte-exatamente: SHA256 3b090974b4abbf5db2543e8ef223cebf78ec8a4b08581d9fa3d7e7beb9b53d47. Decisão registrada antes do código em [accepted S0–S5](../../adr/DECISION-personal-machine-fixture-ga-readback-s0-s5-20261103.md), SHA256 8eeb6da08d28c9f22d3df75cd840758db7e5f57ceda7db3f6f4db2e428ac1186. Nenhum accepted anterior foi editado; branch main e WIP preservados.

## Resultado técnico

O predicado diagnóstico do owner CA aceita somente GA×3 ou D01F01FF×3 uniformes, com todos os demais requisitos de owner/protected/ACEs/SIDs/ordem/flags. Usa a máscara do DENY original já acumulada para exigir coerência das duas ACEs seguintes, sem novo campo/buffer. Setter GA, bytes lidos, primeiro erro, controle positivo, Stop, lifetime, close e retenção permanecem. A mesma validação atende somente os construtores diagnósticos existentes de fixture e READ admitida; o ramo CA comum permanece inalterado. CA comum já aceitava ambas as formas sob sua política anterior: o teste confirma preservação, não uma nova política ou isolamento inferido.

Plano KFX1 incorpora o digest aprovado S0–S5 no preimage original existente; apresentação explicita os dois templates/setter e risco. Nenhum wire, endpoint, serviço, owner, protocolo ou autoridade nova. Sem alteração de quotas, memória, ED11, freshness ou deadlines.

## Qualificação

Quatro builds Selected/Legacy Debug/Release PASS, zero warnings; 15 testes por configuração, 60 pares PASS. Fixture: 370 checks, incluindo 81 casos novos de templates/misturas/64 mutações de bits/estrutura; setter inspecionado GA e controle público da mesma chave retida. Source: 37 casos/147 checks por forma. READ: 27 casos/1474 checks e quatro closes positivos por forma; formas executadas em processos de teste independentes, sem repetir ServiceMain ou resetar referências para simular close. Demais regressões verificam runtime/probe/autoridade/perfis/deadlines/recursos/retenção. APIs mutantes/mocks existentes; nenhum KSP/SCM/endpoint real.

Primeiro build Debug encontrou warning C4456 no teste novo; variável local renomeada, log inicial preservado. Campanha final sem warnings. [Manifesto](MACHINE-FIXTURE-GA-READBACK-OFFLINE-QUALIFICATION.json), SHA256 bc078ed46866d5b1518b2f542887d836f058e74a68babfcc26f93dec3bff4860, contém hashes de fontes e oito logs finais. [Verificação do pacote](MACHINE-FIXTURE-GA-READBACK-PACKAGE-CHECK.json): AST PASS, helpers preservados, mesmos 16 slots de argumentos, não invocado. Não repetir campanhas anteriores inalteradas.

## Imagens e estado operacional

Candidato Selected/Release: **872960 bytes**, SHA256 **f2aa63a82e92137be15438d60dae3b4f0332330b9b8df9a51617211e060c772d**, congelado em papacc_server.fixture-ga-readback-872960.exe. Delta +1024 bytes ante a imagem histórica instalada de 871936 bytes/ID417614100:10223616:73344/SHA256 a6b8ee1ac5ef3e7b6908686f3690ce2a6ca6c4171812a201b7d3024ea6f2ab8a. Não houve leitura operacional nova nem instalação. DLLs/updater/configuração SCM não foram alterados. STOPPED histórico não é lease atual: gate nativo fresco obrigatório.

Chave parcial dea8648758bebcbbcabde016bf911e77.b4f8ff6a41ba3b4d3e5ff403d0d017a7 e todos os originais preservados; não adotados, qualificados, apagados ou reexecutados. Observação atual demonstrou incompatibilidade da projeção antiga com a máscara atual, sem provar retrospectivamente o ramo exato da falha original. Autorização de observação utilizada; histórico/application-close/reservas/retenção/quiescência OS continuam UNKNOWN. Produção DENY, PST0.6.4 preservada.

## Primeiro gate e caminho mínimo

[Pedido UPDATE](ONE-MACHINE-FIXTURE-GA-READBACK-IMAGE-UPDATE-REQUEST.CANDIDATE.md), SHA256 **27253c300c6ee8a7c27c5a3ba9ac36b6ba9b8189fde15f2692feb13598f6448c**, pronto para decisão operacional específica. Ainda NOT_AUTHORIZED/NOT_EXECUTED. Candidato é DATA; executor protegido independente U2 não é substituído. Não há decisão operacional fabricada nem comando com placeholders.

Após aprovação, registrar decisão real e entregar comando manual do wrapper existente adaptado. Após UPDATE reconciliado, preparar nova operação fixture com decisão nova e cópia auxiliar exclusiva em leaf novo F3/R4, preservando chave parcial. Somente após nova fixture/controle positivo/identidade qualificados preparar READ e atos independentes E/Q/S/P. Bilateral/probe/receipt/close/RUNNING continuam provas futuras; sem readiness produtiva. [Próximo prompt](NEXT-MACHINE-FIXTURE-GA-READBACK-IMAGE-UPDATE-AUTHORIZATION.md).
