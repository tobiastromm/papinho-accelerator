from pathlib import Path
import importlib.util,json
r=Path.cwd();p=Path(__file__).resolve().parent;spec=importlib.util.spec_from_file_location('pincheck',p/'verify-image-pin-in-binary.py');m=importlib.util.module_from_spec(spec);spec.loader.exec_module(m)
expected='128834681905d861eb3252c1e0079e1d06a02b46c2f6d34ddb3612bcc85b3dc8';stale='2386d63a256affd48c3dfbcd78ff36380400c86f61a0eeb68e209d6ed4520821';old=(r/'build/q4-final-binding-20261025/Selected/Release/papacc_scm_instrumented_lab.exe').read_bytes();new=(r/'build/q7-source-pin-correction-20261025/Selected/Release/papacc_scm_instrumented_lab.exe').read_bytes()
cases=[('current',new,expected,True),('actual_failed_approved_qualifier',old,expected,False),('wrong_image_hash',new,'f'*64,False),('missing_ascii',new.replace(expected.encode(),b'f'*64),expected,False),('missing_utf16',new.replace(expected.encode('utf-16le'),('f'*64).encode('utf-16le')),expected,False),('truncated_pe',new[:64],expected,False),('not_pe',b'fixture',expected,False)]
rows=[]
for name,b,sha,want in cases:
 ok=True;error=None
 try:m.attest(b,sha,[stale])
 except ValueError as e:ok=False;error=str(e)
 assert ok==want,(name,ok,want);rows.append(dict(case=name,passed=True,accepted=ok,error=error))
(p/'compiled-pin-negative-tests.json').write_text(json.dumps(rows,indent=2));print('COMPILED_PIN_TESTS=PASS cases='+str(len(rows))+'; ACTUAL_FAILED_QUALIFIER_REJECTED; NO_EXECUTION')
