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
* \addtogroup Hamma weatherstation trio RF protocol
* \brief radio protocol for the hamma EWS-Trio
* \author Petre Rodan
* 
* \{
**/

#ifndef __HAMMA_EWS_H__
#define __HAMMA_EWS_H__

#define        HEWS_EVENT_NULL  0
#define  HEWS_EVENT_DECODE_RDY  1

#define          HEWS_VARIANCE  30  // 120us
#define           HEWS_BIT_SEP  126 // 506us
#define                HEWS_L0  247 // 990us
#define                HEWS_L1  492 // 1967us
#define           HEWS_CMD_SEP  992 // 3967us

#define       HEWS_BIT_SEP_MIN  (HEWS_BIT_SEP - HEWS_VARIANCE)
#define       HEWS_BIT_SEP_MAX  (HEWS_BIT_SEP + HEWS_VARIANCE)
#define            HEWS_L0_MIN  (HEWS_L0 - HEWS_VARIANCE)
#define            HEWS_L0_MAX  (HEWS_L0 + HEWS_VARIANCE)
#define            HEWS_L1_MIN  (HEWS_L1 - HEWS_VARIANCE)
#define            HEWS_L1_MAX  (HEWS_L1 + HEWS_VARIANCE)
#define       HEWS_CMD_SEP_MIN  (HEWS_CMD_SEP - HEWS_VARIANCE)

#ifdef __cplusplus
extern "C" {
#endif

void hews_rst(void);
void hews_handler_init(void);
void hews_decode(const uint16_t interval, const uint8_t pol);

uint8_t hews_get_event(void);
void hews_rst_event(void);

#ifdef __cplusplus
}
#endif

#endif

