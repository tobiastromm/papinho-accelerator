from pathlib import Path
import json,hashlib,csv,subprocess,re
root=Path.cwd();base=Path('docs/integration/n0start03-integrated-closeout-20261024');p=base/'real-console-role-binding-20261027';sha=lambda q:hashlib.sha256(Path(q).read_bytes()).hexdigest()
# Validate final native evidence and distinguish discovery from role authority.
results=[]
for config in ['Debug','Release']:
 log=(p/f'{config}-FINAL-v2-tests.log').read_text();assert 'PASS total_checks=40' in log
 for name in ['classic','pseudo','concurrent0','concurrent1']:
  q=p/f'{config}-FINAL-v2-fixtures/{name}.json';j=json.loads(q.read_text());assert j['first']=='NONE' and j['controlled_original_refs_remaining']==0 and not j['retained']
  assert j['unique_seed_and_class49_candidate_sum_known'];assert j['own_peak']<=128
  results.append(dict(configuration=config,case=name,observer_pid=j['observer_pid'],window_pid=j['window_process'],clients=j['clients'],peak=j['own_peak'],sum=j['unique_seed_and_class49_candidate_handles'],nodes=[dict(pid=n['pid'],creation=n['creation'],roles=n['roles'],raw=n['class49_raw'],stable=n['live_stable']) for n in j['nodes']]))
 for suffix in ['build.log','fixture-build.log']:
  assert not re.search(r'warning C\d+|error C\d+',(p/f'{config}-{suffix}').read_text(),re.I)
(p/'FINAL-FIXTURE-RESULTS.json').write_text(json.dumps(results,indent=2)+'\n')
# Current target evidence excludes observers and desktop ancestor from provisional pair subtotal.
actual=[]
for q in sorted(p.glob('OWNER-FINAL-CONCURRENT-*.json')):
 j=json.loads(q.read_text());seed=next(n for n in j['nodes'] if n['roles']&1);host=next(n for n in j['nodes'] if n['roles']&8)
 assert seed['pid']==17204 and seed['creation']==134376272948693901 and host['pid']==20144 and host['creation']==134376272949369467
 assert all(n['live_stable'] and n['original_handle_identity'] for n in j['nodes']);assert j['controlled_original_refs_remaining']==0 and not j['retained']
 actual.append(dict(file=q.name,seed=seed,host=host,distinct_unique_pair_handles=seed['handles']+host['handles'],attachment=j['attached'],attach_error=j['attach_error'],complete_administration='NOT_ESTABLISHED_NO_ELEVATED_CLIENT_LIST_OR_NORMATIVE_BINDING'))
(p/'FACTUAL-ROLE-AND-ACCOUNTING.json').write_text(json.dumps(actual,indent=2)+'\n')
with (p/'PARTICIPANT-IDENTITIES.csv').open('w',encoding='utf-8',newline='') as f:
 rows=[];j=json.loads((p/'OWNER-FINAL-CONCURRENT-1.json').read_text())
 for n in j['nodes']:
  rows.append(dict(pid=n['pid'],creation=n['creation'],role_bits=n['roles'],session=n['session'],elevated=n['elevated'],sid=n['token_user'],image=n['image_path'],volume=n['file_volume'],id_high=n['file_id_high'],id_low=n['file_id_low'],sha256=n['sha256'],handles=n['handles'],private_bytes=n['private_bytes'],working_set=n['working_set'],stable=n['live_stable'],association='CLASS49_METADATA_NOT_AUTHENTICATED',mapped_provenance='UNPROVEN'))
 w=csv.DictWriter(f,fieldnames=list(rows[0]));w.writeheader();w.writerows(rows)
# Historical proof remains byte-exact. Test changes from previous task are already historical.
history=[]
old=base/'native-tpm-host-e2e-20261027'
for name,h in json.loads((old/'OWNER-APPROVED-PACKAGE-PINS.json').read_text()).items():
 actual_hash=sha(old/name);assert h==actual_hash;history.append(dict(path=str(old/name),sha256=actual_hash,unchanged=True))
(p/'HISTORICAL-PINS-UNCHANGED.json').write_text(json.dumps(history,indent=2)+'\n')
adr=Path('docs/adr/ADR-CANDIDATE-tpm-proof-explicit-console-host-binding-20261027.md')
adr.write_text('''---
status: proposed
revision: 1
scope-level: project
scope-target: standalone native C mathematical EK proof; explicit console host binding only
supersedes: []
---
# Candidate — C0–C9 explicit real console host binding

No owner acceptance is inferred from technical investigation. No existing accepted ADR is edited. This proposal does NOT authorize TPM, Q4, NV, service, SCM/ACL, credits, CURRENT, signing or recovery. It requires specific owner normative approval and a later exact manual request after qualification.

## Context and precise change

Exact previous request2f225db7 requires window-process distinct from operator and proof. Actual parent17204 and HWND process17204 are identical, while original-handle native discovery finds separate process20144/conhost with stable creation/token/session/file identity. Readonly pair samples977 then884 handles; neither is a budget grant. Classic and ConPTY fixtures prove that HWND owner varies and is not the locator of every real console host. The proposed change replaces the window-PID role locator, NOT the requirement to identify and count actual distinct operator/native/auxiliary processes.

Accepted [B+D](DECISION-canonical-lab-console-topology-bd-20261023.md), [N7 exact historical scope](DECISION-n0start03-n7-and-ed11-evaluation-plan-20261024.md), [W](DECISION-q4-w0-w7-20261026.md), [L](DECISION-q4-l0-l7-20261026.md), [L1](DECISION-q4-l1-single-root-component-20261027.md), Q0–Q9/R0–R9/RAW1/H0–H9 remain intact. No LAB N7 acceptance transfers to this standalone proof. The exact original native proof request remains spent, not retrospectively amended.

C0. Scope is one future exact owner-manual transient mathematical proof using the existing native C protocol. No service/owner/IPC/intent/authority added. Candidate process IDs below are evidence only, never reusable admission pins by themselves.

C1. Bind operator parent, native proof and actual auxiliary by ORIGINAL process handles plus PID/creation. Enforce approved operator SID, elevation/no impersonation, token continuity, session and lifetime. File volume/ID/size/hash/protection/provenance are separate from mapped-image provenance. Reused PID, object replacement, missing mandatory data or changed identity denies.

C2. HWND owner and console client list are metadata. HWND owner may equal operator or native client without merging the REAL auxiliary into them. A complete contemporary client list must still include the authorized parent and proof and reject unexpected operation participants. This revision never authenticates by PID/name/HWND alone.

C3. NtQueryInformationProcess class49 may DISCOVER a candidate only in a qualified Windows x64 ABI profile. Preserve NTSTATUS, width and raw low bits; do not treat every masked value as a host. In the observed profile, qualified forward flag1 and nonzero aligned masked PID lead to an independently opened candidate; flag2 backlink and flag0 metadata are not additional hosts. Require repeated matching observations from original operator and native handles, original auxiliary identity and fresh census. Unsupported width/status/flags, ambiguous host, change or missing member evidence => DENY. Never call a setter, bypass privilege or rewrite Windows association state. This internal API may change; version/dependency changes invalidate eligibility.

C4. Specific owner risk acceptance would acknowledge that this native discovery plus identity checks does NOT cryptographically authenticate complete console association or mapped image. It is permitted only for the standalone mathematical experiment, with no trust/hardware-generation/admission inference. This is a NEW narrow decision, not implicit reuse of historical N7. If that residual risk is not accepted, retain DENY and require stronger authenticated linkage. Missing controlled process identity, token, file provenance, resource or lifetime evidence is NOT waived.

C5. Count by unique original process identity, not role labels. Operator + distinct actual helper share1024 handles, each process observed separately with live overlap; native proof remains128 and preobserver124. Shared/alias roles of the SAME process object count once; same PID with different creation denies. All actually operation-related additional UI/broker/helper processes must be identified and covered by the applicable existing limits or DENY. Explorer ancestry alone is not an administrative actor: observed desktop baseline stays explicit, not zero/credit; do not exclude any operation-related cost merely because it has an ancestor role. No quota, headroom, freshness, four-role or retention relaxation.

C6. Keep the inherited interactive console for exact confirmation and Escape/Stop, no artificial helper, automatic console-host selection, flag mutation or generic IPC. Context changes/lost console/failed query deny. A diagnostic FreeConsole/AttachConsole of the OBSERVER only is not a future proof topology or a substitute for observed inherited membership. Manual elevated readonly collection must first resolve the remaining client-list/context gap.

C7. Integrate only after acceptance into the existing host's guard/ownership path, preserving two temporary observer references when applicable, first error/freeze, checked close and original-reference retention. Measure added scalar storage/frames and lookup overlap before use; no presumed zero cost or budget credit. Validate concurrency, query failure, role omission, PID reuse, partial acquisition, close uncertainty, Stop and UNKNOWN. Reuse qualified crypto/protocol tests unless affected.

C8. Before any new TPM request require qualified actual topology/role coverage, file/security/identity pins, original handles, fresh124/128/1024 and other applicable obligations, exact current host/boot/operator context and separate owner operational approval. Real EK possession/revocation/custody/NV/Q4 remain separate; read-only topology success does not close E21/ED11.

C9. Prior approval4a5f was used once; no retry. Do not modify consumed request/approval/artifacts or use absence of receipt as authorization. No cleanup/refund/Stop/SCM/ACL/CURRENT/signing/recovery follows from acceptance. ED11/Phase6 PARTIAL until applicable factual gates pass.

## Evidence and remaining gate

[New factual evidence, native tests and alternatives](../integration/n0start03-integrated-closeout-20261024/real-console-role-binding-20261027/CONSOLIDATED-CLOSEOUT.md). Real seed/auxiliary identities and per-process resources observed; current collector could not join elevated console (AttachConsole5) despite FreeConsole success/no clients after detach. A single inherited read-only collector in the existing elevated owner console is prepared, not automatically executed. No new TPM request is ready.
''',encoding='utf-8')
exe=p/'Release/ConsoleRoles.exe';cmd='& "'+str((root/exe))+'" --observe-inherited 17204 134376272948693901'
(p/'MANUAL-READONLY-COMMAND.txt').write_text(cmd+'\n',encoding='utf-8')
(p/'MANUAL-READONLY-HANDOFF.md').write_text(f'''# Complemento read-only no console existente

O TASK atual autoriza investigação nativa não TPM. Este comando NÃO usa a aprovação TPM consumida e não é nova prova/intent/admissão. Necessita do Windows PowerShell já elevado do operador; o agente não contorna UAC. Não iniciar novo console/auxiliar artificial para procurar PASS.

Executável SHA256 `{sha(exe)}`. É o observador nativo de metadados, não NativeTpmProof.exe. Espera o processo17204 com criação134376272948693901, verificada por handle original. Se saiu ou mudou, NEGAR, não trocar o PID silenciosamente. A consulta fixa esse seed mas não confere autoridade por PID. Os outros processos serão descobertos e identificados por handles, token/sessão/criação/arquivo e consultas repetidas; association/mapped image continuam qualificadas no alcance declarado.

Executar uma coleta no próprio PowerShell elevado já existente, sem pipeline ou wrapper:

```powershell
{cmd}
```

O modo inherited NÃO chama FreeConsole/AttachConsole; lê os clientes do console que o observador herdou. Não usa TPM/TBS/NV, rede, serviço, SCM/ACL, root/CURRENT ou aprovação anterior. Não altera console/política de execução nem escreve arquivos protegidos. Enviar o JSON integral. Contagens e lista de clientes são novas observações, não orçamento concedido. Nada autoriza repetir uma prova TPM.

O diagnóstico anterior registrou before_clients2, FreeConsole TRUE/erro0, depois0, AttachConsole FALSE/erro5. A documentação não torna esse código prova universal de UAC; motivo exato do5 permanece indeterminado. O contexto manual herdado elimina a dependência dessa associação externa e permite comparar operador/pai/cliente/auxiliar reais. Se houver retenção, preservar processo e saída UNKNOWN, sem fechar à força.

ADR C0–C9 é PROPOSED, SHA256 `{sha(adr)}`. Leitura diagnóstica não a aceita. Após a evidência, ainda será necessária decisão explícita de binding/risco, integração técnica autorizada e pedido TPM novo com pins finais, se os gates estiverem demonstrados.
''',encoding='utf-8')
print('OBSERVER_SHA256='+sha(exe));print('ADR_SHA256='+sha(adr));print(cmd)
