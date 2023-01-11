
#include <stdint.h>
#include "sig.h"
#include "proj.h"
#include "rf1a.h"
#include "radio.h"

uint8_t radio_rx_buffer[RADIO_RXBUF_SZ];
volatile uint8_t radio_last_event = RADIO_NO_IRQ;
uint8_t radio_state = RADIO_STATE_IDLE;

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

void radio_rx_on(void)
{
    RF1AIES |= BIT9;            // Falling edge of RFIFG9
    RF1AIFG &= ~BIT9;           // Clear a pending interrupt
    RF1AIE |= BIT9;             // Enable the interrupt 

    // Radio is in IDLE following a TX, so strobe SRX to enter Receive Mode
    Strobe(RF_SRX);
    radio_state = RADIO_STATE_RX;
}

void radio_rx_off(void)
{
    RF1AIE &= ~BIT9;            // Disable RX interrupts
    RF1AIFG &= ~BIT9;           // Clear pending IFG

    // It is possible that radio_rx_off is called while radio is receiving a packet.
    // Therefore, it is necessary to flush the RX FIFO after issuing IDLE strobe 
    // such that the RXFIFO is empty prior to receiving a packet.
    Strobe(RF_SIDLE);
    Strobe(RF_SFRX);
    radio_state = RADIO_STATE_IDLE;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT5_VECTOR
__interrupt void cc1101_isr_handler(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(CC1101_VECTOR))) cc1101_isr_handler(void)
#else
#error Compiler not supported!
#endif
{
    uint8_t rx_sz;

    sig3_on;
    switch (__even_in_range(RF1AIV, 32)) {
    case 0:
        break;                  // No RF core interrupt pending
    case 2:
        break;                  // RFIFG0
    case 4:
        break;                  // RFIFG1
    case 6:
        break;                  // RFIFG2
    case 8:
        break;                  // RFIFG3
    case 10:
        break;                  // RFIFG4
    case 12:
        break;                  // RFIFG5
    case 14:
        break;                  // RFIFG6
    case 16:
        break;                  // RFIFG7
    case 18:
        break;                  // RFIFG8
    case 20:                    // RFIFG9
        if (radio_state == RADIO_STATE_TX) {
            RF1AIE &= ~BIT9;    // Disable TX end-of-packet interrupt
            sig0_off;
            radio_last_event = RADIO_TX_IRQ;
            radio_state = RADIO_STATE_IDLE;
        } else if (radio_state == RADIO_STATE_RX) {
            rx_sz = ReadSingleReg(RXBYTES);
            if (rx_sz > RADIO_RXBUF_SZ) {
                rx_sz = RADIO_RXBUF_SZ;
            }
            ReadBurstReg(RF_RXFIFORD, radio_rx_buffer, rx_sz);
            __no_operation();
            radio_last_event = RADIO_RX_IRQ;
        } else {
            radio_state = RADIO_STATE_IDLE;
        }
        break;
    case 22:
        break;                  // RFIFG10
    case 24:
        break;                  // RFIFG11
    case 26:
        break;                  // RFIFG12
    case 28:
        break;                  // RFIFG13
    case 30:
        break;                  // RFIFG14
    case 32:
        break;                  // RFIFG15
    }
    sig3_off;
    __bic_SR_register_on_exit(LPM3_bits);
}
