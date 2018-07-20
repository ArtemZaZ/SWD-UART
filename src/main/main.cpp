#include "project_config.h"
#include "uart_handle.h" 
#include "swd_handle.h"
#include "uart_swd_adapter.h"
#include "repeater.h"

/* P.S архитектура разрешает делать с SWD, что угодно, однако, для одного SWD должен быть свой (один!) SwdHandler(если вы не сделаете свою версию на базе ISwdBus),
** но это не значит, что нельзя подвязывать к SWD несколько физ. интерфейсов и творить другую черную магию.
** можно к одному физ. интерфейсу привязать несколько SWD.
** Все зависит от созданного в будущем протокола между передающей частью программатора и принимающей.
** если нумеровать SWD, можно однозначно с ними по куче интерфейсов общаться.
*/
int main(void)
{
  // реализуешь свой SwdHandle(если нужно, но скорее всего нет), наследуемый от ISwsBus(интерфейс пишуший в swd)
  static swd::SwdHandle swdHandle;    
  swdHandle.init(GPIOA, 3, GPIOA, 4, GPIOA, 5); // SWDHandle полностью еще не закончен, пример в YOBA-DAP-Programmer
  
  uart::uart1.init(uart::Pins::UART1_PA9_PA10, 9600);   // инициализация физ интерфейса
  
  // реализуешь свой адаптер, для своего физ. интерфейса, наследуемый от базового адаптера BaseAdapter
  static adapter::UartSwdAdapter uartSwdAdapter(&swdHandle, &(uart::uart1));  
  
  static repeater::Repeater swdRepeater;  // ретранслятор
  swdRepeater.appendAdapter(&uartSwdAdapter);  // кидаешь в него все адаптеры, что есть, он сам все сделает
  
	while(1)
  {
    swdRepeater.fsm();  // крутишь конечный автомат ретранлятора
  }
  
}
