from pathlib import Path
import json,re,base64,hashlib,struct,sys
p=Path(__file__).resolve().parent
sha=lambda b:hashlib.sha256(b).hexdigest()
b=(Path(sys.argv[1]) if len(sys.argv)>1 else p/'OWNER-MANUAL-READBACK.txt').read_bytes();text=b.decode('utf-8-sig');lines=text.splitlines();objects=[];opens=[];dossier=[]
def sid(raw,off,end=None):
 end=len(raw) if end is None else end
 assert off>=0 and off+8<=end
 revision,count=raw[off],raw[off+1];assert revision==1 and count<=15 and off+8+4*count<=end
 return 'S-1-'+str(int.from_bytes(raw[off+2:off+8],'big'))+''.join('-'+str(struct.unpack_from('<I',raw,off+8+4*k)[0]) for k in range(count))
for number,line in enumerate(lines):
 if line.startswith('Q2_OPEN '):
  m=re.search(r'leaf=(\S+) status=([0-9a-f]{8}) io=([0-9a-f]{8})',line);assert m and m[2]==m[3]=='00000000';opens.append(dict(leaf=m[1],status=m[2],io=m[3]))
 if line.startswith('Q2_OBJECT '):
  path=line.split('path=',1)[1].split(' native=',1)[0];fields=dict(re.findall(r'(\w+)=([^\s;]+)',line));raw=base64.b64decode(lines[number+1].split('Q2_RAW_BASE64=',1)[1].split(';',1)[0],validate=True)
  assert sha(raw)==fields['raw_sha256'];rev,res,control,owner,group,sacl,dacl=struct.unpack_from('<BBHIIII',raw);assert rev==1 and control&0x8000 and control==int(fields['control'],16);assert sid(raw,owner)==fields['owner'] and sid(raw,group)==fields['group'];assert sacl==0
  ar,az,acl_size,count,az2=struct.unpack_from('<BBHHH',raw,dacl);assert ar in (2,4) and dacl+acl_size<=len(raw) and count==int(fields['aces']);aces=[];offset=dacl+8
  for k in range(count):
   typ,flags,size,mask=struct.unpack_from('<BBHI',raw,offset);assert typ in (0,1) and offset+size<=dacl+acl_size and size>=16;aces.append(dict(type=typ,flags=flags,mask=mask,sid=sid(raw,offset+8,offset+size)));offset+=size
  assert offset<=dacl+acl_size
  item={k:v for k,v in fields.items() if k!='path'};item.update(path=path,raw_bytes=len(raw),raw_base64=base64.b64encode(raw).decode(),ordered_aces=aces,SACL_audit_label='UNKNOWN_MASK7_NOT_QUERIED');objects.append(item)
 if line.startswith('N0_INPUT_READ_VERIFIED '):
  path=line.split('path=',1)[1].split(' native=',1)[0];f=dict(re.findall(r'(\w+)=([^\s;]+)',line));f['path']=path;dossier.append(f)
assert len(objects)==11 and len(opens)==10 and len(dossier)==7
assert 'Q2_CONTROLLED_CLOSE=True;' in text and 'Q2_READBACK_FIRST_ERROR=NONE; RETAINED=False;' in text and 'Q2_READBACK_EXIT=0;' in text
known={'C:\\':'417614100:327680:5'}
assert objects[0]['native']=='417614100:327680:5' and objects[6]['native']=='417614100:1966080:79426'
intent,image,sink,config=objects[7:]
assert intent['native']=='417614100:1114112:521672' and intent['size']=='103'
assert intent['content_sha256']==sha(b'ATTEMPT=0b1de869e11493b7cb08fb329578f6ce; CONSUMED; NO_RETRY_NO_RECOVERY; UNKNOWN_UNTIL_FACTUAL_CLOSE\r\n')
assert image['native']=='417614100:1179648:2917414' and image['size']=='434688' and image['content_sha256']=='44d8597469bdd4f40b317f467029ee96b3ad69fdc270bcdbf5e126788f623e5c'
assert sink['native']=='417614100:196608:2922916' and sink['size']=='2048'
assert config['native']=='417614100:393216:1120240' and config['size']=='116' and config['content_sha256']=='a44b0e43dc418d91ee1841a7de4fb03f611a4f2a74870a16a177f9be8dc89a44'
# Independently reproduce the existing native frame bytes from archived ASCII payload;
# this is an expected-content derivation, not another native sink read.
record=next(x.split('Q2_ORIGINAL_DIAGNOSTIC ',1)[1] for x in lines if x.startswith('Q2_ORIGINAL_DIAGNOSTIC ')).replace('\\r','\r').replace('\\n','\n').encode('ascii')
assert len(record)==525 and sha(record)=='cab4f25f9e95257222e8f64b9fadc98f13d040b5681e5d253481cb2b1f09c437'
frames=[]
for kind,payload in [(0,b''),(1,record)]:
 frame=bytearray(1024);frame[:16]=b'PACC_PSR_FILE01\0';frame[16:48]=b'0b1de869e11493b7cb08fb329578f6ce';struct.pack_into('<III',frame,48,1,kind,len(payload));frame[64:64+len(payload)]=payload;checksum=2166136261
 for k,value in enumerate(frame):
  if not 60<=k<64:checksum=((checksum^value)*16777619)&0xffffffff
 struct.pack_into('<I',frame,60,checksum);frames.append(frame)
assert sha(frames[0]+frames[1])==sink['content_sha256']
expected=[('417614100:327680:5','9205e0643299861cdba58173a4e9e01db5a3bc63c4d8c5538bc2292dae214fc8'),('417614100:131072:227641','c0811643edc9703aa06642dfe053b7a0c3062cbaba03b6b169bde6962248f6a6'),('417614100:786432:1087874','688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579'),('417614100:851968:2672648','688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579'),('417614100:720896:2672656','688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579'),('417614100:2883584:2416818','0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814'),('417614100:1966080:2672522','0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814')]
for item,(native,sd) in zip(dossier,expected):assert item['native']==native and item['raw_sha256']==sd
assert dossier[5]['content_sha256']=='15b643829189855149826779e9b6a54c1650ed9df6360de63e90cd35bf4887d7' and dossier[6]['content_sha256']=='5087db7a1e02e3334e827360a464c25b3edc1b0059d0a01d506cf17da6ec4050'
op='S-1-5-21-785699550-1440458426-1682014700-1108';trusted={op,'S-1-5-18','S-1-5-32-544'}
for item in [objects[6],image,config]:
 assert item['owner'] in trusted and int(item['control'],16)&0x1000
 for ace in item['ordered_aces']:
  if ace['type']==0 and not ace['flags']&8 and ace['mask']&0x500d0156:assert ace['sid'] in trusted
for item in [intent,sink]:assert item['owner']==op and item['group']=='S-1-5-32-544' and item['control']=='9004'
assert {x['sid']:x['mask'] for x in intent['ordered_aces']}=={x:0x1f01ff for x in trusted}
assert {x['sid']:x['mask'] for x in sink['ordered_aces']}==dict({x:0x1f01ff for x in trusted},**{'S-1-5-80-3567404667-26456103-3018599297-139793421-3962028451':0x120083})
result={'source_sha256':sha(b),'native_owner_readback':'PASS_AT_OBSERVATION','query_records':opens,'current_raw_objects':objects,'qualified_dossier_readback':dossier,'final_first_error':'NONE','readback_retained':False,'new_inspector_controlled_close':True,'Q2_transition':'NOT_APPLIED','new_identity':'NOT_SELECTED','historical_intent_image_ID_continuity':'NOT_PRINTED_UNKNOWN_NOT_INFERRED_FROM_HASH','historical_raw_continuity_unpinned_objects':'UNKNOWN_NOT_AUTO_ADOPTED','SACL_and_global_quiescence':'UNKNOWN','old_token_and_missing_role_masks':'UNKNOWN','full_sink_hash_independently_reconstructed_match':True,'readback_time':'NOT_PRINTED_NOT_FRESH_ADMISSION','no_protected_read_or_operation_repeated_by_agent':True}
(p/'READBACK-RECONCILIATION.json').write_text(json.dumps(result,indent=2))
print('RECONCILIATION=PASS raw_objects=11 original_parent_queries=10 dossier_objects=7; CHECKS_NATIVE_ID_RAW_SID_ACE_CONTENT_FRAME_BINDING_CLOSE; Q2_NOT_APPLIED')
