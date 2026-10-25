/* Shared existing public CA fixture helpers. No private key or authority. */
static PAPACC_U8 ca_der[PAPACC_CONFIG_V3_CA_BYTES],text_ca[16384],spki[2048];
static unsigned __int64 arena[4096];
static int load_ca(const char *path,PAPACC_SIZE *bytes,PAPACC_U8 digest_key[32])
{
 FILE *f;size_t n;DWORD dn=PAPACC_CONFIG_V3_CA_BYTES,kn=sizeof(spki);PCCERT_CONTEXT cert;
 CHECK(fopen_s(&f,path,"rb")==0&&f!=NULL);n=fread(text_ca,1,sizeof(text_ca),f);CHECK(!ferror(f)&&fclose(f)==0&&n&&n<sizeof(text_ca));
 CHECK(CryptStringToBinaryA((char *)text_ca,(DWORD)n,CRYPT_STRING_BASE64,ca_der,&dn,NULL,NULL));
 cert=CertCreateCertificateContext(X509_ASN_ENCODING,ca_der,dn);CHECK(cert!=NULL);
 CHECK(CryptEncodeObjectEx(X509_ASN_ENCODING,X509_PUBLIC_KEY_INFO,&cert->pCertInfo->SubjectPublicKeyInfo,0,NULL,spki,&kn));
 CHECK(hash(&owner,spki,kn,digest_key));CHECK(CertFreeCertificateContext(cert));*bytes=dn;return 1;
}
static int bind_ca_data(const PAPACC_U8 key[32])
{
 PAPACC_U8 *image=f3+r32(f3+48)+8192,*sn=f3+r32(f3+244)+64,*pg=f3+r32(f3+260)+64;unsigned j;
 memcpy(image+136,key,32);CHECK(hash(&owner,image,8160,image+8160));
 for(j=0;j<(unsigned)f3[252];j++)memcpy(sn+768U*j+568,key,32);
 for(j=0;j<(unsigned)f3[268];j++)memcpy(pg+384U*j+260,key,32);
 return seal3();
}
