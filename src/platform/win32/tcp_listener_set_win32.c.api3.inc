#include "tcp_listener_set_win32.h"

#include <ws2tcpip.h>

static PAPACC_RESULT papacc_tcp_listener_win32_get_port(
    const PAPACC_TCP_SOCKET_WIN32 *socket_context,
    PAPACC_U16 *out_port)
{
    if (socket_context->family == PAPACC_IP_FAMILY_IPV4) {
        struct sockaddr_in address;
        int address_size = (int)sizeof(address);

        if (getsockname(
                socket_context->native_socket,
                (struct sockaddr *)&address,
                &address_size) == SOCKET_ERROR) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        *out_port = (PAPACC_U16)ntohs(address.sin_port);
    } else if (socket_context->family == PAPACC_IP_FAMILY_IPV6) {
        struct sockaddr_in6 address;
        int address_size = (int)sizeof(address);

        if (getsockname(
                socket_context->native_socket,
                (struct sockaddr *)&address,
                &address_size) == SOCKET_ERROR) {
            return PAPACC_RESULT_INTERNAL_ERROR;
        }
        *out_port = (PAPACC_U16)ntohs(address.sin6_port);
    } else {
        return PAPACC_RESULT_INVALID_STATE;
    }

    return (*out_port != 0) ? PAPACC_RESULT_OK : PAPACC_RESULT_INTERNAL_ERROR;
}

PAPACC_RESULT papacc_tcp_listener_set_win32_try_shutdown(PAPACC_TCP_LISTENER_SET_WIN32 *set)
{
 PAPACC_SIZE i;PAPACC_RESULT first=PAPACC_RESULT_OK,r;
 if(!set)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(set->count>set->capacity||(set->count&&!set->entries))return PAPACC_RESULT_INVALID_STATE;
 set->is_active=PAPACC_FALSE;
 for(i=0;i<set->count;++i){r=papacc_tcp_socket_win32_try_close(&set->entries[i].socket);
  if(r!=PAPACC_RESULT_OK&&first==PAPACC_RESULT_OK)first=r;}
 if(first!=PAPACC_RESULT_OK)return first;
 *set=(PAPACC_TCP_LISTENER_SET_WIN32)PAPACC_TCP_LISTENER_SET_WIN32_INITIALIZER;
 return PAPACC_RESULT_OK;
}
PAPACC_RESULT papacc_tcp_listener_set_win32_start(const PAPACC_TCP_PLATFORM *platform,
 const PAPACC_BIND_TARGET *targets,PAPACC_SIZE count,PAPACC_U16 port,
 PAPACC_TCP_LISTENER_ENTRY_WIN32 *entries,PAPACC_SIZE capacity,PAPACC_TCP_LISTENER_SET_WIN32 *set)
{
 PAPACC_SIZE i,n=0U,unsupported=0U;PAPACC_U16 effective=port;PAPACC_RESULT r;
 if(!platform||!targets||!entries||!set||!count)return PAPACC_RESULT_INVALID_ARGUMENT;
 if(!platform->initialized||platform->close_unresolved||set->entries||set->count||set->is_active)return PAPACC_RESULT_INVALID_STATE;
 if(capacity<count)return PAPACC_RESULT_LIMIT_EXCEEDED;
 for(i=0;i<count;++i)entries[i]=(PAPACC_TCP_LISTENER_ENTRY_WIN32)PAPACC_TCP_LISTENER_ENTRY_WIN32_INITIALIZER;
 for(i=0;i<count;++i){
  PAPACC_TCP_LISTENER_ENTRY_WIN32 *entry=&entries[n];entry->target=targets[i];
  r=papacc_tcp_socket_win32_bind(platform,&entry->target,effective,&entry->socket);
  if(r==PAPACC_RESULT_NOT_SUPPORTED&&!entry->socket.is_open){++unsupported;continue;}
  if(entry->socket.is_open)++n;
  if(r!=PAPACC_RESULT_OK)goto fail;
  r=papacc_tcp_socket_win32_listen(&entry->socket);if(r!=PAPACC_RESULT_OK)goto fail;
  if(!effective){r=papacc_tcp_listener_win32_get_port(&entry->socket,&effective);if(r!=PAPACC_RESULT_OK)goto fail;}
 }
 if(!n){set->unsupported_target_count=unsupported;return PAPACC_RESULT_NOT_SUPPORTED;}
 set->entries=entries;set->capacity=capacity;set->count=n;set->unsupported_target_count=unsupported;
 set->bound_port=effective;set->is_active=PAPACC_TRUE;return PAPACC_RESULT_OK;
fail:
 set->entries=entries;set->capacity=capacity;set->count=n;set->unsupported_target_count=unsupported;
 set->bound_port=effective;set->is_active=PAPACC_FALSE;
 /* One owner attempt; any uncertain entry keeps the complete containing array. */
 (void)papacc_tcp_listener_set_win32_try_shutdown(set);return r;
}
void papacc_tcp_listener_set_win32_shutdown(PAPACC_TCP_LISTENER_SET_WIN32 *set)
{(void)papacc_tcp_listener_set_win32_try_shutdown(set);}