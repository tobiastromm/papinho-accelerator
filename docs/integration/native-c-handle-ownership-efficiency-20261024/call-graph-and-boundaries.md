# Call graph factual e fronteiras

```mermaid
flowchart TD
 Q[scm-registration-lab.c instrumented main] --> OP[lab_operator + ig_boot + ig_approval]
 Q --> PB[lab_bound + dependencies + source + SCM readbacks]
 Q --> MUT[admission_enter: mutex canônico]
 Q --> N0[ig_n0_bind: roots / inputs RAW1 / OSfile / atores]
 N0 --> BD[admission_bd_bind: duplicates / tokens / confirmação]
 BD --> CUR[psa_bd_current]
 CUR --> AUX[psa_bd_aux_current]
 AUX --> CTX[ig_n0_current]
 CUR --> CTX
 Q --> CENS[ig_project_census snapshot]
 CENS --> PC[ps_bd_census por entrada]
 PC --> INV[ps_bd_inventory / console]
 INV --> AUX
 Q --> MET[ig_observe por fase]
 MET --> CUR
 MET --> INV
 MET --> PSS[GetProcessHandleCount antes de PSS + thread walks + close]
 Q --> ARM[admission_n0_arm / consume]
 ARM --> CUR
 ARM --> INT[PSA.intent CREATE_NEW]
 Q --> COPY[cycle_copy: write / flush / hash / closewriter / reopenrelative]
 Q --> SINK[psrf_create arm / close]
 Q --> SCM[rebind / readbacks / censo / finalmetrics]
 Q --> START[StartService / bindprocess / monitor / cycle_live]
 START --> STOP[Stop quando aceito / processo terminado / extract]
 STOP --> CLEAN[ig_cleanup: encerrar borrows / bd_close / N0close / cycle_cleanup / finish]
 S[main.c.api3.inc service host] --> DISP[StartServiceCtrlDispatcher / ps_main]
 DISP --> RT[psr_prepare]
 RT --> DIAG[psrd_bind / sink e parent]
 RT --> IMG[ps_image_open / SCM / token / runtime parent-config-image]
 RT --> RCHK[psr_current: token e ACLs]
 RT --> EMIT[recorder original / psrd_emit / close diagnóstico]
 DISP --> RUN[host.run = papacc_server_entry]
 RUN --> CLOSED[server_run_root: qualificação init_closed / poll_closed]
 CLOSED --> RCHK
 CLOSED --> RC[runtime_closed ou retained]
 RC --> RFIN[psr_close + ps_close / SetServiceStatus ou park UNKNOWN]
```

O ciclo positivo é fonte existente, mas os artefatos03 atuais bloqueiam sua entrada. Call graph acima não prova que foi executado. PS_NATIVE/PS_ADMISSION do qualificador e PS_NATIVE do serviço são instâncias em PROCESSOS distintos; não somar seus handles. Censo custa observações no qualificador, não na tabela do processo observado.

N0 contexto possui cinco roots, dois inputs e quatro políticas. Originais actor/control/aux/osfile são duplicados para o owner e depois fechados pelo adapter; configuration/image são emprestados, não duplicados por BD. Inventário/observer são borrowers. GetCurrentProcess/Thread e GetModuleHandle são pseudo/borrowed, não CloseHandle/FreeLibrary. SERVICE_STATUS_HANDLE não recebe CloseHandle; SC_HANDLE usa CloseServiceHandle; EVT usa EvtClose; PSS usa suas APIs próprias.

O runtime de qualificação entra em init_closed/poll_closed, managers sem Sessions/jobs ativos/listener/egress/PST ativo; não é o caminho de produção com rede. O audit segue esse ramo até readiness/Stop/recibo e cita a fronteira dos managers/providers; não afirma auditoria de recursos internos de bibliotecas/OS ou de toda composição produtiva. Nenhum CreateEvent/CreateThread explícito foi encontrado nesses owners/bridges; não se propõe trocar o Stop atômico por evento/worker. SetConsoleCtrlHandler e callback SCM podem implicar trabalho do Windows, cujo custo não foi medido.
