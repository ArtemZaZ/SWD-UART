#include "project_config.h"
#include "uart_handle.h" 
#include "swd_handle.h"
#include "uart_swd_adapter.h"
#include "repeater.h"

int main(void)
{
  static swd::SwdHandle swdHandle;
  swdHandle.init(GPIOA, 3, GPIOA, 4, GPIOA, 5);
  uart::uart1.init(uart::Pins::UART1_PA9_PA10, 9600);
  
  static adapter::UartSwdAdapter uartSwdAdapter(&swdHandle, &(uart::uart1));
  
  static repeater::Repeater swdRepeater;
  swdRepeater.appendAdapter(&uartSwdAdapter);
  
  
  
	while(1)
  {
    swdRepeater.fsm();
  }
  
}
