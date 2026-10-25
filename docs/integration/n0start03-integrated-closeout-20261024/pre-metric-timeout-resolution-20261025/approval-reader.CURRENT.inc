static DWORD ig_approval(const char *expected)
{
 {DWORD aux_guard=lab_aux_gate();if(aux_guard)return aux_guard;}

 unsigned i;DWORD e;BY_HANDLE_FILE_INFORMATION id;
 if(!expected||strlen(expected)!=64)return ERROR_ACCESS_DENIED;
 for(i=0;i<64;i++)if(!((expected[i]>='0'&&expected[i]<='9')||(expected[i]>='a'&&expected[i]<='f')))return ERROR_ACCESS_DENIED;
 ig_approval_file=CreateFileW(
#ifdef PAPACC_N0START03_PRIVATE_POSITIVE
 L"C:\\Projetos\\PapinhoAccelerator\\docs\\integration\\n0start03-integrated-closeout-20261024\\pre-metric-timeout-resolution-20261025\\EXACT-POSITIVE-TIMEOUT-CORRECTION-APPROVED.json",
#elif defined(PAPACC_N0_PRIVATE_STDOUT)
 L"C:\\Projetos\\PapinhoAccelerator\\docs\\integration\\n0start03-integrated-closeout-20261024\\t0-t9-acceptance-and-integration\\EXACT-TRANSPORT-READONLY-APPROVED.json",
#elif defined(PAPACC_READONLY_HANDLE_TABLE_TRACE)
 L"C:\\Projetos\\PapinhoAccelerator\\docs\\integration\\n0start03-integrated-closeout-20261024\\handle-attribution\\EXACT-HANDLE-READONLY-APPROVED.json",
#elif defined(PAPACC_N0START03_READONLY_PINS)
 L"C:\\Projetos\\PapinhoAccelerator\\docs\\integration\\n0start03-fresh-native-readonly-preparation-20261024\\EXACT-FRESH-READONLY-APPROVED.json",
#else
 L"C:\\Projetos\\PapinhoAccelerator\\docs\\integration\\successor-exact-binding-functional-20261024\\EXACT-OPERATION-APPROVED.json",
#endif
 GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_FLAG_OPEN_REPARSE_POINT,NULL);
 if(ig_approval_file==INVALID_HANDLE_VALUE){ig_approval_file=NULL;return lab_error();}
 if(!GetFileInformationByHandle(ig_approval_file,&id))return lab_error();
 if(id.nFileSizeHigh||!id.nFileSizeLow||id.nFileSizeLow>16384||id.nNumberOfLinks!=1||(id.dwFileAttributes&(FILE_ATTRIBUTE_REPARSE_POINT|FILE_ATTRIBUTE_DIRECTORY)))return ERROR_INVALID_DATA;
 e=lab_hash(ig_approval_file,expected);
#ifdef PAPACC_N0START03_PRIVATE_POSITIVE
 if(!e){static const char required[]=IG_PRIVATE_POSITIVE_APPROVAL_PREFIX;char prefix[sizeof(required)-1];DWORD bytes=0;
  if(SetFilePointer(ig_approval_file,0,NULL,FILE_BEGIN)==INVALID_SET_FILE_POINTER&&GetLastError()!=ERROR_SUCCESS)return lab_error();
  if(!ReadFile(ig_approval_file,prefix,sizeof(prefix),&bytes,NULL))return lab_error();
  if(bytes!=sizeof(prefix)||!ig_transport_positive_record(prefix,bytes))return ERROR_ACCESS_DENIED;
 }
#endif
 return e;
}
