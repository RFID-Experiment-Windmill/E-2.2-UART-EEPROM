#pragma once
#include "STC/STCBase.h"

/**
 * @brief Enable interrupt globally
 */
inline void enableGlobalInterrupt(){
    IE = 1;
}

/**
 * @brief Disable interrupt globally
 */
inline void disableGlobalInterrupt()
{
    IE = 0;
}

typedef enum Interrupt { DisableIT, EnableIT } Interrupt_t;