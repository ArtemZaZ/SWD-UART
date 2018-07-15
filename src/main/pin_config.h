#ifndef SWD_PIN_CONFIG_H_
#define SWD_PIN_CONFIG_H_

/* определение портов и пинов SWD */
#define SWDIO_PORT	        GPIOB
#define SWDIO_PIN_NUMBER	  8   // не баг, а фича: так приходится определять для быстродействия, см. макросы в DAP_config.h
#define SWCLK_PORT	        GPIOA
#define SWCLK_PIN_NUMBER	  5
#define nRESET_PORT	        GPIOA
#define nRESET_PIN_NUMBER	  6

#define SWDIO_PIN   ((uint16_t)(1 << SWDIO_PIN_NUMBER))
#define SWCLK_PIN   ((uint16_t)(1 << SWCLK_PIN_NUMBER))
#define nRESET_PIN  ((uint16_t)(1 << nRESET_PIN_NUMBER))

#endif
