
#include <inttypes.h>
#include <msp430.h>
#include <string.h>
#include "glue.h"
#include "proj.h"
#include "sig.h"
#include "ui.h"
#include "hama_ews.h"

#define  HEWS_SIG_SZ  5
#define  HEWS_RES_SZ  4

typedef struct {
    uint8_t dec[HEWS_RES_SZ];
} hews_result;

typedef struct {
    uint8_t b[HEWS_SIG_SZ];     ///< received bytes
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
    if (h.cnt > HEWS_SIG_SZ) {
        //sig3_on;
        return;
    }

    if (pol) {
        if ((interval > HEWS_BIT_SEP_MIN) && (interval < HEWS_BIT_SEP_MAX)) {
            // normal bit separator
            //sig2_on;
        } else {
            //sig3_on;
            hews_decoder_rst();
        }
    } else {
        if ((interval > HEWS_L0_MIN) && (interval < HEWS_L0_MAX)) {
            // logic zero
            h.s--;
            //sig2_on;
        } else if ((interval >  HEWS_L1_MIN) && (interval <  HEWS_L1_MAX)) {
            // logic one
            h.b[h.cnt] |= 1 << h.s;
            h.s--;
            //sig2_on;
        } else if (interval > HEWS_CMD_SEP_MIN) {
            // command is done. report result only if there are zero inconsistencies
            if ((h.cnt != 4) || (h.s != 3)) {
                hews_decoder_rst();
                //sig3_on;
                return;
            }
                                    // |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |
            h_res.dec[0] = h.b[0];  // | remote sensor id (generated randomly during sensor poweron)   |
            h_res.dec[1] = h.b[1];  // | batt  |   ?   | channel 0-2   |    temp sign {0x00, oxff}     |
            h_res.dec[2] = h.b[2];  // |            temperature in int Celsius *10                     |
            h_res.dec[3] = ((h.b[3] & 0xf) << 4 ) | ((h.b[4] & 0xf0) >> 4); // rH%

            hews_last_event = HEWS_EVENT_DECODE_RDY;
            hews_decoder_rst();
            //sig3_on;
        }
    }

    //sig2_off;
    //sig3_off;

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
    print_buf(&h_res.dec[0], HEWS_RES_SZ);
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
