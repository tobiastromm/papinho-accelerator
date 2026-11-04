from pathlib import Path
import hashlib,json
R=Path('C:/Projetos/PapinhoBrowser');P=Path('C:/Projetos/PapinhoAccelerator/docs/integration/n0start03-integrated-closeout-20261024/q4-target-evidence-closure-20261025/browser')
src=R/'src/papinho_c2_s6.inc';b=src.read_bytes();(P/'papinho_c2_s6.PREIMAGE.inc').write_bytes(b)
s=b.decode('utf-8');marker='static void c2_resource_phase(';assert s.count(marker)==1
s=s.replace(marker,(P/'adapter.CANDIDATE.inc').read_text(encoding='ascii')+marker)
old='char values[6][32],peak_values[3][32];const papinho_candidate_resource_sample *r;';assert old in s;s=s.replace(old,old+'C2_TARGET_VIEW view;')
a=s.index('   if(c2_resource_identity(f,r) && (r->presence&128U))for(j=0;j<3U;j++){')
z=s.index('\n   n=_snprintf',a)
s=s[:a]+'   for(j=0;j<3U;j++){\n    if(c2_resource_view(f,i,j,&view) && c2_target_view_classify(&view,&view)==1){\n     peak_mask|=2UL<<j;if(view.value>peak[j])peak[j]=view.value;\n    }\n    if(c2_target_view_admission(&view,&view))return 0;\n   }'+s[z:]
dest=R/'src/papinho_c2_target_evidence.inc';assert not dest.exists()
dest.write_bytes((P/'header.CANDIDATE.inc').read_text(encoding='ascii').encode('ascii'));src.write_bytes(s.encode('utf-8'))
m=R/'Makefile.nt4.mak';mb=m.read_bytes();(P/'Makefile.PREIMAGE.mak').write_bytes(mb);t=mb.decode('utf-8');needle='src\\papinho_c2_s6.inc';assert needle in t
t=t.replace(needle,needle+' src\\papinho_c2_target_evidence.inc');m.write_bytes(t.encode('utf-8'))
(P/'BROWSER-EDIT-HASHES.json').write_text(json.dumps({str(x):hashlib.sha256(x.read_bytes()).hexdigest() for x in [src,dest,m]},indent=2),encoding='utf-8')
