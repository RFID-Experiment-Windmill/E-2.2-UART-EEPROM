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
    return Led == 0;
}

/**
 * @brief Call this method every one microsecond to trigger updates
 */
void onSysTickOneMs()
{
    /**
     * @brief next lighting time in second
     */
    static uint8_t nextLightingTime = 5;
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
            Led           = 1;
            remainOffTime = 1000;
        }
    }
    else {
        if (!--remainOffTime) {
            Led = 0;
            remainLightingTime = nextLightingTime * 1000;
            if (--nextLightingTime == 0) nextLightingTime = 5;
        }
    }
}