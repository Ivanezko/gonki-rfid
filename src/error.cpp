#include <Arduino.h>
#include "rfid.h"


String RFID::error_text(char c)
{
    byte cc = (byte)c;
    switch (cc)
    {
    case 0x00:
        return "OK";
        break;        
    case 0xFD:
        return "Incorrect frame length";
        break;
    case 0xF8:
        return "Antenna disconnected";
        break;
    case 0xF9:
        return "Command execution error";
        break;
    case 0x14:
        return "Fail to adjust the RF power";
        break;
    case 0xFF:
        return "Unrecognised parameters in a command frame";
        break;
    default:
        return "error unknown";
        break;
    }
}