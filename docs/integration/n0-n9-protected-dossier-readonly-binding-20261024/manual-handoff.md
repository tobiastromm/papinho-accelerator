# Próxima prova mínima — somente leitura dos inputs existentes

Não executar novamente Place, o criador N0 ou os launchers históricos. Os dois arquivos já existem. Não apagar, mover ou substituir.

Falta uma leitura contemporânea sob o operador autorizado: o executor não conseguiu abrir a raiz protegida (Win325). O comando abaixo é consulta de arquivo, não preflight operacional, e não executa o qualificador. Em uma nova janela Windows PowerShell5.1 x64 já elevada (sem tipos antigos carregados), usar:

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\n0-protected-input-readonly-manual.ps1" -Mode Verify
```

Se necessário pela política local e já permitido no processo do operador, Scope Process Bypass não altera a política permanente. Não contornar GPO nem criar outro PowerShell como filho do launcher.

O leitor verifica pins de fonte, operador1108, ausência de impersonation, host e boot236441/StartTime2026-10-23T18:36:34.5000000Z, recibo N2P e sete objetos nativos. Mantém originais, compara duas vezes todos os descriptors/conteúdos, confere links/size/reparse/caminho e fecha em ordem inversa. Não cria arquivo/recibo/diretório, não consome intent, não adquire admissão canônica e não faz SCM. Evidence5358 e approved326 mantêm os hashes originais. OWNER/group/DACL observados devem corresponder aos bytes152 pinados; sem SDDL/normalização.

Esperado: sete linhas N0_INPUT_READ_VERIFIED, N0_INPUT_READ_FIRST_ERROR=NONE; RETAINED=False; e N0_INPUT_READ_EXIT=0; OWN_REFERENCES_CLOSED. Retorno diferente mantém DENY. UNKNOWN exige preservar processo/referências; sem kill/timeout/retry automático. Captura de console fornecida pelo owner deve ser registrada como tal, sem timestamp ou recibo nativo inventado. Esses snapshots não eliminam revalidação antes da admissão.

Script de entrada SHA256d0a6dee80ea9a6d2ac15989bba36f2cf5af61c288811b84a8d37679c98e88810; leitor.ps1 SHA2566732b4166a46dc93dbfade6040b084f148e5460b441cd17d5132068ee4bfc0e0; demais hashes em qualified-pins.json. Scripts ficam em desenvolvimento e exigem integridade/verificação independente; a autoverificação do próprio script não autentica uma cópia substituída. A entrada executa o mesmo launcher mantido aberto e hash-verificado em memória, evitando nova leitura de código por nome; fontes do helper são compiladas dos mesmos bytes verificados. Não se concede autoridade ao leitor externo.

## Preflight operacional futuro, NÃO executar

scm-n0-protected-single-powershell.ps1 é sucessor preparado e somente parseado por AST. Caminhos novos de inputs e novo binário Selected Release9abce35a948ec1f7dfaee1e0256c11c8d2c56bb72bae0d6d695d8bcf6fad66f0 estão fixados. DossierHash/EvidenceHash e header de pins ativos continuam vazios. Não rodar esse script para repetir um DENY conhecido.

Após leitura factual e revisão, será necessário um slice explícito de pins (rebuild/novo hash do qualificador se o header mudar) e aprovação específica de um preflight manual exato. PID e criação do auxiliar devem ser contemporâneos, não os históricos. Não se fornece um comando operacional com valores inventados ou se chama o snapshot do leitor autorização. Confirmação, exclusividade, consumo, limites e Stop continuam no owner existente. Start/Stop/rebind continuam sem autorização.
