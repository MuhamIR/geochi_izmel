#ifndef _CRC_H_
#define _CRC_H_

#include "TypeDef.h"

U8 Crc8(const U8 *pcBlock, UI len , U8 init_val, U8 pol);
U8 Crc8(const U8 *pcBlock, UI len);
U16 Crc16(const U8 *pcBlock, UI len);
U16 Crc16(const U16 *pcBlock, UI len, U16 crcStart = 0);

UI calcCRC16 (UI dataCRC16, U8  byte);

#endif

