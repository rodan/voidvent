
//   timer a0 handling
//
//   author:          Petre Rodan <2b4eda@subdimension.ro>
//   available from:  https://github.com/rodan/
//   license:         BSD

#include <msp430.h>
#include "timer0_a.h"

void timer0_a_init(void)
{

    // ID__8 & TAIDEX_3
    // 1 tick is TA0_DIVIDER / MCLK == 32/8000000 = 4 us
    // overflow happens after .004*65535 = 262.14 ms

    __disable_interrupt();
    TA0CTL = TASSEL__SMCLK + MC__CONTINOUS + TACLR + ID__8; // divide SMCLK by 8
#if defined (SMCLK_FREQ_8M) 
    TA0EX0 = TAIDEX_3; // further divide SMCLK by 4
#elif defined (SMCLK_FREQ_16M)
    TA0EX0 = TAIDEX_7; // further divide SMCLK by 8
#endif
    __enable_interrupt();
}

void timer0_a_halt(void)
{
    TA0CTL = MC__STOP;
}

