# Decision — Optional personal Windows SCM hosting and installation selection

Status: ACCEPTED — explicit owner direction, 2026-10-22; documentation only.

The owner requires Windows service installation to be OPTIONAL. Preserve direct execution from CMD/PowerShell using the existing executable/console runtime. Do not install/start SCM implicitly when the user chooses console operation. SCM and console reuse the same runtime and canonical owners; optional hosting does not waive authorization, configuration, Stop, custody or ED11 requirements in either mode.

Select %ProgramFiles%\PapinhoAccelerator as the intended binary installation location. This is a destination selection, not a created installation or a factual native identity; it does not select a writable authority/configuration store or authorize ACL/data layout changes.

The owner nominated the existing .\papinho account for evaluation. It remains a candidate dedicated standard operational account pending factual eligibility, installation association, actual process TokenUser, isolation and credential qualification. Do not create or alter this account in this step. Preserve M1/M5 of DECISION-phase6-personal-mvp-20261022.md and the identity distinctions of DECISION-personal-ca-preinitialization-20261022.md. The owner's request for the usual service account does not specify a replacement principal or authorize SYSTEM/LocalService defaults; no such replacement is selected here. Account SID and SCM service SID remain distinct.

No functional implementation change, account/service/credential/ACL creation, bootstrap, key, certificate, CURRENT, signature or network effect follows. ED11 PARTIAL and operational authority DENY remain. This additive decision preserves the original accepted decisions and does not qualify NT4, box or ISA.
