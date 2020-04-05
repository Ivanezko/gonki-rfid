#include "crc16.h"

unsigned short CRC16::last_result = 0;

unsigned char CRC16::get_byte(int num)
{
  if (num == 0)
    return CRC16::last_result & 0xFF;
  else
    return (CRC16::last_result >> 8) & 0xFF;
}

void CRC16::uiCrc16Cal(unsigned char *pucY, unsigned char ucX)
{
  unsigned char ucI, ucJ;
  unsigned short int uiCrcValue = CRC_PRESET_VALUE;
  for (ucI = 0; ucI < ucX; ucI++)
  {
    uiCrcValue = uiCrcValue ^ *(pucY + ucI);
    for (ucJ = 0; ucJ < 8; ucJ++)
    {
      if (uiCrcValue & 0x0001)
      {
        uiCrcValue = (uiCrcValue >> 1) ^ CRC_POLINOMIAL;
      }
      else
      {
        uiCrcValue = (uiCrcValue >> 1);
      }
    }
  }
  CRC16::last_result = uiCrcValue;
}

void CRC16::sign(unsigned char *arr, int arrlen)
{
  CRC16::uiCrc16Cal(arr, arrlen);
  arr[arrlen] = get_byte(0);
  arr[arrlen + 1] = get_byte(1);
}

bool CRC16::check(unsigned char *arr, int arrlen)
{
  CRC16::uiCrc16Cal(arr, arrlen - 2);
  unsigned char byte_low = get_byte(0);
  unsigned char byte_high = get_byte(1);
  if (byte_low == arr[arrlen - 2] && byte_high == arr[arrlen - 1])
    return true;
  return false;
}
