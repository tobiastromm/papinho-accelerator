using System;using System.Security.Cryptography;
class CngCfb128Fixture {
 static byte[] X(string s){byte[] b=new byte[s.Length/2];for(int i=0;i<b.Length;i++)b[i]=Convert.ToByte(s.Substring(2*i,2),16);return b;}
 static int Main(){try{using(AesCng aes=new AesCng()){aes.KeySize=128;aes.Mode=CipherMode.CFB;aes.FeedbackSize=128;aes.Padding=PaddingMode.None;aes.Key=X("2b7e151628aed2a6abf7158809cf4f3c");aes.IV=X("000102030405060708090a0b0c0d0e0f");using(ICryptoTransform t=aes.CreateEncryptor()){byte[] p=X("6bc1bee22e409f96e93d7e117393172a");string result=BitConverter.ToString(t.TransformFinalBlock(p,0,p.Length)).Replace("-","").ToLowerInvariant();if(result!="3b3fd92eb72dad20333449f8e83cfb4a")return 2;Console.WriteLine("PASS_NATIVE_CNG_AES128_CFB128_NIST_VECTOR_ONLY_NO_TPM");return 0;}}}catch(Exception e){Console.WriteLine("CNG_CFB128_UNAVAILABLE_"+e.HResult.ToString("x8"));return 2;}}
}
