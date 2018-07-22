#include "uart_swd_adapter.h"

namespace adapter
{
  UartSwdAdapter::UartSwdAdapter() : BaseSwdAdapter()
  {
    
  }
  
  UartSwdAdapter::UartSwdAdapter(swd::ISwdBus * swdBus, uart::Handle * uart) : BaseSwdAdapter(swdBus)
  {    
    m_uart = uart;
  }
  
  /*uint8_t UartSwdAdapter::getPackageFromUart(swd::SwdPackage * package)
  {
    
  }
  */
  
  void UartSwdAdapter::readUart(void)
  {
    uint8_t tempByte = 0; // байт для чтения из буффера
    
    // если пришел новый байт и нет флага вхождения в сообщение
    while(m_uart->isNewByte() && !m_enterToMessageFlag) 
    {
      tempByte = m_uart->getByte();      
      // если пришедший байт - стартовый и при этом предыдущее значение - тоже стартовый байт
      if((tempByte == UART_START_MSG_BYTE) && (m_messageBuffer[m_messageBufferCounter] == UART_START_MSG_BYTE))   
      {
        m_enterToMessageFlag = true;  // ставим что произошло вхождение в сообщение 
        m_messageBufferCounter++; // передвигаем каретку
        m_messageBuffer[m_messageBufferCounter++] = tempByte; // записываем и передвигаем каретку еще раз
        break; 
      }
      // записываем пришедшие данный в первыый байт локального буффера, пока не будет ввхождения
      m_messageBuffer[m_messageBufferCounter] = tempByte;   
    }
    
    if(!m_enterToMessageFlag) return;   // если все-таки не было вхождения, а буффер уарта уже пуст, то заканчиваем
    
    // читаем, пока не пуст буффер уарта или не заполнен локальный буффер
    while(m_uart->isNewByte() && (m_messageBufferCounter < UART_SWD_MSG_LEN))
    {
      tempByte = m_uart->getByte();
      // если встретились стоповые байты 
      if((tempByte == UART_STOP_MSG_BYTE) && (m_messageBuffer[m_messageBufferCounter - 1] == UART_STOP_MSG_BYTE))  
      {
        m_enterToMessageFlag = false;   // убираем метку вхождения в сообщение
        m_messageBuffer[m_messageBufferCounter++] = tempByte;  
        if(m_messageBufferCounter < UART_SWD_MSG_LEN)   // если сообщние меньше, чем должно быть
        {
          /* TODO: сделать ошибку */
        }
        m_messageBufferCounter = 0;
        return;        
      }
      m_messageBuffer[m_messageBufferCounter++] = tempByte;  // записываем байт в локальный буффер
    }
    
    // если буффер переполнился, а выхода из сообщения так и не было
    if(m_enterToMessageFlag && m_messageBufferCounter >= UART_SWD_MSG_LEN)
    {
      m_enterToMessageFlag = false;
      m_messageBufferCounter = 0;
      /* TODO: сделать ошибку */
      return;
    }        
  }
  
  void UartSwdAdapter::sendPackageToSwd()
  {
    
  }
  
  void UartSwdAdapter::sendPackageToPhysic()
  {
    
  }
  
  void UartSwdAdapter::readFromPhysic()
  {
    
  }
  
};
