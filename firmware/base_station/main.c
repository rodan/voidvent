
#include <msp430.h>
#include <stdio.h>
#include <string.h>

#include "proj.h"
#include "driverlib.h"
#include "glue.h"
#include "intertechno.h"
#include "rf1a.h"
#include "radio.h"
#include "ui.h"
#include "test.h"
#include "sig.h"

uart_descriptor bc; ///< backchannel uart interface
sch_descriptor sch; ///< scheduler structure
volatile uint8_t port1_last_event = 0;

static void uart_bcl_rx_irq(uint32_t msg)
{
    parse_user_input();
    uart_set_eol(&bc);
}

void check_events(void)
{
    uint16_t msg = SYS_MSG_NULL;
    uint16_t ev = 0;

    // uart RX
    if (uart_get_event(&bc) & UART_EV_RX) {
        msg |= SYS_MSG_UART_BCL_RX;
        uart_rst_event(&bc);
    }
    // transceiver
    if (radio_get_event() & RADIO_TX_IRQ) {
        msg |= SYS_MSG_CC_TX;
        radio_rst_event();
    } else if (radio_get_event() & RADIO_RX_IRQ) {
        msg |= SYS_MSG_CC_RX;
        radio_rst_event();
    }
    // scheduler
    if (sch_get_event(&sch)) {
        msg |= SYS_MSG_SCHEDULER;
        sch_rst_event(&sch);
    }
    ev = sch_get_event_schedule(&sch);
    if (ev) {
        if (ev & (1 << SCHEDULE_PARSE_TMOUT)) {
            msg |= SYS_MSG_PARSE_TMOUT;
        }
        sch_rst_event_schedule(&sch);
    }

    eh_exec(msg);
}

void init_button(void)
{
    // Set up the button as interruptible 
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1IES &= ~BIT1;
    P1IFG = 0;
    P1OUT |= BIT1;
    P1IE |= BIT1;
}

static void scheduler_irq(uint32_t msg)
{
    sch_handler(&sch);
}

int main(void)
{
    // stop watchdog
    WDTCTL = WDTPW | WDTHOLD;
    msp430_hal_init(HAL_GPIO_DIR_OUTPUT | HAL_GPIO_OUT_LOW);
#ifdef USE_SIG
    //sig0_on;
#endif

    // enable GDO0
    P1SEL |= BIT0;
    // enable GDO1
    P3SEL |= BIT6;
    // enable GDO2
    P1SEL |= BIT1;

    PMM_setVCore(2);
    clock_pin_init();
    clock_init();

    bc.baseAddress = USCI_A0_BASE;
    bc.baudrate = BAUDRATE_57600;
    uart_pin_init(&bc);
    uart_init(&bc);
#if defined UART_RX_USES_RINGBUF
    uart_set_rx_irq_handler(&bc, uart_rx_ringbuf_handler);
#else
    uart_set_rx_irq_handler(&bc, uart_rx_simple_handler);
#endif

    sch.baseAddress = TIMER_A1_BASE;
    sch_init(&sch);

    ResetRadioCore();
    InitRadio();

    radio_rx_on();

#if defined (CONFIG_BUTTON)
    init_button();
    // enable button irq
    P1IFG = 0;
    P1IE |= BIT1;
#endif

#ifdef USE_SIG
    //sig0_off;
    sig1_off;
    sig2_off;
    sig3_off;
    sig4_off;
    //sig5_off;
#endif

    eh_init();
    eh_register(&uart_bcl_rx_irq, SYS_MSG_UART_BCL_RX);
    eh_register(&scheduler_irq, SYS_MSG_SCHEDULER);
    it_handler_init();
    _BIS_SR(GIE);

    display_version();

    while (1) {
        // sleep
//#ifdef USE_SIG
        sig4_off;
//#endif
        _BIS_SR(LPM0_bits + GIE);
//#ifdef USE_SIG
        sig4_on;
//#endif
        __no_operation();
//#ifdef USE_WATCHDOG
//        WDTCTL = (WDTCTL & 0xff) | WDTPW | WDTCNTCL;
//#endif
        check_events();
        check_events();
        check_events();

#if 0
        if (port1_last_event) {
            port1_last_event = 0;
            test_transmit();
            P1IE |= BIT1;
        }
#endif

        if (radio_get_state() == RADIO_STATE_IDLE) {
            radio_rx_on();
        }
    }

}

#if defined (CONFIG_BUTTON)

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt void port1_isr_handler(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(PORT1_VECTOR))) port1_isr_handler(void)
#else
#error Compiler not supported!
#endif
{
    switch (__even_in_range(P1IV, 16)) {
    case 0:
        break;
    case 2:
        break;                  // P1.0 IFG
    case 4:                    // P1.1 IFG
        P1IE = 0;
        port1_last_event |= BIT1;
        P1IFG = 0;
        __bic_SR_register_on_exit(LPM3_bits);
        break;
    case 6:
        break;                  // P1.2 IFG
    case 8:
        break;                  // P1.3 IFG
    case 10:
        break;                  // P1.4 IFG
    case 12:
        break;                  // P1.5 IFG
    case 14:
        break;                  // P1.6 IFG
    case 16:
        break;                  // P1.7 IFG
    }
}

#endif
