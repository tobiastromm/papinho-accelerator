"""Offline correlation of owner-provided DATA, never admission or authority."""
import json

class Denied(ValueError):
    pass

def review(data, expected_parent=None, expected_creation=None):
    def require(condition, code):
        if not condition:
            raise Denied(code)
    require(data.get("kind") == "READ_ONLY_CONSOLE_DIAGNOSTIC_NOT_ADMISSION", "DATA_KIND")
    require(data.get("first") == "NONE" and not data.get("retained"), "FAILED_OR_RETAINED")
    require(data.get("controlled_original_refs_remaining") == 0, "UNCLOSED_REFERENCE")
    require(data.get("authority") == "DENY" and data.get("TPM") == "NOT_EXECUTED", "SCOPE")
    nodes = {}
    for n in data["nodes"]:
        require(n["pid"] not in nodes, "DUPLICATE_PID")
        nodes[n["pid"]] = n
    def identity(pid):
        require(pid in nodes, "MISSING_ORIGINAL_PROCESS")
        n = nodes[pid]
        require(n["original_handle_identity"] and n["live_stable"] and n["creation"] > 0
                and n["creation"] == n["creation_after"], "IDENTITY_LIFETIME")
        return n
    observer = identity(data["observer_pid"])
    parent = identity(observer["parent_metadata"])
    require(parent["pid"] != observer["pid"] and parent["creation"] <= observer["creation"], "PARENT_IDENTITY")
    if expected_parent is not None:
        require(parent["pid"] == expected_parent, "EXPECTED_PARENT_MISMATCH")
    if expected_creation is not None:
        require(parent["creation"] == expected_creation, "EXPECTED_CREATION_MISMATCH")
    require(data["client_count"] == 2 and len(data["clients"]) == 2
            and set(data["clients"]) == {parent["pid"], observer["pid"]}, "CLIENT_SET")
    def forward(n):
        require(n["class49_status"] == 0 and n["class49_bytes"] == 8, "CLASS49_QUERY")
        require(n["class49_after_queried"] and n["class49_after_status"] == 0
                and n["class49_raw"] == n["class49_after_raw"], "CLASS49_CHANGED")
        raw = n["class49_raw"]
        require(raw & 3 == 1 and 0 < (raw & ~3) <= 0xffffffff, "CLASS49_PROFILE")
        return raw & ~3
    host_pid = forward(observer)
    require(host_pid == forward(parent), "CONSOLE_CONTEXT_MISMATCH")
    require(host_pid not in {parent["pid"], observer["pid"]}, "AUXILIARY_NOT_DISTINCT")
    host = identity(host_pid)
    for n in [observer, parent, host]:
        require(n["token_known"] and n["elevated"] and n["token_error"] == 0, "PROCESS_TOKEN_UNKNOWN")
        require(n["session"] == observer["session"] and n["token_user"] == observer["token_user"], "TOKEN_SESSION_MISMATCH")
        require(n["file_identity_known"] and n["hash_known"] and len(n["sha256"]) == 64, "FILE_EVIDENCE")
        require(n["metrics_known"] and n["metric_error"] == 0 and n["handles"] > 0, "METRIC_UNKNOWN")
    admin = parent["handles"] + host["handles"]
    require(admin <= 1024, "ADMIN_HANDLES")
    require(data["own_peak"] <= 124, "OWN_OBSERVED_RESERVE")
    current = {observer["pid"], parent["pid"], host["pid"]}
    return {
        "classification": "COHERENT_CURRENT_CONSOLE_DIAGNOSTIC_DATA_ONLY",
        "supplied_seed_matches_current_parent": data["seed_pid"] == parent["pid"],
        "supplied_seed_binding": "MATCH" if data["seed_pid"] == parent["pid"] else "DENY_CROSS_CONSOLE_SEED",
        "observer_pid": observer["pid"], "observer_creation": observer["creation"],
        "actual_parent_pid": parent["pid"], "actual_parent_creation": parent["creation"],
        "actual_auxiliary_pid": host["pid"], "actual_auxiliary_creation": host["creation"],
        "admin_handles_observed": admin, "admin_observed_difference": 1024-admin,
        "admin_private_bytes_observed": parent["private_bytes"]+host["private_bytes"],
        "admin_working_set_observed": parent["working_set"]+host["working_set"],
        "observer_peak": data["own_peak"],
        "outside_current_context_observed": sorted(set(nodes)-current),
        "discarded_unqualified_mixed_subtotal": data["unique_seed_and_class49_candidate_handles"],
        "native_proof_capacity": "NOT_DEMONSTRATED",
        "admission": "DENY", "normative_policy": "C0_C9_PROPOSED_NOT_ACCEPTED",
        "unknowns": ["authenticated console association", "mapped-image provenance", "thread impersonation/token LUID continuity", "host/boot/freshness for future operation", "administrative threads and stack", "unsampled/kernel/transitive costs", "TPM possession"],
    }

if __name__ == "__main__":
    from pathlib import Path
    data=json.loads(Path(__file__).with_name("OWNER-MANUAL-OUTPUT.json").read_text())
    print(json.dumps(review(data),indent=2))
