from pathlib import Path
import json,hashlib,csv,shutil,subprocess
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/q7-scm-readback-correction-20261025';old=p.parent/'q7-source-image-pin-correction-20261025';h=lambda f:hashlib.sha256(f.read_bytes()).hexdigest();a=json.loads((p/'FINAL-ARTIFACTS.json').read_text());reqsha=a['request_sha256'];bindsha=a['binding_sha256'];rows=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  exe=r/'build/q7-scm-readback-correction-20261025'/profile/cfg/'papacc_scm_instrumented_lab.exe';q=subprocess.run(['python',str(p/'verify-image-pin-in-binary.py'),'--qualifier',str(exe),'--image',a['artifacts']['service_image']['path'],'--pin-header',str(r/'tools/qualification/scm-q4-image-pin.h'),'--forbid','2386d63a256affd48c3dfbcd78ff36380400c86f61a0eeb68e209d6ed4520821'],capture_output=True,text=True);assert q.returncode==0;v=json.loads(q.stdout);b=exe.read_bytes();assert 'papacc_server_lab_host_ntstatus_0b1de869e11493b7cb08fb329578f6ce.exe'.encode('utf-16le') in b;assert 'EXACT-Q7-SCM-CORRECTION-APPROVED.json'.encode('utf-16le') in b
  q=subprocess.run([str(exe),'--q4-offline-invalid-probe'],capture_output=True,text=True,timeout=20);assert q.returncode==2 and 'PRIVATE_STDOUT_BOOTSTRAP_DENY' in q.stderr;v.update(profile=profile,config=cfg,historical_predecessor_embedded=True,new_approval_path_embedded=True,invalid_probe_exit=2);rows.append(v)
(p/'compiled-pins-and-entry-checks.json').write_text(json.dumps(rows,indent=2)+'\n')
for item in json.loads((old/'CORRECTED-ARTIFACTS.json').read_text())['artifacts'].values():
 if item['path'].endswith('.ps1'):continue
 assert h(Path(item['path']))==item['sha256']
assert h(old/'EXACT-Q7-CORRECTION-APPROVED.json')=='51ffb049a926d6725a688724e5998ea0118dc4dbd4d95f9dc7664ea12022bd85';assert h(p/'preimages/scm-n0-protected-single-powershell.ps1')=='e90e033d483d9431b89eb4e42f712f4e7a5154b7909ea204732ea6912927d78b';assert not (p/'EXACT-Q7-SCM-CORRECTION-APPROVED.json').exists()
report=f'''# Q7 SCM_READBACK — reconciliação e correção consolidada

## Resultado

A execução manual corretiva N0TOKEN-20261025-01 /10584f02e11f92c92b43cddf0d9d316f avançou além de SOURCE_IMAGE e falhou em SCM_READBACK, first13/ERROR_INVALID_DATA, close0, starts0/stops0, intent_consumed0/effect0/diagnostic_created0, quiescent0 e exit2. Transporte local:fechamento confirmado, first0/retained0. Original comunicado preservado em OWNER-MANUAL-SCM-READBACK-RESULT-AS-REPORTED.txt; FACTUAL-RECONCILIATION.json registra o alcance. Aprovação51ffb049a926d6725a688724e5998ea0118dc4dbd4d95f9dc7664ea12022bd85 foi USADA e não pode ser reutilizada. A anterior c07384... também permanece usada. Nenhum registro aprovado histórico foi sobrescrito.

Não houve consume/criação de imagem/sink/rebind/Start/Stop por esta execução segundo o recibo. Isso não prova ausência global dos nomes nem extingue obrigações históricas. O fechamento local não demonstra quiescência global; UNKNOWN e reservas reais continuam retidos. O diagnóstico genérico pid0/role4 não é prova de quinto processo. Não foram produzidos token de serviço, máscaras do censo ou métricas ED11; 128/1024 não foram observados/concedidos nesta saída. A identidade protegida RAW1 contemporânea nativa também não foi alcançada no qualificador.

## Divergência comprovada

Consultas posteriores estritamente read-only sc.exe qc/queryex/qfailure/qtriggerinfo/qsidtype foram preservadas em arquivos .raw e SCM-READONLY-OBSERVATIONS.json, todos exit0. São observações posteriores, não métricas frescas do ciclo manual. QC mostra serviço existente own-process/DEMAND/NORMAL/conta virtual e ImagePath histórico preservado:

`"C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\papacc_server_lab_host_ntstatus_0b1de869e11493b7cb08fb329578f6ce.exe" service host`

QueryEx mostra STOPPED/PID0/Win325, sem provar quiescência global/histórica. QSIDTYPE mostra UNRESTRICTED; configuração de SID do serviço não é prova de atributos ENABLED no token. Consultas não executam Start/Stop nem mutação SCM/ACL.

O qualificador inicializava native.command com o nome genérico papacc_server_lab_host_ntstatus.exe; cycle_readback(candidate=0) o comparava literalmente via ps_virtual_readback_start. Esse nome diverge do efeito histórico que Q2/Q4 exigem preservar. A integração anterior omitiu esse pin de estado prévio. A divergência textual é suficiente para ERROR_INVALID_DATA; o recibo não imprime qual subpredicado retornou primeiro, portanto não se afirma exclusividade nem ausência de outras divergências não observadas.

## Correção técnica

Apenas no perfil Q4, cycle_readback(candidate=0) agora constrói temporariamente o comando esperado a partir do caminho03 fixo e historicamente corroborado; não copia/adota o texto SCM consultado e não executa a imagem antiga como nova. Após rebind, candidate=1 continua exigindo exatamente cycle_path da identidade10584.... Os ramos padrão/legados continuam usando o comportamento anterior. Toda chamada conserva ps_virtual_readback_start e suas verificações de tipo, startup, conta, dependências, SID type/resolução e configuração; o error control continua exigido. Nenhuma wildcard ou tolerância nova de identidade.

O buffer native.command é restaurado após sucesso, falha de consulta ou falha de composição. O buffer salvo1050WCHAR já existia; foi acrescentado um ponteiro escalar local, não se presume custo de stack/memória zero. Nenhum handle/owner/mutex/logger/serviço/IPC foi acrescentado; Stop, A1, B1, self-token, RAW1, UNKNOWN e quotas continuam. Não houve restauração de ImagePath nem alteração dos arquivos históricos.

A seleção do qualificador e a superfície nativa agora usam o novo caminho de aprovação EXACT-Q7-SCM-CORRECTION-APPROVED.json, ainda AUSENTE. Isso impede reutilizar o registro51ff...; o ramo padrão continua EMPTY_DENY. A nova identidade operacional não foi gerada: os candidatos apenas descrevem o binding técnico da identidade ainda sem intent consumido, sujeito a decisão explícita após falha.

## Qualificação

Quatro configurações de qualificador Selected/Legacy Debug/Release PASS, zero warnings, /WX e toolchain preservados. Build dirs novos; imagem congelada12883468... e DLL/framework anteriores não foram relinkados/substituídos. A dependência explícita do pin e a atestação compilada introduzidas na correção anterior continuam ativas.

A função real cycle_readback foi extraída da fonte atual e compilada em oito fixtures:Q4 e ramo padrão nas quatro configurações.17 casos por execução,136 resultados PASS. Exercita estado prévio, nova imagem pós-rebind, divergência, erro de consulta propagado, startup/error control, falha em cada append e restauração integral do buffer. SCM foi simulado; nenhum desses resultados é consulta/mutação LAB, prova de token ou de recursos. Não elimina as verificações do adapter real.

Quatro checks dos PE finais:pin ASCII/UTF16 da imagem correto, caminho histórico e caminho novo da aprovação incorporados. Quatro probes de argv inválido:exit2 antes de transporte/preflight. PowerShell5.1:31 gates do caminho novo,27 históricos e3 de pins/aprovação em memória:61 PASS. Registro hipotético existiu apenas em memória de fixture; o arquivo novo de aprovação não existe. Não foi chamado launcher operacional, Prepare ou Cycle pelo agente. Evidências anteriores de transporte/console/token/ownership e76regressões permanecem no alcance original; não foram repetidas sem mudança relevante. O fixture de stack-VA que exibiu crash não foi executado.

## Hashes finais

| Artefato | SHA256 |
|---|---|
| Launcher candidato | `{a['artifacts']['launcher']['sha256']}` |
| Qualificador Selected Release candidato | `{a['artifacts']['qualifier']['sha256']}` |
| Imagem congelada preservada | `{a['artifacts']['service_image']['sha256']}` |
| Pedido SCM corrigido candidato | `{reqsha}` |
| Binding corrigido candidato | `{bindsha}` |

FINAL-ARTIFACTS.json, APPLIED-SOURCE.patch, preimages e FINAL-HASHES.json preservam a alteração focal e evidências. Binários anteriores, imagem, approvals e preimagem do launcher foram conferidos por hash. HEAD/main913c90cf2328f7939d8f5b927640601903ac6e91 e demais WIP preservados no alcance dessas verificações. Nenhum ADR accepted foi editado, nenhum commit/push/tag/clone/worktree, quota/crédito/refund ou limpeza.

## Próximo gate

Q7-SCM-CORRECTED-REQUEST.DRAFT.json vincula explicitamente o ImagePath prévio exato e a nova imagem pós-rebind, os artefatos atuais, inputs protegidos originais/N7/políticas e os dois resultados anteriores. Nenhum comando positivo executável ou aprovação foi criado. Uma nova invocação exige decisão específica do proprietário após esta falha, sem herdar retry dos pedidos antigos. Se o escopo requerer outra identidade/decisão normativa, isso deve ser escolhido explicitamente antes de novos objetos/vínculos, não inferido pelo agente.

Qualquer execução futura separadamente autorizada continua condicionada a revalidação pelos handles originais, SID/RAW1/conteúdo, operador/token/host/boot, exclusividade, quatro papéis,128/1024, freshness60s/1s, headroom e obrigações compatíveis. No máximo o consume/imagem/sink/arm/rebind/Start e Stop seguro expressamente aprovados; nenhuma garantia de RUNNING ou orçamento por evidência offline. Novos PIDs/token IDs/nativeIDs de efeitos e métricas permanecem não preenchidos.

O token histórico03 e seus atributos permanecem UNKNOWN. ED11 ainda exige a observação independente e recursos/lifecycle/Stop/fechamento no alcance factual. Phase6 mantém seus demais gates. Q2 continua documental, P3 não é aplicado pós-Start, N7 não autentica integralmente console/mapped image, e as obrigações históricas UNKNOWN/reservas reais não são liberadas.

Q7_RESULT=DENIED_SCM_READBACK_BEFORE_EFFECTS
FIRST_ERROR=13
CLOSE=0_REPORTED
INTENT_CONSUMED=NO
STARTS=0
STOPS=0
EFFECTS=0
PRIOR_APPROVAL=USED_NONREPLAYABLE
SOURCE_IMAGE_GATE=PASSED_IN_REPORTED_RUN
SCM_PREDECESSOR_CORRECTION=QUALIFIED_OFFLINE
SERVICE_TOKEN=NOT_OBSERVED
CENSUS_MASKS=NOT_OBSERVED
INDEPENDENT_BUDGET=NOT_OBSERVED_NOT_GRANTED
GLOBAL_QUIESCENCE=UNPROVEN
NEW_OPERATION_EXECUTED=NO
SCM_MUTATIONS=NONE
ACL_MUTATIONS=NONE
HOST_CREDITS=NONE_GRANTED
ED11_TOTAL_STATUS=PARTIAL
PHASE6=PARTIAL
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
NEXT=OWNER_EXACT_POSTFAILURE_SCM_BINDING_DECISION
'''
(p/'CONSOLIDATED-RECONCILIATION-AND-SCM-CORRECTION.md').write_text(report,encoding='utf-8')
(p/'NEXT-OWNER-DECISION.md').write_text(f'''# Manual operation not authorized

New candidate request SHA256 `{reqsha}` binds the correction, exact predecessor command and artifacts. Approval51ff... was USED for SCM_READBACK/13 with consume0/effects0/starts0. No automatic retry follows. A specific owner decision must accept the new exact binding and decide on a single further manual invocation; none is registered. EXACT-Q7-SCM-CORRECTION-APPROVED.json is absent. Do not run either previous command. No positive executable command is issued.

After valid approval, register its exact decision/one-run manual record without rewriting history, hash it and prepare the literal command using current launcher SHA `{a['artifacts']['launcher']['sha256']}` and the new approval digest. Then the owner alone may execute once, subject to the complete fresh native gates and original runtime confirmations. No separate Prepare, automatic execution, quotas/credits, cleanup/rollback/retry or productive authorities. Retain partial/UNKNOWN results and return full original output. ED11/Phase6 PARTIAL; budget not granted; token not observed.
''',encoding='utf-8')
note=f'\n\n## Q7 SCM_READBACK result and exact predecessor fix - 2026-10-25\n\nSee docs/integration/n0start03-integrated-closeout-20261024/q7-scm-readback-correction-20261025/CONSOLIDATED-RECONCILIATION-AND-SCM-CORRECTION.md. Manual correction passed SOURCE_IMAGE then denied SCM_READBACK/13, close0, consume0/effects0/starts0/stops0; transport closed. Approval51ff... used/nonreplayable. Read-only SCM confirms preserved historical03 ImagePath; code still expected generic image. Q4-only exact predecessor/new-candidate readback fixed without SCM/ACL mutation or identity relaxation. Four qualifier configurations zero warnings,136 actual-function simulated results,61 PS gates and4 compiled-pin/invalid-entry checks PASS. New request candidate {reqsha}; no new approval or operation. Historical UNKNOWN/reservations/global-quiescence limits retained; token/census/budget unobserved. ED11/Phase6 PARTIAL and productive authorities DENY. Next: specific owner post-failure exact binding decision.\n'
for f in [r/'docs/capabilities/canonical-phase6-consolidation.md',p.parent/'INTEGRATED-CLOSEOUT.md',p.parent/'ED11_GATE_MATRIX.csv']:
 d=p/'live-preimages'/f.name;d.parent.mkdir(exist_ok=True);shutil.copyfile(f,d)
 if f.suffix!='.csv':
  with f.open('a',encoding='utf-8') as out:out.write(note)
 else:
  with f.open('a',encoding='utf-8',newline='') as out:csv.writer(out).writerow(['Q7_SCM_READBACK_RESULT','Exact preserved predecessor configuration and independent token observation','DENIED_PRE_EFFECT_SCM_CORRECTION_OFFLINE','q7-scm-readback-correction-20261025/CONSOLIDATED-RECONCILIATION-AND-SCM-CORRECTION.md; first13 close0 consume0 effects0 starts0; actual-function and exact predecessor tests PASS','Token census and fresh budgets not observed; approval51ff used; global quiescence unproven','Owner exact post-failure scope/binding decision then only separately approved manual observation','YES new specific approval required; no retry inferred','Identity configuration RAW1 freshness budget or ownership uncertainty => DENY retain no retry','PENDING_OWNER_POSTFAILURE_SCM_DECISION'])
assert subprocess.check_output(['git','rev-parse','HEAD'],text=True).strip()=='913c90cf2328f7939d8f5b927640601903ac6e91'
print('CONSOLIDATED_RECONCILIATION_AND_LIVE_MATRIX_WRITTEN; NO_NEW_OPERATION')
