# Pedido G5 específico — controlador terminal READ retido

CANDIDATE / NOT_AUTHORIZED / NOT_EXECUTED. 2026-11-03. UMA execuçãoMANUAL semretry, não automática. G0-G9G5 exige decisão específica paraTerminateProcess; autorizações anteriores de sessão/UPDATE/debug/G5 não substituem esta.

## Objeto e evidência

Consulta read-only consumida/concluída first0: SCMSTOPPED/PID0/Win321066/ServiceSpecific1168, configuraçãoOWN_PROCESS16/DEMAND_START3/ERROR_NORMAL1/SID_UNRESTRICTED1/NT SERVICE\PapinhoAccelerator e comando original preservados. ControladorPID5824/criação134382101099776427 vivo. Resultado READ-TERMINAL-STATUS-RESULT.json. Isto não é lease fresco, exitcode do handle original runtime, applicationclose/SCQclose ou READqualificado.

ÚNICO objeto autorizado se aprovado: controladorPID5824/criação134382101099776427, originalTokenPrimary/grupos17/token73773258:0/auth620529:0/modified73772818:0/UserSID S-1-5-21-785699550-1440458426-1682014700-1108, extraído do plano ADMINISTRATOR original da tentativa cbc99489859957d2bbd2d21a4a360a31280ff33dda7573634ac697aedeb53077. Consulta deestado não atestou tokenatual; procedimento deve reconferi-lo antes de efeito. PID somente locator, nenhuma substituição/árvore/filho/runtime/outroprocesso.

Imagem original retidaon-disk C:\Program Files\Papinho\Accelerator\papacc_server.exe:880128bytes/ID417614100:524288:605924/SHA2567ccb080b564ae331a5e212bd455b3b5aff636cae28089c5a4c306268d8687acc, linkúnico/semreparse/nãodiretório. Pins on-disk nãoatestampáginasmapeadas. Não atualizar EXE antes de disposição.

## Gates e efeitos máximos

HostTCORP-TOBIAS/WindowsPowerShell5.1DesktopAMD64/consoleoriginal/operadorSID1108 administradorlegítimosemimpersonação/SeDebug. Abrir somentePIDfixo semherança comQUERY_LIMITED_INFORMATION|SYNCHRONIZE|TERMINATE0x101001; validar/revalidar mesmohandle, criação/liveness/imagem/path e tokenQUERY8/Stats/Useroriginais. Confirmar SCMSTOPPED/PID0 por consultaoriginal antes de qualquer submissão e depois; semStop, alteraçãoSCM ou fallback. Reter referência de imagem/hash até resultado.

Planofixo e confirmaçãoindependente exata DISPOSE_RETAINED_P1_5824_134382101099776427_ONCE. Após nova revalidação, consumoúnico ANTES de UMA TerminateProcess no mesmohandle, exit0xe0000001. Uma espera60000ms eGetExitCodeProcess para observar saída exata; duração deobservação escolhida, não teto nativo/garantia. Qualquer falha/timeout/partial/closeUNKNOWN mantém primeira causa/consumo/UNKNOWN semretry oucomandoalternativo. Fechar própriasreferênciasuma vez; nãorestaurar grants ou liberarreservas porinferência.

## Riscos e limites

Término abrupto eperda deestadovolátil/possívelinterrupçãoIO16pendente, liberaçãoOSsemcloseSCQ, efeitosparciais, closeinterno/retentionhistóricaUNKNOWN são riscos reconhecidos no pedido. Não é SCQclose, READsuccess, refund ou recuperação e não resolve causaKSP/interleaving. Nenhumagarantiade durabilidade/quiescênciaglobal. Somenteestecontrolador, não serviçoWindows (jáobservadoparado), pais/filhos/outrasjanelas.

Semretry/UPDATE/StartStop/EQSP/endpoint/KSP/chaves/fixture/SCM-ACL/limpeza/delete/restore/rollback/refund/CA-CURRENT/rede/TPM-NV-Q4/produção. PreservarHEAD-WIP/históricos/pins/previous/keys/originais/autorização usadas/PST0.6.4. OsG5anteriores são consumidos. Applicationclose/retention/reservas/quiescênciahistóricosUNKNOWN eproduçãoDENY.

## Artefatos e pedido de decisão

Reusa adaptadortransitório único jámodelado,15APIs eUMA chamadaTerminateProcess, semmecanismonovo. Fonteread-terminal-controller-disposition.CANDIDATE.cs SHA2565c9053692e686a934111a816b791d2976a932ad6fb55dcd952c908584c8a0413; wrapperdispose-read-terminal-controller.MANUAL.CANDIDATE.ps1 SHA256f63ee2ab0becf885e99d7c71d43ce3d6f7aa21c0034314589c28c78b89260eeb. Novosbindings e predicadoTokenpuro; compilaçãooffline/AST/negativospuros serão registrados, seminvocarRunFixed. Modeloanterior52casosdefluxo/Stop/partial/closeUNKNOWN reutilizado, semcampanhainalterada repetida.

Solicita decisãoexplícita deUMA execuçãoMANUAL dessesefeitos/riscos. Só depois registrar decisãoefetivamenteemitida em OWNER-READ-TERMINAL-CONTROLLER-DISPOSITION-APPROVAL.txt e fornecercomandohashesreais; nãofabricaraprovacão ou executarautomaticamente. Após saída conferirfirst/consumed/submitted/exitobserved/observerclose/SCM; semretry. Investigaçãoofflineinterleaving16/32 permanece necessária, semnovoUPDATEporhipótese.
