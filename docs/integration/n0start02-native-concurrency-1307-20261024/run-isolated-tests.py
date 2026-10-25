from pathlib import Path
import subprocess,uuid,json
pkg=Path(__file__).resolve().parent
root=pkg/'isolated'
config=root/'config.fixture'
results=[]
def save(): (pkg/'native-results.json').write_text(json.dumps(results,indent=2))
def args(profile,build,tag,mode='isolated-current-owner'):
 return [str(pkg.parents[2]/'build'/pkg.name/profile/build/'papacc_service_cross_admission_test.exe'),r'Local\PACC.Preconsume.Offline.'+uuid.uuid4().hex,str(root/(tag+'.intent')),str(config),'ordinary',mode]
def run(a,cmd='q\n'):
 r=subprocess.run(a,input=cmd,text=True,capture_output=True,timeout=15)
 results.append({'args':a,'input':cmd,'exit':r.returncode,'output':r.stdout,'stderr':r.stderr});save()
 assert r.returncode==0,(r.stdout,r.stderr)
 return r.stdout
def launch(a):
 p=subprocess.Popen(a,stdin=subprocess.PIPE,stdout=subprocess.PIPE,stderr=subprocess.PIPE,text=True)
 lines=[]
 while True:
  line=p.stdout.readline()
  if not line:raise AssertionError('Fixture exited before ENTER: '+''.join(lines))
  lines.append(line)
  if line.startswith('ENTER='):break
 assert lines[-1].startswith('ENTER=0 '),''.join(lines)
 return p,''.join(lines)
def send(p,cmd,marker):
 p.stdin.write(cmd+'\n');p.stdin.flush();lines=[]
 while True:
  line=p.stdout.readline()
  if not line:raise AssertionError('Fixture exited before '+marker)
  lines.append(line)
  if marker in line:return ''.join(lines)
for profile in ['Selected','Legacy']:
 for build in ['Debug','Release']:
  tag=profile+'-'+build
  a=args(profile,build,tag+'-normal');p,begin=launch(a)
  busy=run(a);assert 'ENTER=170 ' in busy and 'REJECT_CLOSE=0' in busy
  tail,err=p.communicate('q\n',timeout=15);assert p.returncode==0 and 'FINISH=0 PHASE=2 MUTEX=0 INTENT=0' in tail
  results.append({'case':'first_acquisition_and_factual_release','config':tag,'output':begin+tail,'exit':p.returncode});save()
  again=run(a);assert 'ENTER=0 ' in again and 'FINISH=0 PHASE=2 MUTEX=0 INTENT=0' in again
  stop=run(args(profile,build,tag+'-stop'),'s\n');assert 'STOP=0' in stop and 'AFTER_STOP=5023' in stop and 'MUTEX=0 INTENT=0' in stop
  invalid=run(args(profile,build,tag+'-owner-invalid','isolated-canonical-owner-negative'));assert 'CreateMutexExW' in invalid and 'ENTER=1307 ' in invalid and 'REJECT_CLOSE=0' in invalid
  mismatch=run(args(profile,build,tag+'-token-divergent','isolated-token-divergent'));assert 'ENTER=5 ' in mismatch and 'REJECT_CLOSE=0' in mismatch
  a=args(profile,build,tag+'-unknown');p,begin=launch(a)
  retained=send(p,'v','UNKNOWN_BEFORE_EFFECT=3');refused=send(p,'r','NO_FORCED_RELEASE')
  assert 'RETAINED_FINISH=5023 PHASE=3 MUTEX=1 OWNED=1' in refused
  busy=run(a);assert 'ENTER=170 ' in busy
  tail,err=p.communicate('x\n',timeout=15)
  assert p.returncode==0
  results.append({'case':'UNKNOWN_retains_mutex_refuses_finish_and_blocks_peer','config':tag,'output':begin+retained+refused+tail,'exit':p.returncode,'fixture_lifetime_ended':True,'semantic_cleanup_claim':False,'mutex_name_reused_after_unknown':False});save()
assert not list(root.glob('*.intent')) and not list(root.glob('*.launch'))
print('PASS FOUR CONFIGURATIONS: first,peer BUSY,factual release,reacquire,Stop,invalid owner1307,token expectation divergent5,UNKNOWN retention/refused finish/peer BUSY. NO INTENT,SCM,LAB,OPERATIONAL MUTEX.')
