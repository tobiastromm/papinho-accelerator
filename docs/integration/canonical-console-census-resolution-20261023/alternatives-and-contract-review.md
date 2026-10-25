# Canonical console census resolution — alternatives and exact blockers

Task PACC-PHASE6-CANONICAL-CONSOLE-CENSUS-RESOLUTION-1. Branch main, HEAD913c90cf2328f7939d8f5b927640601903ac6e91. Existing WIP/source/binaries/pins and historical LABs preserved. Review is documental; no new functional patch or native execution.

## Evidence and authorities

Read the latest106/128 result, existing console/topology/native relationship/tooling reports, accepted ED11 B0–B6 revision4 (especially B4.B.4/B5/B6), canonical census/ig_actor/ig_confirm/operator/token checks, console Stop and canonical administrative owner, Model E, specification§11.2/11.3, Stop/lifetime/pre-close decisions and relevant future listener direction. Applicable accepted governance remains controlling; superseded family ADR0001 is historical, not current authority. External scratch contains the8595-file before inventory and relevant full preimages. Future listener direction remains unchanged because no listener/config implementation changed.

## Alternatives

| Alternative | What it actually changes | Compatibility / disposition |
|---|---|---|
| A: qualifier without console after administrative confirmation | Detaches native child, not its already-running PowerShell/console infrastructure | Does not remove current false conflict. ig_confirm uses CONIN$/CONOUT$/ReadConsoleW; Stop currently receives CTRL_C/BREAK via SetConsoleCtrlHandler. Removing these without binding/replacement is not qualified. Detaching all clients is a larger lifecycle/Stop change. No compatible source patch demonstrated. |
| B: one already-elevated PowerShell | Removes one application process and UAC/intermediate lifetime | Preferred wrapper reduction, but alone does not remove the console host. Current ig_actor rejects identical p1/p2, and B4 explicitly names two PowerShell roles. Requires focal accepted change, not duplicated PID arguments. |
| C: native associated-console classification | Would recognize proven infrastructure and count it | Existing GetConsoleProcessList proves clients, not host ownership; captured window creator was the PowerShell, association/mapped-image proof stayed UNKNOWN. Cannot implement authenticated classification from current evidence. No whitelist or restored heavy instrumentation. |
| D: explicit application/infrastructure policy | Distinguishes resource-only infrastructure eligibility from admission authority | Required to authorize B plus ONE exact designated OS infrastructure instance within FOUR actual processes and unchanged/tighter guards. Proposed, not applied; residual provenance/association uncertainty must be explicitly accepted. No claim of proven authenticated relationship. |

Recommended review path B+D is smaller than removing the qualified Stop/confirmation console path. It does not satisfy a strict requirement of eliminating every dependency on console infrastructure. A remains preferred only if a console-independent confirmation/Stop binding can be proved; none is currently qualified. This limitation is reported instead of claiming topology independence.

## Exact accepted blockers

B4.B.4 says: at most four admitted processes comprising one launcher PowerShell, one elevated PowerShell, one native qualifier and one service; additional operation child/compiler/receiver => STOP. B4.B.6 invalidates changed topology. B4.B.5 requires canonical cross-admission exclusivity and no credits. Therefore current conservative child rejection is consistent with accepted policy even though170 does not prove another admission. It is a topology false conflict, not evidence that the canonical mutex failed.

Source: scm-instrumented-cycle-lab.inc ig_actor validates distinct real process references; entry requires p1!=p2. ig_project_census rejects !participant && (child || project_prefix). It must continue rejecting arbitrary unknown children and concurrent papacc processes until an accepted, proven designation rule exists. scm-registration-lab.c lab_operator/current bind SID/elevation/no impersonation/token continuity; ig_confirm reads the exact native plan from console. apps/server/server_console_win32.c receives CTRL_C/CTRL_BREAK into owner Stop state. None is removed.

## Proposed delta and security review

[Same-policy amendment candidate](../../adr/ADR-CANDIDATE-canonical-lab-console-census-resolution-20261023.md) contains R0–R7. It explicitly requires a decision on resource-only coexistence uncertainty; it does NOT assert authenticated console association. Unknown process identity, unsigned/unqualified image, wrong creation/session/token/file, arbitrary child, real concurrent admission or multiplicity remains DENY. Cross-process authority stays exclusively with the existing owner; a known OS process never acquires authority by being counted. All application gates remain mandatory. No fifth process, new credit,128 increase or production authority.

No GOVERNANCE CONFLICT is being implemented: accepted clauses remain unchanged and the incompatible alternative is held PROPOSED. Approval of this task is approval to prepare the candidate, not to accept or activate it.

## Windows evidence

[Process creation flags](https://learn.microsoft.com/en-us/windows/win32/procthread/process-creation-flags) distinguishes inherited/new/detached console. [FreeConsole](https://learn.microsoft.com/en-us/windows/console/freeconsole) detaches its caller and does not destroy a console shared by other attached processes. [GetConsoleProcessList](https://learn.microsoft.com/en-us/windows/console/getconsoleprocesslist) enumerates attached clients and does not document an authenticated host identity. [SetConsoleCtrlHandler](https://learn.microsoft.com/en-us/windows/console/setconsolectrlhandler) documents per-process handlers and reset on console association changes. These support limits, not a new trust API. No tools installed, ETW captured or custom console recorder introduced.
