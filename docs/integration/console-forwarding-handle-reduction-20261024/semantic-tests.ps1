param([ValidateSet('redirected','interactive')][string]$Kind='redirected',[string]$Result,[string]$Tag='')
$ErrorActionPreference='Stop'
try {
Add-Type -TypeDefinition @"
using System;using System.Text;using System.Runtime.InteropServices;
public static class ForwardingConsoleFixture {
 [StructLayout(LayoutKind.Sequential)] public struct Coord {public short X,Y;}
 [DllImport("kernel32.dll",SetLastError=true)] public static extern IntPtr GetStdHandle(int n);
 [DllImport("kernel32.dll",SetLastError=true)] public static extern bool GetConsoleMode(IntPtr h,out uint mode);
 [DllImport("kernel32.dll",CharSet=CharSet.Unicode,SetLastError=true)] public static extern bool ReadConsoleOutputCharacterW(IntPtr h,StringBuilder text,uint length,Coord position,out uint read);
 [DllImport("kernel32.dll",SetLastError=true)] public static extern bool FreeConsole();
}
"@
$rows=[ordered]@{kind=$Kind;PSVersion=$PSVersionTable.PSVersion.ToString();PSEdition=$PSVersionTable.PSEdition;input_redirected=[Console]::IsInputRedirected;output_redirected=[Console]::IsOutputRedirected;default_encoding=[Console]::OutputEncoding.WebName}
$marker=('CONSOLE_'+[Guid]::NewGuid().ToString('N'));$hostMarker=('HOST_'+[Guid]::NewGuid().ToString('N'))
$transcript=Join-Path $PSScriptRoot ('transcript-'+$Kind+$Tag+'.txt');$info=Join-Path $PSScriptRoot ('information-'+$Kind+$Tag+'.txt')
Start-Transcript -LiteralPath $transcript -NoClobber | Out-Null
Write-Host $hostMarker;[Console]::Out.WriteLine($marker)
Stop-Transcript | Out-Null
& {Write-Host $hostMarker;[Console]::Out.WriteLine($marker)} 6> $info
$trans=Get-Content -Raw -LiteralPath $transcript;$information=Get-Content -Raw -LiteralPath $info
$rows.transcript_host=$trans.Contains($hostMarker);$rows.transcript_console=$trans.Contains($marker);$rows.information_host=$information.Contains($hostMarker);$rows.information_console=$information.Contains($marker)
if(-not$rows.information_host-or$rows.information_console){throw 'STREAM_SEMANTICS'}
$rows.native_protocol=[ordered]@{stdout_decoder='Existing Process.StandardOutput unchanged';ASCII='Protocol strings unchanged';Unicode_managed='Covered by loop StringWriter fixture';default_encoding_changed_by_product=$false}
if($Kind-eq'interactive'){
 if($rows.input_redirected-or$rows.output_redirected){throw 'INTERACTIVE_FIXTURE_REDIRECTED'}
 $inputMode=[uint32]0;$outputMode=[uint32]0;$rows.native_input_mode=[ForwardingConsoleFixture]::GetConsoleMode([ForwardingConsoleFixture]::GetStdHandle(-10),[ref]$inputMode);$rows.native_output_mode=[ForwardingConsoleFixture]::GetConsoleMode([ForwardingConsoleFixture]::GetStdHandle(-11),[ref]$outputMode)
 if(-not$rows.native_input_mode-or-not$rows.native_output_mode){throw 'NO_NATIVE_CONSOLE'}
 $expected='ASCII ação 漢字 😀';$position=[ForwardingConsoleFixture+Coord]::new();$position.X=[int16][Console]::CursorLeft;$position.Y=[int16][Console]::CursorTop
 [Console]::Out.WriteLine($expected);$buffer=[Text.StringBuilder]::new(128);$read=[uint32]0
 $rows.screen_read_ok=[ForwardingConsoleFixture]::ReadConsoleOutputCharacterW([ForwardingConsoleFixture]::GetStdHandle(-11),$buffer,60,$position,[ref]$read)
 $rows.screen_text=$buffer.ToString().Substring(0,[int]$read);$rows.screen_contains_expected=$rows.screen_text.Contains($expected)
 $position.Y=[int16][Console]::CursorTop;$position.X=[int16][Console]::CursorLeft
 Write-Host $expected;$buffer=[Text.StringBuilder]::new(128);$read=[uint32]0
 $null=[ForwardingConsoleFixture]::ReadConsoleOutputCharacterW([ForwardingConsoleFixture]::GetStdHandle(-11),$buffer,60,$position,[ref]$read)
 $rows.write_host_unicode=$buffer.ToString().Substring(0,[int]$read)
 $rows.write_host_unicode_exact=$rows.write_host_unicode.Contains($expected)
 $protocol='IG_CENSUS_NAME_UTF16=006f006c00e100204e2d6587'
 $position.Y=[int16][Console]::CursorTop;$position.X=[int16][Console]::CursorLeft
 [Console]::Out.WriteLine($protocol);$buffer=[Text.StringBuilder]::new(128);$read=[uint32]0
 $null=[ForwardingConsoleFixture]::ReadConsoleOutputCharacterW([ForwardingConsoleFixture]::GetStdHandle(-11),$buffer,60,$position,[ref]$read)
 $rows.ascii_hex_protocol_exact=$buffer.ToString().Substring(0,[int]$read).Contains($protocol)
 if(-not$rows.ascii_hex_protocol_exact){throw 'NATIVE_ASCII_PROTOCOL_LOSS'}
 [Console]::OutputEncoding=[Text.UTF8Encoding]::new($false)
 $position.Y=[int16][Console]::CursorTop;$position.X=[int16][Console]::CursorLeft
 [Console]::Out.WriteLine($expected);$buffer=[Text.StringBuilder]::new(128);$read=[uint32]0
 $null=[ForwardingConsoleFixture]::ReadConsoleOutputCharacterW([ForwardingConsoleFixture]::GetStdHandle(-11),$buffer,60,$position,[ref]$read)
 $rows.fixture_utf8_unicode=$buffer.ToString().Substring(0,[int]$read)
 $rows.fixture_utf8_unicode_exact=$rows.fixture_utf8_unicode.Contains($expected)
 $rows.fixture_only_encoding_change='Not applied to product; proves encoding precondition, not a new authority or operational gate'

 $rows.detached=[ForwardingConsoleFixture]::FreeConsole();$rows.console_loss_test='FreeConsole detach only;not CTRL_CLOSE_EVENT or proof of governed cleanup'
 try{[Console]::Out.WriteLine('AFTER_DETACH');$rows.after_detach_direct='NO_EXCEPTION'}catch{$rows.after_detach_direct=$_.Exception.ToString()}
 try{Write-Host 'HOST_AFTER_DETACH';$rows.after_detach_host='NO_EXCEPTION'}catch{$rows.after_detach_host=$_.Exception.ToString()}
}else{
 # Only the fixture changes encoding for a second explicitly UTF8 redirected case.
 [Console]::OutputEncoding=[Text.UTF8Encoding]::new($false)
 [Console]::Out.WriteLine('UTF8_REDIRECT ação 漢字 😀');[Console]::Out.WriteLine('')
 $rows.fixture_utf8_encoding=[Console]::OutputEncoding.WebName
}
[IO.File]::WriteAllText($Result,($rows|ConvertTo-Json -Depth 6),[Text.UTF8Encoding]::new($false))

} catch { $failure=[ordered]@{error=$_.Exception.ToString();line=$_.InvocationInfo.ScriptLineNumber;position=$_.InvocationInfo.PositionMessage};[IO.File]::WriteAllText($Result,($failure|ConvertTo-Json -Depth 5),[Text.UTF8Encoding]::new($false));exit 1 }
