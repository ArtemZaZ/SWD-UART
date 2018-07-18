#pragma once

#include "i_swd_bus.h"
#include "blocking_circular_buffer.h"

namespace adapter
{
  // базовый класс-адаптер между swd и физ интерфейсом, тут будет записаны некоторые общие поля и некоторые обязательные виртуальные методы
  class BaseSwdAdapter
  {
    public:
      BaseSwdAdapter();
      BaseSwdAdapter(swd::ISwdBus * swdBus);
    protected:
      swd::ISwdBus * swdBus;    // шина swd
      BlockingCircularBuffer< swd::SwdPackage, 255 > SwdPackageIn;  // пакеты прилетающие по физ интерфейсу, но еще не отправленные по swd
      BlockingCircularBuffer< swd::SwdPackage, 255 > SwdPackageOut; // пакеты прилетающие по swd, но еще не отправленные по физ. интерфейсу
  };
  
};
