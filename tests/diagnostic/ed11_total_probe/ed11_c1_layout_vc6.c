/* Isolated ABI sizeof probe; struct extracted byte-exact from qualified C1 source CC2E8C79C717EB66910F58042F730CA165E796B5BAB55CBB58EA6C5804CEB2A9. No Browser implementation rebuilt. */
#define WIN32_LEAN_AND_MEAN
#include <winsock.h>
#include <windows.h>
#include <stdio.h>
#include "C:/Projetos/_tmp/PapinhoBrowser/model-e-implementation-20261011/source/core/networking/papinho_site_pump.h"
struct papinho_site_pump {
    CRITICAL_SECTION lock;
    LONG refs;
    DWORD thread, started, duration;
    SOCKET socket;
    HWND window;
    HANDLE event;
    int wsa, ready, wake, close_started;
    papinho_site_status status;
};
int main(void){printf("C1_LAYOUT_VC6 pump=%lu status=%lu endpoint=%lu token=%lu critical_section=%lu MSG_public_struct=%lu NO_WINDOW_KERNEL_SIZE_CLAIM\n",(unsigned long)sizeof(papinho_site_pump),(unsigned long)sizeof(papinho_site_status),(unsigned long)sizeof(papinho_endpoint_snapshot),(unsigned long)sizeof(papinho_site_token),(unsigned long)sizeof(CRITICAL_SECTION),(unsigned long)sizeof(MSG));return 0;}
