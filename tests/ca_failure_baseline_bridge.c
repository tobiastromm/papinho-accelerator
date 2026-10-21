/* Test-only preimage bridge. Compile the preserved backend with identical
 * synthetic NCrypt functions; never link this into a product target. */
#define _WIN32_WINNT 0x0600
#include "../docs/integration/cng-failure-origin-instrumentation-20261019/preimages/server_ca_key_win32.h"
SECURITY_STATUS WINAPI fake_provider(NCRYPT_PROV_HANDLE *,LPCWSTR,DWORD);
SECURITY_STATUS WINAPI fake_create(NCRYPT_PROV_HANDLE,NCRYPT_KEY_HANDLE *,LPCWSTR,LPCWSTR,DWORD,DWORD);
SECURITY_STATUS WINAPI fake_open(NCRYPT_PROV_HANDLE,NCRYPT_KEY_HANDLE *,LPCWSTR,DWORD,DWORD);
SECURITY_STATUS WINAPI fake_set(NCRYPT_HANDLE,LPCWSTR,PBYTE,DWORD,DWORD);
SECURITY_STATUS WINAPI fake_finalize(NCRYPT_KEY_HANDLE,DWORD);
SECURITY_STATUS WINAPI fake_get(NCRYPT_HANDLE,LPCWSTR,PBYTE,DWORD,DWORD *,DWORD);
SECURITY_STATUS WINAPI fake_export(NCRYPT_KEY_HANDLE,NCRYPT_KEY_HANDLE,LPCWSTR,NCryptBufferDesc *,PBYTE,DWORD,DWORD *,DWORD);
SECURITY_STATUS WINAPI fake_free(NCRYPT_HANDLE);
#define NCryptOpenStorageProvider fake_provider
#define NCryptCreatePersistedKey fake_create
#define NCryptOpenKey fake_open
#define NCryptSetProperty fake_set
#define NCryptFinalizeKey fake_finalize
#define NCryptGetProperty fake_get
#define NCryptExportKey fake_export
#define NCryptFreeObject fake_free
#define papacc_ca_key_create preimage_create
#define papacc_ca_key_open preimage_open
#define papacc_ca_key_validate preimage_validate
#define papacc_ca_key_close preimage_close
#include "../docs/integration/cng-failure-origin-instrumentation-20261019/preimages/server_ca_key_win32.c"
SECURITY_STATUS ca_baseline_run(const BYTE a[16],const BYTE g[16],PSID admin,PSID service,DWORD out[8]) {
 PAPACC_CA_KEY old;SECURITY_STATUS s;
 memset(&old,0,sizeof(old));s=preimage_create(&old,a,g,admin,service);
 if(s==ERROR_SUCCESS)s=preimage_close(&old);
 out[0]=(DWORD)old.state;out[1]=(DWORD)old.created;out[2]=(DWORD)old.close_attempted;out[3]=(DWORD)old.close_confirmed;
 out[4]=(DWORD)old.error;out[5]=old.key!=0;out[6]=old.provider!=0;out[7]=old.retained_token!=0;
 return s;
}