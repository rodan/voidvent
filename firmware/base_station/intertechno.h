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

#ifndef __INTERTECHNO_H__
#define __INTERTECHNO_H__

#define     INTERTECHNO_CMD_ON  0x07    // command for turning switches on
#define    INTERTECHNO_CMD_OFF  0x06    // command for turning switches off
#define     INTERTECHNO_CMD_SP  0x0f    // special devices like doorbells, PIR detectors use this cmd
#define   INTERTECHNO_SEQ_SIZE  16      // sequence buffer size to be allocated
#define INTERTECHNO_SEQ_REPEAT  4       // how many times will the sequence be repeated

#define           ITV_VARIANCE  24      // 96us
#define         ITV_BLIP_TICKS  70      // 280us
#define             ITV_SYNC_L  670     // 2680us
#define           ITV_WORD_SEP  325     // 1300us
#define            ITV_CMD_SEP  2600    // 10400us

#define           ITV_BLIP_MIN  (ITV_BLIP_TICKS - ITV_VARIANCE)
#define           ITV_BLIP_MAX  (ITV_BLIP_TICKS + ITV_VARIANCE)

#define         ITV_SYNC_L_MIN  (ITV_SYNC_L - ITV_VARIANCE)
#define         ITV_SYNC_L_MAX  (ITV_SYNC_L + ITV_VARIANCE)

#define       ITV_WORD_SEP_MIN  (ITV_WORD_SEP - ITV_VARIANCE)
#define       ITV_WORD_SEP_MAX  (ITV_WORD_SEP + ITV_VARIANCE)

#define        ITV_CMD_SEP_MIN  (ITV_CMD_SEP - ITV_VARIANCE)
#define        ITV_CMD_SEP_MAX  (ITV_CMD_SEP + ITV_VARIANCE)

#ifdef __cplusplus
extern "C" {
#endif

void it_handler_init(void);
void it_tx_cmd(const uint8_t prefix, const uint8_t cmd);

void it_decode_rst(void);
void it_decode(const uint16_t interval, const uint8_t pol);
void it_decode_debug(void);

void it_cmd_rst(void);

#ifdef __cplusplus
}
#endif

#endif

