#pragma once
#include "STC/IAP/IAP.h"
#include "STC/UART/UART.h"

#define SavedDataAddr 0x4000

/**
 * @brief Last sent data buffer
 */
char sentData[27];

/**
 * @brief Save data to EEPROM
 *
 * @param data data to save
 */
void saveSentData(char* data)
{
    writeIAPAddr(SavedDataAddr);
    doIAPOp(Erase);
    char ch;
    while ((ch = *data++) != '\0') {
        writeToIAP(ch);
        incIAPAddr();
    }
    writeToIAP('\0');
}

/**
 * @brief Send data saved in errpom
 */
void sendSavedData()
{
    writeIAPAddr(SavedDataAddr);
    // Nothing saved
    if (readFromIAP() == 0xFF) return;
    char ch;
    while ((ch = readFromIAP()) != '\0') {
        sendData(Port1, ch);
        incIAPAddr();
    }
}

/**
 * @brief Send alpha char from 'a'/'A' to the specific end char
 *
 * @param endChar end char
 */
void sendAlpha(char endChar)
{
    uint8_t len;
    char    startChar;
    if (endChar <= 'z' && endChar >= 'a')
        startChar = 'a';
    else if (endChar <= 'Z' && endChar >= 'A')
        startChar = 'A';
    else
        return;

    // With one more \0 char
    len = endChar - (startChar - 1) + 1;
    for (char i = startChar; i <= endChar; i++) {
        sendData(Port1, i);
        sentData[i - startChar] = i;
    }
    sentData[len] = '\0';
    saveSentData(sentData);
}