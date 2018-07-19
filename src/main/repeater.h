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
    adapter::BaseSwdAdapter m_adapters[10];
    // конечный автомат ретранслятора
    void fsm(void);
    // состояния конечного автомата
    enum 
    {
      // Читаем пакет из буффера пакетов текущего адаптера из кольца адаптеров,
      // ставим каретку кольца на слудующий адаптер 
      READ_PACKAGE,   
      WAIT,   // просто проведенное с пользой время
      // Читаем данные с физического интерфеса текущего адаптера из другого кольца адаптеров,
      // ставим каретку на следующий адаптер, запихиваем прочитанные данные в пакет
      READ_DATA,      
      SEND_TO_SWD   // отправляем пакет по SWD
    } m_state;
    
  };
};
