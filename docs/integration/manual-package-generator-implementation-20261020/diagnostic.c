#define out test_output
#define header test_header
#define main original_test_main
#include "../../../tests/manual_package_test.c"
#undef main
#undef out
#undef header
#include "../../../src/provisioning/manual_package.c"
int main(void){MP_PAYLOAD p,q;size_t n=0;MP_RESULT r;setup(&p);printf("parts=%d cfg=%d issuance=%d\n",parts(&p),metadata(p.member[3],0),metadata(p.member[4],1));r=mp_payload_prepare(&p,zip,sizeof(zip),&n,0);printf("result=%d used=%Iu\n",r,n);printf("inspect=%d\n",mp_payload_inspect(zip,502+27+p.member[3].size+p.member[4].size,&q,0));return 0;}
