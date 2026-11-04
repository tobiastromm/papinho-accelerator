"""Pure candidate512-byte DATA codec. No ROOT8 writer, I/O or authority."""
import hashlib,importlib.util
from pathlib import Path
original=Path(__file__).parent/'isolated_protocol.py'
spec=importlib.util.spec_from_file_location('q4_record_data',original);module=importlib.util.module_from_spec(spec)
import sys
sys.modules[spec.name]=module;spec.loader.exec_module(module)
Record=module.Record;Denied=module.Denied

def encode(record):
 raw=record.encode();body=b'PACQ4D01'+raw[8:340]
 return body+hashlib.sha256(body).digest()+bytes(140)
def decode(component):
 if type(component)!=bytes or len(component)!=512 or component[:8]!=b'PACQ4D01' or component[372:]!=bytes(140):raise Denied('COMPONENT_SHAPE')
 if hashlib.sha256(component[:340]).digest()!=component[340:372]:raise Denied('COMPONENT_INTEGRITY')
 old=b'Q4DRAFT1'+component[8:340]
 return Record.decode(old+hashlib.sha256(old).digest())
