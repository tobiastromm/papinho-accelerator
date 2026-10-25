"""Read-only build attestation: frozen image bytes, generated header and PE constants.
This checks compiled constants, not execution/lifetime/SCM or operational authority.
"""
import argparse,hashlib,json,re,struct
from pathlib import Path

def attest(binary,expected,forbidden=()):
 if not re.fullmatch('[0-9a-f]{64}',expected):raise ValueError('INVALID_EXPECTED_SHA')
 if len(binary)<64 or binary[:2]!=b'MZ':raise ValueError('NOT_PE')
 pe=struct.unpack_from('<I',binary,0x3c)[0]
 if pe+24>len(binary) or binary[pe:pe+4]!=b'PE\0\0':raise ValueError('BAD_PE')
 machine,n=struct.unpack_from('<HH',binary,pe+4);opt=struct.unpack_from('<H',binary,pe+20)[0]
 if machine!=0x8664:raise ValueError('NOT_AMD64')
 sections=pe+24+opt;data=None
 for i in range(n):
  off=sections+40*i
  if off+40>len(binary):raise ValueError('TRUNCATED_SECTIONS')
  if binary[off:off+8].rstrip(b'\0')==b'.rdata':
   size,start=struct.unpack_from('<II',binary,off+16)
   if start+size>len(binary):raise ValueError('TRUNCATED_RDATA')
   data=binary[start:start+size]
 if data is None:raise ValueError('NO_RDATA')
 a=data.count(expected.encode('ascii'));w=data.count(expected.encode('utf-16le'))
 if a<1 or w<1:raise ValueError('COMPILED_IMAGE_PIN_MISSING_OR_STALE')
 for old in forbidden:
  if data.count(old.encode('ascii')) or data.count(old.encode('utf-16le')):raise ValueError('STALE_IMAGE_PIN_RETAINED')
 return dict(ascii_count=a,utf16_count=w,section='.rdata',machine='AMD64',status='PASS')

def main():
 ap=argparse.ArgumentParser();ap.add_argument('--qualifier',type=Path,required=True);ap.add_argument('--image',type=Path,required=True);ap.add_argument('--pin-header',type=Path,required=True);ap.add_argument('--forbid',action='append',default=[]);args=ap.parse_args()
 expected=hashlib.sha256(args.image.read_bytes()).hexdigest();pins=re.findall(r'#define PS_BD_N0_IMAGE_SHA(?:_W)? L?"([0-9a-f]{64})"',args.pin_header.read_text())
 if pins!=[expected,expected]:raise ValueError('HEADER_FROZEN_IMAGE_DIVERGENCE')
 out=attest(args.qualifier.read_bytes(),expected,args.forbid);out.update(image_sha256=expected,qualifier_sha256=hashlib.sha256(args.qualifier.read_bytes()).hexdigest(),scope='READ_ONLY_COMPILED_CONSTANTS_NOT_OPERATIONAL_MEASUREMENT');print(json.dumps(out))
if __name__=='__main__':main()
