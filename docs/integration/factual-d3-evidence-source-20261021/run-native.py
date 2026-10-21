from pathlib import Path
import subprocess,json
r=Path(r"C:\Projetos\PapinhoAccelerator");out=r/"build/fd3-20261021/native";out.mkdir(parents=True,exist_ok=False);rows=[]
for mode in ["Debug","Release"]:
 exe=r/f"build/fd3-20261021/Selected/{mode}/papacc_issuer_root_store_test.exe"
 for case in ["--native-history","--native-snapshots","--pending-unknown","--pending-stop"]:
  tag=mode+"-"+case.strip("-");target=out/(tag+".store");assert not target.exists()
  p=subprocess.run([str(exe),case,str(target)],capture_output=True,text=True,timeout=60)
  (out/(tag+".log")).write_text(p.stdout+p.stderr,encoding="utf8")
  rows.append({"configuration":mode,"case":case,"exit":p.returncode,"evidence":"native local filesystem/worker with synthetic component authority; NOT operational CURRENT or D3 source"})
  print(tag,p.returncode,flush=True)
  if p.returncode:break
 if rows[-1]["exit"]:break
(out/"native-results.json").write_text(json.dumps(rows,indent=2),encoding="utf8")
raise SystemExit(any(x["exit"] for x in rows))
