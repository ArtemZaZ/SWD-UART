#pragma once

#include "base_swd_adapter.h"

namespace repeater
{
  // класс-ретранслятор, раскидывает пакеты между кучей интерфейсов из массива base_swd_adapter
  class Repeater
  {
  public:
    Repeater();
    void appendAdapter(adapter::BaseSwdAdapter* adapter);   // добавляет адаптер в список адаптеров
    // конечный автомат ретранслятора
    void fsm(void);
  
  protected:
    // все адаптеры интерфейса SWD
    adapter::BaseSwdAdapter* m_adapters[10] = {0,};
    // количество прицепленных адаптеров
    uint8_t m_adapterNum = 0;    
    /* TODO: надо как-то названия переделать */
    // ф-ия, которая возвращает следующий указатль на адаптер, с которого цепляются пакеты для отправки по swd
    adapter::BaseSwdAdapter* getNextPackageToSwdAdapter(void);
    // ф-ия, которая возвращает следующий указатль на адаптер, с которого цепляются данные c физ интерфейса
    adapter::BaseSwdAdapter* getNextDataFromPhysicAdapter(void);
    // ф-ия, которая возвращает следующий указатль на адаптер, с которого отправляются пакеты по физ интерфейсу 
    adapter::BaseSwdAdapter* getNextPackageToPhysicalPhysicAdapter(void);
  
  private:
    uint8_t m_packageToSwdAdapterCounter = 0;     // индекс текущего адаптера, с которого цепляются пакеты для отправки по swd
    uint8_t m_dataFromPhysicAdapterCounter = 0;   // индекс текущего адаптера, с которого цепляются данные c физ интерфейса
    uint8_t m_packageToPhisicAdapterCounter = 0;  // индекс текущего адаптера, с которого отправляются пакеты по физ интерфейсу    
  };
};
