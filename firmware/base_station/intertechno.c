
#include <inttypes.h>
#include <msp430.h>
#include <string.h>
#include "glue.h"
#include "proj.h"
#include "sig.h"
#include "pwr_mgmt.h"
#include "rf1a.h"
#include "radio.h"
#include "ui.h"
#include "intertechno.h"

static uint8_t rotate_byte(uint8_t in);
uint8_t manchester_decode(const uint8_t in, uint8_t *out);

extern const RF_SETTINGS rfSettings;
extern uint8_t radio_rx_buffer[RADIO_RXBUF_SZ];

#define     ITV_SIG_SZ  8
#define     ITF_SIG_SZ  13
#define IT_RES_MAX_CNT  3

typedef struct {
    uint16_t dec[IT_RES_MAX_CNT];   ///< decoded message in uint16_t format
    uint16_t cnt;                   ///< number of commands decoded
    uint8_t proto;                  ///< proto used for decoding
    int16_t score_f;                ///< score for fixed-length protocol
    int16_t score_v;                ///< score for variable-length protocol
    uint16_t score_t;               ///< total protocol votes
} it_result;

typedef struct {
    uint8_t b[ITV_SIG_SZ];          ///< received message
    uint8_t cnt;                    ///< number of bytes received
    int8_t s;                       ///< shift of current bit in current byte
} it_variable_proto_decoder;

typedef struct {
    uint8_t b[ITF_SIG_SZ];          ///< received message
    uint8_t cnt;                    ///< number of bytes received
    int8_t s;                       ///< shift of current bit in current byte
} it_fixed_proto_decoder;

it_result it_res;
it_variable_proto_decoder it_v;
it_fixed_proto_decoder it_f;

uint8_t it_last_event = IT_EVENT_NULL;

void it_decoders_rst(const uint8_t flag)
{
    if (flag & IT_PROTO_V) {
        memset(&it_v, 0, sizeof(it_v));
        it_v.s = 7;
    }

    if (flag & IT_PROTO_F) {
        memset(&it_f, 0, sizeof(it_f));
        it_f.s = 7;
    }
}

void it_result_rst(void)
{
    memset(&it_res, 0, sizeof(it_res));
}

void it_rst(void)
{
    it_decoders_rst(IT_PROTO_ALL);
    it_result_rst();
}

void it_decode_any_proto(const uint16_t interval, const uint8_t pol)
{
    uint8_t proto_sel = IT_PROTO_ALL;

    if (it_res.proto) {
        proto_sel = it_res.proto;
    }

    if (it_res.score_t > IT_SCORING_TRG_LVL) {
        if (it_res.score_v > it_res.score_f) {
            proto_sel = IT_PROTO_V;
            it_res.proto = IT_PROTO_V;
        } else {
            proto_sel = IT_PROTO_F;
            it_res.proto = IT_PROTO_F;
        }
    }

    if (proto_sel & IT_PROTO_F) {
        it_decode_fixed_proto(interval, pol);
    }

    if (proto_sel & IT_PROTO_V) {
        it_decode_variable_proto(interval, pol);
    }

    if ((interval > IT_SCORING_RST_INTERVAL) && (pol == 0)) {
        it_res.proto = IT_PROTO_UNKNOWN;
        it_res.score_t = 0;
        it_res.score_v = 0;
        it_res.score_f = 0;
    }
}

void it_decode_fixed_proto(const uint16_t interval, const uint8_t pol)
{
    uint8_t i;
    uint8_t dev = 0;
    uint8_t cmd = 0;

    if (it_f.cnt > ITF_SIG_SZ) {
        return;
    }

    if (pol) {
        if ((interval > ITF_1T_BLIP_MIN) && (interval < ITF_1T_BLIP_MAX)) {
            if (it_f.s == 7) {
                it_f.b[it_f.cnt] = 0x80;
            } else {
                it_f.b[it_f.cnt] = 0x88;
            }
            it_f.s--;
            it_res.score_f++;
            it_res.score_t++;
        } else if ((interval > ITF_3T_BLIP_MIN) && (interval < ITF_3T_BLIP_MAX)) {
            it_f.b[it_f.cnt] = 0x8e;
            it_f.s = 0;
            it_res.score_f++;
            it_res.score_t++;
        } else {
            it_res.score_f--;
            it_res.score_t++;
        }
    } else {
        if ((interval > ITF_1T_BLIP_MIN) && (interval < ITF_1T_BLIP_MAX)) {
            it_f.s--;
            it_res.score_f++;
            it_res.score_t++;
        } else if ((interval > ITF_3T_BLIP_MIN) && (interval < ITF_3T_BLIP_MAX)) {
            it_f.s-=3;
            it_res.score_f++;
            it_res.score_t++;
        } else if (interval > ITF_CMD_SEP_MIN) {
            // command is done
            if ((it_f.cnt < 10) || (it_res.cnt > IT_RES_MAX_CNT - 1)) {
                it_decoders_rst(IT_PROTO_ALL);
                return;
            }

            for (i=0; i<8; i++) {
                if (it_f.b[i] == 0x8e) {
                    dev |= 1 << (7 - i);
                }
            }

            it_res.dec[it_res.cnt] = rotate_byte(dev);

            for (i=0; i<4; i++) {
                if (it_f.b[8+i] == 0x8e) {
                    cmd |= 1 << (3 - i);
                }
            }

            switch (cmd) {
                case INTERTECHNO_CMD_ON:
                    it_res.dec[it_res.cnt] |= 0xff00;
                    break;
                case INTERTECHNO_CMD_OFF:
                    break;
                default:
                    it_decoders_rst(IT_PROTO_ALL);
                    return;
                    break;
            }

            it_res.cnt++;
            it_last_event = IT_EVENT_DECODE_RDY;
            it_decoders_rst(IT_PROTO_ALL);
        } else {
            it_res.score_f--;
            it_res.score_t++;
        }
    }

    if (it_f.s < 1) {
        it_f.s = 7;
        it_f.cnt++;
    }
}

void it_decode_variable_proto(const uint16_t interval, const uint8_t pol)
{
    uint8_t dev;
    uint8_t cmd;

    if (it_v.cnt > ITV_SIG_SZ) {
        return;
    }

    if (pol) {
        if ((interval > ITV_L0_MIN) && (interval < ITV_L0_MAX)) {
            it_res.score_v++;
            it_res.score_t++;
        } else {
            it_res.score_v--;
            it_res.score_t++;
        }

    } else {
        if ((interval > ITV_L0_MIN) && (interval < ITV_L0_MAX)) {
            // logic zero
            it_v.s--;
            it_res.score_v++;
            it_res.score_t++;
        } else if ((interval > ITV_SYNC_MIN) && (interval < ITV_SYNC_MAX)) {
            // we got a low level sync, a new command starts now
            it_decoders_rst(IT_PROTO_V);
            it_res.score_v++;
            it_res.score_t++;
        } else if ((interval >  ITV_L1_MIN) && (interval <  ITV_L1_MAX)) {
            // logic one
            it_v.b[it_v.cnt] |= 1 << it_v.s;
            it_v.s--;
            it_res.score_v++;
            it_res.score_t++;
        } else if (interval > ITV_CMD_SEP_MIN) {
            // command is done. report result only if there are zero inconsistencies
            if ((it_v.cnt < 7) || (it_res.cnt > IT_RES_MAX_CNT - 1)) {
                it_decoders_rst(IT_PROTO_ALL);
                return;
            }
            if (manchester_decode(it_v.b[7], &dev) == EXIT_FAILURE) {
                it_decoders_rst(IT_PROTO_ALL);
                return;
            }

            // don't bother using manchester_decode() on the command byte
            // it's much quicker to compare the expected result with our input
            cmd = it_v.b[6] & 0x0f;
            if (cmd == 0x6) {
                cmd = 0x00;
            } else if (cmd == 0x5) {
                cmd = 0xff;
            } else {
                it_decoders_rst(IT_PROTO_ALL);
                return;
            }

            it_res.dec[it_res.cnt] = (cmd << 8) | dev;
            it_res.cnt++;
            it_last_event = IT_EVENT_DECODE_RDY;
            it_decoders_rst(IT_PROTO_ALL);
        } else {
            it_res.score_v--;
            it_res.score_t++;
        }
    }

    if (it_v.s < 0) {
        it_v.s = 7;
        it_v.cnt++;
    }
}

uint8_t manchester_decode(const uint8_t in, uint8_t *out)
{
    uint8_t i;
    uint16_t couple;
    uint8_t errors = 0;

    *out = 0;
    for (i = 0; i < 4; i++) {
        couple = (in >> i*2) & 0x3;

        switch (couple) {
            case 1:
                break;
            case 2:
                *out |= 1 << i;
                break;
            case 0:
            case 3:
                errors = 1;
                break;
        }
    }

    if (errors) {
        return(EXIT_FAILURE);
    }
    return(EXIT_SUCCESS);
}

static uint8_t rotate_byte(uint8_t in)
{
    uint8_t rv = 0;
    rv += (in & 0x10) << 3;
    rv += (in & 0x20) << 1;
    rv += (in & 0x40) >> 1;
    rv += (in & 0x80) >> 3;
    rv += (in & 0x1) << 3;
    rv += (in & 0x2) << 1;
    rv += (in & 0x4) >> 1;
    rv += (in & 0x8) >> 3;
    return rv;
}

void it_tx_cmd(const uint8_t prefix, const uint8_t cmd)
{
    uint8_t p = 0;
    uint8_t rprefix;
    uint8_t it_buff[INTERTECHNO_SEQ_SIZE];
    int8_t i;

    rprefix = rotate_byte(prefix);

    // replace 1 with 0x8e and 0 with 0x88
    for (i = 7; i >= 0; i--) {
        if (rprefix & (1 << i)) {
            it_buff[p] = 0x8e;
        } else {
            it_buff[p] = 0x88;
        }
        p++;
    }

    for (i = 3; i >= 0; i--) {
        if (cmd & (1 << i)) {
            it_buff[p] = 0x8e;
        } else {
            it_buff[p] = 0x88;
        }
        p++;
    }

    // sync sequence
    it_buff[p++] = 0x80;
    it_buff[p++] = 0;
    it_buff[p++] = 0;
    it_buff[p] = 0;

    pwr_mgmt_extend(100);

    if (radio_get_state() == RADIO_STATE_RX) {
        radio_rx_off();
    }
    WriteSingleReg(PKTLEN, INTERTECHNO_SEQ_SIZE * INTERTECHNO_SEQ_REPEAT);

    radio_tx_on();

    // factory remotes send the command sequence 4 times
    for (i = INTERTECHNO_SEQ_REPEAT; i > 0; i--) {
        WriteBurstReg(RF_TXFIFOWR, it_buff, INTERTECHNO_SEQ_SIZE);
    }
    radio_set_state(RADIO_STATE_TX);
    Strobe(RF_STX);             // transmit
}

static void it_tx_handler(uint32_t msg)
{
    Strobe(RF_SIDLE);           // IDLE
    Strobe(RF_SFTX);            // flush TXFIFO
    Strobe(RF_SPWD);            // power-down mode
}

static void it_rx_handler(uint32_t msg)
{
    print_buf(radio_rx_buffer, INTERTECHNO_SEQ_SIZE);
}

static void it_decode_irq(uint32_t msg)
{
    print_buf16(&it_res.dec[0], it_res.cnt);
    it_result_rst();
}

void it_handler_init(void)
{
    eh_register(&it_tx_handler, SYS_MSG_CC_TX);
    eh_register(&it_rx_handler, SYS_MSG_CC_RX);
    eh_register(&it_decode_irq, SYS_MSG_RF_DECODE_RDY);
}

uint8_t it_get_event(void)
{
    return it_last_event;
}

void it_rst_event(void)
{
    it_last_event = IT_EVENT_NULL;
}

