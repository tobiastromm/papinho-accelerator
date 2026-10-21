#ifndef PAPACC_INTERNAL_C2_CODEC_H
#define PAPACC_INTERNAL_C2_CODEC_H
#include <papacc/types.h>
#define PAPACC_C2_FRAME_CAPACITY 16400U
typedef enum PAPACC_C2_CHANNEL { PAPACC_C2_CONTROL=0, PAPACC_C2_ROLE_CHANNEL=1 } PAPACC_C2_CHANNEL;
typedef enum PAPACC_C2_SENDER { PAPACC_C2_BROWSER=0, PAPACC_C2_ACCELERATOR=1 } PAPACC_C2_SENDER;
typedef struct PAPACC_C2_VIEW { PAPACC_U16 type; const PAPACC_U8 *payload; PAPACC_SIZE length; } PAPACC_C2_VIEW;
/* Internal grammar only: successful codec validation confers no runtime authority.
 * View borrows frame until caller's next buffer mutation. No heap/packed/native wire structs.
 * Caller uses disjoint view/storage; encode permits overlapping payload via memmove. */
PAPACC_RESULT papacc_c2_validate_payload(PAPACC_U16,const PAPACC_U8*,PAPACC_SIZE,PAPACC_C2_CHANNEL,PAPACC_C2_SENDER);
PAPACC_RESULT papacc_c2_encode_frame(PAPACC_U16,const PAPACC_U8*,PAPACC_SIZE,PAPACC_C2_CHANNEL,PAPACC_C2_SENDER,PAPACC_U8*,PAPACC_SIZE,PAPACC_SIZE*);
PAPACC_RESULT papacc_c2_decode_frame(const PAPACC_U8*,PAPACC_SIZE,PAPACC_C2_CHANNEL,PAPACC_C2_SENDER,PAPACC_C2_VIEW*);
/* Bounded parser: caller provides fixed frame storage, one emitted borrowed view at a time.
 * Feed stops at first complete frame and reports consumed bytes; next frame requires reset.
 * Header limits checked before payload accumulation; malformed stream stays terminal. */
typedef struct PAPACC_C2_READER { PAPACC_U8 *storage; PAPACC_SIZE capacity,used,expected; PAPACC_C2_CHANNEL channel; PAPACC_C2_SENDER sender; PAPACC_BOOL failed,complete; } PAPACC_C2_READER;
PAPACC_RESULT papacc_c2_reader_init(PAPACC_C2_READER*,PAPACC_U8*,PAPACC_SIZE,PAPACC_C2_CHANNEL,PAPACC_C2_SENDER);
PAPACC_RESULT papacc_c2_reader_feed(PAPACC_C2_READER*,const PAPACC_U8*,PAPACC_SIZE,PAPACC_SIZE*,PAPACC_BOOL*,PAPACC_C2_VIEW*);
/* End all view borrows first. Reset erases accumulated bytes, including on
 * terminal failure; failed parser remains terminal. Caller storage stays owned. */
void papacc_c2_reader_reset(PAPACC_C2_READER*);
#endif
