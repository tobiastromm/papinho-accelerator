from pathlib import Path
import subprocess,json
r=Path.cwd();p=Path(__file__).resolve().parent;out=r/'build/q7-source-pin-incremental-fixture-20261025';out.mkdir(exist_ok=True)
cmake='''cmake_minimum_required(VERSION 3.20)
project(Q7PinDependencyFixture C)
# Simulate unavailable automatic include discovery locally, not in product.
set(CMAKE_C_CL_SHOWINCLUDES_PREFIX "INTENTIONALLY_UNMATCHED_FIXTURE_PREFIX")
add_executable(pin_probe pin_probe.c)
target_compile_options(pin_probe PRIVATE /W4 /WX)
if(EXPLICIT_PIN_DEP)
 set_property(SOURCE pin_probe.c APPEND PROPERTY OBJECT_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/generated-pin.h")
endif()
'''
rows=[]
for explicit in [False,True]:
 src=out/('explicit' if explicit else 'automatic-missing');src.mkdir(exist_ok=True);(src/'CMakeLists.txt').write_text(cmake);(src/'pin_probe.c').write_text('#include <stdio.h>\n#include "generated-pin.h"\nint main(void){puts(PIN_VALUE);return 0;}\n');header=src/'generated-pin.h';header.write_text('#define PIN_VALUE "INITIAL_A"\n');b=src/'build'
 def build(configure,stage):
  cmd=p/f'fixture-{explicit}-{stage}.cmd';s='@echo off\nset VSLANG=1033\ncall "C:\\Program Files (x86)\\Microsoft Visual Studio\\18\\BuildTools\\VC\\Auxiliary\\Build\\vcvars64.bat"\nif errorlevel 1 exit /b %errorlevel%\n'
  if configure:s+='cmake -S '+src.as_posix()+' -B '+b.as_posix()+' -G Ninja -DCMAKE_BUILD_TYPE=Release -DEXPLICIT_PIN_DEP='+('ON' if explicit else 'OFF')+'\nif errorlevel 1 exit /b %errorlevel%\n'
  s+='cmake --build '+b.as_posix()+' --target pin_probe\nexit /b %errorlevel%\n';cmd.write_text(s);q=subprocess.run(['cmd','/c',str(cmd)],capture_output=True);(p/f'fixture-{explicit}-{stage}.log').write_bytes(q.stdout+q.stderr);assert q.returncode==0
 build(True,'initial');initial=subprocess.check_output([str(b/'pin_probe.exe')],text=True).strip();assert initial=='INITIAL_A'
 header.write_text('#define PIN_VALUE "UPDATED_B_LONGER"\n');build(False,'incremental');after=subprocess.check_output([str(b/'pin_probe.exe')],text=True).strip();assert after==('UPDATED_B_LONGER' if explicit else 'INITIAL_A'),(explicit,after)
 rows.append(dict(explicit_dependency=explicit,initial=initial,after_header_change=after,expected='UPDATED_B_LONGER' if explicit else 'INITIAL_A',status='PASS',scope='ISOLATED_BUILD_GRAPH_ONLY_NO_PRODUCT_HEADER_MUTATION'))
(p/'incremental-build-regression.json').write_text(json.dumps(rows,indent=2)+'\n');print(json.dumps(rows,indent=2))
