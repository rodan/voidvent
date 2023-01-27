
#include <stdint.h>
#include "glue.h"
#include "sig.h"
#include "proj.h"
#include "radio.h"
#include "pwr_mgmt.h"

uint8_t pm_state = PWR_MGMT_IDLE;
extern sch_descriptor sch;

void pwr_mgmt_sm(void)
{

    switch (pm_state) {
        case PWR_MGMT_IDLE:
            sig2_off;
            sch_set_trigger_slot(&sch, SCHEDULE_PWR_SM, systime(&sch) + 20, SCH_EVENT_ENABLE);
            pm_state = PWR_MGMT_RADIO_ON;
            radio_rx_off();
            radio_sleep_en();
            break;
        case PWR_MGMT_RADIO_ON:
            sig2_on;
            sch_set_trigger_slot(&sch, SCHEDULE_PWR_SM, systime(&sch) + 1, SCH_EVENT_ENABLE);
            pm_state = PWR_MGMT_IDLE;
            radio_rx_on();
            break;
        default:
            pm_state = PWR_MGMT_IDLE;
            break;
    }
}

void pwr_mgmt_extend(const uint8_t ticks)
{
    sch_set_trigger_slot(&sch, SCHEDULE_PWR_SM, systime(&sch) + ticks, SCH_EVENT_ENABLE);
}

void pwr_mgmt_init(void)
{
    pm_state = PWR_MGMT_RADIO_ON;
    sch_set_trigger_slot(&sch, SCHEDULE_PWR_SM, systime(&sch) + 100, SCH_EVENT_ENABLE);
}

