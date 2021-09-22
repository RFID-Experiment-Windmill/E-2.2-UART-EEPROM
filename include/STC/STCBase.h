#pragma once

#ifndef SDCC
#define SDCC
#endif

#include "STC12C5Axx.h"

/**
 * @brief Frequency of the oscillator
 *
 * Oscillator Frequency -> 11.0592 MHz
 */
#define FreqOsc (11059200)

/**
 * @brief Possible value of the CLK_DIV
 */
#define ClockDivision_0 0x0
#define ClockDivision_2 0x1
#define ClockDivision_4 0x2
#define ClockDivision_8 0x3
#define ClockDivision_16 0x4
#define ClockDivision_32 0x5
#define ClockDivision_64 0x6
#define ClockDivision_128 0x7

/**
 * @brief Division of the oscillator
 */
#define ClockDivision ClockDivision_0

/**
 * @brief Frequency of the System Clock
 */
#define SysClock (FreqOsc << (ClockDivision))

/**
 * @brief How many SysClock passes in 1 millisecond
 */
#define SysClockOfOneMs (SysClock / 1000)
