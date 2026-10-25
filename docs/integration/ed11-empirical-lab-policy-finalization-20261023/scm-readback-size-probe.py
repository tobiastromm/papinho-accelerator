"""Read-only paired cbBufSize diagnosis. No SCM mutators or service start."""
import ctypes,json,datetime
from ctypes import wintypes as W
class CONFIG(ctypes.Structure):
 _fields_=[('type',W.DWORD),('startup',W.DWORD),('error_control',W.DWORD),('image',W.LPVOID),('group',W.LPVOID),('tag',W.DWORD),('dependencies',W.LPVOID),('account',W.LPVOID),('display',W.LPVOID)]
class STATUS(ctypes.Structure):
 _fields_=[(s,W.DWORD) for s in ['type','state','controls','win32_exit','specific_exit','checkpoint','wait_hint','pid','flags']]
class BUFFER(ctypes.Union):
 _fields_=[('align',CONFIG),('data',ctypes.c_byte*16384)]
def main():
 a=ctypes.WinDLL('advapi32',use_last_error=True)
 a.OpenSCManagerW.argtypes=[W.LPCWSTR,W.LPCWSTR,W.DWORD];a.OpenSCManagerW.restype=W.HANDLE
 a.OpenServiceW.argtypes=[W.HANDLE,W.LPCWSTR,W.DWORD];a.OpenServiceW.restype=W.HANDLE
 a.QueryServiceConfigW.argtypes=[W.HANDLE,W.LPVOID,W.DWORD,ctypes.POINTER(W.DWORD)];a.QueryServiceConfigW.restype=W.BOOL
 a.QueryServiceConfig2W.argtypes=[W.HANDLE,W.DWORD,W.LPVOID,W.DWORD,ctypes.POINTER(W.DWORD)];a.QueryServiceConfig2W.restype=W.BOOL
 a.QueryServiceStatusEx.argtypes=[W.HANDLE,W.DWORD,W.LPVOID,W.DWORD,ctypes.POINTER(W.DWORD)];a.QueryServiceStatusEx.restype=W.BOOL
 a.CloseServiceHandle.argtypes=[W.HANDLE];a.CloseServiceHandle.restype=W.BOOL
 out={'utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'service':'PapinhoAcceleratorLab20261023','access':{'manager':1,'service':5},'allocation':16384,'calls':[],'errors':[],'close_errors':[],'struct_size':ctypes.sizeof(CONFIG),'offsets':{s:getattr(CONFIG,s).offset for s,_ in CONFIG._fields_},'scm_mutations':'NONE','start_executed':False};manager=service=None;buf=BUFFER();used=W.DWORD()
 try:
  assert ctypes.sizeof(CONFIG)==64 and CONFIG.image.offset==16 and CONFIG.account.offset==48
  manager=a.OpenSCManagerW(None,None,1)
  if not manager:raise ctypes.WinError(ctypes.get_last_error())
  service=a.OpenServiceW(manager,out['service'],5)
  if not service:raise ctypes.WinError(ctypes.get_last_error())
  status=STATUS()
  if not a.QueryServiceStatusEx(service,0,ctypes.byref(status),ctypes.sizeof(status),ctypes.byref(used)):raise ctypes.WinError(ctypes.get_last_error())
  out['status_before']={s:getattr(status,s) for s,_ in STATUS._fields_}
  for size in [16384,8192]:
   ctypes.memset(ctypes.byref(buf),0,ctypes.sizeof(buf));used.value=0;ctypes.set_last_error(0)
   ok=a.QueryServiceConfigW(service,ctypes.byref(buf),size,ctypes.byref(used));err=0 if ok else ctypes.get_last_error()
   row={'api':'QueryServiceConfigW','cbBufSize':size,'success':bool(ok),'win32':err,'bytes_needed':used.value};out['calls'].append(row)
   if ok:
    cfg=buf.align;base=ctypes.addressof(buf)
    def text(addr):
     if not addr:return None
     if addr<base or addr>=base+size or (addr-base)%2:raise RuntimeError('STRING_POINTER_OUT_OF_BOUNDS')
     value=ctypes.wstring_at(addr,(base+size-addr)//2);nul=value.find('\0')
     if nul<0:raise RuntimeError('UNTERMINATED_STRING')
     return value[:nul]
    row['config']={'type':cfg.type,'startup':cfg.startup,'error_control':cfg.error_control,'image':text(cfg.image),'account':text(cfg.account),'display':text(cfg.display)}
  sid=W.DWORD();used.value=0
  if a.QueryServiceConfig2W(service,5,ctypes.byref(sid),ctypes.sizeof(sid),ctypes.byref(used)):out['sid_type']=sid.value
  else:out['sid_type_error']=ctypes.get_last_error()
 except Exception as ex:out['errors'].append(str(ex))
 finally:
  for name,h in [('service',service),('manager',manager)]:
   if h and not a.CloseServiceHandle(h):out['close_errors'].append({'owner':name,'win32':ctypes.get_last_error(),'disposition':'UNKNOWN_NO_RETRY'})
 print(json.dumps(out,indent=2));return 0 if not out['errors'] and not out['close_errors'] and len(out['calls'])==2 and out['calls'][0]['win32']==1783 and out['calls'][1]['success'] else 2
if __name__=='__main__':raise SystemExit(main())
