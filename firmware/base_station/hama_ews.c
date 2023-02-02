
#include <inttypes.h>
#include <msp430.h>
#include <string.h>
#include "glue.h"
#include "proj.h"
#include "sig.h"
#include "ui.h"
#include "hama_ews.h"

#define  HEWS_SIG_SZ  5

typedef struct {
    uint8_t dec[HEWS_SIG_SZ];
    //uint8_t cnt;
} hews_result;

typedef struct {
    uint8_t b[HEWS_SIG_SZ];     ///< received message
    uint8_t cnt;        ///< number of bytes received
    int8_t s;           ///< shift of current bit in current byte
} hews_proto_decoder;

uint8_t hews_last_event = HEWS_EVENT_NULL;

hews_result h_res;
hews_proto_decoder h;

static void hews_decode_irq(uint32_t msg);

void hews_decoder_rst(void)
{
    memset(&h, 0, sizeof(h));
    h.s = 7;
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
    uint8_t i;

    if (h.cnt > HEWS_SIG_SZ) {
        sig3_on;
        return;
    }

    if (pol) {
        if ((interval > HEWS_BIT_SEP_MIN) && (interval < HEWS_BIT_SEP_MAX)) {
            // normal bit separator
            sig2_on;
        } else {
            sig3_on;
            hews_decoder_rst();
        }
    } else {
        if ((interval > HEWS_L0_MIN) && (interval < HEWS_L0_MAX)) {
            // logic zero
            h.s--;
            sig2_on;
        } else if ((interval >  HEWS_L1_MIN) && (interval <  HEWS_L1_MAX)) {
            // logic one
            h.b[h.cnt] |= 1 << h.s;
            h.s--;
            sig2_on;
        } else if (interval > HEWS_CMD_SEP_MIN) {
            // command is done. report result only if there are zero inconsistencies
            if ((h.cnt != 4) || (h.s != 3)) {
                hews_decoder_rst();
                sig3_on;
                return;
            }
            for (i=0;i<HEWS_SIG_SZ;i++) {
                h_res.dec[i] = h.b[i];
            }
            hews_last_event = HEWS_EVENT_DECODE_RDY;
            hews_decoder_rst();
            sig3_on;
        }
    }

    sig2_off;
    sig3_off;

    if (h.s < 0) {
        h.s = 7;
        h.cnt++;
    }
}

void hews_handler_init(void)
{
    eh_register(&hews_decode_irq, SYS_MSG_W_DECODE_RDY);
}

static void hews_decode_irq(uint32_t msg)
{
    //print_buf(&hews_res.dec[0], hews_res.cnt);
    print_buf(&h_res.dec[0], HEWS_SIG_SZ);
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
