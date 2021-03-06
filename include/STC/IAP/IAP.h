#pragma once
#include "STC/STCBase.h"

/**
 * @brief Write this key before writing any ISP/IAP CMD
 */
#define IAP_KEY 0xA55A

/**
 * @brief Time to wait for Flash operation of specific SysClock
 */
#define IAP_WAIT_TIME_1MHz 0x7
#define IAP_WAIT_TIME_2MHz 0x6
#define IAP_WAIT_TIME_3MHz 0x5
#define IAP_WAIT_TIME_6MHz 0x4
#define IAP_WAIT_TIME_12MHz 0x3
#define IAP_WAIT_TIME_20MHz 0x2
#define IAP_WAIT_TIME_24MHz 0x1
#define IAP_WAIT_TIME_30MHz 0x0

/**
 * @brief Time to wait for Flash operation
 */
#define IAP_WAIT_TIME IAP_WAIT_TIME_12MHz

/**
 * @brief Bootstrap area
 */
typedef enum BootArea { Application, ISP } BootArea_t;

/**
 * @brief Reset system
 *
 * @param boot where to bootstrap from
 */
inline void resetFrom(BootArea_t boot)
{
    switch (boot) {
        case Application: IAP_CONTR = 0x20; break;
        case ISP: IAP_CONTR = 0x60; break;
    }
}

/**
 * @brief IAP Commands
 */
typedef enum IAPCMD { StandBy, Read, Write, Erase } IAPCMD_t;

/**
 * @brief Enable IAP
 */
inline void enableIAP()
{
    IAP_CONTR |= 0x80;
}

/**
 * @brief Disable IAP
 */
inline void disableIAP()
{
    IAP_CONTR &= 0x7F;
}

/**
 * @brief Check if CMD failed
 *
 * @return CMD_FAIL
 */
inline int checkCMDFail()
{
    return IAP_CONTR & 0x10;
}

/**
 * @brief Clear CMD_FAIL bit
 */
inline void clearCMDFail()
{
    IAP_CONTR &= 0xEF;
}

/**
 * @brief Trigger IAP operation
 */
inline void triggerIAPOp()
{
    IAP_TRIG = IAP_KEY;
    IAP_TRIG = IAP_KEY >> 8;
    NOP();
}

/**
 * @brief Do specific IAP Command
 *
 * @param cmd command
 */
inline void doIAPOp(IAPCMD_t cmd)
{
    enableIAP();
    IAP_CONTR |= IAP_WAIT_TIME;
    clearCMDFail();
    switch (cmd) {
        case StandBy: IAP_CMD = 0x00; break;
        case Read: IAP_CMD = 0x01; break;
        case Write: IAP_CMD = 0x02; break;
        case Erase: IAP_CMD = 0x03; break;
    }
    triggerIAPOp();
    disableIAP();
}

/**
 * @brief Write IAP address
 *
 * @param addr address to write
 */
inline void writeIAPAddr(uint16_t addr)
{
    IAP_ADDRL = addr;
    IAP_ADDRH = addr >> 8;
}

/**
 * @brief Increase IAP address by 1
 *
 * @param addr address to write
 */
inline void incIAPAddr()
{
    if (IAP_ADDRL++ == 0xFF) IAP_ADDRH++;
}

/**
 * @brief Decrease IAP address by 1
 *
 * @param addr address to write
 */
inline void decIAPAddr()
{
    if (IAP_ADDRL-- == 0x00) IAP_ADDRH--;
}

/**
 * @brief Write data to specific address
 *
 * @param data data to write
 */
inline void writeToIAP(uint8_t data)
{
    IAP_DATA = data;
    doIAPOp(Write);
}

/**
 * @brief Read data from specific address
 *
 * @return uint8_t data of specific address
 */
inline uint8_t readFromIAP()
{
    doIAPOp(Read);
    return IAP_DATA;
}