import ctypes as c,ctypes.wintypes as w,json,hashlib,sys,subprocess
from pathlib import Path
K=c.WinDLL('kernel32',use_last_error=True);A=c.WinDLL('advapi32',use_last_error=True);N=c.WinDLL('ntdll',use_last_error=True)
P=c.c_void_p;D=w.DWORD
class Info(c.Structure):
 _fields_=[('attributes',D),('creation',w.FILETIME),('access',w.FILETIME),('write',w.FILETIME),('volume',D),('sizeHigh',D),('sizeLow',D),('links',D),('indexHigh',D),('indexLow',D)]
class US(c.Structure): _fields_=[('Length',w.USHORT),('MaximumLength',w.USHORT),('Buffer',w.LPWSTR)]
class OA(c.Structure): _fields_=[('Length',D),('RootDirectory',w.HANDLE),('ObjectName',c.POINTER(US)),('Attributes',D),('SD',P),('QOS',P)]
class IO(c.Structure): _fields_=[('Status',P),('Information',c.c_size_t)]
K.CreateFileW.argtypes=[w.LPCWSTR,D,D,P,D,D,w.HANDLE];K.CreateFileW.restype=w.HANDLE
K.GetFileInformationByHandle.argtypes=[w.HANDLE,c.POINTER(Info)];K.CloseHandle.argtypes=[w.HANDLE]
K.ReadFile.argtypes=[w.HANDLE,P,D,c.POINTER(D),P];K.LocalFree.argtypes=[P];K.LocalFree.restype=P
A.GetSecurityInfo.argtypes=[w.HANDLE,c.c_int,D,c.POINTER(P),c.POINTER(P),c.POINTER(P),c.POINTER(P),c.POINTER(P)];A.GetSecurityInfo.restype=D
A.GetSecurityDescriptorLength.argtypes=[P];A.GetSecurityDescriptorLength.restype=D
A.GetSecurityDescriptorControl.argtypes=[P,c.POINTER(w.WORD),c.POINTER(D)]
A.ConvertSidToStringSidW.argtypes=[P,c.POINTER(P)]
A.ConvertSecurityDescriptorToStringSecurityDescriptorW.argtypes=[P,D,D,c.POINTER(P),c.POINTER(D)]
A.GetAce.argtypes=[P,D,c.POINTER(P)];A.IsValidSid.argtypes=[P]
A.LookupAccountNameW.argtypes=[w.LPCWSTR,w.LPCWSTR,P,c.POINTER(D),w.LPWSTR,c.POINTER(D),c.POINTER(c.c_int)]
N.NtCreateFile.argtypes=[c.POINTER(w.HANDLE),D,c.POINTER(OA),c.POINTER(IO),P,D,D,D,D,P,D];N.NtCreateFile.restype=c.c_long
freed=[]
def release(p):
 if p: assert not K.LocalFree(p),'LOCALFREE_UNCERTAIN'
def sid(p):
 if not p:return None
 assert A.IsValidSid(p)
 out=P();assert A.ConvertSidToStringSidW(p,c.byref(out))
 try:return c.wstring_at(out)
 finally:release(out)
def identity(h):
 i=Info();assert K.GetFileInformationByHandle(h,c.byref(i))
 return i,{'volume':i.volume,'indexHigh':i.indexHigh,'indexLow':i.indexLow,'attributes':i.attributes,'size':i.sizeLow+(i.sizeHigh<<32),'writeHigh':i.write.dwHighDateTime,'writeLow':i.write.dwLowDateTime}
def descriptor(h):
 o=P();g=P();d=P();sd=P();e=A.GetSecurityInfo(h,1,7,c.byref(o),c.byref(g),c.byref(d),None,c.byref(sd));assert e==0,e
 try:
  owner=sid(o);group=sid(g);out=P();ln=D();assert A.ConvertSecurityDescriptorToStringSecurityDescriptorW(sd,1,7,c.byref(out),c.byref(ln))
  try:sddl=c.wstring_at(out)
  finally:release(out)
  control=w.WORD();revision=D();assert A.GetSecurityDescriptorControl(sd,c.byref(control),c.byref(revision))
  aces=[]
  if d:
   count=c.c_ushort.from_address(d.value+4).value
   assert count<=4096
   for idx in range(count):
    a=P();assert A.GetAce(d,idx,c.byref(a));header=c.string_at(a,4);size=int.from_bytes(header[2:4],'little');assert size>=16
    aces.append({'index':idx,'type':header[0],'flags':header[1],'inherited':bool(header[1]&16),'inherit_only':bool(header[1]&8),'mask':hex(D.from_address(a.value+4).value),'sid':sid(P(a.value+8))})
  allowed=['S-1-5-21-785699550-1440458426-1682014700-1108','S-1-5-18','S-1-5-32-544']
  first=None
  if owner not in allowed:first='OWNER_NOT_OPERATOR_SYSTEM_ADMINISTRATORS'
  elif not d:first='NULL_DACL'
  else:
   for a in aces:
    if a['inherit_only']:continue
    if a['type'] not in (0,1):first='ACE_TYPE_'+str(a['index']);break
    if a['type']==0 and int(a['mask'],16)&0x500d0156 and a['sid'] not in allowed:first='FOREIGN_WRITER_ACE_'+str(a['index']);break
  n=A.GetSecurityDescriptorLength(sd);assert 0<n<=65536
  return {'owner':owner,'group':group,'control':hex(control.value),'sddl':sddl,'sd_sha256':hashlib.sha256(c.string_at(sd,n)).hexdigest(),'aces':aces,'first_failed_predicate':first}
 finally:release(sd)

A.LsaOpenPolicy.argtypes=[P,c.POINTER(OA),D,c.POINTER(w.HANDLE)];A.LsaOpenPolicy.restype=c.c_long
A.LsaLookupNames2.argtypes=[w.HANDLE,D,D,c.POINTER(US),c.POINTER(P),c.POINTER(P)];A.LsaLookupNames2.restype=c.c_long
A.LsaNtStatusToWinError.argtypes=[c.c_long];A.LsaNtStatusToWinError.restype=D
A.LsaClose.argtypes=[w.HANDLE];A.LsaClose.restype=c.c_long
A.LsaFreeMemory.argtypes=[P];A.LsaFreeMemory.restype=c.c_long
attrs=OA();attrs.Length=c.sizeof(OA);h=w.HANDLE();st=A.LsaOpenPolicy(None,c.byref(attrs),0x800,c.byref(h));assert st==0,hex(st&0xffffffff)
rows=[]
try:
 for name in [r'NT SERVICE\PapinhoAccelerator',r'NT SERVICE\EventLog']:
  b=c.create_unicode_buffer(name);u=US(len(name)*2,(len(name)+1)*2,c.cast(b,w.LPWSTR));domains=P();sids=P()
  st=A.LsaLookupNames2(h,0,1,c.byref(u),c.byref(domains),c.byref(sids))
  rows.append({'api':'LsaLookupNames2','policy_system':None,'policy_access':'POLICY_LOOKUP_NAMES','flags':0,'count':1,'name':name,'ntstatus':hex(st&0xffffffff),'mapped_win32':A.LsaNtStatusToWinError(st)})
  for mem in [domains,sids]:
   if mem:assert A.LsaFreeMemory(mem)==0,'LSA_FREE_UNCERTAIN'
finally:assert A.LsaClose(h)==0,'LSA_CLOSE_UNCERTAIN'
Path(r'C:\Projetos\PapinhoAccelerator\docs\integration\scm-file-acl-virtual-sid-20261023\lsa-readonly.json').write_text(json.dumps(rows,indent=2))
print(json.dumps(rows,indent=2))
