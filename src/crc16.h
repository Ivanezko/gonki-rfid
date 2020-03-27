#include <Arduino.h>

#define CRC_PRESET_VALUE 0xFFFF
#define CRC_POLINOMIAL 0x8408

class CRC16 {
    private:
        unsigned short last_result;
        unsigned char get_byte(int num);
        void uiCrc16Cal(unsigned char  *pucY, unsigned char ucX);
    public:
        CRC16();
        void sign(unsigned char *arr, int arrlen);
        bool check(unsigned char *arr, int arrlen);
};