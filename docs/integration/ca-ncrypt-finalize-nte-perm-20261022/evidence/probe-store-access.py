import ctypes as c,json
from ctypes import wintypes as w
k=c.WinDLL('kernel32',use_last_error=True);a=c.WinDLL('advapi32',use_last_error=True)
k.GetCurrentProcess.restype=w.HANDLE;k.CloseHandle.argtypes=[w.HANDLE];k.CloseHandle.restype=w.BOOL;k.LocalFree.argtypes=[c.c_void_p];k.LocalFree.restype=c.c_void_p
a.OpenProcessToken.argtypes=[w.HANDLE,w.DWORD,c.POINTER(w.HANDLE)];a.OpenProcessToken.restype=w.BOOL
a.DuplicateToken.argtypes=[w.HANDLE,c.c_int,c.POINTER(w.HANDLE)];a.DuplicateToken.restype=w.BOOL
a.GetNamedSecurityInfoW.argtypes=[w.LPWSTR,c.c_int,w.DWORD,c.c_void_p,c.c_void_p,c.c_void_p,c.c_void_p,c.POINTER(c.c_void_p)];a.GetNamedSecurityInfoW.restype=w.DWORD
class MAP(c.Structure):_fields_=[('read',w.DWORD),('write',w.DWORD),('execute',w.DWORD),('all',w.DWORD)]
a.AccessCheck.argtypes=[c.c_void_p,w.HANDLE,w.DWORD,c.POINTER(MAP),c.c_void_p,c.POINTER(w.DWORD),c.POINTER(w.DWORD),c.POINTER(w.BOOL)];a.AccessCheck.restype=w.BOOL
h=w.HANDLE();imp=w.HANDLE();sd=c.c_void_p();result={'path':r'C:\ProgramData\Microsoft\Crypto\Keys','operation':'READ_SD_AND_ACCESSCHECK_FILE_ADD_FILE_ONLY','thread_impersonation':'NOT_SET'}
try:
 assert a.OpenProcessToken(k.GetCurrentProcess(),10,c.byref(h)),c.get_last_error()
 assert a.DuplicateToken(h,2,c.byref(imp)),c.get_last_error()
 status=a.GetNamedSecurityInfoW(result['path'],1,7,None,None,None,None,c.byref(sd));assert status==0,status
 mapping=MAP(0x120089,0x120116,0x1200a0,0x1f01ff);priv=c.create_string_buffer(512);size=w.DWORD(512);granted=w.DWORD();allowed=w.BOOL()
 assert a.AccessCheck(sd,imp,2,c.byref(mapping),priv,c.byref(size),c.byref(granted),c.byref(allowed)),c.get_last_error()
 result['desired_access']=2;result['granted_access']=granted.value;result['allowed']=bool(allowed.value)
finally:
 if sd:result['sd_free_confirmed']=not bool(k.LocalFree(sd))
 if imp:result['duplicate_close_confirmed']=bool(k.CloseHandle(imp))
 if h:result['token_close_confirmed']=bool(k.CloseHandle(h))
print(json.dumps(result,indent=2));assert all(result.get(x,False) for x in ['sd_free_confirmed','duplicate_close_confirmed','token_close_confirmed'])

