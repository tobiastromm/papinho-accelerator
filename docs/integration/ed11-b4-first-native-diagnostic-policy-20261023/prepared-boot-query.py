"""PREPARED ONLY: one native read-only System boot-event query, no PowerShell module."""
import ctypes,hashlib,json,xml.etree.ElementTree as ET
from ctypes import wintypes as W

def main():
 api=ctypes.WinDLL('wevtapi',use_last_error=True)
 api.EvtQuery.argtypes=[W.HANDLE,W.LPCWSTR,W.LPCWSTR,W.DWORD];api.EvtQuery.restype=W.HANDLE
 api.EvtNext.argtypes=[W.HANDLE,W.DWORD,ctypes.POINTER(W.HANDLE),W.DWORD,W.DWORD,ctypes.POINTER(W.DWORD)];api.EvtNext.restype=W.BOOL
 api.EvtRender.argtypes=[W.HANDLE,W.HANDLE,W.DWORD,W.DWORD,W.LPVOID,ctypes.POINTER(W.DWORD),ctypes.POINTER(W.DWORD)];api.EvtRender.restype=W.BOOL
 api.EvtClose.argtypes=[W.HANDLE];api.EvtClose.restype=W.BOOL
 query=None;event=W.HANDLE();result={'scope':'ONE_READ_ONLY_NATIVE_BOOT_IDENTITY_QUERY','start_executed':False,'errors':[],'close_errors':[]}
 try:
  query=api.EvtQuery(None,'System',"*[System[Provider[@Name='Microsoft-Windows-Kernel-General'] and EventID=12]]",0x201)
  if not query:raise ctypes.WinError(ctypes.get_last_error())
  returned=W.DWORD()
  if not api.EvtNext(query,1,ctypes.byref(event),0,0,ctypes.byref(returned)):raise ctypes.WinError(ctypes.get_last_error())
  if returned.value!=1:raise RuntimeError('NOT_ONE_EVENT')
  used=W.DWORD();props=W.DWORD()
  ok=api.EvtRender(None,event,1,0,None,ctypes.byref(used),ctypes.byref(props));err=ctypes.get_last_error()
  if ok or err!=122 or used.value<2 or used.value>65536:raise RuntimeError('RENDER_SIZE_OR_STATUS_INVALID '+str(err))
  size=used.value;buf=ctypes.create_unicode_buffer((size+1)//2)
  if not api.EvtRender(None,event,1,size,buf,ctypes.byref(used),ctypes.byref(props)):raise ctypes.WinError(ctypes.get_last_error())
  x=ET.fromstring(buf.value);ns={'e':'http://schemas.microsoft.com/win/2004/08/events/event'}
  machine=x.findtext('e:System/e:Computer',namespaces=ns);record=int(x.findtext('e:System/e:EventRecordID',namespaces=ns))
  data={v.attrib.get('Name'):v.text for v in x.findall('e:EventData/e:Data',ns)}
  identity=hashlib.sha256(machine.encode('utf-8')).hexdigest()
  result.update(record=record,computer_sha256=identity,data=data)
  result['expected_host_match']=identity=='c8d6ef2a8acad155f446bbe54499817b21207b15877ec80b0db329e486ae0e01'
  result['expected_record_match']=record==236441
  result['expected_start_match']=data.get('StartTime')=='2026-10-23T18:36:34.5000000Z'
 except Exception as ex:result['errors'].append(str(ex))
 finally:
  for name,h in [('event',event),('query',query)]:
   if h and not api.EvtClose(h):result['close_errors'].append({'owner':name,'win32':ctypes.get_last_error(),'disposition':'UNKNOWN_NO_RETRY'})
 print(json.dumps(result,indent=2))
 return 0 if not result['errors'] and not result['close_errors'] and all(result.get(k) is True for k in ['expected_host_match','expected_record_match','expected_start_match']) else 2
if __name__=='__main__':raise SystemExit(main())
