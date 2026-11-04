# Primary-source checks for the provider/kernel recipe

Checked2026-10-25. These sources support interpretation, not a capacity ceiling or mapped-provider attestation.

Microsoft [PROCESS_MEMORY_COUNTERS_EX](https://learn.microsoft.com/en-us/windows/win32/api/psapi/ns-psapi-process_memory_counters_ex) documents process memory statistics: working set and pool usage in bytes, and PrivateUsage as process private commit. The producer observes that process scope; attribution to one connection/provider would require additional evidence. Adding managed objects or working set to private commit would not create a demonstrated disjoint total. This latter accounting constraint follows the recipe's scope separation, not a new Windows allocation guarantee.

Microsoft [setsockopt](https://learn.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-setsockopt) documents receive/send buffer reservations for SO_RCVBUF/SO_SNDBUF. Inference: those option values alone do not supply a bound for every other kernel/transport allocation or for TLS provider memory. No socket option was changed or network fixture executed here.

OpenSSL3.5 [OPENSSL_malloc](https://docs.openssl.org/3.5/man3/OPENSSL_malloc/) documents memory allocator callbacks and the requirement to replace defaults before allocations occur. Its optional debug allocation counts are conditional on build configuration and count allocation calls, not perConnection total memory. The current normal SDK exposes no such perConnection memory recipe to these consumers. No process-global allocator hook, library rebuild or late replacement was installed; no OpenSSL fact is transferred to the NSS backend.

The local SDK's PST_PROVIDER_INFO describes identity/availability/capabilities rather than byte ceilings; process pool samples and SDK/package hashes cannot fill that gap. Root and Browser native own-process test logs are observations in fixtures only. Thus provider/kernel perConnection values remain UNKNOWN, and supported hard bounds versus a narrowly scoped empirical policy remains an owner decision after evidence. No quotient/delta of fixture samples is proposed as a ceiling.
