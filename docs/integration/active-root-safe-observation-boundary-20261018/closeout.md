# Active root safe observation boundary — stopped before source modification
Task PACC-PHASE6-ACTIVE-ROOT-SAFE-OBSERVATION-BOUNDARY-1. Date2026-10-18.

## Decision
The explicit STOP condition in task sections4/9 applies: safe bounded owner latency cannot be demonstrated with the current synchronous resource producer. No source patch was submitted/applied, including the older two-hunk proposal. This is a factual prerequisite failure, not a new executor denial or a claim that all Windows queries necessarily block indefinitely.

## Concrete dependency
papacc_c8_target_collect_diagnostic (apps/server/server_c8_target_evidence_win32.c:87–124) executes GlobalMemoryStatusEx, GetProcessMemoryInfo, GetProcessHandleCount, GetProcessTimes and pst_get_version synchronously on the binding owner thread. The calls accept no timeout/cancellation input in this implementation. The only completion timestamp is read after those queries return. reduce rejects a duration over100ms AFTER completion; collection_end releases the existing nonwaiting cadence gate AFTER completion. Neither the cadence gate nor this rejection bounds a native call's duration or lets the owner run request_stop while inside it. The100ms rule is not a Mode C freshness/headroom approval.

The console callback in server_console_win32.c only atomically records Stop. The canonical loop in server_run_root_win32.inc:265–280 checks that flag on the owner thread; owner request_stop is called during cleanup. A pre/post flag check can discard an observation but cannot ensure bounded response while a synchronous query has not returned. Catching an overrun after return is observation, not a preemptive deadline. The complete-poll return remains a possible serialized call boundary; it supplies no latency guarantee for an added collector.

The current provenance export adds process/time/image queries. Moving them outside a memcpy window would make that copy simpler but would not remove the resource-collection latency dependency. No empirical idle sample, synthetic delayed fixture or successful build proves a universal query completion bound. Therefore tests on the existing path would not establish the missing guarantee, and adding a surrogate implementation would not justify PASS.

## Scope preserved
No lock/thread/collector/supervisor/cache/wake, operational gate, policy CURRENT, epoch, grant, quota or lifecycle change was made. No active query or network executed. Retained graph/credits remain untouched. The producer, root and proposed patch hashes still match their preceding review preimages. No changes to Browser/PST/PACC wire/production0.6.4. PST0.6.5 remains candidate until integral Phase6 completion.

## Smallest decision/input needed
Before implementing owner-thread active collection, supply an applicable guaranteed collection/Stop-latency contract for the existing mechanism, or explicitly decide a different laboratory latency requirement backed by target evidence. No numerical budget is proposed here. If owner decides to consider observing outside the active owner, its existing mechanism, identity association, lifetime protection, accounting and cancellation/retention contract must first be demonstrated; this task does not authorize creating a collector thread, supervisor or parallel authority. Do not silently substitute an external sampler or accept weakened Stop guarantees.

The current campaign must retain this prerequisite and remain non-executable. Copy-only publication of an already completed snapshot can be designed later, but it cannot be promoted as completion of requirements A–E while collection latency remains unproved. The older proposed patch remains unapplied and is not sufficient to resolve this blocker.

## Result
CURRENT_TASK_COMPLETED=NO
ACTIVE_OBSERVATION_BOUNDARY=NOT_IMPLEMENTED_STOP_CONDITION_MET
COLLECTION_COMPLETION=EXISTING_POST_RETURN_ONLY_NO_COMPLETION_BOUND
SYNCHRONOUS_COPY=NOT_IMPLEMENTED
STOP_REVALIDATION=INSUFFICIENT_TO_BOUND_IN_FLIGHT_COLLECTION
NONREENTRANCY=ACTIVE_INTEGRATION_NOT_PROVEN
EXPORT_SIDE_EFFECTS=SYNCHRONOUS_NATIVE_QUERY_LATENCY_NOT_BOUNDED
BUILDS=NOT_RUN
WARNINGS=NOT_ASSESSED
OFFLINE_TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
ED11_INCREMENTAL_STORAGE=0_NO_SOURCE_CHANGE
ED11_TOTAL_STATUS=PARTIAL
CLIENT_ADMISSION=DISABLED
SITE_TLS_ADMISSION=DENY
ED11_ADMISSION=DENY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
PST065_RELEASE=DEFERRED_UNTIL_PHASE6_COMPLETION
WIP_PRESERVED=YES
EXECUTOR_SOURCE_REVIEW=NOT_SUBMITTED
FIRST_UNRESOLVED_DIVERGENCE=EXISTING_SYNCHRONOUS_COLLECTION_HAS_NO_PROVEN_STOP_LATENCY_BOUND
NEXT=OWNER_DECISION_ON_COLLECTION_LATENCY_REQUIREMENT_BEFORE_PAIR_CAMPAIGN_READINESS