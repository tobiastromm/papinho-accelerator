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

A.GetLengthSid.argtypes=[P];A.GetLengthSid.restype=D
rows=[]
for capacity in [68,32]:
 sb=c.create_string_buffer(68);dn=c.create_unicode_buffer(256);sn=D(capacity);nn=D(256);use=c.c_int();c.set_last_error(0)
 ok=bool(A.LookupAccountNameW(None,r'NT SERVICE\EventLog',sb,c.byref(sn),dn,c.byref(nn),c.byref(use)));e=c.get_last_error() if not ok else 0
 length=A.GetLengthSid(sb) if ok else None
 rows.append({'name':r'NT SERVICE\EventLog','input_capacity':capacity,'success':ok,'returned_cbSid':sn.value,'GetLengthSid':length,'win32':e,'existing_adapter_size_equality_pass':ok and length==sn.value,'proposed_bounded_length_pass':ok and length<=sn.value<=68})
Path(r'C:\Projetos\PapinhoAccelerator\docs\integration\scm-file-acl-virtual-sid-20261023\sid-buffer-contract.json').write_text(json.dumps(rows,indent=2));print(json.dumps(rows,indent=2))
