#pragma once

#include "stdint.h"
#include "stm32f10x.h"
#include "common_functions.h"

#define SWD_TRANSFER_OK     (1<<0)    // дефайны для прием ack по swd
#define SWD_TRANSFER_WAIT   (1<<1)
#define SWD_TRANSFER_FAULT  (1<<2)

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
    SwdPackage() : Start(0),
                   APnDP(0),
                   RnW(0),
                   A(0),
                   Parity(0),
                   Stop(0),
                   Park(0),
                   ACK(0),
                   data(0),
                   dataParity(0) {;}
                     
    SwdPackage(volatile SwdPackage &package) : Start(package.Start),
                                               APnDP(package.APnDP),
                                               RnW(package.RnW),
                                               A(package.A),
                                               Parity(package.Parity),
                                               Stop(package.Stop),
                                               Park(package.Park),
                                               ACK(package.ACK),
                                               data(package.data),
                                               dataParity(package.dataParity) {;}
                                                 
    volatile SwdPackage& operator =(SwdPackage &package) volatile
    {
      Start = package.Start;
      APnDP = package.APnDP;
      RnW = package.RnW;
      A = package.A;
      Parity = package.Parity;
      Stop = package.Stop;
      Park = package.Park;
      ACK = package.ACK;
      data = package.data;
      dataParity = package.dataParity;
      return package;
    }
    
    // упаковщик, про параметры подробнее в i_swd_bus.h и reference manual->SW debug port(стр. 1089)
    void pack(uint8_t p_APnDP, uint8_t p_RnW,
              uint8_t p_A, uint32_t p_data=0)
    {
      Start = 1;
      APnDP = p_APnDP & 1;
      RnW = p_RnW & 1;
      A = p_A & 0x3;
      Park = 1;
      Stop = 0;
      ACK = 0;
      Parity = (Start + (APnDP & 0x2) + (APnDP & 1) + RnW + A) & 1;
      data = p_data;
      dataParity = common_functions::countSetBits(p_data);
    } 
    
    void pack(uint8_t p_request, uint32_t p_data=0)
    {
      Start = 1;
      APnDP = (p_request & 0x40) >> 6;
      RnW = (p_request & 0x20) >> 5;
      A = (p_request & 0x18) >> 3;
      Parity = (p_request & 0x4) >> 2;
      Park = 1;
      Stop = 0;
      ACK = 0;
      data = p_data;
      dataParity = common_functions::countSetBits(p_data);
    }

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

/* структура настройки swd устройства */
namespace swd
{
  // некоторое из этого пока хз что значит, но оно иногда юзается, или нет) 
  // как я понял какая-то часть из этого прилетает от драйвера Keil'a
  struct SwdDevSettings
  {
    uint8_t   debagPort;  
    uint32_t  clockDelay;   // задержка между высоки и низким уровнем тактирования
    uint8_t   idleCycles;   // количество пустых циклов после передачи
    uint16_t  retryCount;
    uint16_t  matchRetry;
    uint32_t  matchMask;
    uint8_t   turnaroid;    // задержка для обработки данных
    uint8_t   dataPhase;
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
      virtual void transferPackage(SwdPackage * package) = 0;   // пересылает пакет туда или обратно) 
      //virtual SwdPackage pack(uint8_t APnDP, uint8_t RnW,
      //                        uint8_t A, uint32_t data=0) = 0;
      virtual inline void writeBit(uint8_t bit) = 0;  // отправка бита по swd
      virtual inline uint8_t readBit(void) = 0;   // чтение бита с swd      
  };
};


