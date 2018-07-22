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
    virtual bool isSwdPackagesToSwdEmpty();   // проверка пуст ли буффер пакетов, прилетающих с уарта 
    virtual bool isSwdPackagesToPhysicEmpty();  // проверка пуст ли буффер пакетов, прилетающих с SWD
    virtual void sendPackageToSwd() = 0;      // отправка пакета(ов) по SWD, обязателен к определению
    virtual void sendPackageToPhysic() = 0;   // отправка пакета(ов) по физ интерфейсу, обязателен к определению
    virtual void readFromPhysic() = 0;        // чтение данных с физ интерфейса
  
  protected:
    swd::ISwdBus * m_swdBus = nullptr;    // шина swd
    BlockingCircularBuffer<swd::SwdPackage, 255> m_swdPackagesToSwd;  // пакеты прилетающие по физ интерфейсу, но еще не отправленные по swd
    BlockingCircularBuffer<swd::SwdPackage, 255> m_swdPackagesToPhysic; // пакеты прилетающие по swd, но еще не отправленные по физ. интерфейсу
  };
  
};
