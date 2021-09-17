#include "BoardBase.h"
#include "LedBlinker.h"
#include "STC/Serial/Serial.h"
#include "STC/Timer/Timer.h"

/**
 * @brief ISR Handler for Timer 0
 */
INTERRUPT(isrTimer0, 1)
{
    reloadTimer(Timer0, ToReloadValueDIV12(SysClockOfOneMs));
    onSysTickOneMs();
}

/**
 * @brief Initialize Timer 0
 */
void initTimer0()
{
    TimerCfg_t cfg = {.source = SysClock_DIV_12, .mode = Counter_BIT_16, .gate = Ignore};
    configureTimer(Timer0, &cfg);
    reloadTimer(Timer0, ToReloadValueDIV12(SysClockOfOneMs));
    startTimer(Timer0);
}

void main()
{
    initTimer0();
    while (1) {}
}