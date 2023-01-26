
#include <stdint.h>
#include "glue.h"
#include "sig.h"
#include "proj.h"
#include "pwr_mgmt.h"

uint8_t pm_state = PWR_MGMT_IDLE;
extern sch_descriptor sch;

void pwr_mgmt_sm(void)
{

    switch (pm_state) {
        case PWR_MGMT_IDLE:
            sig2_switch;
            sch_set_trigger_slot(&sch, SCHEDULE_PWR_SM, systime(&sch) + 12, SCH_EVENT_ENABLE);
            pm_state = PWR_MGMT_RADIO_ON;
            break;
        case PWR_MGMT_RADIO_ON:
            sig2_switch;
            sch_set_trigger_slot(&sch, SCHEDULE_PWR_SM, systime(&sch) + 2, SCH_EVENT_ENABLE);
            pm_state = PWR_MGMT_RF_RDY_TMOUT;
            break;
        case PWR_MGMT_RF_RDY_TMOUT:
            sig2_switch;
            sch_set_trigger_slot(&sch, SCHEDULE_PWR_SM, systime(&sch) + 4, SCH_EVENT_ENABLE);
            pm_state = PWR_MGMT_IDLE;
            break;
        case PWR_MGMT_RSSI_H_TMOUT:
            break;
        case PWR_MGMT_DECODE_TMOUT:
            break;
        case PWR_MGMT_TX_TMOUT:
            break;
        case PWR_MGMT_RADIO_OFF:
            break;
        default:
            pm_state = PWR_MGMT_IDLE;
            break;
    }
}

void pwr_mgmt_init(void)
{
    pm_state = PWR_MGMT_RADIO_ON;
    sch_set_trigger_slot(&sch, SCHEDULE_PWR_SM, systime(&sch) + 100, SCH_EVENT_ENABLE);
}

