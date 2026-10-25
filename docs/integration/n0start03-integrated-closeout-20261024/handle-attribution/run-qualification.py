from pathlib import Path
import json,subprocess,re
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/handle-attribution';old=json.loads((r/'docs/integration/n0start03-integrated-closeout-20261024/ed11-closure-drive/regression-results.json').read_text(encoding='utf-8'));results=[]
cases=[('clean',0),('identity',0),('stop',0),('vquery-fail',0),('trace-query-fail',0),('close-unknown',2),('snapshot-free-unknown',2),('table-core',0),('table-core-query',0),('table-core-stop',0),('table-core-budget',0),('table-core-close',2),('table-core-capture-contradiction',2),('table-core-marker-contradiction',2),('table-core-postcapture-budget',0),('table-core-walk-stop',0)]
for cfg in ['Debug','Release']:
 root=r/'build/ed11-handle-attribution-20261024/Selected'/cfg;exe=root/'papacc_service_observer_stack_va_isolated_test.exe'
 for case,expected in cases:
  v=subprocess.run([str(exe),'--isolated-observer-self',case],capture_output=True,text=True,timeout=30);(p/(cfg+'-qualified-'+case+'.log')).write_text(v.stdout+v.stderr,encoding='utf-8');assert v.returncode==expected,(cfg,case,v.returncode,v.stdout[-1000:]);results.append({'config':cfg,'case':case,'exit':v.returncode,'expected':expected})
 for v0 in old[:15]:
  name=v0['test'];cmd=[str(root/('papacc_'+name+'.exe'))]+(['--model'] if name=='service_n0_sd_test' else [])
  v=subprocess.run(cmd,capture_output=True,text=True,timeout=60);assert v.returncode==0,(cfg,name,v.stdout[-1500:]);(p/(cfg+'-qualified-regression-'+name+'.log')).write_text(v.stdout+v.stderr,encoding='utf-8');results.append({'config':cfg,'test':name,'exit':0})
 log=(p/('build-final2-'+cfg+'.log')).read_text(encoding='utf-8',errors='replace');assert not re.search(r'warning C\d+|error C\d+',log)
(p/'final-qualification-results.json').write_text(json.dumps(results,indent=2),encoding='utf-8')
b=(p/'scm-instrumented-cycle-lab.inc.preimage').read_text(encoding='utf-8');s=(r/'tools/qualification/scm-instrumented-cycle-lab.inc').read_text(encoding='utf-8')
s=re.sub(r'#ifdef PAPACC_READONLY_HANDLE_TABLE_TRACE\n L"[^\n]+\n#elif defined\(PAPACC_N0START03_READONLY_PINS\)', '#ifdef PAPACC_N0START03_READONLY_PINS',s,count=1)
s=re.sub(r'#ifdef PAPACC_READONLY_HANDLE_TABLE_TRACE\n.*?#endif\n','',s,flags=re.S);s=s.replace('#include "scm-readonly-handle-table.inc"\n','')
assert [x for x in s.splitlines() if x.strip()]==[x for x in b.splitlines() if x.strip()]
(p/'trace-off-source-equivalence.json').write_text(json.dumps({'nonblank_source_lines_equal_after_removing_new_optin_blocks':True,'not_binary_equivalence':True}),encoding='utf-8')
print('FINAL_BUILDS_ZERO_WARNINGS;32_OBSERVER_TABLE_EXPECTED_OUTCOMES;30_REGRESSIONS_PASS;MACRO_OFF_SOURCE_EQUIVALENCE_PASS')
