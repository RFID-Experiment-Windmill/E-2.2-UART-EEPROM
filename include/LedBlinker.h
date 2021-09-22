/**
 * @file LedBlinker.h
 * @author Windmill_City
 * @brief Blink Led in specific routines
 * @version 0.1
 * @date 2021-09-15
 *
 * @copyright Copyright (c) 2021
 *
 */
#pragma once
#include "BoardBase.h"
#include "stdint.h"

#define Led Led_P1

int isLedOn()
{
    return !Led;
}

/**
 * @brief Call this method every one microsecond to trigger updates
 */
void onSysTickOneMs()
{
    /**
     * @brief Remain lighting time in microsecond
     */
    static uint16_t remainLightingTime;
    /**
     * @brief Remain off time in microsecond
     */
    static uint16_t remainOffTime;
    if (isLedOn()) {
        if (!--remainLightingTime) {
            Led           = 0;
            remainOffTime = 1000;
        }
    }
    else {
        if (!--remainOffTime) {
            Led                = 1;
            remainLightingTime = 1000;
        }
    }
}