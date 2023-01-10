
#include <inttypes.h>
#include <msp430.h>
#include "glue.h"
#include "proj.h"
#include "rf1a.h"
#include "intertechno.h"

// PATable[1] power level (based on SmartRF Studio)
//#define INTERTECHNO_RF_POWER   0x26     // -12 dBm   ~13mA peak
//#define INTERTECHNO_RF_POWER   0x2d     //  -6 dBm
//#define INTERTECHNO_RF_POWER   0x50     //   0 dBm
#define INTERTECHNO_RF_POWER   0xc6     //  10 dBm   ~18mA peak

uint8_t rotate_byte(uint8_t in);
void it_rf_init(void);
static void it_tx_handler(uint32_t msg);
static void it_rx_handler(uint32_t msg);

extern const RF_SETTINGS rfSettings_fixed;

void it_handler_init(void)
{
    eh_register(&it_tx_handler, SYS_MSG_CC_TX);
    eh_register(&it_rx_handler, SYS_MSG_CC_RX);
}

void it_rf_init(void)
{
    // logic 0 and logic 1 power levels for OOK modulation
    uint8_t PATable[2] = { 0x00, INTERTECHNO_RF_POWER };

    ResetRadioCore();
    WriteRfSettings(&rfSettings_fixed);
    WriteBurstPATable(&PATable[0], 2);
}

uint8_t rotate_byte(uint8_t in)
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

    it_rf_init();

    Strobe(RF_SCAL);            // re-calibrate radio

    // set an interrupt to trigger when the packet is fully sent
    RF1AIES |= BIT9;
    RF1AIFG &= ~BIT9;           // Clear pending interrupts
    RF1AIE |= BIT9;             // Enable TX end-of-packet interrupt

    // factory remotes send the command sequence 4 times
    for (i = 0; i < 4; i++) {
        WriteBurstReg(RF_TXFIFOWR, it_buff, INTERTECHNO_SEQ_SIZE);
    }
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

}

