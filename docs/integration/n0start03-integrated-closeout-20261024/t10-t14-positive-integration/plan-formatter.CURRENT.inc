static void ig_n0_hex(const BYTE bytes[32],char out[65])
{unsigned i;for(i=0;i<32;i++){out[2*i]="0123456789abcdef"[bytes[i]>>4];out[2*i+1]="0123456789abcdef"[bytes[i]&15];}out[64]=0;}
static DWORD ig_n0_plan(const PS_BD_DESCRIPTOR *d,char out[65])
{
 BYTE digest[32];char text[2304];int n;
 n=sprintf_s(text,sizeof(text),
 "%s;%s;%s;%s;%s;%s;%llu;%lu;%llu;%lu;%llu;%lu;%lu;%lu:%lu:%lu:%lu:%lu:%lu;%llu;"
 "config=%lu:%lu:%lu:%lu:%lu;image=%lu:%lu:%lu:%lu:%lu;aux=%lu:%llu:%lu:%lu;file=%lu:%lu:%lu:%lu:%lu:%lu;%s;%s;%s;"
 "APPROVAL=%s;INPUT_IDS=%lu:%lu:%lu:%lu,%lu:%lu:%lu:%lu;INPUT_PROFILE=N2-SD-RAW1-OWNER-GROUP-DACL;L1=2d91d64eace4613f64b2d17188642e85eb5ac41af5f6a5afa1f8e746eaeff27e;H=23d509674be807f01d2c6301cd990f2212a05d6d4d707e56f9f924c6afbd7218;P3=892d51b6a0033b3211c6483adb56d03d728c5f8285dfc8520cf73574c3ee5fad;SERVICE=PapinhoAcceleratorLab20261023;SID=S-1-5-80-3567404667-26456103-3018599297-139793421-3962028451;OPERATOR=S-1-5-21-785699550-1440458426-1682014700-1108;ASSOCIATION=UNPROVEN;CREDITS=NONE_GRANTED;N7_ED11=%s",
 ig_n0_positive?"N0START-20261024-03;CREATE_IMAGE;CREATE_SINK;REBIND;START1;STOP_MAX1;T10_T14=1f293812e9fca107c7fa8ba20abe20fd8ffbca3cdf6e95e58465a95dadbe4dd5":"N0_READ_ONLY",d->attempt,d->config_hash,d->image_hash,d->policy,d->host,d->boot,(unsigned long)d->owner_pid,d->owner_creation,
 (unsigned long)d->actor.pid,d->actor.creation,(unsigned long)d->actor.session,(unsigned long)d->control_tid,
 (unsigned long)d->statistics.TokenId.LowPart,(unsigned long)d->statistics.TokenId.HighPart,
 (unsigned long)d->statistics.AuthenticationId.LowPart,(unsigned long)d->statistics.AuthenticationId.HighPart,
 (unsigned long)d->statistics.ModifiedId.LowPart,(unsigned long)d->statistics.ModifiedId.HighPart,d->challenge,
 (unsigned long)d->config_id.dwVolumeSerialNumber,(unsigned long)d->config_id.nFileIndexHigh,(unsigned long)d->config_id.nFileIndexLow,(unsigned long)d->config_id.nFileSizeHigh,(unsigned long)d->config_id.nFileSizeLow,
 (unsigned long)d->image_id.dwVolumeSerialNumber,(unsigned long)d->image_id.nFileIndexHigh,(unsigned long)d->image_id.nFileIndexLow,(unsigned long)d->image_id.nFileSizeHigh,(unsigned long)d->image_id.nFileSizeLow,
 (unsigned long)d->auxiliary.instance.pid,d->auxiliary.instance.creation,(unsigned long)d->auxiliary.instance.session,(unsigned long)d->auxiliary.parent,
 (unsigned long)d->auxiliary.file_id.dwVolumeSerialNumber,(unsigned long)d->auxiliary.file_id.nFileIndexHigh,(unsigned long)d->auxiliary.file_id.nFileIndexLow,(unsigned long)d->auxiliary.file_id.nFileSizeHigh,(unsigned long)d->auxiliary.file_id.nFileSizeLow,(unsigned long)d->auxiliary.file_id.nNumberOfLinks,
 d->auxiliary.file_hash,d->auxiliary.security_hash,d->auxiliary.provenance_hash,ig_n0_approval_sha?ig_n0_approval_sha:"NONE",
 (unsigned long)ig_n0.dossier_id.dwVolumeSerialNumber,(unsigned long)ig_n0.dossier_id.nFileIndexHigh,(unsigned long)ig_n0.dossier_id.nFileIndexLow,(unsigned long)ig_n0.dossier_id.nFileSizeLow,
 (unsigned long)ig_n0.evidence_id.dwVolumeSerialNumber,(unsigned long)ig_n0.evidence_id.nFileIndexHigh,(unsigned long)ig_n0.evidence_id.nFileIndexLow,(unsigned long)ig_n0.evidence_id.nFileSizeLow,IG_N0_N7_ED11_SHA);
 if(n<=0||!pacc_storage_hash_callback(NULL,(const BYTE *)text,(size_t)n,digest))return ERROR_INVALID_DATA;ig_n0_hex(digest,out);return 0;
}
