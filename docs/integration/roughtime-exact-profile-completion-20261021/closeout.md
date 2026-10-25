# Exact Roughtime profile completion: proposed terms only

AT2, PT0 and PT7 now have explicit proposed trust, byte/campaign/processing and transport/deadline rules. PT3 and coherence references are corrected only as necessary to define the Q interval and distinguish normative choices from deployment qualification. See [conformance/handoff](conformance-and-handoff.md), [byte limits/security](limits-security-and-rationale.md), [census](ed11-census.json), [exact patch](exact-normative-corrections-PROPOSED.patch), [hashes](final-manifest.json).

The proposed bounded data charge is 65192 bytes, not a measured footprint. Added to the prior full conservative scenario, 3813017 bytes already exceeds Windows 3 MiB by 667289 bytes before unknown external/native costs. Dependent implementation is stopped. This does not prevent presenting the normative proposal for review. Errata lookup closure and actual source/PAL/current qualification remain explicitly unproved.

```ini
CURRENT_TASK_COMPLETED=YES
COMPLETION_SCOPE=FOCAL_PROPOSED_NORMATIVE_DELIVERABLES_ONLY
AT2_TIME_BOOTSTRAP=EXPLICIT_BARE_KEY_CANONICAL_AUTHORIZATION_PROPOSED
PT0_LIMITS=EXPLICIT_2048_BYTE_3_SOURCE_6_QUERY_PROFILE_PROPOSED
PT7_TRANSPORT=RFC10049_V1_UDP_ONLY_PROPOSED
PT7_DEADLINES=EXPLICIT_1_3_6_60_SECONDS_PROPOSED
UNCERTAINTY_POLICY=CONDITIONAL_Q_INTERVAL_HULL_AND_REJECTION_CAPS_PROPOSED
AT0_AT9_STATUS=PROPOSED
PT0_PT7_STATUS=PROPOSED
IMPLEMENTATION_AUTHORIZED=NO
UTC_SOURCE=NOT_QUALIFIED
ISSUER_STATUS_BINDING=DENY
UPDATE_U0_U7_BINDING=DENY
CURRENT_PUBLICATION=DENY
SIGNATURE_AUTHORITY=DENY
AUTHORITATIVE_RECOVERY=DENY
ED11_TOTAL_STATUS=PARTIAL
DEPENDENT_IMPLEMENTATION=STOP_ED11_EXCEEDED
BUILDS=NOT_RUN
TESTS=NOT_RUN
NETWORK_TESTS=NOT_RUN
DOCUMENT_RESEARCH=OFFICIAL_HTTPS_ONLY
PST_CHANGED=NO
BROWSER_CHANGED=NO
PACC_WIRE_CHANGED=NO
PRODUCTION_PIN=0.6.4
WIP_PRESERVED=YES
FIRST_UNRESOLVED_DIVERGENCE=FULL_ACTIVATION_CAPACITY_EXCEEDED_AND_ERRATA_REVIEW_INCOMPLETE
NEXT=ROUGHTIME_AT_PT_EXACT_NORMATIVE_ACCEPTANCE
```

No acceptance record, functional source or existing live document is changed. No operational network, clock change, CA/key/certificate, build/test, clone/worktree or commit/push/tag/release. Both project HEADs, branches and baseline hashes are verified after document incorporation. Browser and four LAB directories are untouched. Official approval of document incorporation is not normative acceptance.
