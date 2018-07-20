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
    // ф-ия, которая возвращает следующий указатль на адаптер, из которого цепляется пакет
    adapter::BaseSwdAdapter* getNextPackageAdapter(void);
    // ф-ия, которая возвращает следующий указатль на адаптер, из которого цепляются данные
    adapter::BaseSwdAdapter* getNextDataAdapter(void);
  
  private:
    uint8_t m_packageAdapterCounter = 0;    // индекс текущего адаптера, с которого цепляются пакеты
    uint8_t m_dataAdapterCounter = 0;   // индекс текущего адаптера, с которого цепляются данные
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
    } m_state = READ_PACKAGE;
    
  };
};
