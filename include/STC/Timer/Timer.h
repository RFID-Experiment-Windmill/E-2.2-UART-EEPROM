#pragma once
#include "STC/STCBase.h"
#include "stdint.h"

/**
 * @brief Timer of the STC Chip
 */
typedef enum Timer { Timer0, Timer1 } Timer_t;

typedef enum TimerTriggerSource {
    /**
     * @brief Using SysClock/12 as trigger source
     */
    SysClock_DIV_12,
    /**
     * @brief Using SysClock as trigger source
     */
    SysClock_DIV_1,
    /**
     * @brief Using external trigger source
     * Timer0 -> T0/P3.4
     * Timer1 -> T1/P3.5
     */
    External
} TimerTriggerSource_t;

typedef enum TimerMode {
    Counter_BIT_13,
    Counter_BIT_16,
    /**
     * @brief Auto reload TLx from THx when overflow
     */
    Counter_BIT_8_AutoReload,
    /**
     * @brief Make Timer0 act as a double 8 bit counter/timer
     * TL0 holds value of counter0, using control bits of timer0
     * TH0 holds value of counter1, using control bits of timer1
     *
     * If the configure target is timer1, timer1 will stop in this mode
     */
    Counter_BIT_8_Double_Timer_WITH_SAME_TRIGGER_SOURCE
} TimerMode_t;

typedef enum TimerGateMode {
    /**
     * @brief Enable timer if TRx is High
     * no matter what value INTx is
     */
    Ignore,
    /**
     * @brief Enable timer if TRx is High
     * while the value of INTx is High
     */
    Enable_WHEN_INT_HIGH
} TimerGateMode_t;

typedef struct TimerCfg
{
    TimerTriggerSource_t source;
    TimerMode_t          mode;
    TimerGateMode_t      gate;
} TimerCfg_t, *pTimerCfg;

void configureTimer(Timer_t timer, pTimerCfg cfg)
{
    switch (timer) {
        case Timer0:
            TMOD &= 0xF0;// 1111_0000
            TF0 = 0;     // Clear Interrupt Flag
            ET0 = 1;     // Enable Interrupt
            switch (cfg->source) {
                case External: TMOD |= 0x04; break;
                case SysClock_DIV_1: AUXR |= 0x80; break;
                case SysClock_DIV_12: AUXR &= 0x7F; break;
            }
            switch (cfg->mode) {
                case Counter_BIT_13: /*Already set to 00*/ break;
                case Counter_BIT_16: TMOD |= 0x01; break;
                case Counter_BIT_8_AutoReload: TMOD |= 0x02; break;
                case Counter_BIT_8_Double_Timer_WITH_SAME_TRIGGER_SOURCE: TMOD |= 0x03; break;
            }
            switch (cfg->gate) {
                case Ignore: /*Already set to 0*/ break;
                case Enable_WHEN_INT_HIGH: TMOD |= 0x08; break;
            }
            break;
        case Timer1:
            TMOD &= 0x0F;// 0000_1111
            TF1 = 0;     // Clear Interrupt Flag
            ET1 = 1;     // Enable Interrupt
            switch (cfg->source) {
                case External: TMOD |= 0x40; break;
                case SysClock_DIV_1: AUXR |= 0x40; break;
                case SysClock_DIV_12: AUXR &= 0xBF; break;
            }
            switch (cfg->mode) {
                case Counter_BIT_13: /*Already set to 00*/ break;
                case Counter_BIT_16: TMOD |= 0x10; break;
                case Counter_BIT_8_AutoReload: TMOD |= 0x20; break;
                case Counter_BIT_8_Double_Timer_WITH_SAME_TRIGGER_SOURCE: TMOD |= 0x30; break;
            }
            switch (cfg->gate) {
                case Ignore: /*Already set to 0*/ break;
                case Enable_WHEN_INT_HIGH: TMOD |= 0x80; break;
            }
            break;
    }
}

#define ToReloadValue(x) (0xFFFF - x)
#define ToReloadValueDIV12(x) (0xFFFF - x / 12)

inline void reloadTimer(Timer_t timer, uint16_t reloadValue)
{
    switch (timer) {
        case Timer0:
            TH0 = reloadValue >> 8;
            TL0 = reloadValue;
            break;
        case Timer1:
            TH1 = reloadValue >> 8;
            TL1 = reloadValue;
            break;
    }
}

inline void startTimer(Timer_t timer)
{
    switch (timer) {
        case Timer0: TR0 = 1; break;
        case Timer1: TR1 = 1; break;
    }
}

inline void stopTimer(Timer_t timer)
{
    switch (timer) {
        case Timer0: TR0 = 0; break;
        case Timer1: TR1 = 0; break;
    }
}
