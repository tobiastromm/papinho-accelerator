from pathlib import Path
import os,json,hashlib,difflib,shutil
r=Path.cwd();t=Path(os.environ['TEMP'])/'pacc-v4-packet11-20261022';files={}
p='apps/server/server_configuration_d3_worker_win32.inc';s=(r/p).read_text().replace('pending_v3_image_guard','pending_packet11_image_guard').replace('pending_v3_packet_valid','pending_packet11_valid');s=s.replace('unsigned N,B,K,D,q,off,s,u,v;','unsigned N,B,K,D,H,F,version,q,off,s,u,v;');s=s.replace('if(n<PC_ROOT8_V3_MIN||n>PC_ROOT8_V3_MAX||memcmp(b,"PACROOT8",8)||\n    b[8]!=3U||b[9]', '''if(n<PC_ROOT8_V3_MIN||n>PC_ROOT8_V4_MAX)return 0;
 version=b[8];
 if((version!=3U&&version!=4U)||n<(version==3U?PC_ROOT8_V3_MIN:PC_ROOT8_V4_MIN)||
 n>(version==3U?PC_ROOT8_V3_MAX:PC_ROOT8_V4_MAX)||memcmp(b,"PACROOT8",8)||b[9]''')
s=s.replace('off=512U+1536U*N;', '''H=version==4U?((unsigned)b[284]|((unsigned)b[285]<<8)):0U;
 if(H>3U)return 0;F=version==4U?96U+384U*H:0U;
 off=512U+1536U*N;''');s=s.replace('q=u+v+160U+512U*B+16384U;','q=u+v+F+160U+512U*B+16384U;')
s=s.replace('d3_u32(b+40)!=u+v||d3_u32(b+48)!=u+v+160U+512U*B','d3_u32(b+40)!=u+v+F||d3_u32(b+48)!=u+v+F+160U+512U*B')
s=s.replace('return store03_hash(hash_ctx,b,n-32U,hash)', '''if(version==4U&&(b[274]!=1U||b[275]||d3_u32(b+276)!=u+v||d3_u32(b+280)!=F||
 b[286]!=128U||b[287]!=1U||b[288]!=1U||b[289]))return 0;
 return store03_hash(hash_ctx,b,n-32U,hash)''')
s=s.replace('memcmp(cp,"PACR8CP3",8)||cp[8]!=3U','memcmp(cp,a[8]==3U?"PACR8CP3":"PACR8CP4",8)||cp[8]!=a[8]')
s=s.replace('||b[240]!=3U||b[241]||','||b[240]!=a[8]||b[241]||a[8]>b[8]||')
s=s.replace(' return 1;\n}\n#endif', ''' /* This frozen slice preserves existing temporal rows too. No new source
  * disposition is inferred from encoded bytes or a valid Ed25519 key. */
 K=a[8]==4U?((unsigned)a[284]|((unsigned)a[285]<<8)):0U;
 D=b[8]==4U?((unsigned)b[284]|((unsigned)b[285]<<8)):0U;
 if(K!=D||(K&&memcmp(a+d3_u32(a+276)+64U,b+d3_u32(b+276)+64U,384U*K)))return 0;
 return 1;
}
#endif''')
s=s.replace('future reviewed canonical producer','separately qualified canonical producer');files[p]=s
p='apps/server/server_configuration_pending_win32.inc';files[p]=(r/p).read_text().replace('pending_v3_packet_valid','pending_packet11_valid')
# Existing native fixture, same worker and events; v4 builders already present.
p='tests/v4_transaction_worker_win32_test.c';orig=(r/'tests/v3_transaction_worker_win32_test.c').read_text();s=orig.replace('#include "config_root8_v3_test.c"','#include "config_root8_v4_test.c"').replace('observed_v3[PC_ROOT8_V3_MAX]','observed_v3[PC_ROOT8_V4_MAX]');s=s.replace('PAPACC_SIZE bn;unsigned q;','PAPACC_SIZE bn;const PAPACC_U8 *old_image;unsigned q,base_version;');s=s.replace('CHECK(argc==3);stop=', 'CHECK(argc==4);base_version=(unsigned)atoi(argv[3]);CHECK(base_version==3U||base_version==4U);stop=');a=s.index(' CHECK(setup3(');b=s.index(' CHECK(sizeof(p->next)',a)
s=s[:a]+''' CHECK(setup3(4,4,4));CHECK(cp3());
 if(base_version==4U){CHECK(upgrade4(3));CHECK(cp4());bn=z4;memcpy(base4,e4,bn);old_image=base4;CHECK(next4(bn));}
 else{bn=z3;memcpy(base3,e3,bn);old_image=base3;CHECK(next3(3,bn));CHECK(upgrade4(0));}
 CHECK(pc_root8_v4_base_link(e4,z4,old_image,bn,hash,&owner,component4,NULL)==PC_ROOT8_OK);
'''+s[b:];a=s.index(' CHECK(sizeof(p->next)');tail=s[a:].replace('base3','old_image').replace('e3','e4').replace('z3','z4').replace('V3_NATIVE_SAME_WORKER','V4_NATIVE_SAME_WORKER');s=s[:a]+tail;s=s.replace('#include "config_root8_v4_test.c"','#include "config_root8_v4_test.c"\n#include <stdlib.h>')
a=s.index(' owner.published=1;');s=s[:a]+''' /* Resealed outer digest cannot authorize unknown versions or lost history. */
 p->next[8]=5U;CHECK(hash(&owner,p->next,z4-32U,p->next+z4-32U));CHECK(papacc_v3_worker_fixture_start(&owner,identity)!=0&&!p->thread);p->next[8]=4U;
 CHECK(hash(&owner,p->next,z4-32U,p->next+z4-32U));
 q=r32(p->next+48);p->next[q+8192U+112U]^=1;CHECK(hash(&owner,p->next,z4-32U,p->next+z4-32U));CHECK(papacc_v3_worker_fixture_start(&owner,identity)!=0&&!p->thread);p->next[q+8192U+112U]^=1;
 CHECK(hash(&owner,p->next,z4-32U,p->next+z4-32U));
 if(base_version==4U){q=r32(p->next+276)+64U+88U;p->next[q]^=1;CHECK(hash(&owner,p->next,z4-32U,p->next+z4-32U));CHECK(papacc_v3_worker_fixture_start(&owner,identity)!=0&&!p->thread);p->next[q]^=1;CHECK(hash(&owner,p->next,z4-32U,p->next+z4-32U));}
'''+s[a:];files[p]=s
p='CMakeLists.txt';files[p]=(r/p).read_text()+'''
# Isolated v4 packet11 qualification; same canonical native worker, no authority.
if(PAPACC_ENABLE_OFFLINE_D3_WORKER AND PAPACC_ENABLE_WINDOWS_3MIB_MANAGED_PROFILE)
 add_executable(papacc_v4_transaction_worker_test tests/v4_transaction_worker_win32_test.c
  apps/server/server_configuration_commit_win32.c apps/server/server_config_root8_data_win32.c)
 target_compile_definitions(papacc_v4_transaction_worker_test PRIVATE PAPACC_V3_BACKING_FIXTURE=1
  PAPACC_V4_BACKING_FIXTURE=1 PAPACC_OFFLINE_D3_WORKER=1 PAPACC_ISSUER_STORAGE_COMPONENT_TEST=1)
 target_include_directories(papacc_v4_transaction_worker_test PRIVATE ${PAPACC_GRAPH_INCLUDE_DIRS} src/config src/issuer apps/server)
 target_link_libraries(papacc_v4_transaction_worker_test PRIVATE papacc_config_root8 papacc_server_config
  papacc_issuer papacc_root8_signer_data papacc_storage_sha256 bcrypt ktmw32 crypt32 advapi32)
 target_compile_options(papacc_v4_transaction_worker_test PRIVATE /W4 /WX)
endif()
'''
ops=[];diff=[]
for i,(p,s) in enumerate(files.items()):
 old=(r/p).read_bytes() if (r/p).exists() else b'';b=s.replace('\n','\r\n').encode() if b'\r\n' in old else s.encode();st=t/f'post-{i}.txt';st.write_bytes(b);ops.append(dict(path=p,pre=hashlib.sha256(old).hexdigest() if old else None,post=hashlib.sha256(b).hexdigest(),stage=str(st)));diff.extend(difflib.unified_diff(old.decode().replace('\r\n','\n').splitlines(True),s.splitlines(True),fromfile='a/'+p,tofile='b/'+p))
(t/'operations.json').write_text(json.dumps(ops,indent=2));(t/'functional.patch').write_text(''.join(diff));shutil.copyfile(Path(os.environ['TEMP'])/'pacc-prepared-storage-20261022/apply.py',t/'apply.py');print('STAGED',len(ops),len(''.join(diff)))
