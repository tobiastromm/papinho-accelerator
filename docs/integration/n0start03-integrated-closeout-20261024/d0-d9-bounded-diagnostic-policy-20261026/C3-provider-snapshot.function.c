PST_RESULT papacc_c3_bridge_provider_snapshot(const PAPACC_C3_BRIDGE *b,
 PAPACC_U64 operation,PAPACC_U64 generation,PST_PROVIDER_INFO *out)
{
 PST_PROVIDER_INFO f;PST_RESULT rc;const pst_connection *connection;
 ULONG_PTR a=(ULONG_PTR)b,z=(ULONG_PTR)out;
 if(!b||!out)return PST_RESULT_INVALID_ARGUMENT;
 if((z>=a&&z-a<sizeof(*b))||(z<a&&a-z<sizeof(*out)))return PST_RESULT_INVALID_ARGUMENT;
 if(!owner(b)||b->revoked||b->retained||b->child_destroyed||!b->child_acquired||
    !operation||operation!=b->operation||generation!=b->generation||!b->processor.connection)
  return PST_RESULT_INVALID_STATE;
 connection=b->processor.connection;memset(&f,0,sizeof(f));f.struct_size=sizeof(f);f.api_version=PST_API_VERSION;
 rc=pst_connection_get_provider_info(connection,&f);
 if(rc!=PST_RESULT_OK)return rc;
 if(!owner(b)||b->revoked||b->retained||b->child_destroyed||!b->child_acquired||b->processor.connection!=connection||
    operation!=b->operation||generation!=b->generation||
    !memchr(f.provider_id,0,sizeof(f.provider_id))||strcmp(f.provider_id,"openssl")||
    f.available!=1U||f.initialized>1U)return PST_RESULT_INVALID_STATE;
 *out=f;return PST_RESULT_OK;
}
