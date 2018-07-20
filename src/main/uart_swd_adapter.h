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
    uart::Handle * m_uart = 0;
  
  private:
    uint8_t getPackage(swd::SwdPackage * package); // читаем входной буффер и вытаскиваем оттуда сообщение, возвращает кол-во прочитанных пакетов 
  };
  
};
