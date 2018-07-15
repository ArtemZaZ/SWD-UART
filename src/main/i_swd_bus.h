#ifndef SWD_H_
#define SWD_H_
#include "stdint.h"
#include "stm32f10x.h"
// Исследование протокола SWD в рамках практики
/* 
1) От хоста(в данном случае программатора) поступает 8ми битный packet request, состоящий из:
      0 : Start - Должно быть “1”
      1 : APnDP - 0: DP Access, 1: AP Access
      2 : RnW   - 0: Write Request, 1: Read Request 
      [4:3] : A[3:2]  - Address field of the DP or AP registers
      5 : Parity  - Single bit parity of preceding bits
      6 : Stop  - Должно быть "0"
      7 : Park  -  Not driven by the host. Must be read as “1” by the target because of the pull-up

2) От целевого устройства(отлаживаемой платы) поступает 3х битное подтверждение, состоящее из:
      0..2 : ACK - 001: FAULT, 010: WAIT, 100: OK

3) Пересылается 33 бита данных, от хоста или целевого устройства, смотря что было выбрано в запросе.
      0..31 : WDATA or RDATA  - Write or Read data 
      32 : Parity - Single parity of the 32 data bits
*/

// структура пакета SWD
/*  packetRequest data bits:
  0 : Start - Должно быть “1”
  1 : APnDP - 0: DP Access, 1: AP Access
  2 : RnW   - 0: Write Request, 1: Read Request 
  [4:3] : A[3:2]  - Address field of the DP or AP registers
  5 : Parity  - Single bit parity of preceding bits
  6 : Stop  - Должно быть "0"
  7 : Park  -  Not driven by the host. Must be read as “1” by the target because of the pull-up
*/
/*
  0..2 : ACK - 001: FAULT, 010: WAIT, 100: OK
*/
/*
  0..31 : WDATA or RDATA  - Write or Read data 
  32 : Parity - Single parity of the 32 data bits
*/
namespace swd
{
  struct SwdPackage
  {
    unsigned Start  : 1;
    unsigned APnDP  : 1;
    unsigned RnW    : 1;
    unsigned A      : 2;
    unsigned Parity : 1;
    unsigned Stop   : 1;
    unsigned Park   : 1;    
    unsigned ACK    : 3;    
    uint32_t data   : 32;      
    unsigned dataParity : 1;
  };
};

namespace swd
{
  // Интерфейс шины SWD
  class ISwdBus
  {
    public:
      virtual ~ISwdBus() {};   // деструктор
      virtual void init(GPIO_TypeDef * swdioPort, uint8_t swdioPinNumber,     // SWDIO
                            GPIO_TypeDef * swclkPort, uint8_t swclkPinNumber,    // SWCLK
                            GPIO_TypeDef * nResetPort, uint8_t nResetPinNumber) = 0;  // nRESET
      virtual void transferPackage(SwdPackage package) = 0;   // пересылает пакет туда или обратно) 
    
      virtual inline void writeBit(uint8_t bit) = 0;  // отправка бита по swd
      virtual inline uint8_t readBit(void) = 0;   // чтение бита с swd      
  };
};

#endif /* SWD_H_ */
