#ifndef __PWR_MGMT_H__
#define __PWR_MGMT_H__

#define          PWR_MGMT_IDLE  0
#define      PWR_MGMT_RADIO_ON  0x1

void pwr_mgmt_init(void);
void pwr_mgmt_sm(void);
void pwr_mgmt_extend(const uint8_t ticks);

#endif
