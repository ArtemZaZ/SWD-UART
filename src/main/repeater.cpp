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
  
  adapter::BaseSwdAdapter * Repeater::getNextPackageToSwdAdapter(void)
  {
    if(m_packageToSwdAdapterCounter >= m_adapterNum)  m_packageToSwdAdapterCounter = 0;
    return m_adapters[m_packageToSwdAdapterCounter++];
  }
  
  adapter::BaseSwdAdapter * Repeater::getNextDataFromPhysicAdapter(void)
  {
    if(m_dataFromPhysicAdapterCounter >= m_adapterNum)  m_dataFromPhysicAdapterCounter = 0;
    return m_adapters[m_dataFromPhysicAdapterCounter++];
  }
  
  adapter::BaseSwdAdapter * Repeater::getNextPackageToPhysicalPhysicAdapter(void)
  {
    if(m_packageToPhisicAdapterCounter >= m_adapterNum)  m_packageToPhisicAdapterCounter = 0;
    return m_adapters[m_packageToPhisicAdapterCounter++];
  }
  
  void Repeater::fsm(void)
  {
    // состояния конечного автомата
    static enum  /* TODO: переделать под строгий enum */
    {
      // Читаем пакет из буффера пакетов текущего адаптера из кольца адаптеров,
      // ставим каретку кольца на слудующий адаптер 
      READ_PACKAGE,   
      // Читаем данные с физического интерфеса текущего адаптера из другого кольца адаптеров,
      // ставим каретку на следующий адаптер, запихиваем прочитанные данные в пакет
      READ_DATA,      
      SEND_TO_SWD,   // отправляем пакет по SWD
      SEND_TO_PHYSIC  // отправка пакета по физ интерфейсу
    } state = READ_PACKAGE;
    
    static adapter::BaseSwdAdapter * packageToSwdAdapterPointer = nullptr;    // указатель на адаптер...
    static adapter::BaseSwdAdapter * dataFromPhysicAdapterPointer = nullptr;  // указатель на адаптер...
    static adapter::BaseSwdAdapter * packageToPhisicAdapterPointer = nullptr; // указатель на адаптер...
    
    switch(state)
    {
      case READ_PACKAGE:
        packageToSwdAdapterPointer = getNextPackageToSwdAdapter();
        if(packageToSwdAdapterPointer->isSwdPackagesToSwdEmpty())   // если буффер адаптера пуст
        {
          state = READ_DATA;
          return;
        }        
        state = SEND_TO_SWD;
        return;
      
      case READ_DATA:
        dataFromPhysicAdapterPointer = getNextDataFromPhysicAdapter();
        dataFromPhysicAdapterPointer->readFromPhysic();
        state = SEND_TO_PHYSIC;
        return;
      
      case SEND_TO_SWD:
        // тут отправляем пакет по SWD
        packageToSwdAdapterPointer->transferPackageToSwd();
        state = SEND_TO_PHYSIC;
        return;
      
      case SEND_TO_PHYSIC:
        // тут отправляем пакет по физ интерфейсу
        packageToPhisicAdapterPointer = getNextPackageToPhysicalPhysicAdapter();
        // пропускаем все пустые адаптеры, пока не найдем 1 пакет или адаптеры закончатся
        while(packageToPhisicAdapterPointer->isSwdPackagesToPhysicEmpty())
        {
          if(m_packageToPhisicAdapterCounter <= m_adapterNum)  // закнчились адаптеры
          {
            state = READ_PACKAGE;
            return;
          }
          packageToPhisicAdapterPointer = getNextPackageToPhysicalPhysicAdapter();          
        }
        packageToPhisicAdapterPointer->sendPackageToPhysic();
        state = READ_PACKAGE;
        return;
    }
  }
};
