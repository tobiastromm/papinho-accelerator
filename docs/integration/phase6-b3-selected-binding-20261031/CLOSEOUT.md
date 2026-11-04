# Closeout — seleção B3 registrada e pedido operacional preparado

Task continuada PACC-PHASE6-LIMITED-STAGING-B3-EXACT-PLAN. Branch main; HEAD f341985924d29870d8ff627ed37c8a0874ad6469. Seleção explícita do proprietário nesta retomada, sem autorização operacional. WIP/históricos preservados; sem commit/reset/clean.

## Decisão factual e valores

Conferidos integralmente B0–B8 accepted/hashb4b705cc..., D0–D7 e o plano B3 SHA256e4ecd81a00ed4d09a272ac9e3219b1d3ef08b725049a109b6436dee9ee4b724e. Pisos inequívocos: físico6.296.461.312/commit8.479.985.664bytes, com proveniência phase7 da segunda coleta própria de staging falha132. Não houve divergência nem seleção nova por inferência. OWNER-B3-SELECTION.json registra os valores selecionados, SHA256c013a98b6b3e0e2a154a1b4cc43dd804c055ad53bcfb163c08daa983489713f3. É registro administrativo deste ensaio, não ADR/grant/ledger nem aprovação de execução.

## Binding e implementação

Original selecionado segue o mesmo owner/presenter/packet. Entry --owner-manual-selected-staging exige três digests estritos, deriva fonte da própria imagem, adquire fontes/ancestrais/volume/token/contexto originais, verifica conteúdo contra os pins esperados e conserva referências/SD/identidades. CSPRNG gera uma única tentativa fresca, sem fallback. Depois da geração, READY congela exatamente uma vez H+5, private+9094144, WS+9094144, pisos selecionados, unidade/R e500/120000ms. Não há setter público de capacidade ou refreeze para caber resultado.

Presenter mostra operador, fonte/destino, três arquivos/bytes/hashes, proteção nova, efeitos/risco e critérios, pedindo confirmação nativa específica com nonce. Apenas após esse ato atribuído, no mesmo contexto vivo/owner/thread, staging_admit revalida e pode tomar uma vez antes de efeitos. Seleção administrativa/argumentos/dados isolados não concedem autoridade. Nenhum grant externo fictício ou restauração por arquivo/log/restart.

LIMITED com esse original selecionado usa coleta própria sem transplantar124, mantendo o guard estrito e demais domínios intactos. Falta de plano original/máscara DATA fabricada continua negando. Antes de cada criação/write/flush: revalidação de identidade/proteção por referências, volume, recursos e QPC. Revalidação de metadata não move cursores da cópia; hashes nos pontos de captura/current/readback e bloqueio de compartilhamento de escrita/delete das fontes preservam conteúdo original. Freshness conta desde o começo mais antigo de identity check, volume query e sample, não desde o fim conveniente. Metas permanecem fixas; R/unidade divergente nega. Após todas as cópias existe verificação fresca final.

Antes do primeiro efeito, caller/global>=2R; depois>=R. Nova referência precisa caber em H_READY+5. Pico/OS/kernel opacos continuam UNKNOWN; não são zero ou limite físico demonstrado. Deadline120000ms inclui apresentação até close; expiração suprime novas submissões. Close retornado fora do prazo marca falha sem fabricar referências retidas já fechadas. Nunca matar/desfazer por timeout. Resultado de IO/flush/readback/close continua outcome observado, não pré-condição impossível.

Callers novos são explícitos, fora do caminho de default e sem execução automática. LAB instrumentado nega. Builds sem perfil nativo requerido têm stubs que retornam negação. Não há Start/SCM/CA/CURRENT/rede/TPM/CNG produtivo chamado nesse caminho. Não é promoção a NEW_INSTALL.

## Wrapper/manual e autorização ainda ausente

run-selected-staging.MANUAL.ps1 foi somente analisado por parser (0erros), não executado. Requer PowerShell5.1x64 elevado/console original e hashes do próprio script/registro da FUTURA aprovação antes de chamar. Registro OWNER-STAGING-EXECUTION-AUTHORIZATION.json não existe agora. Script verifica pedido/manifesto, abre os três arquivos read-only sem sharewrite/delete e segura streams durante a única chamada nativa. Boot/SID/host são metadata frescos, não autoridade. Não é mapped-image attestation; source-path/disk-pins não eliminam todos os riscos de loader/substituição de ancestrais. Não afirmar garantia ausente.

A origem de autoridade local é o ato atribuído/confirmado no contexto elegível e a aprovação específica do procedimento; JSON é registro documental, não ledger operacional. Não se promete enforcement persistente de uso único entre processos; aprovação manual é usada uma vez e reconciliada pela saída, mesmo effects0. Nenhuma aprovação histórica é reutilizada. A exceção Process-only de execução do script está explicitada no pedido para aprovação, sem alterar política nesta tarefa.

## Qualificação final

Quatro builds Selected/Legacy Debug/Release PASS, sem warnings MSVC /W4 /WX. Retomada executou builds um por vez, paralelismo de compilação1;17CTests por configuração,68execuções PASS. Logs e BUILD-VERIFICATION.json preservados. Staging56cenários/169checks; recursos53casos/184checks por configuração; demais regressores mantidos.

Modelos cobrem jornada selecionada132→READY137 com cinco criações/cópias/flush/readback/close, limites sem refreeze, original/pin divergente, confirmação negada, replay/consumo, ausência de plano, perfil inválido, recurso/host insuficiente, headroom handles/private/WS, freshness lenta600ms versus500ms, deadline>120s, unidade/R substituída, inicial2R e execuçãoR, queda<R após primeiro efeito, close retornando tarde sem retenção inventada, falhas de IO/Stop/retidos já qualificadas. Entry público completo chamado somente com APIs de filesystem/token/console/RNG/contadores modeladas. Nenhum efeito protegido real; SHA/SD em memória reais. Parsing de script não equivale a execução dele no Windows PowerShell5.1.

Censo gerido Selected1.988.928/Legacy1.820.360; limites3.145.728/2.097.152 inalterados. Novos campos e automáticos nomeados/pins/sampler no cálculo do mesmo owner; não cria pools/threads. Isso não limita stack/provider/kernel/host: custos opacos permanecem no risco selecionado. SYSTEM backgroundTaskHost/crash mostrado pelo usuário não teve causa atribuída ao código. Ferramentas retornaram1450/1455 por recursos/paginação; depois recuperaram. Não houve alteração de pagefile, encerramento de processos ou reparo do Windows. Nenhum resultado de staging é inferido desse incidente.

## Artefato final e pedido pronto para decisão

FINAL-PACKAGE.json SHA2568fb360e36490af2051bdfa167950718f8f44cc1285286af3b52581604d7af161:
- Exe563712bytes, SHA25659e8808e1cdacde27cab30a76265b59def2f28362858f9524e06a36980a9941d.
- libssl1321472bytes, SHA2563fb3cd7804dbe3216c801b470e14461d80214ece99c637ae42ea3d8caf75d7ed.
- libcrypto7217664bytes, SHA25609eec573c9adea156ba2073f8cd61720d0aabeb7562d8498b4ecd21b710a3044.

Total9.102.848bytes. Aumento8.704 em relação ao pacote anterior; growth budget continua9094144 exatos como selecionado, não reajustado. Artefato derivado é apresentado no pedido final para aprovação explícita, sem seleção silenciosa. Bytes anteriores01db... e1d6b... preservados/conferidos antes de substituição em build/staging-observation-preserved-20261031; pins históricos não editados.

ONE-SELECTED-STAGING-REQUEST.CANDIDATE.md, SHA256a9280bc14623dfdf23e4c67caf65b4baa58d0109c34afd348ae7223e5c173d16. Pedido byte-exato PRONTO PARA APROVAÇÃO, não aprovado. Script SHA25688dfb7e8f0de05ca745aecc7fe39d6a9082532260fad2d917aa794c222e4502e. HASHES.json contém decisão de seleção/fontes/artefatos/scripts/pedido. Não criar comando executável com hash futuro da aprovação; após autorização específica, registrar seus bytes/hash e entregar handoff preenchido, sem executar.

Primeiro gate restante: autorização específica desse pedido/artefato final. Originais e recursos reais do alvo serão gates frescos dentro da invocação delimitada; podem negar antes de criar. Não prometer que máquina com pressão de recursos passará ou que a preparação já provou identidade/ACL/volume operacional. Falhas/UNKNOWN são reconciliados sem retry automático, sem outra seleção dos valores já escolhidos e sem inventar nova autoridade.

Caminho: aprovação do pedido → registro e hash da aprovação/handoff manual → uma invocação pelo proprietário → reconciliação de efeitos/primeira falha/close/retidos → se cópia verificada/fechada, pedido separado de SCM_REGISTER_CLOSED parado. Não Start/SCQ/KSP/CA/CURRENT ou publicação PST por consequência. Staging NÃO EXECUTADO/DENY até autorização; E21 PENDING; ED11/Phase6 PARTIAL. HEAD/WIP/históricos/pins/UNKNOWN/reservas/chave fixture/PST0.6.4 preservados.
