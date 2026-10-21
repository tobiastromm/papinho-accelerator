from pathlib import Path
import subprocess,json
R=Path(r"C:\Projetos\PapinhoAccelerator");D=R/"build/kind8-20261021/native";D.mkdir(parents=True,exist_ok=False);rows=[]
for cfg in ["Debug","Release"]:
 for exe,modes in [("papacc_issuer_root_store_test",["--native-history","--native-snapshots","--pending-unknown","--pending-stop"]),("papacc_d3_worker_test",["reject","conflict","unknown","stop"])]:
  for mode in modes:
   name=cfg+"-"+exe+"-"+mode.strip("-");store=D/(name+".store");assert not store.exists()
   p=subprocess.run([str(R/f"build/kind8-20261021/Selected/{cfg}/{exe}.exe"),mode,str(store)],capture_output=True,text=True,timeout=60)
   (D/(name+".log")).write_text(p.stdout+p.stderr,encoding="utf8")
   rows.append({"case":name,"exit":p.returncode,"authority":"SYNTHETIC_COMPONENT_ONLY_NOT_CURRENT_V3"});print(name,p.returncode,flush=True)
   if p.returncode:break
  if rows[-1]["exit"]:break
 if rows[-1]["exit"]:break
(D/"native-results.json").write_text(json.dumps(rows,indent=2),encoding="utf8")
raise SystemExit(any(x["exit"] for x in rows))
