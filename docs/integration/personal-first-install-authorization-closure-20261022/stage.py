from pathlib import Path
import json,hashlib
root=Path(r'C:\Projetos\PapinhoAccelerator'); stage=Path(__file__).parent
source=root/'docs/integration/personal-first-install-authorization-closure-20261022/PERSONAL-F0-F9-CONSOLIDATED-PROPOSED.md'
raw=source.read_bytes(); body=raw[raw.index(b'## F0 '):]
source_hash=hashlib.sha256(raw).hexdigest(); body_hash=hashlib.sha256(body).hexdigest()
header=f'''---
status: accepted
revision: 1
decision-date: 2026-10-22
scope-level: project
scope-target: PapinhoAccelerator personal local first-install authorization
supersedes: []
---
# Decisão — autorização de primeira instalação pessoal/local F0–F9

## Autoridade, incorporação e precedência delimitadas

Task PACC-PHASE6-PERSONAL-FIRST-INSTALL-AUTHORIZATION-CLOSURE-1. O owner decidiu expressamente PERSONAL_LOCAL como escopo atual e INSTITUTIONAL como futuro fora de escopo, autorizando consolidação e aceitação após revisão oficial. Este registro só se torna vigente após aprovação oficial da escrita exata e verificação de hashes; não aplica patches funcionais nem autoriza execução.

Fonte preservada: docs/integration/personal-first-install-authorization-closure-20261022/PERSONAL-F0-F9-CONSOLIDATED-PROPOSED.md, revisão2, SHA-256 {source_hash}.
Corpo F0–F9 incorporado byte-exato desde “## F0”: SHA-256 {body_hash}.
Revisão: docs/integration/personal-first-install-authorization-closure-20261022/conformance-review.md.

Esta aceitação especializa explicitamente o mecanismo de autorização pessoal/local sob P1 da DECISION-local-bootstrap-ceremony-20261019. Mantém ator legítimo fora do candidato, direitos sobre parent, declaração atribuída distinta de fato nativo e demais P0–P9/D2/D4. Nenhum byte de P1 ou outra decisão accepted é alterado/superseded. A origem é o ato local composto sob a política aqui previamente definida, não elevação/flag isolada; não se exige fonte institucional/CA da própria instalação. O limite de confiança no controle administrativo legítimo do host é explícito, sem alegação de propriedade jurídica, presença humana, unicidade global ou fencing.

As três propostas históricas F0–F9/F2–F5/F2–F3 permanecem PROPOSED, preservadas como histórico. Somente o corpo consolidado abaixo é aceito para PERSONAL_LOCAL. Nenhum contrato de delegação PACKAGE_SIGNING com cláusulas homônimas F1–F5/G0–G7 é modificado ou substituído. Institucional permanece DEFERRED, sem fornecedor, fonte ou política corporativa selecionados.

Issuer/custódia cláusula6, bootstrap inicial v1, CURRENT/CAS, root/C8, Model E/ED08 e ED11 permanecem vigentes. CA_INITIALIZE é ato distinto e explícito mesmo quando a interação nomear os dois propósitos. Não se aceitam implicitamente um formato de CA/issuer, bootstrap v4, nova transação ou atalho de publicação. Núcleo portável não depende de SID/Win32/CNG; Windows/box/ISA não são qualificados por norma. Quotas inalteradas; Selected3MiB só com seleção explícita. ED11 PARTIAL.

IMPLEMENTATION_AUTHORIZED=NO; BOOTSTRAP_EXECUTION=NOT_RUN; CURRENT_PUBLICATION=DENY; SIGNATURE_AUTHORITY=DENY; AUTHORITATIVE_RECOVERY=DENY; PRODUCTION_PIN=0.6.4. Não autoriza chaves, certificados, rede, listeners/admissão, PST/Browser/wire, release ou Phase6 closure.

## Corpo normativo incorporado

'''
post=header.encode('utf-8')+body
p='docs/adr/DECISION-personal-first-install-authorization-f0-f9-20261022.md'; assert not (root/p).exists()
(stage/'accepted.md').write_bytes(post)
ops=[dict(path=p,pre=None,post=hashlib.sha256(post).hexdigest(),stage=str(stage/'accepted.md'))]
(stage/'operations.json').write_text(json.dumps(ops,indent=2))
(stage/'incorporation.json').write_text(json.dumps(dict(source=str(source),source_sha256=source_hash,body_sha256=body_hash,target=p,target_sha256=ops[0]['post']),indent=2))
print(json.dumps(ops,indent=2))
