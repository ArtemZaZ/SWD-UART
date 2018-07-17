#include "project_config.h"
#include "uart_handle.h" 
#include "swd_handle.h"

int main(void)
{
  //uart::Handle uart1;
  swd::SwdHandle swdHandle;
  swd::SwdPackage package;
  swdHandle.init(GPIOA, 3, GPIOA, 4, GPIOA, 5);
  package = swdHandle.pack(0,1,0);
  swdHandle.transferPackage(&package);
  
  package = swdHandle.pack(0,1,0);
  swdHandle.transferPackage(&package);
  
  uart::uart1.init(uart::Pins::UART1_PA9_PA10, 9600);
  uart::uart1.sendStaticArray("assassaassdax", 13);
	while(1);
  
}
