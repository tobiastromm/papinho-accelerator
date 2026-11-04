# PapinhoAccelerator + PapinhoBrowser — execução técnica do plano recuperado

TASK: PACC-PHASE6-PRODUCT-CLOSURE-OFFLINE-EXECUTION

## Objetivo

Executar o primeiro pacote técnico sem efeitos operacionais do roadmap recuperado, sem retomar a cadeia TPM/root por default.

Leia docs/capabilities/phase6-scope-and-closure-20261028.md e REQUIREMENTS.csv deste diretório. Preserve active refs/WIP e todas as evidências qualificadas. Não reabrir E5/E11, Model E ou C0 só porque um checkpoint antigo os marcava pendentes.

## Entrega técnica integrada

1. Execute verify_scope.py sem gerar efeitos fora do pacote documental. Reconcile referências alteradas com os hashes da primeira execução; divergência não é licença de substituir pin.
2. Complete um mapa de callers atuais nos dois projetos: C8/commit/CURRENT/epoch/SiteTLS; C2/C3/Browser receipts; MANUAL/AUTOMATIC/shared provisioning; NORMAL M3/RECOVERY; revogação M7; configuração/listeners. Para cada lacuna da matriz identifique função, campo, autoridade vigente e evidência de execução. Não usar root de fixture como dado real.
3. Resolva divergências concretas de documentação e, no escopo técnico já autorizado, de mecanismos offline. Não implementar nova semântica de autorização/consumo, política quantitativa ou formato ainda proposed. Antes de build leia as receitas do alvo; somente repetir testes afetados por uma mudança real. Preserve MSVC/x64 OpenSSL e VC6/x86 NSS, Selected/Legacy/API2/API3/R2.
4. Para E19/E20, produzir anexo D4/D5 completo quanto ao que é fundamentável: origem de cada limite/medida, unidade, multiplicidade, baseline/total/incremento, idade/duração/headroom, módulos/host/boot e invalidação. Parâmetro sem fundamento continua UNKNOWN; registrar a menor escolha realmente normativa. D2 preparation-only não concede diagnóstico com efeitos.
5. Para provisão, emitir checklist executável de jornadas manual/automatic e revogação/restart, com funções existentes e provas ainda ausentes; para Browser incluir certificados/GUI/Mode D/Legacy/NT4. Não copiar o checkpoint API3 inicial como estado atual quando closeout posterior o superou.
6. Produzir um único patch/reconciliação e plano de observações futuras, com dependências e condições de Stop. Não produzir aprovação nem comando positivo enquanto faltarem bindings/autoridade. Não avançar TPM como pré-requisito universal. Q4 histórica permanece preservada e sem liberação de reserva.

## Limites

Nenhum TPM/NV, Q4, ownerAuth, rede, LAB, Start/Stop, intent, SCM/ACL, cleanup/refund, bootstrap, CURRENT, assinatura ou recuperação autoritativa. Nenhuma publicação PST0.6.5 ou mudança de produção0.6.4. Browser fora da raiz de escrita atual requer o mecanismo de permissão correspondente para edição; leitura permanece permitida. Sem novos serviços/owners/IPC genérico ou checkout.

## Critério de conclusão

Mapa com evidência por caller, correções offline justificadas, quantitativos fundamentados ou decisão exata pendente, jornadas verificáveis e pacote operacional futuro somente onde completo. Não encerrar com outra auditoria genérica ou abrir microtarefas; continuar correções técnicas cobertas até a fronteira normativa/operacional real. O resultado não é ED11/Phase6 PASS por documentação.
