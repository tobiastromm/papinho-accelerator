# Próximo slice mínimo — gerador canônico de pacote manual

NEXT=IMPLEMENT_CANONICAL_MANUAL_CLIENT_PACKAGE_GENERATOR.
Escopo: P6-PROVISIONING-MANUAL-PACKAGE. Plano para a próxima autorização;
nenhum código, emissão, importação de material ou PACC wire autorizado aqui.

1. Antes do código, submeter somente D3/D4 e superfície mínima D5 da
   [matriz](RECONCILIATION.md): schema versionado, autenticidade/verificador,
   destinatário/servidor/request/CSR de origem, bounds exatos/validade/replay/
   duplicates, issuer e custódia aprovados. Delimitar dados públicos/segredos e
   sua proteção. Sem essas decisões, parar antes do parser/serializer ou key
   generation. Não selecionar ZIP/JSON/PKCS12/algoritmo/password por conveniência.
2. Reusar admin/cadastro/issuer aprovados. Binding inexistente é blocker factual;
   implementar somente o autorizado na mesma root. Sem outro cadastro/CA/listener.
   Cliente apresenta public key/CSR e demonstra posse conforme contrato; chave
   normal não sai do cliente. Issuer selecionado fornece credencial; servidor
   mantém Principal estável. Cert/fingerprint não concede grants.
3. Gerar pacote de vista administrativa factual e resultado de issuer aprovado,
   com integridade/destinatário/servidor/limites validados. Export não equivale a
   admissão nem commit Browser. Não logar chave/password/token/proof bruto.
   Falha de emissão/commit/export não deixa autorização parcial nem admite retry
   destrutivo; recovery/anti-replay exigem contrato aprovado.
4. A root atual oferece primeiro commit dormente: apply_dormant ->
   transaction_prepare_store -> transaction_commit -> transaction_publish ->
   current_pin. Rejeita live reload. Não fingir que root ativa está dormente.
   Se add cliente exigir live update, fazer o menor binding hot-update autorizado
   na mesma autoridade antes de alegar produto sem restart. Não ampliar
   automaticamente este slice para todo enrollment/revoke. Não editar committed
   direto nem escolher epoch no gerador.
5. Entregar contrato/exemplo público sintético ao Codex Browser pelo
   [handoff](BROWSER-HANDOFF.md). Implementação Browser tem autorização própria;
   reutilizar validator/store/generation/leases e manter granular. Não criar
   segunda biblioteca de credenciais.
6. Sob autorização específica, validar offline: admin/issuer/recipient corretos,
   unauthorized/substituted CSR/PoP, wrong server/recipient, tamper, expiry/replay/
   duplicate, oversize/malformed/traversal e ausência de secret em logs. Import
   deve manter committed em cancel/failure, validar material e publicar
   atomicamente. Exercitar stale writer/crash/leases/Stop/retenção e reportar
   sintético/nativo separadamente; fixture não qualifica produto operacional.

Contabilizar custos incrementais de schema/staging/cópias/scratch/registro/replay,
simultaneidade/retention no ED11 antes dos efeitos. Sem aumentar quotas ou importar
margem histórica; provider/kernel UNKNOWN permanece UNKNOWN. Nenhum custo runtime
foi introduzido nesta tarefa documental, sem medição nova.

DoD próximo: gerador real ligado à autoridade/issuer selecionados, contrato
consumível e negativos; import comprovado na tarefa Browser correspondente.
Não fechar gate por export isolado/fixture, nem fechar as outras cinco entregas.
Phase6 integral/publicação065 permanecem bloqueadas por obrigações abertas.
CLIENT_ADMISSION=DISABLED; SITE_TLS_ADMISSION=DENY; ED11_ADMISSION=DENY.
