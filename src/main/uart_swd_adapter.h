#pragma once

#include "base_swd_adapter.h"
#include "uart_handle.h"
#define UART_SWD_MSG_LEN  12U  // длина сообщения, приходящего по UART // TODO: какнибудь переделать эту дичь
#define UART_START_MSG_BYTE   0xAAU    // стартовый байт
#define UART_STOP_MSG_BYTE  0xBBU      // стоповый байт

namespace adapter
{
  // класс-адаптер между uart и swd
  /* чтение из буффера уарта не должно надолго задерживать работу процессора, т.к. ждать пока пакет целиком
     прилетит - плохая идея(он может вообще не прилететь). Вместо этого используется еще один буффер с ограниченной
     длиной, равной длине пакета(+-). Когда вызывается ф-ия чтения из буффера уарта, если он не пуст, то ищется 
     вхождение в пакет, т.е. его стартовые биты. Когда вхождение произошло, ставится флаг, начинается 
     запись сообщения в буффер данного класса. Если пакет переполняет буффер - он откидывается, возникает ошибка.
     Если нет, то он парсится и отправляется дальше + чтение из буффера уарта прекращается до следующего вызова.
     Если пакет не пришел полностью, а буффер уарта пуст, то чтение прекращается до следующего вызова
  */
  class UartSwdAdapter : public BaseSwdAdapter
  {     
  public:
    UartSwdAdapter();
    UartSwdAdapter(swd::ISwdBus * swdBus, uart::Handle * uart);
    virtual void sendPackageToSwd();      // отправка пакета(ов) по SWD, обязателен к определению
    virtual void sendPackageToPhysic();   // отправка пакета(ов) по физ интерфейсу, обязателен к определению
    virtual void readFromPhysic();  
  protected:
    uart::Handle * m_uart = nullptr;
  
  private:
    uint8_t m_messageBuffer[UART_SWD_MSG_LEN] = {0, };  // локальный буффер для парсинга сообщений 
    uint8_t m_messageBufferCounter = 0;   // каретка локального буффера
    bool m_enterToMessageFlag = false;    // флаг вхождения в буффер
    void readUart(void);
    //uint8_t getPackageFromUart(swd::SwdPackage * package); // читаем входной буффер и вытаскиваем оттуда сообщение, возвращает кол-во прочитанных пакетов 
  };
  
};
