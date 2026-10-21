import pathlib,subprocess,json
r=pathlib.Path.cwd();d=r/'docs/integration/package-delegation-persistence-implementation-20261020';rows=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  b=r/'build/root08-issuer-factual-transaction-binding-20261020'/profile/cfg
  for exe,modes in [('papacc_d3_worker_test',['reject','success','profile-success','conflict','conflict-last','unknown','stop']),('papacc_issuer_root_store_test',['--codec-only','--native-history','--native-snapshots','--pending-success','--pending-conflict','--pending-unknown','--pending-stop'])]:
   for mode in modes:
    tag=profile+'-'+cfg+'-'+exe+'-'+mode.strip('-');target=d/(tag+'.store');assert not target.exists(),str(target)
    q=subprocess.run([str(b/(exe+'.exe')),mode,str(target)],capture_output=True,text=True,timeout=30)
    (d/(tag+'.log')).write_text(q.stdout+q.stderr);rows.append({'case':tag,'exit':q.returncode})
(d/'native-results.json').write_text(json.dumps(rows,indent=2)+'\n')
print('native cases',len(rows),'failures',[x for x in rows if x['exit']])
raise SystemExit(any(x['exit'] for x in rows))
