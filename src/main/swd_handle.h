#pragma once

#include "i_swd_bus.h"

namespace swd
{
  class SwdHandle : public ISwdBus 
  {
    public:
      SwdHandle();  // конструктор
      SwdHandle(SwdDevSettings settings);
      virtual void init(GPIO_TypeDef * swdioPort, uint8_t swdioPinNumber,     // SWDIO
                            GPIO_TypeDef * swclkPort, uint8_t swclkPinNumber,    // SWCLK
                            GPIO_TypeDef * nResetPort, uint8_t nResetPinNumber);  // nRESET
      
      //virtual SwdPackage pack(uint8_t APnDP, uint8_t RnW,
      //                        uint8_t A, uint32_t data=0);  
      
      virtual void transferPackage(SwdPackage * package);       
    
    protected:
      // настройка swd и т.д.
      SwdDevSettings m_settings = {0, };
      // порты
      GPIO_TypeDef * m_swdioPort = nullptr;
      GPIO_TypeDef * m_swclkPort = nullptr;
      GPIO_TypeDef * m_nResetPort = nullptr;
      // номера пинов
      uint8_t m_swdioPinNumber = 0;
      uint8_t m_swclkPinNumber = 0;
      uint8_t m_nResetPinNumber = 0;
      // пины
      uint16_t m_swdioPin = 0;
      uint16_t m_swclkPin = 0;
      uint16_t m_nResetPin = 0;
        
      virtual inline void writeBit(uint8_t bit);
      virtual inline uint8_t readBit(void);

    private:
      inline void swdioOutEnable(void);   // настройка пина swdio на выход
      inline void swdioOutDisable(void);  // настройка пина swdio на вход
      inline void swdioOut(uint8_t bit);  // выставляет уровень на swdio соответственно bit
      inline uint8_t swdioIn(void);       // считывает уровень на входе swdio
      inline void swclkToLow(void);       // прижимаем swclk в ноль
      inline void swclkToHigh(void);      // swclk на высокий уровень
      inline void swclkCycle(void);       // цикл тактирования
      inline void pinDelay(uint32_t delay);
      inline void turnaroid(uint8_t turnaroid); // задержка после отправки
      inline void idleCycles(uint8_t idleCycles);   // пусые циклы swclk
  };  
}
