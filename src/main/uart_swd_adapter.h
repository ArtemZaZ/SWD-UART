#pragma once

#include "base_swd_adapter.h"
#include "uart_handle.h"

namespace adapter
{
  // класс-адаптер между uart и swd
  class UartSwdAdapter : public BaseSwdAdapter
  {     
    public:
      UartSwdAdapter();
      UartSwdAdapter(swd::ISwdBus * swdBus, uart::Handle * uart);
    protected:
      uart::Handle * uart;
  };
  
};
