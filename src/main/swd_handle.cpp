#include "swd_handle.h"
#include "common_functions.h"

namespace swd
{
  // определение всего, что входит в класс SwdHandle
  SwdHandle::SwdHandle()
  {
    
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
  
  uint32_t SwdHandle::transferPackege(SwdPackage package)
  {
    return 1;
  }
  
    
}
