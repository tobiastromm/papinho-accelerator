# Staging corrigido — autorização registrada e handoff

## Decisão e conferência

O proprietário autorizou explicitamente UMA execução manual do pedido SHA256 `72fb886f6ba8e6eafd77dcd59d39d46b0e9f7c07248a6bf13fd380cbfd7f58b8`, task PACC-PHASE6-CORRECTED-STAGING-AUTHORIZATION, reconhecendo o risco residual B0–B8 e proibindo execução automática/retries.

Registro específico: [OWNER-STAGING-EXECUTION-AUTHORIZATION.json](OWNER-STAGING-EXECUTION-AUTHORIZATION.json), SHA256 `0703ffe941a0b643705999c8d7aa26cf75526952e9a87f40ca7d76ce86f43114`.

Foram reconferidos integralmente pedido/efeitos/STOP, incorporação accepted B0–B8/D0–D7, plano/seleção B3, manifesto, receita e os três arquivos. Hashes do código e dos testes correspondem à qualificação anchor/child arquivada: quatro builds sem warnings e 20 CTests PASS. Nenhum rebuild ou alteração dos artefatos/pedido foi necessário. Os registros e estados anteriores de candidatura continuam históricos, sem reescrita.

Pacote aprovado: Selected/Release, 9.110.528 bytes; manifesto `8ec63ccd290a5bcb6a796bbfc56f5a7fa38a639516362d0599c56a58209e24fe`. Artefatos: exe `c0f27f94151ddb05172b667641babf220f3e99f7e55c802672f59da413806ecf`, libssl `3fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed`, libcrypto `09eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044`. Receita `50262d5090e5e70719e1d79675092f28b6c848028566ac3cb8b7373fe9458b52`.

## Procedimento do proprietário

Abrir Windows PowerShell 5.1 x64 como administrador, no console original, sem redirecionar entrada/saída. Executar UMA vez o bloco de uma linha em [MANUAL-COMMAND-AUTHORIZED.txt](MANUAL-COMMAND-AUTHORIZED.txt), SHA256 `5f71570b4be3d2283af4e9d8a73784c4d914338d026f32d2fd66b7b62b580661`.

O comando confere receita e autorização antes de chamar o script; o script confere pedido/manifesto/fontes/bytes/hashes antes da chamada nativa. A exceção de política já está delimitada na seção 7 do pedido aprovado: somente processo filho PowerShell desta invocação, sem alterar console pai, política de usuário/máquina ou GPO. Se uma política superior ainda negar execução, STOP; não ampliar direitos, mudar política persistente ou repetir.

O nativo apresenta o plano e solicita confirmação com nonce fresco. Conferir operador, origem, destino, pacote e limites; confirmar somente se corresponderem ao pedido. O agente não produz ou digita essa confirmação. Prazo de 120 segundos inclui preparação/apresentação/confirmação/fechamento; não é garantia de cancelamento de chamada kernel. Enviar toda a saída posterior sem criar sink operacional ou redirecionar a confirmação.

## Gates ainda frescos

Contexto/token/boot e objetos originais não foram adquiridos pelo agente. Durante a execução, são obrigatórios known folder/caminho, identidades e descritores, proteção anchor/child, ausência de reparse/colisão, fonte original, volume/unidade/espaço, recursos/tempo, READY, nonce e confirmação atribuída. Conferência de pins em disco não substitui esses gates nem atesta imagem mapeada.

B3 permanece: H_READY+5; crescimento privado e working de 9.094.144 bytes cada; pisos físico/commit 6.296.461.312/8.479.985.664; disco 2R inicialmente/R após efeitos; freshness500ms e duração120s. Nenhum teto foi ampliado ou valor UNKNOWN convertido em capacidade.

## STOP e resultado futuro

Parar na primeira divergência, falha, colisão, Stop ou UNKNOWN. Não repetir mesmo com consumed=0/effects=0; não limpar, renomear, adotar ou alterar ACL para tentar novamente. Aprovação administrativa utilizada é distinta de consumo nativo. Preservar arquivos parciais, referências/charges e retenção conforme resultado; retorno do processo não comprova quiescência OS.

EXECUÇÃO PENDENTE: o agente não executou staging ou comando. Consumo, sucesso, objetos criados, hashes pós-cópia e fechamento futuros ainda não observados. Até cinco criações exclusivas previstas, sem autorização para modificar ACLs existentes. Aprovações anteriores permanecem consumidas e não reutilizáveis.

Registro/início SCM, Start/Stop, CA/CURRENT, endpoint/rede operacional, TPM/NV/Q4, cleanup/refund, recuperação e promoção produtiva não foram autorizados. Se houver sucesso verificado, o próximo marco é qualificação do alvo para pedido próprio de registro SCM parado. E21 PENDING; ED11/Phase6 PARTIAL; PST0.6.4/UNKNOWN/históricos/pins/reservas preservados.
