#include "repeater.h"

namespace repeater
{
  void Repeater::fsm(void)
  {
    switch(m_state)
    {
      case READ_PACKAGE:
        if(0)   // если буффер адаптера пуст
        {
          m_state = WAIT;          
        }
        // делаем все что нужно
        m_state = SEND_TO_SWD;
        break;
      
      case READ_DATA:
        // тут читаем сообщение с физ интерфейса
        m_state = READ_PACKAGE;
        break;
      
      case WAIT:
        m_state = READ_DATA;
        break;
      
      case SEND_TO_SWD:
        // тут отправляем пакет по SWD
      
        m_state = READ_PACKAGE;
        break;
    }
  }
};
