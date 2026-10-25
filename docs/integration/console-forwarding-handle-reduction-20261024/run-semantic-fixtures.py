from pathlib import Path
import subprocess,ctypes as C,ctypes.wintypes as W,json,time
ROOT=Path(__file__).resolve().parent;PS=r'C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe'
args=[PS,'-NoProfile','-ExecutionPolicy','Bypass','-File',str(ROOT/'semantic-tests.ps1'),'-Kind','redirected','-Result',str(ROOT/'redirected-results.json')]
r=subprocess.run(args,capture_output=True,timeout=30);(ROOT/'redirected-stdout.bin').write_bytes(r.stdout);(ROOT/'redirected-stderr.bin').write_bytes(r.stderr)
assert r.returncode==0,(r.returncode,r.stderr.decode(errors='replace'))
assert 'UTF8_REDIRECT ação 漢字 😀'.encode('utf-8') in r.stdout
K=C.WinDLL('kernel32',use_last_error=True)
class SI(C.Structure):
 _fields_=[('cb',W.DWORD),('reserved',W.LPWSTR),('desktop',W.LPWSTR),('title',W.LPWSTR)]+[(n,W.DWORD) for n in ['x','y','xsize','ysize','xchars','ychars','fill','flags']]+[('show',W.WORD),('reserved2len',W.WORD),('reserved2',C.c_void_p),('hin',W.HANDLE),('hout',W.HANDLE),('herr',W.HANDLE)]
class PI(C.Structure):_fields_=[('process',W.HANDLE),('thread',W.HANDLE),('pid',W.DWORD),('tid',W.DWORD)]
K.CreateProcessW.argtypes=[W.LPCWSTR,W.LPWSTR,C.c_void_p,C.c_void_p,W.BOOL,W.DWORD,C.c_void_p,W.LPCWSTR,C.POINTER(SI),C.POINTER(PI)];K.CreateProcessW.restype=W.BOOL
K.WaitForSingleObject.argtypes=[W.HANDLE,W.DWORD];K.WaitForSingleObject.restype=W.DWORD
K.GetExitCodeProcess.argtypes=[W.HANDLE,C.POINTER(W.DWORD)];K.GetExitCodeProcess.restype=W.BOOL
K.CloseHandle.argtypes=[W.HANDLE];K.CloseHandle.restype=W.BOOL
si=SI();si.cb=C.sizeof(si);si.flags=1;si.show=0;pi=PI()
args=[PS,'-NoProfile','-ExecutionPolicy','Bypass','-File',str(ROOT/'semantic-tests.ps1'),'-Kind','interactive','-Result',str(ROOT/'interactive-results.json')]
command=C.create_unicode_buffer(subprocess.list2cmdline(args))
assert K.CreateProcessW(PS,command,None,None,False,0x10,None,str(ROOT),C.byref(si),C.byref(pi)),C.get_last_error()
assert K.CloseHandle(pi.thread)
wait=K.WaitForSingleObject(pi.process,30000);assert wait==0,('UNKNOWN_NO_KILL',wait)
code=W.DWORD();assert K.GetExitCodeProcess(pi.process,C.byref(code));assert K.CloseHandle(pi.process)
(ROOT/'semantic-process-results.json').write_text(json.dumps({'redirected_exit':r.returncode,'interactive_exit':code.value,'interactive_pid':pi.pid,'normal_wait_completed':True,'process_thread_references_closed':True,'CREATE_NEW_CONSOLE':True,'inherited_handles':False,'window':'Hidden fixture, no operator input/no LAB'},indent=2))
assert code.value==0,code.value
print('SEMANTIC_FIXTURES_PASS redirect/interact native console/UTF8; inspect transcript/loss outcomes separately')
