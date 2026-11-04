"""Executable isolated worker: synthetic files only, not canonical root publication.
The simulated NV side is a second fixture file, NOT a new authority or a real independent witness.
"""
from pathlib import Path
from dataclasses import replace
import hashlib,json,os,struct,sys,msvcrt,threading
from isolated_protocol import Record,Denied
FRAME=struct.Struct('>4sI32s')
class Owner:
 def __init__(self,path):
  allowed=Path(__file__).resolve().parent/'runs'
  if not Path(path).resolve().is_relative_to(allowed.resolve()) or Path(path).is_symlink():raise Denied('FIXTURE_PATH_ONLY')
  self.writer=threading.get_ident();self.file=open(path,'r+b',buffering=0);self.path=Path(path);self.closed=False;self.first=None;self.retained=False
  try:self.file.seek(0);msvcrt.locking(self.file.fileno(),msvcrt.LK_NBLCK,1)
  except Exception:self.file.close();raise Denied('BUSY')
  self.identity=(os.fstat(self.file.fileno()).st_dev,os.fstat(self.file.fileno()).st_ino)
 def original(self):
  st=self.path.stat();return (st.st_dev,st.st_ino)==self.identity
 def bytes(self):
  if threading.get_ident()!=self.writer:raise Denied('FOREIGN_OWNER')
  if not self.original():raise Denied('OBJECT_CHANGED')
  self.file.seek(0);return self.file.read()
 def close(self,uncertain=False):
  if threading.get_ident()!=self.writer:return False
  if self.closed or self.retained:return False
  if uncertain:self.retained=True;self.first=self.first or 'CLOSE_UNKNOWN';return False
  self.file.close();self.closed=True;return True

def parse(raw):
 if raw[:1]!=b'R':raise Denied('ROOT_MAGIC')
 pos=1;previous=bytes(32);records=[]
 while pos<len(raw):
  if pos+FRAME.size>len(raw):raise Denied('TORN_HEADER')
  magic,size,parent=FRAME.unpack(raw[pos:pos+FRAME.size]);pos+=FRAME.size
  if magic!=b'Q4CF' or size!=372 or parent!=previous or pos+size>len(raw):raise Denied('FRAME')
  r=Record.decode(raw[pos:pos+size]);frame=raw[pos-FRAME.size:pos+size];pos+=size
  if records:
   old=records[-1]
   if replace(r,state=old.state,first=old.first)!=old:raise Denied('BINDING_CHANGE')
   if (old.state,r.state) not in [('PREPARED','CONSUMED'),('CONSUMED','RESULT'),('PREPARED','UNKNOWN'),('CONSUMED','UNKNOWN')]:raise Denied('TRANSITION')
  elif r.state!='PREPARED':raise Denied('INITIAL_STATE')
  records.append(r);previous=hashlib.sha256(frame).digest()
 return records,previous

class DurableWorker:
 def __init__(self,root,witness,record):
  self.root=Owner(root)
  try:self.witness=Owner(witness)
  except Exception:self.root.close();raise
  self.writer=threading.get_ident();self.last_root=self.root.bytes();self.record=record;self.used=False;self.first=None;self.retained=False;self.read_slot=False
 def fail(self,error):
  self.first=self.first or error;self.retained=True;return False
 def append(self,record,phase):
  old=self.root.bytes()
  if old!=self.last_root:raise Denied('CAS_PREIMAGE')
  records,previous=parse(old)
  candidate=old+FRAME.pack(b'Q4CF',372,previous)+record.encode();parse(candidate)
  self.root.file.seek(0,2)
  if os.environ.get('Q4_FAULT')=='torn_'+phase:
   self.root.file.write(candidate[len(old):len(old)+17]);os.fsync(self.root.file.fileno());os._exit(90)
  frame=candidate[len(old):]
  if self.root.file.write(frame)!=len(frame):raise Denied('SHORT_WRITE')
  os.fsync(self.root.file.fileno())
  if self.root.bytes()!=candidate:raise Denied('PERSIST_READBACK')
  self.last_root=candidate
  crash('after_'+phase)
 def counter(self):
  data=self.witness.bytes()
  if len(data)!=72 or data[:32]!=self.record.bindings[4] or hashlib.sha256(data[:40]).digest()!=data[40:]:raise Denied('WITNESS_DATA')
  return struct.unpack('>Q',data[32:40])[0]
 def increment_simulated(self):
  value=self.counter()
  if value!=self.record.baseline or value==(1<<64)-1:raise Denied('BASELINE')
  data=self.record.bindings[4]+struct.pack('>Q',value+1);data+=hashlib.sha256(data).digest()
  self.witness.file.seek(0)
  if self.witness.file.write(data)!=len(data):raise Denied('WITNESS_SHORT')
  os.fsync(self.witness.file.fileno());crash('after_increment')
  if os.environ.get('Q4_FAULT')=='ambiguous_increment':raise Denied('INCREMENT_UNKNOWN')
  if self.counter()!=self.record.expected:raise Denied('COUNTER_READBACK')
 def run(self):
  if threading.get_ident()!=self.writer:return False
  if self.used:return False
  self.used=True
  try:
   current=self.root.bytes()
   if current!=self.last_root:raise Denied('CAS_PREIMAGE')
   records,_=parse(current)
   if records:raise Denied('RESTART_OR_REPLAY') # even PREPARED cannot be adopted
   if self.record.state!='PREPARED' or self.counter()!=self.record.baseline:raise Denied('BASELINE')
   crash('before_prepare');self.append(self.record,'prepare')
   if os.environ.get('Q4_FAULT')=='stop':raise Denied('STOP')
   self.increment_simulated()
   self.record=replace(self.record,state='CONSUMED');self.append(self.record,'commit')
   if self.counter()!=self.record.expected:raise Denied('FINAL_WITNESS')
   # The only read is a model boolean; no protected reader exists in this executable.
   self.read_slot=True;crash('after_read_slot')
   self.record=replace(self.record,state='RESULT');self.append(self.record,'result');return True
  except Exception as e:
   self.fail(str(e))
   # Record a definitive runtime failure only when the existing frame chain is intact.
   # Never retry a failed/torn persistence operation or a hardware command.
   if self.first in ('STOP','INCREMENT_UNKNOWN','BASELINE','COUNTER_READBACK','FINAL_WITNESS'):
    try:
     records,_=parse(self.root.bytes())
     if records and records[-1].state in ('PREPARED','CONSUMED'):
      self.append(replace(records[-1],state='UNKNOWN',first=255),'unknown')
    except Exception:pass # original first error and retained state remain authoritative
   return False
 def close(self):
  if os.environ.get('Q4_FAULT')=='close_unknown':self.root.close(True);self.witness.close(True);return self.fail('CLOSE_UNKNOWN')
  a=self.witness.close();b=self.root.close();return a and b

def crash(at):
 if os.environ.get('Q4_FAULT')==at:os._exit(90)

def main():
 # Exact guarded opt-in and fixture paths; no operational-path option.
 if len(sys.argv)!=5 or sys.argv[1]!='--synthetic-only':return 2
 folder=Path(sys.argv[2]).resolve();allowed=Path(__file__).resolve().parent/'runs'
 if not folder.is_relative_to(allowed.resolve()):raise Denied('FIXTURE_PATH_ONLY')
 record_path=Path(sys.argv[3]).resolve()
 if not record_path.is_relative_to(folder):raise Denied('FIXTURE_RECORD_PATH_ONLY')
 record=Record.decode(record_path.read_bytes())
 if sys.argv[4]!='SIMULATED_NO_AUTHORITY':return 2
 w=None
 try:
  w=DurableWorker(folder/'root.fixture',folder/'nv.SIMULATED.fixture',record);ok=w.run();close=w.close()
  print(json.dumps({'ok':ok,'first':w.first,'close':close,'retained':w.retained,'model_read_slot':w.read_slot,'Q4':False,'TPM':'NOT_ACCESSED','authority':'DENY'}));return 0 if ok and close else 2
 except Exception as e:
  print(json.dumps({'ok':False,'first':str(e),'authority':'DENY'}));return 2
if __name__=='__main__':sys.exit(main())
