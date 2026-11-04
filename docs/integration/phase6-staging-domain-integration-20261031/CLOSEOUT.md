# Closeout — D0–D7 aceita e admissão própria integrada offline

Task PACC-PHASE6-STAGING-DOMAIN-ACCEPTANCE-AND-POLICY-INTEGRATION.
Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469; sem commit/reset/clean. WIP anterior preservado.

## Decisão

Registro normativo prévio ao código: ../../adr/DECISION-personal-staging-envelope-domain-d0-d7-20261031.md. Ato explícito do proprietário desta task; candidato integral/hash5cad769d78c7f731653022ca00c9e80f8263177393424793f0d745459e8ed71c conferido antes e depois. Original PROPOSED preservado. D0–D7 é aceito por incorporação, sem ampliar domínio ou autoridade operacional. Demais decisões accepted permanecem intactas.

## Implementação e callers

server_staging_resources_win32.inc acrescenta staging_policy_missing e staging_admit no mesmo owner. Recursos observados são DATA, não grants: diagnóstico mask=0 não aprova. O avaliador reconstrói pendências e conserva materialmente ausentes as cinco classes ainda sem fonte positiva qualificada: envelope nativo/transitivo, margem/política do host, retenção, atribuição administrativa e custo de metadados/efeitos FS. Observação/volume têm predicados negativos separados; dados inválidos ou incompletos acrescentam pendências. Preserva primeira falha, thread/invocação/ownership, Stop e estado de consumo.

A admissão própria não deriva um limite de handles dos números LAB/TPM/SCQ e não escolhe substituto. Retorna ERROR_NOT_SUPPORTED para a política material incompleta; mesmo eventual máscara vazia não torna DATA autoridade (fallback DENY). Nenhum setter, callback positivo, grant, formato persistido ou owner novo.

server_launcher_qualification_staging_win32.inc separa staging_apply_composition (composição de efeitos privada anteriormente exercitada por mocks) de staging_apply (gate). O único caminho do entrypoint público passa pelo gate antes de consumo/efeitos. A composição bruta só é chamada no código do produto pelo wrapper admitido; testes de componentes a exercitam explicitamente com APIs mutantes modeladas. Confirmação de presenter permanece distinta da admissão. O entrypoint público retorna DENY e fecha conforme governança; não interpreta readiness, snapshot ou nonce como capacidade.

IMPORTANTE: guards numéricos conservadores já existentes no caminho de observação/preparação não foram removidos. D1 proíbe removê-los antes de existir política suficiente. Eles não são a definição da nova política de admissão. LAB/TPM/SCQ não foram modificados. Esta integração não promete que uma futura coleta ultrapassaria o ponto132 nem emite nova coleta. Remover/trocar esses guards agora, com cinco classes de custo UNKNOWN, seria prematuro. Não há novo ceiling.

## Qualificação

Selected/Legacy Debug/Release: quatro builds PASS com /W4 /WX, sem diagnósticos MSVC de warning/error nos logs. Mesmo toolchain/targets existentes;17 CTests por configuração,68 execuções PASS. Logs preservados neste diretório. Native counters/known folders/RNG/IO/SCM mutante modelados nos testes; SHA256 e parsing SD/SID em memória reais. Sem coleta ou efeito operacional.

Staging:48 cenários/141 checks preservados para composição bruta e falhas parciais, replay/Stop/close/retenção. Recursos:31 casos/111 checks por configuração (antes30/99): acrescentada rejeição de máscara caller zerada, snapshot handles132 ou0, volume desconhecido, erro de observação, Stop, consumo anterior, thread divergente e estado fechado. Gate negado antes de creates/writes/consumo. Regressores existentes de registro/SCM modelado preservam os domínios separados; nenhuma função desses domínios foi alterada. Paths59 checks; close guard seis estados vivos/retidos. Não se reivindica prova real de token/volume/OS ou capacidade.

Layout gerido sem campos novos: Selected1.988.704, Legacy1.820.136 segundo censo; não é total privado/working set, limite de heap nativo ou garantia de margem física. Código das funções acrescenta caminho/stack a considerar em futura qualificação nativa; esse custo não é presumido zero.

## Artefatos e evidências

HASHES.json registra decisão, proposta, fontes, teste e executável novo. Selected/Release555.008bytes, SHA256fdc3c7f03aa6183cedfcb343699a674283260586472abb4eab2377b3a5c4f0f7. Não está selecionado/aprovado para operação. Bytes anteriores4a68... preservados em build/staging-observation-preserved-20261031/approved-3acf8867 e conferidos antes do rebuild; são cópias documentais, não identidade nativa original ou staging protegido. Nenhum pin operacional é substituído.

Reutilizados: packet7360/copy4096/hash1024/16slots e censos anteriores; snapshots109→132 e memória como históricos pontuais; codec/volume e falhas de retenção em modelo. Nada disso comprova espaço de disco atual, teto de pico ou capacidade positiva. Ambas coletas132 permanecem falhas e autorizações consumidas.

## Primeiro gate e caminho mínimo

Não há pedido de staging pronto. O primeiro gate de admissão é a fonte fundamentada do envelope nativo/transitivo e margem necessária para o efeito delimitado, não prova universal de origem de cada handle. Também continuam materialmente pendentes retenção/administrativo/FS e originais/proteção/volume não alcançados. A menor coleta somente de free space ou a repetição109→132 não supre essas classes nem justifica retirar o guard.

Próxima ação técnica: confrontar o envelope com os custos por fase já existentes e explicitar um modelo conservador de suficiência, inclusive custo opaco e falha parcial; se faltar evidência para um componente específico, preparar só a prova que o discrimine, com instrumentos/custos/autoridade próprios. Não inventar limite nem iniciar tracing. Não exigir futuro serviço para custos do staging.

Com política e predicados fundamentados, qualificar eventual alteração dos guards conservadores sob escopo próprio e então preparar pedido protegido byte-exato. Após staging aprovado/executado/reconciliado, readback da imagem original e autorização separada SCM_REGISTER_CLOSED possibilitam pedido de registro parado. Isso não autoriza Start/Stop, endpoint, sessão SCQ, KSP, CA/CURRENT ou NEW_INSTALL completo.

Staging DENY; E21 PENDING; ED11/Phase6 PARTIAL. Nenhuma operação protegida executada. Históricos/UNKNOWN/reservas/chave fixture e PST0.6.4 preservados.
