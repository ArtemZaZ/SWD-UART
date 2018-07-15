#include "project_config.h"
#include "uart_handle.h" 
#include "DAP_config.h"
#include "swd_handle.h"

int main(void)
{
  //uart::Handle uart1;
  swd::SwdHandle swdHandle;
  swd::SwdPackage package;
  
  package = swdHandle.pack(1,1,2,1,1,100);
  
  uart::uart1.init(uart::Pins::UART1_PA9_PA10, 9600);
  uart::uart1.sendStaticArray("assassaassdax", 13);
	while(1);
  
}
