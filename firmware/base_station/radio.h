/*
* Copyright (c) 2023, Petre Rodan
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met: 
* 
* 1. Redistributions of source code must retain the above copyright notice, this
*    list of conditions and the following disclaimer. 
* 2. Redistributions in binary form must reproduce the above copyright notice,
*    this list of conditions and the following disclaimer in the documentation
*    and/or other materials provided with the distribution. 
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/**
* \addtogroup intertechno radio controlled automation systems
* \brief driver for the Intertechno protocol
* \author Petre Rodan
* 
* \{
**/

#ifndef __RADIO_H__
#define __RADIO_H__

#define     RADIO_NO_IRQ  0x0
#define     RADIO_TX_IRQ  0x1
#define     RADIO_RX_IRQ  0x2

#define RADIO_STATE_IDLE  0x0
#define   RADIO_STATE_RX  0x1
#define   RADIO_STATE_TX  0x2

#define   RADIO_RXBUF_SZ  16

#ifdef __cplusplus
extern "C" {
#endif

uint8_t radio_get_event(void);
void radio_rst_event(void);

void radio_rx_on(void);
void radio_rx_off(void);
void radio_tx_on(void);
void radio_sleep_en(void);

uint8_t radio_get_state(void);
void radio_set_state(const uint8_t state);

void radio_parse_on(void);
void radio_parse_off(void);

#ifdef __cplusplus
}
#endif

#endif

