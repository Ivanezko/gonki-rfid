#include <Arduino.h>

#define CRC_PRESET_VALUE 0xFFFF
#define CRC_POLINOMIAL 0x8408

class CRC16
{
private:
    static unsigned char get_byte(int num);
    static void uiCrc16Cal(unsigned char *pucY, unsigned char ucX);

public:
    static unsigned short last_result;
    static void sign(unsigned char *arr, int arrlen);
    static bool check(unsigned char *arr, int arrlen);
};