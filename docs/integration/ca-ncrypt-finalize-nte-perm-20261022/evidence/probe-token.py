import ctypes as c,json
from ctypes import wintypes as w
k=c.WinDLL('kernel32',use_last_error=True);a=c.WinDLL('advapi32',use_last_error=True)
k.GetCurrentProcess.restype=w.HANDLE;k.CloseHandle.argtypes=[w.HANDLE];k.CloseHandle.restype=w.BOOL
k.GetCurrentThread.restype=w.HANDLE
a.OpenProcessToken.argtypes=[w.HANDLE,w.DWORD,c.POINTER(w.HANDLE)];a.OpenProcessToken.restype=w.BOOL
a.OpenThreadToken.argtypes=[w.HANDLE,w.DWORD,w.BOOL,c.POINTER(w.HANDLE)];a.OpenThreadToken.restype=w.BOOL
a.GetTokenInformation.argtypes=[w.HANDLE,c.c_int,c.c_void_p,w.DWORD,c.POINTER(w.DWORD)];a.GetTokenInformation.restype=w.BOOL
a.ConvertSidToStringSidW.argtypes=[c.c_void_p,c.POINTER(c.c_void_p)];a.ConvertSidToStringSidW.restype=w.BOOL
k.LocalFree.argtypes=[c.c_void_p];k.LocalFree.restype=c.c_void_p
class LUID(c.Structure):_fields_=[('LowPart',w.DWORD),('HighPart',w.LONG)]
class STATS(c.Structure):_fields_=[('TokenId',LUID),('AuthenticationId',LUID),('ExpirationTime',c.c_longlong),('TokenType',w.DWORD),('ImpersonationLevel',w.DWORD),('DynamicCharged',w.DWORD),('DynamicAvailable',w.DWORD),('GroupCount',w.DWORD),('PrivilegeCount',w.DWORD),('ModifiedId',LUID)]
def info(h,n,obj):
 size=w.DWORD();assert a.GetTokenInformation(h,n,c.byref(obj),c.sizeof(obj),c.byref(size)),(n,c.get_last_error());return obj
h=w.HANDLE();assert a.OpenProcessToken(k.GetCurrentProcess(),8,c.byref(h)),c.get_last_error()
thread=w.HANDLE();has_thread=bool(a.OpenThreadToken(k.GetCurrentThread(),8,True,c.byref(thread)));thread_error=0 if has_thread else c.get_last_error()
assert has_thread or thread_error==1008,thread_error
result={'thread_token_present':has_thread,'thread_token_error':thread_error}
try:
 user=info(h,1,c.create_string_buffer(512));sid=c.c_void_p.from_buffer(user).value;out=c.c_void_p();assert a.ConvertSidToStringSidW(sid,c.byref(out)),c.get_last_error()
 try:result['sid']=c.wstring_at(out.value)
 finally:assert not k.LocalFree(out),'SID_LOCALFREE_UNKNOWN'
 result['elevation_type']=info(h,18,w.DWORD()).value;result['elevated']=bool(info(h,20,w.DWORD()).value);result['session']=info(h,12,w.DWORD()).value
 stats=info(h,10,STATS());result['statistics']={n: [getattr(stats,n).HighPart,getattr(stats,n).LowPart] for n in ['TokenId','AuthenticationId','ModifiedId']}
finally:
 if has_thread:result['thread_token_close']=bool(k.CloseHandle(thread))
 result['process_token_close']=bool(k.CloseHandle(h))
print(json.dumps(result,indent=2));assert result['process_token_close'] and (not has_thread or result['thread_token_close'])
