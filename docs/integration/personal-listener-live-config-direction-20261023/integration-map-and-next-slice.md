# Listener selection and live configuration: integration map, 2026-10-23

No network executed; source inspection only. This is additive owner-direction consolidation, not a replacement architecture. Original addendum is preserved alongside this map.

| Area | Existing mechanism | Remaining exact boundary |
|---|---|---|
| Persistent identity | src/network/network_interface.h: is_valid + opaque U64. Win32 network_interfaces_win32.c uses adapter Luid.Value when available. | No new ID, name/IP/index alias, wire identity or normalization migration. |
| Enumeration | papacc_network_discover_local_snapshot, Win32 GetAdaptersAddresses; server_cli_interfaces.c, --list-interfaces. | Portable catalog exposes up/loopback/name, not a complete LAN safety/type/eligibility classifier. New CLI equivalent can reuse this projection; do not infer safe LAN from UP. |
| Resolution | persistent_bind_selection/bind_selection/bind_target and immutable interface snapshot owner in launcher. | defaultLan must resolve once into current typed ID and retain binding; exact eligible policy and grammar need focal review. I1 decimal interface_ref is unchanged. |
| .conf | src/config/config_source.c binding/listener blocks; accepted interface_ref quoted decimal; same parser/validation/frontend. | Do not implement interface= or defaultLan in current grammar by assumption; no historical config overwrite. |
| Multiple listeners | accepted ADR0009/N3 collection; parser PC_LISTENERS_MAX4 with target max; multi-socket network listener set. | Current canonical source_binding requires exactly ONE logical listener; pc_validate rejects same-port candidates pending endpoint resolution. Multiple sockets != multiple logical listeners. Preserve requirement; do not claim full multi-listener runtime already implemented. |
| Commands | config validate/diff/apply --source and show-effective in existing CLI. apply currently DENY; no factual effective CURRENT. | Add compatible show/status/equivalent aliases only after semantic review; no new CLI/worker. |
| Application | configuration_commit, canonical root/pins/CAS/worker and source binding; existing immutable generations and Stop/retention. | No live frontend is implemented. GOVERNANCE CONFLICT for new-authoritative .conf apply bypassing CURRENT: N5/D2.2-D2.3 require commit/current, owner forbids CURRENT change by simple reload. Prepare exact specialization, do not choose silently or publish anything. |
| Policy separation | allow_network_egress/destination/egress policy vs listener interface/profile/admission. | No egress permission opens an external listener. |

## One next slice

PERSONAL_LISTENER_SELECTION_AND_LIVE_CONFIGURATION_CONTRACT_RECONCILIATION. Begin after protected runtime task/first-start preparation as appropriate; do not interrupt it or run parallel network implementation.

Review exact selector representation, normative LAN eligibility and pin behavior, .conf compatibility/default/explicit precedence, listener conflict/capacity and effective status projection. Reconcile simple activation/reload versus NEW authoritative candidate commit under N5/D2 with owner-approved exact clauses. Keep any new exception PROPOSED until approval; candidate resolution models may not authorize sockets/CURRENT. Produce one reviewed implementation handoff, not several new architectural subsystems. Any later implementation requires exact hunks and DATA/negative/native offline evidence, no productive network under this addendum.

Required cases: 0/1/several eligible interfaces; unsafe/unknown LAN; explicit selection precedence; selected interface disappears; IP change same ID; changed snapshot/revision; overlap with explicit listener; existing conflict rules; Stop; old/new lifetime; capacity; partial effect and UNKNOWN; unchanged ports/TLS/security and Internet-egress independence. Numeric IDs and topology are inputs to models, not fabricated machine facts.

## Roadmap fit

Current phase stays protected listener-disabled LAB runtime -> separately approved same-service native Start/Status/Stop -> one selector/live-config contract slice -> bounded canonical implementation and later authorized native listener qualification -> remaining personal install/CA/issuer/provisioning/revocation/ED11/end-to-end Phase6 work. SCM stays optional; direct mode uses the same runtime/configuration. No Phase6/PST0.6.5 release closure follows.
