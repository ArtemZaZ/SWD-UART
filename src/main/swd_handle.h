#include "i_swd_bus.h"

namespace swd
{
  class SwdHandle : public ISwdBus 
  {
    public:
      SwdHandle();  // конструктор
      virtual SwdPackage pack(uint8_t APnDP, uint8_t RnW,
                              uint8_t A, uint8_t Park,
                              uint8_t ACK=0, uint32_t data=0);  // упаковщик
      virtual uint32_t transferPackege(SwdPackage package);       
  };  
}
