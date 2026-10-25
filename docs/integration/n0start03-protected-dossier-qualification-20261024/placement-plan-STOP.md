# Plano de colocação — STOP, sem autorização executável

Operação normativa N0START-20261024-03; tentativa 0b1de869e11493b7cb08fb329578f6ce. Os dois candidatos deste pacote são documentos comuns de revisão, não inputs protegidos nem aprovação operacional. Ver candidate-hashes.json para os bytes exatos. Nenhum objeto foi colocado.

Destino pretendido: C:\ProgramData\PapinhoAccelerator\Protected\N0. Nomes exclusivos: papacc-n2-0b1de869e11493b7cb08fb329578f6ce.evidence (7573 bytes) e papacc-n2-0b1de869e11493b7cb08fb329578f6ce.approved (397 bytes). O sufixo approved é o formato privado existente; não significa aprovação do candidato.

## Primeira pré-condição ausente

Abertura read-only de C:\ProgramData\PapinhoAccelerator negada no coletor SID S-1-5-21-896877160-3262526087-2538948522-1003, token_elevated=0, Administrators não habilitado, sem impersonation. Os dois primeiros ancestrais coincidiram com os pins; os três diretórios privados NÃO foram revalidados. A exceção Win32Exception foi preservada, mas seu código numérico não foi serializado. Não inferir divergência de ACL nem sua conformidade contemporânea. O contexto observado é insuficiente; causa exclusiva da negação não foi demonstrada sem comparar o objeto em contexto autorizado.

Menor consulta manual existente, em Windows PowerShell 5.1 x64 já elevado do operador autorizado, sem novo PowerShell, sem Place, sem preflight:

```powershell
& "C:\Projetos\PapinhoAccelerator\tools\qualification\n0-protected-input-readonly.ps1" -Mode Verify -ExpectedScriptSha256 6732b4166a46dc93dbfade6040b084f148e5460b441cd17d5132068ee4bfc0e0
```

Esse leitor verifica a cadeia e os DOIS INPUTS HISTÓRICOS, não coloca nem qualifica arquivos sucessores inexistentes. Não executar criador de raiz ou placer antigo. Divergência implica STOP sem correção de ACL. A autorização atual de leitura não autoriza colocação.

## Sequência futura condicionada

1. Revalidar operador/host/boot e cadeia pelos handles originais: IDs históricos de referência PapinhoAccelerator=417614100:786432:1087874, Protected=417614100:851968:2672648, N0=417614100:720896:2672656; descritor original104/control9004/SHA688654f3c898e8ae3115cac83e777c4b24c9c57320fa8551b505d8dacc0ad579. Verificar owner/group BA, SYSTEM→BA FullAccess OI/CI, ausência reparse e direitos dos ancestrais contra exclusão/renomeação/substituição não autorizada. Não alterar pai ou ACL.
2. Adaptar focalmente o mecanismo de colocação EXISTENTE aos dois novos conteúdos/nomes/pins, após autorização/revisão de fonte e qualificação. O script atual Place contém nomes/fontes da tentativa antiga: NÃO o executar para estes candidatos. Sem novo owner/logger/registry.
3. Conferir ausência nativa dos nomes pelo pai original, sem adotar colisões. Não há prova atual de ausência: a consulta parou no ancestral protegido. CREATE_NEW/criação exclusiva final continua obrigatório, consulta não reserva nome.
4. Aplicar desde criação os 152 bytes originais do descritor de arquivo SHA0fcfd0572daee1f64b98daf960276db86f55043460ba700e927bfb46b31b4814, owner autorizado/groupBA/SY+BA+operador conforme mecanismo existente. Não regenerar por SDDL nem aplicar setter a objeto existente. Revalidar hashes, tipo/links/ID/RAW1, flush/readback, fechamento; preservar partial/UNKNOWN sem delete/rollback/retry.
5. Emitir recibo exclusivo e só então preparar pins com IDs efetivamente observados. Não inventar IDs, tokens, aprovação, PID ou métricas. Aprovação específica de colocação exige comando/pins/efeitos exatos revisados; não há comando positivo neste pacote.

O conteúdo candidato preserva que a raiz está pendente na data de coleta. Colocação posterior não altera esses bytes retrospectivamente: recibo fresco separado documentará a qualificação. Alteração de qualquer candidato exige novos hashes e nova revisão.
