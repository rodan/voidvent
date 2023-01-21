
#include <stdint.h>
#include <msp430.h>
#include "sig.h"
#include "rf1a.h"
#include "radio.h"
#include "test.h"

#define  RSSI_IDX           (PACKET_LEN)    // Index of appended RSSI 
#define  CRC_LQI_IDX        (PACKET_LEN+1)  // Index of appended LQI, checksum
#define  CRC_OK             (BIT7)          // CRC_OK bit

const unsigned char test_tx_buffer[TEST_PACKET_LEN]= {0xAA, 0xBB, 0xCC, 0xDD, 0xEE};

void transmit(unsigned char *buffer, unsigned char length)
{
    RF1AIES |= BIT9;
    RF1AIFG &= ~BIT9;                         // Clear pending interrupts
    RF1AIE |= BIT9;                           // Enable TX end-of-packet interrupt

    WriteBurstReg(RF_TXFIFOWR, buffer, length);
      
    Strobe(RF_STX);                           // Strobe STX
}

void test_transmit(void)
{
    //sig0_on;
    if (radio_get_state() == RADIO_STATE_RX) {
        radio_rx_off();
    }
  
    radio_set_state(RADIO_STATE_TX);
    transmit( (unsigned char*)test_tx_buffer, sizeof test_tx_buffer);
    //sig0_off;
}


