from pathlib import Path
import subprocess,time,json,re,hashlib
r=Path(__file__).resolve().parents[4];p=Path(__file__).resolve().parent
campaign=p/'campaigns'/str(time.time_ns());campaign.mkdir(parents=True,exist_ok=False)
results=[];hashes={}
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  exe=r/'build/ed11-positive-composition-20261025'/profile/cfg/'fixture.exe';hashes[str(exe.relative_to(r))]=hashlib.sha256(exe.read_bytes()).hexdigest()
  for case in ['clean','race','access-denied','acquire-fail','close-unknown','saturation','stop','stale','terminated','alive','token','replay','self-unknown','legacy-live','last-snapshot-unknown','last-access-unknown','last-token-unknown']:
   label=f'{profile}-{cfg}-{case}';run=r/'build/n0start03-positive-resource-requalification-20261024/runs'/('composition-final-'+label+'-'+str(time.time_ns()));run.mkdir(parents=True,exist_ok=False)
   log=campaign/(label+'.log')
   with log.open('wb') as f:
    q=subprocess.Popen([str(exe),str(run),case,'1'],stdout=f,stderr=subprocess.STDOUT,creationflags=0x08000000)
    try:code=q.wait(timeout=35)
    except subprocess.TimeoutExpired:
     (campaign/'RETAINED-RUN.json').write_text(json.dumps({'pid':q.pid,'case':label,'no_kill_no_retry':True}));raise
   s=log.read_text(encoding='ascii');expected=2 if case in ['close-unknown','self-unknown','last-snapshot-unknown','last-access-unknown','last-token-unknown'] else 0
   good=code==expected and 'COMPOSITION_PARENT_CLOSED' in s and 'result_received=1 first=0 retained=0' in s
   if case in ['clean','race','alive']:good &= all(x in s for x in ['LAB_TOKEN_USER=','LAB_PROCESS_OBSERVATION','LAB_ENDPOINT_TABLES=','COMPOSED_STACK','own_references_closed=1','COMPOSED_NATIVE api=GetExtendedTcpTable_v4'])
   if case=='race':good &= 'competitors=10 private_object_matches=0 result=0' in s
   if case.startswith('last-'):good &= 'LAST_USE_UNKNOWN first=5' in s and 'original_retained=1 freeze=1 stop=1 no_retry=1 NO_NEW_ACQUISITION' in s
   result={'profile':profile,'config':cfg,'case':case,'exit':code,'expected':expected,'pass':bool(good),'log':str(log.relative_to(p)),'run_directory':str(run.relative_to(r))}
   results.append(result)
   (campaign/'results.json').write_text(json.dumps(results,indent=2)+'\n',encoding='utf-8')
   if not good:print(label,code,s[-2500:]);raise SystemExit(1)
(p/'final-native-composition-results.json').write_text(json.dumps({'campaign':str(campaign.relative_to(p)),'results':results,'binary_hashes':hashes},indent=2)+'\n',encoding='utf-8')
print('PASS',len(results),'final outcomes, no product entry')
