"""Isolated candidate protocol. No disk ledger, TPM transport, root publication or authority.
Fixed wire format is a proposed documentary component, NOT ROOT8/issuer bytes.
"""
from dataclasses import dataclass, replace
import hashlib, hmac, struct, threading

MAGIC=b'Q4DRAFT1'
FIELDS=('root','predecessor','approval','history','device_generation','context','objects','artifacts','custody')
STATES={'PREPARED':1,'CONSUMED':2,'RESULT':3,'UNKNOWN':4}
ERRORS={name:n+1 for n,name in enumerate(('IDENTITY','WITNESS_BASELINE','PREPARE_PERSIST','INCREMENT_UNKNOWN','COUNTER_DIVERGED','COMMIT_UNKNOWN','CALLBACK_UNKNOWN','LEASE','WITNESS_CURRENT','STOP','CLOSE_UNKNOWN'))}
HEADER=struct.Struct('>8sBBHQQQQQ')
MAX=(1<<64)-1
class Denied(Exception): pass

@dataclass(frozen=True)
class Record:
    bindings: tuple
    baseline: int
    expected: int
    pid: int
    creation: int
    state: str='PREPARED'
    first: int=0
    def encode(self):
        if len(self.bindings)!=len(FIELDS) or any(type(x)!=bytes or len(x)!=32 or x==bytes(32) for x in self.bindings):
            raise Denied('BINDING')
        if self.state not in STATES or not 0<=self.baseline<MAX or self.expected!=self.baseline+1:
            raise Denied('COUNTER_OR_STATE')
        if not 0<self.pid<=0xffffffff or not 0<self.creation<=MAX or not 0<=self.first<=MAX:
            raise Denied('IDENTITY_OR_FIRST')
        body=HEADER.pack(MAGIC,1,STATES[self.state],0,self.baseline,self.expected,self.pid,self.creation,self.first)+b''.join(self.bindings)
        return body+hashlib.sha256(body).digest()
    @classmethod
    def decode(cls,raw):
        if type(raw)!=bytes or len(raw)!=HEADER.size+len(FIELDS)*32+32: raise Denied('SIZE')
        body,checksum=raw[:-32],raw[-32:]
        if not hmac.compare_digest(hashlib.sha256(body).digest(),checksum): raise Denied('CONTENT')
        m,v,s,res,b,n,p,c,f=HEADER.unpack(body[:HEADER.size])
        if m!=MAGIC or v!=1 or res!=0 or s not in STATES.values(): raise Denied('VERSION')
        bindings=tuple(body[i:i+32] for i in range(HEADER.size,len(body),32))
        r=cls(bindings,b,n,p,c,next(k for k,val in STATES.items() if val==s),f)
        if r.encode()!=raw: raise Denied('CANONICAL')
        return r

class SimulatedWitness:
    """Non-restorable witness assumption for model ONLY. No native APIs."""
    def __init__(self,value,identity): self.value=value;self.identity=identity;self.commands=0
    def increment(self):
        self.commands+=1
        if self.value>=MAX: raise Denied('EXHAUSTED')
        self.value+=1
        return self.value

class Invocation:
    def __init__(self,record):
        record.encode()
        if record.state!='PREPARED': raise Denied('RESTART_SPENT')
        self.record=record;self.first=None;self.frozen=False;self.busy=threading.Lock()
        self.live=True;self.read=False;self.issued=False;self.closed=False;self.retained=False
    def fail(self,error):
        if self.first is None:self.first=error
        self.frozen=True;self.retained=True
        self.record=replace(self.record,state='UNKNOWN',first=self.record.first or ERRORS.get(self.first,255))
        return False
    def consume(self,witness,persist_prepare,persist_consumed,authenticated_identity):
        if not self.busy.acquire(False): return False
        try:
            if self.frozen or not self.live or self.issued:return False
            # Before irreversible command: persist exact prepared bytes in same-owner CAS.
            # Callbacks here are simulated, never accepted as deployed durable storage.
            self.issued=True
            if authenticated_identity!=self.record.bindings[FIELDS.index('device_generation')]: return self.fail('IDENTITY')
            if witness.identity!=authenticated_identity or witness.value!=self.record.baseline:return self.fail('WITNESS_BASELINE')
            if not persist_prepare(self.record.encode()): return self.fail('PREPARE_PERSIST')
            if self.frozen or not self.live:return False
            if witness.value!=self.record.baseline or witness.identity!=authenticated_identity:return self.fail('WITNESS_BASELINE')
            try: value=witness.increment()
            except Exception:return self.fail('INCREMENT_UNKNOWN')
            if value!=self.record.expected:return self.fail('COUNTER_DIVERGED')
            consumed=replace(self.record,state='CONSUMED')
            if not persist_consumed(consumed.encode()):return self.fail('COMMIT_UNKNOWN')
            if self.frozen or not self.live:return False
            self.record=consumed
            return True
        except Exception:return self.fail('CALLBACK_UNKNOWN')
        finally:self.busy.release()
    def read_once(self,bindings,pid,creation,witness):
        if not self.busy.acquire(False):return False
        try:
            if self.frozen or self.read or not self.live or self.record.state!='CONSUMED':return False
            if bindings!=self.record.bindings or (pid,creation)!=(self.record.pid,self.record.creation):return self.fail('LEASE')
            if witness.identity!=self.record.bindings[4] or witness.value!=self.record.expected:return self.fail('WITNESS_CURRENT')
            self.read=True
            return True # MODEL_READ_SLOT_ONLY, never operational permission
        finally:self.busy.release()
    def stop(self):self.live=False;self.fail('STOP')
    def close(self,confirmed):
        if self.closed or self.retained:return False # uncertain reference is never retried
        if not confirmed:return self.fail('CLOSE_UNKNOWN')
        self.closed=True;self.live=False;return True

class ActivationReplyFixture:
    """Tests bounded ActivateCredential TPM2B_DIGEST parameter parsing only.
    TPM session response authentication/decryption must precede this parser in a real adapter.
    This fixture has no transport or hardware-proof result, and no replay persistence.
    """
    def __init__(self,expected,context):
        if len(expected) not in (30,32) or len(context)!=32:raise Denied('SHAPE')
        self.expected=expected;self.context=context;self.used=False
    def check(self,parameters,context):
        if self.used:return False
        self.used=True
        return type(parameters)==bytes and len(parameters)==2+len(self.expected) and parameters[:2]==len(self.expected).to_bytes(2,'big') and hmac.compare_digest(context,self.context) and hmac.compare_digest(parameters[2:],self.expected)
