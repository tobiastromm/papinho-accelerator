#include "pst_socket_attachment_win32.h"
#include "papinho_secure_transport_win32.h"
PAPACC_RESULT papacc_pst_socket_attach(PAPACC_PST_SECURE_PROCESSOR *processor,
 SOCKET *native_alias,pst_transport **retained,PAPACC_BOOL *consumed)
{
 pst_transport *transport=NULL;
 PAPACC_RESULT result;
 if(consumed!=NULL)*consumed=PAPACC_FALSE;
 if(processor==NULL||native_alias==NULL||retained==NULL||consumed==NULL)
  return PAPACC_RESULT_INVALID_ARGUMENT;
 if(*retained!=NULL||*native_alias==INVALID_SOCKET||
    processor->state!=PAPACC_PST_SECURE_PROCESSOR_CREATED)
  return PAPACC_RESULT_INVALID_STATE;
 if(pst_win32_socket_transport_create((pst_size)*native_alias,&transport)!=PST_RESULT_OK)
  return PAPACC_RESULT_INTERNAL_ERROR;
 /* Successful create owns the socket; never close through the old alias. */
 *native_alias=INVALID_SOCKET;
 result=papacc_pst_secure_processor_attach(processor,transport,consumed);
 if(*consumed!=PAPACC_TRUE){
  *retained=transport;
  return result==PAPACC_RESULT_OK?PAPACC_RESULT_INVALID_STATE:result;
 }
 return result;
}