static DWORD scqi_chain_close(PAPACC_SCQ_IMAGE_CHAIN *g)
{
 unsigned i,index;HANDLE *h;
 if(!g->phase||g->phase==3U)return 0;
 if(g->self!=g||!g->owner||g->thread!=GetCurrentThreadId()||g->phase==4U)return ERROR_INVALID_STATE;
 for(i=0;i<=PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS;i++){
  index=PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS-i;
  h=i==0U?&g->image:&g->directory[index]; /* image/child before protecting parents */
  if(*h){if(g->close_attempted&(1U<<index))goto unknown;g->close_attempted|=1U<<index;if(!CloseHandle(*h))goto unknown;*h=NULL;}
 }
 for(i=0;i<=PAPACC_SCQ_IMAGE_DIRECTORY_SLOTS;i++)if(g->sd[i]){if(LocalFree(g->sd[i]))goto unknown;g->sd[i]=NULL;}
 if(g->scratch){if(LocalFree(g->scratch))goto unknown;g->scratch=NULL;}
 if(g->installer){if(LocalFree(g->installer))goto unknown;g->installer=NULL;}
 g->phase=3U;return 0;
unknown:g->phase=4U;return scqi_chain_fail(g,ERROR_INVALID_HANDLE);
}
