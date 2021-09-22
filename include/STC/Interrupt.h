#pragma once
#include "STC/STCBase.h"

/**
 * @brief Enable interrupt globally
 */
inline void enableGlobalInterrupt(){
    EA = 1;
}

/**
 * @brief Disable interrupt globally
 */
inline void disableGlobalInterrupt()
{
    EA = 0;
}

typedef enum Interrupt { DisableIT, EnableIT } Interrupt_t;