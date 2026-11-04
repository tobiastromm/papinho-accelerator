using System;
using System.Runtime.InteropServices;
class TpmDeviceInfoReadOnly {
 [StructLayout(LayoutKind.Sequential)] struct Info { public UInt32 structVersion,tpmVersion,tpmInterfaceType,tpmImpRevision; }
 [DllImport("tbs.dll",ExactSpelling=true)] static extern UInt32 Tbsi_GetDeviceInfo(UInt32 size,ref Info info);
 static int Main() {
  Info i=new Info();i.structVersion=2;
  UInt32 code=Tbsi_GetDeviceInfo((UInt32)Marshal.SizeOf(typeof(Info)),ref i);
  Console.WriteLine("TBS_RESULT=0x"+code.ToString("x8")+"; structVersion="+i.structVersion+"; tpmVersion="+i.tpmVersion+"; interface="+i.tpmInterfaceType+"; revision="+i.tpmImpRevision+"; NV_COMMANDS=NONE; AUTHORITY=NONE");
  return 0;
 }
}
