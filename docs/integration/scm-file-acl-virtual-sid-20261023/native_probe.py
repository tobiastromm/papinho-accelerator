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
root=Path(r'C:\Projetos\PapinhoAccelerator');target=root/'build/scm-protected-installation-binding-20261022/Selected/Release'
ph=K.CreateFileW(str(target),0x20080,1,None,3,0x2200000,None);assert ph!=w.HANDLE(-1).value
report={'parent':None,'files':[],'sid_api':[]}
try:
 pi,pid=identity(ph);assert (pi.volume,pi.indexHigh,pi.indexLow)==(417614100,1966080,79426);assert pi.attributes&0x410==0x10
 report['parent']={'identity':pid,'descriptor':descriptor(ph)}
 for leaf in ['papacc.conf','papacc_server.exe']:
  buf=c.create_unicode_buffer(leaf);u=US(len(leaf)*2,(len(leaf)+1)*2,c.cast(buf,w.LPWSTR));oa=OA(c.sizeof(OA),ph,c.pointer(u),0x1040,None,None);io=IO();fh=w.HANDLE()
  status=N.NtCreateFile(c.byref(fh),0x120081,c.byref(oa),c.byref(io),None,0,1,1,0x200060,None,0);assert status>=0,hex(status&0xffffffff)
  try:
   before,ids=identity(fh);assert not before.attributes&0x410
   sd=descriptor(fh);sha=hashlib.sha256();data=c.create_string_buffer(8192);n=D()
   while True:
    assert K.ReadFile(fh,data,len(data),c.byref(n),None)
    if not n.value:break
    sha.update(data.raw[:n.value])
   after,ids2=identity(fh);assert ids==ids2,'IDENTITY_CHANGED'
   report['files'].append({'path':str(target/leaf),'native_identity':ids,'relative_to_held_parent':True,'hash':sha.hexdigest(),'descriptor':sd,'closed':None})
  finally:
   ok=bool(K.CloseHandle(fh));assert ok;report['files'][-1]['closed']=ok
finally:report['parent_closed']=bool(K.CloseHandle(ph));assert report['parent_closed']
for name in [r'NT SERVICE\PapinhoAccelerator',r'NT SERVICE\EventLog']:
 for system in [None,'.']:
  sb=c.create_string_buffer(68);dn=c.create_unicode_buffer(256);sn=D(68);nn=D(256);use=c.c_int();c.set_last_error(0)
  ok=bool(A.LookupAccountNameW(system,name,sb,c.byref(sn),dn,c.byref(nn),c.byref(use)));err=c.get_last_error() if not ok else 0
  report['sid_api'].append({'api':'LookupAccountNameW','system':system,'name':name,'input_sid_bytes':68,'input_domain_chars':256,'success':ok,'win32_error':err,'sid_bytes':sn.value,'domain_chars':nn.value,'use':use.value,'sid':sid(c.cast(sb,P)) if ok else None,'domain':dn.value if ok else None})
out=root/'docs/integration/scm-file-acl-virtual-sid-20261023/native-readonly.json';out.write_text(json.dumps(report,indent=2))
print(json.dumps({'files':[{ 'path':f['path'],'identity':f['native_identity'],'hash':f['hash'],'first_failed':f['descriptor']['first_failed_predicate']} for f in report['files']],'sid_api':report['sid_api']},indent=2))
