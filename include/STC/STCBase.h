#pragma once

#ifndef SDCC
#define SDCC
#endif

#include "STC12C5Axx.h"

/**
 * @brief Frequency of the oscillator
 *
 * Oscillator Frequency -> 6 MHz
 */
#define FreqOsc (6000000)

/**
 * @brief Frequency of the System Clock
 */
#define SysClock (FreqOsc << (CLK_DIV & 0x7))

/**
 * @brief How many SysClock passes in 1 millisecond
 */
#define SysClockOfOneMs (SysClock / 1000)