
#include <stdint.h>
#include "sig.h"
#include "proj.h"
#include "radio.h"

volatile uint8_t radio_last_event = 0;

uint8_t radio_get_event(void)
{
    return radio_last_event;
}

void radio_rst_event(void)
{
    radio_last_event = 0;
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
    switch (__even_in_range(RF1AIV, 32))
    {
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
    case 20:                   // RFIFG9
        radio_last_event |= RADIO_TX_IRQ;
        RF1AIE &= ~BIT9;        // Disable TX end-of-packet interrupt
        sig0_off;
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
    __bic_SR_register_on_exit(LPM3_bits);
}
