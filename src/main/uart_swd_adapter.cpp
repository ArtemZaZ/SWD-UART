#include "uart_swd_adapter.h"

namespace adapter
{
  UartSwdAdapter::UartSwdAdapter() : BaseSwdAdapter()
  {
    
  }
  
  UartSwdAdapter::UartSwdAdapter(swd::ISwdBus * swdBus, uart::Handle * uart) : BaseSwdAdapter(swdBus)
  {    
    this->uart = uart;
  }
};
