from pathlib import Path
import subprocess,json,re,sys,datetime,hashlib
root=Path.cwd();p=root/'docs/integration/n0start03-integrated-closeout-20261024/t0-t9-acceptance-and-integration';results=[]
out=p/'runs'/datetime.datetime.now(datetime.timezone.utc).strftime('%Y%m%dT%H%M%S%fZ');out.mkdir(parents=True,exist_ok=False)
def run(exe,args,expected,log):
 with log.open('wb') as f:
  q=subprocess.Popen([str(exe)]+args,stdout=f,stderr=subprocess.STDOUT)
  try:code=q.wait(timeout=45)
  except subprocess.TimeoutExpired:
   (p/'RETAINED-RUN.json').write_text(json.dumps({'pid':q.pid,'args':args,'no_kill':True}));raise RuntimeError('UNKNOWN_NO_KILL_NO_RETRY')
 assert code==expected,(str(exe),args,code,log.read_text(errors='replace')[-1500:])
 results.append({'binary':str(exe.relative_to(root)),'args':args,'exit':code,'expected':expected,'log':str(log.relative_to(p))})
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  folder=root/'build/ed11-private-stdout-20261025'/profile/cfg
  core=folder/'papacc_noninherited_transport_integration_test.exe'
  modes=[['clean'],['reentrant'],['create-fail'],['wrong-peer'],['token-deny'],['spoof-replay'],['race'],['write-fail']]+[['--fault',s] for s in ['name-deny','birth-deny','impersonation','close-unknown','fclose-unknown']]
  for args in modes:run(core,args,0,out/('qualified-'+profile+'-'+cfg+'-core-'+'-'.join(args)+'.log'))
  for name in ([] if '--core-only' in sys.argv else json.loads((p/'regression-test-list.json').read_text())):run(folder/('papacc_'+name+'.exe'),['--model'] if name=='service_n0_sd_test' else [],0,out/('qualified-'+profile+'-'+cfg+'-'+name+'.log'))
  build=(p/('build-'+profile+'-'+cfg+'-final.log')).read_text(errors='replace')
  assert not re.search(r'warning [A-Z]+\d+|error [A-Z]+\d+',build,re.I)
destination=p/('native-core-final-results.json' if '--core-only' in sys.argv else 'native-and-regression-results.json')
if destination.exists():(out/'previous-results.json').write_bytes(destination.read_bytes())
destination.write_text(json.dumps(results,indent=2))
(out/'source-and-binary-hashes.json').write_text(json.dumps({str(f.relative_to(root)):hashlib.sha256(f.read_bytes()).hexdigest() for f in [root/'tools/qualification/scm-n0-private-stdout-native.c',root/'tools/qualification/scm-n0-private-stdout-adapter.inc']+list((root/'build/ed11-private-stdout-20261025').glob('*/*/papacc_noninherited_transport_integration_test.exe'))},indent=2))
print('PASS',len(results),'expected outcomes; 4 profiles, no operational owner entry')
