#pragma once
#include "STC/STCBase.h"
#include "STC/Timer/Timer.h"
#include "stdint.h"

/**
 * @brief Set trigger source of BRT
 *
 * @param source trigger source
 */
inline void setBRTSource(TimerTriggerSource_t source)
{
    switch (source) {
        case SysClock_DIV_1: AUXR &= 0xFB; break;
        case SysClock_DIV_12: AUXR |= 0x04; break;
    }
}

#define BaudToReloadValue(baud) ToReloadValue(SysClock / baud)
#define BaudDoubledToReloadValue(baud) ToReloadValue(SysClock / baud / 2)
#define BaudToReloadValueDIV12(baud) ToReloadValueDIV12(SysClock / baud)
#define BaudDoubledToReloadValueDIV12(baud) ToReloadValueDIV12(SysClock / baud / 2)

/**
 * @brief Set reload value of BRT
 *
 * @param reloadValue reload value
 */
inline void reloadBRT(uint16_t reloadValue)
{
    BRT = reloadValue;
}

/**
 * @brief Start BRT
 */
inline void startBRT()
{
    AUXR |= 0x10;
}

/**
 * @brief Stop BRT
 */
inline void stopBRT()
{
    AUXR &= 0xEF;
}

/**
 * @brief Serial port of the STC Chip
 */
typedef enum SerialPort { Port1, Port2 } SerialPort_t;

typedef enum BaudGenerator {
    SysClk,
    /**
     * @brief Alternative BaudGenerator for Port1
     * Port2 can only use Timer_BRT as its Baud Generator
     */
    Timer_1,
    Timer_BRT
} BaudGenerator_t;

typedef enum PortMode {
    UART8,
    UART9,
    /**
     * @brief Multi-machine communication
     */
    EUART
} PortMode_t;

typedef enum BaudMode { Normal, Double } BaudMode_t;

typedef struct PortCfg
{
    PortMode_t      mode;
    BaudGenerator_t baudGen;
    /**
     * @brief Contorl if Double Baud Rate
     * Ignore if PortMode is UART8 and its BaudGen is SysClk
     */
    BaudMode_t baudMode;
} PortCfg_t, *pPortCfg;

/**
 * @brief Configure Serial Port
 *
 * @param port which port to config
 * @param cfg configuration
 */
void configurePort(SerialPort_t port, pPortCfg cfg)
{
    switch (port) {
        case Port1:
            // Disable Address select
            SM2 = 0;
            switch (cfg->mode) {
                case UART8: SM0 = 0; break;
                case EUART:
                    // Select Address at initial
                    SM2 = 1;
                case UART9: SM1 = 1; break;
            }
            switch (cfg->baudGen) {
                case Timer_1:
                    SM1 = 1;
                    // Select Timer 1
                    AUXR &= 0xFE;
                    startTimer(Timer1);
                    break;
                case Timer_BRT:
                    SM1 = 1;
                    // Select BRT
                    AUXR |= 0x01;
                    startBRT();
                    break;
                case SysClk: SM1 = 0; break;
            }
            switch (cfg->baudMode) {
                case Normal: PCON &= 0x7F; break;
                case Double: PCON |= 0x80; break;
            }
            break;
        case Port2:
            // Disable Address select
            S2CON &= 0xEF;
            switch (cfg->mode) {
                case UART8: S2CON &= 0x7F; break;
                case EUART:
                    // Select Address at initial
                    S2CON |= 0x20;
                case UART9: S2CON |= 0x80; break;
            }
            switch (cfg->baudGen) {
                case Timer_1: /* Unsupported */ break;
                case Timer_BRT:
                    S2CON |= 0x40;
                    startBRT();
                    break;
                case SysClk: S2CON &= 0xBF; break;
            }
            switch (cfg->baudMode) {
                case Normal: AUXR &= 0xF7; break;
                case Double: AUXR |= 0x08; break;
            }
            break;
    }
}

inline uint8_t readPort(SerialPort_t port)
{
    switch (port) {
        case Port1: return SBUF;
        case Port2: return S2BUF;
    }
}

inline void writePort(SerialPort_t port, uint8_t data)
{
    switch (port) {
        case Port1: SBUF = data;
        case Port2: S2BUF = data;
    }
}

/**
 * @brief Check if TI is set
 * 
 * @param port which port to check
 * @return TI of specific port
 */
inline int checkTI(SerialPort_t port)
{
    switch (port) {
        case Port1: return TI;
        case Port2: return S2CON & 0x02;
    }
}

/**
 * @brief Clear TI bit of specific port
 *
 * @param port which port to clear
 */
inline void clearTI(SerialPort_t port)
{
    switch (port) {
        case Port1: TI = 0;
        case Port2: S2CON &= 0xFD;
    }
}

/**
 * @brief Check if RI is set
 * 
 * @param port which port to check
 * @return RI of specific port
 */
inline int checkRI(SerialPort_t port)
{
    switch (port) {
        case Port1: return RI;
        case Port2: return S2CON & 0x01;
    }
}

/**
 * @brief Clear RI bit of specific port
 *
 * @param port which port to clear
 */
inline void clearRI(SerialPort_t port)
{
    switch (port) {
        case Port1: RI = 0;
        case Port2: S2CON &= 0xFE;
    }
}

/**
 * @brief Send data and handle TI in isr
 * 
 * @param port which port to send
 * @param data data to send
 */
inline void sendDataTI(SerialPort_t port, uint8_t data)
{
    writePort(port, data);
}

/**
 * @brief Send data and wait before TI is set
 * 
 * @param port which port to send
 * @param data data to send
 */
inline void sendData(SerialPort_t port, uint8_t data)
{
    sendDataTI(port, data);
    while (!checkTI(port))
        ;
    clearTI(port);
}