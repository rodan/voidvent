
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

void it_handler_init(void)
{
    eh_register(&it_tx_handler, SYS_MSG_CC_TX);
    eh_register(&it_rx_handler, SYS_MSG_CC_RX);
}

void it_rf_init(void)
{
    // logic 0 and logic 1 power levels for OOK modulation
#ifdef CONFIG_MOD_INTERTECHNO_PW
    uint8_t PATable[2] = { 0x00, it_pwr[it_pwr_level] };
#else
    uint8_t PATable[2] = { 0x00, INTERTECHNO_RF_POWER };
#endif

    ResetRadioCore();

    // minimal register changes
    WriteSingleReg(IOCFG0, 0x06);       //GDO0 Output Configuration
    WriteSingleReg(PKTLEN, INTERTECHNO_SEQ_SIZE*4);       //Packet Length
    WriteSingleReg(PKTCTRL0, 0x00);     //Packet Automation Control
    WriteSingleReg(FREQ2, 0x10);        //Frequency Control Word, High Byte
    WriteSingleReg(FREQ1, 0xB0);        //Frequency Control Word, Middle Byte
    WriteSingleReg(FREQ0, 0x71);        //Frequency Control Word, Low Byte
    WriteSingleReg(MDMCFG4, 0x86);      //Modem Configuration
    WriteSingleReg(MDMCFG3, 0x70);      //Modem Configuration
    WriteSingleReg(MDMCFG2, 0x30);      //Modem Configuration   0x32 sync mode!!
    WriteSingleReg(MDMCFG1, 0x02);      //Modem Configuration
    WriteSingleReg(MCSM1, 0x00);        //Main Radio Control State Machine Configuration
    WriteSingleReg(MCSM0, 0x00);        //Main Radio Control State Machine Configuration
    WriteSingleReg(FOCCFG, 0x76);       //Frequency Offset Compensation Configuration
    WriteSingleReg(WOREVT1, 0x87);      //High Byte Event0 Timeout
    WriteSingleReg(WOREVT0, 0x6B);      //Low Byte Event0 Timeout
    WriteSingleReg(WORCTRL, 0xF8);      //Wake On Radio Control  sets a reserved bit ?!
    WriteSingleReg(FREND0, 0x11);       //Front End TX Configuration
    WriteSingleReg(TEST0, 0x09);        //Various Test Settings

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

