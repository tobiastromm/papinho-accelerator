"""Public-data analysis only. Existing OpenSSL binary; no trust-store installation or TPM.
No AIA/revocation fetching: openssl verify uses supplied files and no default stores.
A successful cryptographic path is DATA, never Q4 admission/device possession.
"""
import hashlib, subprocess
from pathlib import Path
class EvidenceDenied(Exception):pass

def digest(b):return hashlib.sha256(b).hexdigest()
class OfflinePublicEvidence:
    def __init__(self,executable,expected_hash,runner=subprocess.run):
        self.executable=Path(executable).resolve(strict=True)
        if digest(self.executable.read_bytes())!=expected_hash:raise EvidenceDenied('TOOL_HASH')
        self.runner=runner
    def command(self,*args,input=None):
        result=self.runner([str(self.executable),*args],input=input,stdout=subprocess.PIPE,stderr=subprocess.PIPE,timeout=15,check=False)
        if result.returncode!=0:raise EvidenceDenied('PUBLIC_DATA_PARSE_OR_CHAIN_DENY')
        if len(result.stdout)>1048576:raise EvidenceDenied('OUTPUT_SIZE')
        return result.stdout
    def analyze(self,cert_der,ek_spki_der,roots_pem,expected_cert_hash,expected_spki_hash,expected_roots_hash,verification_unix_time,intermediates_pem=None):
        # Independent typed hashes: cmdlet EK fingerprint is NOT assumed to be SPKI.
        cert=Path(cert_der).read_bytes();key=Path(ek_spki_der).read_bytes();roots=Path(roots_pem).read_bytes()
        if not all(0<len(x)<=1048576 for x in (cert,key,roots)):raise EvidenceDenied('INPUT_SIZE')
        if (digest(cert),digest(key),digest(roots))!=(expected_cert_hash,expected_spki_hash,expected_roots_hash):raise EvidenceDenied('INPUT_HASH')
        if type(verification_unix_time)!=int or not 0<verification_unix_time<2**63:raise EvidenceDenied('TIME')
        # Parse from captured bytes, not rereading the path after hashing.
        cert_pem=self.command('x509','-inform','DER','-outform','PEM',input=cert)
        cert_key_pem=self.command('x509','-inform','DER','-pubkey','-noout',input=cert)
        cert_key_der=self.command('pkey','-pubin','-outform','DER',input=cert_key_pem)
        supplied_der=self.command('pkey','-pubin','-inform','DER','-outform','DER',input=key)
        if supplied_der!=key or cert_key_der!=key:raise EvidenceDenied('EK_SPKI_MISMATCH')
        extensions=self.command('x509','-inform','DER','-noout','-ext','extendedKeyUsage,basicConstraints,keyUsage',input=cert).decode('ascii',errors='strict')
        if '2.23.133.8.1' not in extensions and 'Endorsement Key Certificate' not in extensions:raise EvidenceDenied('EK_CERTIFICATE_PURPOSE')
        if 'CA:FALSE' not in extensions or 'CA:TRUE' in extensions:raise EvidenceDenied('EK_CERTIFICATE_CONSTRAINTS')
        # Offline cryptographic path analysis only. verify never downloads AIA/CRL.
        # Explicit bundle and no default system stores; revocation completeness is UNKNOWN.
        # Path inputs are NOT an original-handle security binding and cannot grant authority.
        extra=[];intermediate_bytes=None
        if intermediates_pem is not None:
            intermediate_bytes=Path(intermediates_pem).read_bytes()
            if not 0<len(intermediate_bytes)<=1048576:raise EvidenceDenied('INTERMEDIATE_SIZE')
            extra=['-untrusted',str(Path(intermediates_pem).resolve())]
        self.command('verify','-no-CAfile','-no-CApath','-no-CAstore','-trusted',str(Path(roots_pem).resolve()),*extra,'-purpose','any','-attime',str(verification_unix_time),input=cert_pem)
        if intermediates_pem is not None and Path(intermediates_pem).read_bytes()!=intermediate_bytes:raise EvidenceDenied('INTERMEDIATE_CHANGED')
        if Path(roots_pem).read_bytes()!=roots:raise EvidenceDenied('ANCHOR_CHANGED')
        return {'certificate_der_sha256':digest(cert),'canonical_spki_sha256':digest(key),
                'roots_candidate_sha256':digest(roots),'public_key_match':'PASS_DATA_ONLY',
                'chain':'CRYPTOGRAPHIC_PATH_DATA_ONLY','revocation':'UNKNOWN_OFFLINE',
                'input_identity':'PATH_DATA_NOT_ORIGINAL_HANDLE_PROOF',
                'anchor_designation':'NOT_GRANTED','possession':'NOT_EXECUTED',
                'generation_continuity':'NOT_ESTABLISHED','authority':'DENY'}
