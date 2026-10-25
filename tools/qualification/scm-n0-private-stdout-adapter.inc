/* Scoped stdout dispatcher for the instrumented include composition.
 * Installed before runtime helpers, so their printf/puts/fflush share the
 * explicit stream. stderr/console and legacy/default builds are unchanged.
 * Placement does not enable the positive entry or grant operational authority. */
#include <stdarg.h>
__declspec(noreturn) static void ig_transport_write_failed(void){papacc_server_console_win32_request_stop();native.retained=1;for(;;)Sleep(1000);}
static int ig_transport_printf(const char *format,...){va_list a;int n;va_start(a,format);n=PaccTransportVPrintf(format,a);va_end(a);if(n<0)ig_transport_write_failed();return n;}
static int ig_transport_puts(const char *text){int n=PaccTransportPuts(text);if(n==EOF)ig_transport_write_failed();return n;}
static int ig_transport_flush(FILE *stream){int n=stream==stdout?PaccTransportFlush():fflush(stream);if(n==EOF)ig_transport_write_failed();return n;}
#define printf ig_transport_printf
#define puts ig_transport_puts
#define fflush ig_transport_flush
