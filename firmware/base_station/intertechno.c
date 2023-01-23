
#include <inttypes.h>
#include <msp430.h>
#include <string.h>
#include "glue.h"
#include "proj.h"
#include "sig.h"
#include "rf1a.h"
#include "radio.h"
#include "ui.h"
#include "intertechno.h"

static uint8_t rotate_byte(uint8_t in);
static void it_tx_handler(uint32_t msg);
static void it_rx_handler(uint32_t msg);

extern const RF_SETTINGS rfSettings;
extern uint8_t radio_rx_buffer[RADIO_RXBUF_SZ];

#define      IT_SIG_SZ  33
#define IT_CMD_MAX_CNT  4

uint8_t it_w[IT_SIG_SZ];
uint8_t it_w_cnt = 0;
uint16_t it_cmd[IT_CMD_MAX_CNT];
uint16_t it_cmd_cnt = 0;

void it_handler_init(void)
{
    eh_register(&it_tx_handler, SYS_MSG_CC_TX);
    eh_register(&it_rx_handler, SYS_MSG_CC_RX);
}

void it_decode_rst(void)
{
    it_w_cnt = 0;
    memset(it_w, 0, IT_SIG_SZ);
}

void it_cmd_rst(void)
{
    it_cmd_cnt = 0;
    memset(it_cmd, 0, IT_CMD_MAX_CNT * 2);
}

void it_decode(const uint16_t interval, const uint8_t pol)
{
    uint8_t state;
    uint8_t multiplier;
    uint8_t device;
    uint8_t cmd;

    if (it_w_cnt > IT_SIG_SZ) {
        return;
    }

    if (pol) {
        // could be a high level blip
        if ((interval > ITV_BLIP_MIN) && (interval < ITV_BLIP_MAX)) {
            // count these
            it_w[it_w_cnt]++;
        }
    } else {
        // could be a blip separator, low level sync seq or word separator
        //if ((interval > ITV_BLIP_MIN) && (interval < ITV_BLIP_MAX)) {
        // we got a blip
        //} else 
        if ((interval > ITV_SYNC_L_MIN) && (interval < ITV_SYNC_L_MAX)) {
            // we got a low level sync seq
            // a new command starts now
            it_decode_rst();
        } else if ((interval > ITV_WORD_SEP_MIN) && (interval < ITV_WORD_SEP_MAX)) {
            // word is done
            if (it_w_cnt < IT_SIG_SZ) {
                it_w_cnt++;
            } else {
                it_decode_rst();
            }
        } else if (interval > ITV_CMD_SEP_MIN) {
            // command is done
            if ((it_w_cnt < 10) || (it_cmd_cnt > IT_CMD_MAX_CNT - 1)) {
                it_decode_rst();
                return;
            }

            if ((it_w[it_w_cnt - 9] != 2) || (it_w[it_w_cnt - 8] != 1) || (it_w[it_w_cnt - 7] != 3)
                || (it_w[it_w_cnt - 6] != 2)) {
                it_decode_rst();
                return;
            }

            state = (it_w[it_w_cnt - 5] << 4) | it_w[it_w_cnt - 4];
            multiplier = (it_w[it_w_cnt - 3] << 4) | it_w[it_w_cnt - 2];
            device = (it_w[it_w_cnt - 1] << 4) | it_w[it_w_cnt];

            switch (state) {
            case 0x13:
            case 0x12:
                cmd = 0xff;
                break;
            case 0x22:
            case 0x21:
                cmd = 0;
                break;
            default:
                it_decode_rst();
                return;
            }

            switch (multiplier) {
            case 0x22:
                if ((state == 0x13) || (state == 0x22)) {
                    multiplier = 0;
                } else if ((state == 0x12) || (state == 0x21)) {
                    multiplier = 3;
                } else {
                    it_decode_rst();
                    return;
                }
                break;
            case 0x21:
                multiplier = 0;
                break;
            case 0x13:
            case 0x12:
                multiplier = 1;
                break;
            case 0x32:
            case 0x31:
                multiplier = 2;
                break;
            case 0x23:
                multiplier = 3;
                break;
            default:
                it_decode_rst();
                return;
            }

            switch (device) {
            case 0x21:
                device = 1;
                break;
            case 0x12:
                device = 2;
                break;
            case 0x31:
                device = 3;
                break;
            case 0x22:
                device = 4;
                break;
            default:
                it_decode_rst();
                return;
            }

            it_cmd[it_cmd_cnt] = (cmd << 8) | (4 * multiplier + device);
            it_cmd_cnt++;
        }
    }
}

void it_decode_debug(void)
{
    print_buf16(&it_cmd[0], it_cmd_cnt);
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

    if (radio_get_state() == RADIO_STATE_RX) {
        radio_rx_off();
    }
    WriteSingleReg(PKTLEN, INTERTECHNO_SEQ_SIZE * INTERTECHNO_SEQ_REPEAT);
    Strobe(RF_SCAL);            // re-calibrate radio

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
    //Strobe(RF_SPWD);            // power-down mode
}

static void it_rx_handler(uint32_t msg)
{
    print_buf(radio_rx_buffer, INTERTECHNO_SEQ_SIZE);
}
