#include "i_swd_bus.h"

namespace swd
{
  class SwdHandle : public ISwdBus 
  {
    public:
      SwdHandle();  // конструктор
      virtual void init(GPIO_TypeDef * swdioPort, uint8_t swdioPinNumber,     // SWDIO
                            GPIO_TypeDef * swclkPort, uint8_t swclkPinNumber,    // SWCLK
                            GPIO_TypeDef * nResetPort, uint8_t nResetPinNumber);  // nRESET
      
      virtual SwdPackage pack(uint8_t APnDP, uint8_t RnW,
                              uint8_t A, uint8_t Park,
                              uint8_t ACK=0, uint32_t data=0);  // упаковщик
      
      virtual void transferPackage(SwdPackage * package);       
    
    protected:
      // порты
      GPIO_TypeDef * swdioPort;
      GPIO_TypeDef * swclkPort;
      GPIO_TypeDef * nResetPort;
      // номера пинов
      uint8_t swdioPinNumber;
      uint8_t swclkPinNumber;
      uint8_t nResetPinNumber;
      // пины
      uint16_t swdioPin;
      uint16_t swclkPin;
      uint16_t nResetPin;
    
      int32_t clockDelay;
      
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
      inline void pinDelay(int32_t delay);
  };  
}
