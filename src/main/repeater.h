#pragma once

#include "base_swd_adapter.h"

namespace repeater
{
  // класс-ретранслятор, раскидывает пакеты между кучей интерфейсов из массива base_swd_adapter
  class Repeater
  {
  public:
    Repeater();
  
  protected:
    // все адаптеры интерфейса SWD
    adapter::BaseSwdAdapter adapters[10];
  };
};
