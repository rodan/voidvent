
#include <inttypes.h>
#include <msp430.h>
#include <string.h>
#include "glue.h"
#include "proj.h"
#include "sig.h"
#include "ui.h"
#include "hamma_ews.h"

#define  HEWS_SIG_SZ  5

typedef struct {
    uint8_t dec[HEWS_SIG_SZ];
    uint8_t cnt;
} hews_result;

typedef struct {
    uint8_t b[HEWS_SIG_SZ];     ///< received message
    uint8_t cnt;        ///< number of bytes received
    int8_t s;           ///< shift of current bit in current byte
} hews_proto_decoder;

uint8_t hews_last_event = HEWS_EVENT_NULL;

hews_result h_res;
hews_proto_decoder h_p;

static void hews_decode_irq(uint32_t msg);

void hews_decoder_rst(void)
{
    memset(&h_p, 0, sizeof(h_p));
    h_p.s = 7;
}

void hews_result_rst(void)
{
    memset(&h_res, 0, sizeof(h_res));
}

void hews_rst(void)
{
    hews_decoder_rst();
    hews_result_rst();
}

void hews_decode(const uint16_t interval, const uint8_t pol)
{
    
}

void hews_handler_init(void)
{
    eh_register(&hews_decode_irq, SYS_MSG_W_DECODE_RDY);
}

static void hews_decode_irq(uint32_t msg)
{
    //print_buf(&hews_res.dec[0], hews_res.cnt);
    print_buf(&h_res.dec[0], h_res.cnt);
    hews_result_rst();
}

uint8_t hews_get_event(void)
{
    return hews_last_event;
}

void hews_rst_event(void)
{
    hews_last_event = HEWS_EVENT_NULL;
}
