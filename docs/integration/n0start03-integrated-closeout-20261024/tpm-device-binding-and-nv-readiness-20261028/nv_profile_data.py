"""Isolated DATA validation only. No TPM, secrets, root writes or authority."""
import hashlib, struct
INDEX=0x01804143
ATTR=0x00040014
WRITTEN=0x20000000

def public_profile(area, name, initialized):
    if type(initialized) is not bool or type(area) is not bytes or type(name) is not bytes or len(area)!=14 or len(name)!=34:
        raise ValueError("PROFILE_SHAPE")
    index, alg, attrs, policy_len, size=struct.unpack(">IHIHH",area)
    expected=ATTR | (WRITTEN if initialized else 0)
    if (index,alg,attrs,policy_len,size)!=(INDEX,11,expected,0,8):
        raise ValueError("PROFILE_FIELDS")
    if name!=b"\x00\x0b"+hashlib.sha256(area).digest():
        raise ValueError("PROFILE_NAME")
    return {"index":index,"initialized":initialized,"Name":name.hex(),"authority":"DENY"}

def next_counter(baseline, observed):
    if type(baseline) is not int or type(observed) is not int or not 0<=baseline<2**64-1 or observed!=baseline+1:
        raise ValueError("COUNTER_DIVERGED_NO_REBASE")
    return observed

def readiness(facts):
    required=("anchor_designated","revocation_acceptable_by_contract","same_nonrestorable_device_generation", "owner_hierarchy_custody", "custodian_designated", "protected_ciphertext_original", "authenticated_NV_adapter", "root_live_authority_manifests", "fresh_resources", "separate_exact_NV_approval")
    missing=[k for k in required if facts.get(k) is not True]
    return {"ready":not missing,"missing":missing,"authority":"DENY","scope":"DATA_ONLY_NOT_AN_ADMISSION_PATH"}

# Mock-only NV_Read framing. No transport or session establishment exists here.
import hmac

def _read_inputs(session_key, auth, name, caller, tpm, session):
    if any(type(x) is not bytes or len(x)!=32 for x in (session_key,auth,caller,tpm)) or type(name) is not bytes or len(name)!=34 or name[:2]!=b"\x00\x0b" or type(session) is not int or not 0x02000000<=session<=0x02ffffff:
        raise ValueError("READ_BINDING")

def read_command(session_key, auth, name, caller, tpm, session):
    _read_inputs(session_key,auth,name,caller,tpm,session)
    parameters=struct.pack(">HH",8,0)
    cp=hashlib.sha256(struct.pack(">I",0x14e)+name+name+parameters).digest()
    mac=hmac.new(session_key+auth.rstrip(b"\x00"),cp+caller+tpm+b"\x00",hashlib.sha256).digest()
    authorization=struct.pack(">IH",session,32)+caller+b"\x00"+struct.pack(">H",32)+mac
    body=struct.pack(">III",INDEX,INDEX,len(authorization))+authorization+parameters
    return struct.pack(">HII",0x8002,10+len(body),0x14e)+body

def read_response(packet, session_key, auth, name, caller, tpm, session):
    _read_inputs(session_key,auth,name,caller,tpm,session)
    # sessions response, exact8-byte buffer, nonce32, continue-clear, HMAC32
    if type(packet) is not bytes or len(packet)!=93 or packet[:2]!=b"\x80\x02" or struct.unpack(">II",packet[2:10])!=(93,0) or packet[10:16]!=b"\x00\x00\x00\x0a\x00\x08" or packet[24:26]!=b"\x00\x20" or packet[58:61]!=b"\x00\x00\x20":
        raise ValueError("READ_RESPONSE_SHAPE_OR_RC_NO_RETRY")
    rp=hashlib.sha256(struct.pack(">II",0,0x14e)+packet[14:24]).digest()
    expected=hmac.new(session_key+auth.rstrip(b"\x00"),rp+packet[26:58]+caller+b"\x00",hashlib.sha256).digest()
    if not hmac.compare_digest(expected,packet[61:93]):raise ValueError("READ_HMAC_UNKNOWN")
    return struct.unpack(">Q",packet[16:24])[0]
