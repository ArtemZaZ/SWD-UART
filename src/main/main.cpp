#include "project_config.h"
#include "uart_handle.h" 
#include "swd_handle.h"
#include "uart_swd_adapter.h"

int main(void)
{
  //uart::Handle uart1;
  swd::SwdHandle swdHandle;
  //swd::SwdPackage package;
  swdHandle.init(GPIOA, 3, GPIOA, 4, GPIOA, 5);
  
  adapter::UartSwdAdapter uartAdapter(&swdHandle, &(uart::uart1));
  
  uart::uart1.init(uart::Pins::UART1_PA9_PA10, 9600);
  uart::uart1.sendStaticArray("assassaassdax", 13);
	while(1);
  
}
