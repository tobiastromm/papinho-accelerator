from pathlib import Path
import hashlib,json,shutil
r=Path.cwd();p=r/'docs/integration/n0start03-integrated-closeout-20261024/q4-final-pins-and-q7-readiness-20261025';old=p.parent/'q4-annex-acceptance-and-binding-20261025';d=r/'tools/qualification'
names=['scm-n0-context-pins.h','scm-n0-context.inc','scm-n0-private-stdout-entry.inc','scm-lab-attempt-binding.h','scm-instrumented-cycle-lab.inc','scm-runtime-cycle-lab.inc','scm-n0-protected-single-powershell.ps1','scm-q4-image-pin.h'];(p/'preimages').mkdir();base={}
for name in names:
 f=d/name;shutil.copyfile(f,p/'preimages'/name);base[str(f.relative_to(r))]=hashlib.sha256(f.read_bytes()).hexdigest()
(p/'source-preimages.json').write_text(json.dumps(base,indent=2))
n7='1d6bca5386e3520034570d17cf6f526fdef4c19b52aee633113e1bd8e5b40fe7';q0='97763395f521d0a8d15ee0afe22b61705897d6d97c2c1f8905ad9114e8b1b9ca';q4='590aa1c695adce55d6ba17e9ce508d747090c05c2d61cd44a7ced603cf6c4db4';q2='5794e17265af6e34d335591b0fed42e5d9048aec81e751578fd9f4132cf524e1';ev='bb548b4f2a866d33445d39e2bb72e0093b60b04380cc4da6fd70c4855a0038fa';ds='c63209c7591f404eead7ca8e3e4b91b5b3c8db13e47ad0e637c6e2c3a36a46e6'
def save(name,s):(d/name).write_text(s,newline='\r\n')
s=(d/names[0]).read_text().replace('Successor input pins intentionally EMPTY until separately qualified placement.','Original receipt-pinned inputs; not execution or budget authority.').replace('#define IG_N0_N7_ED11_SHA ""',f'#define IG_N0_N7_ED11_SHA "{n7}"',1)
marker='#if !defined(PAPACC_Q4_TOKEN_DIAGNOSTIC) && (defined(PAPACC_N0START03_READONLY_PINS) || defined(PAPACC_N0START03_PRIVATE_POSITIVE))'
block='#if defined(PAPACC_Q4_TOKEN_DIAGNOSTIC) && defined(PAPACC_N0START03_PRIVATE_POSITIVE)\n'
for k,v in dict(DOSSIER_SHA='"'+ds+'"',EVIDENCE_SHA='"'+ev+'"',DOSSIER_VOLUME='417614100UL',DOSSIER_HIGH='327680UL',DOSSIER_LOW='2932527UL',DOSSIER_BYTES='397UL',EVIDENCE_VOLUME='417614100UL',EVIDENCE_HIGH='2555904UL',EVIDENCE_LOW='433780UL',EVIDENCE_BYTES='8432UL').items():block+=f'#define IG_N0_{k} {v}\n'
s=s.replace(marker,block+marker.replace('#if ','#elif ',1));save(names[0],s)
s=(d/names[1]).read_text().replace('#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC\n (void)d;(void)out;return ERROR_ACCESS_DENIED; /* no fabricated future plan */\n#else\n','').replace('\n#endif\n}\nstatic DWORD ig_n0_sd','\n}\nstatic DWORD ig_n0_sd',1)
a=s.index('L1=2d91');b=s.index('SERVICE=',a);legacy=s[a:b];s=s[:a]+'" IG_N0_DISPOSITION_PLAN "'+s[b:]
macro=f'#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC\n#define IG_N0_DISPOSITION_PLAN "Q0={q0};Q4={q4};Q2={q2};"\n#else\n#define IG_N0_DISPOSITION_PLAN "{legacy}"\n#endif\n'
s=s.replace('static DWORD ig_n0_plan(',macro+'static DWORD ig_n0_plan(',1);save(names[1],s)
s=(d/names[2]).read_text();a=s.index('#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC');b=s.index('#else',a);s=s[:a]+s[b+len('#else\n'):];s=s.rsplit('#endif',1)[0];save(names[2],s)
s=(d/names[3]).read_text().replace('/* Protected inputs, exact N7, final binding and fresh budget remain absent. */\n#define PACC_LAB_BINDING_READY 0','/* Static binding only; exact Q7 and fresh admission remain mandatory. */\n#define PACC_LAB_BINDING_READY 1');save(names[3],s)
s=(d/names[4]).read_text();line=next(x for x in s.splitlines() if x.startswith('static const WCHAR ig_intent_path'))
canon=r'C:\\Projetos\\PapinhoAccelerator\\build\\scm-protected-installation-binding-20261022\\Selected\\Release\\papacc-psr-'
s=s.replace(line,'#ifdef PAPACC_Q4_TOKEN_DIAGNOSTIC\nstatic const WCHAR ig_intent_path[]=L"'+canon+'" PACC_LAB_ATTEMPT_W L".intent";\n#else\n'+line+'\n#endif')
approval=r'C:\\Projetos\\PapinhoAccelerator\\docs\\integration\\n0start03-integrated-closeout-20261024\\q4-final-pins-and-q7-readiness-20261025\\EXACT-Q7-APPROVED.json'
s=s.replace('#ifdef PAPACC_N0START03_PRIVATE_POSITIVE\n L"C:', '#if defined(PAPACC_Q4_TOKEN_DIAGNOSTIC)\n L"'+approval+'",\n#elif defined(PAPACC_N0START03_PRIVATE_POSITIVE)\n L"C:',1);save(names[4],s)
s=(d/names[5]).read_text().replace('q4-binding-20261025','q4-final-binding-20261025');save(names[5],s)
s=(d/names[6]).read_text();start=s.index(" throw 'Q4_BINDING_INCOMPLETE_DENY");end=s.index('\n',start)
block=" $Qualifier=$Root+'\\build\\q4-final-binding-20261025\\Selected\\Release\\papacc_scm_instrumented_lab.exe'\n $QualifierHash='"+'0'*64+"'\n $TransportNative=$Root+'\\build\\q4-final-binding-20261025\\Selected\\Release\\papacc_n0_private_stdout.dll'\n $TransportNativeHash='"+'0'*64+"'\n $DossierHash='"+ds+"'\n $EvidenceHash='"+ev+"'\n $ApprovalPath=$Root+'\\docs\\integration\\n0start03-integrated-closeout-20261024\\q4-final-pins-and-q7-readiness-20261025\\EXACT-Q7-APPROVED.json'"
s=s[:start]+block+s[end:]
s=s.replace("if($Profile-cne'N0START03ReadOnly'-and$Profile-cne'N0START03Positive')","if($Profile-cne'N0START03ReadOnly'-and$Profile-cne'N0START03Positive'-and$Profile-cne'Q4TokenDiagnostic')")
s=s.replace("if(($Profile-ceq'N0START03Positive'-and$Mode-cne'Cycle'", "if((($Profile-ceq'N0START03Positive'-or$Profile-ceq'Q4TokenDiagnostic')-and$Mode-cne'Cycle'")
s=s.replace("if($Profile-ceq'N0START03Positive'){Assert-PositiveApproval", "if($Profile-ceq'N0START03Positive'-or$Profile-ceq'Q4TokenDiagnostic'){Assert-PositiveApproval")
extra=" if($Profile-ceq'Q4TokenDiagnostic'){\n"
for key,value in dict(Q0_Q9_sha256=q0,Q4_annex_sha256=q4,Q2_record_sha256=q2,N7_sha256=n7,evaluation_plan_sha256='d93cff9c596ad68529fb836f1dc45280af9686828efcf939ac48621a01b4e9e6').items():extra+=f"  if($record.{key}-isnot[string]-or$record.{key}-cne'{value}'){{throw 'Q7_POLICY_BINDING_DENY'}}\n"
extra+="  if($record.q7_activation_authorized-isnot[bool]-or$record.q7_activation_authorized-ne$true){throw 'Q7_EXACT_OWNER_AUTHORITY_REQUIRED'}\n }\n"
s=s.replace(' if($record.artifacts.launcher.path-cne$ExpectedPath',extra+' if($record.artifacts.launcher.path-cne$ExpectedPath')
oldpin=next(x for x in s.splitlines() if x.startswith(" Pin ($Root+'\\docs\\adr\\DECISION-n0start03-n7-and"))
new=" if($Profile-ceq'Q4TokenDiagnostic'){\n"
for path,value in [('docs/adr/DECISION-q4-specific-n7-and-independent-evaluation-20261025.md',n7),('docs/adr/DECISION-q4-independent-service-token-observation-annex-20261025.md',q4),('docs/adr/DECISION-n0start03-poststart-disposition-and-one-token-observation-20261025.md',q0),('docs/integration/n0start03-integrated-closeout-20261024/q2-protected-readback-native-result-20261025/Q2-EXACT-DISPOSITION-RECORDED.md',q2),('docs/integration/n0start03-integrated-closeout-20261024/q4-n7-protected-inputs-and-activation-20261025/INDEPENDENT-ED11-EVALUATION-PLAN.md','d93cff9c596ad68529fb836f1dc45280af9686828efcf939ac48621a01b4e9e6')]:new+="  Pin ($Root+'\\"+path.replace('/','\\')+"') '"+value+"'\n"
s=s.replace(oldpin,new+' }else{\n'+oldpin+'\n }')
for ext in ['approved','evidence']:
 s=s.replace("Pin 'C:\\ProgramData\\PapinhoAccelerator\\Protected\\N0\\papacc-n2-0b1de869e11493b7cb08fb329578f6ce."+ext+"'", "Pin ('C:\\ProgramData\\PapinhoAccelerator\\Protected\\N0\\papacc-n2-'+$Attempt+'."+ext+"')")
save(names[6],s)
for name in ['run-builds.py','finalize-derived-pins.py']:
 (p/name).write_text((old/name).read_text().replace('q4-binding-20261025','q4-final-binding-20261025'))
for profile in ['Selected','Legacy']:
 for cfg in ['Debug','Release']:
  name=f'build-{profile}-{cfg}.cmd';(p/name).write_text((old/name).read_text().replace('q4-binding-20261025','q4-final-binding-20261025'))
print('INTEGRATION_WRITTEN; NO_OPERATION')
