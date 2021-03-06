#include "swd_handle.h"
#include "stm32f10x_gpio.h"

namespace swd
{
  // определение всего, что входит в класс SwdHandle
  SwdHandle::SwdHandle()
  {
    // default settings
    m_settings.debagPort = 0;
    m_settings.clockDelay = 2;
    m_settings.retryCount = 100;
    m_settings.idleCycles = 0;
    m_settings.matchRetry = 0;
    m_settings.matchMask = 0x00;
    m_settings.dataPhase = 0;
    m_settings.turnaroid = 1;
  }

  SwdHandle::SwdHandle(SwdDevSettings settings) : m_settings(settings)
  {
    
  }
  
  void SwdHandle::init(GPIO_TypeDef * swdioPort, uint8_t swdioPinNumber,     // SWDIO
                            GPIO_TypeDef * swclkPort, uint8_t swclkPinNumber,    // SWCLK
                            GPIO_TypeDef * nResetPort, uint8_t nResetPinNumber)   // nRESET
  {
    m_swdioPin = (uint16_t)(1 << swdioPinNumber);
    m_swclkPin = (uint16_t)(1 << swclkPinNumber);
    m_nResetPin = (uint16_t)(1 << nResetPinNumber);
    m_swdioPort = swdioPort;
    m_swclkPort = swclkPort;
    m_nResetPort = nResetPort;
    m_swdioPinNumber = swdioPinNumber;
    m_swclkPinNumber = swclkPinNumber;
    m_nResetPinNumber = nResetPinNumber;
    
    /* инициализация переферии, мб придется делать на HAL */
    RCC->APB2ENR |= gpioToRcc(m_swdioPort);
    RCC->APB2ENR |= gpioToRcc(m_swclkPort);
    RCC->APB2ENR |= gpioToRcc(m_nResetPort);
    
    m_swdioPort->BSRR = m_swdioPin;
    m_swclkPort->BSRR = m_swclkPin;
    m_nResetPort->BRR = m_nResetPin;
    
    GPIO_InitTypeDef swdioInit;
    swdioInit.GPIO_Pin = m_swdioPin;
    swdioInit.GPIO_Mode = GPIO_Mode_Out_PP;
    swdioInit.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(m_swdioPort, &swdioInit);
    
    GPIO_InitTypeDef swclkInit;
    swclkInit.GPIO_Pin = m_swclkPin;
    swclkInit.GPIO_Mode = GPIO_Mode_Out_PP;
    swclkInit.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(m_swclkPort, &swclkInit);    
    /* nReset пока не трогаю */
  }
  
  /*
  SwdPackage SwdHandle::pack(uint8_t APnDP, uint8_t RnW,
                  uint8_t A, uint32_t data)
  {
    static SwdPackage package;
    package.Start = 1;
    package.APnDP = APnDP & 1;
    package.RnW = RnW & 1;
    package.A = A & 0x3;
    package.Park = 1;
    package.Stop = 0;
    package.ACK = 0;
    package.Parity = (package.Start + (package.APnDP & 0x2) + (package.APnDP & 1) + package.RnW + package.A) & 1;
    package.data = data;
    package.dataParity = common_functions::countSetBits(data);
    return package;
  }
  */
  
  inline void SwdHandle::writeBit(uint8_t bit)
  {
    swdioOut(bit);
    swclkToLow();
    pinDelay(m_settings.clockDelay);
    swclkToHigh();
    pinDelay(m_settings.clockDelay);
  }
  
  inline uint8_t SwdHandle::readBit(void)
  {
    swclkToLow();
    pinDelay(m_settings.clockDelay);
    uint8_t bit = swdioIn();
    swclkToHigh();
    pinDelay(m_settings.clockDelay);
    return bit;
  }
  
  inline void SwdHandle::swdioOutEnable(void)  
  {
    if(m_swdioPinNumber < 8)
      m_swdioPort->CRL = (m_swdioPort->CRL & ~(0xF << (4*m_swdioPinNumber))) | (0x03 << (4*m_swdioPinNumber)); 
    else
      m_swdioPort->CRH = (m_swdioPort->CRH & ~(0xF << (4*(m_swdioPinNumber - 8)))) | (0x03 << (4*(m_swdioPinNumber - 8))); 
  }

  inline void SwdHandle::swdioOutDisable(void)
  {
    if(m_swdioPinNumber < 8)
      m_swdioPort->CRL = (m_swdioPort->CRL & ~(0xF << (4*m_swdioPinNumber))) | (0x08 << (4*m_swdioPinNumber));
    else
      m_swdioPort->CRH = (m_swdioPort->CRH & ~(0xF << (4*(m_swdioPinNumber - 8)))) | (0x08 << (4*(m_swdioPinNumber - 8)));
  }
  
  inline void SwdHandle::swdioOut(uint8_t bit)
  {
    if(bit & 1)
      m_swdioPort->BSRR = m_swdioPin;
    else
      m_swdioPort->BRR = m_swdioPin;
  }
  
  inline uint8_t SwdHandle::swdioIn(void)
  {
    return (m_swdioPort->IDR & m_swdioPin) ? 1 : 0;
  }
  
  inline void SwdHandle::swclkToLow(void)
  {
    m_swclkPort->BRR = m_swclkPin;
  }
  
  inline void SwdHandle::swclkToHigh(void)
  {
    m_swclkPort->BSRR = m_swclkPin;
  }
  
  inline void SwdHandle::swclkCycle(void)
  {
    swclkToLow();
    pinDelay(m_settings.clockDelay);
    swclkToHigh();
    pinDelay(m_settings.clockDelay);
  }
  
  void SwdHandle::transferPackage(SwdPackage * package)
  {
    __disable_irq();
    // отправка request
    swdioOutEnable();   // будем писать
    writeBit(package->Start);
    writeBit(package->APnDP);
    writeBit(package->RnW);
    writeBit(package->A >> 1); // старший бит
    writeBit(package->A);  // младший бит
    writeBit(package->Parity);
    writeBit(package->Stop);
    writeBit(package->Park);
    swdioOutDisable();  // будем читать
    
    // тут должно быть что-то типо задержки для того, чтобы дать время на обработку
    turnaroid(m_settings.turnaroid);
    
    package->ACK =  readBit();    // читаем ack побитово
    package->ACK |= readBit() << 1;
    package->ACK |= readBit() << 2;
    
    if(package->ACK == SWD_TRANSFER_OK)   // можно общаться
    {
      if(package->RnW)  // если взапросе чтение данных
      {
        uint32_t data = 0;  // сюда запсываются прочитанные данные
        uint8_t parity = 0; // а тут считаем четность
        uint8_t bit = 0;    // временная переменная для храненияя значения бита
        for(uint8_t n = 31; n + 1; n--)   // 32 прохода
        {
          bit = readBit();
          data |= (bit << n);
          parity += bit;          
        }
        bit = readBit(); // читаем четность
        if((parity ^ bit) & 1)
        {
          /*
            TODO: ошибка чтения тут
          */
        }
        package->data = data;                
        // тут опять задержка на обработку
        turnaroid(m_settings.turnaroid);        
        swdioOutEnable(); // будем писать        
      }
      else  // отправка данных
      {       
        // задержка на обработку
        turnaroid(m_settings.turnaroid);        
        swdioOutEnable(); // будем писать
        uint32_t data = package->data;
        for(uint8_t n = 31; n + 1; n--) // 32 прохода
        {
          writeBit((data >> n) & 1);  // записываем по 1 биту, начиная со старшего
        }
        writeBit(package->dataParity); // записываем четность
      }      
      // тут должен быть пустой цикл swclk и еще какая-то лабуда
      idleCycles(m_settings.idleCycles);
      swdioOut(1);      
      __enable_irq();
      return;
    }
    
    if((package->ACK == SWD_TRANSFER_WAIT) || (package->ACK == SWD_TRANSFER_FAULT))
    {
      if(m_settings.dataPhase && (package->RnW))  // если режим чтения и видимо dataPhase значит, что все таки данные шлются и их надо пропустить
      {
        turnaroid(32+1);  // 32(data) + 1(parity)
      }
      turnaroid(m_settings.turnaroid);
      
      swdioOutEnable();
      if(m_settings.dataPhase && !(package->RnW))  // если режим записи
      {
        swdioOut(0);
        turnaroid(32+1);  // 32(data) + 1(parity)
      }
      swdioOut(1);
      __enable_irq();
      return;
    }    
    // тут ловится ошибка в протоколе, тип не поймал ни одного нормального ack
    
    turnaroid(m_settings.turnaroid + 32 + 1);
    swdioOutEnable();
    swdioOut(1);
    __enable_irq();
    return;    
  }
  
  inline void SwdHandle::turnaroid(uint8_t turnaroid)
  {
    for(;turnaroid; turnaroid--)  swclkCycle();     
  }
  
  inline void SwdHandle::idleCycles(uint8_t idleCycles)
  {
    if(idleCycles)
    {
      swdioOut(0);
      for(;idleCycles; idleCycles--)  swclkCycle();
    }
  }    
  
  inline void SwdHandle::pinDelay(uint32_t delay)
  {
    volatile uint32_t count;
    count = delay;
    while(--count);
  }    
}
