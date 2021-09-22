#include "BoardBase.h"

#include "STC/Timer/Timer.h"
#include "STC/UART/UART.h"

#include "AlphaSender.h"
#include "LedBlinker.h"

/**
 * @brief ISR Handler for Timer 0
 */
INTERRUPT(isrTimer0, 1)
{
    reloadTimer(Timer0, ToReloadValueDIV12(SysClockOfOneMs));
    onSysTickOneMs();
}

INTERRUPT(isrUART1, 4)
{
    if (checkRI(Port1)) {
        sendAlpha(readPort(Port1));
        clearRI(Port1);
    }
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

void initUART1()
{
    setBRTSource(SysClock_DIV_12);
    PortCfg_t cfg = {.mode = UART8, .baudGen = Timer_BRT, .baudMode = Normal};
    reloadBRT(BaudToReloadValueDIV12(9600));

    configurePort(Port1, &cfg);
}

void main()
{
    initTimer0();
    initUART1();
    enableIAP();
    sendSavedData();
    while (1) {}
}