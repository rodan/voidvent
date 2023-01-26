#ifndef __PWR_MGMT_H__
#define __PWR_MGMT_H__

#define          PWR_MGMT_IDLE  0
#define  PWR_MGMT_RF_RDY_TMOUT  0x1
#define  PWR_MGMT_RSSI_H_TMOUT  0x2
#define  PWR_MGMT_DECODE_TMOUT  0x4
#define      PWR_MGMT_TX_TMOUT  0x8
#define     PWR_MGMT_RADIO_ON   0x10
#define     PWR_MGMT_RADIO_OFF  0x20

void pwr_mgmt_init(void);
void pwr_mgmt_sm(void);

#endif
