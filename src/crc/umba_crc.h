#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

uint8_t umba_crc8 (const uint8_t * buf, uint32_t len);
uint16_t umba_crc16 (const uint8_t * buf, uint32_t len);
uint32_t umba_crc32 (const uint8_t * buf, uint32_t len);

#ifdef __cplusplus
}
#endif
