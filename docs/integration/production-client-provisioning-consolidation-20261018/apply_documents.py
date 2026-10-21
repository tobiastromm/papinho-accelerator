from pathlib import Path
import subprocess,json,difflib,re,hashlib
A=Path(r"C:\Projetos\PapinhoAccelerator")
D=A/"docs/integration/production-client-provisioning-consolidation-20261018"
E=Path(r"C:\Users\tobias\.vscode\extensions\openai.chatgpt-26.1007.21434-win32-x64\bin\windows-x86_64\codex.exe")
def apply(p,new):
 b=json.loads((D/"BASELINE.json").read_text(encoding="utf-8"))["accelerator"]["files"]
 t=A/p
 if p in b:
  assert hashlib.sha256(t.read_bytes()).hexdigest()==b[p], "STOP: preimage changed "+p
  old=t.read_text(encoding="utf-8")
  delta="".join(list(difflib.unified_diff(old.splitlines(True),new.splitlines(True),n=3))[2:])
  delta=re.sub(r"(?m)^@@[^\n]*@@[^\n]*$", "@@",delta)
  patch="*** Begin Patch\n*** Update File: "+p+"\n"+delta+"*** End Patch\n"
 else:
  assert not t.exists(), "STOP: new path exists "+p
  patch="*** Begin Patch\n*** Add File: "+p+"\n"+"".join("+"+s+"\n" for s in new.splitlines())+"*** End Patch\n"
 assert len(patch)<29000
 r=subprocess.run([str(E),"--codex-run-as-apply-patch",patch],cwd=A,stdout=subprocess.PIPE,stderr=subprocess.STDOUT,text=True)
 lp=D/"EXECUTOR-LOG.json"; logs=json.loads(lp.read_text()) if lp.exists() else []
 logs.append({"path":p,"exit_code":r.returncode,"output":r.stdout})
 lp.write_text(json.dumps(logs,ensure_ascii=False,indent=2)+"\n",encoding="utf-8")
 if r.returncode: raise SystemExit("STOP: official patch failed; no alternate mechanism: "+p+" "+r.stdout)
 assert t.read_text(encoding="utf-8")==new, "STOP: postimage mismatch "+p
 print("APPLIED "+p,flush=True)
