@echo off
"C:\Windows\Microsoft.NET\Framework64\v4.0.30319\csc.exe" /nologo /target:library /platform:x64 /warnaserror /out:build\ed11-positive-private-20261025\Selected\Release\Pacc.N0PrivateStdout.Budget.Binding.dll tools\qualification\scm-n0-private-stdout-binding.cs build\ed11-positive-private-20261025\Selected\Release\PaccFileAcl.qualified-source.cs
exit /b %errorlevel%
