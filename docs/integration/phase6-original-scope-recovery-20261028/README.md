# Primeiro pacote técnico — Phase 6 recuperada

Pacote de auditoria, sem autoridade operacional. Relatório principal: ../../capabilities/phase6-scope-and-closure-20261028.md.

- REQUIREMENTS.csv: matriz única, incluindo cada C0–C12, ED11 E01–E22, Modes A–D e entregas adicionadas.
- DEPENDENCY-CHAIN.md: causa, norma, perfil, circularidade e limites por dependência histórica.
- CALLER-BOUNDARIES.csv: nove fronteiras conferidas em código atual, com símbolo e próxima ação sem efeitos.
- NEXT-TECHNICAL-EXECUTION.md: próximo prompt técnico, já delimitado sem efeitos.
- verify_scope.py: valida cobertura/IDs/campos e referências, registra SHA256 das fontes e refs git. Não compila, não executa testes produto, não abre ProgramData nem credenciais; escreve apenas SCOPE-VERIFICATION.json neste diretório.

Execução a partir do repositório Accelerator:

```powershell
python docs/integration/phase6-original-scope-recovery-20261028/verify_scope.py
```

PASS do verificador significa somente consistência estrutural documental, não qualificação de requisito. As linhas E01–E22 preservam status/campos históricos da matriz LAB e recebem anotação de perfil; linhas históricas Q4 adicionais não são somadas como gates ED11. Nenhuma evidência limitada foi promovida para PASS integral.
