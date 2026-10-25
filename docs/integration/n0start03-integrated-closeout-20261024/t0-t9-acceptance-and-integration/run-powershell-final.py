from pathlib import Path
import subprocess,json
p=Path(__file__).resolve().parent;ps=r'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe';results=[]
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  for mode in ['clean','large','console','forward-error','missing-final','console-loss','preloaded-native']:
   name=profile+'-'+cfg+'-'+mode;result=p/('ps-qualified-'+name+'.json');args=[ps,'-NoLogo','-NoProfile','-ExecutionPolicy','Bypass','-File',str(p/'powershell-fixture.ps1'),'-Case',mode,'-Config',cfg,'-Profile',profile,'-ResultPath',str(result)]
   si=subprocess.STARTUPINFO();si.dwFlags=subprocess.STARTF_USESHOWWINDOW;si.wShowWindow=0
   interactive=mode in ['console','console-loss'];log=None
   if interactive:q=subprocess.Popen(args,startupinfo=si,creationflags=subprocess.CREATE_NEW_CONSOLE)
   else:
    log=(p/('ps-qualified-'+name+'.log')).open('wb');q=subprocess.Popen(args,stdout=log,stderr=subprocess.STDOUT)
   try:code=q.wait(timeout=25)
   except subprocess.TimeoutExpired:
    (p/'RETAINED-PS-QUALIFIED.json').write_text(json.dumps({'pid':q.pid,'mode':mode,'no_kill':True}));raise
   if log:log.close()
   data=json.loads(result.read_text(encoding='utf-8-sig')) if result.exists() else {}
   assert code==0 and data.get('result')=='PASS',(name,code,data)
   results.append(data)
(p/'powershell-qualified-results.json').write_text(json.dumps(results,indent=2));print('PASS',len(results),'PowerShell5.1 cases, canonical forwarding fragment; no operational dispatch')
