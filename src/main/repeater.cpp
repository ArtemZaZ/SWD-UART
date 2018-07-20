#include "repeater.h"

namespace repeater
{
  Repeater::Repeater() 
  {
    
  }
  
  void Repeater::appendAdapter(adapter::BaseSwdAdapter* adapter)
  {
    m_adapters[m_adapterNum] = adapter;
    m_adapterNum++;
  }
  
  adapter::BaseSwdAdapter* Repeater::getNextPackageAdapter(void)
  {
    if(m_packageAdapterCounter > m_adapterNum)  m_packageAdapterCounter = 0;
    return m_adapters[m_packageAdapterCounter++];
  }
  
  adapter::BaseSwdAdapter* Repeater::getNextDataAdapter(void)
  {
    if(m_dataAdapterCounter > m_adapterNum)  m_dataAdapterCounter = 0;
    return m_adapters[m_dataAdapterCounter++];
  }
  
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
