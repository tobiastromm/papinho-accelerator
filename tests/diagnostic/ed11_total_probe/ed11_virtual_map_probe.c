/* Isolated modern virtual-map observer. No Browser APIs or production C2. */
#define main inherited_single_probe_main
#include "../ed11_resource_probe/ed11_resource_probe.c"
#undef main
static int mapping(const char *phase) {
 SYSTEM_INFO s;MEMORY_BASIC_INFORMATION m;uintptr_t at,max,next;size_t n;unsigned regions=0;
 unsigned long long priv=0,img=0,mapped=0,reserved=0,other=0;
 GetSystemInfo(&s);at=(uintptr_t)s.lpMinimumApplicationAddress;max=(uintptr_t)s.lpMaximumApplicationAddress;
 while(at<=max){n=VirtualQuery((const void*)at,&m,sizeof(m));if(!n)return 0;++regions;
  if(m.State==MEM_COMMIT){if(m.Type==MEM_PRIVATE)priv+=(unsigned long long)m.RegionSize;else if(m.Type==MEM_IMAGE)img+=(unsigned long long)m.RegionSize;else if(m.Type==MEM_MAPPED)mapped+=(unsigned long long)m.RegionSize;else other+=(unsigned long long)m.RegionSize;}
  else if(m.State==MEM_RESERVE)reserved+=(unsigned long long)m.RegionSize;
  if((uintptr_t)m.BaseAddress>UINTPTR_MAX-m.RegionSize)return 0;next=(uintptr_t)m.BaseAddress+m.RegionSize;if(next<=at)return 0;at=next;
 }
 printf("{\"event\":\"COMMITTED_VIRTUAL_MAP\",\"phase\":\"%s\",\"private_view_bytes\":%llu,\"image_view_bytes\":%llu,\"mapped_view_bytes\":%llu,\"other_committed_bytes\":%llu,\"total_committed_virtual_views\":%llu,\"reserved_virtual_bytes\":%llu,\"regions\":%u,\"valid\":true,\"class\":\"VIRTUAL_ADDRESS_VIEWS_NOT_OWNED_ALLOCATION_OR_PHYSICAL_BYTES\"}\n",phase,priv,img,mapped,other,priv+img+mapped+other,reserved,regions);fflush(stdout);return 1;
}
int main(int argc,char **argv) {
 Probe p,q;WSADATA wd;PST_RELEASE_INFO info;PST_RESULT x;
 if(argc!=2)return 64;
 memset(payload,0x5a,sizeof(payload));SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
 if(!mapping("BASELINE_BEFORE_PST_AND_WINSOCK"))return 80;measure(NULL,"MAP_BASELINE");
 if(WSAStartup(MAKEWORD(2,2),&wd)||pst_win32_register_builtin_providers()!=0)return 65;
 if(!init(&p,70001,argv[1],"openssl",NULL))return 66;
 if(!mapping("ONE_OPERATION_SETUP_SHARED_RUNTIME_AND_CREDENTIALS"))return 81;
 if(pst_release_info_init(&info,(pst_u32)sizeof(info)))return 67;x=pst_connection_try_release(p.client,&info);
 release_event(&p,"MAP_ACTUAL_MEMBERSHIP_PRECONDITION",x,&info);
 if(x!=PST_RESULT_INVALID_STATE||info.result!=x||info.disposition!=PST_RELEASE_DISPOSITION_PRECONDITION_RETAINED)return 68;
 if(!mapping("REAL_PRECONDITION_RETAINED_SAME_CONNECTION"))return 82;
 if(!handshake(&p)||!pressure(&p))return 69;
 if(!init(&q,70002,argv[1],"openssl",p.rt)||!handshake(&q)||!pressure(&q))return 70;
 if(!mapping("TWO_OPERATIONS_FOUR_PST_CONTEXTS_SHARED_RUNTIME"))return 83;
 if(!close_probe(&p,1,0)||!close_probe(&q,1,0)||!cleanup(&q))return 71;
 if(!mapping("SECOND_OPERATION_DESTROYED_SHARED_RUNTIME_STILL_OWNED"))return 84;
 if(!cleanup(&p))return 72;
 if(WSACleanup())return 73;
 if(!mapping("ALL_CHILDREN_DESTROYED_AND_WINSOCK_CLEANED"))return 85;measure(NULL,"MAP_FINAL");
 if(measure_failed)return 86;
 printf("{\"result\":\"PASS\",\"scope\":\"NORMAL_OPENSSL_VIRTUAL_MAP_CLASSIFICATION_NOT_TOTAL_C2_OR_KERNEL_CAP\"}\n");return 0;
}
