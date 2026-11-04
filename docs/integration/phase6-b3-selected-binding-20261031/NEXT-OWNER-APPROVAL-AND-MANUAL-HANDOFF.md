# Próxima etapa — autorização específica e handoff manual

Mesmo projeto/branch/owner. Seleção B3 já registrada; não pedir novamente os mesmos valores. B0–B8/D0–D7 accepted intactos. Ler CLOSEOUT.md e pedido integral ONE-SELECTED-STAGING-REQUEST.CANDIDATE.md, SHA256a9280bc14623dfdf23e4c67caf65b4baa58d0109c34afd348ae7223e5c173d16.

Este prompt NÃO autoriza operação. Sem aprovação explícita do proprietário ao pedido exato, manter PENDING/DENY, não criar registro de aprovação nem executar script/exe/coleta. O primeiro gate agora é essa autorização operacional, não outra ADR/seleção/investigação de custos.

Após aprovação válida: conferir bytes/pins finais, preservar pedido imutável, registrar ato literal em OWNER-STAGING-EXECUTION-AUTHORIZATION.json com decision AUTHORIZE_ONE_OWNER_MANUAL_SELECTED_STAGING, maximum_manual_invocations1, automatic_execution=false, request_sha256 e package_manifest_sha256 verificados. Calcular hash literal. Entregar ao proprietário handoff completo (hash do script88dfb7e8f0de05ca745aecc7fe39d6a9082532260fad2d917aa794c222e4502e ANTES da chamada, hash real do registro aprovado, Process policy salva/restaurada somente se aprovada no pedido). Não inventar hash/nonce/SID/PID/boot futuros. Nenhum build inalterado ou troca silenciosa de pacote; se houver divergência concreta, STOP.

NÃO executar automaticamente. Uma invocação manual pelo proprietário e nenhum retry. Ao receber saída, reconciliar aprovação usada versus consumed nativo, originais/pins, READY+5 e crescimento9094144, pisos físico6296461312/commit8479985664, unidade/R e2R/R, freshness500ms/duração120000ms, efeitos/status/hash/readback/primeira falha, close e retenção. Custo opaco permanece UNKNOWN; nenhum PASS produtivo por ensaio. Corrigir offline defeitos demonstrados sem repetir operação consumida ou gerar aprovação.

Sem SCM/Start/Stop, endpoint/rede/CA/CURRENT, TPM/NV/Q4, limpeza/refund/recuperação ou outros efeitos. Preservar HEAD/WIP/históricos/pins/UNKNOWN/chave fixture/reservas/PST0.6.4. Somente após cópia verificada e fechada preparar pedido independente de registro SCM parado.
