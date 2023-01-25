
#include <stdint.h>
#include "sig.h"
#include "proj.h"
#include "intertechno.h"
#include "timer0_a.h"
#include "rf1a.h"
#include "radio.h"

uint8_t radio_rx_buffer[RADIO_RXBUF_SZ];
volatile uint8_t radio_last_event = RADIO_NO_IRQ;
uint8_t radio_state = RADIO_STATE_IDLE;

uint16_t last_ccr = 0;

uint8_t radio_get_event(void)
{
    return radio_last_event;
}

void radio_rst_event(void)
{
    radio_last_event = RADIO_NO_IRQ;
}

uint8_t radio_get_state(void)
{
    return radio_state;
}

void radio_set_state(const uint8_t state)
{
    radio_state = state;
}

void radio_parse_on(void)
{
    // set up an interrupt based on GDO1 signal which is setup in asynchronous serial mode
    // we are most probably already on a high level, trigger on a falling edge first
    RF1AIES |= BIT1;
    RF1AIFG &= ~BIT1;           // Clear a pending interrupt

    timer0_a_init();
    it_rst();
    last_ccr = TA0R;
    RF1AIE |= BIT1;             // Enable the interrupt
}

void radio_parse_off(void)
{
    RF1AIE &= ~BIT1;            // Enable the interrupt
    RF1AIFG &= ~BIT1;           // Clear a pending interrupt

    // disable timer0a
    timer0_a_halt();
}

void radio_tx_on(void)
{
    // set an interrupt to trigger when the packet is fully sent
    RF1AIES |= BIT9;
    RF1AIFG &= ~BIT9;           // Clear pending interrupts
    RF1AIE |= BIT9;             // Enable TX end-of-packet interrupt
}

void radio_rx_on(void)
{
    // RX is done in asyncronous mode
    WriteSingleReg(PKTCTRL0, 0x30);
#if CONFIG_RX_FIFO
    // set up FIFO-based receive
    RF1AIES |= BIT9;            // Falling edge of RFIFG9
    RF1AIFG &= ~BIT9;           // Clear a pending interrupt
    RF1AIE |= BIT9;             // Enable the interrupt 
#endif

    RF1AIFG &= ~BITD;           // Clear a pending interrupt
    RF1AIE |= BITD;             // Enable the interrupt 

    // Radio is in IDLE following a TX, so strobe SRX to enter Receive Mode
    Strobe(RF_SRX);
    radio_state = RADIO_STATE_RX;
}

void radio_rx_off(void)
{
    RF1AIE &= ~BIT9;            // Disable RX interrupts
    RF1AIFG &= ~BIT9;           // Clear pending IFG

    RF1AIE &= ~BIT1;            // Enable the interrupt
    RF1AIFG &= ~BIT1;           // Clear a pending interrupt

    RF1AIE &= ~BITD;            // Disable RX interrupts
    RF1AIFG &= ~BITD;           // Clear pending IFG

    // back to non-async mode
    WriteSingleReg(PKTCTRL0, 0x00);

    // It is possible that radio_rx_off is called while radio is receiving a packet.
    // Therefore, it is necessary to flush the RX FIFO after issuing IDLE strobe 
    // such that the RXFIFO is empty prior to receiving a packet.
    Strobe(RF_SIDLE);
    Strobe(RF_SFRX);
    radio_state = RADIO_STATE_IDLE;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=CC1101_VECTOR
__interrupt void cc1101_isr_handler(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(CC1101_VECTOR))) cc1101_isr_handler(void)
#else
#error Compiler not supported!
#endif
{
    uint8_t rx_sz;
    uint8_t pol;
    uint16_t cur_ccr;

    sig3_on;
    switch (__even_in_range(RF1AIV, 32)) {
    case 0:                     // No RF core interrupt pending
        break;
    case 2:                     // RFIFG0
        // irq based on GDO0 signal - programmable using IOCFG0 (0x02) register of radio core
        break;
    case 4:                     // RFIFG1
        // irq based on GDO1 signal - programmable using IOCFG1 (0x01) register of radio core
        cur_ccr = TA0R;
        if (RF1AIN & BIT1) {
            // high
            sig1_on;
            pol = 0;
            // trigger on a falling edge next
            RF1AIES |= BIT1;
        } else {
            // low
            sig1_off;
            pol = 1;
            // trigger on a rising edge next
            RF1AIES &= ~BIT1;
        }
        // changing RF1IES can trigger a false interrupt flag
        // so clear it preemptively
        RF1AIFG &= ~BIT1;
        it_decode_any_proto(cur_ccr - last_ccr, pol);
        last_ccr = cur_ccr;
        break;
    case 6:                     // RFIFG2
        // irq based on GDO2 signal - programmable using IOCFG2 (0x00) register of radio core
        break;
    case 8:                     // RFIFG3
        break;
    case 10:                    // RFIFG4
        break;
    case 12:                    // RFIFG5
        break;
    case 14:                    // RFIFG6
        break;
    case 16:                    // RFIFG7
        break;
    case 18:                    // RFIFG8
        break;
    case 20:                    // RFIFG9
        // Positive edge: Sync word sent or received.
        // RFIFG9 Negative edge: End of packet or in RX when optional address check fails or RX FIFO overflows or in TX
        // when TX FIFO underflows. (Equal to GDOx_CFG=6)

        // in case of intertechno signals, the RX FIFO is not used, and the signal is parsed via RFIFG1
        if (radio_state == RADIO_STATE_TX) {
            RF1AIE &= ~BIT9;    // Disable TX end-of-packet interrupt
            radio_last_event = RADIO_TX_IRQ;
            radio_state = RADIO_STATE_IDLE;
        } else if (radio_state == RADIO_STATE_RX) {
            rx_sz = ReadSingleReg(RXBYTES);
            if (rx_sz > RADIO_RXBUF_SZ) {
                rx_sz = RADIO_RXBUF_SZ;
            }
            ReadBurstReg(RF_RXFIFORD, radio_rx_buffer, rx_sz);
            __no_operation();
            radio_rx_on();
            radio_last_event = RADIO_RX_IRQ;
        } else {
            radio_state = RADIO_STATE_IDLE;
        }
        break;
    case 22:                    // RFIFG10
        break;
    case 24:                    // RFIFG11
        break;
    case 26:                    // RFIFG12
        break;
    case 28:                    // RFIFG13
        cur_ccr = TA0R;
        // Positive edge: Carrier sense. RSSI level is above threshold.
        // Negative edge: RSSI level is below threshold. (Equal to GDOx_CFG=14)
        //sig2_switch; // page 675
        if (RF1AIN & BITD) {
            // signal detected, start parsing
            radio_parse_on();
            // trigger on falling edge next
            RF1AIES |= BITD;
        } else {
            // force decode the last command, 
            // don't count on pol being (RF1AIN & BIT1)
            it_decode_any_proto(cur_ccr - last_ccr, 0);

            // signal lost, stop parsing
            radio_parse_off();
            // trigger on rising edge next
            RF1AIES &= ~BITD;
        }
        // changing RF1IES can trigger a false interrupt flag
        // so clear it preemptively
        RF1AIFG &= ~BITD;

        break;
    case 30:                    // RFIFG14
        break;
    case 32:                    // RFIFG15
        break;
    }
    sig3_off;
    __bic_SR_register_on_exit(LPM3_bits);
}
