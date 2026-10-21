from pathlib import Path
import subprocess, json, sys, hashlib
exe=Path(sys.argv[1]).resolve(); dest=Path(sys.argv[2]);dest.mkdir(parents=True,exist_ok=True)
minimal=b'format_version 1\nclient_admission DISABLED\nsite_tls_admission DENY\ned11_admission DENY\nnetwork_egress DISABLED\n'
base=b'format_version 1\nbinding input\n mode ALL\nend\nchannel_tls tls\n security_ref "DO_NOT_LOG_SECRET_123"\nend\naccess_auth auth\n method NONE\nend\ncapability_policy policy\n enable 2 1\n rule no DENY 2 1\nend\nlistener front\n binding_ref input\n port 7443\n transport SECURE_PRINCIPAL\n channel_tls_ref tls\n admission OPEN\n authentication_ref auth\n policy_ref policy\nend\n'
# Do not invoke normal product launch: every invocation is config/help only.
cases=[('negative-gates',minimal,['validate'],0,'VALIDATION_SCOPE=OFFLINE_ONLY'),
 ('empty',b'',['validate'],1,'line='),('comments',b'# hi\n; ignored\n',['validate'],1,'line='),
 ('disabled',b'format_version 1\n; garbage "\n',['validate'],0,'SOURCE=VALIDATED'),
 ('inline-semicolon',b'format_version 1; bad\n',['validate'],1,'column='),
 ('unknown',b'format_version 1\nsecret_unknown x\n',['validate'],1,'unknown'),
 ('partial-block',b'format_version 1\nbinding input\n mode ALL\n',['validate'],1,'end required'),
 ('utf8',b'format_version 1\n#\xff',['validate'],1,'invalid UTF8'),
 ('nul-byte',minimal+b'\x00',['validate'],1,'invalid UTF8'),
 ('too-large',b' '*8193,['validate'],1,'source limit'),
 ('all-unresolved',base,['validate'],2,'FACTUAL_ROOT_SOURCE_UNAVAILABLE'),
 ('selected',base.replace(b'mode ALL',b'mode SELECTED\n interface_ref "123"'),['validate'],2,'INTERFACE_TOPOLOGY_NOT_QUALIFIED'),
 ('provider',base.replace(b'method NONE',b'method CERTIFICATE_IDENTITY\n provider_ref "DO_NOT_LOG_SECRET_456"').replace(b'admission OPEN',b'admission CLOSED'),['validate'],2,'AUTH_PROVIDER_PENDING'),
 ('legacy',base.replace(b'SECURE_PRINCIPAL',b'LEGACY_ENDPOINT'),['validate'],1,'Legacy unsupported'),
 ('profile-absent',base.replace(b' transport SECURE_PRINCIPAL\n',b''),['validate'],1,'explicit profile'),
 ('diff-no-current',minimal,['diff'],2,'CURRENT_PIN=ABSENT'),
 ('diff-invalid',b'bad',['diff'],1,'NO_COMPARISON_EMITTED'),
 ('apply-denied',base,['apply'],2,'APPLY=DENY_UNTIL_QUALIFIED')]
results=[]
for name,data,cmd,code,marker in cases:
 path=dest/(name+'.conf');path.write_bytes(data)
 before=hashlib.sha256(path.read_bytes()).hexdigest()
 p=subprocess.run([str(exe),'config',*cmd,'--source',str(path)],capture_output=True,text=True,timeout=20)
 output=p.stdout+p.stderr
 assert p.returncode==code,(name,p.returncode,code,output)
 assert marker in output,(name,marker,output)
 assert 'DO_NOT_LOG_SECRET' not in output and 'secret_unknown' not in output,(name,'secret/token echo')
 assert hashlib.sha256(path.read_bytes()).hexdigest()==before
 results.append({'case':name,'exit':p.returncode,'output':output,'source_unchanged':True})
for name,args,code,marker in [
 ('show-no-current',['config','show-effective'],2,'NO_EFFECTIVE_STATE_EMITTED'),
 ('apply-no-read',['config','apply','--source',str(dest/'not-created.conf')],2,'APPLY=DENY'),
 ('missing-source',['config','validate','--source',str(dest/'not-created.conf')],1,'SOURCE=UNAVAILABLE'),
 ('invalid-args',['config','validate'],1,'INVALID_COMMAND'),
 ('unknown-command',['config','bootstrap'],1,'INVALID_COMMAND'),
 ('no-extra-options',['config','show-effective','--port','39999'],1,'INVALID_COMMAND'),
 ('help',['--help'],0,'config validate|diff|apply')]:
 p=subprocess.run([str(exe),*args],capture_output=True,text=True,timeout=20)
 output=p.stdout+p.stderr
 assert p.returncode==code and marker in output,(name,p.returncode,output)
 results.append({'case':name,'exit':p.returncode,'output':output})
assert not (dest/'not-created.conf').exists()
(dest/'results.json').write_text(json.dumps({'cases':results,'count':len(results),'scope':'real candidate CLI; no root/CURRENT/bootstrapping/network tested'},indent=2),encoding='utf-8')
print('CLI_OFFLINE_PASS',len(results))
