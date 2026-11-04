# Escopo a completar — NÃO É PEDIDO OPERACIONAL PRONTO

Estado NOT_READY. Sem autorização ou comando executável.
Primeira lacuna única priorizada: composição protegida descrita no closeout.

Fonte comum:
`C:\Projetos\PapinhoAccelerator\build\phase6-functional-20261028\Selected\Release`.
Destino planejado, sujeito a known folder e gates originais:
`C:\Program Files\Papinho\Accelerator`.

| Arquivo | Bytes | SHA256 |
|---|---:|---|
| papacc_server.exe | 534016 | afa312e63638c5c0ac77a210c3216f994ed210d6eb3a3708b5d9ac14b70c4dc9 |
| libssl-3-x64.dll | 1321472 | 3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed |
| libcrypto-3-x64.dll | 7217664 | 09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044 |

Até dois diretórios exclusivamente novos, Papinho e Accelerator, e exatamente
três arquivos novos. Não copiar DLLs Windows/UCRT por inferência. Não executar
candidato, registrar serviço, criar configuração/root/CA/CURRENT ou tocar em
ProgramData. Não migrar/adotar instalação existente. Objetos parciais não recebem
exclusão/rollback automático.

## Autoridade, proteção e lifetime

Operador: proprietário local legitimamente elegível na mesma invocação, com
token/identidade originais e confirmação atribuída ao escopo exato. Não designar
sandbox, herdar NEW_INSTALL/SCQ/LAB ou criar aprovação em nome do proprietário.

Direitos por fase: consulta do token; leitura/hash/read-control das fontes
retidas; consulta/traverse/read-control dos ancestrais e criação legítima de
filho; nos objetos exclusivamente novos, escrita/proteção inicial, flush,
leitura/hash/identidade e fechamento. Sem take-ownership, backup/restore ou
reescrita de ACL existente.

Descritor dos novos objetos deve ser explícito/protegido e qualificado: writers
apenas administrador legítimo/SYSTEM/Administrators conforme predicado accepted;
leitura/execução pública separada de escrita e custódia CA. Masks, owner e
representação final precisam passar pelo adapter/testes, não são declarados
seguros por este texto. Não substituir a política Windows dos ancestrais pelo
descritor de aplicação.

## Ordem exigida, ainda sem implementação qualificada

1. Reter token, fontes e ancestrais; verificar todos os tamanhos/hashes, ausência
   de reparse, contexto e identidades antes de efeitos persistentes.
2. Apresentar plano/efeitos; confirmação específica; congelar backings; consumir
   uma vez antes do primeiro create. Revalidar antes de cada efeito.
3. Criar Papinho/Accelerator relativamente aos parents retidos, FILE_CREATE,
   nunca OPEN_IF/overwrite. Anexar identidades factuais após cada criação.
4. Criar cada arquivo com proteção inicial; copiar de fontes ainda retidas;
   tratar I/O parcial; flush, releitura, tamanho/hash/proteção/identidade por
   originais. Não reabrir path como substituto de identidade.
5. Registrar efeito/primeiro erro/close por objeto. Sucesso só após três cópias
   verificadas e fechamento conhecido. Não alegar atomicidade física do conjunto.

STOP por colisão, identidade/hash/descritor divergente, referências/contexto
perdidos, Stop/cancelamento, I/O incerto ou recurso obrigatório UNKNOWN. Consumo
não rearma, não autoriza retry/nome novo. Incerteza conserva cohort/backings e
créditos no mesmo owner até disposição factual; sem timeout como close/refund.
Identidades novas são gates da operação, nunca valores inventados no plano.

A falta da implementação/testes desse protocolo impede converter este escopo
em pedido executável. Aprovação futura também não supriria essa prova técnica.
