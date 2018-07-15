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
    
  }
  
  inline uint8_t SwdHandle::readBit(void)
  {
    
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
  
  inline void SwdHandle::swclkToLow(void)
  {
    swclkPort->BRR = swclkPin;
  }
  
  inline void SwdHandle::swclkToHigh(void)
  {
    swclkPort->BSRR = swclkPin;
  }
    
  void SwdHandle::transferPackage(SwdPackage package)
  {
    
  }
  
    
}
