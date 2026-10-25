@echo off
"C:\Windows\Microsoft.NET\Framework64\v4.0.30319\csc.exe" /nologo /target:library /platform:x64 /warnaserror /out:build\ed11-private-stdout-20261025\Selected\Release\Pacc.N0PrivateStdout.Binding.dll tools\qualification\scm-n0-private-stdout-binding.cs
if errorlevel 1 exit /b %errorlevel%
"C:\Windows\Microsoft.NET\Framework64\v4.0.30319\csc.exe" /nologo /target:library /platform:x64 /warnaserror /out:build\ed11-private-stdout-20261025\Selected\Release\Pacc.TransportConsole.Test.dll tests\noninherited_transport_console_test.cs
exit /b %errorlevel%
