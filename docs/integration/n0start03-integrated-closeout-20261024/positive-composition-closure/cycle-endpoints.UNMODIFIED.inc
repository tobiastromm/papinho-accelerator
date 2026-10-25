static DWORD cycle_no_endpoints(DWORD pid)
{
 union {MIB_TCPTABLE_OWNER_PID align;BYTE bytes[65536];}tcp;
 union {MIB_UDPTABLE_OWNER_PID align;BYTE bytes[65536];}udp;
 union {MIB_TCP6TABLE_OWNER_PID align;BYTE bytes[65536];}tcp6;
 union {MIB_UDP6TABLE_OWNER_PID align;BYTE bytes[65536];}udp6;
 DWORD n=sizeof(tcp),e,i;
 e=GetExtendedTcpTable(&tcp,&n,FALSE,AF_INET,TCP_TABLE_OWNER_PID_ALL,0);if(e)return e;
 if(n>sizeof(tcp)||n<sizeof(DWORD)||tcp.align.dwNumEntries>(n-FIELD_OFFSET(MIB_TCPTABLE_OWNER_PID,table))/sizeof(MIB_TCPROW_OWNER_PID))return ERROR_INVALID_DATA;
 for(i=0;i<tcp.align.dwNumEntries;i++)if(tcp.align.table[i].dwOwningPid==pid)return ERROR_ACCESS_DENIED;
 n=sizeof(udp);e=GetExtendedUdpTable(&udp,&n,FALSE,AF_INET,UDP_TABLE_OWNER_PID,0);if(e)return e;
 if(n>sizeof(udp)||n<sizeof(DWORD)||udp.align.dwNumEntries>(n-FIELD_OFFSET(MIB_UDPTABLE_OWNER_PID,table))/sizeof(MIB_UDPROW_OWNER_PID))return ERROR_INVALID_DATA;
 for(i=0;i<udp.align.dwNumEntries;i++)if(udp.align.table[i].dwOwningPid==pid)return ERROR_ACCESS_DENIED;
 n=sizeof(tcp6);e=GetExtendedTcpTable(&tcp6,&n,FALSE,AF_INET6,TCP_TABLE_OWNER_PID_ALL,0);if(e)return e;
 if(n>sizeof(tcp6)||n<sizeof(DWORD)||tcp6.align.dwNumEntries>(n-FIELD_OFFSET(MIB_TCP6TABLE_OWNER_PID,table))/sizeof(MIB_TCP6ROW_OWNER_PID))return ERROR_INVALID_DATA;
 for(i=0;i<tcp6.align.dwNumEntries;i++)if(tcp6.align.table[i].dwOwningPid==pid)return ERROR_ACCESS_DENIED;
 n=sizeof(udp6);e=GetExtendedUdpTable(&udp6,&n,FALSE,AF_INET6,UDP_TABLE_OWNER_PID,0);if(e)return e;
 if(n>sizeof(udp6)||n<sizeof(DWORD)||udp6.align.dwNumEntries>(n-FIELD_OFFSET(MIB_UDP6TABLE_OWNER_PID,table))/sizeof(MIB_UDP6ROW_OWNER_PID))return ERROR_INVALID_DATA;
 for(i=0;i<udp6.align.dwNumEntries;i++)if(udp6.align.table[i].dwOwningPid==pid)return ERROR_ACCESS_DENIED;
 puts("LAB_ENDPOINT_TABLES=NO_IPV4_IPV6_TCP_UDP_ROWS_FOR_PID_SNAPSHOT; NO_SOCKET_OPENED_BY_QUERY");return ERROR_SUCCESS;
}
