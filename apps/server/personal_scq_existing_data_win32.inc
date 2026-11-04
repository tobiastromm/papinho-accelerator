/* P1 DATA only, private in-memory console selection. No persisted/wire format,
 * authority registration, confirmation, consumption or native object lookup.
 * Original protected documents and attributed E/Q/S independently remain gates. */
static DWORD scqc_existing_data_parse_mode(const char *line,PAPACC_SCQ_EXISTING_SELECTION *out,int read_diagnostic)
{
 const char *prefix=read_diagnostic?"P1 MACHINE_READ AMD64 ":"P1 TRANSPORT_ONLY AMD64 ";size_t prefix_bytes=strlen(prefix);
 PAPACC_SCQ_EXISTING_SELECTION value;const char *at;unsigned i,j,d;ULONGLONG n;
 if(!line||!out||strncmp(line,prefix,prefix_bytes))return ERROR_ACCESS_DENIED;
 memset(&value,0,sizeof(value));at=line+prefix_bytes;value.architecture=0x8664U;
 for(i=0;i<9U;i++){
  if(*at<'0'||*at>'9')return ERROR_INVALID_DATA;n=0;
  do{d=(unsigned)(*at++-'0');if(n>((ULONGLONG)MAXDWORD-d)/10U)return ERROR_INVALID_DATA;n=n*10U+d;}while(*at>='0'&&*at<='9');
  value.identity[i]=(DWORD)n;
  if(*at++!=(i%3U==2U?' ':':'))return ERROR_INVALID_DATA;
 }
 for(i=0;i<32U;i++){
  BYTE v=0;for(j=0;j<2U;j++){
   char ch=*at++;if(!ch||!((ch>='0'&&ch<='9')||(ch>='a'&&ch<='f')))return ERROR_INVALID_DATA;
   v=(BYTE)((v<<4)|(ch<='9'?ch-'0':ch-'a'+10));
  }value.provenance[i]=v;
 }
 if(*at)return ERROR_INVALID_DATA;
 for(i=0;i<3U;i++)if(!value.identity[3U*i]||!(value.identity[3U*i+1U]|value.identity[3U*i+2U]))return ERROR_INVALID_DATA;
 *out=value;return 0; /* Never a grant: native selected-object constructor still required. */
}

static DWORD scqc_existing_data_parse(const char *line,PAPACC_SCQ_EXISTING_SELECTION *out)
{return scqc_existing_data_parse_mode(line,out,0);}
