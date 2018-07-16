#include "swd_handle.h"
#include "common_functions.h"

namespace swd
{
  // определение всего, что входит в класс SwdHandle
  SwdHandle::SwdHandle()
  {
    
  }  
  
  void SwdHandle::init(GPIO_TypeDef * swdioPort, uint8_t swdioPinNumber,     // SWDIO
                            GPIO_TypeDef * swclkPort, uint8_t swclkPinNumber,    // SWCLK
                            GPIO_TypeDef * nResetPort, uint8_t nResetPinNumber)   // nRESET
  {
    swdioPin = (uint16_t)(1 << swdioPinNumber);
    swclkPin = (uint16_t)(1 << swclkPinNumber);
    nResetPin = (uint16_t)(1 << nResetPinNumber);
    this->swdioPort = swdioPort;
    this->swclkPort = swclkPort;
    this->nResetPort = nResetPort;
    this->swdioPinNumber = swdioPinNumber;
    this->swclkPinNumber = swclkPinNumber;
    this->nResetPinNumber = nResetPinNumber;
    
  }
  
  SwdPackage SwdHandle::pack(uint8_t APnDP, uint8_t RnW,
                  uint8_t A, uint8_t Park, uint8_t ACK,
                  uint32_t data)
  {
    SwdPackage package;
    package.Start = 1;
    package.APnDP = APnDP & 1;
    package.RnW = RnW & 1;
    package.A = A & 0x3;
    package.Park = Park & 1;
    package.Stop = 0;
    package.ACK = ACK & 0x7;
    package.Parity = (package.Start + (package.APnDP & 0x2) + (package.APnDP & 1) + package.RnW + package.A) & 1;
    package.data = data;
    package.dataParity = common_functions::countSetBits(data);
    return package;
  }
  
  inline void SwdHandle::writeBit(uint8_t bit)
  {
    swdioOut(bit);
    swclkToLow();
    pinDelay(clockDelay);
    swclkToHigh();
    pinDelay(clockDelay);
  }
  
  inline uint8_t SwdHandle::readBit(void)
  {
    swclkToLow();
    pinDelay(clockDelay);
    uint8_t bit = swdioIn();
    swclkToHigh();
    pinDelay(clockDelay);
    return bit;
  }
  
  inline void SwdHandle::swdioOutEnable(void)  
  {
    if(swdioPin < 8)
      swdioPort->CRL = (swdioPort->CRL & ~(0xF << (4*swdioPinNumber))) | (0x03 << (4*swdioPinNumber)); 
    else
      swdioPort->CRH = (swdioPort->CRH & ~(0xF << (4*(swdioPinNumber - 8)))) | (0x03 << (4*(swdioPinNumber - 8))); 
  }

  inline void SwdHandle::swdioOutDisable(void)
  {
    if(swdioPin < 8)
      swdioPort->CRL = (swdioPort->CRL & ~(0xF << (4*swdioPinNumber))) | (0x08 << (4*swdioPinNumber));
    else
      swdioPort->CRH = (swdioPort->CRH & ~(0xF << (4*(swdioPinNumber - 8)))) | (0x08 << (4*(swdioPinNumber - 8)));
  }
  
  inline void SwdHandle::swdioOut(uint8_t bit)
  {
    if(bit & 1)
      swdioPort->BSRR = swdioPin;
    else
      swdioPort->BRR = swdioPin;
  }
  
  inline uint8_t SwdHandle::swdioIn(void)
  {
    return (swdioPort->IDR & swdioPin) ? 1 : 0;
  }
  
  inline void SwdHandle::swclkToLow(void)
  {
    swclkPort->BRR = swclkPin;
  }
  
  inline void SwdHandle::swclkToHigh(void)
  {
    swclkPort->BSRR = swclkPin;
  }
  
  inline void SwdHandle::swclkCycle(void)
  {
    swclkToLow();
    pinDelay(clockDelay);
    swclkToHigh();
    pinDelay(clockDelay);
  }
  
  void SwdHandle::transferPackage(SwdPackage * package)
  {
    // отправка request
    writeBit(package->Start);
    writeBit(package->APnDP);
    writeBit(package->RnW);
    writeBit(package->A >> 1); // старший бит
    writeBit(package->A);  // младший бит
    writeBit(package->Parity);
    writeBit(package->Stop);
    writeBit(package->Park);
    
    /*
    тут должно быть что-то типо задержки для того, чтобы дать время на обработку
    */
    
    package->ACK =  readBit();    // читаем ack побитово
    package->ACK |= readBit() << 1;
    package->ACK |= readBit() << 2;
    
    if(package->ACK == SWD_TRANSFER_OK)
    {
      
    }
    if((package->ACK == SWD_TRANSFER_WAIT) || (package->ACK == SWD_TRANSFER_FAULT))
    {
      
    }
    
  }
  
  inline void SwdHandle::pinDelay(int32_t delay)
  {
    volatile int32_t count;
    count = delay;
    while(--count);
  }    
}
