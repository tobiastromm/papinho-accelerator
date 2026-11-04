#include "C:/Projetos/PapinhoBrowser/src/papinho_accelerator_client.c"
const unsigned long quant_layout[] = {
sizeof(papinho_c2_s6_context),
sizeof(C2_ROLE),
sizeof(papinho_session_storage),
sizeof(papinho_accel_pst_group),
sizeof(papinho_accelerator_persistent_request_job),
sizeof(papinho_pst_owner),
sizeof(papinho_c2_s6_context)+sizeof(papinho_session_storage)+sizeof(papinho_accel_pst_group)+4U*sizeof(papinho_accelerator_persistent_request_job)+3U*sizeof(papinho_pst_owner)+2U*C2_HTTP_CAP+65536U
};
