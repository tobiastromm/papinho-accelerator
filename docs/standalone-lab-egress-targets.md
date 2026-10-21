# Standalone controlled lab egress targets

`papacc_lab_egress_targets.exe` is a lab-only simulated remote-site launcher.
It exposes plaintext HTTP and TLS 1.3 HTTPS sites on loopback. It does not
expose PACC and does not participate in the Browser-to-Accelerator path.

## Start and stop

Choose two unused ports explicitly and a unique named stop event:

```powershell
$ready = Join-Path $env:TEMP 'papacc-lab-targets.ready'
$stop = 'Local\PapaccStandaloneLabTargetsStop'
.\build\ninja\papacc_lab_egress_targets.exe `
  --http-port 18080 --https-port 18443 `
  --ready-file $ready --stop-event $stop
```

The process atomically publishes the ready file after both listeners are
active. Stop it by setting the named manual-reset event. It waits for workers,
releases PST and sockets, removes the ready file and exits. The selected ports
are lab values, not production defaults.

```text
HTTP  = 127.0.0.1:<http-port>, body papacc-tcp-plain-ok
HTTPS = 127.0.0.1:<https-port>, TLS 1.3, body papacc-mode-b-ok
```

The HTTPS certificate has `DNS:papacc-test-server`. The Accelerator opens the
target by that exact name; the bounded resolver maps it to the separately
authorized endpoint `127.0.0.1:<https-port>`. Certificate and peer-name
verification stay enabled. No hosts-file or public DNS entry is required.
Both listeners use the existing bounded multi-accept implementation. The
dedicated `papacc.lab_egress_targets.multi_connection` regression starts one
standalone process, observes readiness, completes three direct HTTP connections
and three direct TLS 1.3 HTTPS connections without restart, verifies the HTTPS
certificate and `papacc-test-server` identity, and then proves clean stop. The
real-server regression separately proves controlled HTTP and HTTPS through
`papacc_server.exe`.

## Policy and real server

For ports `18080` and `18443`, set:

```powershell
$env:PAPACC_EGRESS_POLICY_REF = 'nt4-lab-controlled'
$env:PAPACC_EGRESS_PROFILE_REF = 'default-route'
$env:PAPACC_EGRESS_RULE_0_HOST = 'papacc-test-server'
$env:PAPACC_EGRESS_RULE_0_IPV4 = '127.0.0.1'
$env:PAPACC_EGRESS_RULE_0_PORT = '18443'
$env:PAPACC_EGRESS_RULE_1_HOST = 'papacc-plain-test-server'
$env:PAPACC_EGRESS_RULE_1_IPV4 = '127.0.0.1'
$env:PAPACC_EGRESS_RULE_1_PORT = '18080'
$env:PAPACC_OUTBOUND_TLS_PROFILE_REF = 'lab-private-ca'
$env:PAPACC_OUTBOUND_TLS_CA_DER_B64 = '<path-to-ca.der.b64>'
```

Together with the Secure Principal variables, start the production executable:

```powershell
.\build\ninja\papacc_server.exe --port 4433 --all-interfaces `
  --listener-profile secure-principal `
  --security-configuration-ref nt4-lab-secure `
  --allow-network-egress --egress-policy-ref nt4-lab-controlled `
  --egress-profile-ref default-route `
  --outbound-tls-profile-ref lab-private-ca --log-level info
```

The Browser uses the current IPv4 of an UP interface reachable from NT4. Run
`papacc_server.exe --list-interfaces`; optionally copy its Persistent ID
unchanged into `--interface-id`. CONTROL and fresh DATA connections use the
same port. Only the exact hostname+port and endpoint+port pairs above are
allowed; all other sensitive destinations remain denied. Inbound client trust
is not reused as outbound site trust.

## Controlled Browser Mode A TLS 1.2 origin

`papacc_mode_a_tls12_target.exe` is a separate test-only HTTPS origin. Its
default endpoint is loopback port `19443`, its certificate identity is
`localhost`, and its TLS policy fixes both minimum and maximum
to TLS 1.2. It returns `HTTP/1.1 200 OK` with body
`papinho-mode-a-tls12-ok`. It does not implement PACC, CONTROL, DATA, Secure
Principal, Network Egress, or production server behavior.

The target accepts optional `--port`, `--ready-file`, and `--stop-event`
arguments. The automated `papacc.mode_a_tls12_target.integration` regression
uses an ephemeral port, completes three sequential verified TLS 1.2 requests,
rejects a TLS-1.3-only client and a wrong expected hostname, and proves bounded
event-driven shutdown.

This fixture preserves the Browser's current local BearSSL behavior. The
Browser's current suppression of `BR_ERR_X509_NOT_TRUSTED` remains unchanged;
therefore this current-scope fixture proves protocol compatibility, hostname
path behavior and HTTP over TLS, but does **not** prove governed certificate
chain trust or complete server authentication in Browser Mode A. That trust
hardening is deferred to a separate Papinho project.
