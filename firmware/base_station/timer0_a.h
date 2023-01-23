#ifndef __TIMER0_A_H__
#define __TIMER0_A_H__

//#define  _10ms  6666 // 65535*0.01/(12/8000000*65535) aka 0.01/(12/8000000)
#define  _1ms   250
#define  _2ms   500
#define  _4ms   1000 // 0.004/(32/8000000)
#define  _10ms  2500 // 65535*0.01/(32/8000000*65535) aka 0.01/(32/8000000)
#define  _20ms  5000
#define  _260ms  65000

void timer0_a_init(void);
void timer0_a_halt(void);

#endif
