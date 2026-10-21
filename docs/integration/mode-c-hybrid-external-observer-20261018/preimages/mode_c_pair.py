"""Minimal receipt-reference extension of the preserved joint E10 consumer.
No network, admission, credit mutation or native qualification. CLI reads files only.
"""
import hashlib
import importlib.util
import json
from pathlib import Path
import re
import sys

BASE = Path(__file__).resolve().parents[2]
LEGACY = BASE / 'build/phase6-c2-c3-20261012/c7-e10-e5-e11-20261014/finite-implementation-20261014'
MAX = 262144
U64 = (1 << 64) - 1


def historical_joint(server_raw, browser_runs):
    """Preserve the existing two-operation v4 verifier; no reinterpretation."""
    sys.path.insert(0, str(LEGACY))
    try:
        spec = importlib.util.spec_from_file_location('preserved_joint_v4', LEGACY / 'joint_v4.py')
        module = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(module)
        return module.joint_metadata(server_raw, browser_runs)
    finally:
        sys.path.pop(0)


class Invalid(ValueError):
    pass


def require(ok, reason):
    if not ok:
        raise Invalid(reason)


def keys(d, names):
    require(type(d) is dict and set(d) == set(names.split()), 'FIELDS')


def integer(v, positive=False):
    require(type(v) is int and (1 if positive else 0) <= v <= U64, 'INTEGER')


def digest(v):
    require(type(v) is str and re.fullmatch('[0-9a-f]{64}', v) is not None, 'DIGEST')


def canonical(value):
    return json.dumps(value, sort_keys=True, separators=(',', ':'), ensure_ascii=True).encode('ascii')


def unique(pairs):
    out = {}
    for k, v in pairs:
        require(k not in out, 'DUPLICATE_KEY')
        out[k] = v
    return out


def unpack(raw):
    require(type(raw) is bytes and len(raw) <= MAX, 'INPUT_BOUND')
    env = json.loads(raw, object_pairs_hook=unique, parse_constant=lambda _: (_ for _ in ()).throw(Invalid('NUMBER')))
    keys(env, 'format version body body_bytes sha256')
    require(env['format'] == 'MODE_C_RECEIPT_REFERENCES' and type(env['version']) is int and env['version'] == 1, 'VERSION')
    body = canonical(env['body'])
    integer(env['body_bytes']); digest(env['sha256'])
    require(len(body) == env['body_bytes'] and hashlib.sha256(body).hexdigest() == env['sha256'], 'INTEGRITY')
    return env['body']


JOIN = 'request operation generation epoch attempt resolution'.split()
# Role-local stages; no ordering comparison of clocks or local Session IDs.
STAGES = ['OPEN_RESULT', 'BIND_APP', 'ACK_APP', 'BIND_RECORD', 'ACK_RECORD',
          'TLS_READY', 'PREFIXES', 'FENCES', 'ACKS', 'FINAL', 'STOP', 'TEARDOWN']


def consume_mode_c(browser_raw, accelerator_raw, artifacts):
    """Validate two minimal reference receipts against supplied immutable artifacts.

    Artifacts are explicit bytes keyed by SHA256, never paths or live owners.
    Normalized public facts must be produced by existing receipt adapters. Their
    authenticity is not established by hashes; output labels this limitation.
    """
    result = {'status': 'INCOMPLETE', 'reason': 'MISSING_EVIDENCE',
              'ED11_PASS': False, 'admission': 'DENY', 'native_qualified': False,
              'provenance': 'REPORTED_ARTIFACTS_NOT_AUTHENTICATED'}
    missing = []
    try:
        require(type(artifacts) is dict and len(artifacts) <= 16, 'ARTIFACT_BOUND')
        require(all(type(v) is bytes for v in artifacts.values()), 'ARTIFACT_TYPE')
        require(sum(len(v) for v in artifacts.values()) <= 4 * MAX, 'ARTIFACT_BYTES')
        if browser_raw is None or accelerator_raw is None:
            return result
        receipts = [unpack(browser_raw), unpack(accelerator_raw)]
        identities = []
        facts_by_role = {}
        for role, b in zip(('BROWSER', 'ACCELERATOR'), receipts):
            keys(b, 'role run process creation image sdk build_ref origin join refs incomplete' + (' producer_ref' if 'producer_ref' in b else ''))
            require(b['role'] == role, 'ROLE')
            require(type(b['run']) is str and 1 <= len(b['run']) <= 127 and b['run'].isascii() and all(32 <= ord(c) < 127 for c in b['run']), 'RUN')
            integer(b['process'], True); integer(b['creation'], True)
            require(b['origin'] in ('NATIVE', 'SYNTHETIC') and type(b['incomplete']) is bool, 'ORIGIN')
            if b['incomplete']:
                missing.append(role + ':CAPTURE_INCOMPLETE')
            for k in ('image', 'sdk', 'build_ref'):
                digest(b[k])
                if b[k] not in artifacts:
                    missing.append(role + ':' + k)
                else:
                    require(hashlib.sha256(artifacts[b[k]]).hexdigest() == b[k], 'ARTIFACT_HASH')
            identities.append((b['process'], b['creation']))
            if b['join'] is None:
                missing.append(role + ':JOIN')
                facts_by_role[role] = {}
                continue
            keys(b['join'], ' '.join(JOIN))
            for k in JOIN:
                integer(b['join'][k], True)
            require(b['join']['attempt'] <= 0xffffffff and b['join']['resolution'] <= 0xffffffff, 'OPEN_PROVENANCE_RANGE')
            require(type(b['refs']) is list and len(b['refs']) <= 32, 'REF_BOUND')
            found = {}; seen = set(); last = 0
            for ref in b['refs']:
                keys(ref, 'artifact sequence kind')
                digest(ref['artifact']); integer(ref['sequence'], True)
                require(ref['kind'] in STAGES and ref['kind'] not in found, 'DUPLICATE_OR_KIND')
                key = (ref['artifact'], ref['sequence'])
                require(key not in seen, 'DUPLICATE_REFERENCE'); seen.add(key)
                if ref['artifact'] not in artifacts:
                    missing.append(role + ':' + ref['kind']); found[ref['kind']] = None
                    continue
                raw = artifacts[ref['artifact']]
                require(len(raw) <= MAX and hashlib.sha256(raw).hexdigest() == ref['artifact'], 'ARTIFACT_HASH')
                source = json.loads(raw, object_pairs_hook=unique)
                keys(source, 'role run process creation origin join facts')
                require(all(source[k] == b[k] for k in ('role', 'run', 'process', 'creation', 'origin', 'join')), 'SOURCE_IDENTITY')
                integer(source['process'], True); integer(source['creation'], True)
                keys(source['join'], ' '.join(JOIN))
                for name in JOIN: integer(source['join'][name], True)
                require(type(source['facts']) is list and len(source['facts']) <= 32, 'FACT_BOUND')
                all_sequences = []
                for item in source['facts']:
                    keys(item, 'sequence kind action values')
                    integer(item['sequence'], True)
                    all_sequences.append(item['sequence'])
                require(len(set(all_sequences)) == len(all_sequences) and all_sequences == sorted(all_sequences), 'SOURCE_SEQUENCE')
                matched = [f for f in source['facts'] if f['sequence'] == ref['sequence']]
                require(len(matched) == 1 and matched[0]['kind'] == ref['kind'], 'REFERENCE_DIVERGENCE')
                fact = matched[0]
                require(ref['sequence'] > last, 'LOCAL_SEQUENCE'); last = ref['sequence']
                expected = ('VALIDATED_RECEIVE' if role == 'BROWSER' else 'FULL_WRITE') if ref['kind'] in ('OPEN_RESULT', 'ACK_APP', 'ACK_RECORD', 'TLS_READY', 'FINAL') else ('FULL_WRITE' if role == 'BROWSER' else 'VALIDATED_RECEIVE') if ref['kind'] in ('BIND_APP', 'BIND_RECORD') else 'LOCAL_FACT'
                require(fact['action'] == expected, 'NOT_RECEIPT_OR_FULL_WRITE')
                found[ref['kind']] = fact
            for kind in STAGES:
                if kind not in found:
                    missing.append(role + ':' + kind)
            # BIND must precede its ACK; both ACKs precede verified TLS/application.
            edges = [('OPEN_RESULT','BIND_APP'),('OPEN_RESULT','BIND_RECORD'),
                     ('BIND_APP','ACK_APP'),('BIND_RECORD','ACK_RECORD'),
                     ('ACK_APP','TLS_READY'),('ACK_RECORD','TLS_READY'),
                     ('TLS_READY','PREFIXES'),('PREFIXES','FENCES'),('FENCES','ACKS'),
                     ('ACKS','FINAL'),('FINAL','TEARDOWN'),('STOP','TEARDOWN')]
            for a, z in edges:
                if found.get(a) and found.get(z):
                    require(found[a]['sequence'] < found[z]['sequence'], 'CAUSAL_ORDER')
            for kind, f in found.items():
                if not f:
                    continue
                v = f['values']
                if kind == 'TLS_READY':
                    keys(v, 'version verification alpn'); require(v == {'version': 0x0304, 'verification': 7, 'alpn': 'http/1.1'}, 'TLS_FACTS')
                elif kind in ('PREFIXES', 'FENCES', 'ACKS'):
                    require(type(v) is list and len(v) == 4, 'PREFIX_COUNT')
                    for n in v: integer(n)
                elif kind == 'TEARDOWN':
                    keys(v, 'disposition callbacks leases')
                    require(v['disposition'] in ('DESTROYED','PRECONDITION_RETAINED','CLOSE_UNRESOLVED_RETAINED'), 'DISPOSITION')
                    integer(v['callbacks']); integer(v['leases'])
                    if v['disposition'] != 'DESTROYED' or v['callbacks'] or v['leases']:
                        missing.append(role + ':RETAINED_OR_PENDING')
                elif kind == 'FINAL':
                    keys(v, 'outcome ownership'); require(type(v['outcome']) is int and type(v['ownership']) is int and v['outcome'] in (1,2,3) and v['ownership'] in (0,1,2,3), 'FINAL')
                    if v['outcome'] != 1 or v['ownership'] != 1:
                        missing.append(role + ':NON_CLEAN_FINAL')
                else:
                    require(v is None, 'UNEXPECTED_VALUES')
            if all(found.get(k) for k in ('PREFIXES','FENCES','ACKS')):
                require(found['PREFIXES']['values'] == found['FENCES']['values'] == found['ACKS']['values'], 'PREFIX_DIVERGENCE')
            facts_by_role[role] = found
        require(identities[0] != identities[1], 'SAME_PROCESS_LIFETIME')
        a, z = receipts
        if a['join'] is not None and z['join'] is not None:
            require(a['join'] == z['join'], 'PAIR_JOIN_DIVERGENCE')
        require(a['origin'] == z['origin'], 'ORIGIN_MIX')
        for kind in ('TLS_READY','PREFIXES','FINAL'):
            x, y = facts_by_role['BROWSER'].get(kind), facts_by_role['ACCELERATOR'].get(kind)
            if x and y: require(x['values'] == y['values'], 'CROSS_RECEIPT_DIVERGENCE')
        # Protocol identity plus complementary write/validated-receive facts joins
        # reported execution. This never authenticates raw caller-supplied files.
        result.update(status='INCOMPLETE' if missing else 'CORRELATED',
                      reason=','.join(sorted(set(missing))) if missing else 'MATCHING_REPORTED_RECEIPTS',
                      evidence_origin=a['origin'])
        return result
    except (Invalid, ValueError, KeyError, TypeError, OverflowError, RecursionError, UnicodeError) as exc:
        result.update(status='INVALID', reason=str(exc)[:120] if isinstance(exc, Invalid) else type(exc).__name__)
        return result



def recorded_time_coherence(p):
    """Recorded local facts only. No Mode C freshness policy is installed.
    Coherence with a recorded expiry is NOT approval of that expiry for Mode C.
    No threshold or comparison of Browser and Accelerator clocks is made.
    """
    result = {'freshness_policy':'NOT_ESTABLISHED','validity_result':'INCOMPLETE',
              'temporal_coherence':'ABSENT','clock_domain':None,
              'observed_ms':None,'validated_ms':None,'valid_until_ms':None}
    t = p.get('timestamps')
    if t is None:
        return result
    keys(t, 'observed_ms validated_ms valid_until_ms clock recorded_validity mode_c_policy_ref')
    for name in ('observed_ms','validated_ms','valid_until_ms'):
        if t[name] is not None:
            integer(t[name])
        result[name] = t[name]
    if t['clock'] is None:
        return result
    clock = t['clock']; keys(clock, 'source process creation')
    integer(clock['process'], True); integer(clock['creation'], True)
    require(clock['process'] == p['process'] and clock['creation'] == p['creation'], 'CLOCK_DOMAIN_DIVERGENCE')
    result['clock_domain'] = clock
    if clock['source'] != 'PAL_MONOTONIC_NS_TO_MS':
        result['temporal_coherence'] = 'UNVERIFIED_CLOCK_SOURCE'
        return result
    if t['recorded_validity'] is None:
        result['temporal_coherence'] = 'VALIDITY_REFERENCE_ABSENT'
        return result
    ref = t['recorded_validity']; keys(ref, 'producer sequence invocation')
    integer(ref['sequence'], True); integer(ref['invocation'], True)
    require(ref['producer'] == 'PAPACC_C8_TARGET_EVIDENCE' and ref['sequence'] == p['sequence'] and ref['invocation'] == p['invocation'], 'VALIDITY_REFERENCE_DIVERGENCE')
    if any(t[k] is None for k in ('observed_ms','validated_ms','valid_until_ms')):
        return result
    if t['validated_ms'] < t['observed_ms']:
        result['temporal_coherence'] = 'REGRESSIVE'
    elif t['valid_until_ms'] < t['observed_ms']:
        result['temporal_coherence'] = 'INVERTED_RECORDED_INTERVAL'
    elif t['validated_ms'] > t['valid_until_ms']:
        result['temporal_coherence'] = 'OUTSIDE_RECORDED_VALIDITY'
    else:
        result['temporal_coherence'] = 'COHERENT'
    # Even a supplied non-null policy reference cannot create an approved rule.
    # No approved Mode C applicability registry/authority is implemented here.
    return result

def validate_producer_binding(receipt_raw, process_raw, launch_raw, build_raw):
    """Consistency of existing receipt, native self-observation and controlled
    launcher/build evidence. Does not authenticate arbitrary supplied JSON.
    Neither launch evidence nor an expected hash is itself an admission permit.
    """
    out = {'status': 'INCOMPLETE', 'reason': 'MISSING_PRODUCER',
           'admission': 'DENY', 'ED11_PASS': False, 'native_qualified': False,
           'provenance': 'CONSISTENCY_ONLY_NOT_REMOTE_ATTESTATION',
           'freshness_policy':'NOT_ESTABLISHED','validity_result':'INCOMPLETE'}
    try:
        b = unpack(receipt_raw)
        if process_raw is None or launch_raw is None or build_raw is None or 'producer_ref' not in b:
            return out
        require(all(type(raw) is bytes and len(raw) <= MAX for raw in (process_raw, launch_raw, build_raw)), 'PROVENANCE_BOUND')
        digest(b['producer_ref'])
        require(hashlib.sha256(process_raw).hexdigest() == b['producer_ref'], 'PRODUCER_HASH')
        p = json.loads(process_raw, object_pairs_hook=unique)
        l = json.loads(launch_raw, object_pairs_hook=unique)
        build = json.loads(build_raw, object_pairs_hook=unique)
        # Public fields are observed inside the existing producer; launcher
        # holds its own child's process handle, independently checks birth/PID,
        # and hashes the path actually reported by that child before/after exit.
        require(p['format'] == {'ACCELERATOR':'C8_OWN_PROCESS_PROVENANCE','BROWSER':'BROWSER_OWN_PROCESS_PROVENANCE'}[b['role']] and type(p['version']) is int and p['version'] == 1, 'PRODUCER_VERSION')
        require(p['origin'] == b['origin'] and l['origin'] == ('NATIVE_CONTROLLED_OFFLINE_CHILD' if b['origin'] == 'NATIVE' else 'SYNTHETIC'), 'PRODUCER_ORIGIN')
        for k in ('process', 'creation'):
            integer(p[k], True); integer(l[k], True)
            require(p[k] == l[k] == b[k], 'PROCESS_LIFETIME')
        integer(p['thread'], True); integer(p['invocation'], True); integer(p['sequence'], True)
        for name in ('revision','epoch','configured','current'): integer(p[name])
        require(p['configured'] in (0,1) and p['current'] in (0,1), 'CONTEXT_FLAGS')
        require(type(p['image_path']) is str and type(p['mapped_path']) is str and p['mapped_path'].startswith('\\Device\\'), 'OS_IMAGE_PATH')
        path = lambda x: x.replace('\\', '/').casefold()
        require(path(p['image_path']) == path(l['observed_module_path']) == path(l['executed_path']), 'OBSERVED_IMAGE_PATH')
        require(p['mapped_path'] == l['mapped_image_path'], 'MAPPED_IMAGE_PATH')
        for k in ('actual_file_sha256', 'file_sha256_before'):
            require(type(l[k]) is str, 'DIGEST_TYPE'); digest(l[k].lower())
        require(l['actual_file_sha256'].lower() == l['file_sha256_before'].lower() == b['image'] == build['image_sha256'], 'EXECUTED_IMAGE_HASH')
        require(hashlib.sha256(build_raw).hexdigest() == b['build_ref'], 'BUILD_REFERENCE')
        digest(build['sdk_artifact_sha256']); digest(build['sdk_compiled_identity']); digest(p['sdk_inventory'])
        require(build['sdk_profile'] == 'NORMAL_PST_065_API3_SPI4' and build['sdk_artifact_sha256'] == b['sdk'] and build['sdk_compiled_identity'] == p['sdk_inventory'], 'SDK_BINDING')
        require(type(p['build_debug']) is int and p['build_debug'] in (0,1) and build['configuration'] == ('Debug' if p['build_debug'] else 'Release'), 'BUILD_CONFIGURATION')
        require(type(l['exit']) is int and l['exit'] == 0, 'PRODUCER_EXIT')
        require(p['provider'] == p['kernel'] == 'UNKNOWN' and p['admission'] == 'DENY', 'RESOURCE_SCOPE')
        for name in ('private','baseline','peak','handles','physical','commit'):
            integer(p['ed11'][name])
        require(p['ed11']['baseline'] <= p['ed11']['peak'] and p['ed11']['private'] <= p['ed11']['peak'], 'BASELINE_PEAK')
        out.update(recorded_time_coherence(p))
        # A real baseline is useful provenance, NOT a current operation epoch.
        # Keep the process association and resource scope distinct from grants.
        if b['join'] is None or not p['configured'] or not p['current'] or not p['revision'] or not p['epoch']:
            out.update(reason='PROCESS_BOUND_BASELINE_ONLY', process_binding='CONSISTENT_REPORTED_OS_OBSERVATION')
            return out
        require(p['epoch'] == b['join']['epoch'], 'PRODUCER_EPOCH')
        out.update(reason='MODE_C_VALIDITY_NOT_ESTABLISHED', process_binding='CONSISTENT_REPORTED_OS_OBSERVATION')
        return out
    except (Invalid, ValueError, KeyError, TypeError, OverflowError, UnicodeError, RecursionError) as exc:
        out.update(status='INVALID', reason=str(exc)[:120] if isinstance(exc, Invalid) else type(exc).__name__)
        return out


def consume_provenanced_mode_c(browser_raw, accelerator_raw, artifacts, producers):
    """Required producer-binding gate layered on the existing receipt verifier.
    producers maps role to (process bytes, controlled launch bytes, build bytes).
    Old reported-only entry remains available without stronger claims.
    """
    result = consume_mode_c(browser_raw, accelerator_raw, artifacts)
    result.update(freshness_policy='NOT_ESTABLISHED',validity_result='INCOMPLETE')
    if result['status'] == 'INVALID':
        return result
    if type(producers) is not dict or set(producers) != {'BROWSER','ACCELERATOR'}:
        return dict(result, status='INCOMPLETE', reason='MISSING_PRODUCER_BINDING')
    bindings = {}
    for role, raw in (('BROWSER',browser_raw),('ACCELERATOR',accelerator_raw)):
        values = producers[role]
        if type(values) is not tuple or len(values) != 3:
            return dict(result, status='INVALID', reason='PRODUCER_FIELDS')
        bindings[role] = validate_producer_binding(raw, *values)
    if any(v['status'] == 'INVALID' for v in bindings.values()):
        return dict(result, status='INVALID', reason='PRODUCER_DIVERGENCE', producer_bindings=bindings)
    if any(v['status'] != 'CORRELATED' for v in bindings.values()):
        return dict(result, status='INCOMPLETE', reason='PRODUCER_INCOMPLETE', producer_bindings=bindings)
    return dict(result, producer_bindings=bindings)

def main():
    # Explicit artifact paths only; no network, directory search or live APIs.
    if len(sys.argv) < 3:
        raise SystemExit('usage: mode_c_pair.py browser.json accelerator.json [artifact ...]')
    def read(p, limit):
        with open(p, 'rb') as f:
            data = f.read(limit + 1)
        require(len(data) <= limit, 'INPUT_BOUND')
        return data
    artifacts = {}
    for path in sys.argv[3:]:
        data = read(path, MAX); artifacts[hashlib.sha256(data).hexdigest()] = data
    out = consume_mode_c(read(sys.argv[1], MAX), read(sys.argv[2], MAX), artifacts)
    print(json.dumps(out, sort_keys=True))
    return 0 if out['status'] == 'CORRELATED' else 2 if out['status'] == 'INCOMPLETE' else 1


if __name__ == '__main__':
    raise SystemExit(main())