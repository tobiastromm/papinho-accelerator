---
status: accepted
revision: 1
decision-date: 2026-10-22
scope-level: project
scope-target: PapinhoAccelerator independent disposable CA LAB non-overwrite experiment
decision-makers: [Tobias Tromm]
supersedes: []
---
# Decisão — gate CA LAB sem overwrite G1–G5

## Autoridade e alcance

Task PACC-PHASE6-CA-LAB-NONOVERWRITE-GATE-ACCEPTANCE-1. O owner autorizou aceitação condicional após revisão integral e aprovação oficial. Este registro vigora após aprovação oficial desta escrita exata e verificação dos hashes. Proposta original preservada: [G1–G5 PROPOSED](../integration/ca-lab-atomic-key-exclusivity-20261022/LAB-NONOVERWRITE-GATE-PROPOSED.md), SHA-256 `a38f42437cfefb2268fec9fe6dfe32f18962304de303c8c0e750607799200d6e`. Corpo G1–G5 abaixo incorporado byte-exato, SHA-256 `05f4d542a9e65d2b300bb4d16bd96f6dc1485cedf5aa83e41a851c0137e6999e`. Status/tarefa propostos do cabeçalho original não são promovidos.

Aceita-se SOMENTE o gate para UMA experiência LAB descartável independente, na identidade exata G1. Substitui a exigência procedimental anterior de prova prévia absoluta de ausência por resultado nativo da criação sem overwrite. Isto NÃO declara a identidade ausente antes da chamada. O sucesso de Create não confirma Finalize, persistência completa, proteção ou autoridade. NTE_EXISTS e toda outra falha implicam STOP. Não se afirma atomicidade de toda a sequência.

A revisão resolve a condição de escopo de G5: esta experiência não substitui/reconstrói a CA anterior, não dá continuidade à inicialização incerta anterior, não concede CA produtiva e não apaga sua dívida UNKNOWN. Assim não se aplica a proibição de tentar outra identidade PARA CONTORNAR a incerteza como autorização de recuperação: issuer/custódia cláusula6 e bootstrap P8 permanecem integrais. Se o uso real buscar substituição/recovery ou outro identificador, está fora desta decisão e deve negar. Não há exceção genérica àquelas cláusulas.

As expressões condicionais/futuras históricas em G2/G5 passam a ter somente esta aceitação normativa delimitada, sem promover patches/execução. P1/P3, F0–F9, custódia CA, bootstrap/CURRENT, Model E/ED08 e ED11 não são alterados ou superseded. Não se infere capacidade, qualificação concorrente/fencing, target ou autoridade. Núcleo portável, owners, root/worker/pins e quotas inalterados; Windows KSP é escopo LAB desta norma, não dependência universal.

## Lifetime e evidência exigidos para a implementação futura

Estes são estados observacionais da operação, não novos enums/ABI/registry/owner:

- BEFORE_CREATE: identidade preparada e tentativa ainda sem resultado nativo; nenhuma ausência presumida. Provider adquirido, se houver, tem ownership e fechamento separados.
- CREATE_CONFIRMED: retorno ERROR_SUCCESS, handle recebido sob o mesmo owner e created observado. Ainda sem permissão de uso/finalização presumida. Colisão ou qualquer falha interrompe antes das próximas etapas.
- PROPERTIES_APPLIED: cada propriedade exigida registrada como sucesso no mesmo handle. Falha em qualquer setter impede Finalize e mantém disposição persistente não demonstrada.
- FINALIZE_CONFIRMED: retorno ERROR_SUCCESS de Finalize, distinto de public validation/READY. Não transforma chave LAB em autoridade.
- FINALIZE_FAILED: tentativa observada, erro original preservado, sem confirmação de finalização; novos efeitos negados, sem retry. Não presumir ausência ou rollback.
- CLEANUP_CONFIRMED: somente a liberação factual dos recursos cujas APIs retornaram sucesso. FreeObject confirma fechamento do handle, nunca exclusão persistente. Se uma exclusão LAB vier a ser especificamente autorizada, sua confirmação deve ser distinta e consome o handle conforme a API; nunca free duplo.
- CLEANUP_UNCERTAIN: manter referências ainda possuídas, ownership/borrows/credit e evidências no owner existente até quiescência/liberação factual. Nenhum descarte por timeout, erro ou encerramento de processo. Falha de observabilidade não vira liberação presumida.
- PERSISTED_UNKNOWN: resultado ortogonal aos handles; pode coexistir com CLEANUP_CONFIRMED. Retenção persistente anterior e eventual nova retenção são charges independentes. UNKNOWN não rearma a tentativa, autoriza outra identidade ou permite exclusão automática.

Stop/cancelamento/restart invalidam novos efeitos conforme contratos; não provam cancelamento da operação nativa ou rollback. Erros e resultados intermediários não são sobrescritos por um código final de launcher. Históricos/recibos não restauram autorização.

## Limites

IMPLEMENTATION_AUTHORIZED=NO
NEW_LAB_EXECUTION=NOT_RUN
PREVIOUS_LAB_KEY_DISPOSITION=UNKNOWN
CA_CUSTODY_INITIALIZATION=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
PRODUCTION_PIN=0.6.4

Aceitação não modifica o launcher, não autoriza geração, propriedade nativa, finalize, delete, rede operacional, certificado ou bootstrap. A menor atualização do launcher é apenas preparada em documento separado para futura implementação/qualificação com revisão e execução manual específica. Provider/kernel continuam UNKNOWN, não zero.

## Corpo G1–G5 incorporado

G1. Restrict the experiment to the already prepared distinct LAB name PapinhoAccelerator.CA.2345e07adf714045a20d1c8b6fdf5aaf.a2bcd6d2ed3043d083e7e50342d6fab0, Microsoft Software KSP/MACHINE. Before execution its state is NOT_OBSERVED, never ABSENT by assumption. The old name PapinhoAccelerator.CA.cb87632463bb48dd8a34fa8ea6f3911b.030fa34dd44f41e8b143b9da09532cf2 stays UNKNOWN and untouched. This independent experiment is not replacement/recovery of it. Any evidence that the new name was attempted/retained stops this first-attempt operation.

G2. Subject to official review and later manual owner execution, replace only the prior LAB absolute pre-query absence requirement with the documented provider non-overwrite barrier. Use the existing call NCryptCreatePersistedKey(provider,&owner.key,NCRYPT_RSA_ALGORITHM,exact_new_name,0,NCRYPT_MACHINE_KEY_FLAG), never overwrite. OpenKey/BAD_KEYSET is not authorization or absence proof. NTE_EXISTS is collision/STOP. Every other failure/uncertain return also stops without retry, existing-key open, deletion, alternate name/provider/scope or fallback. Record the returned status and owner state before the next action. Success confirms only this native create step, not global history/authority.

G3. Only confirmed success allows required properties and Finalize on that same owned handle. Setters, Finalize and public validation must each succeed before READY. A later collision or unexpected concurrent provider behavior is STOP. No claim is made that create/set/finalize/persist is atomic, that a pending name is reserved against every racer, or that failure rolls back persistence. Stronger concurrency/fencing remains unqualified.

G4. Preserve existing owner/borrows/Stop/first-error/quiescence. FreeObject does not prove persistent deletion. Unknown effects/cleanup retain evidence, references where possible and resource charges; no timeout/free, double-free, restart rearm, new identity or automatic retry. Prior UNKNOWN object and any new uncertainty are charged independently. No automatic deletion of uncertain objects. Cleanup of a confirmed new LAB key needs the later exact approval and cannot touch the old or colliding object.

G5. Preserve operator/UAC/real console, fixed hashes, exact plan confirmation, one consumed attempt, no private export/certificate/signature/root/CURRENT/network and ED11 PARTIAL. This would specialize only the LAB pre-query gate, not productive bootstrap P1/P3/P8 or issuer clause6. If official review finds those clauses prohibit this separate experiment, retain STOP pending an explicit designated normative exception. No accepted contract is changed by inference. Future launcher hunks must remove the mandatory absence-preflight dependency, consume immediately before one submission, record create/finalize/cleanup separately, and require manual owner execution. No such hunk is applied now.
