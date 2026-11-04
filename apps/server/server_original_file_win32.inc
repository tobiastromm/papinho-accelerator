static int scm_register_id(const BY_HANDLE_FILE_INFORMATION *a,const BY_HANDLE_FILE_INFORMATION *b)
{return a->dwVolumeSerialNumber==b->dwVolumeSerialNumber&&a->nFileIndexHigh==b->nFileIndexHigh&&a->nFileIndexLow==b->nFileIndexLow&&
 a->nFileSizeHigh==b->nFileSizeHigh&&a->nFileSizeLow==b->nFileSizeLow&&a->dwFileAttributes==b->dwFileAttributes&&
 !memcmp(&a->ftLastWriteTime,&b->ftLastWriteTime,sizeof(FILETIME));}
static int scm_register_hash(HANDLE file,BYTE digest[32])
{
 PACC_STORAGE_SHA256 hash;BYTE block[1024];DWORD n;LARGE_INTEGER zero;int ok=0;zero.QuadPart=0;
 memset(&hash,0,sizeof(hash));
 if(!SetFilePointerEx(file,zero,NULL,FILE_BEGIN)||pacc_storage_sha_init(&hash)!=PACC_HASH_OK)goto done;
 for(;;){if(papacc_server_console_win32_stop_requested()||!ReadFile(file,block,sizeof(block),&n,NULL))goto done;
 if(!n)break;if(pacc_storage_sha_update(&hash,block,n,NULL,NULL)!=PACC_HASH_OK)goto done;}
 ok=pacc_storage_sha_final(&hash,digest,32,NULL,NULL)==PACC_HASH_OK;
done:SecureZeroMemory(&hash,sizeof(hash));SecureZeroMemory(block,sizeof(block));return ok;
}
