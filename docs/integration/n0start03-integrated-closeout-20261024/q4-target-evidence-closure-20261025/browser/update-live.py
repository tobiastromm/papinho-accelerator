from pathlib import Path
p=Path('C:/Projetos/PapinhoBrowser/docs/mode-c-c2-factual-handoff.md')
q=Path('C:/Projetos/PapinhoAccelerator/docs/integration/n0start03-integrated-closeout-20261024/q4-target-evidence-closure-20261025/browser')
b=p.read_bytes();(q/'mode-c-c2-factual-handoff.PREIMAGE.md').write_bytes(b)
p.write_bytes(b+(q/'live-update.txt').read_bytes())
